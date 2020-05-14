#include "assembler.hpp"

static std::vector<std::string> registers
{
    "c0", "c1", "c2", "c3",
    "u0", "u1", "u2", "u3",
    "l0", "l1", "l2", "l3",
    "f0", "f1", "f2", "f3"
};

static std::vector<std::string> instructions
{
    "add", "sub", "mul", "div", "mod", "inc", "dec",
    "lshft", "rshft", "and", "or", "xor", "cmpl",
    "if", "else", "grt0", "grt1", "greq0", "greq1", "less0", "less1",
    "lseq0", "lseq1", "iseq0", "iseq1", "neq0", "neq1", "not0", "not1",
    "cand", "cor",
    "call", "jmp"
};

std::vector<std::string> unaryInstructions
{
    "inc", "dec", "cmpl", "not0", "not1", "cand", "cor",
};

std::vector<std::string> binaryInstructions
{
    "add", "sub", "mul", "div", "mod",
    "lshft", "rshft", "and", "or", "xor",
    "grt0",
    "grt1",
    "greq0",
    "greq1",
    "less0",
    "less1",
    "lseq0",
    "lseq1",
    "iseq0",
    "iseq1",
    "neq0",
    "neq1",
};

Falcon::Assembler::Lexer::Lexer(std::string __text)
    : text(__text), currentChar(__text[0]), cursor(0)
{
}

void Falcon::Assembler::Lexer::advance()
{
    if (this->cursor > this->text.size())
    {
        this->cursor        = this->text.size();
        this->currentChar   = '\0';
    }
    this->currentChar = this->text[++this->cursor];
}

void Falcon::Assembler::Lexer::makeNum(std::vector<Token> & tokens)
{
    std::string num;
    bool fraction = false;

    while (this->cursor < this->text.size() && (std::isdigit(this->currentChar) || this->currentChar == '.'))
    {
        if (!fraction && this->currentChar == '.')
        {
            fraction = true;
        }
        num.push_back(this->currentChar);
        this->advance();
    }

    this->cursor -= 2;
    this->advance();

    Token token;
    if (fraction)
    {
        token.type      = Token::FLOAT;
        token.value.f   = std::strtof(num.c_str(), NULL);
    }
    else
    {
        token.type      = Token::P_INT;
        token.value.u   = std::strtoul(num.c_str(), NULL, 10);
    }

    tokens.push_back(token);
}

std::string Falcon::Assembler::Lexer::makeStr()
{
    std::string word;

    while(this->cursor < this->text.size() && (std::isalpha(this->currentChar) == 0 || this->currentChar == '_' || std::isdigit(this->currentChar) == 0))
    {
        if (this->currentChar == ':' || this->currentChar == ' ' || this->currentChar == '\n' || this->currentChar == '\t')
        {
            break;
        }
        word.push_back(this->currentChar);
        this->advance();
    }

    this->cursor -= 2;
    this->advance();

    return word;
}

std::vector<Falcon::Assembler::Token> Falcon::Assembler::Lexer::process()
{
    std::vector<Token> tokens;

    while (this->cursor < this->text.size())
    {
        if (this->currentChar == ' ' || this->currentChar == '\t')
        {
            //Do nothing
        }
        else if (this->currentChar == '\n')
        {
            Token token;
            token.type = Token::ENDL;
            tokens.push_back(token);
        }
        else if (this->currentChar == '.')
        {
            this->advance();
            Token token;
            token.type = Token::FUNCTION;
            token.name = this->makeStr();
            tokens.push_back(token);
        }
        else if (this->currentChar == ':')
        {
            Token token;
            token.type = Token::COLON;
            tokens.push_back(token);
        }
        else if (std::isdigit(this->currentChar))
        {
            this->makeNum(tokens);
        }
        else if (std::isalpha(this->currentChar))
        {
            std::string str = this->makeStr();
            if (std::find(registers.begin(), registers.end(), str) != registers.end())
            {
                Token token;
                token.type = Token::REGISTER;
                token.name = str;
                tokens.push_back(token);
            }
            else if (std::find(instructions.begin(), instructions.end(), str) != instructions.end())
            {
                Token token;
                token.type = Token::INSTRUCTION;
                token.name = str;
                tokens.push_back(token);
            }
        }

        this->advance();
    }
    Token token;
    token.type = Token::END;
    tokens.push_back(token);

    return tokens;
}

Falcon::Assembler::ExprAST::~ExprAST()
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, uint64_t __uint)
    : type(__type), u(__uint)
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, int64_t __int)
    : type(__type), l(__int)
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, _Float64 __float)
    : type(__type), f(__float)
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, Token __reg, uint8_t __offset)
    : type(__type), reg(__reg), regOffset(__offset)
{
}

Falcon::Assembler::ExprStatement::ExprStatement(Token __instruction, bool __atom2Used, ExprAtom __atom0, ExprAtom __atom1)
    : instruction(__instruction), atom2Used(__atom2Used), atoms{__atom0, __atom1}
{
}

Falcon::Assembler::ExprFunction::ExprFunction(Token __identifier)
    : identifier(__identifier)
{
}

Falcon::Assembler::ExprExtern::ExprExtern(Token __identifier)
    : identifier(__identifier)
{
}

Falcon::Assembler::Parser::Parser(std::vector<Token> __tokens)
    : tokens(__tokens), currentToken(__tokens[0]), cursor(0)
{
}

void Falcon::Assembler::Parser::advance()
{
    if (this->cursor >= this->tokens.size() - 1)
    {
        this->cursor = this->tokens.size();
        Token token;
        this->currentToken = token;
        return;
    }
    this->currentToken = this->tokens[++this->cursor];
}

void Falcon::Assembler::Parser::atom(ExprAST ** ast)
{
    if (this->currentToken.type == Token::P_INT)
    {
        *ast = new ExprAtom(ExprAtom::UINT, this->currentToken.value.u);
    }
    else if (this->currentToken.type == Token::N_INT)
    {
        *ast = new ExprAtom(ExprAtom::INT, this->currentToken.value.l);
    }
    else if (this->currentToken.type == Token::FLOAT)
    {
        *ast = new ExprAtom(ExprAtom::FLOAT, this->currentToken.value.f);
    }
    else if (this->currentToken.type == Token::REGISTER)
    {
        *ast = new ExprAtom(ExprAtom::REGISTER, this->currentToken, 0);
    }

    this->advance();
}

void Falcon::Assembler::Parser::statement(ExprAST ** ast)
{
    if (std::find(binaryInstructions.begin(), binaryInstructions.end(), this->currentToken.name) != binaryInstructions.end())
    {
        Token token = this->currentToken;

        ExprAtom * atoms[] =
        {
            nullptr,
            nullptr
        };

        this->advance();
        this->atom((ExprAST **)&atoms[0]);
        this->atom((ExprAST **)&atoms[1]);

        *ast = new ExprStatement(token, true, *atoms[0], *atoms[1]);
    }
}

void Falcon::Assembler::Parser::function(ExprAST ** ast)
{
    *ast = new ExprFunction(this->currentToken);
    this->advance();
    this->advance();
    this->advance();
    while (this->cursor < this->tokens.size() && this->currentToken.type != Token::END)
    {
        if (this->currentToken.type == Token::INSTRUCTION)
        {
            ExprStatement * statement = nullptr;
            this->statement((ExprAST **)&statement);
            ((ExprFunction *)*ast)->statements.push_back(*statement);
        }
        this->advance();
    }
}

Falcon::Assembler::ExprAST * Falcon::Assembler::Parser::process()
{
    ExprAST * ast = nullptr;

    while (this->cursor < this->tokens.size())
    {
        if (this->currentToken.type == Token::FUNCTION)
        {
            this->function(&ast);
        }
        this->advance();
    }

    return ast;
}
