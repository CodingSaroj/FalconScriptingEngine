/* Copyright (c) 2020 Saroj Kumar
 * All rights reserved.
 * 
 * This file is licensed under the MIT License.
 * See the "LICENSE" file at the root directory or https://mit-license.org for details.
 */
#include "FalconPCH.hpp"

#include "fali/SemanticAnalyzer.hpp"
#include "fali/Serialize.hpp"

#include "fcc/falconscript/Lexer.hpp"
#include "fcc/falconscript/Parser.hpp"

int main()
{
    Falcon::FCC::FalconScript::Lexer lexer(R"(
        void free(ptr p)
        {
        }

        namespace Std
        {
            class String
            {
            private:
                ptr m_Buffer;
                uint64 m_Size;

            public:
                oneone;

                constructor(tmp ptr str)
                {
                    m_Buffer = str;
                }

                destructor()
                {
                    free(m_Buffer);
                }

                ptr Data()
                {
                    return m_Buffer;
                }

                uint64 Size()
                {
                    return m_Size;
                }
            }
        }
        
        int32 main()
        {
            ptr p = 0;
            Std.String str = Std.String(p);
            uint64 size = str.Size();
        }
    )");

    Falcon::FCC::FalconScript::Parser parser(std::move(lexer.Lex()));

    auto module = std::move(parser.Parse());

    Falcon::FALI::Serialize(&module);

    Falcon::FALI::SemanticAnalyzer semanticAnalyzer(&module);

    if (semanticAnalyzer.Analyze())
    {
        std::cout<<"Semantic analysis passed!\n";
    }

    //auto functions = Falcon::FALI::ASTToIR(module);
    //
    //for (auto & function : functions)
    //{
    //    Falcon::FALI::Serialize(function);

    //    Falcon::FALI::Optimizations::ConstantFolding(function);

    //    Falcon::FALI::Serialize(function);

    //    Falcon::FALI::IRToAssembly irToASM(function);

    //    std::cout<<irToASM.Generate()<<"\n";
    //}
}
