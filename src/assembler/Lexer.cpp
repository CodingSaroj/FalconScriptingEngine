#include "Lexer.hpp"

namespace Falcon
{
    namespace Assembler
    {
        template<typename T>
        constexpr bool FindInVector(const std::vector<T> & vec, const T & data) { return std::find(vec.begin(), vec.end(), data) != vec.end(); }

        Lexer::Lexer(const std::string & text)
            : m_Cursor(0)
        {
            std::vector<std::string> lines;

            std::string current;

            for (char c : text)
            {
                if (c == '\n')
                {
                    lines.emplace_back(current);
                    current.clear();
                }
                else
                {
                    current += c;
                }
            }

            std::vector<std::pair<std::string, std::string>> regexes
            {
                {R"(;.*)"                                   , "comment"},
                {R"(0b[01]+)"                               , "binary"},
                {R"(0[0-3][0-7][0-7])"                      , "octal"},
                {R"(0)"                                     , "octal zero"},
                {R"([1-9][0-9]*)"                           , "decimal"},
                {R"(0x[0-9a-fA-F]+)"                        , "hexadecimal"},
                {R"([\+-][0-9]+\.[0-9]+)"                   , "float"},
                {R"([\+-][0-9]+\.[0-9]+e[\+-][0-9]+)"       , "float exponent"},
                {R"('.')"                                   , "char"},
                {R"("[^"]*")"                               , "string"},
                {R"([a-zA-Z_@#$%][a-zA-Z0-9_@#$%]*)"        , "identifier"},
                {R"([(){}.,:])"                             , "arbitarory"}
            };

            std::string finalRegexStr;

            for (auto & regex : regexes)
            {
                finalRegexStr += "(" + regex.first + ")|";
            }

            finalRegexStr.pop_back();

            std::regex finalRegex;

            try
            {
                finalRegex = std::regex(finalRegexStr, std::regex_constants::extended);
            }
            catch (std::regex_error err)
            {
                FLCN_ASSERT(false, "Assembler::Lexer", "Regex Error: {}", err.what());
            }

            for (size_t i = 1; i <= lines.size(); i++)
            {
                auto begin = std::sregex_iterator(lines[i - 1].begin(), lines[i - 1].end(), finalRegex);
                auto end = std::sregex_iterator();
                
                for (auto iter = begin; iter != end; ++iter)
                {
                    size_t matchIndex = 0;

                    for (; matchIndex < iter->size(); matchIndex++)
                    {
                        if (!iter->str(matchIndex + 1).empty())
                        {
                            break;
                        }
                    }

                    std::string match = iter->str(matchIndex + 1);
                    std::string matchType = regexes[matchIndex].second;

                    if (matchType == "binary")
                    {
                        m_Tokens.emplace_back(i, strtoul(match.c_str() + 2, nullptr, 2));
                    }
                    else if (matchType == "octal")
                    {
                        m_Tokens.emplace_back(i, strtoul(match.c_str() + 1, nullptr, 8));
                    }
                    else if (matchType == "octal zero")
                    {
                        m_Tokens.emplace_back(i, (uint64_t)0);
                    }
                    else if (matchType == "decimal")
                    {
                        m_Tokens.emplace_back(i, strtol(match.c_str(), nullptr, 10));
                    }
                    else if (matchType == "hexadecimal")
                    {
                        m_Tokens.emplace_back(i, strtoul(match.c_str(), nullptr, 16));
                    }
                    else if (matchType == "float")
                    {
                        m_Tokens.emplace_back(i, strtod(match.c_str(), nullptr));
                    }
                    else if (matchType == "float exponent")
                    {
                        m_Tokens.emplace_back(i, strtod(match.c_str(), nullptr));
                    }
                    else if (matchType == "char")
                    {
                        m_Tokens.emplace_back(i, match[1]);
                    }
                    else if (matchType == "string")
                    {
                        m_Tokens.emplace_back(i, match.substr(1, match.size() - 2));
                    }
                    else if (matchType == "identifier")
                    {
                        auto toUpper =  [](std::string str)->std::string
                        {
                            for (char & c : str) { c = std::toupper(c); }

                            return str;
                        };

                        if (FindInVector(RegisterType::s_Names, toUpper(match)))
                        {
                            m_Tokens.emplace_back(i, Token::RegisterType{toUpper(match)});
                        }
                        else if (match[0] == '@')
                        {
                            if (FindInVector(RegisterType::s_Names, toUpper(match.substr(1))))
                            {
                                m_Tokens.emplace_back(i, Token::RegisterType{toUpper(match)});
                            }
                        }
                        else if (match[0] == '[' && match.back() == ']')
                        {
                            if (FindInVector(RegisterType::s_Names, toUpper(match.substr(1, match.size() - 2))))
                            {
                                m_Tokens.emplace_back(i, Token::RegisterType{toUpper(match)});
                            }
                        }
                        else if (FindInVector(OpCode::s_Names, toUpper(match)))
                        {
                            m_Tokens.emplace_back(i, Token::InstructionType{toUpper(match)});
                        }
                        else if (toUpper(match) == "SECT")
                        {
                            m_Tokens.emplace_back(i, Token::SectionType{});
                        }
                        else if (FindInVector<std::string>({"META", "MAP", "LOCAL", "ATTRIBUTE", "FUNCTION", "STRUCT", "ALIAS"}, toUpper(match)))
                        {
                            m_Tokens.emplace_back(i, toUpper(match));
                        }
                        else
                        {
                            m_Tokens.emplace_back(i, match);
                        }
                    }
                    else if (matchType == "arbitarory")
                    {
                        m_Tokens.emplace_back(i, Token::ArbitaroryType{match[0]});
                    }
                }
                
                m_Tokens.emplace_back(i, Token::NewLineType{});
            }
        }

        Token Lexer::Lex()
        {            
            if (m_Cursor == m_Tokens.size())
            {
                return Token();
            }
            else
            {
                return m_Tokens[m_Cursor++];
            }
        }

        Token Lexer::Peek()
        {
            if (m_Cursor == m_Tokens.size())
            {
                return Token();
            }
            else
            {
                return m_Tokens[m_Cursor];
            }
        }
    }
}
