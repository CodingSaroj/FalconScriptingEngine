#include "vm.hpp"

static void op_add(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c + arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u + arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l + arg2.value.l;
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        arg1.value.f = arg1.value.f + arg2.value.f;
    }
}


static void op_sub(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c - arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u - arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l - arg2.value.l;
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        arg1.value.f = arg1.value.f - arg2.value.f;
    }
}


static void op_mul(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c * arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u * arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l * arg2.value.l;
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        arg1.value.f = arg1.value.f * arg2.value.f;
    }
}


static void op_div(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c / arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u / arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l / arg2.value.l;
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        arg1.value.f = arg1.value.f / arg2.value.f;
    }
}


static void op_mod(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c % arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u % arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l % arg2.value.l;
    }
}


static void op_inc(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = ++ arg1.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = ++ arg1.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = ++ arg1.value.l;
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        arg1.value.f = ++ arg1.value.f;
    }
}


static void op_dec(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = -- arg1.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = -- arg1.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = -- arg1.value.l;
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        arg1.value.f = -- arg1.value.f;
    }
}



static void op_lshft(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c << arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u << arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l << arg2.value.l;
    }
}


static void op_rshft(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c >> arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u >> arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l >> arg2.value.l;
    }
}


static void op_and(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c & arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u & arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l & arg2.value.l;
    }
}


static void op_or(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c | arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u | arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l | arg2.value.l;
    }
}


static void op_xor(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        arg1.value.c = arg1.value.c ^ arg2.value.c;
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        arg1.value.u = arg1.value.u ^ arg2.value.u;
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        arg1.value.l = arg1.value.l ^ arg2.value.l;
    }
}


static void op_cmpl(Falcon::VM & vm)
{
  Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

  Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);

  if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
  {
      arg1.value.c = ~ arg1.value.c;
  }
  else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
  {
      arg1.value.u = ~ arg1.value.u;
  }
  else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
  {
      arg1.value.l = ~ arg1.value.l;
  }
}



static void op_grt0(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(0, arg1.value.c > arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(0, arg1.value.u > arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(0, arg1.value.l > arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(0, arg1.value.f > arg2.value.f);
    }
}


static void op_grt1(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(1, arg1.value.c > arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(1, arg1.value.u > arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(1, arg1.value.l > arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(1, arg1.value.f > arg2.value.f);
    }
}


static void op_greq0(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(0, arg1.value.c >= arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(0, arg1.value.u >= arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(0, arg1.value.l >= arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(0, arg1.value.f >= arg2.value.f);
    }
}


static void op_greq1(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(1, arg1.value.c >= arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(1, arg1.value.u >= arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(1, arg1.value.l >= arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(1, arg1.value.f >= arg2.value.f);
    }
}


static void op_less0(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(0, arg1.value.c < arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(0, arg1.value.u < arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(0, arg1.value.l < arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(0, arg1.value.f < arg2.value.f);
    }
}


static void op_less1(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(1, arg1.value.c < arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(1, arg1.value.u < arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(1, arg1.value.l < arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(1, arg1.value.f < arg2.value.f);
    }
}


static void op_lseq0(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(0, arg1.value.c <= arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(0, arg1.value.u <= arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(0, arg1.value.l <= arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(0, arg1.value.f <= arg2.value.f);
    }
}


static void op_lseq1(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(1, arg1.value.c <= arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(1, arg1.value.u <= arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(1, arg1.value.l <= arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(1, arg1.value.f <= arg2.value.f);
    }
}


static void op_iseq0(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(0, arg1.value.c == arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(0, arg1.value.u == arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(0, arg1.value.l == arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(0, arg1.value.f == arg2.value.f);
    }
}


static void op_iseq1(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(1, arg1.value.c == arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(1, arg1.value.u == arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(1, arg1.value.l == arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(1, arg1.value.f == arg2.value.f);
    }
}


static void op_neq0(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(0, arg1.value.c != arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(0, arg1.value.u != arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(0, arg1.value.l != arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(0, arg1.value.f != arg2.value.f);
    }
}


static void op_neq1(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & arg2 = vm.getRegister(inst.arg2, inst.extra.arg2_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(1, arg1.value.c != arg2.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(1, arg1.value.u != arg2.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(1, arg1.value.l != arg2.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(1, arg1.value.f != arg2.value.f);
    }
}

static void op_not0(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(0, ! arg1.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(0, ! arg1.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(0, ! arg1.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(0, ! arg1.value.f);
    }
}

static void op_not1(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & arg1 = vm.getRegister(inst.arg1, inst.arg1_offset);

    if ((arg1.type >= Falcon::REGISTER_C0 && arg1.type <= Falcon::REGISTER_C3) || arg1.type == Falcon::REGISTER_CSP)
    {
        vm.setCmpResult(1, ! arg1.value.c);
    }
    else if ((arg1.type >= Falcon::REGISTER_U0 && arg1.type <= Falcon::REGISTER_U3) || arg1.type == Falcon::REGISTER_USP)
    {
        vm.setCmpResult(1, ! arg1.value.u);
    }
    else if ((arg1.type >= Falcon::REGISTER_L0 && arg1.type <= Falcon::REGISTER_L3) || arg1.type == Falcon::REGISTER_LSP)
    {
        vm.setCmpResult(1, ! arg1.value.l);
    }
    else if ((arg1.type >= Falcon::REGISTER_F0 && arg1.type <= Falcon::REGISTER_F3) || arg1.type == Falcon::REGISTER_FSP)
    {
        vm.setCmpResult(1, ! arg1.value.f);
    }
}

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

        vm.gotoFunction(vm.getSymbol(*(uint16_t *)bytes));
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

        vm.gotoFunction(vm.getSymbol(*(uint16_t *)bytes));
    }
}

static void op_push(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & reg = vm.getRegister(inst.arg1, inst.arg1_offset);

    if ((reg.type >= Falcon::REGISTER_C0 && reg.type <= Falcon::REGISTER_C3) || reg.type == Falcon::REGISTER_CSP)
    {
        vm.pushChar(reg.value.c);
    }
    else if ((reg.type >= Falcon::REGISTER_U0 && reg.type <= Falcon::REGISTER_U3) || reg.type == Falcon::REGISTER_USP)
    {
        vm.pushUint(reg.value.u);
    }
    else if ((reg.type >= Falcon::REGISTER_L0 && reg.type <= Falcon::REGISTER_L3) || reg.type == Falcon::REGISTER_LSP)
    {
        vm.pushChar(reg.value.l);
    }
    else if ((reg.type >= Falcon::REGISTER_F0 && reg.type <= Falcon::REGISTER_F3) || reg.type == Falcon::REGISTER_FSP)
    {
        vm.pushChar(reg.value.f);
    }
}

static void op_pop(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & reg = vm.getRegister(inst.arg1, inst.arg1_offset);

    if ((reg.type >= Falcon::REGISTER_C0 && reg.type <= Falcon::REGISTER_C3) || reg.type == Falcon::REGISTER_CSP)
    {
        reg.value.c = vm.popChar();
    }
    else if ((reg.type >= Falcon::REGISTER_U0 && reg.type <= Falcon::REGISTER_U3) || reg.type == Falcon::REGISTER_USP)
    {
        reg.value.u = vm.popUint();
    }
    else if ((reg.type >= Falcon::REGISTER_L0 && reg.type <= Falcon::REGISTER_L3) || reg.type == Falcon::REGISTER_LSP)
    {
        reg.value.l = vm.popInt();
    }
    else if ((reg.type >= Falcon::REGISTER_F0 && reg.type <= Falcon::REGISTER_F3) || reg.type == Falcon::REGISTER_FSP)
    {
        reg.value.f = vm.popFloat();
    }
    else if (reg.type == Falcon::REGISTER_NULL)
    {
        vm.popUint();
    }
}

static void op_mov(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & reg = vm.getRegister(inst.arg1, inst.arg1_offset);
    
    if ((reg.type >= Falcon::REGISTER_C0 && reg.type <= Falcon::REGISTER_C3) || reg.type == Falcon::REGISTER_USP)
    {
        reg.value.c = inst.extra.arg2_offset;
    }
    else if ((reg.type >= Falcon::REGISTER_U0 && reg.type <= Falcon::REGISTER_U3) || reg.type == Falcon::REGISTER_USP)
    {
        reg.value.u = inst.extra.u;
    }
    else if ((reg.type >= Falcon::REGISTER_L0 && reg.type <= Falcon::REGISTER_L3) || reg.type == Falcon::REGISTER_USP)
    {
        reg.value.l = inst.extra.l;
    }
    else if ((reg.type >= Falcon::REGISTER_F0 && reg.type <= Falcon::REGISTER_F3) || reg.type == Falcon::REGISTER_USP)
    {
        reg.value.f = inst.extra.f;
    }
}

static void op_movr(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    Falcon::Internal::Register & reg0 = vm.getRegister(inst.arg1, inst.arg1_offset);
    Falcon::Internal::Register & reg1 = vm.getRegister(inst.arg1, inst.arg1_offset);

    if ((reg0.type >= Falcon::REGISTER_C0 && reg0.type <= Falcon::REGISTER_C3) || reg0.type == Falcon::REGISTER_USP)
    {
        reg0.value.c = reg1.value.c;
    }
    else if ((reg0.type >= Falcon::REGISTER_U0 && reg0.type <= Falcon::REGISTER_U3) || reg0.type == Falcon::REGISTER_USP)
    {
        reg0.value.u = reg1.value.u;
    }
    else if ((reg0.type >= Falcon::REGISTER_L0 && reg0.type <= Falcon::REGISTER_L3) || reg0.type == Falcon::REGISTER_USP)
    {
        reg0.value.l = reg1.value.l;
    }
    else if ((reg0.type >= Falcon::REGISTER_F0 && reg0.type <= Falcon::REGISTER_F3) || reg0.type == Falcon::REGISTER_USP)
    {
        reg0.value.f = reg1.value.f;
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

    vm.gotoFunction(vm.getSymbol(*(uint16_t *)bytes));
}

static void op_jmp(Falcon::VM & vm)
{
    Falcon::Internal::Instruction inst = vm.getCurrentInstruction();

    vm.setJmp(!inst.arg1 ? true : false);
}

Falcon::VM::VM(const std::string & __bytecode)
    : instructionPtr(0), stack_pointer(0), advanceOnly(false), jmpStart(false), jmpEnd(false)
{
    for (int i = 0; i < 8192; i++)
    {
        this->stack[i].value.u = 0;
    }
    
    for (int i = 0; i < 17; i++)
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

    this->operators[INSTRUCTION_PUSH]   = op_push;
    this->operators[INSTRUCTION_POP]    = op_pop;

    this->operators[INSTRUCTION_MOV]    = op_mov;
    this->operators[INSTRUCTION_MOVR]   = op_movr;

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
        else if (inst.type == INSTRUCTION_PUSH || inst.type == INSTRUCTION_POP)
        {
            inst.arg1               = (RegisterType)(((__bytecode[i] & 0b00000011) << 3) | ((__bytecode[i + 1] & 0b11100000) >> 5));
            inst.arg1_offset        = ((__bytecode[i + 1] & 0b00011111) << 3) | ((__bytecode[i + 2] & 0b11100000) >> 5);
            inst.arg2               = (RegisterType)((__bytecode[i + 2] & 0b00011111));
            
            i -= 1;
        }
        else if (inst.type == INSTRUCTION_MOV)
        {
            inst.arg1               = (RegisterType)(((__bytecode[i] & 0b00000011) << 3) | ((__bytecode[i + 1] & 0b11100000) >> 5));
            inst.arg1_offset        = ((__bytecode[i + 1] & 0b00011111) << 3) | ((__bytecode[i + 2] & 0b11100000) >> 5);
            inst.arg2               = (RegisterType)((__bytecode[i + 2] & 0b00011111));

            if (inst.arg1 >= REGISTER_C0 && inst.arg1 <= REGISTER_C3)
            {
                inst.extra.arg2_offset = __bytecode[i + 3];
            }
            else
            {
                uint8_t bytes[] =
                {
                    __bytecode[i + 3], __bytecode[i + 4], __bytecode[i + 5], __bytecode[i + 6],
                    __bytecode[i + 7], __bytecode[i + 8], __bytecode[i + 9], __bytecode[i + 10]
                };
                inst.extra.u = *(uint64_t *)bytes;
                i += 7;
            }
        }
        else
        {
            inst.arg1               = (RegisterType)(((__bytecode[i] & 0b00000011) << 3) | ((__bytecode[i + 1] & 0b11100000) >> 5));
            inst.arg1_offset        = ((__bytecode[i + 1] & 0b00011111) << 3) | ((__bytecode[i + 2] & 0b11100000) >> 5);
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

Falcon::Internal::Register & Falcon::VM::getRegister(RegisterType id, uint8_t offset)
{
    assert(id < 21);
    if (id > REGISTER_CSP && id < REGISTER_FSP)
    {
        Internal::Register & reg = this->stack[this->stack_pointer - offset];
        reg.type = id;
        return reg;
    }
    else
    {
        return this->registers[id];
    }
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

void Falcon::VM::pushChar(char data)
{
    this->stack[this->stack_pointer++].value.c = (uint8_t)data;
}

void Falcon::VM::pushUint(uint64_t data)
{
    this->stack[this->stack_pointer++].value.u = data;
}

void Falcon::VM::pushInt(int64_t data)
{
    this->stack[this->stack_pointer++].value.l = data;
}

void Falcon::VM::pushFloat(double data)
{
    this->stack[this->stack_pointer++].value.f = data;
}

char Falcon::VM::popChar()
{
    this->stack_pointer--;
    return this->stack[this->stack_pointer].value.c;
}

uint64_t Falcon::VM::popUint()
{
    this->stack_pointer--;
    return this->stack[this->stack_pointer].value.u;
}

int64_t Falcon::VM::popInt()
{
    this->stack_pointer--;
    return this->stack[this->stack_pointer].value.l;
}

double Falcon::VM::popFloat()
{
    this->stack_pointer--;
    return this->stack[this->stack_pointer].value.f;
}

void Falcon::VM::run(std::string function)
{
    this->stackFrame.push(this->instructionPtr);
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
            this->instructionPtr = this->stackFrame.top();
            continue;
        }
        else if (this->jmpEnd)
        {
            this->stackFrame.pop();
            this->instructionPtr = this->stackFrame.top();
        }
        this->advance();
        if (this->currentInstruction.type == INSTRUCTION_END)
        {
            if (this->stackFrame.size() == 1)
            {
                break;
            }
            else
            {
                this->instructionPtr = this->stackFrame.top();
                this->stackFrame.pop();
            }
        }
        if (this->operators[this->currentInstruction.type])
        {
            this->operators[this->currentInstruction.type](*this);
        }
    }
    this->instructionPtr = this->stackFrame.top();
    this->stackFrame.pop();
}

void Falcon::VM::gotoFunction(std::string function)
{
    std::pair<bool, uint64_t> func = this->functions[function];
    if (!func.first)
    {
        this->stackFrame.push(this->instructionPtr);
        this->instructionPtr = func.second;
    }
    else
    {
        ((void( * )(VM &))func.second)(*this);
    }
}
