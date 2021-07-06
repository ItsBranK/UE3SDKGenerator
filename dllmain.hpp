#pragma once
#include "pch.hpp"
#include "Printers.hpp"
#include "Engine/Engine.hpp"

/*
	TO-DO:
	- Improve the File class.
	- I reversed TMap but still have no idea how to determine what the key and value objects are, they are always null. So wtf?
	- Figure out why some shit is off by 4 bytes seemingly randomly, happens on both x32 and x64 modes.
*/

namespace Utils
{
	MODULEINFO GetModuleInfo(LPCTSTR lpModuleName);
	uintptr_t FindPattern(HMODULE module, const uint8_t* pattern, const char* mask);
	bool MapExists(std::multimap<std::string, std::string>& map, const std::string& key, const std::string& value);
	bool SortPropertyPair(std::pair<class UProperty*, std::string> uPropertyA, std::pair<class UProperty*, std::string> uPropertyB);
	bool SortProperty(class UProperty* uPropertyA, class UProperty* uPropertyB);
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
	EPropertyTypes GetPropertyType(class UProperty* uProperty, std::string& uPropertyType, bool returnFunction);
	size_t GetPropertySize(class UProperty* uProperty);
}

namespace StructGenerator
{
	UScriptStruct* FindLargestStruct(const std::string& structFullName);
	void GenerateStruct(File& file, class UScriptStruct* uScriptStruct);
	void GenerateStructProperties(File& file, class UScriptStruct* uScriptStruct, class UObject* pPackageObj);
	void ProcessStructs(File& file, class UObject* uPackageObj);
}

namespace ConstGenerator
{
	void GenerateConst(File& file, class UConst* uConst);
	void ProcessConsts(File& file, class UObject* uPackageObj);
}

namespace EnumGenerator
{
	void GenerateEnum(File& file, class UEnum* uEnum);
	void ProcessEnums(File& file, class UObject* uPackageObj);
}

namespace ClassGenerator
{
	void GenerateFields(std::ostringstream& classStream, class UClass* uClass, EClassTypes classType);
	void GenerateClass(File& file, class UClass* uClass);
	void GenerateClassProperties(File& file, class UClass* uClass, class UObject* uPackageObj);
	void ProcessClasses(File& file, class UObject* uPackageObj);
}

namespace ParameterGenerator
{
	void GenerateParameter(File& file, class UClass* uClass);
	void ProcessParameters(File& file, class UObject* uPackageObj);
}

namespace FunctionGenerator
{
	void GenerateVirtualFunctions(File& file);
	void GenerateFunctionCode(File& file, class UClass* uClass);
	void GenerateFunctionDescription(File& file, class UClass* uClass);
	void ProcessFunctions(File& file, class UObject* uPackageObj);
}

namespace Generator
{
	extern File LogFile;
	extern std::vector<std::pair<std::string, int32_t>> vConstants;
	extern std::vector<class UObject*> vIncludes;

	std::string GenerateValidName(const std::string& invalidName);
	std::string GenerateUniqueName(class UClass* uClass);
	std::string GenerateUniqueName(class UFunction* uFunction, class UClass* uClass);
	std::string GenerateIndexName(class UObject* uObject, bool pushBack);
	void MakeWindowsFunction(std::string& functionName);

	void GenerateConstants();
	void GenerateHeaders();
	void GenerateDefines();
	void ProcessPackages();
	void GenerateSDK();
	void DumpInstances(bool dumpGNames, bool dumpGObjects, bool dumpGObjectsFull);
	void Initialize(bool createLogFile);
}