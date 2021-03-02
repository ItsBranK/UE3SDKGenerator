#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

class File
{
private:
	std::ofstream Stream;

public:
	File() {}

	File(const std::string& directory, const std::string& name)
	{
		Create(directory, name);
	}

	~File()
	{
		Close();
	}

public:
	bool IsOpen()
	{
		return Stream.is_open();
	}

	void Create(const std::string& directory, const std::string& name)
	{
		Stream.open(directory + "\\" + name);
	}

	void Close()
	{
		if (IsOpen())
		{
			Stream.close();
		}
	}

	void Flush()
	{
		Stream << std::flush;
	}

	void Write(const std::string& string)
	{
		Stream << string;
	}

	void WriteLine(const std::string& string)
	{
		Stream << string << "\n";
	}

	void Write(std::ostringstream& stream)
	{
		Stream << stream.str();
		stream.str(std::string());
	}

	void NewLine()
	{
		Stream << "\n";
	}

	void Pad(const char& fill, int32_t size, bool left)
	{
		Stream << std::setfill(fill) << std::setw(size);

		if (left)
		{
			Stream << std::left;
		}
		else
		{
			Stream << std::right;
		}
	}

	void Hex(uintptr_t address, uint32_t size)
	{
		Stream << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(size) << std::right << address << std::uppercase;
	}

	void Decimal(uintptr_t address, uint32_t size)
	{
		Stream << std::dec << std::setfill('0') << std::setw(size) << std::right << address;
	}

	void Precision(uint32_t precision, float flt)
	{
		Stream << std::setprecision(precision) << flt;
	}
};

namespace Printers
{
	void PrintHeader(File& file, const std::string& fileName, const std::string& fileExtension, bool pragmaPackPush);
	void PrintSection(File& file, const std::string& sectionName);
	void PrintFooter(File& file, bool pragmaPackPop);

	void EmptyStream(std::ostringstream& stream);

	void MakeSpacer(std::ostringstream& stream, int32_t size);
	void MakeSpacer(std::ostringstream& stream, int32_t size, std::string string, bool left);
	void MakeSpacer(std::ostringstream& streamIn, std::ostringstream& streamOut, int32_t size);
	void MakeDecimal(std::ostringstream& stream, uintptr_t address, int32_t size);
	void MakeHex(std::ostringstream& stream, uintptr_t address, int32_t size);
}