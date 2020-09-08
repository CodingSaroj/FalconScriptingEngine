#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>

#include "vm/Debugger.hpp"

#include "fali/Object.hpp"
#include "fali/Reader.hpp"

static std::array<std::string, 12> builtinTypes{"void", "ptr", "char", "uchar", "short", "ushort", "int", "uint", "long", "ulong", "float", "double"};

static struct
{
    std::string InputName = "a.fali";
} s_State;

void printHelp()
{
    std::cout<<"Usage: fldb [COMMAND_LINE_OPTIONS] [FILE]\n\n";
    std::cout<<"COMMAND_LINE_OPTIONS:\n";
    std::cout<<"    -h or --help    : Display this help and exit.\n";
    std::cout<<"FILE:\n";
    std::cout<<"    FALI file to be debugged.\n";
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
            s_State.InputName = str;
        }
    }
}

static std::string printVar(const std::string & type, void * data, const std::string & padding = "")
{
    std::string output;
    
    if (std::find(builtinTypes.begin(), builtinTypes.end(), type) != builtinTypes.end())
    {
        if (type == "ptr")
        {
            std::stringstream ss;

            ss<<std::hex<<*(uint64_t *)data;

            output = "0x" + ss.str();
        }
        else if (type == "char")
        {
            output = std::to_string(+*(char *)data) + " ";
            output += "'" + std::string((char *)data) + "'";
        }
        else if (type == "uchar")
        {
            output = std::to_string(+*(uint8_t *)data) + " ";
            output += "'" + std::string((char *)data) + "'";
        }
        else if (type == "short")
        {
            output = std::to_string(*(int16_t *)data);
        }
        else if (type == "ushort")
        {
            output = std::to_string(*(uint16_t *)data);
        }
        else if (type == "int")
        {
            output = std::to_string(*(uint32_t *)data);
        }
        else if (type == "uint")
        {
            output = std::to_string(*(int32_t *)data);
        }
        else if (type == "long")
        {
            output = std::to_string(*(uint64_t *)data);
        }
        else if (type == "ulong")
        {
            output = std::to_string(*(int64_t *)data);
        }
        else if (type == "float")
        {
            output = std::to_string(*(float *)data);
        }
        else if (type == "double")
        {
            output = std::to_string(*(double *)data);
        }
        else
        {
            output = "void";
        }
    }
    else if (Falcon::FALI::Object::IsValid(type))
    {
        if (Falcon::FALI::Object::GetObjectData(type).MemberOffsets.count("base") == 1)
        {
            output = printVar(Falcon::FALI::Object::GetObjectData(type).MemberOffsets["base"].first, data);
        }
        else
        {
            output = "{\n";

            for (auto member : Falcon::FALI::Object::GetMemberIterable(type))
            {
                output += padding + "    " + member.first + " = " + printVar(member.second.first, ((uint8_t *)data) + member.second.second, padding + "    ") + "\n";
            }
            
            output += padding + "}";
        }
    }
    else if (type.find_first_of('[') != std::string::npos)
    {
        std::string rawType(type.begin(), type.begin() + (type.find_first_of('[') - 1));

        if (!Falcon::FALI::Object::IsValid(rawType))
        {
            output = "Invalid type `" + rawType + "`";

            return output;
        }

        std::string sizeStr(type.begin() + (type.find_first_of('[') + 1), type.begin() + (type.find_first_of(']') - 1));

        uint64_t size = strtoul(sizeStr.c_str(), nullptr, 10);

        output = "[\n";

        for (uint64_t i = 0; i < size; i++)
        {
            output += padding + "    [" + std::to_string(i) + "] = " + printVar(rawType, ((uint8_t *)data) + (i * Falcon::FALI::Object::GetObjectData(rawType).Size), padding + "    ") + "\n";
        }
        
        output += padding + "]";
    }
    else
    {
        output = "Invalid type `" + type + "`";
    }

    return output;
}

int main(int argc, char * argv[])
{
    parseCmdArgs(argc, argv);

    Falcon::FALI::Reader reader(s_State.InputName);

    Falcon::Debugger debugger(
        reader.GetCode(),
        reader.GetDebugData(),
        Falcon::Debugger::DebuggerFunctions{Falcon::FALI::MangleFunction, Falcon::FALI::DemangleFunction, Falcon::FALI::MangleIdentifier, Falcon::FALI::DemangleIdentifier},
        "fldb",
        [](const std::string & type, void * data)->std::string
        {
            return printVar(type, data);
        }
    );

    debugger.shell();
}
