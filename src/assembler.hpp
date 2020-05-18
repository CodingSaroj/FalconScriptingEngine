#ifndef FALCON_ASSEMBLER_HPP
#define FALCON_ASSEMBLER_HPP

#include <algorithm>
#include <iostream>
#include <map>
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
                double    f;
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
                virtual void codeGen(std::ostream & out) = 0;
        };

        class ExprAtom : public ExprAST
        {
            public:
                enum AtomType
                {
                    CHAR,
                    UINT,
                    INT,
                    FLOAT,
                    REGISTER
                } type;

                union
                {
                    char        c;
                    uint64_t    u;
                    int64_t     l;
                    double      f;
                };
                
                Token   reg;
                uint8_t regOffset;

                ExprAtom(AtomType __type, char __char);
                ExprAtom(AtomType __type, uint64_t __uint);
                ExprAtom(AtomType __type, int64_t __int);
                ExprAtom(AtomType __type, double __float);
                ExprAtom(AtomType __type, Token __reg, uint8_t __offset);

                void codeGen(std::ostream & out) override;
        };

        class ExprStatement : public ExprAST
        {
            public:
                Token       instruction;
                bool        atom2Used;
                ExprAtom    atoms[2];

                ExprStatement(Token __instruction, bool __atom2Used, ExprAtom __atom0, ExprAtom __atom1);

                void codeGen(std::ostream & out) override;
        };

        class ExprFunction : public ExprAST
        {
            public:
                uint16_t                    id;
                std::vector<ExprStatement>  statements;

                ExprFunction(uint16_t __id);

                void codeGen(std::ostream & out) override;
        };

        class ExprExtern : public ExprAST
        {
            public:
                Token identifier;

                ExprExtern(Token __identifier);

                void codeGen(std::ostream & out) override;
        };

        class Parser
        {
            private:
                const std::vector<Token>    tokens;
                Token                       currentToken;
                uint64_t                    cursor;

                std::unordered_map<std::string, uint16_t> symbolTable;
                uint16_t symbolID;

                void advance();

                void atom(ExprAST ** ast);
                void statement(ExprAST ** ast);
                void function(ExprAST ** ast, std::vector<ExprAST *> & functions);

            public:
                Parser(std::vector<Token> __tokens);

                std::pair<std::vector<ExprAST *>, std::unordered_map<std::string, uint16_t>> process();
        };

        class Generator
        {
            private:
                const std::unordered_map<std::string, uint16_t> symbolTable;

                std::vector<ExprAST *> ast;

                enum class Scope : uint8_t
                {
                    TOP,
                    FUNCTION,
                    STATEMENT
                } scope;

            public:
                class Error
                {
                    public:
                        enum ErrorType : uint8_t
                        {
                            NONE,
                            UNEXPECTED_NUMBER,
                            UNEXPECTED_REGISTER,
                            UNEXPECTED_STATEMENT,
                            INVALID_OPERATION,
                            INVALID_OPERAND,
                            EXPECTED_COLON,
                            UNDEFINED_ROUTINE,
                            UNDEFINED_LABEL,
                            MULTIPLE_DEFINITION_ROUTINE,
                            MULTIPLE_DEFINITION_LABEL
                        } type;

                        std::string data;

                        Error(ErrorType __type, std::string __data = "");
                };

                Generator(std::vector<ExprAST *> __ast, std::unordered_map<std::string, uint16_t> & __symbolTable);

                Error process(std::ostream & out);
        };
    }
}

#endif
