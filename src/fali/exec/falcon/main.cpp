#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>

#include "../../Object.hpp"

#include "../../Context.hpp"
#include "../../Reader.hpp"

static struct
{
    bool InputInitialized = false;

    std::string InputName = "a.fali";

    std::vector<std::string> Args;
} s_State;

void printHelp()
{
    std::cout<<"Usage: falcon [COMMAND_LINE_OPTIONS] [FILE] [ARGS]\n\n";
    std::cout<<"COMMAND_LINE_OPTIONS:\n";
    std::cout<<"    -h or --help    : Display this help and exit.\n";
    std::cout<<"FILE:\n";
    std::cout<<"    FALI file to be executed.\n";
    std::cout<<"ARGS:\n";
    std::cout<<"    Arguments to be passed to the `main` functoin.\n";
}

void parseCmdArgs(int argc, char * argv[])
{
    if (argc == 1)
    {
        printHelp();
        exit(2);
    }

    for (int i = 1; i < argc; i++)
    {
        std::string str(argv[i]);

        if (str == "-h" || str == "--help")
        {
            printHelp();
            exit(2);
        }
        else
        {
            if (s_State.InputInitialized)
            {
                s_State.Args.emplace_back(str);
            }
            else
            {
                s_State.InputName = str;
                s_State.InputInitialized = true;
            } 
        }
    }
}

int main(int argc, char * argv[])
{
    parseCmdArgs(argc, argv);

    Falcon::FALI::Reader reader(s_State.InputName);

    Falcon::FALI::Context ctxt(reader.GetCode());

    return ctxt.call<int>(Falcon::FALI::MangleFunction("main", {"uint", "ptr"}), (uint32_t)s_State.Args.size(), (uint64_t)s_State.Args.data());
}
