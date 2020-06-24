#ifndef FALCON_OPCODE_HPP
#define FALCON_OPCODE_HPP

#include <cstdint>

namespace Falcon
{
    namespace OpCode
    {
        enum OpCode
        {
            UADD8, UADD16, UADD32, UADD64,
            USUB8, USUB16, USUB32, USUB64,
            UMUL8, UMUL16, UMUL32, UMUL64,
            UDIV8, UDIV16, UDIV32, UDIV64,
            UMOD8, UMOD16, UMOD32, UMOD64,
            UINC8, UINC16, UINC32, UINC64,
            UDEC8, UDEC16, UDEC32, UDEC64,

            ULST8, ULST16, ULST32, ULST64,
            URST8, URST16, URST32, URST64,
            UBND8, UBND16, UBND32, UBND64,
            UBOR8, UBOR16, UBOR32, UBOR64,
            UXOR8, UXOR16, UXOR32, UXOR64,
            UCML8, UCML16, UCML32, UCML64,
            UNOT8, UNOT16, UNOT32, UNOT64,

            UGRT8, UGRT16, UGRT32, UGRT64,
            ULES8, ULES16, ULES32, ULES64,
            UGRE8, UGRE16, UGRE32, UGRE64,
            ULSE8, ULSE16, ULSE32, ULSE64,
            UISE8, UISE16, UISE32, UISE64,
            UNEQ8, UNEQ16, UNEQ32, UNEQ64,

            UMOV8, UMOV16, UMOV32, UMOV64,
            UMVR8, UMVR16, UMVR32, UMVR64,

            UPSH8, UPSH16, UPSH32, UPSH64,
            UPOP8, UPOP16, UPOP32, UPOP64,

            LND,   LOR,

            POPNUL,

            ALLOC, FREE,

            JMT,   JMF,    CALL,   EXTRN,
            RET,   STOP
        };
    }
}

#endif
