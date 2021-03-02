#include "Printers.h"
#include "Engine/Rocket League/Configuration.h"

namespace Printers
{
    void PrintHeader(File& file, const std::string& fileName, const std::string& fileExtension, bool pragmaPackPush)
    {
        file.WriteLine("/*");
        file.WriteLine("#############################################################################################");
        file.WriteLine("# " +  Configuration::GameName + " (" + Configuration::GameVersion + ") SDK");
        file.WriteLine("# Generated with the UE3SDKGenerator " + Configuration::GeneratorVersion);
        file.WriteLine("# ========================================================================================= #");
        file.WriteLine("# File: " + fileName + "." + fileExtension);
        file.WriteLine("# ========================================================================================= #");
        file.WriteLine("# Credits: " + Configuration::GeneratorCredits);
        file.WriteLine("# Links: " + Configuration::GeneratorLinks);
        file.WriteLine("#############################################################################################");
        file.WriteLine("*/");

        if (fileName != "SdkHeaders")
        {
            if (fileExtension == "h" && fileName != "GameDefines")
            {
                file.WriteLine("#pragma once");

                if (Configuration::UsingConstants)
                {
                    file.WriteLine("#include \"../SdkConstants.h\"");
                }
            }
            else if (fileExtension == "cpp" && fileName != "GameDefines")
            {
                file.WriteLine("#include \"../SdkHeaders.h\"");
            }
        }

        if (pragmaPackPush)
        {
            file.NewLine();
            file.WriteLine("#ifdef _MSC_VER");
            file.WriteLine("\t#pragma pack(push, 0x" + std::to_string(Configuration::Alignment) + ")");
            file.WriteLine("#endif");
        }
    }

    void PrintSection(File& file, const std::string& sectionName)
    {
        file.NewLine();
        file.WriteLine("/*");
        file.WriteLine("# ========================================================================================= #");
        file.WriteLine("# " + sectionName);
        file.WriteLine("# ========================================================================================= #");
        file.WriteLine("*/");
        file.NewLine();
    }

    void PrintFooter(File& file, bool pragmaPackPop)
    {
        file.WriteLine("/*");
        file.WriteLine("# ========================================================================================= #");
        file.WriteLine("#");
        file.WriteLine("# ========================================================================================= #");
        file.WriteLine("*/");

        if (pragmaPackPop)
        {
            file.NewLine();
            file.WriteLine("#ifdef _MSC_VER");
            file.WriteLine("\t#pragma pack(pop)");
            file.WriteLine("#endif");
        }
    }

    void EmptyStream(std::ostringstream& stream)
    {
        stream.str(std::string());
    }

    void MakeSpacer(std::ostringstream& stream, int32_t size)
    {
        stream << std::setfill(' ') << std::setw(size) << std::left;
    }

    void MakeSpacer(std::ostringstream& stream, int32_t size, std::string string, bool left)
    {
        stream << std::setfill(' ') << std::setw(size);
        
        if (left)
        {
            stream << std::left << string;
        }
        else
        {
            stream << std::right << string;
        }
    }

    void MakeSpacer(std::ostringstream& streamIn, std::ostringstream& streamOut, int32_t size)
    {
        streamIn << std::setfill(' ') << std::setw(size) << std::left << streamOut.str();
    }

    void MakeDecimal(std::ostringstream& stream, uintptr_t address, int32_t size)
    {
        stream << std::dec << std::setfill('0') << std::setw(size) << std::right << address;
    }

    void MakeHex(std::ostringstream& stream, uintptr_t address, int32_t size)
    {
        stream << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(size) << std::right << address << std::uppercase;
    }
}