#include <chrono>

#include "../Context.hpp"
#include "../Reader.hpp"

using namespace Falcon;
using namespace Falcon::FALI;

static bool Record = false;
static size_t Usage = 0;

struct vec2 { double x, y; };

void * operator new(size_t size)
{
    if (Record)
    {
        Usage += size;
    }

    return malloc(size);
}

int main()
{
    Object::New("vec2");
    Object::AddMember("vec2", "float32", "x");
    Object::AddMember("vec2", "float32", "y");

    float x = 1.0, y = 2.0;

    Record = true;
    Object obj("vec2");
    Record = false;

    std::cout<<Usage<<" bytes used.\n";
}
