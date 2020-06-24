#include "VM.hpp"

namespace Falcon
{
    VM::VM(uint8_t * code)
        : m_Code(code)
    {
        m_Operators[(uint8_t)OpCode::UADD8] = std::bind(&VM::uadd8, this);
        m_Operators[(uint8_t)OpCode::UADD16] = std::bind(&VM::uadd16, this);
        m_Operators[(uint8_t)OpCode::UADD32] = std::bind(&VM::uadd32, this);
        m_Operators[(uint8_t)OpCode::UADD64] = std::bind(&VM::uadd64, this);

        m_Operators[(uint8_t)OpCode::USUB8] = std::bind(&VM::usub8, this);
        m_Operators[(uint8_t)OpCode::USUB16] = std::bind(&VM::usub16, this);
        m_Operators[(uint8_t)OpCode::USUB32] = std::bind(&VM::usub32, this);
        m_Operators[(uint8_t)OpCode::USUB64] = std::bind(&VM::usub64, this);

        m_Operators[(uint8_t)OpCode::UMUL8] = std::bind(&VM::umul8, this);
        m_Operators[(uint8_t)OpCode::UMUL16] = std::bind(&VM::umul16, this);
        m_Operators[(uint8_t)OpCode::UMUL32] = std::bind(&VM::umul32, this);
        m_Operators[(uint8_t)OpCode::UMUL64] = std::bind(&VM::umul64, this);

        m_Operators[(uint8_t)OpCode::UDIV8] = std::bind(&VM::udiv8, this);
        m_Operators[(uint8_t)OpCode::UDIV16] = std::bind(&VM::udiv16, this);
        m_Operators[(uint8_t)OpCode::UDIV32] = std::bind(&VM::udiv32, this);
        m_Operators[(uint8_t)OpCode::UDIV64] = std::bind(&VM::udiv64, this);

        m_Operators[(uint8_t)OpCode::UMOD8] = std::bind(&VM::umod8, this);
        m_Operators[(uint8_t)OpCode::UMOD16] = std::bind(&VM::umod16, this);
        m_Operators[(uint8_t)OpCode::UMOD32] = std::bind(&VM::umod32, this);
        m_Operators[(uint8_t)OpCode::UMOD64] = std::bind(&VM::umod64, this);

        m_Operators[(uint8_t)OpCode::UINC8] = std::bind(&VM::uinc8, this);
        m_Operators[(uint8_t)OpCode::UINC16] = std::bind(&VM::uinc16, this);
        m_Operators[(uint8_t)OpCode::UINC32] = std::bind(&VM::uinc32, this);
        m_Operators[(uint8_t)OpCode::UINC64] = std::bind(&VM::uinc64, this);

        m_Operators[(uint8_t)OpCode::UDEC8] = std::bind(&VM::udec8, this);
        m_Operators[(uint8_t)OpCode::UDEC16] = std::bind(&VM::udec16, this);
        m_Operators[(uint8_t)OpCode::UDEC32] = std::bind(&VM::udec32, this);
        m_Operators[(uint8_t)OpCode::UDEC64] = std::bind(&VM::udec64, this);

        m_Operators[(uint8_t)OpCode::ULST8] = std::bind(&VM::ulst8, this);
        m_Operators[(uint8_t)OpCode::ULST16] = std::bind(&VM::ulst16, this);
        m_Operators[(uint8_t)OpCode::ULST32] = std::bind(&VM::ulst32, this);
        m_Operators[(uint8_t)OpCode::ULST64] = std::bind(&VM::ulst64, this);

        m_Operators[(uint8_t)OpCode::URST8] = std::bind(&VM::urst8, this);
        m_Operators[(uint8_t)OpCode::URST16] = std::bind(&VM::urst16, this);
        m_Operators[(uint8_t)OpCode::URST32] = std::bind(&VM::urst32, this);
        m_Operators[(uint8_t)OpCode::URST64] = std::bind(&VM::urst64, this);

        m_Operators[(uint8_t)OpCode::UBND8] = std::bind(&VM::ubnd8, this);
        m_Operators[(uint8_t)OpCode::UBND16] = std::bind(&VM::ubnd16, this);
        m_Operators[(uint8_t)OpCode::UBND32] = std::bind(&VM::ubnd32, this);
        m_Operators[(uint8_t)OpCode::UBND64] = std::bind(&VM::ubnd64, this);

        m_Operators[(uint8_t)OpCode::UBOR8] = std::bind(&VM::ubor8, this);
        m_Operators[(uint8_t)OpCode::UBOR16] = std::bind(&VM::ubor16, this);
        m_Operators[(uint8_t)OpCode::UBOR32] = std::bind(&VM::ubor32, this);
        m_Operators[(uint8_t)OpCode::UBOR64] = std::bind(&VM::ubor64, this);

        m_Operators[(uint8_t)OpCode::UXOR8] = std::bind(&VM::uxor8, this);
        m_Operators[(uint8_t)OpCode::UXOR16] = std::bind(&VM::uxor16, this);
        m_Operators[(uint8_t)OpCode::UXOR32] = std::bind(&VM::uxor32, this);
        m_Operators[(uint8_t)OpCode::UXOR64] = std::bind(&VM::uxor64, this);

        m_Operators[(uint8_t)OpCode::UCML8] = std::bind(&VM::ucml8, this);
        m_Operators[(uint8_t)OpCode::UCML16] = std::bind(&VM::ucml16, this);
        m_Operators[(uint8_t)OpCode::UCML32] = std::bind(&VM::ucml32, this);
        m_Operators[(uint8_t)OpCode::UCML64] = std::bind(&VM::ucml64, this);

        m_Operators[(uint8_t)OpCode::UNOT8] = std::bind(&VM::unot8, this);
        m_Operators[(uint8_t)OpCode::UNOT16] = std::bind(&VM::unot16, this);
        m_Operators[(uint8_t)OpCode::UNOT32] = std::bind(&VM::unot32, this);
        m_Operators[(uint8_t)OpCode::UNOT64] = std::bind(&VM::unot64, this);

        m_Operators[(uint8_t)OpCode::UGRT8] = std::bind(&VM::ugrt8, this);
        m_Operators[(uint8_t)OpCode::UGRT16] = std::bind(&VM::ugrt16, this);
        m_Operators[(uint8_t)OpCode::UGRT32] = std::bind(&VM::ugrt32, this);
        m_Operators[(uint8_t)OpCode::UGRT64] = std::bind(&VM::ugrt64, this);

        m_Operators[(uint8_t)OpCode::ULES8] = std::bind(&VM::ules8, this);
        m_Operators[(uint8_t)OpCode::ULES16] = std::bind(&VM::ules16, this);
        m_Operators[(uint8_t)OpCode::ULES32] = std::bind(&VM::ules32, this);
        m_Operators[(uint8_t)OpCode::ULES64] = std::bind(&VM::ules64, this);

        m_Operators[(uint8_t)OpCode::UGRE8] = std::bind(&VM::ugre8, this);
        m_Operators[(uint8_t)OpCode::UGRE16] = std::bind(&VM::ugre16, this);
        m_Operators[(uint8_t)OpCode::UGRE32] = std::bind(&VM::ugre32, this);
        m_Operators[(uint8_t)OpCode::UGRE64] = std::bind(&VM::ugre64, this);

        m_Operators[(uint8_t)OpCode::ULSE8] = std::bind(&VM::ulse8, this);
        m_Operators[(uint8_t)OpCode::ULSE16] = std::bind(&VM::ulse16, this);
        m_Operators[(uint8_t)OpCode::ULSE32] = std::bind(&VM::ulse32, this);
        m_Operators[(uint8_t)OpCode::ULSE64] = std::bind(&VM::ulse64, this);

        m_Operators[(uint8_t)OpCode::UISE8] = std::bind(&VM::uise8, this);
        m_Operators[(uint8_t)OpCode::UISE16] = std::bind(&VM::uise16, this);
        m_Operators[(uint8_t)OpCode::UISE32] = std::bind(&VM::uise32, this);
        m_Operators[(uint8_t)OpCode::UISE64] = std::bind(&VM::uise64, this);

        m_Operators[(uint8_t)OpCode::UNEQ8] = std::bind(&VM::uneq8, this);
        m_Operators[(uint8_t)OpCode::UNEQ16] = std::bind(&VM::uneq16, this);
        m_Operators[(uint8_t)OpCode::UNEQ32] = std::bind(&VM::uneq32, this);
        m_Operators[(uint8_t)OpCode::UNEQ64] = std::bind(&VM::uneq64, this);

        m_Operators[(uint8_t)OpCode::LND] = std::bind(&VM::lnd, this);
        m_Operators[(uint8_t)OpCode::LOR] = std::bind(&VM::lor, this);

        m_Operators[(uint8_t)OpCode::UMOV8] = std::bind(&VM::umov8, this);
        m_Operators[(uint8_t)OpCode::UMOV16] = std::bind(&VM::umov16, this);
        m_Operators[(uint8_t)OpCode::UMOV32] = std::bind(&VM::umov32, this);
        m_Operators[(uint8_t)OpCode::UMOV64] = std::bind(&VM::umov64, this);

        m_Operators[(uint8_t)OpCode::UPSH8] = std::bind(&VM::upsh8, this);
        m_Operators[(uint8_t)OpCode::UPSH16] = std::bind(&VM::upsh16, this);
        m_Operators[(uint8_t)OpCode::UPSH32] = std::bind(&VM::upsh32, this);
        m_Operators[(uint8_t)OpCode::UPSH64] = std::bind(&VM::upsh64, this);
        
        m_Operators[(uint8_t)OpCode::UPOP8] = std::bind(&VM::upop8, this);
        m_Operators[(uint8_t)OpCode::UPOP16] = std::bind(&VM::upop16, this);
        m_Operators[(uint8_t)OpCode::UPOP32] = std::bind(&VM::upop32, this);
        m_Operators[(uint8_t)OpCode::UPOP64] = std::bind(&VM::upop64, this);

        m_Operators[(uint8_t)OpCode::JMT] = std::bind(&VM::jmt, this);
        m_Operators[(uint8_t)OpCode::JMF] = std::bind(&VM::jmf, this);

        m_Operators[(uint8_t)OpCode::CALL] = std::bind(&VM::call, this);
    }

    define(`BINFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.$3 = r0.$3 $2 r1.$3;
    }')

    define(`UNFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.$3 = $2 r0.$3;
    }')

    define(`BINCMPFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.$3 $2 r1.$3;
    }')

    define(`UNCMPFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT]] = $2 r0.$3;
    }')

    define(`MOVFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        uint8_t bytes[$3];
        for (int i = 0; i < $3; i++)
        {
            bytes[i] = m_Code[++m_IP];
        }
        r0.$4 = *($2 *)bytes;
    }')

    define(`PSHFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        push((uint8_t *)&r0.$3, $2);
    }')

    define(`POPFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
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

    void VM::lnd()
    {
        m_Cmp[0] = m_Cmp[0] && m_Cmp[1];
    }

    void VM::lor()
    {
        m_Cmp[0] = m_Cmp[0] || m_Cmp[1];
    }

    MOVFUN(umov8, uint8_t, 1, u8)

    MOVFUN(umov16, uint16_t, 2, u16)
    
    MOVFUN(umov32, uint32_t, 4, u32)
    
    MOVFUN(umov64, uint64_t, 8, u64)

    PSHFUN(upsh8, 1, u8)
    
    PSHFUN(upsh16, 2, u16)
    
    PSHFUN(upsh32, 4, u32)
    
    PSHFUN(upsh64, 8, u64)

    POPFUN(upop8, uint8_t, 1, u8)
    
    POPFUN(upop16, uint16_t, 2, u16)
    
    POPFUN(upop32, uint32_t, 4, u32)
    
    POPFUN(upop64, uint64_t, 8, u64)

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
    
    Register & VM::getRegister(RegisterType::RegisterType type, uint64_t offset)
    {
        if (type == RegisterType::SP)
        {
            return *(Register *)&m_Stack[m_SP - offset];
        }

        return m_Registers[type];
    }

    void VM::push(uint8_t * data, uint8_t size)
    {
        for (int i = 0; i < size; i++)
        {
            m_SP++;
            m_Stack[m_SP - 1] = data[i];
        }
    }

    uint8_t * VM::pop(uint8_t size)
    {
        m_SP -= size;

        return &m_Stack[m_SP];
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
