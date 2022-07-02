#include "Printer.hpp"
#include "Engine/Engine.hpp"

namespace Printer
{
    void Empty(std::ostringstream& stream)
    {
        stream.str(std::string());
    }

    void FillRight(std::ostringstream& stream, char fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ostringstream& stream, char fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::left;
    }

    void FillRight(std::ofstream& stream, char fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::right;
    }

    void FillLeft(std::ofstream& stream, char fill, uint64_t width)
    {
        stream <<  std::setfill(fill) << std::setw(width) << std::left;
    }

    std::string Hex(uintptr_t address, uint64_t width)
    {
        std::ostringstream stream;
        stream << "0x" << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::hex << address;
        return stream.str();
    }

    std::string Decimal(uintptr_t address, uint64_t width)
    {
        std::ostringstream stream;
        stream << std::setfill('0') << std::setw(width) << std::right << std::uppercase << std::dec << address;
        return stream.str();
    }

    std::string Precision(float value, uint64_t precision)
    {
        std::ostringstream stream;
        stream << std::setprecision(precision) << value;
        return stream.str();
    }

    void Header(std::ostringstream& stream, const std::string& fileName, const std::string& fileExtension, bool pragmaPush)
    {
        stream << "/*\n";
        stream << "#############################################################################################\n";
        stream << "# " << Configuration::GameName << " (" << Configuration::GameVersion + ") SDK\n";
        stream << "# Generated with the UE3SDKGenerator " << Engine::GeneratorVersion << "\n";
        stream << "# ========================================================================================= #\n";
        stream << "# File: " << fileName << "." << fileExtension << "\n";
        stream << "# ========================================================================================= #\n";
        stream << "# Credits: " << Engine::GeneratorCredits << "\n";
        stream << "# Links: " << Engine::GeneratorLinks << "\n";
        stream << "#############################################################################################\n";
        stream << "*/\n";

        if (fileName != "SdkHeaders")
        {
            if (fileExtension == "hpp" && fileName != "GameDefines")
            {
                stream << "#pragma once\n";

                if (Configuration::UsingConstants)
                {
                    stream << "#include \"../SdkConstants.hpp\"\n";
                }
            }
            else if (fileExtension == "cpp" && fileName != "GameDefines")
            {
                stream << "#include \"../SdkHeaders.hpp\"\n";
            }
        }

        if (pragmaPush)
        {
            stream << "\n#ifdef _MSC_VER\n";
            stream << "\t#pragma pack(push, 0x" + std::to_string(Configuration::FinalAlignment) + ")\n";
            stream << "#endif\n";
        }
    }

    void Header(std::ofstream& stream, const std::string& fileName, const std::string& fileExtension, bool pragmaPush)
    {
        std::ostringstream sStream;
        Header(sStream, fileName, fileExtension, pragmaPush);
        stream << sStream.str();
    }

    void Section(std::ostringstream& stream, const std::string& sectionName)
    {
        stream << "\n/*\n";
        stream << "# ========================================================================================= #\n";
        stream << "# " << sectionName << "\n";
        stream << "# ========================================================================================= #\n";
        stream << "*/\n\n";
    }

    void Section(std::ofstream& stream, const std::string& sectionName)
    {
        std::ostringstream sStream;
        Section(sStream, sectionName);
        stream << sStream.str();
    }

    void Footer(std::ostringstream& stream, bool pragmaPop)
    {
        stream << "/*\n";
        stream << "# ========================================================================================= #\n";
        stream << "#\n";
        stream << "# ========================================================================================= #\n";
        stream << "*/\n";

        if (pragmaPop)
        {
            stream << "\n#ifdef _MSC_VER\n";
            stream << "\t#pragma pack(pop)\n";
            stream << "#endif\n";
        }
    }

    void Footer(std::ofstream& stream, bool pragmaPop)
    {
        std::ostringstream sStream;
        Footer(sStream, pragmaPop);
        stream << sStream.str();
    }
}