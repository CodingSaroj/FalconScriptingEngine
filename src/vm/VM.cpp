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
        
        m_Operators[OpCode::IADD8] = std::bind(&VM::iadd8, this);
        m_Operators[OpCode::IADD16] = std::bind(&VM::iadd16, this);
        m_Operators[OpCode::IADD32] = std::bind(&VM::iadd32, this);
        m_Operators[OpCode::IADD64] = std::bind(&VM::iadd64, this);

        m_Operators[OpCode::ISUB8] = std::bind(&VM::isub8, this);
        m_Operators[OpCode::ISUB16] = std::bind(&VM::isub16, this);
        m_Operators[OpCode::ISUB32] = std::bind(&VM::isub32, this);
        m_Operators[OpCode::ISUB64] = std::bind(&VM::isub64, this);

        m_Operators[OpCode::IMUL8] = std::bind(&VM::imul8, this);
        m_Operators[OpCode::IMUL16] = std::bind(&VM::imul16, this);
        m_Operators[OpCode::IMUL32] = std::bind(&VM::imul32, this);
        m_Operators[OpCode::IMUL64] = std::bind(&VM::imul64, this);

        m_Operators[OpCode::IDIV8] = std::bind(&VM::idiv8, this);
        m_Operators[OpCode::IDIV16] = std::bind(&VM::idiv16, this);
        m_Operators[OpCode::IDIV32] = std::bind(&VM::idiv32, this);
        m_Operators[OpCode::IDIV64] = std::bind(&VM::idiv64, this);

        m_Operators[OpCode::IMOD8] = std::bind(&VM::imod8, this);
        m_Operators[OpCode::IMOD16] = std::bind(&VM::imod16, this);
        m_Operators[OpCode::IMOD32] = std::bind(&VM::imod32, this);
        m_Operators[OpCode::IMOD64] = std::bind(&VM::imod64, this);

        m_Operators[OpCode::IINC8] = std::bind(&VM::iinc8, this);
        m_Operators[OpCode::IINC16] = std::bind(&VM::iinc16, this);
        m_Operators[OpCode::IINC32] = std::bind(&VM::iinc32, this);
        m_Operators[OpCode::IINC64] = std::bind(&VM::iinc64, this);

        m_Operators[OpCode::IDEC8] = std::bind(&VM::idec8, this);
        m_Operators[OpCode::IDEC16] = std::bind(&VM::idec16, this);
        m_Operators[OpCode::IDEC32] = std::bind(&VM::idec32, this);
        m_Operators[OpCode::IDEC64] = std::bind(&VM::idec64, this);

        m_Operators[OpCode::ILST8] = std::bind(&VM::ilst8, this);
        m_Operators[OpCode::ILST16] = std::bind(&VM::ilst16, this);
        m_Operators[OpCode::ILST32] = std::bind(&VM::ilst32, this);
        m_Operators[OpCode::ILST64] = std::bind(&VM::ilst64, this);

        m_Operators[OpCode::IRST8] = std::bind(&VM::irst8, this);
        m_Operators[OpCode::IRST16] = std::bind(&VM::irst16, this);
        m_Operators[OpCode::IRST32] = std::bind(&VM::irst32, this);
        m_Operators[OpCode::IRST64] = std::bind(&VM::irst64, this);

        m_Operators[OpCode::IBND8] = std::bind(&VM::ibnd8, this);
        m_Operators[OpCode::IBND16] = std::bind(&VM::ibnd16, this);
        m_Operators[OpCode::IBND32] = std::bind(&VM::ibnd32, this);
        m_Operators[OpCode::IBND64] = std::bind(&VM::ibnd64, this);

        m_Operators[OpCode::IBOR8] = std::bind(&VM::ibor8, this);
        m_Operators[OpCode::IBOR16] = std::bind(&VM::ibor16, this);
        m_Operators[OpCode::IBOR32] = std::bind(&VM::ibor32, this);
        m_Operators[OpCode::IBOR64] = std::bind(&VM::ibor64, this);

        m_Operators[OpCode::IXOR8] = std::bind(&VM::ixor8, this);
        m_Operators[OpCode::IXOR16] = std::bind(&VM::ixor16, this);
        m_Operators[OpCode::IXOR32] = std::bind(&VM::ixor32, this);
        m_Operators[OpCode::IXOR64] = std::bind(&VM::ixor64, this);

        m_Operators[OpCode::ICML8] = std::bind(&VM::icml8, this);
        m_Operators[OpCode::ICML16] = std::bind(&VM::icml16, this);
        m_Operators[OpCode::ICML32] = std::bind(&VM::icml32, this);
        m_Operators[OpCode::ICML64] = std::bind(&VM::icml64, this);

        m_Operators[OpCode::INOT8] = std::bind(&VM::inot8, this);
        m_Operators[OpCode::INOT16] = std::bind(&VM::inot16, this);
        m_Operators[OpCode::INOT32] = std::bind(&VM::inot32, this);
        m_Operators[OpCode::INOT64] = std::bind(&VM::inot64, this);

        m_Operators[OpCode::IGRT8] = std::bind(&VM::igrt8, this);
        m_Operators[OpCode::IGRT16] = std::bind(&VM::igrt16, this);
        m_Operators[OpCode::IGRT32] = std::bind(&VM::igrt32, this);
        m_Operators[OpCode::IGRT64] = std::bind(&VM::igrt64, this);

        m_Operators[OpCode::ILES8] = std::bind(&VM::iles8, this);
        m_Operators[OpCode::ILES16] = std::bind(&VM::iles16, this);
        m_Operators[OpCode::ILES32] = std::bind(&VM::iles32, this);
        m_Operators[OpCode::ILES64] = std::bind(&VM::iles64, this);

        m_Operators[OpCode::IGRE8] = std::bind(&VM::igre8, this);
        m_Operators[OpCode::IGRE16] = std::bind(&VM::igre16, this);
        m_Operators[OpCode::IGRE32] = std::bind(&VM::igre32, this);
        m_Operators[OpCode::IGRE64] = std::bind(&VM::igre64, this);

        m_Operators[OpCode::ILSE8] = std::bind(&VM::ilse8, this);
        m_Operators[OpCode::ILSE16] = std::bind(&VM::ilse16, this);
        m_Operators[OpCode::ILSE32] = std::bind(&VM::ilse32, this);
        m_Operators[OpCode::ILSE64] = std::bind(&VM::ilse64, this);

        m_Operators[OpCode::IISE8] = std::bind(&VM::iise8, this);
        m_Operators[OpCode::IISE16] = std::bind(&VM::iise16, this);
        m_Operators[OpCode::IISE32] = std::bind(&VM::iise32, this);
        m_Operators[OpCode::IISE64] = std::bind(&VM::iise64, this);

        m_Operators[OpCode::INEQ8] = std::bind(&VM::ineq8, this);
        m_Operators[OpCode::INEQ16] = std::bind(&VM::ineq16, this);
        m_Operators[OpCode::INEQ32] = std::bind(&VM::ineq32, this);
        m_Operators[OpCode::INEQ64] = std::bind(&VM::ineq64, this);

        m_Operators[OpCode::IMOV8] = std::bind(&VM::imov8, this);
        m_Operators[OpCode::IMOV16] = std::bind(&VM::imov16, this);
        m_Operators[OpCode::IMOV32] = std::bind(&VM::imov32, this);
        m_Operators[OpCode::IMOV64] = std::bind(&VM::imov64, this);

        m_Operators[OpCode::IPSH8] = std::bind(&VM::ipsh8, this);
        m_Operators[OpCode::IPSH16] = std::bind(&VM::ipsh16, this);
        m_Operators[OpCode::IPSH32] = std::bind(&VM::ipsh32, this);
        m_Operators[OpCode::IPSH64] = std::bind(&VM::ipsh64, this);
        
        m_Operators[OpCode::IPOP8] = std::bind(&VM::ipop8, this);
        m_Operators[OpCode::IPOP16] = std::bind(&VM::ipop16, this);
        m_Operators[OpCode::IPOP32] = std::bind(&VM::ipop32, this);
        m_Operators[OpCode::IPOP64] = std::bind(&VM::ipop64, this);

        m_Operators[OpCode::FADD32] = std::bind(&VM::fadd32, this);
        m_Operators[OpCode::FADD64] = std::bind(&VM::fadd64, this);

        m_Operators[OpCode::FSUB32] = std::bind(&VM::fsub32, this);
        m_Operators[OpCode::FSUB64] = std::bind(&VM::fsub64, this);

        m_Operators[OpCode::FMUL32] = std::bind(&VM::fmul32, this);
        m_Operators[OpCode::FMUL64] = std::bind(&VM::fmul64, this);

        m_Operators[OpCode::FDIV32] = std::bind(&VM::fdiv32, this);
        m_Operators[OpCode::FDIV64] = std::bind(&VM::fdiv64, this);

        m_Operators[OpCode::FINC32] = std::bind(&VM::finc32, this);
        m_Operators[OpCode::FINC64] = std::bind(&VM::finc64, this);

        m_Operators[OpCode::FDEC32] = std::bind(&VM::fdec32, this);
        m_Operators[OpCode::FDEC64] = std::bind(&VM::fdec64, this);

        m_Operators[OpCode::FGRT32] = std::bind(&VM::fgrt32, this);
        m_Operators[OpCode::FGRT64] = std::bind(&VM::fgrt64, this);

        m_Operators[OpCode::FLES32] = std::bind(&VM::fles32, this);
        m_Operators[OpCode::FLES64] = std::bind(&VM::fles64, this);

        m_Operators[OpCode::FGRE32] = std::bind(&VM::fgre32, this);
        m_Operators[OpCode::FGRE64] = std::bind(&VM::fgre64, this);

        m_Operators[OpCode::FLSE32] = std::bind(&VM::flse32, this);
        m_Operators[OpCode::FLSE64] = std::bind(&VM::flse64, this);

        m_Operators[OpCode::FISE32] = std::bind(&VM::fise32, this);
        m_Operators[OpCode::FISE64] = std::bind(&VM::fise64, this);

        m_Operators[OpCode::FNEQ32] = std::bind(&VM::fneq32, this);
        m_Operators[OpCode::FNEQ64] = std::bind(&VM::fneq64, this);

        m_Operators[OpCode::FMOV32] = std::bind(&VM::fmov32, this);
        m_Operators[OpCode::FMOV64] = std::bind(&VM::fmov64, this);

        m_Operators[OpCode::FPSH32] = std::bind(&VM::fpsh32, this);
        m_Operators[OpCode::FPSH64] = std::bind(&VM::fpsh64, this);
        
        m_Operators[OpCode::FPOP32] = std::bind(&VM::fpop32, this);
        m_Operators[OpCode::FPOP64] = std::bind(&VM::fpop64, this);
 
        m_Operators[OpCode::RADD8] = std::bind(&VM::radd8, this);
        m_Operators[OpCode::RADD16] = std::bind(&VM::radd16, this);
        m_Operators[OpCode::RADD32] = std::bind(&VM::radd32, this);
        m_Operators[OpCode::RADD64] = std::bind(&VM::radd64, this);

        m_Operators[OpCode::RSUB8] = std::bind(&VM::rsub8, this);
        m_Operators[OpCode::RSUB16] = std::bind(&VM::rsub16, this);
        m_Operators[OpCode::RSUB32] = std::bind(&VM::rsub32, this);
        m_Operators[OpCode::RSUB64] = std::bind(&VM::rsub64, this);

        m_Operators[OpCode::LND] = std::bind(&VM::lnd, this);
        m_Operators[OpCode::LOR] = std::bind(&VM::lor, this);

        m_Operators[OpCode::POPNUL] = std::bind(&VM::popnul, this);

        m_Operators[OpCode::ALLOC] = std::bind(&VM::alloc, this);
        m_Operators[OpCode::FREE] = std::bind(&VM::free, this);

        m_Operators[OpCode::JMP] = std::bind(&VM::jmp, this);
        m_Operators[OpCode::JMT] = std::bind(&VM::jmt, this);
        m_Operators[OpCode::JMF] = std::bind(&VM::jmf, this);

        m_Operators[OpCode::CALL] = std::bind(&VM::call, this);
        m_Operators[OpCode::EXTRN] = std::bind(&VM::extrn, this);
        m_Operators[OpCode::RET] = std::bind(&VM::ret, this);
    }

    VM::~VM()
    {
        ::free(m_Heap);
    }

    
    
    
    
    
    
    
    
    
    void VM::uadd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 + r1.u8;
    }

    void VM::uadd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 + r1.u16;
    }
    
    void VM::uadd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 + r1.u32;
    }
    
    void VM::uadd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 + r1.u64;
    }

    void VM::usub8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 - r1.u8;
    }

    void VM::usub16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 - r1.u16;
    }
    
    void VM::usub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 - r1.u32;
    }
    
    void VM::usub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 - r1.u64;
    }

    void VM::umul8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 * r1.u8;
    }

    void VM::umul16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 * r1.u16;
    }
    
    void VM::umul32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 * r1.u32;
    }
    
    void VM::umul64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 * r1.u64;
    }

    void VM::udiv8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 / r1.u8;
    }

    void VM::udiv16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 / r1.u16;
    }
    
    void VM::udiv32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 / r1.u32;
    }
    
    void VM::udiv64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 / r1.u64;
    }

    void VM::umod8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 % r1.u8;
    }

    void VM::umod16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 % r1.u16;
    }
    
    void VM::umod32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 % r1.u32;
    }
    
    void VM::umod64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 % r1.u64;
    }

    void VM::uinc8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u8 = ++ r0.u8;
    }

    void VM::uinc16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u16 = ++ r0.u16;
    }
    
    void VM::uinc32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u32 = ++ r0.u32;
    }
    
    void VM::uinc64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u64 = ++ r0.u64;
    }

    void VM::udec8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u8 = -- r0.u8;
    }

    void VM::udec16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u16 = -- r0.u16;
    }
    
    void VM::udec32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u32 = -- r0.u32;
    }
    
    void VM::udec64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u64 = -- r0.u64;
    }

    void VM::ulst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 << r1.u8;
    }

    void VM::ulst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 << r1.u16;
    }
    
    void VM::ulst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 << r1.u32;
    }
    
    void VM::ulst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 << r1.u64;
    }

    void VM::urst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 >> r1.u8;
    }

    void VM::urst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 >> r1.u16;
    }
    
    void VM::urst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 >> r1.u32;
    }
    
    void VM::urst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 >> r1.u64;
    }

    void VM::ubnd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 & r1.u8;
    }

    void VM::ubnd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 & r1.u16;
    }
    
    void VM::ubnd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 & r1.u32;
    }
    
    void VM::ubnd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 & r1.u64;
    }

    void VM::ubor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 | r1.u8;
    }

    void VM::ubor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 | r1.u16;
    }
    
    void VM::ubor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 | r1.u32;
    }
    
    void VM::ubor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 | r1.u64;
    }

    void VM::uxor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u8 = r0.u8 ^ r1.u8;
    }

    void VM::uxor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u16 = r0.u16 ^ r1.u16;
    }
    
    void VM::uxor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u32 = r0.u32 ^ r1.u32;
    }
    
    void VM::uxor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.u64 = r0.u64 ^ r1.u64;
    }

    void VM::ucml8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u8 = ~ r0.u8;
    }

    void VM::ucml16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u16 = ~ r0.u16;
    }
    
    void VM::ucml32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u32 = ~ r0.u32;
    }
    
    void VM::ucml64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u64 = ~ r0.u64;
    }

    void VM::unot8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u8 = ! r0.u8;
    }

    void VM::unot16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u16 = ! r0.u16;
    }
    
    void VM::unot32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u32 = ! r0.u32;
    }
    
    void VM::unot64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u64 = ! r0.u64;
    }

    void VM::ugrt8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u8 > r1.u8;
    }

    void VM::ugrt16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u16 > r1.u16;
    }
    
    void VM::ugrt32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u32 > r1.u32;
    }
    
    void VM::ugrt64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u64 > r1.u64;
    }

    void VM::ules8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u8 < r1.u8;
    }

    void VM::ules16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u16 < r1.u16;
    }
    
    void VM::ules32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u32 < r1.u32;
    }
    
    void VM::ules64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u64 < r1.u64;
    }

    void VM::ugre8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u8 >= r1.u8;
    }

    void VM::ugre16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u16 >= r1.u16;
    }
    
    void VM::ugre32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u32 >= r1.u32;
    }
    
    void VM::ugre64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u64 >= r1.u64;
    }

    void VM::ulse8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u8 <= r1.u8;
    }

    void VM::ulse16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u16 <= r1.u16;
    }
    
    void VM::ulse32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u32 <= r1.u32;
    }
    
    void VM::ulse64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u64 <= r1.u64;
    }

    void VM::uise8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u8 == r1.u8;
    }

    void VM::uise16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u16 == r1.u16;
    }
    
    void VM::uise32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u32 == r1.u32;
    }
    
    void VM::uise64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u64 == r1.u64;
    }

    void VM::uneq8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u8 != r1.u8;
    }

    void VM::uneq16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u16 != r1.u16;
    }
    
    void VM::uneq32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u32 != r1.u32;
    }
    
    void VM::uneq64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.u64 != r1.u64;
    }

    void VM::umov8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[1];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 1);
        m_IP += 1;

        r0.u8 = *(uint8_t *)bytes;
    }

    void VM::umov16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[2];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 2);
        m_IP += 2;

        r0.u16 = *(uint16_t *)bytes;
    }
    
    void VM::umov32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[4];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 4);
        m_IP += 4;

        r0.u32 = *(uint32_t *)bytes;
    }
    
    void VM::umov64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[8];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 8);
        m_IP += 8;

        r0.u64 = *(uint64_t *)bytes;
    }

    void VM::umvr8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.u8 = r1.u8;
    }

    void VM::umvr16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.u16 = r1.u16;
    }
    
    void VM::umvr32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.u32 = r1.u32;
    }
    
    void VM::umvr64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.u64 = r1.u64;
    }

    void VM::upsh8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.u8, 1);
    }
    
    void VM::upsh16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.u16, 2);
    }
    
    void VM::upsh32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.u32, 4);
    }
    
    void VM::upsh64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.u64, 8);
    }

    void VM::upop8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u8 = *(uint8_t *)pop(1);
    }
    
    void VM::upop16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u16 = *(uint16_t *)pop(2);
    }
    
    void VM::upop32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u32 = *(uint32_t *)pop(4);
    }
    
    void VM::upop64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.u64 = *(uint64_t *)pop(8);
    }

    void VM::iadd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 + r1.i8;
    }

    void VM::iadd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 + r1.i16;
    }
    
    void VM::iadd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 + r1.i32;
    }
    
    void VM::iadd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 + r1.i64;
    }

    void VM::isub8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 - r1.i8;
    }

    void VM::isub16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 - r1.i16;
    }
    
    void VM::isub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 - r1.i32;
    }
    
    void VM::isub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 - r1.i64;
    }

    void VM::imul8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 * r1.i8;
    }

    void VM::imul16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 * r1.i16;
    }
    
    void VM::imul32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 * r1.i32;
    }
    
    void VM::imul64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 * r1.i64;
    }

    void VM::idiv8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 / r1.i8;
    }

    void VM::idiv16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 / r1.i16;
    }
    
    void VM::idiv32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 / r1.i32;
    }
    
    void VM::idiv64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 / r1.i64;
    }

    void VM::imod8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 % r1.i8;
    }

    void VM::imod16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 % r1.i16;
    }
    
    void VM::imod32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 % r1.i32;
    }
    
    void VM::imod64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 % r1.i64;
    }

    void VM::iinc8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i8 = ++ r0.i8;
    }

    void VM::iinc16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i16 = ++ r0.i16;
    }
    
    void VM::iinc32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i32 = ++ r0.i32;
    }
    
    void VM::iinc64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i64 = ++ r0.i64;
    }

    void VM::idec8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i8 = -- r0.i8;
    }

    void VM::idec16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i16 = -- r0.i16;
    }
    
    void VM::idec32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i32 = -- r0.i32;
    }
    
    void VM::idec64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i64 = -- r0.i64;
    }

    void VM::ilst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 << r1.i8;
    }

    void VM::ilst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 << r1.i16;
    }
    
    void VM::ilst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 << r1.i32;
    }
    
    void VM::ilst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 << r1.i64;
    }

    void VM::irst8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 >> r1.i8;
    }

    void VM::irst16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 >> r1.i16;
    }
    
    void VM::irst32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 >> r1.i32;
    }
    
    void VM::irst64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 >> r1.i64;
    }

    void VM::ibnd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 & r1.i8;
    }

    void VM::ibnd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 & r1.i16;
    }
    
    void VM::ibnd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 & r1.i32;
    }
    
    void VM::ibnd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 & r1.i64;
    }

    void VM::ibor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 | r1.i8;
    }

    void VM::ibor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 | r1.i16;
    }
    
    void VM::ibor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 | r1.i32;
    }
    
    void VM::ibor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 | r1.i64;
    }

    void VM::ixor8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i8 = r0.i8 ^ r1.i8;
    }

    void VM::ixor16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i16 = r0.i16 ^ r1.i16;
    }
    
    void VM::ixor32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i32 = r0.i32 ^ r1.i32;
    }
    
    void VM::ixor64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.i64 = r0.i64 ^ r1.i64;
    }

    void VM::icml8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i8 = ~ r0.i8;
    }

    void VM::icml16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i16 = ~ r0.i16;
    }
    
    void VM::icml32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i32 = ~ r0.i32;
    }
    
    void VM::icml64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i64 = ~ r0.i64;
    }

    void VM::inot8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i8 = ! r0.i8;
    }

    void VM::inot16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i16 = ! r0.i16;
    }
    
    void VM::inot32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i32 = ! r0.i32;
    }
    
    void VM::inot64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i64 = ! r0.i64;
    }

    void VM::igrt8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i8 > r1.i8;
    }

    void VM::igrt16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i16 > r1.i16;
    }
    
    void VM::igrt32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i32 > r1.i32;
    }
    
    void VM::igrt64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i64 > r1.i64;
    }

    void VM::iles8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i8 < r1.i8;
    }

    void VM::iles16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i16 < r1.i16;
    }
    
    void VM::iles32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i32 < r1.i32;
    }
    
    void VM::iles64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i64 < r1.i64;
    }

    void VM::igre8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i8 >= r1.i8;
    }

    void VM::igre16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i16 >= r1.i16;
    }
    
    void VM::igre32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i32 >= r1.i32;
    }
    
    void VM::igre64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i64 >= r1.i64;
    }

    void VM::ilse8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i8 <= r1.i8;
    }

    void VM::ilse16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i16 <= r1.i16;
    }
    
    void VM::ilse32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i32 <= r1.i32;
    }
    
    void VM::ilse64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i64 <= r1.i64;
    }

    void VM::iise8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i8 == r1.i8;
    }

    void VM::iise16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i16 == r1.i16;
    }
    
    void VM::iise32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i32 == r1.i32;
    }
    
    void VM::iise64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i64 == r1.i64;
    }

    void VM::ineq8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i8 != r1.i8;
    }

    void VM::ineq16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i16 != r1.i16;
    }
    
    void VM::ineq32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i32 != r1.i32;
    }
    
    void VM::ineq64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.i64 != r1.i64;
    }

    void VM::imov8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[1];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 1);
        m_IP += 1;

        r0.i8 = *(int8_t *)bytes;
    }

    void VM::imov16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[2];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 2);
        m_IP += 2;

        r0.i16 = *(int16_t *)bytes;
    }
    
    void VM::imov32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[4];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 4);
        m_IP += 4;

        r0.i32 = *(int32_t *)bytes;
    }
    
    void VM::imov64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[8];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 8);
        m_IP += 8;

        r0.i64 = *(int64_t *)bytes;
    }

    void VM::imvr8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.i8 = r1.i8;
    }

    void VM::imvr16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.i16 = r1.i16;
    }
    
    void VM::imvr32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.i32 = r1.i32;
    }
    
    void VM::imvr64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.i64 = r1.i64;
    }

    void VM::ipsh8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.i8, 1);
    }
    
    void VM::ipsh16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.i16, 2);
    }
    
    void VM::ipsh32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.i32, 4);
    }
    
    void VM::ipsh64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.i64, 8);
    }

    void VM::ipop8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i8 = *(int8_t *)pop(1);
    }
    
    void VM::ipop16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i16 = *(int16_t *)pop(2);
    }
    
    void VM::ipop32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i32 = *(int32_t *)pop(4);
    }
    
    void VM::ipop64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.i64 = *(int64_t *)pop(8);
    }

    void VM::fadd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f32 = r0.f32 + r1.f32;
    }
    
    void VM::fadd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f64 = r0.f64 + r1.f64;
    }

    void VM::fsub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f32 = r0.f32 - r1.f32;
    }
    
    void VM::fsub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f64 = r0.f64 - r1.f64;
    }

    void VM::fmul32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f32 = r0.f32 * r1.f32;
    }
    
    void VM::fmul64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f64 = r0.f64 * r1.f64;
    }

    void VM::fdiv32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f32 = r0.f32 / r1.f32;
    }
    
    void VM::fdiv64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.f64 = r0.f64 / r1.f64;
    }
    
    void VM::finc32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.f32 = ++ r0.f32;
    }
    
    void VM::finc64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.f64 = ++ r0.f64;
    }

    void VM::fdec32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.f32 = -- r0.f32;
    }
    
    void VM::fdec64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.f64 = -- r0.f64;
    }

    void VM::fgrt32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f32 > r1.f32;
    }
    
    void VM::fgrt64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f64 > r1.f64;
    }

    void VM::fles32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f32 < r1.f32;
    }
    
    void VM::fles64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f64 < r1.f64;
    }

    void VM::fgre32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f32 >= r1.f32;
    }
    
    void VM::fgre64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f64 >= r1.f64;
    }

    void VM::flse32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f32 <= r1.f32;
    }
    
    void VM::flse64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f64 <= r1.f64;
    }

    void VM::fise32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f32 == r1.f32;
    }
    
    void VM::fise64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f64 == r1.f64;
    }

    void VM::fneq32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f32 != r1.f32;
    }
    
    void VM::fneq64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        m_Cmp[(uint8_t)getRegister(RegisterType::CMT, 0).u8] = r0.f64 != r1.f64;
    }

    void VM::fmov32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[4];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 4);
        m_IP += 4;

        r0.f32 = *(float *)bytes;
    }
    
    void VM::fmov64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        uint8_t bytes[8];
        
        memcpy((uint8_t *)bytes, &m_Code[m_IP + 1], 8);
        m_IP += 8;

        r0.f64 = *(double *)bytes;
    }

    void VM::fmvr32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.f32 = r1.f32;
    }
    
    void VM::fmvr64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);

        r0.f64 = r1.f64;
    }
    
    void VM::fpsh32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.f32, 4);
    }
    
    void VM::fpsh64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        push((uint8_t *)&r0.f64, 8);
    }

    void VM::fpop32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.f32 = *(float *)pop(4);
    }
    
    void VM::fpop64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        r0.f64 = *(double *)pop(8);
    }

    void VM::radd8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr + (uint64_t)r1.ptr);
    }

    void VM::radd16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr + (uint64_t)r1.ptr);
    }
    
    void VM::radd32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr + (uint64_t)r1.ptr);
    }
    
    void VM::radd64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr + (uint64_t)r1.ptr);
    }

    void VM::rsub8()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr - (uint64_t)r1.ptr);
    }

    void VM::rsub16()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr - (uint64_t)r1.ptr);
    }
    
    void VM::rsub32()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr - (uint64_t)r1.ptr);
    }
    
    void VM::rsub64()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.ptr = (void *)((uint64_t)r0.ptr - (uint64_t)r1.ptr);
    }

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

    void VM::jmp()
    {
        m_IP = *(uint64_t *)&m_Code[++m_IP];
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
        uint64_t currentIP = m_IP + 12;

        uint32_t argsSize = *(uint32_t *)&m_Code[++m_IP];

        m_IP += 3;

        uint8_t * args = new uint8_t[argsSize];

        memcpy(args, pop(argsSize), argsSize);

        push((uint8_t *)&m_SP, 8);
        push((uint8_t *)&currentIP, 8);
        push((uint8_t *)&m_FP, 8);

        m_FP = m_SP;

        push(args, argsSize);

        delete[] args;

        m_IP = *(uint64_t *)&m_Code[++m_IP];
        m_IP--;
    }

    void VM::extrn()
    {
        uint32_t id = *(uint64_t *)&m_Code[++m_IP];
        
        if (m_ExternalFunctions.count(id) != 0)
        {
            m_ExternalFunctions[id](*this);
        }

        m_IP += 7;
    }

    void VM::ret()
    {
        uint64_t retSize = *(uint32_t *)&m_Code[++m_IP];
        uint8_t * retData = pop(retSize);

        m_SP = m_FP;

        m_FP = *(uint64_t *)pop(8);
        m_IP = *(uint64_t *)pop(8);
        m_SP = *(uint64_t *)pop(8);

        push(retData, retSize);
    }
    
    Register & VM::getRegister(RegisterType::RegisterType type, uint64_t offset)
    {
        if (type == RegisterType::SP)
        {
            return *(Register *)&m_Stack[m_FP + offset];
        }
        else if (type >= RegisterType::R1 && type <= RegisterType::R4)
        {
            return m_NoFollowRef ? *m_RegisterMap[type] : *(Register *)(m_RegisterMap[type]->ptr);
        }

        return *m_RegisterMap[type];
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

        OpCode::OpCode op = (OpCode::OpCode)m_Code[m_IP];

        while (op != OpCode::STOP)
        {
            m_Operators[(uint8_t)op]();

            op = (OpCode::OpCode)m_Code[++m_IP];
        }
    }
}
