#include "../vm/OpCode.hpp"
#include "../vm/Register.hpp"

#include "Context.hpp"
#include "Falcon/src/assembler/TokenTypes.hpp"
#include "Lexer.hpp"
#include "LogLevel.hpp"
#include <cctype>

namespace Falcon
{
    namespace Assembler
    {
        static const std::string identChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_@$");
        static const std::string whiteSpaces(" \n\t");
        static const std::string octDigits("01234567");
        static const std::string decDigits("0123456789");
        static const std::string hexDigits("0123456789abcdefABCDEF");
       
        template<typename T>
        constexpr bool findInVector(std::vector<T> & vec, const T & data) { return std::find(vec.begin(), vec.end(), data) != vec.end(); }

        Lexer::Lexer(const std::string & text)
            : m_Text(text), m_Cursor(0), m_CurrentChar(text[0])
        {
        }

        Token Lexer::processNumber(bool sign)
        {
            std::string number;
            bool isFloat                = false;
            enum { OCT, DEC, HEX } base = DEC;

            if (sign) { number += '-'; }

            if (m_CurrentChar == '0')
            {
                if (this->advance() == 'x' || this->advance() == 'X') { base = HEX; }
                else { base = OCT; }

                this->advance();
            }

            while (m_CurrentChar != '\0' && whiteSpaces.find(m_CurrentChar) == std::string::npos)
            {
                if (m_CurrentChar == '.')
                {
                    isFloat = true;
                }
                else if (base == OCT)
                {
                    if (octDigits.find(m_CurrentChar) == std::string::npos) { break; }
                }
                else if (base == DEC)
                {
                    if (decDigits.find(m_CurrentChar) == std::string::npos) { break; }
                }
                else if (base == HEX)
                {
                    if (hexDigits.find(m_CurrentChar) == std::string::npos) { break; }
                }

                number += m_CurrentChar;

                this->advance();
            }

            if (m_CurrentChar)
            {
                m_Cursor -= 2;
                this->advance();
            }

            if (isFloat) { return Token(TokenType::FLOAT, std::strtod(number.c_str(), NULL)); }

            if (base == OCT) { return Token(TokenType::UINT, std::strtoul(number.c_str(), NULL, 8)); }
            
            else if (base == DEC) { return Token(sign ? TokenType::INT : TokenType::UINT, std::strtol(number.c_str(), NULL, 10)); }
            
            else { return Token(TokenType::UINT, std::strtoul(number.c_str(), NULL, 16)); }
        }

        Token Lexer::processChar()
        {
            char value = this->advance();

            if (this->advance() != '\'')
            {
                Log(LogLevel::ERR, "Expected a `'`.");
                exit(2);
            }

            return Token(TokenType::CHAR, value);
        }

        Token Lexer::processStr()
        {
            std::string str;

            while (m_CurrentChar != '\0' && whiteSpaces.find(m_CurrentChar) == std::string::npos)
            {
                //If m_CurrentChar isn't a valid identifier character or a digit.
                if (identChars.find(m_CurrentChar) == std::string::npos && decDigits.find(m_CurrentChar) == std::string::npos) { break; }
                
                str += m_CurrentChar;

                this->advance();
            }

            //If this isn't the end.
            if (m_CurrentChar)
            {
                //Step back.
                m_Cursor -= 2;
                this->advance();
            }

            auto toUpper =  [](std::string str)->std::string
                            {
                                for (char & c : str) { c = std::toupper(c); }

                                return str;
                            };

            if (findInVector(RegisterType::names, toUpper(str))) { return Token(TokenType::REGISTER, toUpper(str)); }

            else if (findInVector(OpCode::names, toUpper(str))) { return Token(TokenType::INSTRUCTION, toUpper(str)); }

            else if (str == "sect") { return Token(TokenType::SECTION); }

            return Token(TokenType::IDENTIFIER, str);
        }

        Token Lexer::lex()
        {
            Token token((TokenType)0);

            bool newline = false;

            //Checkpoint for skipping Tokens.
            CheckToken:

            if (m_Text[m_Cursor] == '\0')
            {
            }
            else if (whiteSpaces.find(m_CurrentChar) != std::string::npos)
            {
                if (m_CurrentChar == '\n')
                {
                    token = Token(TokenType::NEWLINE);
                    newline = true;
                    Context::Line++;
                    Context::Character = 0;
                }
                else
                {
                    //Skip the whitespace.
                    this->advance();
                    goto CheckToken;
                }
            }
            else if (m_CurrentChar == '-' && decDigits.find(m_Text[m_Cursor + 1]) != std::string::npos)
            {
                //Consume the '-' sign.
                this->advance();

                //Retrieve the negative number Token.
                token = this->processNumber(true);
            }
            else if (decDigits.find(m_CurrentChar) != std::string::npos)
            {
                token = this->processNumber();
            }
            else if (identChars.find(m_CurrentChar) != std::string::npos)
            {
                token = this->processStr();
            }
            else if (m_CurrentChar == '\'')
            {
                token = this->processChar();
            }
            else
            {
                //Return arbitrary token.
                token = Token((TokenType)m_CurrentChar);
            }

            this->advance();

            return token;
        }

        Token Lexer::peek()
        {
            //Backup cursor
            uint64_t tmp = m_Cursor;

            //Get the Token
            Token token = lex();

            //Restore the cursor
            m_Cursor = tmp - 1;
            this->advance();

            return token;
        }
    }
}
