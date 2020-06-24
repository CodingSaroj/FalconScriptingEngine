#ifndef FALCON_VM_HPP
#define FALCON_VM_HPP

#include <functional>
#include <stack>
#include <unordered_map>

#include "OpCode.hpp"
#include "Register.hpp"

#ifndef FALCON_VM_STACK_SIZE
    #define FALCON_VM_STACK_SIZE 65536
#endif

namespace Falcon
{
    class VM
    {
        public:
            VM(uint8_t * code);

            Register &  getRegister(RegisterType::RegisterType type, uint64_t offset);

            void        push(uint8_t * data, uint8_t size);
            uint8_t *   pop(uint8_t size);

            void run(uint64_t entryPoint);

        private:
            uint8_t * m_Code;
            
            uint64_t  m_IP;
            uint64_t  m_SP;
            bool      m_Cmp[2];

            std::stack<uint64_t> m_StackFrame;

            Register  m_Registers[21];

            uint8_t   m_Stack[FALCON_VM_STACK_SIZE];

            std::function<void()> m_Operators[(uint8_t)OpCode::STOP + 1];

            void uadd8();
            void uadd16();
            void uadd32();
            void uadd64();

            void usub8();
            void usub16();
            void usub32();
            void usub64();

            void umul8();
            void umul16();
            void umul32();
            void umul64();

            void udiv8();
            void udiv16();
            void udiv32();
            void udiv64();

            void umod8();
            void umod16();
            void umod32();
            void umod64();

            void uinc8();
            void uinc16();
            void uinc32();
            void uinc64();

            void udec8();
            void udec16();
            void udec32();
            void udec64();
            
            void ulst8();
            void ulst16();
            void ulst32();
            void ulst64();

            void urst8();
            void urst16();
            void urst32();
            void urst64();

            void ubnd8();
            void ubnd16();
            void ubnd32();
            void ubnd64();

            void ubor8();
            void ubor16();
            void ubor32();
            void ubor64();

            void uxor8();
            void uxor16();
            void uxor32();
            void uxor64();

            void ucml8();
            void ucml16();
            void ucml32();
            void ucml64();

            void unot8();
            void unot16();
            void unot32();
            void unot64();

            void ugrt8();
            void ugrt16();
            void ugrt32();
            void ugrt64();

            void ules8();
            void ules16();
            void ules32();
            void ules64();

            void ugre8();
            void ugre16();
            void ugre32();
            void ugre64();

            void ulse8();
            void ulse16();
            void ulse32();
            void ulse64();

            void uise8();
            void uise16();
            void uise32();
            void uise64();

            void uneq8();
            void uneq16();
            void uneq32();
            void uneq64();

            void lnd();

            void lor();

            void umov8();
            void umov16();
            void umov32();
            void umov64();

            void upsh8();
            void upsh16();
            void upsh32();
            void upsh64();

            void upop8();
            void upop16();
            void upop32();
            void upop64();

            void jmt();
            void jmf();

            void call();
            void extrn();
    };
}

#endif
