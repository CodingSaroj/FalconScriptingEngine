#ifndef FALCON_VM_HPP
#define FALCON_VM_HPP

#include <algorithm>
#include <functional>
#include <stack>
#include <unordered_map>
#include <vector>

#include <cstdlib>
#include <cstring>

#include "OpCode.hpp"
#include "Register.hpp"

#ifndef FALCON_VM_STACK_SIZE
    #define FALCON_VM_STACK_SIZE 65536
#endif

#define FALCON_VM_DEFAULT_HEAP_SIZE 8388608

namespace Falcon
{
    class VM
    {
        public:
            VM(uint8_t * code, uint64_t heapSize = FALCON_VM_DEFAULT_HEAP_SIZE);
            ~VM();

            Register &  getRegister(RegisterType::RegisterType type, uint64_t offset);

            void        push(uint8_t * data, uint8_t size);
            uint8_t *   pop(uint8_t size);

            void *      allocMemory(uint64_t size);
            void        freeMemory(void * address, uint64_t size);

            void externalFunction(uint32_t id, std::function<void(VM&)> function);

            void run(uint64_t entryPoint);

        private:
            uint8_t * m_Code;
            
            uint64_t  m_IP;
            uint64_t  m_SP;
            bool      m_Cmp[2];
            bool      m_NoFollowRef;

            std::stack<uint64_t> m_StackFrame;

            Register   m_Registers[12];

            Register * m_RegisterMap[RegisterType::AO1 + 1];

            uint8_t    m_Stack[FALCON_VM_STACK_SIZE];

            uint8_t *  m_Heap;
            uint64_t   m_HeapSize;

            std::vector<bool> m_AllocationBitset;

            std::function<void()> m_Operators[(uint8_t)OpCode::STOP + 1];

            std::unordered_map<uint32_t, std::function<void(VM&)>> m_ExternalFunctions;

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

            void umov8();
            void umov16();
            void umov32();
            void umov64();

            void umvr8();
            void umvr16();
            void umvr32();
            void umvr64();

            void upsh8();
            void upsh16();
            void upsh32();
            void upsh64();

            void upop8();
            void upop16();
            void upop32();
            void upop64();

            void iadd8();
            void iadd16();
            void iadd32();
            void iadd64();

            void isub8();
            void isub16();
            void isub32();
            void isub64();

            void imul8();
            void imul16();
            void imul32();
            void imul64();

            void idiv8();
            void idiv16();
            void idiv32();
            void idiv64();

            void imod8();
            void imod16();
            void imod32();
            void imod64();

            void iinc8();
            void iinc16();
            void iinc32();
            void iinc64();

            void idec8();
            void idec16();
            void idec32();
            void idec64();
            
            void ilst8();
            void ilst16();
            void ilst32();
            void ilst64();

            void irst8();
            void irst16();
            void irst32();
            void irst64();

            void ibnd8();
            void ibnd16();
            void ibnd32();
            void ibnd64();

            void ibor8();
            void ibor16();
            void ibor32();
            void ibor64();

            void ixor8();
            void ixor16();
            void ixor32();
            void ixor64();

            void icml8();
            void icml16();
            void icml32();
            void icml64();

            void inot8();
            void inot16();
            void inot32();
            void inot64();

            void igrt8();
            void igrt16();
            void igrt32();
            void igrt64();

            void iles8();
            void iles16();
            void iles32();
            void iles64();

            void igre8();
            void igre16();
            void igre32();
            void igre64();

            void ilse8();
            void ilse16();
            void ilse32();
            void ilse64();

            void iise8();
            void iise16();
            void iise32();
            void iise64();

            void ineq8();
            void ineq16();
            void ineq32();
            void ineq64();

            void imov8();
            void imov16();
            void imov32();
            void imov64();

            void imvr8();
            void imvr16();
            void imvr32();
            void imvr64();

            void ipsh8();
            void ipsh16();
            void ipsh32();
            void ipsh64();

            void ipop8();
            void ipop16();
            void ipop32();
            void ipop64();

            void fadd32();
            void fadd64();

            void fsub32();
            void fsub64();

            void fmul32();
            void fmul64();

            void fdiv32();
            void fdiv64();

            void finc32();
            void finc64();

            void fdec32();
            void fdec64();
            
            void fgrt32();
            void fgrt64();

            void fles32();
            void fles64();

            void fgre32();
            void fgre64();

            void flse32();
            void flse64();

            void fise32();
            void fise64();

            void fneq32();
            void fneq64();

            void fmov32();
            void fmov64();

            void fmvr32();
            void fmvr64();

            void fpsh32();
            void fpsh64();

            void fpop32();
            void fpop64();

            void radd8();
            void radd16();
            void radd32();
            void radd64();

            void rsub8();
            void rsub16();
            void rsub32();
            void rsub64();

            void lnd();
            void lor();

            void popnul();
            
            void alloc();
            void free();

            void jmt();
            void jmf();

            void call();
            void extrn();
    };
}

#endif
