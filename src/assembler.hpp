#ifndef FALCON_ASSEMBLER_HPP
#define FALCON_ASSEMBLER_HPP

#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "vm.hpp"

namespace Falcon
{
    namespace Assembler
    {
        struct Token
        {
            enum TokenType
            {
                CHAR,
                P_INT,
                N_INT,
                FLOAT,
                COLON,
                FUNCTION,
                INSTRUCTION,
                REGISTER,
                ENDL,
                END
            } type;

            union
            {
                char        c;
                uint64_t    u;
                int64_t     l;
                _Float64    f;
            } value;
            std::string name;
        };

        class Lexer
        {
            private:
                const std::string   text;
                char                currentChar;
                uint64_t            cursor;

                void        advance();

                void        makeNum(std::vector<Token> & tokens);
                std::string makeStr();

            public:
                Lexer(std::string __text);

                std::vector<Token> process();
        };

        class ExprAST
        {
            protected:
                virtual ~ExprAST() = 0;
        };

        class ExprAtom : public ExprAST
        {
            public:
                enum AtomType
                {
                    UINT,
                    INT,
                    FLOAT,
                    REGISTER
                } type;

                union
                {
                    uint64_t    u;
                    int64_t     l;
                    _Float64    f;
                };

                struct
                {
                    Token   reg;
                    uint8_t regOffset;
                };

                ExprAtom(AtomType __type, uint64_t __uint);
                ExprAtom(AtomType __type, int64_t __int);
                ExprAtom(AtomType __type, _Float64 __float);
                ExprAtom(AtomType __type, Token __reg, uint8_t __offset);
        };

        class ExprStatement : public ExprAST
        {
            public:
                Token       instruction;
                bool        atom2Used;
                ExprAtom    atoms[2];

                ExprStatement(Token __instruction, bool __atom2Used, ExprAtom __atom0, ExprAtom __atom1);
        };

        class ExprFunction : public ExprAST
        {
            public:
                Token                       identifier;
                std::vector<ExprStatement>  statements;

                ExprFunction(Token __identifier);
        };

        class ExprExtern : public ExprAST
        {
            public:
                Token identifier;

                ExprExtern(Token __identifier);
        };

        class Parser
        {
            private:
                const std::vector<Token>    tokens;
                Token                       currentToken;
                uint64_t                    cursor;

                void advance();

                void atom(ExprAST ** ast);
                void statement(ExprAST ** ast);
                void function(ExprAST ** ast);

            public:
                Parser(std::vector<Token> __tokens);

                ExprAST * process();
        };

        class Generator
        {
            private:
                const ExprAST * ast;

            public:
                enum class Error
                {
                    NONE,
                    INVALID_OPERATION,
                    INVALID_OPERAND,
                    EXPECTED_COLON,
                    UNDEFINED_ROUTINE,
                    UNDEFINED_LABEL,
                    MULTIPLE_DEFINITION_ROUTINE,
                    MULTIPLE_DEFINITION_LABEL
                };

                Generator(ExprAST * ast);

                Error process(std::ostream & out);
        };
    }
}

#endif
