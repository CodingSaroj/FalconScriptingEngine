#include "OpCode.hpp"

namespace Falcon
{
    namespace OpCode
    {
        std::vector<std::string> s_Names =
        {
            "UADD8", "UADD16", "UADD32", "UADD64",
            "USUB8", "USUB16", "USUB32", "USUB64",
            "UMUL8", "UMUL16", "UMUL32", "UMUL64",
            "UDIV8", "UDIV16", "UDIV32", "UDIV64",
            "UMOD8", "UMOD16", "UMOD32", "UMOD64",
            "UINC8", "UINC16", "UINC32", "UINC64",
            "UDEC8", "UDEC16", "UDEC32", "UDEC64",

            "ULST8", "ULST16", "ULST32", "ULST64",
            "URST8", "URST16", "URST32", "URST64",
            "UBND8", "UBND16", "UBND32", "UBND64",
            "UBOR8", "UBOR16", "UBOR32", "UBOR64",
            "UXOR8", "UXOR16", "UXOR32", "UXOR64",
            "UCML8", "UCML16", "UCML32", "UCML64",

            "UGRT8", "UGRT16", "UGRT32", "UGRT64",
            "ULES8", "ULES16", "ULES32", "ULES64",
            "UGRE8", "UGRE16", "UGRE32", "UGRE64",
            "ULSE8", "ULSE16", "ULSE32", "ULSE64",
            "UISE8", "UISE16", "UISE32", "UISE64",
            "UNEQ8", "UNEQ16", "UNEQ32", "UNEQ64",

            "IADD8", "IADD16", "IADD32", "IADD64",
            "ISUB8", "ISUB16", "ISUB32", "ISUB64",
            "IMUL8", "IMUL16", "IMUL32", "IMUL64",
            "IDIV8", "IDIV16", "IDIV32", "IDIV64",
            "IMOD8", "IMOD16", "IMOD32", "IMOD64",
            "IINC8", "IINC16", "IINC32", "IINC64",
            "IDEC8", "IDEC16", "IDEC32", "IDEC64",

            "ILST8", "ILST16", "ILST32", "ILST64",
            "IRST8", "IRST16", "IRST32", "IRST64",
            "IBND8", "IBND16", "IBND32", "IBND64",
            "IBOR8", "IBOR16", "IBOR32", "IBOR64",
            "IXOR8", "IXOR16", "IXOR32", "IXOR64",
            "ICML8", "ICML16", "ICML32", "ICML64",

            "IGRT8", "IGRT16", "IGRT32", "IGRT64",
            "ILES8", "ILES16", "ILES32", "ILES64",
            "IGRE8", "IGRE16", "IGRE32", "IGRE64",
            "ILSE8", "ILSE16", "ILSE32", "ILSE64",
            "IISE8", "IISE16", "IISE32", "IISE64",
            "INEQ8", "INEQ16", "INEQ32", "INEQ64",

            "FADD32", "FADD64",
            "FSUB32", "FSUB64",
            "FMUL32", "FMUL64",
            "FDIV32", "FDIV64",
            "FINC32", "FINC64",
            "FDEC32", "FDEC64",

            "FGRT32", "FGRT64",
            "FLES32", "FLES64",
            "FGRE32", "FGRE64",
            "FLSE32", "FLSE64",
            "FISE32", "FISE64",
            "FNEQ32", "FNEQ64",

            "PSH8", "PSH16", "PSH32", "PSH64",
            "POP8", "POP16", "POP32", "POP64",

            "MOV8", "MOV16", "MOV32", "MOV64",
            "MVR8", "MVR16", "MVR32", "MVR64",

            "PSHSTR",

            "LND", "LOR", "NOT",

            "LOAD8",  "LOAD16", "LOAD32", "LOAD64",
            "LODREF",

            "PSHNUL", "POPNUL",

            "JMP",   "JMT", "JMF",  "CALL",
            "EXTRN", "RET", "FUNC", "STOP"
        };

        static const Layout BASE = Layout::BASE;
        static const Layout UN_REG = Layout::UN_REG;
        static const Layout UN_NUM = Layout::UN_NUM;
        static const Layout UN_STR = Layout::UN_STR;
        static const Layout BIN_REG_REG = Layout::BIN_REG_REG;
        static const Layout BIN_MOV = Layout::BIN_MOV;
        static const Layout BIN_CALL = Layout::BIN_CALL;

        std::vector<Layout> s_Layouts
        {
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            UN_REG, UN_REG, UN_REG, UN_REG,
            UN_REG, UN_REG, UN_REG, UN_REG,

            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            UN_REG, UN_REG, UN_REG, UN_REG,

            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,

            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            UN_REG, UN_REG, UN_REG, UN_REG,
            UN_REG, UN_REG, UN_REG, UN_REG,

            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            UN_REG, UN_REG, UN_REG, UN_REG,

            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,

            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,
            UN_REG, UN_REG,
            UN_REG, UN_REG,

            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,
            BIN_REG_REG, BIN_REG_REG,

            BIN_MOV, BIN_MOV, BIN_MOV, BIN_MOV,
            BIN_REG_REG, BIN_REG_REG, BIN_REG_REG, BIN_REG_REG,

            UN_REG, UN_REG, UN_REG, UN_REG,
            UN_REG, UN_REG, UN_REG, UN_REG,

            UN_STR,

            BASE, BASE, BASE,

            BIN_MOV, BIN_MOV, BIN_MOV, BIN_MOV,
            BIN_MOV,

            UN_NUM, UN_NUM,

            UN_NUM, UN_NUM, UN_NUM, BIN_CALL,
            UN_STR, UN_NUM, BASE, BASE
        };
    }
}
