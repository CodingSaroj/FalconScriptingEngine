#include "assembler.hpp"

static std::vector<std::string> registers
{
    "c0", "c1", "c2", "c3",
    "u0", "u1", "u2", "u3",
    "l0", "l1", "l2", "l3",
    "f0", "f1", "f2", "f3",
    "ptr", "psp",
    "csp", "usp", "lsp", "fsp",
    "null"
};

static std::vector<std::string> instructions
{
    "add", "sub", "mul", "div", "mod", "inc", "dec",
    "lshft", "rshft", "and", "or", "xor", "cmpl",
    "if", "else", "grt0", "grt1", "greq0", "greq1", "less0", "less1",
    "lseq0", "lseq1", "iseq0", "iseq1", "neq0", "neq1", "not0", "not1",
    "cand", "cor",
    "push", "pop",
    "alloc", "free",
    "ref", "deref",
    "mov", "movr",
    "cast", "raise",
    "call", "jmp"
};

std::vector<std::string> unaryInstructions
{
    "inc", "dec", "cmpl", "not0", "not1", "cand", "cor", "push", "pop", "raise"
};

std::vector<std::string> binaryInstructions
{
    "add", "sub", "mul", "div", "mod",
    "lshft", "rshft", "and", "or", "xor",
    "grt0", "grt1", "greq0", "greq1", "less0", "less1",
    "lseq0", "lseq1", "iseq0", "iseq1", "neq0", "neq1",
    "alloc", "free",
    "ref", "deref",
    "mov", "movr",
    "cast"
};

std::unordered_map<std::string, Falcon::InstructionType> instructionMap
{
    {"add"      ,   Falcon::INSTRUCTION_ADD},
    {"sub"      ,   Falcon::INSTRUCTION_SUB},
    {"mul"      ,   Falcon::INSTRUCTION_MUL},
    {"div"      ,   Falcon::INSTRUCTION_DIV},
    {"mod"      ,   Falcon::INSTRUCTION_MOD},
    {"inc"      ,   Falcon::INSTRUCTION_INC},
    {"dec"      ,   Falcon::INSTRUCTION_DEC},
    {"lshft"    ,   Falcon::INSTRUCTION_LSHFT},
    {"rshft"    ,   Falcon::INSTRUCTION_RSHFT},
    {"and"      ,   Falcon::INSTRUCTION_AND},
    {"or"       ,   Falcon::INSTRUCTION_OR},
    {"xor"      ,   Falcon::INSTRUCTION_XOR},
    {"cmpl"     ,   Falcon::INSTRUCTION_CMPL},
    {"if"       ,   Falcon::INSTRUCTION_IF},
    {"else"     ,   Falcon::INSTRUCTION_ELSE},
    {"grt9"     ,   Falcon::INSTRUCTION_GRT0},
    {"grt1"     ,   Falcon::INSTRUCTION_GRT1},
    {"greq0"    ,   Falcon::INSTRUCTION_GREQ0},
    {"greq1"    ,   Falcon::INSTRUCTION_GREQ1},
    {"less0"    ,   Falcon::INSTRUCTION_LESS0},
    {"less1"    ,   Falcon::INSTRUCTION_LESS1},
    {"lseq0"    ,   Falcon::INSTRUCTION_LSEQ0},
    {"lseq1"    ,   Falcon::INSTRUCTION_LSEQ1},
    {"iseq0"    ,   Falcon::INSTRUCTION_ISEQ0},
    {"iseq1"    ,   Falcon::INSTRUCTION_ISEQ1},
    {"neq0"     ,   Falcon::INSTRUCTION_NEQ0},
    {"neq1"     ,   Falcon::INSTRUCTION_NEQ1},
    {"not0"     ,   Falcon::INSTRUCTION_NOT0},
    {"not1"     ,   Falcon::INSTRUCTION_NOT1},
    {"push"     ,   Falcon::INSTRUCTION_PUSH},
    {"pop"      ,   Falcon::INSTRUCTION_POP},
    {"alloc"    ,   Falcon::INSTRUCTION_ALLOC},
    {"free"     ,   Falcon::INSTRUCTION_FREE},
    {"ref"      ,   Falcon::INSTRUCTION_REF},
    {"deref"    ,   Falcon::INSTRUCTION_DEREF},
    {"mov"      ,   Falcon::INSTRUCTION_MOV},
    {"movr"     ,   Falcon::INSTRUCTION_MOVR},
    {"cast"     ,   Falcon::INSTRUCTION_CAST},
    {"call"     ,   Falcon::INSTRUCTION_CALL},
    {"jmp"      ,   Falcon::INSTRUCTION_JMP},
    {"raise"    ,   Falcon::INSTRUCTION_RAISE}
};

std::unordered_map<std::string, uint8_t> semanticInstructions   /* 0b 1100 1100
                                                                 *    Arg0 Arg1
                                                                 *
                                                                 * 0        0           0               0
                                                                 * Int bit  Float bit   Pointer bit     Null bit
                                                                 *
                                                                 * Special cases:
                                                                 *     Arg1 = 0000 = unary
                                                                 *     Arg = 0001 = temporary value (32, 32.01 etc.)
                                                                 *
                                                                 *     instruction = 0b00000000 = function
                                                                 */
{
    {"add"      ,   0b11001100},
    {"sub"      ,   0b11001100},
    {"mul"      ,   0b11001100},
    {"div"      ,   0b11001100},
    {"mod"      ,   0b11001100},
    {"inc"      ,   0b11000000},
    {"dec"      ,   0b11000000},
    {"lshft"    ,   0b10001000},
    {"rshft"    ,   0b10001000},
    {"and"      ,   0b10001000},
    {"or"       ,   0b10001000},
    {"xor"      ,   0b10001000},
    {"cmpl"     ,   0b10000000},
    {"if"       ,   0b00000000},
    {"else"     ,   0b00000000},
    {"grt0"     ,   0b11101110},
    {"grt1"     ,   0b11101110},
    {"greq0"    ,   0b11101110},
    {"greq1"    ,   0b11101110},
    {"less0"    ,   0b11101110},
    {"less1"    ,   0b11101110},
    {"lseq0"    ,   0b11101110},
    {"lseq1"    ,   0b11101110},
    {"iseq0"    ,   0b11101110},
    {"iseq1"    ,   0b11101110},
    {"neq0"     ,   0b11101110},
    {"neq1"     ,   0b11101110},
    {"not0"     ,   0b11100000},
    {"not1"     ,   0b11100000},
    {"push"     ,   0b11100000},
    {"pop"      ,   0b11110000},
    {"alloc"    ,   0b00101000},
    {"free"     ,   0b00101000},
    {"ref"      ,   0b00101110},
    {"deref"    ,   0b11100010},
    {"mov"      ,   0b11100001},
    {"movr"     ,   0b11101110},
    {"cast"     ,   0b11101110},
    {"call"     ,   0b00000000},
    {"jmp"      ,   0b00010000},
    {"raise"    ,   0b10000000}
};

static inline Falcon::RegisterType getRegisterType(std::string type)
{
    
    if (type == "c0")
    {
        return Falcon::REGISTER_C0;
    }
    else if (type == "c1")
    {
        return Falcon::REGISTER_C1;
    }
    else if (type == "c2")
    {
        return Falcon::REGISTER_C2;
    }
    else if (type == "c3")
    {
        return Falcon::REGISTER_C3;
    }
    else if (type == "u0")
    {
        return Falcon::REGISTER_U0;
    }
    else if (type == "u1")
    {
        return Falcon::REGISTER_U1;
    }
    else if (type == "u2")
    {
        return Falcon::REGISTER_U2;
    }
    else if (type == "u3")
    {
        return Falcon::REGISTER_U3;
    }
    else if (type == "l0")
    {
        return Falcon::REGISTER_L0;
    }
    else if (type == "l1")
    {
        return Falcon::REGISTER_L1;
    }
    else if (type == "l2")
    {
        return Falcon::REGISTER_L2;
    }
    else if (type == "l3")
    {
        return Falcon::REGISTER_L3;
    }
    else if (type == "f0")
    {
        return Falcon::REGISTER_F0;
    }
    else if (type == "f1")
    {
        return Falcon::REGISTER_F1;
    }
    else if (type == "f2")
    {
        return Falcon::REGISTER_F2;
    }
    else if (type == "f3")
    {
        return Falcon::REGISTER_F3;
    }
    else if (type == "ptr")
    {
        return Falcon::REGISTER_PTR;
    }
    else if (type == "csp")
    {
        return Falcon::REGISTER_CSP;
    }
    else if (type == "usp")
    {
        return Falcon::REGISTER_USP;
    }
    else if (type == "lsp")
    {
        return Falcon::REGISTER_LSP;
    }
    else if (type == "fsp")
    {
        return Falcon::REGISTER_FSP;
    }
    else if (type == "psp")
    {
        return Falcon::REGISTER_PSP;
    }
    else if (type == "null")
    {
        return Falcon::REGISTER_NULL;
    }
}

static inline uint8_t getSemanticRegister(std::string type)
{
    
    if (type == "c0")
    {
        return 0b00001000;
    }
    else if (type == "c1")
    {
        return 0b00001000;
    }
    else if (type == "c2")
    {
        return 0b00001000;
    }
    else if (type == "c3")
    {
        return 0b00001000;
    }
    else if (type == "u0")
    {
        return 0b00001000;
    }
    else if (type == "u1")
    {
        return 0b00001000;
    }
    else if (type == "u2")
    {
        return 0b00001000;
    }
    else if (type == "u3")
    {
        return 0b00001000;
    }
    else if (type == "l0")
    {
        return 0b00001000;
    }
    else if (type == "l1")
    {
        return 0b00001000;
    }
    else if (type == "l2")
    {
        return 0b00001000;
    }
    else if (type == "l3")
    {
        return 0b00001000;
    }
    else if (type == "f0")
    {
        return 0b00000100;
    }
    else if (type == "f1")
    {
        return 0b00000100;
    }
    else if (type == "f2")
    {
        return 0b00000100;
    }
    else if (type == "f3")
    {
        return 0b00000100;
    }
    else if (type == "ptr")
    {
        return 0b00000010;
    }
    else if (type == "csp")
    {
        return 0b00001000;
    }
    else if (type == "usp")
    {
        return 0b00001000;
    }
    else if (type == "lsp")
    {
        return 0b00001000;
    }
    else if (type == "fsp")
    {
        return 0b00000100;
    }
    else if (type == "psp")
    {
        return 0b00000010;
    }
    else if (type == "null")
    {
        return 0b00000001;
    }
}

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

void Falcon::Assembler::Lexer::makeNum(std::vector<Token> & tokens, uint64_t line)
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
    token.line = line;
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
        if (this->currentChar == ':' || this->currentChar == ' ' || this->currentChar == '\n' || this->currentChar == '\t' || this->currentChar == '(')
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
    uint64_t line = 1;

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
            token.line = line;
            tokens.push_back(token);
            line++;
        }
        else if (this->currentChar == '.')
        {
            this->advance();
            Token token;
            token.type = Token::FUNCTION;
            token.name = this->makeStr();
            token.line = line;
            tokens.push_back(token);
        }
        else if (this->currentChar == '%')
        {
            this->advance();
            Token token;
            token.type = Token::EXTERN;
            token.name = this->makeStr();
            token.line = line;
            tokens.push_back(token);
        }
        else if (this->currentChar == ':')
        {
            Token token;
            token.type = Token::COLON;
            token.line = line;
            tokens.push_back(token);
        }
        else if (this->currentChar == '(')
        {
            this->advance();

            std::string str;

            while (this->currentChar != ')')
            {
                str.push_back(this->currentChar);
                this->advance();
            }

            Token token;
            token.type = Token::FUNCTION;
            token.name = str;
            token.line = line;
            tokens.push_back(token);

            this->advance();
        }
        else if (std::isdigit(this->currentChar))
        {
            this->makeNum(tokens, line);
        }
        else if (this->currentChar == '\'')
        {
            this->advance();

            Token token;
            token.type = Token::CHAR;
            token.value.c = this->currentChar;
            token.line = line;
            tokens.push_back(token);

            this->advance();
        }
        else if (std::isalpha(this->currentChar))
        {
            std::string str = this->makeStr();
            if (std::find(registers.begin(), registers.end(), str) != registers.end())
            {
                Token token;
                token.type = Token::REGISTER;
                token.name = str;
                token.line = line;
                this->advance();
                if (this->currentChar == '(')
                {
                    this->advance();

                    std::string number;

                    while (this->currentChar != ')')
                    {
                        number.push_back(this->currentChar);
                        this->advance();
                    }

                    token.value.c = std::strtol(number.c_str(), NULL, 10);
                }
                else
                {
                    this->cursor -= 2;
                    this->advance();
                }
                tokens.push_back(token);
            }
            else if (std::find(instructions.begin(), instructions.end(), str) != instructions.end())
            {
                Token token;
                token.type = Token::INSTRUCTION;
                token.name = str;
                token.line = line;
                tokens.push_back(token);
            }
            else if (str == "end")
            {
                Token token;
                token.type = Token::END;
                token.line = line;
                tokens.push_back(token);
            }
            else
            {
                Internal::CompileTimeError("InvalidToken", std::string("Invalid token \'") + str + "\'", line);
            }
        }

        this->advance();
    }

    return tokens;
}

Falcon::Assembler::ExprAST::~ExprAST()
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, char __char)
    : type(__type), c(__char), semanticValue(0b00000001)
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, uint64_t __uint)
    : type(__type), u(__uint), semanticValue(0b00000001)
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, int64_t __int)
    : type(__type), l(__int), semanticValue(0b00000001)
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, double __float)
    : type(__type), f(__float), semanticValue(0b00000001)
{
}

Falcon::Assembler::ExprAtom::ExprAtom(AtomType __type, Token __reg, uint8_t __offset)
    : type(__type), reg(__reg), regOffset(__offset), semanticValue(getSemanticRegister(__reg.name))
{
}

void Falcon::Assembler::ExprAtom::codeGen(std::ostream & out)
{
}

Falcon::Assembler::ExprStatement::ExprStatement(Token __instruction, bool __atom2Used, ExprAtom __atom0, ExprAtom __atom1)
    : instruction(__instruction), atom2Used(__atom2Used), atoms{__atom0, __atom1}
{
    /*if (this->instruction.name != "pop")
    {
        if (this->atoms[0].type == ExprAtom::REGISTER && this->atoms[0].reg.name == "null")
        {
            Internal::CompileTimeError("Reading/WritingNullRegister", "Reading from or writing to null register", this->instruction.line);
        }
        else if (this->atoms[1].type == ExprAtom::REGISTER && this->atoms[1].reg.name == "null")
        {
            Internal::CompileTimeError("ReadingNullRegister", "Reading from null register", this->instruction.line);
        }
    }*/
    uint8_t semanticInstruction = semanticInstructions[this->instruction.name];
    uint8_t semanticReg0        = this->atoms[0].semanticValue;
    uint8_t semanticReg1        = this->atoms[1].semanticValue;
    uint8_t semanticArgs        = (semanticReg0 << 4) | semanticReg1;

    auto invokeError = [this]()
    {
        std::string description("Instruction \'");
        description.append(this->instruction.name);
        description.append("\' has invalid arguements");
        Internal::CompileTimeError("TypeMismatch", description, this->instruction.line); 
    };

    if (semanticInstruction == 0b00000000)
    {
    }
    else if ((semanticInstruction & 0b00001111) == 0b00000000)
    {
        if ((semanticInstruction | (semanticReg0 << 4)) != semanticInstruction)
        {
            invokeError();
        }
    }
    else
    {
        if ((semanticInstruction & 0b00001111) == 0b00000001)
        {
            if ((semanticInstruction | (semanticReg0 << 4)) != semanticInstruction)
            {
                invokeError();
            }
            else if (this->atoms[1].type == ExprAtom::REGISTER)
            {
                invokeError();
            }
        }
        else
        {
            if ((semanticInstruction | semanticArgs) != semanticInstruction)
            {
                invokeError();
            }
        }
    }
}

void Falcon::Assembler::ExprStatement::codeGen(std::ostream & out)
{
    if (this->instruction.name == "call" || this->instruction.name == "jmp" || this->instruction.name == "if" || this->instruction.name == "else")
    {
        InstructionType inst;

        if (this->instruction.name == "call")
        {
            inst = INSTRUCTION_CALL;
        }
        else if (this->instruction.name == "jmp")
        {
            inst = INSTRUCTION_JMP;
        }
        else if (this->instruction.name == "if")
        {
            inst = INSTRUCTION_IF;
        }
        else if (this->instruction.name == "else")
        {
            inst = INSTRUCTION_ELSE;
        }

        inst = (InstructionType)((inst & 0b00111111) << 2);
        
        int8_t bytes[] =
        {
            this->atoms[0].c,
            this->atoms[1].c
        };

        out.write((char *)&inst, 1);
        out.write((char *)bytes, 2);
    }
    else if (this->instruction.type == Token::END)
    {
        InstructionType end = INSTRUCTION_END;
        end = (InstructionType)((end & 0b00111111) << 2);
        out.write((char *)&end, 1);
    }
    else if (this->instruction.name == "mov")
    {
        RegisterType type = getRegisterType(this->atoms[0].reg.name);

        if (type >= REGISTER_C0 && type <= REGISTER_C3)
        {
            uint8_t bytes[4];

            bytes[0] = (0b00111111 & instructionMap[this->instruction.name]) << 2;
            bytes[0] = bytes[0] | (0b00000011 & (getRegisterType(this->atoms[0].reg.name) >> 3));
            bytes[1] = (0b11100000 & (getRegisterType(this->atoms[0].reg.name) << 5));
            bytes[1] = bytes[1] | (0b00011111 & (this->atoms[0].regOffset >> 3));
            bytes[2] = (0b11100000 & (this->atoms[0].regOffset << 5));
            bytes[2] = bytes[2] | (0b00011111 & (getRegisterType(this->atoms[1].reg.name)));
            bytes[3] = this->atoms[1].c;

            out.write((char *)bytes, 4);
        }
        else
        {
            uint8_t bytes[11];

            bytes[0]    = (0b00111111 & instructionMap[this->instruction.name]) << 2;
            bytes[0]    = bytes[0] | (0b00000011 & (getRegisterType(this->atoms[0].reg.name) >> 3));
            bytes[1]    = (0b11100000 & (getRegisterType(this->atoms[0].reg.name) << 5));
            bytes[1]    = bytes[1] | (0b00011111 & (this->atoms[0].regOffset >> 3));
            bytes[2]    = (0b11100000 & (this->atoms[0].regOffset << 5));
            bytes[2]    = bytes[2] | (0b00011111 & (getRegisterType(this->atoms[1].reg.name)));

            bytes[3]    = ((uint8_t *)&this->atoms[1].u)[0];
            bytes[4]    = ((uint8_t *)&this->atoms[1].u)[1];
            bytes[5]    = ((uint8_t *)&this->atoms[1].u)[2];
            bytes[6]    = ((uint8_t *)&this->atoms[1].u)[3];
            bytes[7]    = ((uint8_t *)&this->atoms[1].u)[4];
            bytes[8]    = ((uint8_t *)&this->atoms[1].u)[5];
            bytes[9]    = ((uint8_t *)&this->atoms[1].u)[6];
            bytes[10]   = ((uint8_t *)&this->atoms[1].u)[7];

            out.write((char *)bytes, 11);
        }
    }
    else if (this->instruction.name == "push" || this->instruction.name == "pop")
    {
        RegisterType type = getRegisterType(this->atoms[0].reg.name);
        
        uint8_t bytes[3];

        bytes[0] = (0b00111111 & instructionMap[this->instruction.name]) << 2;
        bytes[0] = bytes[0] | (0b00000011 & (getRegisterType(this->atoms[0].reg.name) >> 3));
        bytes[1] = (0b11100000 & (getRegisterType(this->atoms[0].reg.name) << 5));
        bytes[1] = bytes[1] | (0b00011111 & (this->atoms[0].regOffset >> 3));
        bytes[2] = (0b11100000 & (this->atoms[0].regOffset << 5));
        bytes[2] = bytes[2] | (0b00011111 & (getRegisterType(this->atoms[1].reg.name)));

        out.write((char *)bytes, 3);
    }
    else
    {
        uint8_t bytes[4];

        bytes[0] = (0b00111111 & instructionMap[this->instruction.name]) << 2;
        bytes[0] = bytes[0] | (0b00000011 & (getRegisterType(this->atoms[0].reg.name) >> 3));
        bytes[1] = (0b11100000 & (getRegisterType(this->atoms[0].reg.name) << 5));
        bytes[1] = bytes[1] | (0b00011111 & (this->atoms[0].regOffset >> 3));
        bytes[2] = (0b11100000 & (this->atoms[0].regOffset << 5));
        bytes[2] = bytes[2] | (0b00011111 & (getRegisterType(this->atoms[1].reg.name)));
        bytes[3] = this->atoms[1].regOffset;

        out.write((char *)bytes, 4);
    }
}

Falcon::Assembler::ExprFunction::ExprFunction(uint16_t __id)
    : id(__id)
{
}

void Falcon::Assembler::ExprFunction::codeGen(std::ostream & out)
{
    InstructionType start = INSTRUCTION_START;
    start = (InstructionType)((start & 0b00111111) << 2);

    out.write((char *)&start, 1);
    out.write((char *)&this->id, 2);

    for (ExprStatement & statement : this->statements)
    {
        statement.codeGen(out);
    }
}

Falcon::Assembler::ExprExtern::ExprExtern(Token __identifier)
    : identifier(__identifier)
{
}

void Falcon::Assembler::ExprExtern::codeGen(std::ostream & out)
{
}

Falcon::Assembler::Parser::Parser(std::vector<Token> __tokens)
    : tokens(__tokens), symbolID(0), currentToken(__tokens[0]), cursor(0)
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
    if (this->currentToken.type == Token::CHAR)
    {
        *ast = new ExprAtom(ExprAtom::CHAR, this->currentToken.value.c);
    }
    else if (this->currentToken.type == Token::P_INT)
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
        *ast = new ExprAtom(ExprAtom::REGISTER, this->currentToken, this->currentToken.value.c);
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
    else if (std::find(unaryInstructions.begin(), unaryInstructions.end(), this->currentToken.name) != unaryInstructions.end())
    {
        Token token = this->currentToken;

        ExprAtom * atom = nullptr;

        this->advance();
        this->atom((ExprAST **)&atom);

        *ast = new ExprStatement(token, false, *atom, *atom);
    }
    else if (this->currentToken.name == "call" || this->currentToken.name == "jmp" || this->currentToken.name == "if" || this->currentToken.name == "else")
    {
        Token token = this->currentToken;

        this->advance();
        
        if (this->currentToken.name != "jmp"){
            if (this->symbolTable.count(this->currentToken.name) == 0)
            {
                this->symbolTable.insert(std::pair<std::string, uint16_t>(this->currentToken.name, this->symbolID++));
            }

            uint16_t symbol = this->symbolTable[this->currentToken.name];

            *ast = new ExprStatement(token, true, ExprAtom(ExprAtom::CHAR, ((char *)&symbol)[0]), ExprAtom(ExprAtom::CHAR, ((char *)&symbol)[1]));
        }
        else
        {
            *ast = new ExprStatement(token, true, ExprAtom(ExprAtom::CHAR, ((char)this->currentToken.value.u)), ExprAtom(ExprAtom::CHAR, '\0'));
        }
    }
    else if (this->currentToken.type == Token::END)
    {
        *ast = new ExprStatement(this->currentToken, false, ExprAtom(ExprAtom::UINT, (uint64_t)0), ExprAtom(ExprAtom::UINT, (uint64_t)0)); 
    }
}

void Falcon::Assembler::Parser::function(ExprAST ** ast, std::vector<ExprAST *> & functions)
{
    if (this->symbolTable.count(this->currentToken.name) == 0)
    {
        *ast = new ExprFunction(this->symbolID++);

        ExprFunction * function = (ExprFunction *)*ast;

        this->symbolTable.insert(std::pair<std::string, uint16_t>(this->currentToken.name, function->id));
    }
    else
    {
        *ast = new ExprFunction(this->symbolTable[this->currentToken.name]);
    }

    ExprFunction * function = (ExprFunction *)*ast;

    this->advance();
    this->advance();
    this->advance();

    while (this->cursor < this->tokens.size() && this->currentToken.type != Token::END)
    {
        if (this->currentToken.type == Token::INSTRUCTION)
        {
            ExprStatement * statement = nullptr;
            this->statement((ExprAST **)&statement);
            function->statements.push_back(*statement);
        }
        else if (this->currentToken.type == Token::FUNCTION)
        {
            ExprAST * subAST = nullptr;
            this->function(&subAST, functions);
            functions.emplace_back(subAST);
        }
        else if (this->currentToken.type == Token::REGISTER)
        {
            Internal::CompileTimeError("InvalidRegisterExpectedFunction", std::string("Invalid register \'") + this->currentToken.name + "\', expected a function", this->currentToken.line);
        }
        else if (this->currentToken.type == Token::P_INT || this->currentToken.type == Token::N_INT || this->currentToken.type == Token::FLOAT)
        {
            Internal::CompileTimeError("InvalidNumberExpectedFunction", "Invalid number, expected a function", this->currentToken.line);
        }

        this->advance();
    }

    ExprStatement * statement = nullptr;

    if (this->currentToken.type == Token::END)
    {
        this->statement((ExprAST **)&statement);    
    }
    else
    {
        this->cursor -= 2;
        this->advance();
        Internal::CompileTimeError("ExpectedEnd", "Expected \'end\'", this->currentToken.line);
    }

    function->statements.push_back(*statement);
}

std::pair<std::vector<Falcon::Assembler::ExprAST *>, std::unordered_map<std::string, uint16_t>> Falcon::Assembler::Parser::process()
{
    std::vector<ExprAST *> functions{nullptr};

    while (this->cursor < this->tokens.size())
    {
        if (this->currentToken.type == Token::FUNCTION)
        {
            this->function(&functions[0], functions);
        }
        else if (this->currentToken.type == Token::EXTERN)
        {
            this->symbolTable.insert(std::pair<std::string, uint16_t>(this->currentToken.name, this->symbolID++));
        }
        else if (this->currentToken.type == Token::INSTRUCTION)
        {
            Internal::CompileTimeError("InvalidInstructionExpectedFunction", std::string("Invalid instruction \'") + this->currentToken.name + "\', expected a function", this->currentToken.line);
        }
        else if (this->currentToken.type == Token::REGISTER)
        {
            Internal::CompileTimeError("InvalidRegisterExpectedFunction", std::string("Invalid register \'") + this->currentToken.name + "\', expected a function", this->currentToken.line);
        }
        else if (this->currentToken.type == Token::P_INT || this->currentToken.type == Token::N_INT || this->currentToken.type == Token::FLOAT)
        {
            Internal::CompileTimeError("InvalidNumberExpectedFunction", "Invalid number, expected a function", this->currentToken.line);
        }
        else if (this->currentToken.type == Token::END)
        {
            Internal::CompileTimeError("UnexpectedEnd", "Unexpected \'end\'", this->currentToken.line);
        }
        this->advance();
    }

    return std::pair<std::vector<ExprAST *>, std::unordered_map<std::string, uint16_t>>(functions, this->symbolTable);
}

Falcon::Assembler::Generator::Generator(std::vector<ExprAST *> __ast, std::unordered_map<std::string, uint16_t> & __symbolTable)
    : ast(__ast), symbolTable(__symbolTable)
{
}

void Falcon::Assembler::Generator::process(std::ostream & out)
{
    {
        InstructionType symbol = INSTRUCTION_SYMBOL;
        symbol = (InstructionType)((symbol & 0b00111111) << 2);

        for (std::pair<std::string, uint16_t> sym : this->symbolTable)
        {
            out.write((char *)&symbol, 1);
            out.write((char *)&sym.second, 2);
            out.write(sym.first.c_str(), sym.first.size() + 1);
        }
    }

    for (ExprAST * func : this->ast)
    {
        if (dynamic_cast<ExprFunction *>(func))
        {
            ((ExprFunction *)func)->codeGen(out);
        }
    }
}
