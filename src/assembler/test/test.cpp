#include <fstream>
#include <sstream>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Serialize.hpp"

using namespace Falcon::Assembler;

int main()
{
    std::ifstream in("hello.fasm");
    std::stringstream ss;
    ss<<in.rdbuf();
    in.close();

    Lexer lexer(ss.str());
    Parser parser(std::bind(&Lexer::lex, &lexer), std::bind(&Lexer::peek, &lexer));

    while (ASTNode * node = parser.parse())
    {
        Serialize(node);
    }
}
