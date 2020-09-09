#include "Reader.hpp"

namespace Falcon
{
    namespace FALI
    {
        Reader::Reader(const std::string & fileName)
        {
            std::vector<uint8_t> fileData;

            {
                std::ifstream in(fileName, std::ios_base::binary);

                in>>std::noskipws;

                std::istream_iterator<uint8_t> iter(in);
                std::istream_iterator<uint8_t> eos;

                fileData = std::move(std::vector<uint8_t>(iter, eos));

                in.close();
            }

            uint64_t offset = 0;

            if (std::string((char *)&fileData[offset], 4) != "FALI")
            {
                std::cout<<"File not FALI bytecode!\n";
            }

            offset += 4;
            
            {
                if (std::string((char *)&fileData[offset], 4) != "CODE")
                {
                    std::cout<<"No code section in file.\n";
                }

                offset += 4;

                uint64_t codeSectionSize = *(uint64_t *)&fileData[offset];

                Endian::LittleToSystem(codeSectionSize);

                offset += 8;

                readCodeSection(&fileData[offset], codeSectionSize);

                offset += codeSectionSize;
            }

            if (std::string((char *)&fileData[offset], 4) == "DEBG")
            {
                offset += 4;

                uint64_t debugSectionSize = *(uint64_t *)&fileData[offset];

                Endian::LittleToSystem(debugSectionSize);
                
                offset += 8;

                readDebugSection(&fileData[offset], debugSectionSize);

                offset += debugSectionSize;
            }

            if (std::string((char *)&fileData[offset], 4) == "REFL")
            {
                offset += 4;

                uint64_t reflSectionSize = *(uint64_t *)&fileData[offset];

                Endian::LittleToSystem(reflSectionSize);

                offset += 8;

                readReflectionSection(&fileData[offset], reflSectionSize);

                offset += reflSectionSize;
            }
        }

        void Reader::readCodeSection(uint8_t * start, uint64_t size)
        {
            m_Code = new uint8_t[size];
            memcpy(m_Code, start, size);
        }

        void Reader::readDebugSection(uint8_t * start, uint64_t size)
        {
            uint64_t offset = 0;

            while (offset < size)
            {
                std::string functionName((char *)&start[offset]);

                offset += functionName.size() + 1;

                offset++;

                {
                    std::string signature((char *)&start[offset]);

                    m_DebugData.FunctionData[functionName].Signature = signature;

                    offset += signature.size() + 1;

                    uint64_t startLine = *(uint64_t *)&start[offset];

                    Endian::LittleToSystem(startLine);

                    m_DebugData.FunctionData[functionName].StartLine = startLine;

                    offset += 8;

                    uint64_t endLine = *(uint64_t *)&start[offset];
                    
                    Endian::LittleToSystem(endLine);

                    m_DebugData.FunctionData[functionName].EndLine = endLine;

                    offset += 8;
                }

                offset++;

                while (start[offset] != 'V')
                {
                    uint64_t startLoc = *(uint64_t *)&start[offset];

                    Endian::LittleToSystem(startLoc);

                    offset += 8;
                    
                    uint64_t lineNum = *(uint64_t *)&start[offset];

                    Endian::LittleToSystem(lineNum);

                    offset += 8;

                    std::string lineData((char *)&start[offset]);

                    offset += lineData.size() + 1;

                    m_DebugData.LineData[startLoc] = { lineNum, lineData };
                }

                offset++;

                while (start[offset] != 'E')
                {
                    std::string name((char *)&start[offset]);

                    offset += name.size() + 1;

                    std::string type((char *)&start[offset]);

                    offset += type.size() + 1;

                    uint64_t stackOffset = *(uint64_t *)&start[offset];

                    Endian::LittleToSystem(stackOffset);

                    offset += 8;

                    m_DebugData.FunctionData[functionName].LocalVariables[name] = { type, stackOffset };
                }

                offset++;
            }
        }

        void Reader::readReflectionSection(uint8_t * start, uint64_t size)
        {
            uint64_t offset = 0;

            while (offset < size)
            {
                if (start[offset] == 'T')
                {
                    offset++;

                    std::string name((char *)&start[offset]);

                    offset += name.size() + 1;

                    std::vector<std::string> attribs;

                    while (start[offset] != 'T')
                    {
                        std::string attrib((char *)&start[offset]);

                        attribs.emplace_back(attrib);

                        offset += attrib.size() + 1;
                    }

                    offset++;

                    AttributeTable::New(name, attribs);
                }
                else if (start[offset] == 'F')
                {
                    offset++;

                    std::string retType((char *)&start[offset]);
                    
                    offset += retType.size() + 1;

                    std::string name((char *)&start[offset]);

                    offset += name.size() + 1;

                    std::vector<std::string> params;

                    while (start[offset] != 'E')
                    {
                        std::string param((char *)&start[offset]);

                        params.emplace_back(param);

                        offset += param.size() + 1;
                    }

                    offset++;

                    Function::AddFunction(retType, name, params);
                }
                else if (start[offset] == 'S')
                {
                    offset++;

                    std::string name((char *)&start[offset]);
                    
                    offset += name.size() + 1;

                    std::vector<std::pair<std::string, std::string>> members;

                    while (start[offset] != 'E')
                    {
                        std::string type((char *)&start[offset]);

                        offset += type.size() + 1;

                        std::string name((char *)&start[offset]);

                        offset += name.size() + 1;

                        members.emplace_back(type, name);
                    }

                    offset++;

                    Object::New(name);
                    
                    for (auto member : members)
                    {
                        Object::AddMember(name, member.first, member.second);
                    }
                }
                else if (start[offset] == 'A')
                {
                    offset++;

                    std::string name((char *)&start[offset]);

                    offset += name.size() + 1;

                    std::string base((char *)&start[offset]);

                    offset += base.size() + 1;

                    offset++;

                    Object::New(name, true);

                    Object::AddMember(name, base, "base");
                }
            }
        }
        
        uint8_t * Reader::GetCode()
        {
            return m_Code;
        }

        DebugData Reader::GetDebugData()
        {
            return m_DebugData;
        }
    }
}
