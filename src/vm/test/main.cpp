#include <iostream>
#include <iomanip>
#include <chrono>

#include "../src/VM.hpp"

using namespace Falcon;

int main()
{
    uint8_t code[] =
    {
        OpCode::UMOV32, RegisterType::A32, 0x01, 0x00, 0x00, 0x00,
        OpCode::UPSH32, RegisterType::A32,
        OpCode::UPSH32, RegisterType::A32,
        OpCode::UMOV64, RegisterType::AO0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::UMOV64, RegisterType::AO1, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::ULSE32, RegisterType::SP, RegisterType::SP,
        OpCode::JMT,    0x2C, 0x00, 0x00, 0x00,
        OpCode::JMF,    0x71, 0x00, 0x00, 0x00,
        OpCode::STOP,

        OpCode::UMOV64, RegisterType::AO0, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::UMOV64, RegisterType::AO1, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::UMUL32, RegisterType::SP, RegisterType::SP,
        OpCode::UMOV64, RegisterType::AO0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::UINC32, RegisterType::SP,
        OpCode::UMOV64, RegisterType::AO0, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::UMOV64, RegisterType::AO1, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        OpCode::ULSE32, RegisterType::SP, RegisterType::SP,
        OpCode::JMT,    0x2C, 0x00, 0x00, 0x00,
        OpCode::JMF,    0x71, 0x00, 0x00, 0x00,
        OpCode::RET,
        
        OpCode::UPOP32, RegisterType::D32,
        OpCode::UGRT32, RegisterType::SP, RegisterType::SP,
        OpCode::RET,
    };

    VM vm(code);

    uint32_t in = 20;
    vm.push((uint8_t *)&in, 4);

    uint32_t res = 0;
    double time = 0.0;

    for (int i = 0; i < 1000; i++)
    {
        auto start = std::chrono::_V2::high_resolution_clock::now();
        vm.run(0);
        auto end = std::chrono::_V2::high_resolution_clock::now();
        auto duration = end - start;
        time += duration.count() / 1000000.0;
        res = *(uint32_t *)vm.pop(4);
    }

    std::cout<<"Result: "<<res<<" in "<<std::fixed<<time / 1000.0<<"ms\n";
}
