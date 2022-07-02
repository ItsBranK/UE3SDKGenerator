#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>
#include <cstdarg>

namespace Printer
{
	void Empty(std::ostringstream& stream);
	void FillRight(std::ostringstream& stream, char fill, uint64_t width);
	void FillLeft(std::ostringstream& stream, char fill, uint64_t width);
	void FillRight(std::ofstream& stream, char fill, uint64_t width);
	void FillLeft(std::ofstream& stream, char fill, uint64_t width);
	std::string Hex(uintptr_t address, uint64_t width);
	std::string Decimal(uintptr_t address, uint64_t width);
	std::string Precision(float value, uint64_t precision);

	void Header(std::ostringstream& stream, const std::string& fileName, const std::string& fileExtension, bool pragmaPush);
	void Header(std::ofstream& stream, const std::string& fileName, const std::string& fileExtension, bool pragmaPush);
	void Section(std::ostringstream& stream, const std::string& sectionName);
	void Section(std::ofstream& stream, const std::string& sectionName);
	void Footer(std::ostringstream& stream, bool pragmaPop);
	void Footer(std::ofstream& stream, bool pragmaPop);
}