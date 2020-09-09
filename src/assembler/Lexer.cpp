#include "Lexer.hpp"

namespace Falcon
{
    namespace Assembler
    {
        static const std::string identChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_@#$%");
        static const std::string operators("@[]");
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

        Token Lexer::lexNumber(bool sign)
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

            if (isFloat) { return Token(TokenType::FLOAT, (double)std::strtod(number.c_str(), NULL)); }

            if (base == OCT) { return Token(TokenType::UINT, (uint64_t)std::strtoul(number.c_str(), NULL, 8)); }
            
            else if (base == DEC) { return sign ? Token(TokenType::INT, (int64_t)std::strtol(number.c_str(), NULL, 10)) : Token(TokenType::UINT, (uint64_t)std::strtoul(number.c_str(), NULL, 10)); }
            
            else { return Token(TokenType::UINT, (uint64_t)std::strtoul(number.c_str(), NULL, 16)); }
        }

        Token Lexer::lexChar()
        {
            char value = this->advance();

            if (this->advance() != '\'')
            {
                Log(LogLevel::ERR, "Expected a `'`.");
                exit(2);
            }

            return Token(TokenType::CHAR, value);
        }

        Token Lexer::lexStr()
        {
            std::string str;

            if (m_CurrentChar == '\"')
            {
                this->advance();

                while (m_CurrentChar != '\0' && m_CurrentChar != '\"')
                {
                    str += m_CurrentChar;

                    this->advance();
                }

                this->advance();
            }
            else
            {
                while (m_CurrentChar != '\0' && whiteSpaces.find(m_CurrentChar) == std::string::npos)
                {
                    //If m_CurrentChar isn't a valid identifier character, operator or a digit.
                    if (identChars.find(m_CurrentChar) == std::string::npos && operators.find(m_CurrentChar) == std::string::npos && decDigits.find(m_CurrentChar) == std::string::npos)
                    {
                        break;
                    }
                
                    str += m_CurrentChar;

                    this->advance();
                }
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

            if (findInVector(RegisterType::s_Names, toUpper(str)))
            {
                return Token(TokenType::REGISTER, toUpper(str));
            }
            else if (str[0] == '@')
            {
                if (findInVector(RegisterType::s_Names, toUpper(str.substr(1))))
                {
                    return Token(TokenType::REGISTER, toUpper(str));
                }
            }
            else if (str[0] == '[' && str[str.size() - 1] == ']')
            {
                if (findInVector(RegisterType::s_Names, toUpper(str.substr(1, str.size() - 2))))
                {
                    return Token(TokenType::REGISTER, toUpper(str));
                }
            }
            else if (findInVector(OpCode::s_Names, toUpper(str)))
            {
                return Token(TokenType::INSTRUCTION, toUpper(str));
            }
            else if (toUpper(str) == "SECT")
            {
                return Token(TokenType::SECTION);
            }
            else if (toUpper(str) == "META")
            {
                return Token(TokenType::IDENTIFIER, toUpper(str));
            }
            else if (toUpper(str) == "MAP")
            {
                return Token(TokenType::IDENTIFIER, toUpper(str));
            }
            else if (toUpper(str) == "LOCAL")
            {
                return Token(TokenType::IDENTIFIER, toUpper(str));
            }
            else if (toUpper(str) == "ATTRIBUTE")
            {
                return Token(TokenType::IDENTIFIER, toUpper(str));
            }
            else if (toUpper(str) == "FUNCTION")
            {
                return Token(TokenType::IDENTIFIER, toUpper(str));
            }
            else if (toUpper(str) == "STRUCT")
            {
                return Token(TokenType::IDENTIFIER, toUpper(str));
            }
            else if (toUpper(str) == "ALIAS")
            {
                return Token(TokenType::IDENTIFIER, toUpper(str));
            }
            
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
                token = this->lexNumber(true);
            }
            else if (decDigits.find(m_CurrentChar) != std::string::npos)
            {
                token = this->lexNumber();
            }
            else if (m_CurrentChar == '\"' || identChars.find(m_CurrentChar) != std::string::npos || operators.find(m_CurrentChar) != std::string::npos)
            {
                token = this->lexStr();
            }
            else if (m_CurrentChar == '\'')
            {
                token = this->lexChar();
            }
            else if (m_CurrentChar == ';')
            {
                while (m_CurrentChar != '\n')
                {
                    this->advance();
                }

                token = Token(TokenType::NEWLINE);
                Context::Line++;
                Context::Character = 0;
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
            //Backup cursor, line and character
            uint64_t tmp = m_Cursor;
            uint64_t line = Context::Line;
            uint64_t character = Context::Character;

            //Get the Token
            Token token = lex();

            //Restore the cursor, line and character
            m_Cursor = tmp - 1;
            Context::Line = line;
            Context::Character = character;
            this->advance();

            return token;
        }
    }
}
