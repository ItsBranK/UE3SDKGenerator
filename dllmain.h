#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <inttypes.h>
#include <stdint.h>
#include <direct.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <filesystem>

#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include "Printers.h"
#include "Engine/Rocket League/GameDefines.h"
#include "Engine/Rocket League/ObjectFunctions.h"
#include "Engine/Rocket League/PiecesOfCode.h"
#include "Engine/Rocket League/Configuration.h"

/*
	Current Changes in v2.0.9:
	- Changed how GName instances are dumped.
	- Increased FNameEntry's buffer.
	- Added GetName, GetIndex, and GetWideName functions to the FNameEntry struct.
	- Updated pieces of code.

	TO-DO:
	- Fully reverse FScriptDelegate.
	- Figure out how TMap (UMapProperty) works, both key and value are null.
	- Figure out why some shit is off by 4 bytes seemingly randomly, happens on both x32 and x64 modes.
*/

namespace Utils
{
	MODULEINFO GetModuleInfo(LPCTSTR lpModuleName);
	uintptr_t FindPattern(HMODULE module, const unsigned char* pattern, const char* mask);
	bool MapExists(std::multimap<std::string, std::string>& map, std::string& key, std::string& value);
	bool SortPropertyPair(std::pair<UProperty*, std::string> uPropertyA, std::pair<UProperty*, std::string> uPropertyB);
	bool SortProperty(UProperty* uPropertyA, UProperty* uPropertyB);
	bool IsStructProperty(EPropertyTypes propertyType);
	bool IsBitField(EPropertyTypes propertyType);
	bool IsBitField(unsigned long dimension);
	bool AreGObjectsValid();
	bool AreGNamesValid();
}

namespace Retrievers
{
	void GetAllPropertyFlags(std::ostringstream& stream, uint64_t propertyFlags);
	void GetAllFunctionFlags(std::ostringstream& stream, uint64_t functionFlags);
	EPropertyTypes GetPropertyType(UProperty* uProperty, std::string& uPropertyType, bool returnFunction);
	size_t GetPropertySize(UProperty* uProperty);
}

namespace StructGenerator
{
	UScriptStruct* FindLargestStruct(const std::string& structFullName);
	void GenerateStruct(File& file, UScriptStruct* uScriptStruct);
	void GenerateStructProperties(File& file, UScriptStruct* uScriptStruct, UObject* pPackageObj);
	void ProcessStructs(File& file, UObject* uPackageObj);
}

namespace ConstGenerator
{
	void GenerateConst(File& file, UConst* uConst);
	void ProcessConsts(File& file, UObject* uPackageObj);
}

namespace EnumGenerator
{
	void GenerateEnum(File& file, UEnum* uEnum);
	void ProcessEnums(File& file, UObject* uPackageObj);
}

namespace ClassGenerator
{
	void GenerateClass(File& file, UClass* uClass);
	void GenerateClassProperties(File& file, UClass* uClass, UObject* uPackageObj);
	void ProcessClasses(File& file, UObject* uPackageObj);
}

namespace ParameterGenerator
{
	void GenerateParameter(File& file,  UClass* uClass);
	void ProcessParameters(File& file, UObject* uPackageObj);
}

namespace FunctionGenerator
{
	void GenerateVirtualFunctions(File& file);
	void GenerateFunctionCode(File& file, UClass* uClass);
	void GenerateFunctionDescription(File& file,  UClass* uClass);
	void ProcessFunctions(File& file, UObject* uPackageObj);
}

namespace Generator
{
	extern File LogFile;
	extern std::vector<std::pair<std::string, int>> vConstants;
	extern std::vector<UObject*> vIncludes;

	std::string GenerateValidName(const std::string& invalidName);
	std::string GenerateUniqueName(UClass* uClass);
	std::string GenerateUniqueName(UFunction* uFunction, UClass* uClass);
	std::string GenerateIndexName(UObject* uObject, bool pushBack);
	void MakeWindowsFunction(std::string& functionName);

	void GenerateConstants();
	void GenerateHeaders();
	void GenerateDefines();
	void ProcessPackages();
	void GenerateSDK();
	void DumpInstances(bool dumpGNames, bool dumpGObjects, bool dumpGObjectsFull);
	void Initialize(bool createLogFile);
}