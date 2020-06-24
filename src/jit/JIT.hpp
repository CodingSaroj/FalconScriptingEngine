#ifndef FALCON_IJIT_HPP
#define FALCON_IJIT_HPP

namespace Falcon
{
    class JIT
    {
        protected:
            std::vector<uint8_t> machineCode;

        public:
            JIT();
            virtual ~JIT() = default;

            std::vector<uint8_t> getMachineCode();

            void clear();

            virtual void add8() = 0;
            virtual void add64u() = 0;
            virtual void add64l() = 0;
            virtual void add64f() = 0;
            virtual void sub8() = 0;
            virtual void sub64u() = 0;
            virtual void sub64l() = 0;
            virtual void sub64f() = 0;
            virtual void mul8() = 0;
            virtual void mul64u() = 0;
            virtual void mul64l() = 0;
            virtual void mul64f() = 0;
            virtual void div64c() = 0;
            virtual void div64u() = 0;
            virtual void div64l() = 0;
            virtual void div64f() = 0;
            virtual void mod8() = 0;
            virtual void mod64u() = 0;
            virtual void mod64l() = 0;
            virtual void inc8() = 0;
            virtual void inc64u() = 0;
            virtual void inc64l() = 0;
            virtual void inc64f() = 0;
            virtual void dec8() = 0;
            virtual void dec64u() = 0;
            virtual void dec64l() = 0;
            virtual void dec64f() = 0;
    };
}

#endif
