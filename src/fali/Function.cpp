#include "Function.hpp"

namespace Falcon
{
    namespace FALI
    {
        std::unordered_map<std::string, Function::FunctionData> Function::s_Functions;
        
        Function::Function(FunctionType function)
            : m_Function(function)
        {
        }

        void * Function::Call(std::vector<void *> args)
        {
            return m_Function(args);
        }

        void Function::AddFunction(const std::string & retType, const std::string & name, std::vector<std::string> paramTypes)
        {
            s_Functions[name] = {retType, name, paramTypes};
        }
        
        void Function::AddFunction(const std::string & retType, const std::string & name, std::vector<std::string> paramTypes, Function base)
        {
            s_Functions[name] = {retType, name, paramTypes, new Function(base)};
        }

        void Function::SetBase(const std::string & name, Function base)
        {
            s_Functions[name].Base = new Function(base);
        }
    }
}
