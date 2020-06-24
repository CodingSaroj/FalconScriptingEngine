#include "VM.hpp"

namespace Falcon
{
    VM::VM(uint8_t * code, uint64_t heapSize)
        : m_Code(code), m_HeapSize(heapSize)
    {
        m_Heap = (uint8_t *)malloc(m_HeapSize);

        m_AllocationBitset.reserve(m_HeapSize);
        std::fill(m_AllocationBitset.begin(), m_AllocationBitset.end(), false);

        m_RegisterMap[RegisterType::A8] = &m_Registers[0];
        m_RegisterMap[RegisterType::A16] = &m_Registers[0];
        m_RegisterMap[RegisterType::A32] = &m_Registers[0];
        m_RegisterMap[RegisterType::A64] = &m_Registers[0];
        
        m_RegisterMap[RegisterType::B8] = &m_Registers[1];
        m_RegisterMap[RegisterType::B16] = &m_Registers[1];
        m_RegisterMap[RegisterType::B32] = &m_Registers[1];
        m_RegisterMap[RegisterType::B64] = &m_Registers[1];
        
        m_RegisterMap[RegisterType::C8] = &m_Registers[2];
        m_RegisterMap[RegisterType::C16] = &m_Registers[2];
        m_RegisterMap[RegisterType::C32] = &m_Registers[2];
        m_RegisterMap[RegisterType::C64] = &m_Registers[2];

        m_RegisterMap[RegisterType::D8] = &m_Registers[3];
        m_RegisterMap[RegisterType::D16] = &m_Registers[3];
        m_RegisterMap[RegisterType::D32] = &m_Registers[3];
        m_RegisterMap[RegisterType::D64] = &m_Registers[3];
        
        m_RegisterMap[RegisterType::R1] = &m_Registers[4];
        m_RegisterMap[RegisterType::R2] = &m_Registers[5];
        m_RegisterMap[RegisterType::R3] = &m_Registers[6];
        m_RegisterMap[RegisterType::R4] = &m_Registers[7];

        m_RegisterMap[RegisterType::SP] = &m_Registers[8];
        m_RegisterMap[RegisterType::CMT] = &m_Registers[9];
        m_RegisterMap[RegisterType::AO0] = &m_Registers[10];
        m_RegisterMap[RegisterType::AO1] = &m_Registers[11];

        m_Operators[OpCode::UADD8] = std::bind(&VM::uadd8, this);
        m_Operators[OpCode::UADD16] = std::bind(&VM::uadd16, this);
        m_Operators[OpCode::UADD32] = std::bind(&VM::uadd32, this);
        m_Operators[OpCode::UADD64] = std::bind(&VM::uadd64, this);

        m_Operators[OpCode::USUB8] = std::bind(&VM::usub8, this);
        m_Operators[OpCode::USUB16] = std::bind(&VM::usub16, this);
        m_Operators[OpCode::USUB32] = std::bind(&VM::usub32, this);
        m_Operators[OpCode::USUB64] = std::bind(&VM::usub64, this);

        m_Operators[OpCode::UMUL8] = std::bind(&VM::umul8, this);
        m_Operators[OpCode::UMUL16] = std::bind(&VM::umul16, this);
        m_Operators[OpCode::UMUL32] = std::bind(&VM::umul32, this);
        m_Operators[OpCode::UMUL64] = std::bind(&VM::umul64, this);

        m_Operators[OpCode::UDIV8] = std::bind(&VM::udiv8, this);
        m_Operators[OpCode::UDIV16] = std::bind(&VM::udiv16, this);
        m_Operators[OpCode::UDIV32] = std::bind(&VM::udiv32, this);
        m_Operators[OpCode::UDIV64] = std::bind(&VM::udiv64, this);

        m_Operators[OpCode::UMOD8] = std::bind(&VM::umod8, this);
        m_Operators[OpCode::UMOD16] = std::bind(&VM::umod16, this);
        m_Operators[OpCode::UMOD32] = std::bind(&VM::umod32, this);
        m_Operators[OpCode::UMOD64] = std::bind(&VM::umod64, this);

        m_Operators[OpCode::UINC8] = std::bind(&VM::uinc8, this);
        m_Operators[OpCode::UINC16] = std::bind(&VM::uinc16, this);
        m_Operators[OpCode::UINC32] = std::bind(&VM::uinc32, this);
        m_Operators[OpCode::UINC64] = std::bind(&VM::uinc64, this);

        m_Operators[OpCode::UDEC8] = std::bind(&VM::udec8, this);
        m_Operators[OpCode::UDEC16] = std::bind(&VM::udec16, this);
        m_Operators[OpCode::UDEC32] = std::bind(&VM::udec32, this);
        m_Operators[OpCode::UDEC64] = std::bind(&VM::udec64, this);

        m_Operators[OpCode::ULST8] = std::bind(&VM::ulst8, this);
        m_Operators[OpCode::ULST16] = std::bind(&VM::ulst16, this);
        m_Operators[OpCode::ULST32] = std::bind(&VM::ulst32, this);
        m_Operators[OpCode::ULST64] = std::bind(&VM::ulst64, this);

        m_Operators[OpCode::URST8] = std::bind(&VM::urst8, this);
        m_Operators[OpCode::URST16] = std::bind(&VM::urst16, this);
        m_Operators[OpCode::URST32] = std::bind(&VM::urst32, this);
        m_Operators[OpCode::URST64] = std::bind(&VM::urst64, this);

        m_Operators[OpCode::UBND8] = std::bind(&VM::ubnd8, this);
        m_Operators[OpCode::UBND16] = std::bind(&VM::ubnd16, this);
        m_Operators[OpCode::UBND32] = std::bind(&VM::ubnd32, this);
        m_Operators[OpCode::UBND64] = std::bind(&VM::ubnd64, this);

        m_Operators[OpCode::UBOR8] = std::bind(&VM::ubor8, this);
        m_Operators[OpCode::UBOR16] = std::bind(&VM::ubor16, this);
        m_Operators[OpCode::UBOR32] = std::bind(&VM::ubor32, this);
        m_Operators[OpCode::UBOR64] = std::bind(&VM::ubor64, this);

        m_Operators[OpCode::UXOR8] = std::bind(&VM::uxor8, this);
        m_Operators[OpCode::UXOR16] = std::bind(&VM::uxor16, this);
        m_Operators[OpCode::UXOR32] = std::bind(&VM::uxor32, this);
        m_Operators[OpCode::UXOR64] = std::bind(&VM::uxor64, this);

        m_Operators[OpCode::UCML8] = std::bind(&VM::ucml8, this);
        m_Operators[OpCode::UCML16] = std::bind(&VM::ucml16, this);
        m_Operators[OpCode::UCML32] = std::bind(&VM::ucml32, this);
        m_Operators[OpCode::UCML64] = std::bind(&VM::ucml64, this);

        m_Operators[OpCode::UNOT8] = std::bind(&VM::unot8, this);
        m_Operators[OpCode::UNOT16] = std::bind(&VM::unot16, this);
        m_Operators[OpCode::UNOT32] = std::bind(&VM::unot32, this);
        m_Operators[OpCode::UNOT64] = std::bind(&VM::unot64, this);

        m_Operators[OpCode::UGRT8] = std::bind(&VM::ugrt8, this);
        m_Operators[OpCode::UGRT16] = std::bind(&VM::ugrt16, this);
        m_Operators[OpCode::UGRT32] = std::bind(&VM::ugrt32, this);
        m_Operators[OpCode::UGRT64] = std::bind(&VM::ugrt64, this);

        m_Operators[OpCode::ULES8] = std::bind(&VM::ules8, this);
        m_Operators[OpCode::ULES16] = std::bind(&VM::ules16, this);
        m_Operators[OpCode::ULES32] = std::bind(&VM::ules32, this);
        m_Operators[OpCode::ULES64] = std::bind(&VM::ules64, this);

        m_Operators[OpCode::UGRE8] = std::bind(&VM::ugre8, this);
        m_Operators[OpCode::UGRE16] = std::bind(&VM::ugre16, this);
        m_Operators[OpCode::UGRE32] = std::bind(&VM::ugre32, this);
        m_Operators[OpCode::UGRE64] = std::bind(&VM::ugre64, this);

        m_Operators[OpCode::ULSE8] = std::bind(&VM::ulse8, this);
        m_Operators[OpCode::ULSE16] = std::bind(&VM::ulse16, this);
        m_Operators[OpCode::ULSE32] = std::bind(&VM::ulse32, this);
        m_Operators[OpCode::ULSE64] = std::bind(&VM::ulse64, this);

        m_Operators[OpCode::UISE8] = std::bind(&VM::uise8, this);
        m_Operators[OpCode::UISE16] = std::bind(&VM::uise16, this);
        m_Operators[OpCode::UISE32] = std::bind(&VM::uise32, this);
        m_Operators[OpCode::UISE64] = std::bind(&VM::uise64, this);

        m_Operators[OpCode::UNEQ8] = std::bind(&VM::uneq8, this);
        m_Operators[OpCode::UNEQ16] = std::bind(&VM::uneq16, this);
        m_Operators[OpCode::UNEQ32] = std::bind(&VM::uneq32, this);
        m_Operators[OpCode::UNEQ64] = std::bind(&VM::uneq64, this);

        m_Operators[OpCode::UMOV8] = std::bind(&VM::umov8, this);
        m_Operators[OpCode::UMOV16] = std::bind(&VM::umov16, this);
        m_Operators[OpCode::UMOV32] = std::bind(&VM::umov32, this);
        m_Operators[OpCode::UMOV64] = std::bind(&VM::umov64, this);

        m_Operators[OpCode::UPSH8] = std::bind(&VM::upsh8, this);
        m_Operators[OpCode::UPSH16] = std::bind(&VM::upsh16, this);
        m_Operators[OpCode::UPSH32] = std::bind(&VM::upsh32, this);
        m_Operators[OpCode::UPSH64] = std::bind(&VM::upsh64, this);
        
        m_Operators[OpCode::UPOP8] = std::bind(&VM::upop8, this);
        m_Operators[OpCode::UPOP16] = std::bind(&VM::upop16, this);
        m_Operators[OpCode::UPOP32] = std::bind(&VM::upop32, this);
        m_Operators[OpCode::UPOP64] = std::bind(&VM::upop64, this);
        
        m_Operators[OpCode::LND] = std::bind(&VM::lnd, this);
        m_Operators[OpCode::LOR] = std::bind(&VM::lor, this);

        m_Operators[OpCode::POPNUL] = std::bind(&VM::popnul, this);

        m_Operators[OpCode::ALLOC] = std::bind(&VM::alloc, this);
        m_Operators[OpCode::FREE] = std::bind(&VM::free, this);

        m_Operators[OpCode::JMT] = std::bind(&VM::jmt, this);
        m_Operators[OpCode::JMF] = std::bind(&VM::jmf, this);

        m_Operators[OpCode::CALL] = std::bind(&VM::call, this);
        m_Operators[OpCode::EXTRN] = std::bind(&VM::extrn, this);
    }

    VM::~VM()
    {
        ::free(m_Heap);
    }

    define(`BINFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.$3 = r0.$3 $2 r1.$3;
    }')

    define(`UNFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.$3 = $2 r0.$3;
    }')

    define(`BINCMPFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.$3 $2 r1.$3;
    }')

    define(`UNCMPFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = $2 r0.$3;
    }')

    define(`MOVFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[$3];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], $3);
        m_IP += $3;

        r0.$4 = *($2 *)bytes;
    }')

    define(`MVRFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.$4 = r1.$4;
    }')

    define(`PSHFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.$3, $2);
    }')

    define(`POPFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.$4 = *($2 *)pop($3);
    }')

    BINFUN(uadd8, +, u8)

    BINFUN(uadd16, +, u16)
    
    BINFUN(uadd32, +, u32)
    
    BINFUN(uadd64, +, u64)

    BINFUN(usub8, -, u8)

    BINFUN(usub16, -, u16)
    
    BINFUN(usub32, -, u32)
    
    BINFUN(usub64, -, u64)

    BINFUN(umul8, *, u8)

    BINFUN(umul16, *, u16)
    
    BINFUN(umul32, *, u32)
    
    BINFUN(umul64, *, u64)

    BINFUN(udiv8, /, u8)

    BINFUN(udiv16, /, u16)
    
    BINFUN(udiv32, /, u32)
    
    BINFUN(udiv64, /, u64)

    BINFUN(umod8, %, u8)

    BINFUN(umod16, %, u16)
    
    BINFUN(umod32, %, u32)
    
    BINFUN(umod64, %, u64)

    UNFUN(uinc8, ++, u8)

    UNFUN(uinc16, ++, u16)
    
    UNFUN(uinc32, ++, u32)
    
    UNFUN(uinc64, ++, u64)

    UNFUN(udec8, --, u8)

    UNFUN(udec16, --, u16)
    
    UNFUN(udec32, --, u32)
    
    UNFUN(udec64, --, u64)

    BINFUN(ulst8, <<, u8)

    BINFUN(ulst16, <<, u16)
    
    BINFUN(ulst32, <<, u32)
    
    BINFUN(ulst64, <<, u64)

    BINFUN(urst8, >>, u8)

    BINFUN(urst16, >>, u16)
    
    BINFUN(urst32, >>, u32)
    
    BINFUN(urst64, >>, u64)

    BINFUN(ubnd8, &, u8)

    BINFUN(ubnd16, &, u16)
    
    BINFUN(ubnd32, &, u32)
    
    BINFUN(ubnd64, &, u64)

    BINFUN(ubor8, |, u8)

    BINFUN(ubor16, |, u16)
    
    BINFUN(ubor32, |, u32)
    
    BINFUN(ubor64, |, u64)

    BINFUN(uxor8, ^, u8)

    BINFUN(uxor16, ^, u16)
    
    BINFUN(uxor32, ^, u32)
    
    BINFUN(uxor64, ^, u64)

    UNFUN(ucml8, ~, u8)

    UNFUN(ucml16, ~, u16)
    
    UNFUN(ucml32, ~, u32)
    
    UNFUN(ucml64, ~, u64)

    UNFUN(unot8, !, u8)

    UNFUN(unot16, !, u16)
    
    UNFUN(unot32, !, u32)
    
    UNFUN(unot64, !, u64)

    BINCMPFUN(ugrt8, >, u8)

    BINCMPFUN(ugrt16, >, u16)
    
    BINCMPFUN(ugrt32, >, u32)
    
    BINCMPFUN(ugrt64, >, u64)

    BINCMPFUN(ules8, <, u8)

    BINCMPFUN(ules16, <, u16)
    
    BINCMPFUN(ules32, <, u32)
    
    BINCMPFUN(ules64, <, u64)

    BINCMPFUN(ugre8, >=, u8)

    BINCMPFUN(ugre16, >=, u16)
    
    BINCMPFUN(ugre32, >=, u32)
    
    BINCMPFUN(ugre64, >=, u64)

    BINCMPFUN(ulse8, <=, u8)

    BINCMPFUN(ulse16, <=, u16)
    
    BINCMPFUN(ulse32, <=, u32)
    
    BINCMPFUN(ulse64, <=, u64)

    BINCMPFUN(uise8, ==, u8)

    BINCMPFUN(uise16, ==, u16)
    
    BINCMPFUN(uise32, ==, u32)
    
    BINCMPFUN(uise64, ==, u64)

    BINCMPFUN(uneq8, !=, u8)

    BINCMPFUN(uneq16, !=, u16)
    
    BINCMPFUN(uneq32, !=, u32)
    
    BINCMPFUN(uneq64, !=, u64)

    MOVFUN(umov8, uint8_t, 1, u8)

    MOVFUN(umov16, uint16_t, 2, u16)
    
    MOVFUN(umov32, uint32_t, 4, u32)
    
    MOVFUN(umov64, uint64_t, 8, u64)

    MVRFUN(umvr8, uint8_t, 1, u8)

    MVRFUN(umvr16, uint16_t, 2, u16)
    
    MVRFUN(umvr32, uint32_t, 4, u32)
    
    MVRFUN(umvr64, uint64_t, 8, u64)

    PSHFUN(upsh8, 1, u8)
    
    PSHFUN(upsh16, 2, u16)
    
    PSHFUN(upsh32, 4, u32)
    
    PSHFUN(upsh64, 8, u64)

    POPFUN(upop8, uint8_t, 1, u8)
    
    POPFUN(upop16, uint16_t, 2, u16)
    
    POPFUN(upop32, uint32_t, 4, u32)
    
    POPFUN(upop64, uint64_t, 8, u64)

    void VM::lnd()
    {
        m_Cmp[0] = m_Cmp[0] && m_Cmp[1];
    }

    void VM::lor()
    {
        m_Cmp[0] = m_Cmp[0] || m_Cmp[1];
    }

    void VM::popnul()
    {
        uint32_t size = *(uint32_t *)&m_Code[++m_IP];
        pop(size);

        m_IP += 3;
    }

    void VM::alloc()
    {
        m_NoFollowRef = true;

        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], 0);
        
        m_NoFollowRef = false;

        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], 0);

        r0.ptr = allocMemory(r1.u64);
    }

    void VM::free()
    {
        m_NoFollowRef = true;

        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], 0);
        
        m_NoFollowRef = false;
        
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], 0);

        freeMemory(r0.ptr, r1.u64);
    }

    void VM::jmt()
    {
        if (m_Cmp[0])
        {
            call();
        }
        else
        {
            m_IP += 4;
        }
    }

    void VM::jmf()
    {
        if (!m_Cmp[0])
        {
            call();
        }
        else
        {
            m_IP += 4;
        }
    }

    void VM::call()
    {
        m_StackFrame.push(m_IP + 4);
        m_IP = *(uint32_t *)&m_Code[++m_IP];
        m_IP--;
    }

    void VM::extrn()
    {
        uint32_t id = *(uint32_t *)&m_Code[++m_IP];
        
        if (m_ExternalFunctions.count(id) != 0)
        {
            m_ExternalFunctions[id](*this);
        }

        m_IP += 3;
    }
    
    Register & VM::getRegister(RegisterType::RegisterType type, uint64_t offset)
    {
        if (type == RegisterType::SP)
        {
            return *(Register *)&m_Stack[m_SP - offset];
        }
        else if (type >= RegisterType::R1 && type <= RegisterType::R4)
        {
            return m_NoFollowRef ? *m_RegisterMap[type] : *(Register *)(m_RegisterMap[type]->ptr);
        }

        return *m_RegisterMap[type];
    }

    void VM::push(uint8_t * data, uint8_t size)
    {
        memcpy(&m_Stack[m_SP], data, size);
        m_SP += size;
    }

    uint8_t * VM::pop(uint8_t size)
    {
        m_SP -= size;

        return &m_Stack[m_SP];
    }

    void * VM::allocMemory(uint64_t size)
    {
        uint64_t address = 0;
        uint64_t availableSize = 0;

        for (int i = 0; i < m_HeapSize; i++)
        {
            if (!m_Heap[i])
            {
                if (!availableSize)
                {
                    address = i + 1;
                }
                
                availableSize++;

                if (availableSize == size)
                {
                    address--;
                    break;
                }
            }
            else
            {
                address = 0;
                availableSize = 0;
            }
        }

        if (address)
        {
            std::fill(m_AllocationBitset.begin() + address, m_AllocationBitset.begin() + address + size, true);
        }

        return (void *)&m_Heap[address];
    }

    void VM::freeMemory(void * address, uint64_t size)
    {
        uint64_t start = ((uint64_t)address - (uint64_t)m_Heap) - 1;
        std::fill(m_AllocationBitset.begin() + start, m_AllocationBitset.begin() + start + size, false);
    }

    void VM::externalFunction(uint32_t id, std::function<void(VM&)> function)
    {
        if (m_ExternalFunctions.count(id) == 0)
        {
            m_ExternalFunctions[id] = function;
        }
    }

    void VM::run(uint64_t entryPoint)
    {
        m_IP = entryPoint;

        uint8_t op = m_Code[m_IP];

        while (op != OpCode::STOP)
        {
            if (op == OpCode::RET)
            {
                m_IP = m_StackFrame.top();
                m_StackFrame.pop();
            }
            else
            {
                m_Operators[op]();
            }

            op = m_Code[++m_IP];
        }
    }
}
