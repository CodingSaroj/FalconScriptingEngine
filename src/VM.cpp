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

    

    

    

    

    

    

    

    void VM::uadd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 + r1.u8;
    }

    void VM::uadd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 + r1.u16;
    }
    
    void VM::uadd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 + r1.u32;
    }
    
    void VM::uadd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 + r1.u64;
    }

    void VM::usub8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 - r1.u8;
    }

    void VM::usub16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 - r1.u16;
    }
    
    void VM::usub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 - r1.u32;
    }
    
    void VM::usub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 - r1.u64;
    }

    void VM::umul8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 * r1.u8;
    }

    void VM::umul16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 * r1.u16;
    }
    
    void VM::umul32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 * r1.u32;
    }
    
    void VM::umul64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 * r1.u64;
    }

    void VM::udiv8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 / r1.u8;
    }

    void VM::udiv16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 / r1.u16;
    }
    
    void VM::udiv32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 / r1.u32;
    }
    
    void VM::udiv64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 / r1.u64;
    }

    void VM::umod8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 % r1.u8;
    }

    void VM::umod16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 % r1.u16;
    }
    
    void VM::umod32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 % r1.u32;
    }
    
    void VM::umod64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 % r1.u64;
    }

    void VM::uinc8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u8 = ++ r0.u8;
    }

    void VM::uinc16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u16 = ++ r0.u16;
    }
    
    void VM::uinc32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u32 = ++ r0.u32;
    }
    
    void VM::uinc64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u64 = ++ r0.u64;
    }

    void VM::udec8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u8 = -- r0.u8;
    }

    void VM::udec16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u16 = -- r0.u16;
    }
    
    void VM::udec32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u32 = -- r0.u32;
    }
    
    void VM::udec64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u64 = -- r0.u64;
    }

    void VM::ulst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 << r1.u8;
    }

    void VM::ulst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 << r1.u16;
    }
    
    void VM::ulst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 << r1.u32;
    }
    
    void VM::ulst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 << r1.u64;
    }

    void VM::urst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 >> r1.u8;
    }

    void VM::urst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 >> r1.u16;
    }
    
    void VM::urst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 >> r1.u32;
    }
    
    void VM::urst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 >> r1.u64;
    }

    void VM::ubnd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 & r1.u8;
    }

    void VM::ubnd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 & r1.u16;
    }
    
    void VM::ubnd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 & r1.u32;
    }
    
    void VM::ubnd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 & r1.u64;
    }

    void VM::ubor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 | r1.u8;
    }

    void VM::ubor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 | r1.u16;
    }
    
    void VM::ubor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 | r1.u32;
    }
    
    void VM::ubor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 | r1.u64;
    }

    void VM::uxor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u8 = r0.u8 ^ r1.u8;
    }

    void VM::uxor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u16 = r0.u16 ^ r1.u16;
    }
    
    void VM::uxor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u32 = r0.u32 ^ r1.u32;
    }
    
    void VM::uxor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        r0.u64 = r0.u64 ^ r1.u64;
    }

    void VM::ucml8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u8 = ~ r0.u8;
    }

    void VM::ucml16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u16 = ~ r0.u16;
    }
    
    void VM::ucml32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u32 = ~ r0.u32;
    }
    
    void VM::ucml64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u64 = ~ r0.u64;
    }

    void VM::unot8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u8 = ! r0.u8;
    }

    void VM::unot16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u16 = ! r0.u16;
    }
    
    void VM::unot32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u32 = ! r0.u32;
    }
    
    void VM::unot64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u64 = ! r0.u64;
    }

    void VM::ugrt8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u8 > r1.u8;
    }

    void VM::ugrt16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u16 > r1.u16;
    }
    
    void VM::ugrt32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u32 > r1.u32;
    }
    
    void VM::ugrt64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u64 > r1.u64;
    }

    void VM::ules8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u8 < r1.u8;
    }

    void VM::ules16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u16 < r1.u16;
    }
    
    void VM::ules32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u32 < r1.u32;
    }
    
    void VM::ules64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u64 < r1.u64;
    }

    void VM::ugre8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u8 >= r1.u8;
    }

    void VM::ugre16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u16 >= r1.u16;
    }
    
    void VM::ugre32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u32 >= r1.u32;
    }
    
    void VM::ugre64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u64 >= r1.u64;
    }

    void VM::ulse8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u8 <= r1.u8;
    }

    void VM::ulse16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u16 <= r1.u16;
    }
    
    void VM::ulse32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u32 <= r1.u32;
    }
    
    void VM::ulse64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u64 <= r1.u64;
    }

    void VM::uise8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u8 == r1.u8;
    }

    void VM::uise16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u16 == r1.u16;
    }
    
    void VM::uise32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u32 == r1.u32;
    }
    
    void VM::uise64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u64 == r1.u64;
    }

    void VM::uneq8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u8 != r1.u8;
    }

    void VM::uneq16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u16 != r1.u16;
    }
    
    void VM::uneq32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u32 != r1.u32;
    }
    
    void VM::uneq64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO1].u64);
        m_Cmp[(uint8_t)m_Registers[(uint8_t)RegisterType::RegisterType::CMT].u8] = r0.u64 != r1.u64;
    }

    void VM::lnd()
    {
        m_Cmp[0] = m_Cmp[0] && m_Cmp[1];
    }

    void VM::lor()
    {
        m_Cmp[0] = m_Cmp[0] || m_Cmp[1];
    }

    void VM::umov8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        uint8_t bytes[1];
        for (int i = 0; i < 1; i++)
        {
            bytes[i] = m_Code[++m_IP];
        }
        r0.u8 = *(uint8_t *)bytes;
    }

    void VM::umov16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        uint8_t bytes[2];
        for (int i = 0; i < 2; i++)
        {
            bytes[i] = m_Code[++m_IP];
        }
        r0.u16 = *(uint16_t *)bytes;
    }
    
    void VM::umov32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        uint8_t bytes[4];
        for (int i = 0; i < 4; i++)
        {
            bytes[i] = m_Code[++m_IP];
        }
        r0.u32 = *(uint32_t *)bytes;
    }
    
    void VM::umov64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        uint8_t bytes[8];
        for (int i = 0; i < 8; i++)
        {
            bytes[i] = m_Code[++m_IP];
        }
        r0.u64 = *(uint64_t *)bytes;
    }

    void VM::upsh8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        push((uint8_t *)&r0.u8, 1);
    }
    
    void VM::upsh16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        push((uint8_t *)&r0.u16, 2);
    }
    
    void VM::upsh32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        push((uint8_t *)&r0.u32, 4);
    }
    
    void VM::upsh64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        push((uint8_t *)&r0.u64, 8);
    }

    void VM::upop8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u8 = *(uint8_t *)pop(1);
    }
    
    void VM::upop16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u16 = *(uint16_t *)pop(2);
    }
    
    void VM::upop32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u32 = *(uint32_t *)pop(4);
    }
    
    void VM::upop64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], m_Registers[RegisterType::AO0].u64);
        r0.u64 = *(uint64_t *)pop(8);
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
