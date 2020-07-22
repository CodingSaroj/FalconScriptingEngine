#include "OpCode.hpp"

namespace Falcon
{
    namespace OpCode
    {
        std::vector<std::string> names =
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
            "UNOT8", "UNOT16", "UNOT32", "UNOT64",

            "UGRT8", "UGRT16", "UGRT32", "UGRT64",
            "ULES8", "ULES16", "ULES32", "ULES64",
            "UGRE8", "UGRE16", "UGRE32", "UGRE64",
            "ULSE8", "ULSE16", "ULSE32", "ULSE64",
            "UISE8", "UISE16", "UISE32", "UISE64",
            "UNEQ8", "UNEQ16", "UNEQ32", "UNEQ64",

            "UMOV8", "UMOV16", "UMOV32", "UMOV64",
            "UMVR8", "UMVR16", "UMVR32", "UMVR64",

            "UPSH8", "UPSH16", "UPSH32", "UPSH64",
            "UPOP8", "UPOP16", "UPOP32", "UPOP64",

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
            "INOT8", "INOT16", "INOT32", "INOT64",

            "IGRT8", "IGRT16", "IGRT32", "IGRT64",
            "ILES8", "ILES16", "ILES32", "ILES64",
            "IGRE8", "IGRE16", "IGRE32", "IGRE64",
            "ILSE8", "ILSE16", "ILSE32", "ILSE64",
            "IISE8", "IISE16", "IISE32", "IISE64",
            "INEQ8", "INEQ16", "INEQ32", "INEQ64",

            "IMOV8", "IMOV16", "IMOV32", "IMOV64",
            "IMVR8", "IMVR16", "IMVR32", "IMVR64",

            "IPSH8", "IPSH16", "IPSH32", "IPSH64",
            "IPOP8", "IPOP16", "IPOP32", "IPOP64",

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

            "FMOV32", "FMOV64",
            "FMVR32", "FMVR64",

            "FPSH32", "FPSH64",
            "FPOP32", "FPOP64",

            "RADD8", "RADD16", "RADD32", "RADD64",
            "RSUB8", "RSUB16", "RSUB32", "RSUB64",
 
            "LND",   "LOR",

            "POPNUL",

            "ALLOC", "FREE",

            "JMT",   "JMF",     "JMP",  "CALL",
            "EXTRN", "RET",   "STOP"
        };
 
    }
}
