#include <fstream>
#include <iostream>
#include <iterator>

#include "../SemanticAnalyzer.hpp"
#include "../Combiner.hpp"
#include "../Generator.hpp"
#include "../Serialize.hpp"

static struct
{
    bool Debug = false;

    #ifdef DEBUG
        bool LogTokens = false, LogAST = false;
    #endif

    std::vector<std::string> InputNames{"a.fasm"};
    std::string TargetName = "a.fali";
} s_State;

void printHelp()
{
    std::cout<<"Usage: fasm [COMMAND_LINE_OPTIONS] [FILE(s)]\n\n";
    std::cout<<"COMMAND_LINE_OPTIONS:\n";
    std::cout<<"    -h or --help                  : Print this help and exit.\n";
    std::cout<<"    -o [FILE] or --output=[FILE]  : Set the output file as FILE. Default is a.fali.\n";
    std::cout<<"    -dbg or --debug               : Write debug data to the output.\n";
    std::cout<<"FILE(s):\n";
    std::cout<<"    Add FILE to input files. No input file sets input file as a.fasm.\n";
}

void parseCmdArgs(int argc, char * argv[])
{
    if (argc == 1)
    {
        printHelp();
        exit(2);
    }

    for (int i = 1; i < argc; i++)
    {
        std::string str(argv[i]);

        if (str == "-h" || str == "--help")
        {
            printHelp();
            exit(2);
        }
        else if (str == "-dbg" || str == "--debug")
        {
            s_State.Debug = true;
        }
        #ifdef DEBUG
            else if (str == "-ltk" || str == "--log-tokens")
            {
                s_State.LogTokens = true;
            }
            else if (str == "-last" || str == "--log-ast")
            {
                s_State.LogAST = true;
            }
        #endif
        else if (str == "-o")
        {
            if (i == argc - 1)
            {
                std::cout<<"Command line option `-o` should have filename after it.\n";
                printHelp();
                exit(2);
            }

            s_State.TargetName = argv[++i];
        }
        else if (str.find("--out=") != std::string::npos)
        {
            if (str.size() == 6)
            {
                std::cout<<"Command line option `--out=` should have filename after it.\n";
                printHelp();
                exit(2);
            }

            s_State.TargetName = str.substr(6);
        }
        else
        {
            if (str[0] == '-')
            {
                std::cout<<"Invalid command line option `"<<str<<"`.\n";
                printHelp();
                exit(2);
            }

            if (s_State.InputNames[0] == "a.fasm")
            {
                s_State.InputNames.erase(s_State.InputNames.begin());
            }

            s_State.InputNames.emplace_back(str);
        }
    }
}

int main(int argc, char * argv[])
{
    parseCmdArgs(argc, argv);
    
    std::vector<Falcon::Assembler::ASTNode *> asts;

    for (auto filename : s_State.InputNames)
    {
        std::ifstream in(filename);

        if (!in)
        {
            std::cout<<"Input file error in `"<<filename<<"`: "<<strerror(errno)<<"\n";
            exit(2);
        }

        in>>std::noskipws;

        std::istream_iterator<char> eos;

        std::istream_iterator<char> it(in);

        std::string inStr(it, eos);

        in.close();

        #ifdef DEBUG
            if (s_State.LogTokens)
            {
                Falcon::Assembler::Lexer lexer(inStr);

                Falcon::Assembler::Token token((Falcon::Assembler::TokenType)'\0');

                while ((token = lexer.lex()).Type != (Falcon::Assembler::TokenType)'\0')
                {
                    Falcon::Assembler::Serialize(token);
                }
            }
        #endif

        Falcon::Assembler::Lexer lexer(inStr);

        Falcon::Assembler::Parser parser(std::bind(&Falcon::Assembler::Lexer::lex, &lexer), std::bind(&Falcon::Assembler::Lexer::peek, &lexer));

        asts.emplace_back(parser.parse());
    }

    Falcon::Assembler::Combiner combiner(asts);

    #ifdef DEBUG
        if (s_State.LogAST)
        {
            Falcon::Assembler::Combiner combiner(asts);

            Falcon::Assembler::Serialize(combiner.combine());
        }
    #endif

    Falcon::Assembler::SemanticAnalyzer semanticAnalyzer(combiner.combine());

    Falcon::Assembler::Generator generator(semanticAnalyzer.analyze(), s_State.Debug);

    std::ofstream out(s_State.TargetName);

    out<<generator.generate();

    out.close();
}
