#include <chrono>

#include "../Reader.hpp"
#include "../Context.hpp"

using namespace Falcon;
using namespace Falcon::FALI;

int main()
{
    Reader reader("hello.flcn");

    Context ctxt(reader.GetCode());

    std::cout<<ctxt.call<uint64_t>("main")<<"\n";
}
