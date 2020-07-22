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
    define(`BINPTRFUN',
    `void VM::$1()
    {
        Register & r0 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO0, 0).u64);
        Register & r1 = getRegister((RegisterType::RegisterType)m_Code[++m_IP], getRegister(RegisterType::AO1, 0).u64);
        r0.$3 = (void *)((uint64_t)r0.$3 $2 (uint64_t)r1.$3);
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

    BINFUN(iadd8, +, i8)

    BINFUN(iadd16, +, i16)
    
    BINFUN(iadd32, +, i32)
    
    BINFUN(iadd64, +, i64)

    BINFUN(isub8, -, i8)

    BINFUN(isub16, -, i16)
    
    BINFUN(isub32, -, i32)
    
    BINFUN(isub64, -, i64)

    BINFUN(imul8, *, i8)

    BINFUN(imul16, *, i16)
    
    BINFUN(imul32, *, i32)
    
    BINFUN(imul64, *, i64)

    BINFUN(idiv8, /, i8)

    BINFUN(idiv16, /, i16)
    
    BINFUN(idiv32, /, i32)
    
    BINFUN(idiv64, /, i64)

    BINFUN(imod8, %, i8)

    BINFUN(imod16, %, i16)
    
    BINFUN(imod32, %, i32)
    
    BINFUN(imod64, %, i64)

    UNFUN(iinc8, ++, i8)

    UNFUN(iinc16, ++, i16)
    
    UNFUN(iinc32, ++, i32)
    
    UNFUN(iinc64, ++, i64)

    UNFUN(idec8, --, i8)

    UNFUN(idec16, --, i16)
    
    UNFUN(idec32, --, i32)
    
    UNFUN(idec64, --, i64)

    BINFUN(ilst8, <<, i8)

    BINFUN(ilst16, <<, i16)
    
    BINFUN(ilst32, <<, i32)
    
    BINFUN(ilst64, <<, i64)

    BINFUN(irst8, >>, i8)

    BINFUN(irst16, >>, i16)
    
    BINFUN(irst32, >>, i32)
    
    BINFUN(irst64, >>, i64)

    BINFUN(ibnd8, &, i8)

    BINFUN(ibnd16, &, i16)
    
    BINFUN(ibnd32, &, i32)
    
    BINFUN(ibnd64, &, i64)

    BINFUN(ibor8, |, i8)

    BINFUN(ibor16, |, i16)
    
    BINFUN(ibor32, |, i32)
    
    BINFUN(ibor64, |, i64)

    BINFUN(ixor8, ^, i8)

    BINFUN(ixor16, ^, i16)
    
    BINFUN(ixor32, ^, i32)
    
    BINFUN(ixor64, ^, i64)

    UNFUN(icml8, ~, i8)

    UNFUN(icml16, ~, i16)
    
    UNFUN(icml32, ~, i32)
    
    UNFUN(icml64, ~, i64)

    UNFUN(inot8, !, i8)

    UNFUN(inot16, !, i16)
    
    UNFUN(inot32, !, i32)
    
    UNFUN(inot64, !, i64)

    BINCMPFUN(igrt8, >, i8)

    BINCMPFUN(igrt16, >, i16)
    
    BINCMPFUN(igrt32, >, i32)
    
    BINCMPFUN(igrt64, >, i64)

    BINCMPFUN(iles8, <, i8)

    BINCMPFUN(iles16, <, i16)
    
    BINCMPFUN(iles32, <, i32)
    
    BINCMPFUN(iles64, <, i64)

    BINCMPFUN(igre8, >=, i8)

    BINCMPFUN(igre16, >=, i16)
    
    BINCMPFUN(igre32, >=, i32)
    
    BINCMPFUN(igre64, >=, i64)

    BINCMPFUN(ilse8, <=, i8)

    BINCMPFUN(ilse16, <=, i16)
    
    BINCMPFUN(ilse32, <=, i32)
    
    BINCMPFUN(ilse64, <=, i64)

    BINCMPFUN(iise8, ==, i8)

    BINCMPFUN(iise16, ==, i16)
    
    BINCMPFUN(iise32, ==, i32)
    
    BINCMPFUN(iise64, ==, i64)

    BINCMPFUN(ineq8, !=, i8)

    BINCMPFUN(ineq16, !=, i16)
    
    BINCMPFUN(ineq32, !=, i32)
    
    BINCMPFUN(ineq64, !=, i64)

    MOVFUN(imov8, int8_t, 1, i8)

    MOVFUN(imov16, int16_t, 2, i16)
    
    MOVFUN(imov32, int32_t, 4, i32)
    
    MOVFUN(imov64, int64_t, 8, i64)

    MVRFUN(imvr8, int8_t, 1, i8)

    MVRFUN(imvr16, int16_t, 2, i16)
    
    MVRFUN(imvr32, int32_t, 4, i32)
    
    MVRFUN(imvr64, int64_t, 8, i64)

    PSHFUN(ipsh8, 1, i8)
    
    PSHFUN(ipsh16, 2, i16)
    
    PSHFUN(ipsh32, 4, i32)
    
    PSHFUN(ipsh64, 8, i64)

    POPFUN(ipop8, int8_t, 1, i8)
    
    POPFUN(ipop16, int16_t, 2, i16)
    
    POPFUN(ipop32, int32_t, 4, i32)
    
    POPFUN(ipop64, int64_t, 8, i64)

    BINFUN(fadd32, +, f32)
    
    BINFUN(fadd64, +, f64)

    BINFUN(fsub32, -, f32)
    
    BINFUN(fsub64, -, f64)

    BINFUN(fmul32, *, f32)
    
    BINFUN(fmul64, *, f64)

    BINFUN(fdiv32, /, f32)
    
    BINFUN(fdiv64, /, f64)
    
    UNFUN(finc32, ++, f32)
    
    UNFUN(finc64, ++, f64)

    UNFUN(fdec32, --, f32)
    
    UNFUN(fdec64, --, f64)

    BINCMPFUN(fgrt32, >, f32)
    
    BINCMPFUN(fgrt64, >, f64)

    BINCMPFUN(fles32, <, f32)
    
    BINCMPFUN(fles64, <, f64)

    BINCMPFUN(fgre32, >=, f32)
    
    BINCMPFUN(fgre64, >=, f64)

    BINCMPFUN(flse32, <=, f32)
    
    BINCMPFUN(flse64, <=, f64)

    BINCMPFUN(fise32, ==, f32)
    
    BINCMPFUN(fise64, ==, f64)

    BINCMPFUN(fneq32, !=, f32)
    
    BINCMPFUN(fneq64, !=, f64)

    MOVFUN(fmov32, float, 4, f32)
    
    MOVFUN(fmov64, double, 8, f64)

    MVRFUN(fmvr32, float, 4, f32)
    
    MVRFUN(fmvr64, double, 8, f64)
    
    PSHFUN(fpsh32, 4, f32)
    
    PSHFUN(fpsh64, 8, f64)

    POPFUN(fpop32, float, 4, f32)
    
    POPFUN(fpop64, double, 8, f64)

    BINPTRFUN(radd8, +, ptr)

    BINPTRFUN(radd16, +, ptr)
    
    BINPTRFUN(radd32, +, ptr)
    
    BINPTRFUN(radd64, +, ptr)

    BINPTRFUN(rsub8, -, ptr)

    BINPTRFUN(rsub16, -, ptr)
    
    BINPTRFUN(rsub32, -, ptr)
    
    BINPTRFUN(rsub64, -, ptr)

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
