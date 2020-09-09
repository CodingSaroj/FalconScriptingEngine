#include "VM.hpp"

namespace Falcon
{
    VM::VM(uint8_t * code)
        : m_Code(code), m_IP(0), m_SP(0), m_FP(0), m_InstructionStart(0)
    {
        m_Operators[OpCode::UADD8] = &VM::uadd8;
        m_Operators[OpCode::UADD16] = &VM::uadd16;
        m_Operators[OpCode::UADD32] = &VM::uadd32;
        m_Operators[OpCode::UADD64] = &VM::uadd64;

        m_Operators[OpCode::USUB8] = &VM::usub8;
        m_Operators[OpCode::USUB16] = &VM::usub16;
        m_Operators[OpCode::USUB32] = &VM::usub32;
        m_Operators[OpCode::USUB64] = &VM::usub64;

        m_Operators[OpCode::UMUL8] = &VM::umul8;
        m_Operators[OpCode::UMUL16] = &VM::umul16;
        m_Operators[OpCode::UMUL32] = &VM::umul32;
        m_Operators[OpCode::UMUL64] = &VM::umul64;

        m_Operators[OpCode::UDIV8] = &VM::udiv8;
        m_Operators[OpCode::UDIV16] = &VM::udiv16;
        m_Operators[OpCode::UDIV32] = &VM::udiv32;
        m_Operators[OpCode::UDIV64] = &VM::udiv64;

        m_Operators[OpCode::UMOD8] = &VM::umod8;
        m_Operators[OpCode::UMOD16] = &VM::umod16;
        m_Operators[OpCode::UMOD32] = &VM::umod32;
        m_Operators[OpCode::UMOD64] = &VM::umod64;

        m_Operators[OpCode::UINC8] = &VM::uinc8;
        m_Operators[OpCode::UINC16] = &VM::uinc16;
        m_Operators[OpCode::UINC32] = &VM::uinc32;
        m_Operators[OpCode::UINC64] = &VM::uinc64;

        m_Operators[OpCode::UDEC8] = &VM::udec8;
        m_Operators[OpCode::UDEC16] = &VM::udec16;
        m_Operators[OpCode::UDEC32] = &VM::udec32;
        m_Operators[OpCode::UDEC64] = &VM::udec64;

        m_Operators[OpCode::ULST8] = &VM::ulst8;
        m_Operators[OpCode::ULST16] = &VM::ulst16;
        m_Operators[OpCode::ULST32] = &VM::ulst32;
        m_Operators[OpCode::ULST64] = &VM::ulst64;

        m_Operators[OpCode::URST8] = &VM::urst8;
        m_Operators[OpCode::URST16] = &VM::urst16;
        m_Operators[OpCode::URST32] = &VM::urst32;
        m_Operators[OpCode::URST64] = &VM::urst64;

        m_Operators[OpCode::UBND8] = &VM::ubnd8;
        m_Operators[OpCode::UBND16] = &VM::ubnd16;
        m_Operators[OpCode::UBND32] = &VM::ubnd32;
        m_Operators[OpCode::UBND64] = &VM::ubnd64;

        m_Operators[OpCode::UBOR8] = &VM::ubor8;
        m_Operators[OpCode::UBOR16] = &VM::ubor16;
        m_Operators[OpCode::UBOR32] = &VM::ubor32;
        m_Operators[OpCode::UBOR64] = &VM::ubor64;

        m_Operators[OpCode::UXOR8] = &VM::uxor8;
        m_Operators[OpCode::UXOR16] = &VM::uxor16;
        m_Operators[OpCode::UXOR32] = &VM::uxor32;
        m_Operators[OpCode::UXOR64] = &VM::uxor64;

        m_Operators[OpCode::UCML8] = &VM::ucml8;
        m_Operators[OpCode::UCML16] = &VM::ucml16;
        m_Operators[OpCode::UCML32] = &VM::ucml32;
        m_Operators[OpCode::UCML64] = &VM::ucml64;

        m_Operators[OpCode::UGRT8] = &VM::ugrt8;
        m_Operators[OpCode::UGRT16] = &VM::ugrt16;
        m_Operators[OpCode::UGRT32] = &VM::ugrt32;
        m_Operators[OpCode::UGRT64] = &VM::ugrt64;

        m_Operators[OpCode::ULES8] = &VM::ules8;
        m_Operators[OpCode::ULES16] = &VM::ules16;
        m_Operators[OpCode::ULES32] = &VM::ules32;
        m_Operators[OpCode::ULES64] = &VM::ules64;

        m_Operators[OpCode::UGRE8] = &VM::ugre8;
        m_Operators[OpCode::UGRE16] = &VM::ugre16;
        m_Operators[OpCode::UGRE32] = &VM::ugre32;
        m_Operators[OpCode::UGRE64] = &VM::ugre64;

        m_Operators[OpCode::ULSE8] = &VM::ulse8;
        m_Operators[OpCode::ULSE16] = &VM::ulse16;
        m_Operators[OpCode::ULSE32] = &VM::ulse32;
        m_Operators[OpCode::ULSE64] = &VM::ulse64;

        m_Operators[OpCode::UISE8] = &VM::uise8;
        m_Operators[OpCode::UISE16] = &VM::uise16;
        m_Operators[OpCode::UISE32] = &VM::uise32;
        m_Operators[OpCode::UISE64] = &VM::uise64;

        m_Operators[OpCode::UNEQ8] = &VM::uneq8;
        m_Operators[OpCode::UNEQ16] = &VM::uneq16;
        m_Operators[OpCode::UNEQ32] = &VM::uneq32;
        m_Operators[OpCode::UNEQ64] = &VM::uneq64;

        m_Operators[OpCode::IADD8] = &VM::iadd8;
        m_Operators[OpCode::IADD16] = &VM::iadd16;
        m_Operators[OpCode::IADD32] = &VM::iadd32;
        m_Operators[OpCode::IADD64] = &VM::iadd64;

        m_Operators[OpCode::ISUB8] = &VM::isub8;
        m_Operators[OpCode::ISUB16] = &VM::isub16;
        m_Operators[OpCode::ISUB32] = &VM::isub32;
        m_Operators[OpCode::ISUB64] = &VM::isub64;

        m_Operators[OpCode::IMUL8] = &VM::imul8;
        m_Operators[OpCode::IMUL16] = &VM::imul16;
        m_Operators[OpCode::IMUL32] = &VM::imul32;
        m_Operators[OpCode::IMUL64] = &VM::imul64;

        m_Operators[OpCode::IDIV8] = &VM::idiv8;
        m_Operators[OpCode::IDIV16] = &VM::idiv16;
        m_Operators[OpCode::IDIV32] = &VM::idiv32;
        m_Operators[OpCode::IDIV64] = &VM::idiv64;

        m_Operators[OpCode::IMOD8] = &VM::imod8;
        m_Operators[OpCode::IMOD16] = &VM::imod16;
        m_Operators[OpCode::IMOD32] = &VM::imod32;
        m_Operators[OpCode::IMOD64] = &VM::imod64;

        m_Operators[OpCode::IINC8] = &VM::iinc8;
        m_Operators[OpCode::IINC16] = &VM::iinc16;
        m_Operators[OpCode::IINC32] = &VM::iinc32;
        m_Operators[OpCode::IINC64] = &VM::iinc64;

        m_Operators[OpCode::IDEC8] = &VM::idec8;
        m_Operators[OpCode::IDEC16] = &VM::idec16;
        m_Operators[OpCode::IDEC32] = &VM::idec32;
        m_Operators[OpCode::IDEC64] = &VM::idec64;

        m_Operators[OpCode::ILST8] = &VM::ilst8;
        m_Operators[OpCode::ILST16] = &VM::ilst16;
        m_Operators[OpCode::ILST32] = &VM::ilst32;
        m_Operators[OpCode::ILST64] = &VM::ilst64;

        m_Operators[OpCode::IRST8] = &VM::irst8;
        m_Operators[OpCode::IRST16] = &VM::irst16;
        m_Operators[OpCode::IRST32] = &VM::irst32;
        m_Operators[OpCode::IRST64] = &VM::irst64;

        m_Operators[OpCode::IBND8] = &VM::ibnd8;
        m_Operators[OpCode::IBND16] = &VM::ibnd16;
        m_Operators[OpCode::IBND32] = &VM::ibnd32;
        m_Operators[OpCode::IBND64] = &VM::ibnd64;

        m_Operators[OpCode::IBOR8] = &VM::ibor8;
        m_Operators[OpCode::IBOR16] = &VM::ibor16;
        m_Operators[OpCode::IBOR32] = &VM::ibor32;
        m_Operators[OpCode::IBOR64] = &VM::ibor64;

        m_Operators[OpCode::IXOR8] = &VM::ixor8;
        m_Operators[OpCode::IXOR16] = &VM::ixor16;
        m_Operators[OpCode::IXOR32] = &VM::ixor32;
        m_Operators[OpCode::IXOR64] = &VM::ixor64;

        m_Operators[OpCode::ICML8] = &VM::icml8;
        m_Operators[OpCode::ICML16] = &VM::icml16;
        m_Operators[OpCode::ICML32] = &VM::icml32;
        m_Operators[OpCode::ICML64] = &VM::icml64;

        m_Operators[OpCode::IGRT8] = &VM::igrt8;
        m_Operators[OpCode::IGRT16] = &VM::igrt16;
        m_Operators[OpCode::IGRT32] = &VM::igrt32;
        m_Operators[OpCode::IGRT64] = &VM::igrt64;

        m_Operators[OpCode::ILES8] = &VM::iles8;
        m_Operators[OpCode::ILES16] = &VM::iles16;
        m_Operators[OpCode::ILES32] = &VM::iles32;
        m_Operators[OpCode::ILES64] = &VM::iles64;

        m_Operators[OpCode::IGRE8] = &VM::igre8;
        m_Operators[OpCode::IGRE16] = &VM::igre16;
        m_Operators[OpCode::IGRE32] = &VM::igre32;
        m_Operators[OpCode::IGRE64] = &VM::igre64;

        m_Operators[OpCode::ILSE8] = &VM::ilse8;
        m_Operators[OpCode::ILSE16] = &VM::ilse16;
        m_Operators[OpCode::ILSE32] = &VM::ilse32;
        m_Operators[OpCode::ILSE64] = &VM::ilse64;

        m_Operators[OpCode::IISE8] = &VM::iise8;
        m_Operators[OpCode::IISE16] = &VM::iise16;
        m_Operators[OpCode::IISE32] = &VM::iise32;
        m_Operators[OpCode::IISE64] = &VM::iise64;

        m_Operators[OpCode::INEQ8] = &VM::ineq8;
        m_Operators[OpCode::INEQ16] = &VM::ineq16;
        m_Operators[OpCode::INEQ32] = &VM::ineq32;
        m_Operators[OpCode::INEQ64] = &VM::ineq64;

        m_Operators[OpCode::FADD32] = &VM::fadd32;
        m_Operators[OpCode::FADD64] = &VM::fadd64;

        m_Operators[OpCode::FSUB32] = &VM::fsub32;
        m_Operators[OpCode::FSUB64] = &VM::fsub64;

        m_Operators[OpCode::FMUL32] = &VM::fmul32;
        m_Operators[OpCode::FMUL64] = &VM::fmul64;

        m_Operators[OpCode::FDIV32] = &VM::fdiv32;
        m_Operators[OpCode::FDIV64] = &VM::fdiv64;

        m_Operators[OpCode::FINC32] = &VM::finc32;
        m_Operators[OpCode::FINC64] = &VM::finc64;

        m_Operators[OpCode::FDEC32] = &VM::fdec32;
        m_Operators[OpCode::FDEC64] = &VM::fdec64;

        m_Operators[OpCode::FGRT32] = &VM::fgrt32;
        m_Operators[OpCode::FGRT64] = &VM::fgrt64;

        m_Operators[OpCode::FLES32] = &VM::fles32;
        m_Operators[OpCode::FLES64] = &VM::fles64;

        m_Operators[OpCode::FGRE32] = &VM::fgre32;
        m_Operators[OpCode::FGRE64] = &VM::fgre64;

        m_Operators[OpCode::FLSE32] = &VM::flse32;
        m_Operators[OpCode::FLSE64] = &VM::flse64;

        m_Operators[OpCode::FISE32] = &VM::fise32;
        m_Operators[OpCode::FISE64] = &VM::fise64;

        m_Operators[OpCode::FNEQ32] = &VM::fneq32;
        m_Operators[OpCode::FNEQ64] = &VM::fneq64;
        
        m_Operators[OpCode::MOV8] = &VM::mov8;
        m_Operators[OpCode::MOV16] = &VM::mov16;
        m_Operators[OpCode::MOV32] = &VM::mov32;
        m_Operators[OpCode::MOV64] = &VM::mov64;
         
        m_Operators[OpCode::MVR8] = &VM::mvr8;
        m_Operators[OpCode::MVR16] = &VM::mvr16;
        m_Operators[OpCode::MVR32] = &VM::mvr32;
        m_Operators[OpCode::MVR64] = &VM::mvr64;

        m_Operators[OpCode::PSH8] = &VM::psh8;
        m_Operators[OpCode::PSH16] = &VM::psh16;
        m_Operators[OpCode::PSH32] = &VM::psh32;
        m_Operators[OpCode::PSH64] = &VM::psh64;
         
        m_Operators[OpCode::POP8] = &VM::pop8;
        m_Operators[OpCode::POP16] = &VM::pop16;
        m_Operators[OpCode::POP32] = &VM::pop32;
        m_Operators[OpCode::POP64] = &VM::pop64;

        m_Operators[OpCode::PSHSTR] = &VM::pshstr;
 
        m_Operators[OpCode::LND] = &VM::lnd;
        m_Operators[OpCode::LOR] = &VM::lor;
        m_Operators[OpCode::NOT] = &VM::lnot;

        m_Operators[OpCode::LOAD8] = &VM::load8;
        m_Operators[OpCode::LOAD16] = &VM::load16;
        m_Operators[OpCode::LOAD32] = &VM::load32;
        m_Operators[OpCode::LOAD64] = &VM::load64;

        m_Operators[OpCode::LODREF] = &VM::lodref;

        m_Operators[OpCode::PSHNUL] = &VM::pshnul;
        m_Operators[OpCode::POPNUL] = &VM::popnul;

        m_Operators[OpCode::JMP] = &VM::jmp;
        m_Operators[OpCode::JMT] = &VM::jmt;
        m_Operators[OpCode::JMF] = &VM::jmf;

        m_Operators[OpCode::CALL] = &VM::call;
        m_Operators[OpCode::EXTRN] = &VM::extrn;
        m_Operators[OpCode::RET] = &VM::ret;
        m_Operators[OpCode::FUNC] = &VM::stop;
        m_Operators[OpCode::STOP] = &VM::stop;

        collectSymbols();
    }

    void VM::stop()
    {
        m_Running = false;
    }

    void VM::collectSymbols()
    {
        OpCode::OpCode op = (OpCode::OpCode)m_Code[m_IP];

        while (op != OpCode::FUNC)
        {
            std::string name;
            uint64_t location;

            while (char c = (char)m_Code[m_IP++])
            {
                name += c;
            }

            location = *(uint64_t *)&m_Code[m_IP];

            m_IP += 8;

            m_Functions[name] = location;

            op = (OpCode::OpCode)m_Code[m_IP];
        }

        m_Code += m_IP;
    }

    void VM::dumpState()
    {
        std::cout<<(Common::Colors::Yellow | Common::Colors::Bold)<<"State Dump"<<Common::Colors::White<<":\n";
        std::cout<<"    Instruction              : "<<Disassembler::DisassembleInstruction(&m_Code[m_InstructionStart])<<"\n";
        std::cout<<std::hex<<std::setfill('0');
        std::cout<<"    Instruction Pointer (IP) : 0x"<<std::setw(16)<<m_IP<<"\n";
        std::cout<<"    Stack Pointer (SP)       : 0x"<<std::setw(16)<<m_SP<<"\n";
        std::cout<<"    Frame Pointer (FP)       : 0x"<<std::setw(16)<<m_FP<<"\n\n";
        
        for (uint16_t i = 0; i < 4; i++)
        {
            auto reg = m_Registers[i];

            std::cout<<std::setfill('0');

            std::cout<<"    R"<<i + 1<<":\n";

            std::cout<<"        u8  : 0x"<<std::setw(2)<<(uint16_t)reg.u8<<" '"<<reg.u8<<"'                   i8  : 0x"<<std::setw(2)<<(int16_t)reg.i8<<" '"<<reg.i8<<"' \n";

            std::cout<<"        u16 : 0x"<<std::setw(4)<<reg.u16<<"                    i16 : 0x"<<std::setw(4)<<reg.i16<<" \n";

            std::cout<<"        u32 : 0x"<<std::setw(8)<<reg.u32<<"                i32 : 0x"<<std::setw(8)<<reg.u32<<" \n";

            std::cout<<"        u64 : 0x"<<std::setw(16)<<reg.u64<<"        i64 : 0x"<<std::setw(16)<<reg.u64<<" \n";

            std::cout<<std::setfill(' ');

            std::cout<<std::dec;
            std::cout<<"        f32 : "<<reg.f32<<" \n";
            std::cout<<"        f64 : "<<reg.f64<<" \n";
            std::cout<<std::hex;
        }

        std::cout<<std::dec;
    }

    void VM::uadd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 + r1.u8;
    }

    void VM::uadd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 + r1.u16;
    }
    
    void VM::uadd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 + r1.u32;
    }
    
    void VM::uadd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 + r1.u64;
    }

    void VM::usub8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 - r1.u8;
    }

    void VM::usub16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 - r1.u16;
    }
    
    void VM::usub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 - r1.u32;
    }
    
    void VM::usub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 - r1.u64;
    }

    void VM::umul8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 * r1.u8;
    }

    void VM::umul16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 * r1.u16;
    }
    
    void VM::umul32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 * r1.u32;
    }
    
    void VM::umul64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 * r1.u64;
    }

    void VM::udiv8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 / r1.u8;
    }

    void VM::udiv16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 / r1.u16;
    }
    
    void VM::udiv32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 / r1.u32;
    }
    
    void VM::udiv64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 / r1.u64;
    }

    void VM::umod8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 % r1.u8;
    }

    void VM::umod16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 % r1.u16;
    }
    
    void VM::umod32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 % r1.u32;
    }
    
    void VM::umod64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 % r1.u64;
    }

    void VM::uinc8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = ++ r0.u8;
    }

    void VM::uinc16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = ++ r0.u16;
    }
    
    void VM::uinc32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = ++ r0.u32;
    }
    
    void VM::uinc64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = ++ r0.u64;
    }

    void VM::udec8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = -- r0.u8;
    }

    void VM::udec16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = -- r0.u16;
    }
    
    void VM::udec32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = -- r0.u32;
    }
    
    void VM::udec64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = -- r0.u64;
    }

    void VM::ulst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 << r1.u8;
    }

    void VM::ulst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 << r1.u16;
    }
    
    void VM::ulst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 << r1.u32;
    }
    
    void VM::ulst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 << r1.u64;
    }

    void VM::urst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 >> r1.u8;
    }

    void VM::urst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 >> r1.u16;
    }
    
    void VM::urst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 >> r1.u32;
    }
    
    void VM::urst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 >> r1.u64;
    }

    void VM::ubnd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 & r1.u8;
    }

    void VM::ubnd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 & r1.u16;
    }
    
    void VM::ubnd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 & r1.u32;
    }
    
    void VM::ubnd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 & r1.u64;
    }

    void VM::ubor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 | r1.u8;
    }

    void VM::ubor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 | r1.u16;
    }
    
    void VM::ubor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 | r1.u32;
    }
    
    void VM::ubor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 | r1.u64;
    }

    void VM::uxor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = r0.u8 ^ r1.u8;
    }

    void VM::uxor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = r0.u16 ^ r1.u16;
    }
    
    void VM::uxor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = r0.u32 ^ r1.u32;
    }
    
    void VM::uxor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = r0.u64 ^ r1.u64;
    }

    void VM::ucml8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = ~ r0.u8;
    }

    void VM::ucml16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = ~ r0.u16;
    }
    
    void VM::ucml32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = ~ r0.u32;
    }
    
    void VM::ucml64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = ~ r0.u64;
    }

    void VM::ugrt8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u8 > r1.u8;
    }

    void VM::ugrt16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u16 > r1.u16;
    }
    
    void VM::ugrt32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u32 > r1.u32;
    }
    
    void VM::ugrt64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u64 > r1.u64;
    }

    void VM::ules8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u8 < r1.u8;
    }

    void VM::ules16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u16 < r1.u16;
    }
    
    void VM::ules32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u32 < r1.u32;
    }
    
    void VM::ules64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u64 < r1.u64;
    }

    void VM::ugre8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u8 >= r1.u8;
    }

    void VM::ugre16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u16 >= r1.u16;
    }
    
    void VM::ugre32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u32 >= r1.u32;
    }
    
    void VM::ugre64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u64 >= r1.u64;
    }

    void VM::ulse8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u8 <= r1.u8;
    }

    void VM::ulse16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u16 <= r1.u16;
    }
    
    void VM::ulse32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u32 <= r1.u32;
    }
    
    void VM::ulse64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u64 <= r1.u64;
    }

    void VM::uise8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u8 == r1.u8;
    }

    void VM::uise16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u16 == r1.u16;
    }
    
    void VM::uise32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u32 == r1.u32;
    }
    
    void VM::uise64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u64 == r1.u64;
    }

    void VM::uneq8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u8 != r1.u8;
    }

    void VM::uneq16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u16 != r1.u16;
    }
    
    void VM::uneq32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u32 != r1.u32;
    }
    
    void VM::uneq64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.u64 != r1.u64;
    }

    void VM::iadd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 + r1.i8;
    }

    void VM::iadd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 + r1.i16;
    }
    
    void VM::iadd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 + r1.i32;
    }
    
    void VM::iadd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 + r1.i64;
    }

    void VM::isub8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 - r1.i8;
    }

    void VM::isub16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 - r1.i16;
    }
    
    void VM::isub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 - r1.i32;
    }
    
    void VM::isub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 - r1.i64;
    }

    void VM::imul8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 * r1.i8;
    }

    void VM::imul16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 * r1.i16;
    }
    
    void VM::imul32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 * r1.i32;
    }
    
    void VM::imul64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 * r1.i64;
    }

    void VM::idiv8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 / r1.i8;
    }

    void VM::idiv16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 / r1.i16;
    }
    
    void VM::idiv32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 / r1.i32;
    }
    
    void VM::idiv64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 / r1.i64;
    }

    void VM::imod8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 % r1.i8;
    }

    void VM::imod16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 % r1.i16;
    }
    
    void VM::imod32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 % r1.i32;
    }
    
    void VM::imod64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 % r1.i64;
    }

    void VM::iinc8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = ++ r0.i8;
    }

    void VM::iinc16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = ++ r0.i16;
    }
    
    void VM::iinc32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = ++ r0.i32;
    }
    
    void VM::iinc64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = ++ r0.i64;
    }

    void VM::idec8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = -- r0.i8;
    }

    void VM::idec16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = -- r0.i16;
    }
    
    void VM::idec32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = -- r0.i32;
    }
    
    void VM::idec64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = -- r0.i64;
    }

    void VM::ilst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 << r1.i8;
    }

    void VM::ilst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 << r1.i16;
    }
    
    void VM::ilst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 << r1.i32;
    }
    
    void VM::ilst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 << r1.i64;
    }

    void VM::irst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 >> r1.i8;
    }

    void VM::irst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 >> r1.i16;
    }
    
    void VM::irst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 >> r1.i32;
    }
    
    void VM::irst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 >> r1.i64;
    }

    void VM::ibnd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 & r1.i8;
    }

    void VM::ibnd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 & r1.i16;
    }
    
    void VM::ibnd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 & r1.i32;
    }
    
    void VM::ibnd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 & r1.i64;
    }

    void VM::ibor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 | r1.i8;
    }

    void VM::ibor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 | r1.i16;
    }
    
    void VM::ibor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 | r1.i32;
    }
    
    void VM::ibor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 | r1.i64;
    }

    void VM::ixor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = r0.i8 ^ r1.i8;
    }

    void VM::ixor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = r0.i16 ^ r1.i16;
    }
    
    void VM::ixor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = r0.i32 ^ r1.i32;
    }
    
    void VM::ixor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = r0.i64 ^ r1.i64;
    }

    void VM::icml8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i8 = ~ r0.i8;
    }

    void VM::icml16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i16 = ~ r0.i16;
    }
    
    void VM::icml32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i32 = ~ r0.i32;
    }
    
    void VM::icml64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.i64 = ~ r0.i64;
    }

    void VM::igrt8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i8 > r1.i8;
    }

    void VM::igrt16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i16 > r1.i16;
    }
    
    void VM::igrt32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i32 > r1.i32;
    }
    
    void VM::igrt64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i64 > r1.i64;
    }

    void VM::iles8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i8 < r1.i8;
    }

    void VM::iles16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i16 < r1.i16;
    }
    
    void VM::iles32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i32 < r1.i32;
    }
    
    void VM::iles64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i64 < r1.i64;
    }

    void VM::igre8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i8 >= r1.i8;
    }

    void VM::igre16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i16 >= r1.i16;
    }
    
    void VM::igre32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i32 >= r1.i32;
    }
    
    void VM::igre64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i64 >= r1.i64;
    }

    void VM::ilse8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i8 <= r1.i8;
    }

    void VM::ilse16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i16 <= r1.i16;
    }
    
    void VM::ilse32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i32 <= r1.i32;
    }
    
    void VM::ilse64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i64 <= r1.i64;
    }

    void VM::iise8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i8 == r1.i8;
    }

    void VM::iise16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i16 == r1.i16;
    }
    
    void VM::iise32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i32 == r1.i32;
    }
    
    void VM::iise64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i64 == r1.i64;
    }

    void VM::ineq8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i8 != r1.i8;
    }

    void VM::ineq16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i16 != r1.i16;
    }
    
    void VM::ineq32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i32 != r1.i32;
    }
    
    void VM::ineq64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.i64 != r1.i64;
    } 

    void VM::fadd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f32 = r0.f32 + r1.f32;
    }
    
    void VM::fadd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f64 = r0.f64 + r1.f64;
    }

    void VM::fsub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f32 = r0.f32 - r1.f32;
    }
    
    void VM::fsub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f64 = r0.f64 - r1.f64;
    }

    void VM::fmul32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f32 = r0.f32 * r1.f32;
    }
    
    void VM::fmul64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f64 = r0.f64 * r1.f64;
    }

    void VM::fdiv32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f32 = r0.f32 / r1.f32;
    }
    
    void VM::fdiv64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f64 = r0.f64 / r1.f64;
    }
    
    void VM::finc32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f32 = ++ r0.f32;
    }
    
    void VM::finc64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f64 = ++ r0.f64;
    }

    void VM::fdec32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f32 = -- r0.f32;
    }
    
    void VM::fdec64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.f64 = -- r0.f64;
    }

    void VM::fgrt32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f32 > r1.f32;
    }
    
    void VM::fgrt64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f64 > r1.f64;
    }

    void VM::fles32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f32 < r1.f32;
    }
    
    void VM::fles64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f64 < r1.f64;
    }

    void VM::fgre32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f32 >= r1.f32;
    }
    
    void VM::fgre64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f64 >= r1.f64;
    }

    void VM::flse32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f32 <= r1.f32;
    }
    
    void VM::flse64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f64 <= r1.f64;
    }

    void VM::fise32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f32 == r1.f32;
    }
    
    void VM::fise64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f64 == r1.f64;
    }

    void VM::fneq32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f32 != r1.f32;
    }
    
    void VM::fneq64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        m_Cmp[1] = m_Cmp[0];
        m_Cmp[0] = r0.f64 != r1.f64;
    }

    void VM::mov8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint8_t bytes[1];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 1);
        m_IP += 1;

        r0.u8 = *(uint8_t *)bytes;
    }

    void VM::mov16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint8_t bytes[2];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 2);
        m_IP += 2;

        r0.u16 = *(uint16_t *)bytes;

        Endian::LittleToSystem(r0.u16);
    }
    
    void VM::mov32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint8_t bytes[4];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 4);
        m_IP += 4;

        r0.u32 = *(uint32_t *)bytes;

        Endian::LittleToSystem(r0.u32);
    }
    
    void VM::mov64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint8_t bytes[8];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 8);
        m_IP += 8;

        r0.u64 = *(uint64_t *)bytes;

        Endian::LittleToSystem(r0.u64);
    }

    void VM::mvr8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);

        r0.u8 = r1.u8;
    }

    void VM::mvr16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);

        r0.u16 = r1.u16;
    }
    
    void VM::mvr32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);

        r0.u32 = r1.u32;
    }
    
    void VM::mvr64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);

        r0.u64 = r1.u64;
    }

    void VM::psh8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        push((uint8_t *)&r0.u8, 1);
    }
    
    void VM::psh16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        push((uint8_t *)&r0.u16, 2);
    }
    
    void VM::psh32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        push((uint8_t *)&r0.u32, 4);
    }
    
    void VM::psh64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        push((uint8_t *)&r0.u64, 8);
    }

    void VM::pop8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u8 = *(uint8_t *)pop(1);
    }
    
    void VM::pop16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u16 = *(uint16_t *)pop(2);
    }
    
    void VM::pop32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u32 = *(uint32_t *)pop(4);
    }
    
    void VM::pop64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        r0.u64 = *(uint64_t *)pop(8);
    }
    
    void VM::pshstr()
    {
        char c = '\0';

        while ((c = (char)m_Code[++m_IP]) != '\0')
        {
            push((uint8_t *)&c, 1);
        }

        push((uint8_t *)&c, 1);
    }

    void VM::lnd()
    {
        m_Cmp[0] = m_Cmp[0] && m_Cmp[1];
    }

    void VM::lor()
    {
        m_Cmp[0] = m_Cmp[0] || m_Cmp[1];
    }

    void VM::lnot()
    {
        m_Cmp[0] = ! m_Cmp[0];
    }
    
    void VM::load8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        
        memcpy((uint8_t *)&r0, &m_Stack[m_FP + *(uint32_t *)&m_Code[++m_IP]], 1);
        m_IP += 3;
    }

    void VM::load16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint32_t offset = *(uint32_t *)&m_Code[++m_IP];

        Endian::LittleToSystem(offset);

        m_IP += 3;

        memcpy((uint8_t *)&r0, &m_Stack[m_FP + offset], 2);
    }

    void VM::load32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint32_t offset = *(uint32_t *)&m_Code[++m_IP];

        Endian::LittleToSystem(offset);
        
        m_IP += 3;

        memcpy((uint8_t *)&r0, &m_Stack[m_FP + offset], 4);
    }

    void VM::load64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint32_t offset = *(uint32_t *)&m_Code[++m_IP];

        Endian::LittleToSystem(offset);
        
        m_IP += 3;

        memcpy((uint8_t *)&r0, &m_Stack[m_FP + offset], 8);
    }

    void VM::lodref()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP]);
        uint32_t offset = *(uint32_t *)&m_Code[++m_IP];

        Endian::LittleToSystem(offset);

        uint64_t address = (uint64_t)&m_Stack[m_FP + offset];

        m_IP += 3;

        memcpy((uint8_t *)&r0, &address, 8);
    }

    void VM::pshnul()
    {
        uint32_t size = *(uint32_t *)&m_Code[++m_IP];

        Endian::LittleToSystem(size);

        m_SP += size;

        m_IP += 3;
    }

    void VM::popnul()
    {
        uint32_t size = *(uint32_t *)&m_Code[++m_IP];
        pop(size);

        m_IP += 3;
    }

    void VM::jmp()
    {
        m_IP = *(uint64_t *)&m_Code[++m_IP];

        Endian::LittleToSystem(m_IP);

        m_IP--;
    }

    void VM::jmt()
    {
        if (m_Cmp[0])
        {
            jmp();
        }
        else
        {
            m_IP += 8;
        }
    }

    void VM::jmf()
    {
        if (!m_Cmp[0])
        {
            jmp();
        }
        else
        {
            m_IP += 8;
        }
    }

    void VM::call()
    {
        uint32_t argsSize = *(uint32_t *)&m_Code[++m_IP];

        Endian::LittleToSystem(argsSize);

        m_IP += 3;

        uint8_t * args = new uint8_t[argsSize];

        memcpy(args, pop(argsSize), argsSize);

        std::string name;

        while (char c = (char)m_Code[++m_IP])
        {
            name += c;
        }

        uint64_t returnIP = m_IP;

        push((uint8_t *)&m_SP, 8);
        push((uint8_t *)&returnIP, 8);
        push((uint8_t *)&m_FP, 8);

        m_FP = m_SP;

        push(args, argsSize);

        delete[] args;

        if (m_Functions.count(name) != 0)
        {
            m_IP = m_Functions[name]; 
        }
    }

    void VM::extrn()
    {
        std::string name;

        while (char c = (char)m_Code[++m_IP])
        {
            name += c;
        }
        
        if (m_ExternalFunctions.count(name) != 0)
        {
            m_ExternalFunctions[name](*this);
        }
    }

    void VM::ret()
    {
        uint64_t retSize = *(uint32_t *)&m_Code[++m_IP];
        
        Endian::LittleToSystem(retSize);

        uint8_t * retData = pop(retSize);

        m_SP = m_FP;

        m_FP = *(uint64_t *)pop(8);
        m_IP = *(uint64_t *)pop(8);
        m_SP = *(uint64_t *)pop(8);

        push(retData, retSize);

        if (m_FP == 0)
        {
            m_Running = false;
        }
    }
    
    void VM::push(uint8_t * data, uint64_t size)
    {
        memcpy(&m_Stack[m_SP], data, size);
        m_SP += size;
    }

    uint8_t * VM::pop(uint64_t size)
    {
        m_SP -= size;

        return &m_Stack[m_SP];
    }

    std::unordered_map<int, std::function<void(int)>> VM::getSignalHandlers()
    {
        return
        {
            {
                SIGABRT,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Aborted.\n";
                    dumpState();
                    exit(SIGABRT);
                }
            },
            {
                SIGILL,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Illegal CPU Instruction.\n";
                    dumpState();
                    exit(SIGILL);
                }
            },
            {
                SIGINT,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Interrupted.\n";
                    m_Running = false;
                }
            },
            {
                SIGFPE,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Floating point exception.\n";
                    dumpState();
                    exit(SIGFPE);
                }
            },
            {
                SIGSEGV,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Segmentation fault.\n";
                    dumpState();
                    exit(SIGSEGV);
                }
            },
            {
                SIGTERM,
                [this](int signal)
                {
                    std::cout<<(Common::Colors::Red | Common::Colors::Bold)<<"Runtime Error:"<<Common::Colors::White<<" Terminating.\n";
                    m_Running = false;
                }
            }
        };
    }

    void VM::externalFunction(const std::string & name, std::function<void(VM&)> function)
    {
        if (m_ExternalFunctions.count(name) == 0)
        {
            m_ExternalFunctions[name] = function;
        }
    }

    void VM::run(std::string function, uint64_t argsSize)
    {
        m_IP = m_Functions[function] + 1;

        uint8_t * args = new uint8_t[argsSize];

        memcpy(args, pop(argsSize), argsSize);

        uint64_t returnIP = 0;

        push((uint8_t *)&m_SP, 8);
        push((uint8_t *)&returnIP, 8);
        push((uint8_t *)&m_FP, 8);

        m_FP = m_SP;

        push(args, argsSize);

        delete[] args;

        m_Running = true;
        OpCode::OpCode op = (OpCode::OpCode)m_Code[m_IP];

        while (m_Running)
        {
            m_InstructionStart = m_IP;

            (this->*m_Operators[(uint8_t)op])();

            op = (OpCode::OpCode)m_Code[++m_IP];
        }
    }
}
