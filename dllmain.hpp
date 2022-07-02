#pragma once
#include "pch.hpp"
#include "Printer.hpp"
#include "Engine/Engine.hpp"

namespace Utils
{
	void Messagebox(const std::string& message, const std::string& title, uint32_t flags);
	MODULEINFO GetModuleInfo(LPCTSTR moduleName);
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
	EPropertyTypes GetPropertyType(class UProperty* uProperty, std::string& propertyType, bool returnFunction);
	size_t GetPropertySize(class UProperty* uProperty);
	uintptr_t GetEntryPoint();
	uintptr_t GetOffset(uintptr_t address);
}

namespace ConstGenerator
{
	void GenerateConst(std::ofstream& file, class UConst* constant);
	void ProcessConsts(std::ofstream& file, class UObject* packageObj);
}

namespace EnumGenerator
{
	extern std::unordered_map<std::string, std::vector<class UEnum*>> mEnums;
	void GenerateEnum(std::ofstream& file, class UEnum* uEnum);
	void ProcessEnums(std::ofstream& file, class UObject* packageObj);
}

namespace StructGenerator
{
	extern std::vector<std::string> vGeneratedStructs;
	class UScriptStruct* FindLargestStruct(const std::string& structFullName);
	void GenerateStructFields(std::ofstream& structStream, EClassTypes structType);
	void GenerateStruct(std::ofstream& file, class UScriptStruct* scriptStruct);
	void GenerateStructProperties(std::ofstream& file, class UScriptStruct* scriptStruct, class UObject* packageObj);
	void ProcessStructs(std::ofstream& file, class UObject* packageObj);
}

namespace ClassGenerator
{
	extern std::unordered_map<std::string, int32_t> mGeneratedClasses;
	void GenerateClassFields(std::ostringstream& classStream, class UClass* uClass, EClassTypes classType);
	void GenerateClass(std::ofstream& file, class UClass* uClass);
	void GenerateClassProperties(std::ofstream& file, class UClass* uClass, class UObject* packageObj);
	void ProcessClasses(std::ofstream& file, class UObject* packageObj);
}

namespace ParameterGenerator
{
	void GenerateParameter(std::ofstream& file, class UClass* uClass);
	void ProcessParameters(std::ofstream& file, class UObject* packageObj);
}

namespace FunctionGenerator
{
	void GenerateVirtualFunctions(std::ofstream& file);
	void GenerateFunctionCode(std::ofstream& file, class UClass* uClass);
	void GenerateFunctionDescription(std::ofstream& file, class UClass* uClass);
	void ProcessFunctions(std::ofstream& file, class UObject* packageObj);
}

namespace Generator
{
	extern bool GlobalsInitialized;
	extern std::ofstream LogFile;
	extern std::vector<class UObject*> vPackages;
	extern std::vector<class UObject*> vIncludes;
	extern std::vector<std::pair<std::string, int32_t>> vConstants;

	std::string CreateValidName(std::string name);
	std::string CreateUniqueName(class UClass* uClass);
	std::string CreateUniqueName(class UFunction* uFunction, class UClass* uClass);
	std::string CreateIndexName(class UObject* uObject, bool pushBack);
	void MakeWindowsFunction(std::string& functionName);

	void GenerateConstants();
	void GenerateHeaders();
	void GenerateDefines();
	void ProcessPackages(std::filesystem::path directory);
	void GenerateSDK();
	void DumpInstances(bool dumpNames, bool dumpObjects);
	bool Initialize(bool createLogFile);
}