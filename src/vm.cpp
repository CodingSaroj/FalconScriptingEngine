#include "vm.hpp"

#define OP_BIN_FUNC(name, sign) \
static void op_##name(Falcon::VM & vm)\
{\
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();\
\
    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1);\
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2);\
\
    if (arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            arg1.value.c = arg1.value.c sign arg2.value.c;\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            arg1.value.c = arg1.value.c sign arg2.value.u;\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            arg1.value.c = arg1.value.c sign arg2.value.l;\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            arg1.value.c = arg1.value.c sign arg2.value.f;\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            arg1.value.u = arg1.value.u sign arg2.value.c;\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            arg1.value.u = arg1.value.u sign arg2.value.u;\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            arg1.value.u = arg1.value.u sign arg2.value.l;\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            arg1.value.u = arg1.value.u sign arg2.value.f;\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            arg1.value.l = arg1.value.l sign arg2.value.c;\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            arg1.value.l = arg1.value.l sign arg2.value.u;\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            arg1.value.l = arg1.value.l sign arg2.value.l;\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            arg1.value.l = arg1.value.l sign arg2.value.f;\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            arg1.value.f = arg1.value.f sign arg2.value.c;\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            arg1.value.f = arg1.value.f sign arg2.value.u;\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            arg1.value.f = arg1.value.f sign arg2.value.l;\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            arg1.value.f = arg1.value.f sign arg2.value.f;\
        }\
    }\
}\

#define OP_BIN_FUNC_NO_FLOAT(name, sign) \
static void op_##name(Falcon::VM & vm)\
{\
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();\
\
    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1);\
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2);\
\
    if (arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            arg1.value.c = arg1.value.c sign arg2.value.c;\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            arg1.value.c = arg1.value.c sign arg2.value.u;\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            arg1.value.c = arg1.value.c sign arg2.value.l;\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            arg1.value.u = arg1.value.u sign arg2.value.c;\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            arg1.value.u = arg1.value.u sign arg2.value.u;\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            arg1.value.u = arg1.value.u sign arg2.value.l;\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            arg1.value.l = arg1.value.l sign arg2.value.c;\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            arg1.value.l = arg1.value.l sign arg2.value.u;\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            arg1.value.l = arg1.value.l sign arg2.value.l;\
        }\
    }\
}\

#define OP_UN_FUNC(name, sign) \
static void op_##name(Falcon::VM & vm)\
{\
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();\
\
    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1);\
\
    if (arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3)\
    {\
        arg1.value.c = sign arg1.value.c;\
    }\
    else if (arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3)\
    {\
        arg1.value.u = sign arg1.value.u;\
    }\
    else if (arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3)\
    {\
        arg1.value.l = sign arg1.value.l;\
    }\
    else if (arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3)\
    {\
        arg1.value.f = sign arg1.value.f;\
    }\
}\

#define OP_UN_FUNC_NO_FLOAT(name, sign) \
static void op_##name(Falcon::VM & vm)\
{\
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();\
\
    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1);\
\
    if (arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3)\
    {\
        arg1.value.c = sign arg1.value.c;\
    }\
    else if (arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3)\
    {\
        arg1.value.u = sign arg1.value.u;\
    }\
    else if (arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3)\
    {\
        arg1.value.l = sign arg1.value.l;\
    }\
}\

#define OP_BIN_C_FUNC(name, sign, id) \
static void op_##name(Falcon::VM & vm)\
{\
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();\
\
    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1);\
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2);\
\
    if (arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            vm.setCmpResult(id, arg1.value.c sign arg2.value.c);\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            vm.setCmpResult(id, arg1.value.c sign arg2.value.u);\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            vm.setCmpResult(id, arg1.value.c sign arg2.value.l);\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            vm.setCmpResult(id, arg1.value.c sign arg2.value.f);\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            vm.setCmpResult(id, arg1.value.u sign arg2.value.c);\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            vm.setCmpResult(id, arg1.value.u sign arg2.value.u);\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            vm.setCmpResult(id, arg1.value.u sign arg2.value.l);\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            vm.setCmpResult(id, arg1.value.u sign arg2.value.f);\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            vm.setCmpResult(id, arg1.value.l sign arg2.value.c);\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            vm.setCmpResult(id, arg1.value.l sign arg2.value.u);\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            vm.setCmpResult(id, arg1.value.l sign arg2.value.l);\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            vm.setCmpResult(id, arg1.value.l sign arg2.value.f);\
        }\
    }\
    else if (arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3)\
    {\
        if (arg2.type >= Falcon::REGISTER_C0 && arg2.type <= Falcon::REGISTER_C3)\
        {\
            vm.setCmpResult(id, arg1.value.f sign arg2.value.c);\
        }\
        else if (arg2.type >= Falcon::REGISTER_U0 && arg2.type <= Falcon::REGISTER_U3)\
        {\
            vm.setCmpResult(id, arg1.value.f sign arg2.value.u);\
        }\
        else if (arg2.type >= Falcon::REGISTER_L0 && arg2.type <= Falcon::REGISTER_L3)\
        {\
            vm.setCmpResult(id, arg1.value.f sign arg2.value.l);\
        }\
        else if (arg2.type >= Falcon::REGISTER_F0 && arg2.type <= Falcon::REGISTER_F3)\
        {\
            vm.setCmpResult(id, arg1.value.f sign arg2.value.f);\
        }\
    }\
}\

#define OP_UN_C_FUNC(name, sign, id) \
static void op_##name(Falcon::VM & vm)\
{\
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();\
\
    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1);\
\
    if (arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3)\
    {\
        vm.setCmpResult(id, sign arg1.value.c);\
    }\
    else if (arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3)\
    {\
        vm.setCmpResult(id, sign arg1.value.u);\
    }\
    else if (arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3)\
    {\
        vm.setCmpResult(id, sign arg1.value.l);\
    }\
    else if (arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3)\
    {\
        vm.setCmpResult(id, sign arg1.value.f);\
    }\
}\

OP_BIN_FUNC(add, +)
OP_BIN_FUNC(sub, -)
OP_BIN_FUNC(mul, *)
OP_BIN_FUNC(div, /)
OP_BIN_FUNC_NO_FLOAT(mod, %)
OP_UN_FUNC(inc, ++)
OP_UN_FUNC(dec, --)

OP_BIN_FUNC_NO_FLOAT(lshft, <<)
OP_BIN_FUNC_NO_FLOAT(rshft, >>)
OP_BIN_FUNC_NO_FLOAT(and, &)
OP_BIN_FUNC_NO_FLOAT(or, |)
OP_BIN_FUNC_NO_FLOAT(xor, ^)
OP_UN_FUNC_NO_FLOAT(cmpl, ~)

OP_BIN_C_FUNC(grt0, >, 0)
OP_BIN_C_FUNC(grt1, >, 1)
OP_BIN_C_FUNC(greq0, >=, 0)
OP_BIN_C_FUNC(greq1, >=, 1)
OP_BIN_C_FUNC(less0, <, 0)
OP_BIN_C_FUNC(less1, <, 1)
OP_BIN_C_FUNC(lseq0, <=, 0)
OP_BIN_C_FUNC(lseq1, <=, 1)
OP_BIN_C_FUNC(iseq0, ==, 0)
OP_BIN_C_FUNC(iseq1, ==, 1)
OP_BIN_C_FUNC(neq0, !=, 0)
OP_BIN_C_FUNC(neq1, !=, 1)
OP_UN_C_FUNC(not0, !, 0)
OP_UN_C_FUNC(not1, !, 1)

static void op_cand(Falcon::VM & vm)
{
    vm.setCmpResult(0, vm.getCmpResult(0) && vm.getCmpResult(1));
}

static void op_cor(Falcon::VM & vm)
{
    vm.setCmpResult(0, vm.getCmpResult(0) || vm.getCmpResult(1));
}

static void op_if(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    if (vm.getCmpResult(0))
    {
        uint8_t bytes[] =
        {
            inst.arg1_offset,
            inst.extra.arg2_offset
        };

        vm.run(vm.getSymbol(*(uint16_t *)bytes));
    }
}

static void op_else(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    if (!vm.getCmpResult(0))
    {
        uint8_t bytes[] =
        {
            inst.arg1_offset,
            inst.extra.arg2_offset
        };

        vm.run(vm.getSymbol(*(uint16_t *)bytes));
    }
}

static void op_call(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    uint8_t bytes[] =
    {
        inst.arg1_offset,
        inst.extra.arg2_offset
    };

    vm.run(vm.getSymbol(*(uint16_t *)bytes));
}

static void op_jmp(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    vm.setJmp(!inst.arg1 ? true : false);
}

Falcon::VM::VM(const std::string & __bytecode)
    : instructionPtr(0), stack_pointer(0), advanceOnly(false), jmpStart(false), jmpEnd(false)
{
    for (int i = 0; i < 128; i++)
    {
        this->stack[i] = 0;
    }
    
    for (int i = 0; i < 20; i++)
    {
        if (i < 2)
        {
            this->cmpResult[i] = 0;
        }
        this->registers[i].type = (RegisterType)i;
        this->registers[i].value.u = 0;
    }

    for (int i = 0; i < 47; i++)
    {
        this->operators[i] = 0;
    }

    this->operators[INSTRUCTION_ADD]    = op_add;
    this->operators[INSTRUCTION_SUB]    = op_sub;
    this->operators[INSTRUCTION_MUL]    = op_mul;
    this->operators[INSTRUCTION_DIV]    = op_div;
    this->operators[INSTRUCTION_MOD]    = op_mod;
    this->operators[INSTRUCTION_INC]    = op_inc;
    this->operators[INSTRUCTION_DEC]    = op_dec;

    this->operators[INSTRUCTION_LSHFT]  = op_lshft;
    this->operators[INSTRUCTION_RSHFT]  = op_rshft;
    this->operators[INSTRUCTION_AND]    = op_and;
    this->operators[INSTRUCTION_OR]     = op_or;
    this->operators[INSTRUCTION_XOR]    = op_xor;
    this->operators[INSTRUCTION_CMPL]   = op_cmpl;

    this->operators[INSTRUCTION_IF]     = op_if;
    this->operators[INSTRUCTION_ELSE]   = op_else;
    this->operators[INSTRUCTION_GRT0]   = op_grt0;
    this->operators[INSTRUCTION_GRT1]   = op_grt1;
    this->operators[INSTRUCTION_GREQ0]  = op_greq0;
    this->operators[INSTRUCTION_GREQ1]  = op_greq1;
    this->operators[INSTRUCTION_LESS0]  = op_less0;
    this->operators[INSTRUCTION_LESS1]  = op_less1;
    this->operators[INSTRUCTION_LSEQ0]  = op_lseq0;
    this->operators[INSTRUCTION_LSEQ1]  = op_lseq1;
    this->operators[INSTRUCTION_ISEQ0]  = op_iseq0;
    this->operators[INSTRUCTION_ISEQ1]  = op_iseq1;
    this->operators[INSTRUCTION_NEQ0]   = op_neq0;
    this->operators[INSTRUCTION_NEQ1]   = op_neq1;
    this->operators[INSTRUCTION_NOT0]   = op_not0;
    this->operators[INSTRUCTION_NOT1]   = op_not1;
    this->operators[INSTRUCTION_CAND]   = op_cand;
    this->operators[INSTRUCTION_COR]    = op_cor;

    this->operators[INSTRUCTION_CALL]   = op_call;
    this->operators[INSTRUCTION_JMP]    = op_jmp;

    this->compile(__bytecode);
}

void Falcon::VM::compile(std::string __bytecode)
{
    for (int i = 0; i < __bytecode.size(); i += 4)
    {
        Internal::Instruction inst;
        inst.type               = (InstructionType)((__bytecode[i] & 0b11111100) >> 2);
        if (inst.type == INSTRUCTION_SYMBOL)
        {
            inst.arg1_offset        = __bytecode[i + 1];
            inst.extra.arg2_offset  = __bytecode[i + 2];

            char c = '\0';
            int count = 0;

            do
            {
                c = __bytecode[i + (count + 3)];
                inst.symbol.push_back(c);
                count++;
            }
            while (c != 0);

            inst.symbol.pop_back();

            this->registerSymbol(*(uint16_t *)&__bytecode[i + 1], inst.symbol);

            if (count > 0)
            {
                i += --count;
            }
        }
        else if (inst.type == INSTRUCTION_START || inst.type == INSTRUCTION_IF || inst.type == INSTRUCTION_ELSE || inst.type == INSTRUCTION_CALL)
        {
            inst.arg1_offset        = __bytecode[i + 1];
            inst.extra.arg2_offset  = __bytecode[i + 2];
            if (inst.type == INSTRUCTION_START)
            {
                this->registerFunction(this->getSymbol(*(uint16_t *)&__bytecode[i + 1]), this->instructions.size() + 1);
            }
            i -= 1;
        }
        else if (inst.type == INSTRUCTION_JMP)
        {
            inst.arg1_offset = __bytecode[i + 1];

            i -= 2;
        }
        else if (inst.type == INSTRUCTION_END || inst.type == INSTRUCTION_CAND || inst.type == INSTRUCTION_COR)
        {
            i -= 3;
        }
        else
        {
            inst.arg1               = (RegisterType)(((__bytecode[i] & 0b00000011) << 3) | ((__bytecode[i + 1] & 0b11100000) >> 5));
            inst.arg1_offset        = (__bytecode[i + 1] & 0b00011111) | ((__bytecode[i + 2] & 0b11100000));
            inst.arg2               = (RegisterType)((__bytecode[i + 2] & 0b00011111));
            inst.extra.arg2_offset  = __bytecode[i + 3];
        }

        this->instructions.push_back(inst);
    }
}

uint64_t Falcon::VM::getInstructionPtr()
{
    return this->instructionPtr;
}

Falcon::Internal::Instruction & Falcon::VM::getCurrentInstruction()
{
    return this->instructions[this->instructionPtr - 1];
}

bool Falcon::VM::getCmpResult(uint8_t id)
{
    assert(id < 2);
    return this->cmpResult[id];
}

void Falcon::VM::setCmpResult(uint8_t id, bool res)
{
    assert(id < 2);
    this->cmpResult[id] = res;
}

void Falcon::VM::setJmp(bool start)
{
    if (start)
    {
        this->jmpStart = true;
    }
    else
    {
        this->jmpEnd = true;
    }
}

Falcon::Internal::Register & Falcon::VM::getRegister(RegisterType id)
{
    assert(id < 20);
    return this->registers[id];
}

void Falcon::VM::registerSymbol(uint16_t id, std::string name)
{
    this->symbols.insert(std::pair<uint16_t, std::string>(id, name));
}

std::string Falcon::VM::getSymbol(uint16_t id)
{
    return this->symbols[id];
}

void Falcon::VM::registerFunction(std::string name, uint64_t location)
{
    this->functions.insert(std::pair<std::string, std::pair<bool, uint64_t>>(name, std::pair<bool, uint64_t>(false, location)));
}

void Falcon::VM::externalFunction(std::string name, void( * function)(VM & vm))
{
    this->functions.insert(std::pair<std::string, std::pair<bool, uint64_t>>(name, std::pair<bool, uint64_t>(true, (uint64_t)function)));
}

void Falcon::VM::_push64(uint64_t data)
{
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[0];
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[1];
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[2];
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[3];
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[4];
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[5];
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[6];
    this->stack[this->stack_pointer++] = ((uint8_t *)&data)[7];
}

void Falcon::VM::pushChar(char data)
{
    this->stack[this->stack_pointer++] = (uint8_t)data;
}

void Falcon::VM::pushUint(uint64_t data)
{
    _push64(data);
}

void Falcon::VM::pushInt(int64_t data)
{
    _push64(*(uint64_t *)&data);
}

void Falcon::VM::pushFloat(double data)
{
    _push64(*(uint64_t *)&data);
}

char Falcon::VM::popChar()
{
    this->stack_pointer--;
    return *(char *)&this->stack[this->stack_pointer];
}

uint64_t Falcon::VM::popUint()
{
    this->stack_pointer -= 8;
    return *(uint64_t *)&this->stack[this->stack_pointer];
}

int64_t Falcon::VM::popInt()
{
    this->stack_pointer -= 8;
    return *(int64_t *)&this->stack[this->stack_pointer];
}

double Falcon::VM::popFloat()
{
    this->stack_pointer -= 8;
    return *(double *)&this->stack[this->stack_pointer];
}

void Falcon::VM::run(std::string function)
{
    uint64_t crsr = this->instructionPtr;
    std::pair<bool, uint64_t> & data = this->functions[function];
    if (data.first)
    {
        ((void( * )(VM &))data.second)(*this);
        return;
    }
    else
    {
        this->instructionPtr = data.second;
    }
    while (this->instructionPtr < this->instructions.size())
    {
        if (this->jmpStart)
        {
            this->instructionPtr = crsr;
            continue;
        }
        else if (this->jmpEnd)
        {
            this->instructionPtr = crsr;
            return;
        }
        this->advance();
        if (this->currenInstruction.type == INSTRUCTION_END)
        {
            break;
        }
        if (this->operators[this->currenInstruction.type])
        {
            this->operators[this->currenInstruction.type](*this);
        }
    }
    this->instructionPtr = crsr;
}
