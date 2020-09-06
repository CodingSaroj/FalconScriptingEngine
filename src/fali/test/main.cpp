#include <chrono>

#include "../Context.hpp"
#include "../Reader.hpp"

using namespace Falcon;
using namespace Falcon::FALI;

struct vec2 { double x, y; };

int main()
{
    Reader reader("hello.fali");

    FALI::Context ctxt(reader.GetCode());

    vec2 vc1{2.0, 3.0}, vc2{3.0, 2.0};

    Object v1("vec2", &vc1), v2("vec2", &vc2);

    std::cout<<"Before:\n";
    std::cout<<"    v1: "<<v1.get<double>("x")<<", "<<v1.get<double>("y")<<"\n";
    std::cout<<"    v2: "<<v2.get<double>("x")<<", "<<v2.get<double>("y")<<"\n";

    memcpy(v1.self(), v1.callMember("vec2", "vec2$add", {&v2}).self(), sizeof(vec2));
    
    std::cout<<"After:\n";
    std::cout<<"    v1: "<<v1.get<double>("x")<<", "<<v1.get<double>("y")<<"\n";
    std::cout<<"    v2: "<<v2.get<double>("x")<<", "<<v2.get<double>("y")<<"\n";
}
