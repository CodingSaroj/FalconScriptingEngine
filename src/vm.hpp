#ifndef FALCON_VM_HPP
#define FALCON_VM_HPP

#include <iostream>
#include <stack>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <csignal>
#include <cstdint>
#include <cstring>

#ifndef FALCON_VM_STACK_SIZE
    #define FALCON_VM_STACK_SIZE 8192
#endif

namespace Falcon
{
    enum InstructionType : uint8_t
    {
        INSTRUCTION_ADD,
        INSTRUCTION_SUB,
        INSTRUCTION_MUL,
        INSTRUCTION_DIV,
        INSTRUCTION_MOD,
        INSTRUCTION_INC,
        INSTRUCTION_DEC,

        INSTRUCTION_LSHFT,
        INSTRUCTION_RSHFT,
        INSTRUCTION_AND,
        INSTRUCTION_OR,
        INSTRUCTION_XOR,
        INSTRUCTION_CMPL,

        INSTRUCTION_IF,
        INSTRUCTION_ELSE,
        INSTRUCTION_GRT0,
        INSTRUCTION_GRT1,
        INSTRUCTION_GREQ0,
        INSTRUCTION_GREQ1,
        INSTRUCTION_LESS0,
        INSTRUCTION_LESS1,
        INSTRUCTION_LSEQ0,
        INSTRUCTION_LSEQ1,
        INSTRUCTION_ISEQ0,
        INSTRUCTION_ISEQ1,
        INSTRUCTION_NEQ0,
        INSTRUCTION_NEQ1,
        INSTRUCTION_NOT0,
        INSTRUCTION_NOT1,
        INSTRUCTION_CAND,
        INSTRUCTION_COR,

        INSTRUCTION_PUSH,
        INSTRUCTION_POP,
        INSTRUCTION_MOV,
        INSTRUCTION_MOVR,
        INSTRUCTION_CAST,
        INSTRUCTION_CALL,
        INSTRUCTION_JMP,
        INSTRUCTION_RAISE,
        INSTRUCTION_SYMBOL,
        INSTRUCTION_EXTERN,
        INSTRUCTION_START,
        INSTRUCTION_END
    };

    enum RegisterType : uint8_t
    {
        REGISTER_C0,
        REGISTER_C1,
        REGISTER_C2,
        REGISTER_C3,
        REGISTER_U0,
        REGISTER_U1,
        REGISTER_U2,
        REGISTER_U3,
        REGISTER_L0,
        REGISTER_L1,
        REGISTER_L2,
        REGISTER_L3,
        REGISTER_F0,
        REGISTER_F1,
        REGISTER_F2,
        REGISTER_F3,
        REGISTER_CSP,
        REGISTER_USP,
        REGISTER_LSP,
        REGISTER_FSP,
        REGISTER_NULL
    };

    namespace Internal
    {
        struct Instruction
        {
            InstructionType type        : 6;
            RegisterType    arg1        : 5;
            uint8_t         arg1_offset    ;
            RegisterType    arg2        : 5;
            union
            {
                uint8_t     arg2_offset;
                uint64_t    u;
                int64_t     l;
                double      f;
            } extra;
            std::string     symbol;
        };

        struct Register
        {
            union Value
            {
                char        c;
                uint64_t    u;
                int64_t     l;
                double      f;
            };
            
            RegisterType    type;
            Value           value;
        };
        
        class CompileTimeError
        {
            public:
                CompileTimeError(std::string name, std::string description, uint64_t location, std::string filename = "");
        };

        class RuntimeError
        {
            public:
                RuntimeError(std::string name, std::string description, std::string function, uint64_t location, std::stack<std::pair<std::string, uint64_t>> stackTrace);
        };
    }

    class VM
    {
        private:
            /*
             * Flags
             */
            bool    advanceOnly;
            bool    jmpStart;
            bool    jmpEnd;
            bool    cmpResult[2];

            /*
             * Bytecode related
             */
            std::vector<Internal::Instruction>  instructions;
            Internal::Instruction               currentInstruction;
            uint64_t                            instructionPtr;

            /*
             * State
             */
            std::stack<std::pair<std::string, uint64_t>>    stackFrame;
            std::string                                     currentFunction;

            /*
             * Registers
             */
            Internal::Register  registers[17];

            /*
             * Stack
             */
            Internal::Register  stack[FALCON_VM_STACK_SIZE];
            uint16_t            stackPtr;

            /*
             * Operators
             */
            void( *     operators[47])(VM &);

            /*
             * Symbols:
             *
             * uint64_t     -> id
             * std::string  -> name
             *
             *
             * Function map:
             *
             * std::string  -> name
             * bool         -> external flag
             * uint64_t     -> bytecode address or function pointer
             */
            std::unordered_map<uint64_t, std::string> symbols;
            std::unordered_map<std::string, std::pair<bool, uint64_t>> functions;

            void compile(std::string __bytecode);

            inline void advance()
            {
                if (this->instructionPtr + 1 > this->instructions.size())
                {
                    this->instructionPtr = this->instructions.size();
                    Internal::Instruction i;
                    this->currentInstruction = i;
                    return;
                }
                this->currentInstruction = this->instructions[this->instructionPtr++];
            }

        public:
            VM(const std::string & __bytecode);
       
            /*
             * Flag getters and setters
             */
            bool        getCmpResult(uint8_t id);
            void        setCmpResult(uint8_t id, bool res);
            void        setJmp(bool start);

            uint64_t                    getInstructionPtr();
            Internal::Instruction &     getCurrentInstruction();

            std::string                                     getCurrentFunction();
            std::stack<std::pair<std::string, uint64_t>>    getStackTrace();

            /*
             * Register getter
             */
            Internal::Register & getRegister(RegisterType type, uint8_t offset);
            
            /*
             * Function adding functions
             */
            std::string getSymbol(uint16_t id);
            void        registerSymbol(uint16_t id, std::string name);
            void        registerFunction(std::string name, uint64_t location);
            void        externalFunction(std::string name, void( * function)(VM & vm));

            /*
             * Stack manipulation
             */
            void        pushChar(char data);
            void        pushUint(uint64_t data);
            void        pushInt(int64_t data);
            void        pushFloat(double data);
            char        popChar();
            uint64_t    popUint();
            int64_t     popInt();
            double      popFloat();

            /*
             * Call a function
             */
            void        run(std::string function);
            void        gotoFunction(std::string function);
    };
}

#endif
