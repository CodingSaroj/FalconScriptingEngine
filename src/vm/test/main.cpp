#include <iostream>
#include <iomanip>
#include <chrono>

#include "../Debugger.hpp"

using namespace Falcon;

int main()
{
    uint8_t code[] =
    {
        'm', 'a', 'i', 'n', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        'f', 'a', 'c', 't', 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        OpCode::FUNC,

        OpCode::MOV64, RegisterType::R1, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::PSH64, RegisterType::R1,
        OpCode::CALL,   0x08, 0x00, 0x00, 0x00, 'f', 'a', 'c', 't', 0x00,
        OpCode::STOP,

        OpCode::FUNC,

        OpCode::MOV64, RegisterType::R1, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::PSH64, RegisterType::R1,
        OpCode::PSH64, RegisterType::R1,

        OpCode::LODREF, RegisterType::R1, 0x10, 0x00, 0x00, 0x00,
        OpCode::LODREF, RegisterType::R2, 0x00, 0x00, 0x00, 0x00,
        OpCode::ULSE64, 0b00000100 | RegisterType::R1, 0b00000100 | RegisterType::R2,
        OpCode::JMT,    0x49, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::POPNUL, 0x08, 0x00, 0x00, 0x00,
        OpCode::RET,    0x08, 0x00, 0x00, 0x00,

        OpCode::LODREF, RegisterType::R1, 0x08, 0x00, 0x00, 0x00,
        OpCode::LODREF, RegisterType::R2, 0x10, 0x00, 0x00, 0x00,
        OpCode::UMUL64, 0b00000100 | RegisterType::R1, 0b00000100 | RegisterType::R2,
        OpCode::LODREF, RegisterType::R1, 0x10, 0x00, 0x00, 0x00,
        OpCode::UINC64, 0b00000100 | RegisterType::R1,

        OpCode::JMP,    0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

        OpCode::FUNC,
        0xFF
    };

    /*Debugger vm(code, {
        {
            {1,  {1,  "    fact(20);"}},
            {4,  {2,  "}"}},
            {6,  {3,  "    u64 result = 1;"}},
            {9,  {5,  "    for (int i = 0; i < value; i++)"}},
            {13, {10, "    return result;"}},
            {15, {7,  "        result *= i;"}},
            {18, {5,  "    for (int i = 0; i < value; i++)"}},
        },
        {
            {"main", {"void main()", 1, 2}},
            {"fact", {"uint64 fact(uint64)", 3, 10, {{"value", {"uint64", 0}}, {"result", {"uint64", 8}}, {"i", {"uint64", 16}}}}}
        }
    }, [](const std::string & type, void * mem)->std::string
    {
        if (type == "uint64")
        {
            return std::to_string(*(uint64_t *)mem);
        }
        
        return "";
    });*/

    //vm.setBreakpoint(1);
    
    VM vm(code);
    
    double time = 0.0;
    
    for (uint64_t i = 0; i < 1000; i++)
    {
        auto s = std::chrono::high_resolution_clock::now();
        vm.run();
        auto e = std::chrono::high_resolution_clock::now();
        auto d = e - s;
        time += d.count() / 1000000.0;
    }

    std::cout<<"Finished in "<<std::fixed<<time / 1000.0<<"ms.\n";
}
