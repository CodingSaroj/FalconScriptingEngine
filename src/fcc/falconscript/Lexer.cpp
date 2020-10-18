#include "Lexer.hpp"

namespace Falcon
{
    namespace FCC
    {
        namespace FalconScript
        {
            Lexer::Lexer(const std::string & text)
                : m_Text(text)
            {
            }

            std::vector<Token> Lexer::Lex()
            {
                std::vector<Token> tokens;

                std::vector<std::string> lines;

                std::string current;

                for (char c : m_Text)
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
                    {R"([\+-]?[0-9]+\.[0-9]+)"          , "real"},
                    {R"([\+-][0-9]+)"                   , "int"},
                    {R"(0)"                             , "whole_zero"},
                    {R"(0x[0-9a-fA-F]+)"                , "whole_hex"},
                    {R"([1-9][0-9]*)"                   , "whole_dec"},
                    {R"(0[0-7][0-7][0-7])"              , "whole_oct"},
                    {R"(0b[01]+)"                       , "whole_bin"},
                    {R"('.')"                           , "char"},
                    {R"([A-Za-z_][A-Za-z0-9_]*)"        , "word"},
                    {R"([\(\)\{\},:;])"                 , "punctuator"},
                    {R"([\+*/%&|^~><=!.-][\+=&|-]?)"    , "operator"},
                    {R"(//.+//)"                        , "comment"}
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
                    finalRegex = std::move(std::regex(finalRegexStr, std::regex_constants::extended));
                }
                catch (std::regex_error & err)
                {
                    FLCN_ASSERT(false, "Lexer", "Regex error: {}", err.what());
                }

                for (size_t i = 0; i < lines.size(); i++)
                {
                    auto begin = std::sregex_iterator(lines[i].begin(), lines[i].end(), finalRegex);
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

                        std::string & type = regexes[matchIndex].second;

                        if (type == "real")
                        {
                            tokens.emplace_back(i + 1, strtod(iter->str(matchIndex + 1).c_str(), nullptr));
                        }
                        else if (type == "int")
                        {
                            tokens.emplace_back(i + 1, strtol(iter->str(matchIndex + 1).c_str(), nullptr, 10));
                        }
                        else if (type == "whole_zero")
                        {
                            tokens.emplace_back(i + 1, (uint64_t)0);
                        }
                        else if (type == "whole_hex")
                        {
                            tokens.emplace_back(i + 1, strtoul(iter->str(matchIndex + 1).c_str() + 2, nullptr, 16));
                        }
                        else if (type == "whole_dec")
                        {
                            tokens.emplace_back(i + 1, strtoul(iter->str(matchIndex + 1).c_str(), nullptr, 10));
                        }
                        else if (type == "whole_oct")
                        {
                            tokens.emplace_back(i + 1, strtoul(iter->str(matchIndex + 1).c_str() + 1, nullptr, 8));
                        }
                        else if (type == "whole_bin")
                        {
                            tokens.emplace_back(i + 1, strtoul(iter->str(matchIndex + 1).c_str() + 2, nullptr, 2));
                        }
                        else if (type == "char")
                        {
                            tokens.emplace_back(i + 1, iter->str(matchIndex + 1)[1]);
                        }
                        else if (type == "word")
                        {
                            tokens.emplace_back(i + 1, WordType{iter->str(matchIndex + 1)});
                        }
                        else if (type == "operator")
                        {
                            tokens.emplace_back(i + 1, OperatorType{iter->str(matchIndex + 1)});
                        }
                        else if (type == "punctuator")
                        {
                            tokens.emplace_back(i + 1, PunctuatorType{iter->str(matchIndex + 1)[0]});
                        }
                    }
                }

                tokens.emplace_back(Token());

                return std::move(tokens);
            }
        }
    }
}
