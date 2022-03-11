#include "dllmain.hpp"

namespace Utils
{
    void Messagebox(const std::string& message, const std::string& title, uint64_t flags)
    {
        MessageBoxA(NULL, message.c_str(), title.c_str(), flags);
    }

    MODULEINFO GetModuleInfo(LPCTSTR moduleName)
    {
        MODULEINFO miInfos = { NULL };
        HMODULE hmModule = GetModuleHandle(moduleName);

        if (hmModule)
        {
            GetModuleInformation(GetCurrentProcess(), hmModule, &miInfos, sizeof(MODULEINFO));
        }

        return miInfos;
    }

    uintptr_t FindPattern(HMODULE module, const uint8_t* pattern, const char* mask)
    {
        MODULEINFO miInfos = { NULL };
        GetModuleInformation(GetCurrentProcess(), module, &miInfos, sizeof(MODULEINFO));

        uintptr_t start = reinterpret_cast<uintptr_t>(module);
        size_t length = miInfos.SizeOfImage;

        size_t pos = 0;
        size_t maskLength = std::strlen(mask) - 1;

        for (uintptr_t retAddress = start; retAddress < start + length; retAddress++)
        {
            if (*reinterpret_cast<uint8_t*>(retAddress) == pattern[pos] || mask[pos] == '?')
            {
                if (pos == maskLength)
                {
                    return (retAddress - maskLength);
                }

                pos++;
            }
            else
            {
                retAddress -= pos;
                pos = 0;
            }
        }

        return NULL;
    }

    bool MapExists(std::multimap<std::string, std::string>& map, const std::string& key, const std::string& value)
    {
        std::pair<std::multimap<std::string, std::string>::iterator, std::multimap<std::string, std::string>::iterator> prRange;
        prRange = map.equal_range(key);

        for (std::multimap<std::string, std::string>::iterator it = prRange.first; it != prRange.second; it++)
        {
            if ((*it).second == value)
            {
                return true;
            }
        }

        return false;
    }

    bool SortPropertyPair(std::pair<class UProperty*, std::string> propertyA, std::pair<class UProperty*, std::string> propertyB)
    {
        if (propertyA.first->Offset == propertyB.first->Offset && propertyA.first->IsA(UBoolProperty::StaticClass()) && propertyB.first->IsA(UBoolProperty::StaticClass()))
        {
            return (static_cast<UBoolProperty*>(propertyA.first)->BitMask < static_cast<UBoolProperty*>(propertyB.first)->BitMask);
        }
       
        return (propertyA.first->Offset < propertyB.first->Offset);
    }

    bool SortProperty(class UProperty* propertyA, class UProperty* propertyB)
    {
        if (propertyA->Offset == propertyB->Offset && propertyA->IsA(UBoolProperty::StaticClass()) && propertyB->IsA(UBoolProperty::StaticClass()))
        {
            return (static_cast<UBoolProperty*>(propertyA)->BitMask < static_cast<UBoolProperty*>(propertyB)->BitMask);
        }
       
        return (propertyA->Offset < propertyB->Offset);
    }

    bool IsStructProperty(EPropertyTypes propertyType)
    {
        if (propertyType == EPropertyTypes::TYPE_TARRAY
            || propertyType == EPropertyTypes::TYPE_TMAP
            || propertyType == EPropertyTypes::TYPE_FNAMEENTRY
            || propertyType == EPropertyTypes::TYPE_FNAME
            || propertyType == EPropertyTypes::TYPE_FSTRING
            || propertyType == EPropertyTypes::TYPE_FSCRIPTDELEGATE
            || propertyType == EPropertyTypes::TYPE_FPOINTER
            || propertyType == EPropertyTypes::TYPE_FSTRUCT)
        {
            return true;
        }

        return false;
    }

    bool IsBitField(EPropertyTypes propertyType)
    {
        if (propertyType == EPropertyTypes::TYPE_INT8
            || propertyType == EPropertyTypes::TYPE_ULONG
            || propertyType == EPropertyTypes::TYPE_UINT64)
        {
            return true;
        }

        return false;
    }

    bool IsBitField(unsigned long dimension)
    {
        if (dimension == 1
            || dimension == 2
            || dimension == 3)
        {
            return true;
        }

        return false;
    }

    bool AreGObjectsValid()
    {
        if (UObject::GObjObjects()->Num() > 0 && UObject::GObjObjects()->Max() > UObject::GObjObjects()->Num())
        {
            return true;
        }

        return false;
    }

    bool AreGNamesValid()
    {
        if (FName::Names()->Num() > 0 && FName::Names()->Max() > FName::Names()->Num())
        {
            return true;
        }

        return false;
    }
}

namespace Retrievers
{
    void GetAllPropertyFlags(std::ostringstream& stream, uint64_t propertyFlags)
    {
        bool first = true;

        if (propertyFlags & EPropertyFlags::CPF_Edit) { stream << (first ? "(" : " | ") << "CPF_Edit";							                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Const) { stream << (first ? "(" : " | ") << "CPF_Const";						                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Input) { stream << (first ? "(" : " | ") << "CPF_Input";						                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ExportObject) { stream << (first ? "(" : " | ") << "CPF_ExportObject";			                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_OptionalParm) { stream << (first ? "(" : " | ") << "CPF_OptionalParm";			                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Net) { stream << (first ? "(" : " | ") << "CPF_Net";							                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditConstArray) { stream << (first ? "(" : " | ") << "CPF_EditConstArray";		                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Parm) { stream << (first ? "(" : " | ") << "CPF_Parm";							                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_OutParm) { stream << (first ? "(" : " | ") << "CPF_OutParm";					                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_SkipParm) { stream << (first ? "(" : " | ") << "CPF_SkipParm";					                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ReturnParm) { stream << (first ? "(" : " | ") << "CPF_ReturnParm";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_CoerceParm) { stream << (first ? "(" : " | ") << "CPF_CoerceParm";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Native) { stream << (first ? "(" : " | ") << "CPF_Native";						                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Transient) { stream << (first ? "(" : " | ") << "CPF_Transient";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Config) { stream << (first ? "(" : " | ") << "CPF_Config";						                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Localized) { stream << (first ? "(" : " | ") << "CPF_Localized";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Travel) { stream << (first ? "(" : " | ") << "CPF_Travel";						                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditConst) { stream << (first ? "(" : " | ") << "CPF_EditConst";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_GlobalConfig) { stream << (first ? "(" : " | ") << "CPF_GlobalConfig";			                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Component) { stream << (first ? "(" : " | ") << "CPF_Component";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NeedCtorLink) { stream << (first ? "(" : " | ") << "CPF_NeedCtorLink";			                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NoExport) { stream << (first ? "(" : " | ") << "CPF_NoExport";					                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NoClear) { stream << (first ? "(" : " | ") << "CPF_NoClear";					                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditInline) { stream << (first ? "(" : " | ") << "CPF_EditInline";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EdFindable) { stream << (first ? "(" : " | ") << "CPF_EdFindable";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditInlineUse) { stream << (first ? "(" : " | ") << "CPF_EditInlineUse";		                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Deprecated) { stream << (first ? " " : " | ") << "CPF_Deprecated";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditInlineNotify) { stream << (first ? "(" : " | ") << "CPF_EditInlineNotify";	                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_RepNotify) { stream << (first ? "(" : " | ") << "CPF_RepNotify";				                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Interp) { stream << (first ? "(" : " | ") << "CPF_Interp";						                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NonTransactional) { stream << (first ? "(" : " | ") << "CPF_NonTransactional";	                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_EditorOnly) { stream << (first ? "(" : " | ") << "CPF_EditorOnly";	                                            first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NoDestructor) { stream << (first ? "(" : " | ") << "CPF_NoDestructor";	                                        first = false; }
        if (propertyFlags & EPropertyFlags::CPF_AutoWeak) { stream << (first ? "(" : " | ") << "CPF_AutoWeak";	                                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ContainsInstancedReference) { stream << (first ? "(" : " | ") << "CPF_ContainsInstancedReference";              first = false; }
        if (propertyFlags & EPropertyFlags::CPF_SimpleDisplay) { stream << (first ? "(" : " | ") << "CPF_SimpleDisplay";	                                    first = false; }
        if (propertyFlags & EPropertyFlags::CPF_AdvancedDisplay) { stream << (first ? "(" : " | ") << "CPF_AdvancedDisplay";	                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_Protected) { stream << (first ? "(" : " | ") << "CPF_Protected";	                                            first = false; }
        if (propertyFlags & EPropertyFlags::CPF_BlueprintCallable) { stream << (first ? "(" : " | ") << "CPF_BlueprintCallable";                                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_BlueprintAuthorityOnly) { stream << (first ? "(" : " | ") << "CPF_BlueprintAuthorityOnly";                      first = false; }
        if (propertyFlags & EPropertyFlags::CPF_TextExportTransient) { stream << (first ? "(" : " | ") << "CPF_TextExportTransient";                            first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NonPIEDuplicateTransient) { stream << (first ? "(" : " | ") << "CPF_NonPIEDuplicateTransient";	                first = false; }
        if (propertyFlags & EPropertyFlags::CPF_ExposeOnSpawn) { stream << (first ? "(" : " | ") << "CPF_ExposeOnSpawn";                                        first = false; }
        if (propertyFlags & EPropertyFlags::CPF_PersistentInstance) { stream << (first ? "(" : " | ") << "CPF_PersistentInstance";                              first = false; }
        if (propertyFlags & EPropertyFlags::CPF_UObjectWrapper) { stream << (first ? "(" : " | ") << "CPF_UObjectWrapper";	                                    first = false; }
        if (propertyFlags & EPropertyFlags::CPF_HasGetValueTypeHash) { stream << (first ? "(" : " | ") << "CPF_HasGetValueTypeHash";                            first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NativeAccessSpecifierPublic) { stream << (first ? "(" : " | ") << "CPF_NativeAccessSpecifierPublic";            first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NativeAccessSpecifierProtected) { stream << (first ? "(" : " | ") << "CPF_NativeAccessSpecifierProtected";      first = false; }
        if (propertyFlags & EPropertyFlags::CPF_NativeAccessSpecifierPrivate) { stream << (first ? "(" : " | ") << "CPF_NativeAccessSpecifierPrivate";          first = false; }
        if (propertyFlags & EPropertyFlags::CPF_SkipSerialization) { stream << (first ? "(" : " | ") << "CPF_SkipSerialization";                                first = false; }

        if (!first)
        {
            stream << ")";
        }
    }

    void GetAllFunctionFlags(std::ostringstream& stream, uint64_t functionFlags)
    {
        bool first = true;

        if (functionFlags & EFunctionFlags::FUNC_None) { stream << (first ? "(" : " | ") << "FUNC_None";						                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Final) { stream << (first ? "(" : " | ") << "FUNC_Final";						                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_RequiredAPI) { stream << (first ? "(" : " | ") << "FUNC_RequiredAPI";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintAuthorityOnly) { stream << (first ? "(" : " | ") << "FUNC_BlueprintAuthorityOnly";    first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintCosmetic) { stream << (first ? "(" : " | ") << "FUNC_BlueprintCosmetic";              first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Net) { stream << (first ? "(" : " | ") << "FUNC_Net";					    	                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetReliable) { stream << (first ? "(" : " | ") << "FUNC_NetReliable";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetRequest) { stream << (first ? "(" : " | ") << "FUNC_NetRequest";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Exec) { stream << (first ? "(" : " | ") << "FUNC_Exec";						                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Native) { stream << (first ? "(" : " | ") << "FUNC_Native";					                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Event) { stream << (first ? "(" : " | ") << "FUNC_Event";						                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetResponse) { stream << (first ? "(" : " | ") << "FUNC_NetResponse";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Static) { stream << (first ? "(" : " | ") << "FUNC_Static";					                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetMulticast) { stream << (first ? "(" : " | ") << "FUNC_NetMulticast";		                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_UbergraphFunction) { stream << (first ? "(" : " | ") << "FUNC_UbergraphFunction";              first = false; }
        if (functionFlags & EFunctionFlags::FUNC_MulticastDelegate) { stream << (first ? "(" : " | ") << "FUNC_MulticastDelegate";              first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Public) { stream << (first ? "(" : " | ") << "FUNC_Public";					                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Private) { stream << (first ? "(" : " | ") << "FUNC_Private";					                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Protected) { stream << (first ? "(" : " | ") << "FUNC_Protected";				                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Delegate) { stream << (first ? "(" : " | ") << "FUNC_Delegate";				                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetServer) { stream << (first ? "(" : " | ") << "FUNC_NetServer";				                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_HasOutParms) { stream << (first ? "(" : " | ") << "FUNC_HasOutParms";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_HasDefaults) { stream << (first ? "(" : " | ") << "FUNC_HasDefaults";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetClient) { stream << (first ? "(" : " | ") << "FUNC_NetClient";				                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_DLLImport) { stream << (first ? "(" : " | ") << "FUNC_DLLImport";				                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintCallable) { stream << (first ? "(" : " | ") << "FUNC_BlueprintCallable";              first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintEvent) { stream << (first ? "(" : " | ") << "FUNC_BlueprintEvent";	                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_BlueprintPure) { stream << (first ? "(" : " | ") << "FUNC_BlueprintPure";		                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_EditorOnly) { stream << (first ? "(" : " | ") << "FUNC_EditorOnly";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_Const) { stream << (first ? "(" : " | ") << "FUNC_Const";						                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_NetValidate) { stream << (first ? "(" : " | ") << "FUNC_NetValidate";			                first = false; }
        if (functionFlags & EFunctionFlags::FUNC_AllFlags) { stream << (first ? "(" : " | ") << "FUNC_AllFlags";				                first = false; }

        if (!first)
        {
            stream << ")";
        }
    }

    EPropertyTypes GetPropertyType(class UProperty* property, std::string& propertyType, bool returnFunction)
    {
        if (property)
        {
            if (property->IsA(UStructProperty::StaticClass()))
            {
                UStructProperty* structProperty = static_cast<UStructProperty*>(property);

                if (structProperty && structProperty->Struct)
                {
                    uint32_t propertyCount = UObject::CountObject<UScriptStruct>(structProperty->Struct->GetName());

                    if (propertyCount > 1)
                    {
                        if (structProperty->Struct->Outer)
                        {
                            propertyType = "struct " + Generator::CreateValidName(structProperty->Struct->Outer->GetNameCPP()) + "_" + Generator::CreateValidName(structProperty->Struct->GetNameCPP());
                        }
                        else
                        {
                            propertyType = "struct " + Generator::CreateValidName(structProperty->Struct->GetNameCPP()) + "_Outer" + Printer::Decimal(propertyCount, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        }
                    }
                    else
                    {
                        propertyType = "struct " + Generator::CreateValidName(structProperty->Struct->GetNameCPP());
                    }

                    return EPropertyTypes::TYPE_FSTRUCT;
                }
            }
            else if (property->IsA(UStrProperty::StaticClass()))
            {
                propertyType = "class FString";
                return EPropertyTypes::TYPE_FSTRING;
            }
            else if (property->IsA(UQWordProperty::StaticClass()))
            {
                propertyType = "uint64_t";
                return EPropertyTypes::TYPE_UINT64;
            }
            else if (property->IsA(UObjectProperty::StaticClass()))
            { 
                UObjectProperty* objectProperty = static_cast<UObjectProperty*>(property);

                if (objectProperty && objectProperty->PropertyClass)
                {
                    propertyType = "class " + Generator::CreateValidName(objectProperty->PropertyClass->GetNameCPP()) + "*";
                    return EPropertyTypes::TYPE_UPOINTER;
                }
            }
            else if (property->IsA(UClassProperty::StaticClass()))
            {
                UClassProperty* classProperty = static_cast<UClassProperty*>(property);

                if (classProperty && classProperty->MetaClass)
                {
                    propertyType = "class " + Generator::CreateValidName(classProperty->MetaClass->GetNameCPP()) + "*";
                    return EPropertyTypes::TYPE_UPOINTER;
                }
            }
            else if (property->IsA(UNameProperty::StaticClass()))
            {
                propertyType = "struct FName";
                return EPropertyTypes::TYPE_FNAME;
            }
            else if (property->IsA(UMapProperty::StaticClass()))
            {
                UMapProperty* mapProperty = static_cast<UMapProperty*>(property);
                std::string mapKey, mapValue;

                if (mapProperty && mapProperty->Key && mapProperty->Value)
                {
                    if (GetPropertyType(mapProperty->Key, mapKey, returnFunction) != EPropertyTypes::TYPE_UNKNOWN && GetPropertyType(mapProperty->Value, mapValue, returnFunction) != EPropertyTypes::TYPE_UNKNOWN)
                    {
                        propertyType = "TMap<" + mapKey + ", " + mapValue + ">";
                        return EPropertyTypes::TYPE_TMAP;
                    }
                }
            }
            else if (property->IsA(UIntProperty::StaticClass()))
            {
                propertyType = "int32_t";
                return EPropertyTypes::TYPE_INT32;
            }
            else if (property->IsA(UInterfaceProperty::StaticClass()))
            {
                UInterfaceProperty* interfaceProperty = static_cast<UInterfaceProperty*>(property);

                if (interfaceProperty && interfaceProperty->InterfaceClass)
                {
                    propertyType = "class " + Generator::CreateValidName(interfaceProperty->InterfaceClass->GetNameCPP()) + "*";
                    return EPropertyTypes::TYPE_UPOINTER;
                }
            }
            else if (property->IsA(UFloatProperty::StaticClass()))
            {
                propertyType = "float";
                return EPropertyTypes::TYPE_FLOAT;   
            }
            else if (property->IsA(UDelegateProperty::StaticClass()))
            {
                propertyType = "struct FScriptDelegate";
                return EPropertyTypes::TYPE_FSCRIPTDELEGATE;
            }
            else if (property->IsA(UByteProperty::StaticClass()))
            {
                propertyType = "uint8_t";
                return EPropertyTypes::TYPE_UINT8;
            }
            else if (property->IsA(UBoolProperty::StaticClass()))
            {
                if (returnFunction)
                {
                    propertyType = "bool";
                    return EPropertyTypes::TYPE_BOOL;
                }
                else
                {
                    propertyType = "unsigned long";
                    return EPropertyTypes::TYPE_ULONG;
                }
            }
            else if (property->IsA(UArrayProperty::StaticClass()))
            {
                UArrayProperty* arrayProperty = static_cast<UArrayProperty*>(property);

                if (arrayProperty && arrayProperty->Inner)
                {
                    std::string innerProperty;

                    if (GetPropertyType(arrayProperty->Inner, innerProperty, returnFunction) != EPropertyTypes::TYPE_UNKNOWN)
                    {
                        propertyType = "TArray<" + innerProperty + ">";
                        return EPropertyTypes::TYPE_TARRAY;
                    }
                }
            }
        }
        
        return EPropertyTypes::TYPE_UNKNOWN;
    }

    size_t GetPropertySize(UProperty* property)
    {
        if (property)
        {
            if (property->IsA(UStructProperty::StaticClass()))
            {
                return property->ElementSize;
            }
            else if (property->IsA(UStrProperty::StaticClass()))
            {
                return sizeof(FString);
            }
            else if (property->IsA(UQWordProperty::StaticClass()))
            {
                return sizeof(uint64_t);
            }
            else if (property->IsA(UObjectProperty::StaticClass()))
            {
                return sizeof(uintptr_t);
            }
            else if (property->IsA(UClassProperty::StaticClass()))
            {
                return sizeof(uintptr_t);
            }
            else if (property->IsA(UNameProperty::StaticClass()))
            {
                return sizeof(FName);
            }
            else if (property->IsA(UMapProperty::StaticClass()))
            {
                return property->ElementSize;
            }
            else if (property->IsA(UIntProperty::StaticClass()))
            {
                return sizeof(int32_t);
            }
            else if (property->IsA(UInterfaceProperty::StaticClass()))
            {
                return sizeof(uintptr_t);
            }
            else if (property->IsA(UFloatProperty::StaticClass()))
            {
                return sizeof(float);
            }
            else if (property->IsA(UDelegateProperty::StaticClass()))
            {
                return property->ElementSize;
            }
            else if (property->IsA(UByteProperty::StaticClass()))
            {
                return sizeof(uint8_t);
            }
            else if (property->IsA(UBoolProperty::StaticClass()))
            {
                return sizeof(unsigned long);
            }
            else if (property->IsA(UArrayProperty::StaticClass()))
            {
                return sizeof(TArray<uintptr_t>);
            }
        }
        
        return 0;
    }

    uintptr_t GetEntryPoint()
    {
        return reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
    }

    uintptr_t GetOffset(uintptr_t address)
    {
        uintptr_t entryPoint = GetEntryPoint();

        if (address > entryPoint)
        {
            return (address - entryPoint);
        }
        
        return 0x0;
    }
}

namespace ConstGenerator
{
    void GenerateConst(std::ofstream& file, class UConst* constant)
    {
        std::ostringstream constStream;
        std::ostringstream valueStream;

        static std::multimap<std::string, std::string> nameValueMap;
        std::string constName = Generator::CreateValidName(constant->GetName());

        if (constName.find("Default__") == std::string::npos)
        {
            if (Generator::LogFile.is_open())
            {
                std::string propertyTitle = "Const: ";

                Generator::LogFile << propertyTitle;
                Printer::FillRight(Generator::LogFile, ' ', constName.size());
                Generator::LogFile << constName;
                Printer::FillRight(Generator::LogFile, ' ', 75 - (constName.size() + propertyTitle.size()));
                Generator::LogFile << " - Instance: " << Printer::Hex(reinterpret_cast<uintptr_t>(constant), sizeof(constant)) << std::endl;
                Generator::LogFile.flush();
            }

            std::string constValue = constant->Value.ToString();
            size_t mapSize = nameValueMap.count(constName);

            if (mapSize == 0)
            {
                nameValueMap.insert(std::make_pair(constName, constValue));

                file << "#define CONST_" << constName;
                Printer::FillLeft(file, ' ', Configuration::ConstSpacer - constName.length());
                file << " " << constValue << "\n";
            }
            else if (!Utils::MapExists(nameValueMap, constName, constValue))
            {
                nameValueMap.insert(std::make_pair(constName, constValue));

                file << "#define CONST_" << constName << Printer::Decimal(mapSize, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                Printer::FillLeft(file, ' ', Configuration::ConstSpacer - constName.length());
                file << " " << constValue << "\n";
            }   
        }
    }

    void ProcessConsts(std::ofstream& file, class UObject* packageObj)
    {
        for (UObject* object : *UObject::GObjObjects())
        {
            if (object)
            {
                UObject* packageObject = object->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == packageObj && object->IsA(UConst::StaticClass()))
                    {
                        GenerateConst(file, static_cast<UConst*>(object));
                    }
                }
            }
        }
    }
}

namespace EnumGenerator
{
    void GenerateEnum(std::ofstream& file, class UEnum* uEnum)
    {
        std::ostringstream enumStream;
        std::ostringstream propertyStream;

        std::string enumFullName = uEnum->GetFullName();
        std::string enumName = Generator::CreateValidName(uEnum->GetName());

        if (enumName.find("Default__") == std::string::npos)
        {
            if (Generator::LogFile.is_open())
            {
                std::string propertyTitle = "Enum: ";

                Generator::LogFile << propertyTitle;
                Printer::FillRight(Generator::LogFile, ' ', enumName.size());
                Generator::LogFile << enumName;
                Printer::FillRight(Generator::LogFile, ' ', 75 - (enumName.size() + propertyTitle.size()));
                Generator::LogFile << " - Instance: " << Printer::Hex(reinterpret_cast<uintptr_t>(uEnum), sizeof(uEnum)) << std::endl;
                Generator::LogFile.flush();
            }

            uint32_t enumCount = UObject::CountObject<UEnum>(uEnum->GetName());

            if (enumCount > 1)
            {
                enumName = Generator::CreateValidName(uEnum->Outer->GetName()) + "_" + enumName;
            }

            if (enumName.find("E") != 0)
            {
                enumName = "E" + enumName;
            }

            if (Configuration::UsingEnumClasses)
            {
                enumStream << "// " << enumFullName << "\n";
                enumStream << "enum class " << enumName << " : " << Configuration::EnumClassType << "\n";
                enumStream << "{" << "\n";
            }
            else
            {
                enumStream << "// " << enumFullName << "\n";
                enumStream << "enum " << enumName << "\n";
                enumStream << "{" << "\n";
            }

            std::map<std::string, uint32_t> propertiesMap;

            for (int32_t i = 0; i < uEnum->Names.Num(); i++)
            {
                std::string propertyName = Generator::CreateValidName(uEnum->Names[i].ToString());

                size_t maxPos = propertyName.find("_MAX");

                if (maxPos != std::string::npos)
                {
                    propertyName.replace(maxPos, 4, "_END");
                }

                if (propertiesMap.count(propertyName) == 0)
                {
                    propertiesMap[propertyName] = 1;
                    propertyStream << propertyName;
                }
                else
                {
                    propertyStream << propertyName << Printer::Decimal(propertiesMap[propertyName], static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                    propertiesMap[propertyName]++;
                }

                enumStream << "\t";
                Printer::FillLeft(enumStream, ' ', Configuration::EnumSpacer);
                enumStream << propertyStream.str() << " = " << std::to_string(i);

                if (i != uEnum->Names.Num() - 1)
                {
                    enumStream << "," << "\n";
                }
                else
                {
                    enumStream << "\n";
                }

                Printer::Empty(propertyStream);
            }

            enumStream << "};\n\n";

            file << enumStream.str();
        }
    }

    void ProcessEnums(std::ofstream& file, class UObject* packageObj)
    {
        for (UObject* object : *UObject::GObjObjects())
        {
            if (object)
            {
                UObject* packageObject = object->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == packageObj && object->IsA(UEnum::StaticClass()))
                    {
                        GenerateEnum(file, static_cast<UEnum*>(object));
                    }
                }
            }
        }
    }
}

namespace StructGenerator
{
    class UScriptStruct* FindLargestStruct(const std::string& structFullName)
    {
        unsigned long propertySize = 0;
        UScriptStruct* largestStruct = nullptr;

        for (UObject* object : *UObject::GObjObjects())
        {
            if (object && object->IsA(UScriptStruct::StaticClass()))
            {
                if (object->GetFullName() == structFullName)
                {
                    UScriptStruct* scriptStruct = static_cast<UScriptStruct*>(object);

                    if (scriptStruct->PropertySize >= propertySize)
                    {
                        largestStruct = scriptStruct;
                        propertySize = scriptStruct->PropertySize;
                    }
                }
            }
        }

        return largestStruct;
    }

    void GenerateStructFields(std::ofstream& structStream, EClassTypes structType)
    {
        std::ostringstream propertyStream;

        size_t localSize = 0;
        size_t startOffset = 0;
        size_t missedOffset = 0;
        size_t lastOffset = 0;

        std::map<uintptr_t, ClassField> fields = Fields::GetOrderedFields(structType, localSize, startOffset);

        if (fields.size() > 0)
        {
            lastOffset = startOffset;
            int32_t unknownDataIndex = 0;

            for (const std::pair<uintptr_t, ClassField>& field : fields)
            {
                if ((lastOffset + field.second.Size) < field.second.Offset)
                {
                    missedOffset = field.second.Offset - lastOffset;

                    if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                    {
                        propertyStream << "\tuint8_t UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                        structStream << propertyStream.str() << " // " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        structStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") DYNAMIC FIELD PADDING\n";
                        Printer::Empty(propertyStream);

                        unknownDataIndex++;
                    }
                }

                structStream << "\t" << field.second.Type << " // " << Printer::Hex(field.second.Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                structStream << " (" << Printer::Hex(field.second.Size, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")\n";

                lastOffset = field.second.Offset + field.second.Size;
            }

            if (structType != EClassTypes::CLASS_FNAMEENTRY && lastOffset < localSize)
            {
                missedOffset = localSize - lastOffset;

                if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                {
                    propertyStream << "\tuint8_t UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                    structStream << propertyStream.str() << " // " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") DYNAMIC FIELD PADDING\n";
                }
            }
        }
        else
        {
            Generator::LogFile << "Error: No registered fields found for struct \"" << std::to_string(static_cast<int32_t>(structType)) << "\"\n";
            Utils::Messagebox("Error: No registered fields found for struct \"" + std::to_string(static_cast<int32_t>(structType)) + "\"!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);
        }
    }

    void GenerateStruct(std::ofstream& file, class UScriptStruct* scriptStruct)
    {
        std::ostringstream structStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        std::string structFullName = scriptStruct->GetFullName();
        std::string structName = Generator::CreateValidName(scriptStruct->GetName());
        std::string structNameCPP = Generator::CreateValidName(scriptStruct->GetNameCPP());
        std::string structOuterNameCPP = Generator::CreateValidName(scriptStruct->Outer->GetNameCPP());

        if (structNameCPP == "FPointer" || structNameCPP == "FQWord")
        {
            return;
        }

        if (Generator::LogFile.is_open())
        {
            std::string propertyTitle = "ScriptStruct: ";

            Generator::LogFile << propertyTitle;
            Printer::FillRight(Generator::LogFile, ' ', structName.size());
            Generator::LogFile << structName;
            Printer::FillRight(Generator::LogFile, ' ', 75 - (structName.size() + propertyTitle.size()));
            Generator::LogFile << " - Instance: " << Printer::Hex(reinterpret_cast<uintptr_t>(scriptStruct), sizeof(scriptStruct)) << std::endl;
            Generator::LogFile.flush();
        }

        structStream << "// " << structFullName << "\n";

        size_t size = 0;
        size_t lastOffset = 0;
        size_t missedOffset = 0;

        UScriptStruct* superField = static_cast<UScriptStruct*>(scriptStruct->SuperField);
        uint32_t structCount = UObject::CountObject<UScriptStruct>(structName);

        if (superField && superField != scriptStruct)
        {
            size = scriptStruct->PropertySize - superField->PropertySize;
            lastOffset = superField->PropertySize;

            std::string fieldName = Generator::CreateValidName(superField->GetName());
            std::string fieldNameCPP = Generator::CreateValidName(superField->GetNameCPP());
            std::string fieldOuterNameCPP = Generator::CreateValidName(superField->Outer->GetNameCPP());

            uint32_t fieldStructCount = UObject::CountObject<UScriptStruct>(fieldName);

            structStream << "// " << Printer::Hex(size, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
            structStream << " (" << Printer::Hex(superField->PropertySize, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
            structStream << " - " << Printer::Hex(scriptStruct->PropertySize, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
            structStream << ")\n";

            if (structCount > 1)
            {
                structStream << "struct " << structOuterNameCPP << "_" << structNameCPP << " : ";
            }
            else
            {
                structStream << "struct " << structNameCPP << " : ";
            }

            if (fieldStructCount > 1)
            {
                structStream << fieldOuterNameCPP << "_" << fieldNameCPP << "\n";
            }
            else
            {
                structStream << fieldNameCPP << "\n";
            }
        }
        else
        {
            size = scriptStruct->PropertySize;

            structStream << "// " << Printer::Hex(size, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << "\n";

            if (structCount > 1)
            {
                structStream << "struct " << structOuterNameCPP << "_" << structNameCPP << "\n";
            }
            else
            {
                structStream << "struct " << structNameCPP << "\n";
            }
        }

        structStream << "{\n";

        std::vector<UProperty*> vProperty;

        for (UProperty* property = static_cast<UProperty*>(scriptStruct->Children); property; property = static_cast<UProperty*>(property->Next))
        {
            if (property->ElementSize > 0 && !property->IsA(UScriptStruct::StaticClass()))
            {
                vProperty.push_back(property);
            }
        }

        std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

        std::map<std::string, uint32_t> propertyNameMap;
        uint32_t unknownDataIndex = 0;

        for (size_t i = 0; i < vProperty.size(); i++)
        {
            UProperty* property = vProperty[i];

            if (lastOffset < property->Offset)
            {
                missedOffset = property->Offset - lastOffset;

                if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                {
                    propertyStream << "UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                    structStream << "\t";
                    Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                    structStream << "uint8_t ";
                    Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                    structStream << propertyStream.str() << "\t\t// " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") MISSED OFFSET\n";
                    Printer::Empty(propertyStream);

                    unknownDataIndex++;
                }
            }

            std::string propertyType;

            if (Retrievers::GetPropertyType(property, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
            {
                size_t correctElementSize = Retrievers::GetPropertySize(property);
                std::string propertyName = Generator::CreateValidName(property->GetName());

                if (propertyNameMap.count(propertyName) == 0)
                {
                    propertyNameMap[propertyName] = 1;
                    propertyStream << propertyName;
                }
                else
                {
                    propertyStream << propertyName << Printer::Decimal(propertyNameMap[propertyName], static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                    propertyNameMap[propertyName]++;
                }

                if (property->ArrayDim > 1)
                {
                    if (!property->IsA(UInterfaceProperty::StaticClass()))
                    {
                        propertyStream << "[" << Printer::Hex(property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "]";
                    }

                    correctElementSize *= property->ArrayDim;
                }

                if (property->IsA(UBoolProperty::StaticClass()))
                {
                    propertyStream << " : 1";
                }

                Retrievers::GetAllPropertyFlags(flagStream, property->PropertyFlags);

                int32_t offsetError = (property->ElementSize * property->ArrayDim) - (correctElementSize * property->ArrayDim);

                if (property->IsA(UInterfaceProperty::StaticClass()))
                {
                    size_t interfaceSize = Retrievers::GetPropertySize(property);

                    if (offsetError == interfaceSize)
                    {
                        offsetError -= interfaceSize;
                    }

                    structStream << "\t";
                    Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                    structStream << propertyType << " " << propertyStream.str();

                    if (property->ArrayDim > 1)
                    {
                        structStream << "_Object[" << Printer::Hex(property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";
                    }
                    else
                    {
                        structStream << "_Object;";
                    }

                    Printer::FillRight(structStream, ' ', Configuration::StructSpacer - (propertyStream.str().size() + 8));
                    structStream << "// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << " (" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                    structStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";
                    structStream << flagStream.str() << "\n";
                    
                    structStream << "\t";
                    Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                    structStream << propertyType << " " << propertyStream.str();

                    if (property->ArrayDim > 1)
                    {
                        structStream << "_Interface[" << Printer::Hex(property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";
                    }
                    else
                    {
                        structStream << "_Interface;";
                    }

                    Printer::FillRight(structStream, ' ', Configuration::StructSpacer - (propertyStream.str().size() + 11));
                    structStream << "// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << " (" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                    structStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";

                    Printer::FillLeft(structStream, ' ', static_cast<uint64_t>(EWidthTypes::WIDTH_FIELD));
                }
                else
                {
                    structStream << "\t";
                    Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                    structStream << propertyType << " " << propertyStream.str() << ";";

                    Printer::FillRight(structStream, ' ', Configuration::StructSpacer - (propertyStream.str().size() + 1));
                    structStream << "// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << " (" << Printer::Hex((property->ElementSize * property->ArrayDim), static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                    structStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";

                    if (property->IsA(UBoolProperty::StaticClass()))
                    {
                        structStream << "[" << Printer::Hex(static_cast<UBoolProperty*>(property)->BitMask, static_cast<uint64_t>(EWidthTypes::WIDTH_BITMASK)) << "] ";
                    }
                    else
                    {
                        Printer::FillLeft(structStream, ' ', static_cast<uint64_t>(EWidthTypes::WIDTH_FIELD));
                    }
                }

                structStream << flagStream.str() << "\n";

                if (offsetError > 0)
                {
                    propertyStream << "UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[" << Printer::Hex(offsetError, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                    structStream << "\t";
                    Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                    structStream << "uint8_t ";
                    Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                    structStream << propertyStream.str() << "// " << Printer::Hex(property->Offset + offsetError, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    Printer::Empty(propertyStream);

                    structStream << " (" << Printer::Hex(offsetError, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") FIX WRONG SIZE OF PREVIOUS PROPERTY";
                    structStream << " [Original: " << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << ", Missing: " << Printer::Hex(offsetError, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << "]\n";

                    unknownDataIndex++;
                }

                Printer::Empty(propertyStream);
                Printer::Empty(flagStream);
            }
            else
            {
                propertyStream << "UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                propertyStream << "[" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                structStream << "\t";
                Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                structStream << "uint8_t ";
                Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                structStream << propertyStream.str() << "// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                structStream << " (" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                structStream << ") UNKNOWN PROPERTY: " << property->GetFullName() << "\n";
                Printer::Empty(propertyStream);

                unknownDataIndex++;
            }

            lastOffset = property->Offset + (property->ElementSize * property->ArrayDim);
        }

        if (lastOffset < scriptStruct->PropertySize)
        {
            missedOffset = scriptStruct->PropertySize - lastOffset;

            if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
            {
                propertyStream << "UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                structStream << "\t";
                Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                structStream << "uint8_t ";
                Printer::FillLeft(structStream, ' ', Configuration::StructSpacer);
                structStream << propertyStream.str() << "\t\t// " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                structStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") MISSED OFFSET\n";
                Printer::Empty(propertyStream);
            }
        }

        structStream << "};\n\n";

        file << structStream.str();
    }

    void GenerateStructProperties(std::ofstream& file, class UScriptStruct* scriptStruct, class UObject* packageObj)
    {
        UObject* packageObject = scriptStruct->GetPackageObj();

        if (packageObject && packageObject == packageObj)
        {
            static std::vector<std::string> generatedStructs;

            std::string structFullName = scriptStruct->GetFullName();
            std::string structName = scriptStruct->GetName();

            if (structName.find("Default__") != std::string::npos && structName.find("<uninitialized>") != std::string::npos)
            {
                return;
            }

            if (find(generatedStructs.begin(), generatedStructs.end(), structFullName) == generatedStructs.end())
            {
                scriptStruct = FindLargestStruct(structFullName);

                if (scriptStruct->SuperField && scriptStruct->SuperField != scriptStruct && std::find(generatedStructs.begin(), generatedStructs.end(), (static_cast<UScriptStruct*>(scriptStruct->SuperField))->GetFullName()) == generatedStructs.end())
                {
                    GenerateStructProperties(file, static_cast<UScriptStruct*>(scriptStruct->SuperField), packageObj);
                }

                for (UProperty* structChild = static_cast<UProperty*>(scriptStruct->Children); structChild; structChild = static_cast<UProperty*>(structChild->Next))
                {
                    std::string propertyType;
                    EPropertyTypes childType = Retrievers::GetPropertyType(structChild, propertyType, false);

                    if (childType == EPropertyTypes::TYPE_FSTRUCT)
                    {
                        UScriptStruct* propertyStruct = static_cast<UScriptStruct*>(static_cast<UStructProperty*>(structChild)->Struct);

                        if (propertyStruct && propertyStruct != scriptStruct && std::find(generatedStructs.begin(), generatedStructs.end(), propertyStruct->GetFullName()) == generatedStructs.end())
                        {
                            GenerateStructProperties(file, propertyStruct, packageObj);
                        }
                    }
                    else if (childType == EPropertyTypes::TYPE_TARRAY)
                    {
                        UScriptStruct* propertyStruct = static_cast<UScriptStruct*>(static_cast<UStructProperty*>(static_cast<UArrayProperty*>(structChild)->Inner)->Struct);

                        if (propertyStruct != scriptStruct
                            && Retrievers::GetPropertyType(static_cast<UArrayProperty*>(structChild)->Inner, propertyType, false) == EPropertyTypes::TYPE_FSTRUCT
                            && find(generatedStructs.begin(), generatedStructs.end(), propertyStruct->GetFullName()) == generatedStructs.end())
                        {
                            GenerateStructProperties(file, propertyStruct, packageObj);
                        }
                    }
                }

                GenerateStruct(file, scriptStruct);
                generatedStructs.push_back(structFullName);
            }   
        }
    }

    void ProcessStructs(std::ofstream& file, class UObject* packageObj)
    {
        for (UObject* object : *UObject::GObjObjects())
        {
            if (object)
            {
                UObject* packageObject = object->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == packageObj && object->IsA(UScriptStruct::StaticClass()))
                    {
                        GenerateStructProperties(file, static_cast<UScriptStruct*>(object), packageObject);
                    }
                }
            }
        }
    }
}

namespace ClassGenerator
{
    void GenerateClassFields(std::ostringstream& classStream, class UClass* uClass, EClassTypes classType)
    {
        std::ostringstream propertyStream;

        size_t localSize = 0;
        size_t startOffset = 0;
        size_t missedOffset = 0;
        size_t lastOffset = 0;

        std::map<uintptr_t, ClassField> fields = Fields::GetOrderedFields(classType, localSize, startOffset);

        if (fields.size() > 0)
        {
            if (uClass->PropertySize == localSize)
            {
                lastOffset = startOffset;
                int32_t unknownDataIndex = 0;

                for (const std::pair<uintptr_t, ClassField>& field : fields)
                {
                    if ((lastOffset + field.second.Size) < field.second.Offset)
                    {
                        missedOffset = field.second.Offset - lastOffset;

                        if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                        {
                            propertyStream << "\tuint8_t UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                            propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                            classStream << propertyStream.str() << " // " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                            classStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") DYNAMIC FIELD PADDING\n";
                            Printer::Empty(propertyStream);

                            unknownDataIndex++;
                        }
                    }

                    classStream << "\t" << field.second.Type << " // " << Printer::Hex(field.second.Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << " (" << Printer::Hex(field.second.Size, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")\n";

                    lastOffset = field.second.Offset + field.second.Size;
                }

                if (lastOffset < uClass->PropertySize)
                {
                    missedOffset = uClass->PropertySize - lastOffset;

                    if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                    {
                        propertyStream << "\tuint8_t UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                        classStream << propertyStream.str() << " // " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") DYNAMIC FIELD PADDING\n";
                    }
                }
            }
            else
            {
                if (Generator::LogFile.is_open())
                {
                    Generator::LogFile << "Error: INCORRECT CLASS SIZE DETECTED FOR CLASS " << uClass->GetName() << "\n";
                    Generator::LogFile << "Error: REGISTERED CLASS SIZE: " << std::to_string(localSize) << "\n";
                    Generator::LogFile << "Error: ACTUAL CLASS SIZE: " << std::to_string(uClass->PropertySize) << "\n";
                }

                Utils::Messagebox("Error: Incorrect class size detected, check the log for more details!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);
            }
        }
        else
        {
            Generator::LogFile << "Error: No registered fields found for class \"" << uClass->GetName() << "\"\n";
            Utils::Messagebox("Error: No registered fields found for class \"" + uClass->GetName() + "\"!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);
        }
    }

    void GenerateClass(std::ofstream& file, class UClass* uClass)
    {
        std::ostringstream classStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        size_t size = 0;
        size_t lastOffset = 0;
        size_t missedOffset = 0;

        UClass* uSuperClass = static_cast<UClass*>(uClass->SuperField);

        std::string className = Generator::CreateValidName(uClass->GetName());
        std::string classNameCPP = Generator::CreateValidName(uClass->GetNameCPP());
        std::string classFullName = uClass->GetFullName();

        if (Generator::LogFile.is_open())
        {
            std::string propertyTitle = "Class: ";

            Generator::LogFile << propertyTitle;
            Printer::FillRight(Generator::LogFile, ' ', className.size());
            Generator::LogFile << className;
            Printer::FillRight(Generator::LogFile, ' ', 75 - (className.size() + propertyTitle.size()));
            Generator::LogFile << " - Instance: " << Printer::Hex(reinterpret_cast<uintptr_t>(uClass), sizeof(uClass)) << std::endl;
            Generator::LogFile.flush();
        }

        std::vector<UProperty*> vProperty;

        for (UProperty* property = static_cast<UProperty*>(uClass->Children); property; property = static_cast<UProperty*>(property->Next))
        {
            if (!property->IsA(UFunction::StaticClass())
                && !property->IsA(UConst::StaticClass())
                && !property->IsA(UEnum::StaticClass())
                && !property->IsA(UScriptStruct::StaticClass())
                && property->ElementSize > 0
                && (!uSuperClass || (uSuperClass && uSuperClass != uClass && property->Offset >= uSuperClass->PropertySize)))
            {
                vProperty.push_back(property);
            }
        }

        std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

        classStream << "// " << classFullName << "\n";

        if (uSuperClass && uSuperClass != uClass)
        {
            size = uClass->PropertySize - uSuperClass->PropertySize;
            lastOffset = uSuperClass->PropertySize;

            classStream << "// " << Printer::Hex(size, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
            classStream << " (" << Printer::Hex(uSuperClass->PropertySize, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
            classStream << " - " << Printer::Hex(uClass->PropertySize, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
            classStream << ")\n" << "class " << classNameCPP << " : public " << Generator::CreateValidName(uSuperClass->GetNameCPP());
        }
        else
        {
            size = uClass->PropertySize;

            classStream << "// " << Printer::Hex(size, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << "\n";
            classStream << "class " << classNameCPP;
        }

        classStream << "\n{\npublic:\n";

        if (uClass == UField::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UFIELD); }
        else if (uClass == UEnum::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UENUM); }
        else if (uClass == UConst::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UCONST); }
        else if (uClass == UProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UPROPERTY); }
        else if (uClass == UStruct::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_USTRUCT); }
        else if (uClass == UFunction::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UFUNCTION); }
        else if (uClass == UStructProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_USTRUCTPROPERTY); }
        else if (uClass == UObjectProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UOBJECTPROPERTY); }
        else if (uClass == UMapProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UMAPPROPERTY); }
        else if (uClass == UInterfaceProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UINTERFACEPROPERTY); }
        else if (uClass == UDelegateProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UDELEGATEPROPERTY); }
        else if (uClass == UByteProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UBYTEPROPERTY); }
        else if (uClass == UBoolProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UBOOLPROPERTY); }
        else if (uClass == UArrayProperty::StaticClass()) { GenerateClassFields(classStream, uClass, EClassTypes::CLASS_UARRAYPROPERTY); }
        else
        {
            std::map<std::string, uint32_t> propertyNameMap;
            uint32_t unknownDataIndex = 0;

            for (size_t i = 0; i < vProperty.size(); i++)
            {
                UProperty* property = vProperty[i];

                std::string propertyName = Generator::CreateValidName(property->GetName());

                if (uClass == UObject::StaticClass())
                {
                    if (!Configuration::UsingDetours && propertyName.find("VfTable") != std::string::npos)
                    {
                        lastOffset = property->Offset + (property->ElementSize * property->ArrayDim);
                        continue;
                    }
                }

                if (lastOffset < property->Offset)
                {
                    missedOffset = property->Offset - lastOffset;

                    if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                    {
                        propertyStream << "UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                        classStream << "\t";
                        Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer);
                        classStream << "uint8_t ";
                        Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer - 3);
                        classStream << propertyStream.str() << "// " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") MISSED OFFSET\n";
                        Printer::Empty(propertyStream);

                        unknownDataIndex++;
                    }
                }

                std::string propertyType;

                if (Retrievers::GetPropertyType(property, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    size_t correctElementSize = Retrievers::GetPropertySize(property);

                    if (propertyNameMap.count(propertyName) == 0)
                    {
                        propertyNameMap[propertyName] = 1;
                        propertyStream << propertyName;
                    }
                    else
                    {
                        propertyStream << propertyName << Printer::Decimal(propertyNameMap[propertyName], static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyNameMap[propertyName]++;
                    }

                    if (property->ArrayDim > 1)
                    {
                        if (!property->IsA(UInterfaceProperty::StaticClass()))
                        {
                            propertyStream << "[" << Printer::Hex(property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "]";
                        }

                        correctElementSize *= property->ArrayDim;
                    }

                    if (property->IsA(UBoolProperty::StaticClass()))
                    {
                        propertyStream << " : 1";
                    }

                    Retrievers::GetAllPropertyFlags(flagStream, property->PropertyFlags);

                    int32_t offsetError = (property->ElementSize * property->ArrayDim) - (correctElementSize * property->ArrayDim);

                    if (property->IsA(UInterfaceProperty::StaticClass()))
                    {
                        size_t interfaceSize = Retrievers::GetPropertySize(property);

                        if (offsetError == interfaceSize)
                        {
                            offsetError -= interfaceSize;
                        }

                        classStream << "\t";
                        Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer);
                        classStream << propertyType << " " << propertyStream.str();

                        if (property->ArrayDim > 1)
                        {
                            classStream << "_Object[" << Printer::Hex(property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";
                        }
                        else
                        {
                            classStream << "_Object;";
                        }

                        Printer::FillRight(classStream, ' ', Configuration::ClassSpacer - (propertyStream.str().size() + 8));
                        classStream << "// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (" << Printer::Hex((property->ElementSize * property->ArrayDim) - interfaceSize, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                        classStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";
                        classStream << flagStream.str() << "\n";

                        classStream << "\t";
                        Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer);
                        classStream << propertyType << " " << propertyStream.str();

                        if (property->ArrayDim > 1)
                        {
                            classStream << "_Interface[" << Printer::Hex(property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";
                        }
                        else
                        {
                            classStream << "_Interface;";
                        }

                        Printer::FillRight(classStream, ' ', Configuration::ClassSpacer - (propertyStream.str().size() + 11));
                        classStream << "// " << Printer::Hex(property->Offset + interfaceSize, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (" << Printer::Hex((property->ElementSize * property->ArrayDim) - interfaceSize, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                        classStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";

                        Printer::FillLeft(classStream, ' ', static_cast<uint64_t>(EWidthTypes::WIDTH_FIELD));
                    }
                    else
                    {
                        classStream << "\t";
                        Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer);
                        classStream << propertyType << " " << propertyStream.str() << ";";

                        Printer::FillRight(classStream, ' ', Configuration::ClassSpacer - (propertyStream.str().size() + 1));
                        classStream << "// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (" << Printer::Hex((property->ElementSize * property->ArrayDim), static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                        classStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";

                        if (property->IsA(UBoolProperty::StaticClass()))
                        {
                            classStream << "[" << Printer::Hex(static_cast<UBoolProperty*>(property)->BitMask, static_cast<uint64_t>(EWidthTypes::WIDTH_BITMASK)) << "] ";
                        }
                        else
                        {
                            Printer::FillLeft(classStream, ' ', static_cast<uint64_t>(EWidthTypes::WIDTH_FIELD));
                        }
                    }

                    classStream << flagStream.str() << "\n";

                    if (offsetError > 0)
                    {
                        propertyStream << "_UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyStream << "[" << Printer::Hex(offsetError, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                        classStream << "\t";
                        Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer);
                        classStream << "uint8_t ";
                        Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer - 3);
                        classStream << propertyStream.str() << "// " << Printer::Hex((property->Offset + offsetError), static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (" << Printer::Hex(offsetError, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") FIX WRONG SIZE OF PREVIOUS PROPERTY ";
                        classStream << " [Original:" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << ", Missing: " << Printer::Hex(offsetError, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << "]\n";
                        Printer::Empty(propertyStream);

                        unknownDataIndex++;
                    }

                    Printer::Empty(propertyStream);
                    Printer::Empty(flagStream);
                }
                else
                {
                    propertyStream << "UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                    classStream << "\t";
                    Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer);
                    classStream << "uint8_t ";
                    Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer - 3);
                    classStream << propertyStream.str() << "// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << " (" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") UNKNOWN PROPERTY: " << property->GetFullName() << "\n";
                    Printer::Empty(propertyStream);

                    unknownDataIndex++;
                }

                lastOffset = property->Offset + (property->ElementSize * property->ArrayDim);
            }

            if (lastOffset < uClass->PropertySize)
            {
                missedOffset = uClass->PropertySize - lastOffset;

                if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                {
                    propertyStream << "UnknownData" << Printer::Decimal(unknownDataIndex, static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];";

                    classStream << "\t";
                    Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer);
                    classStream << "uint8_t ";
                    Printer::FillLeft(classStream, ' ', Configuration::ClassSpacer - 3);
                    classStream << propertyStream.str() << "// " << Printer::Hex(lastOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << " (" << Printer::Hex(missedOffset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ") MISSED OFFSET\n";
                    Printer::Empty(propertyStream);
                }
            }
        }

        classStream << "\npublic:\n";

        if (Configuration::UsingConstants)
        {
            classStream << "\tstatic UClass* StaticClass()\n";
            classStream << "\t{\n";
            classStream << "\t\tstatic UClass* uClassPointer = nullptr;\n\n";
            classStream << "\t\tif (!uClassPointer)\n";
            classStream << "\t\t{\n";
            classStream << "\t\t\tuClassPointer = reinterpret_cast<UClass*>(UObject::GObjObjects()->At(" << Generator::CreateValidName(Generator::CreateIndexName(uClass, true)) << "));\n";
            classStream << "\t\t}\n\n";
            classStream << "\t\treturn uClassPointer;\n";
            classStream << "\t};\n\n";;
        }
        else
        {
            classStream << "\tstatic UClass* StaticClass()\n";
            classStream << "\t{\n";
            classStream << "\t\tstatic UClass* uClassPointer = nullptr;\n\n";
            classStream << "\t\tif (!uClassPointer)\n";
            classStream << "\t\t{\n";
            classStream << "\t\t\tuClassPointer = UObject::FindClass(\"" << classFullName << "\");\n";
            classStream << "\t\t}\n\n";
            classStream << "\t\treturn uClassPointer;\n";
            classStream << "\t};\n\n";
        }

        if (uClass == UObject::StaticClass())
        {
            classStream << PiecesOfCode::UObject_FunctionDescriptions;
        }

        file << classStream.str();
        Printer::Empty(classStream);

        FunctionGenerator::GenerateFunctionDescription(file, uClass);

        if (uClass == UObject::StaticClass())
        {
            if (Configuration::UsingDetours)
            {
                classStream << "\tvoid ProcessEvent(class UFunction* uFunction, void* uParams, void* uResult);\n";
            }
            else
            {
                FunctionGenerator::GenerateVirtualFunctions(file);
            }
        }
        else if (uClass == UFunction::StaticClass())
        {
            classStream << "\tstatic UFunction* FindFunction(const std::string& functionFullName);\n";
        }

        classStream << "};\n\n";

        file << classStream.str();
    }

    void GenerateClassProperties(std::ofstream& file, class UClass* uClass, class UObject* uPackageObj)
    {
        UObject* packageObject = uClass->GetPackageObj();

        if (packageObject)
        {
            if (std::find(Generator::vIncludes.begin(), Generator::vIncludes.end(), uPackageObj) == Generator::vIncludes.end())
            {
                Generator::vIncludes.push_back(uPackageObj);
            }

            if (packageObject != uPackageObj)
            {
                std::vector<UObject*>::iterator itPO = std::find(Generator::vIncludes.begin(), Generator::vIncludes.end(), packageObject);
                std::vector<UObject*>::iterator itPTP = std::find(Generator::vIncludes.begin(), Generator::vIncludes.end(), uPackageObj);

                if (itPO == Generator::vIncludes.end())
                {
                    Generator::vIncludes.insert(itPTP, packageObject);
                }
                else if (itPO >= itPTP)
                {
                    Generator::vIncludes.insert(itPTP, packageObject);
                    Generator::vIncludes.erase(itPO);
                }

                return;
            }

            static std::map<std::string, int32_t> classNames;

            std::string className = Generator::CreateValidName(uClass->GetName());
            std::string classFullName = uClass->GetFullName();

            if (className.find("Default__") == std::string::npos)
            {
                if (classNames.find(classFullName) == classNames.end())
                {
                    if (uClass->SuperField && uClass->SuperField != uClass)
                    {
                        if (classNames.find(uClass->SuperField->GetFullName()) == classNames.end())
                        {
                            GenerateClassProperties(file, static_cast<UClass*>(uClass->SuperField), uPackageObj);
                        }
                    }

                    GenerateClass(file, uClass);
                    classNames.insert(std::make_pair(classFullName, uClass->ObjectInternalInteger));
                }   
            }
        }
    }

    void ProcessClasses(std::ofstream& file, class UObject* uPackageObj)
    {
        for (UObject* object : *UObject::GObjObjects())
        {
            if (object)
            {
                UObject* packageObject = object->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == uPackageObj && object->IsA(UClass::StaticClass()))
                    {
                        GenerateClassProperties(file, static_cast<UClass*>(object), packageObject);
                    }
                }
            }
        }
    }
}

namespace ParameterGenerator
{
    void GenerateParameter(std::ofstream& file, class UClass* uClass)
    {
        std::ostringstream parameterStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        std::vector<UFunction*> vFunction;

        for (UProperty* property = static_cast<UProperty*>(uClass->Children); property; property = static_cast<UProperty*>(property->Next))
        {
            if (property->IsA(UFunction::StaticClass()))
            {
                vFunction.push_back(reinterpret_cast<UFunction*>(property));
            }
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* function = vFunction[i];

            std::string functionFullName = function->GetFullName();
            std::string functionName = Generator::CreateValidName(function->GetName());
            std::string classNameCPP = Generator::CreateValidName(uClass->GetNameCPP());

            parameterStream << "// " << functionFullName << "\n";
            parameterStream << "// [" << Printer::Hex(function->FunctionFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_FUNCTION)) << "] ";

            if ((function->FunctionFlags & EFunctionFlags::FUNC_Native) && function->iNative)
            {
                parameterStream << " (iNative[" << std::to_string(function->iNative) << "])";
            }

            if (function->FunctionFlags & EFunctionFlags::FUNC_Exec) { propertyStream << "exec"; }
            else if (function->FunctionFlags & EFunctionFlags::FUNC_Event) { propertyStream << "event"; }
            else { propertyStream << "exec"; }

            Generator::MakeWindowsFunction(functionName);

            parameterStream << "\nstruct " << classNameCPP << "_" << propertyStream.str() << functionName << "_Params\n" << "{\n";

            Printer::Empty(propertyStream);

            std::vector<UProperty*> vProperty;

            for (UProperty* property = static_cast<UProperty*>(function->Children); property; property = static_cast<UProperty*>(property->Next))
            {
                if (property->ElementSize > 0)
                {
                    vProperty.push_back(property);
                }
            }

            std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

            std::map<std::string, uint32_t> propertyNameMap;

            for (size_t i = 0; i < vProperty.size(); i++)
            {
                UProperty* property = vProperty[i];
                std::string propertyType;

                if (Retrievers::GetPropertyType(property, propertyType, true) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    std::string propertyName = Generator::CreateValidName(property->GetName());

                    if (propertyNameMap.count(propertyName) == 0)
                    {
                        propertyNameMap[propertyName] = 1;
                        propertyStream << propertyName;
                    }
                    else
                    {
                        propertyStream << propertyName << Printer::Decimal(propertyNameMap[propertyName], static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyNameMap[propertyName]++;
                    }

                    if (property->ArrayDim > 1)
                    {
                        propertyStream << "[" << Printer::Hex(property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "]";
                    }

                    if (property->IsA(UBoolProperty::StaticClass()))
                    {
                        propertyStream << " : 1";
                    }

                    propertyStream << ";";

                    Retrievers::GetAllPropertyFlags(flagStream, property->PropertyFlags);

                    if (property->PropertyFlags & EPropertyFlags::CPF_Parm)
                    {
                        parameterStream << "\t";
                        Printer::FillLeft(parameterStream, ' ', Configuration::FunctionSpacer);
                        parameterStream << propertyType << " ";
                        Printer::FillLeft(parameterStream, ' ', Configuration::FunctionSpacer);
                        parameterStream << propertyStream.str() << "\t\t// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        parameterStream << " (" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                        parameterStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";
                    }
                    else
                    {
                        parameterStream << "\t// ";
                        Printer::FillLeft(parameterStream, ' ', Configuration::FunctionSpacer);
                        parameterStream << propertyType << " ";
                        Printer::FillLeft(parameterStream, ' ', Configuration::FunctionSpacer);
                        parameterStream << propertyStream.str() << "\t\t// " << Printer::Hex(property->Offset, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE));
                        parameterStream << " (" << Printer::Hex(property->ElementSize * property->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_SIZE)) << ")";
                        parameterStream << " [" << Printer::Hex(property->PropertyFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_PROPERTY)) << "] ";
                    }

                    if (property->IsA(UBoolProperty::StaticClass()))
                    {
                        parameterStream << "[" << Printer::Hex(static_cast<UBoolProperty*>(property)->BitMask, static_cast<uint64_t>(EWidthTypes::WIDTH_BITMASK)) << "] ";
                    }
                    else
                    {
                        Printer::FillLeft(parameterStream, ' ', static_cast<uint64_t>(EWidthTypes::WIDTH_FIELD));
                    }

                    parameterStream << flagStream.str() << "\n";

                    Printer::Empty(propertyStream);
                    Printer::Empty(flagStream);
                }
                else
                {
                    parameterStream << "\t// UNKNOWN PROPERTY: " << property->GetFullName() << "\n";
                }
            }

            parameterStream << "};\n\n";
        }

        file << parameterStream.str();
    }

    void ProcessParameters(std::ofstream& file, UObject* packageObj)
    {
        for (UObject* object : *UObject::GObjObjects())
        {
            if (object)
            {
                UObject* packageObject = object->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == packageObj && object->IsA(UClass::StaticClass()))
                    {
                        GenerateParameter(file, static_cast<UClass*>(object));
                    }
                }
            }
        }
    }
}

namespace FunctionGenerator
{
    void GenerateVirtualFunctions(std::ofstream& file)
    {
        uintptr_t processEventAddress = 0x0;
        
        if (!Configuration::UsingOffsets)
        {
            processEventAddress = Utils::FindPattern(GetModuleHandle(NULL), Configuration::ProcessEventPattern, Configuration::ProcessEventMask);
        }
        else
        {
            processEventAddress = reinterpret_cast<uintptr_t*>(UObject::StaticClass()->VfTableObject.Dummy)[Configuration::ProcessEventIndex];
        }

        if (processEventAddress != 0x0)
        {
            file << "\n\t// Virtual Functions\n\n";

            for (int32_t index = 0; index < 512; index++)
            {
                uintptr_t virtualFunction = reinterpret_cast<uintptr_t*>(UObject::StaticClass()->VfTableObject.Dummy)[index];

                if (virtualFunction == processEventAddress)
                {
                    file << "\tvirtual void ProcessEvent(class UFunction* function, void* uParams, void* uResult = nullptr);";
                    file << "\t\t\t// " << Printer::Hex(virtualFunction, sizeof(virtualFunction)) << "\n";
                    break;
                }
                else
                {
                    file << "\tvirtual void VirtualFunction" << std::to_string(index) << "();";
                    file << "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t// " << Printer::Hex(virtualFunction, sizeof(virtualFunction)) << "\n";
                }
            }
        }
        else
        {
            file << "\n\t// FIX PROCESS EVENT IN CONFIGURATION.CPP";
            Utils::Messagebox("Warning: Process event is not configured correctly in \"Configuration.cpp\"!", "UE3SDKGenerator", MB_ICONWARNING | MB_OK);
        }
    }

    void GenerateFunctionCode(std::ofstream& file, class UClass* uClass)
    {
        std::ostringstream codeStream;
        std::ostringstream functionStream;
        std::vector<UFunction*> vFunction;

        static bool objectFunctions = false;
        static bool functionFunctions = false;

        if (!objectFunctions && uClass == UObject::StaticClass())
        {
            codeStream << PiecesOfCode::UObject_Functions;

            if (Configuration::UsingDetours)
            {
                codeStream << "template<typename T> T GetVirtualFunction(const void* instance, std::size_t index)\n";
                codeStream << "{\n";
                codeStream << "\tauto vtable = *static_cast<const void***>(const_cast<void*>(instance));\n";
                codeStream << "\treturn reinterpret_cast<T>(vtable[index]);\n";
                codeStream << "}\n\n";

                codeStream << "void UObject::ProcessEvent(class UFunction* function, void* uParams, void* uResult = nullptr)\n";
                codeStream << "{\n";
                codeStream << "\tGetVirtualFunction<void(*)(class UObject*, class UFunction*, void*)>(this, " << Configuration::ProcessEventIndex << ")(this, function, uParams);\n";
                codeStream << "}\n\n";
            }

            objectFunctions = true;
        }

        if (!functionFunctions && uClass == UFunction::StaticClass())
        {
            codeStream << PiecesOfCode::UFunction_Functions;

            functionFunctions = true;
        }

        for (UProperty* property = static_cast<UProperty*>(uClass->Children); property; property = static_cast<UProperty*>(property->Next))
        {
            if (property->IsA(UFunction::StaticClass()))
            {
                vFunction.push_back(reinterpret_cast<UFunction*>(property));
            }
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* function = vFunction[i];

            std::string functionFullName = function->GetFullName();
            std::string functionName = Generator::CreateValidName(function->GetName());
            std::string classNameCPP = Generator::CreateValidName(uClass->GetNameCPP());

            Retrievers::GetAllFunctionFlags(functionStream, function->FunctionFlags);
            codeStream << "// " << functionFullName << "\n";
            codeStream << "// [" << Printer::Hex(function->FunctionFlags, static_cast<uint64_t>(EWidthTypes::WIDTH_FUNCTION)) << "] " << functionStream.str();
            Printer::Empty(functionStream);

            if ((function->FunctionFlags & EFunctionFlags::FUNC_Native) && function->iNative)
            {
                codeStream << " (iNative[" << std::to_string(function->iNative) << "])";
            }

            std::vector<std::pair<UProperty*, std::string>> propertyParams;
            std::vector<std::pair<UProperty*, std::string>> propertyOutParams;
            std::vector<std::pair<UProperty*, std::string>> propertyBothParams;
            std::pair<UProperty*, std::string> propertyReturnParm;
            std::map<std::string, uint32_t> propertyNameMap;

            for (UProperty* property = static_cast<UProperty*>(function->Children); property; property = static_cast<UProperty*>(property->Next))
            {
                if (property->ElementSize > 0)
                {
                    std::string propertyNameBuffer = Generator::CreateValidName(property->GetName());
                    std::string propertyNameUnique;

                    if (propertyNameMap.count(propertyNameBuffer) == 0)
                    {
                        propertyNameMap[propertyNameBuffer] = 1;
                        propertyNameUnique = propertyNameBuffer;
                    }
                    else
                    {
                        functionStream << propertyNameBuffer << Printer::Decimal(propertyNameMap[propertyNameBuffer], static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyNameUnique = functionStream.str();
                        Printer::Empty(functionStream);
                        propertyNameMap[propertyNameBuffer]++;
                    }

                    if (property->PropertyFlags & EPropertyFlags::CPF_ReturnParm)
                    {
                        propertyReturnParm = make_pair(property, propertyNameUnique);
                    }
                    else if ((property->PropertyFlags & EPropertyFlags::CPF_Parm) && (property->PropertyFlags & EPropertyFlags::CPF_OutParm))
                    {
                        propertyOutParams.push_back(make_pair(property, propertyNameUnique));
                        propertyBothParams.push_back(make_pair(property, propertyNameUnique));
                    }
                    else if (property->PropertyFlags & EPropertyFlags::CPF_Parm)
                    {
                        propertyParams.push_back(make_pair(property, propertyNameUnique));
                    }
                }
            }

            std::sort(propertyParams.begin(), propertyParams.end(), Utils::SortPropertyPair);
            std::sort(propertyOutParams.begin(), propertyOutParams.end(), Utils::SortPropertyPair);
            std::sort(propertyBothParams.begin(), propertyBothParams.end(), Utils::SortPropertyPair);

            codeStream << "\n// Parameter info:\n";

            std::string propertyType;

            if (propertyReturnParm.first && Retrievers::GetPropertyType(propertyReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UNKNOWN)
            {
                Retrievers::GetAllPropertyFlags(functionStream, propertyReturnParm.first->PropertyFlags);
                codeStream << "// ";
                Printer::FillLeft(codeStream, ' ', Configuration::CommentSpacer);
                codeStream << propertyType << " ";
                Printer::FillLeft(codeStream, ' ', Configuration::CommentSpacer);
                codeStream << propertyReturnParm.second << " " << functionStream.str() << "\n";
                Printer::Empty(functionStream);
            }

            for (size_t i = 0; i < propertyParams.size(); i++)
            {
                std::pair<UProperty*, std::string> property(propertyParams[i]);

                if (Retrievers::GetPropertyType(property.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    Retrievers::GetAllPropertyFlags(functionStream, property.first->PropertyFlags);
                    codeStream << "// ";
                    Printer::FillLeft(codeStream, ' ', Configuration::CommentSpacer);
                    codeStream << propertyType << " ";
                    Printer::FillLeft(codeStream, ' ', Configuration::CommentSpacer);
                    codeStream << property.second << " " << functionStream.str() << "\n";
                    Printer::Empty(functionStream);
                }
            }

            for (size_t i = 0; i < propertyOutParams.size(); i++)
            {
                std::pair<UProperty*, std::string> property(propertyOutParams[i]);

                if (Retrievers::GetPropertyType(property.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    Retrievers::GetAllPropertyFlags(functionStream, property.first->PropertyFlags);
                    codeStream << "// ";
                    Printer::FillLeft(codeStream, ' ', Configuration::CommentSpacer);
                    codeStream << propertyType << " ";
                    Printer::FillLeft(codeStream, ' ', Configuration::CommentSpacer);
                    codeStream << property.second << " " << functionStream.str() << "\n";
                    Printer::Empty(functionStream);
                }
            }

            if (propertyReturnParm.first && Retrievers::GetPropertyType(propertyReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UNKNOWN)
            {
                codeStream << "\n" << propertyType;
            }
            else
            {
                codeStream << "\nvoid";
            }

            Generator::MakeWindowsFunction(functionName);

            if (function->FunctionFlags & EFunctionFlags::FUNC_Exec) { codeStream << " " << classNameCPP << "::" << functionName << "("; }
            else if (function->FunctionFlags & EFunctionFlags::FUNC_Event) { codeStream << " " << classNameCPP << "::event" << functionName << "("; }
            else { codeStream << " " << classNameCPP << "::" << functionName << "("; }

            bool printComma = false;

            for (size_t i = 0; i < propertyParams.size(); i++)
            {
                std::pair<UProperty*, std::string> property(propertyParams[i]);

                if (Retrievers::GetPropertyType(property.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (printComma)
                    {
                        codeStream << ", ";
                    }

                    codeStream << propertyType << " " << property.second;

                    printComma = true;
                }
            }

            for (size_t i = 0; i < propertyOutParams.size(); i++)
            {
                std::pair<UProperty*, std::string> property(propertyOutParams[i]);

                if (Retrievers::GetPropertyType(property.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (property.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                    {
                        if (printComma)
                        {
                            codeStream << ", ";
                        }

                        codeStream << propertyType << "& " << property.second;

                        printComma = true;
                    }
                }
            }

            if (Configuration::UsingConstants)
            {
                codeStream << ")\n";
                codeStream << "{\n";
                codeStream << "\tstatic UFunction* uFn" << functionName << " = nullptr;\n\n";
                codeStream << "\tif (!uFn" << functionName << ")\n";
                codeStream << "\t{\n";
                codeStream << "\t\tuFn" << functionName << " = reinterpret_cast<UFunction*>(UObject::GObjObjects()->At(" << Generator::CreateValidName(Generator::CreateIndexName(function, true)) << "));\n";
                codeStream << "\t}\n\n";
                codeStream << "\t" << classNameCPP << "_";
            }
            else
            {
                codeStream << ")\n";
                codeStream << "{\n";
                codeStream << "\tstatic UFunction* uFn" << functionName << " = nullptr;\n\n";
                codeStream << "\tif (!uFn" << functionName << ")\n";
                codeStream << "\t{\n";
                codeStream << "\t\tuFn" << functionName << " = UFunction::FindFunction(\"" << functionFullName << "\");\n";
                codeStream << "\t}\n\n";
                codeStream << "\t" << classNameCPP << "_";
            }

            if (function->FunctionFlags & EFunctionFlags::FUNC_Exec) { codeStream << "exec"; }
            else if (function->FunctionFlags & EFunctionFlags::FUNC_Event) { codeStream << "event"; }
            else { codeStream << "exec"; }

            codeStream << functionName << "_Params " << functionName << "_Params;\n";

            for (size_t i = 0; i < propertyParams.size(); i++)
            {
                std::pair<UProperty*, std::string> property(propertyParams[i]);

                EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(property.first, propertyType, false);

                if (property.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                {
                    EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(property.first, propertyType, false);

                    if (propertyTypeResult != EPropertyTypes::TYPE_UNKNOWN)
                    {
                        if (!Utils::IsBitField(propertyTypeResult) || !Utils::IsBitField(property.first->ArrayDim))
                        {
                            codeStream << "\tmemcpy_s(&" << functionName << "_Params." << property.second << ", ";
                            codeStream << Printer::Hex(property.first->ElementSize * property.first->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ", &" << property.second << ", ";
                            codeStream << Printer::Hex(property.first->ElementSize * property.first->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ");\n";
                        }
                        else if (!Utils::IsStructProperty(propertyTypeResult))
                        {
                            codeStream << "\t" << functionName << "_Params." << property.second << " = " << property.second << ";\n";
                        }
                    }
                }
            }

            for (size_t i = 0; i < propertyBothParams.size(); i++)
            {
                std::pair<UProperty*, std::string> property(propertyBothParams[i]);

                EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(property.first, propertyType, false);

                if (property.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                {
                    EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(property.first, propertyType, false);

                    if (propertyTypeResult != EPropertyTypes::TYPE_UNKNOWN)
                    {
                        if (!Utils::IsBitField(propertyTypeResult) || !Utils::IsBitField(property.first->ArrayDim))
                        {
                            codeStream << "\tmemcpy_s(&" << functionName << "_Params." << property.second << ", ";
                            codeStream << Printer::Hex(property.first->ElementSize * property.first->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ", &" << property.second << ", ";
                            codeStream << Printer::Hex(property.first->ElementSize * property.first->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ");\n";
                        }
                        else if (!Utils::IsStructProperty(propertyTypeResult))
                        {
                            codeStream << "\t" << functionName << "_Params." << " = " << property.second << ";\n";
                        }
                    }
                }
            }

            bool hasNativeIndex = function->iNative > 0 ? true : false;
            bool isNativeFunction = function->FunctionFlags & EFunctionFlags::FUNC_Native;

            if (isNativeFunction && hasNativeIndex)
            {
                codeStream << "\n\tuint16_t iNativeIndex = uFn" << functionName << "->iNative;\n\tuFn" << functionName << "->iNative = 0;\n";
            }

            if (isNativeFunction)
            {
                codeStream << "\n\tuFn" << functionName << "->FunctionFlags |= ~" << Printer::Hex(EFunctionFlags::FUNC_Native, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << ";\n";
            }

            if ((function->FunctionFlags & EFunctionFlags::FUNC_Static) && (function->FunctionFlags != EFunctionFlags::FUNC_AllFlags))
            {
                codeStream << "\n\t" << classNameCPP << "::StaticClass()->ProcessEvent(" << "uFn" << functionName << ", &" << functionName << "_Params, nullptr);\n";
            }
            else
            {
                codeStream << "\n\tthis->ProcessEvent(uFn" << functionName << ", &" << functionName << "_Params, nullptr);\n";
            }

            if (isNativeFunction)
            {
                codeStream << "\n\tuFn" << functionName << "->FunctionFlags |= " << Printer::Hex(EFunctionFlags::FUNC_Native, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << ";\n";
            }

            if (isNativeFunction && hasNativeIndex)
            {
                codeStream << "\n\tuFn" << functionName << "->iNative = iNativeIndex;\n";
            }

            if (propertyOutParams.size() > 0)
            {
                codeStream << "\n";

                for (size_t i = 0; i < propertyOutParams.size(); i++)
                {
                    std::pair<UProperty*, std::string> property(propertyOutParams[i]);

                    if (property.first->PropertyFlags & EPropertyFlags::CPF_OutParm)
                    {
                        EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(property.first, propertyType, false);

                        if (propertyTypeResult != EPropertyTypes::TYPE_UNKNOWN)
                        {
                            if (!Utils::IsBitField(propertyTypeResult) || !Utils::IsBitField(property.first->ArrayDim))
                            {
                                codeStream << "\tmemcpy_s(&" << property.second << ", ";
                                codeStream << Printer::Hex(property.first->ElementSize * property.first->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE));
                                codeStream << ", &" << functionName << "_Params." << property.second << ", ";
                                codeStream << Printer::Hex(property.first->ElementSize * property.first->ArrayDim, static_cast<uint64_t>(EWidthTypes::WIDTH_NONE));
                                codeStream << ");\n";
                            }
                            else if (!Utils::IsStructProperty(propertyTypeResult))
                            {
                                codeStream << "\t" << property.second << " = " << functionName << "_Params." << property.second << ";\n";
                            }
                        }
                    }
                }
            }

            if (propertyReturnParm.first && Retrievers::GetPropertyType(propertyReturnParm.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
            {
                codeStream << "\n\treturn " << functionName << "_Params." << propertyReturnParm.second << ";\n";
            }

            codeStream << "};\n\n";
        }

        file << codeStream.str();
    }

    void GenerateFunctionDescription(std::ofstream& file, class UClass* uClass)
    {
        std::ostringstream functionStream;
        std::ostringstream propertyStream;

        std::vector<UFunction*> vFunction;

        for (UProperty* property = static_cast<UProperty*>(uClass->Children); property; property = static_cast<UProperty*>(property->Next))
        {
            if (property->IsA(UFunction::StaticClass()))
            {
                vFunction.push_back(reinterpret_cast<UFunction*>(property));
            }
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* function = vFunction[i];

            std::string functionName = Generator::CreateValidName(function->GetName());

            std::vector<std::pair<UProperty*, std::string>> propertyParams;
            std::vector<std::pair<UProperty*, std::string>> propertyOutParams;
            std::pair<UProperty*, std::string> propertyReturnParm;

            std::map<std::string, uint32_t> propertyNameMap;

            for (UProperty* property = static_cast<UProperty*>(function->Children); property; property = static_cast<UProperty*>(property->Next))
            {
                if (property->ElementSize > 0)
                {
                    std::string propertyNameBuffer = Generator::CreateValidName(property->GetName());
                    std::string propertyNameUnique;

                    if (propertyNameMap.count(propertyNameBuffer) == 0)
                    {
                        propertyNameMap[propertyNameBuffer] = 1;
                        propertyNameUnique = propertyNameBuffer;
                    }
                    else
                    {
                        propertyStream << propertyNameBuffer << Printer::Decimal(propertyNameMap[propertyNameBuffer], static_cast<uint64_t>(EWidthTypes::WIDTH_BYTE));
                        propertyNameUnique = propertyStream.str();
                        Printer::Empty(propertyStream);
                        propertyNameMap[propertyNameBuffer]++;
                    }

                    if (property->PropertyFlags & EPropertyFlags::CPF_ReturnParm) { propertyReturnParm = std::make_pair(property, propertyNameUnique); }
                    else if (property->PropertyFlags & EPropertyFlags::CPF_OutParm) { propertyOutParams.push_back(std::make_pair(property, propertyNameUnique)); }
                    else if (property->PropertyFlags & EPropertyFlags::CPF_Parm) { propertyParams.push_back(std::make_pair(property, propertyNameUnique)); }
                }
            }

            std::sort(propertyParams.begin(), propertyParams.end(), Utils::SortPropertyPair);
            std::sort(propertyOutParams.begin(), propertyOutParams.end(), Utils::SortPropertyPair);

            bool isStatic = (function->FunctionFlags & EFunctionFlags::FUNC_Static) && (function->FunctionFlags != EFunctionFlags::FUNC_AllFlags);

            std::string propertyType;

            if (propertyReturnParm.first && Retrievers::GetPropertyType(propertyReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UNKNOWN)
            {
                functionStream << "\t" << (isStatic ? "static " : "") << propertyType;
            }
            else
            {
                functionStream << "\t" << (isStatic ? "static " : "") << "void";
            }

            Generator::MakeWindowsFunction(functionName);

            if (function->FunctionFlags & EFunctionFlags::FUNC_Exec) { functionStream << " " << functionName << "("; }
            else if (function->FunctionFlags & EFunctionFlags::FUNC_Event) { functionStream << " event" << functionName << "("; }
            else { functionStream << " " << functionName << "("; }

            bool printComma = false;

            for (size_t i = 0; i < propertyParams.size(); i++ )
            {
                std::pair<UProperty*, std::string> property(propertyParams[i]);

                if (Retrievers::GetPropertyType(property.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (printComma)
                    {
                        functionStream << ", ";
                    }

                    functionStream << propertyType << " " << property.second;

                    printComma = true;
                }
            }

            for (size_t i = 0; i < propertyOutParams.size(); i++ )
            {
                std::pair<UProperty*, std::string> property(propertyOutParams[i]);

                if (Retrievers::GetPropertyType(property.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (property.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                    {
                        if (printComma)
                        {
                            functionStream << ", ";
                        }

                        functionStream << propertyType << "& " << property.second;

                        printComma = true;
                    }
                }
            }

            functionStream << ");\n";
        }

        file << functionStream.str();
    }

    void ProcessFunctions(std::ofstream& file, class UObject* packageObj)
    {
        for (UObject* object : *UObject::GObjObjects())
        {
            if (object)
            {
                UObject* packageObject = object->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == packageObj && object->IsA(UClass::StaticClass()))
                    {
                        GenerateFunctionCode(file, static_cast<UClass*>(object));
                    }
                }
            }
        }
    }
}

namespace Generator
{
    bool GlobalsInitialized = false;
    std::ofstream LogFile;
    std::vector<class UObject*> vPackages{};
    std::vector<class UObject*> vIncludes{};
    std::vector<std::pair<std::string, int32_t>> vConstants{};

    std::string CreateValidName(const std::string& invalidName)
    {
        std::string newName = invalidName;

        for (size_t i = 0; i < newName.size(); i++)
        {
            if (newName[i] == ' '
                || newName[i] == '?'
                || newName[i] == '+'
                || newName[i] == '-'
                || newName[i] == ':'
                || newName[i] == '/'
                || newName[i] == '^'
                || newName[i] == '('
                || newName[i] == ')'
                || newName[i] == '['
                || newName[i] == ']'
                || newName[i] == '<'
                || newName[i] == '>'
                || newName[i] == '&'
                || newName[i] == '.'
                || newName[i] == '#'
                || newName[i] == '\''
                || newName[i] == '"'
                || newName[i] == '%')
            {
                newName[i] = '_';
            }
        }

        return newName;
    }

    std::string CreateUniqueName(class UClass* uClass)
    {
        std::ostringstream nameBuffer;

        UClass* superClass = static_cast<UClass*>(uClass->SuperField);
        std::string classNameCPP = CreateValidName(uClass->GetNameCPP());

        for (size_t i = 0; i < classNameCPP.size(); i++)
        {
            classNameCPP[i] = std::toupper(classNameCPP[i]);
        }

        nameBuffer << classNameCPP;

        if (superClass)
        {
            std::string superClassNameCPP = CreateValidName(superClass->GetNameCPP());

            for (size_t i = 0; i < superClassNameCPP.size(); i++)
            {
                superClassNameCPP[i] = std::toupper(superClassNameCPP[i]);
            }

            nameBuffer << "_" << superClassNameCPP;
        }

        return nameBuffer.str();
    }

    std::string CreateUniqueName(class UFunction* function, class UClass* uClass)
    {
        std::ostringstream nameBuffer;

        std::string functionName = CreateValidName(function->GetName());
        std::string classNameCPP = CreateValidName(uClass->GetNameCPP());

        for (size_t i = 0; i < functionName.size(); i++)
        {
            functionName[i] = std::toupper(functionName[i]);
        }

        for (size_t i = 0; i < classNameCPP.size(); i++)
        {
            classNameCPP[i] = std::toupper(classNameCPP[i]);
        }

        nameBuffer << classNameCPP << "_" << functionName;

        return nameBuffer.str();
    }

    std::string CreateIndexName(class UObject* uObject, bool pushBack)
    {
        std::string objectFullName = uObject->GetFullName();

        for (size_t i = 0; i < objectFullName.length(); i++)
        {
            objectFullName[i] = std::toupper(objectFullName[i]);
        }

        for (size_t i = 0; i < objectFullName.length(); ++i)
        {
            if (objectFullName[i] == ' ' || objectFullName[i] == '.')
            {
                objectFullName[i] = '_';
            }
        }

        objectFullName = "IDX_" + objectFullName;

        if (Configuration::UsingConstants && pushBack)
        {
            std::pair<std::string, int> pConstant = std::make_pair(objectFullName, uObject->ObjectInternalInteger);

            if (std::find(vConstants.begin(), vConstants.end(), pConstant) == vConstants.end())
            {
                vConstants.push_back(pConstant);
            }
        }

        return objectFullName;
    }

    void MakeWindowsFunction(std::string& functionName)
    {
        size_t gctW = functionName.find("GetCurrentTime");
        size_t goW = functionName.find("GetObject");
        size_t dfW = functionName.find("DeleteFile");
        size_t dtW = functionName.find("DrawText");
        size_t smW = functionName.find("SendMessage");

        if (gctW != std::string::npos
            || goW != std::string::npos
            || dfW != std::string::npos
            || dtW != std::string::npos
            || smW != std::string::npos)
        {
            functionName += "W";
        }
    }

    void GenerateConstants()
    {
        if (Configuration::UsingConstants)
        {
            std::ofstream file(Configuration::GeneratorDirectory / Configuration::GameNameShort / "SdkConstants.hpp");

            file << "#pragma once\n";

            for (UObject* object : *UObject::GObjObjects())
            {
                if (object)
                {
                    if (object->IsA(UFunction::StaticClass()) || object->IsA(UClass::StaticClass()))
                    {
                        std::string objectFullName = CreateIndexName(object, false);
                        objectFullName = CreateValidName(objectFullName);

                        std::pair<std::string, int32_t> constant = std::make_pair(objectFullName, object->ObjectInternalInteger);

                        if (std::find(vConstants.begin(), vConstants.end(), constant) == vConstants.end())
                        {
                            vConstants.push_back(constant);
                        }
                    }
                }
            }

            for (const std::pair<std::string, int32_t>& constant : vConstants)
            {
                file << "#define " << constant.first;
                Printer::FillRight(file, ' ', 150 - constant.first.size());
                file << std::to_string(constant.second) << "\n";
            }

            file.close();
        }
    }

    void GenerateHeaders()
    {
        std::ofstream file(Configuration::GeneratorDirectory / Configuration::GameNameShort / "SdkHeaders.hpp");

        Printer::Header(file, "SdkHeaders", "hpp", false);

        file << "#pragma once\n";

        Printer::Section(file, "Includes");

        file << "#include \"GameDefines.hpp\"\n";

        for (size_t i = 0; i < vIncludes.size(); i++)
        {
            file << "#include \"SDK_HEADERS\\" << vIncludes[i]->GetName() << "_structs.hpp\"\n";
            file << "#include \"SDK_HEADERS\\" << vIncludes[i]->GetName() << "_classes.hpp\"\n";
            file << "#include \"SDK_HEADERS\\" << vIncludes[i]->GetName() << "_parameters.hpp\"\n";
        }

        file << "\n";

        Printer::Footer(file, false);

        file.close();
    }

    void GenerateDefines()
    {
        std::ofstream file(Configuration::GeneratorDirectory / Configuration::GameNameShort / "GameDefines.hpp");

        Printer::Header(file, "GameDefines", "hpp", false);

        file << "#pragma once\n";
        file << "#include <algorithm>\n";
        file << "#include <locale>\n";
        file << "#include <stdlib.h>\n";
        file << "#include <xlocale>\n";
        file << "#include <ctype.h>\n";
        file << "#include <chrono>\n";
        file << "#include <thread>\n";
        file << "#include <vector>\n";
        file << "#include <map>\n";

        Printer::Section(file, "Flags");

        if (Configuration::PrintFlagEnums)
        {
            file << PiecesOfCode::EEnumFlags;
        }

        Printer::Section(file, "Globals");

        if (Configuration::UsingOffsets)
        {
            file << "// GObjects\n";
            file << "#define GObjects_Offset\t\t(uintptr_t)" << Printer::Hex(Configuration::GObjectsOffset, sizeof(Configuration::GObjectsOffset)) << "\n";

            file << "// GNames\n";
            file << "#define GNames_Offset\t\t(uintptr_t)" << Printer::Hex(Configuration::GNamesOffset, sizeof(Configuration::GNamesOffset)) << "\n";
        }
        else
        {
            file << "// GObjects\n";
            file << "#define GObjects_Pattern\t\t(const uint8_t*)\"" << Configuration::GObjectsString + "\"\n";
            file << "#define GObjects_Mask\t\t\t(const char*)\"" << std::string(Configuration::GObjectsMask) + "\"\n";

            file << "// GNames\n";
            file << "#define GNames_Pattern\t\t\t(const uint8_t*)\"" << Configuration::GNamesString + "\"\n";
            file << "#define GNames_Mask\t\t\t\t(const char*)\"" << std::string(Configuration::GNamesMask) + "\"\n";

            file << "// Process Event\n";
            file << "#define ProcessEvent_Pattern\t(const uint8_t*)\"" << Configuration::ProcessEventString << "\"\n";
            file << "#define ProcessEvent_Mask\t\t(const char*)\"" << std::string(Configuration::ProcessEventMask) << "\"\n";
        }

        Printer::Section(file, "Structs");

        file << PiecesOfCode::TArray_Iterator << "\n";
        file << PiecesOfCode::TArray_Class << "\n";
        file << PiecesOfCode::TMap_Class;

        Printer::Section(file, "Globals");

        file << "extern TArray<class UObject*>* GObjects;\n";
        file << "extern TArray<struct FNameEntry*>* GNames;\n";

        Printer::Section(file, "Structs");

#ifdef CHARACTER_UTF16
        file << PiecesOfCode::FNameEntry_UPPER;
        StructGenerator::GenerateStructFields(file, EClassTypes::CLASS_FNAMEENTRY);
        file << PiecesOfCode::FNameEntry_UTF16 << "\n";
        file << PiecesOfCode::FName_UTF16 << "\n";
        file << PiecesOfCode::FString_UTF16 << "\n";
#endif
#ifdef CHARACTER_UTF8
        file << PiecesOfCode::FNameEntry_UPPER;
        StructGenerator::GenerateStructFields(file, EClassTypes::CLASS_FNAMEENTRY);
        file << PiecesOfCode::FNameEntry_UTF8 << "\n";
        file << PiecesOfCode::FName_UTF8 << "\n";
        file << PiecesOfCode::FString_UTF8 << "\n";
#endif

        file << "struct FScriptDelegate\n";
        file << "{\n";
        file << "\tuint8_t UnknownData00[" << Printer::Hex(sizeof(FScriptDelegate), static_cast<uint64_t>(EWidthTypes::WIDTH_NONE)) << "];\n";
        file <<  "};\n";

        file << PiecesOfCode::FPointer_Struct << "\n";
        file << PiecesOfCode::FQWord_Struct << "\n";

        Printer::Footer(file, false);

        file.close();

        file.open(Configuration::GeneratorDirectory / Configuration::GameNameShort / "GameDefines.cpp");

        Printer::Header(file, "GameDefines", "cpp", false);

        file << "#include \"GameDefines.hpp\"\n";
       
        Printer::Section(file, "Initialize Global Variables");

        file << "TArray<class UObject*>* GObjects{};\n";
        file << "TArray<struct FNameEntry*>* GNames{};\n\n";

        Printer::Footer(file, false);

        file.close();
    }

    void ProcessPackages(std::filesystem::path directory)
    {
        if (std::filesystem::exists(directory))
        {
            for (UObject* object : *UObject::GObjObjects())
            {
                if (object)
                {
                    if (object->IsA(UClass::StaticClass()))
                    {
                        UObject* packageObject = object->GetPackageObj();

                        if (packageObject)
                        {
                            if (find(vPackages.begin(), vPackages.end(), packageObject) == vPackages.end())
                            {
                                vPackages.push_back(packageObject);
                                std::string packageName = CreateValidName(packageObject->GetName());

                                LogFile << "\nProcessing Package: " << packageName << "\n\n";

                                std::ofstream file;

                                // Structs

                                file.open(directory / (packageName + "_structs.hpp"));

                                Printer::Header(file, packageName + "_structs", "hpp", true);
                                Printer::Section(file, "Script Structs");
                                StructGenerator::ProcessStructs(file, packageObject);
                                Printer::Footer(file, true);

                                file.close();

                                // Classes

                                file.open(directory / (packageName + "_classes.hpp"));

                                Printer::Header(file, packageName + "_classes", "hpp", true);
                                Printer::Section(file, "Constants");
                                ConstGenerator::ProcessConsts(file, packageObject);
                                Printer::Section(file, "Enums");
                                EnumGenerator::ProcessEnums(file, packageObject);
                                Printer::Section(file, "Classes");
                                ClassGenerator::ProcessClasses(file, packageObject);
                                Printer::Footer(file, true);                 

                                file.close();

                                // Parameters

                                file.open(directory / (packageName + "_parameters.hpp"));

                                Printer::Header(file, packageName + "_parameters", "hpp", true);
                                Printer::Section(file, "Function Parameters");
                                ParameterGenerator::ProcessParameters(file, packageObject);
                                Printer::Footer(file, true);

                                file.close();

                                // Functions

                                file.open(directory / (packageName + "_classes.cpp"));

                                Printer::Header(file, packageName + "_classes", "cpp", true);
                                Printer::Section(file, "Functions");
                                FunctionGenerator::ProcessFunctions(file, packageObject);
                                Printer::Footer(file, true);

                                file.close();
                            }   
                        }
                    }
                }
            }
        }
        else
        {
            Utils::Messagebox("Failed locate the desired directory, cannot generate an SDK at this time!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);
        }
    }

    void GenerateSDK()
    {
        std::filesystem::path fullDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort;
        std::filesystem::path headerDirectory = fullDirectory / "SDK_HEADERS";

        std::filesystem::create_directory(Configuration::GeneratorDirectory);
        std::filesystem::create_directory(fullDirectory);
        std::filesystem::create_directory(headerDirectory);

        if (std::filesystem::exists(headerDirectory))
        {
            if (Initialize(true))
            {
                Utils::Messagebox("SDK generation has started, do not close the game until prompted to do so!", "UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);

                std::chrono::time_point startTime = std::chrono::system_clock::now();

                ProcessPackages(headerDirectory);
                GenerateHeaders();
                GenerateDefines();
                GenerateConstants();

                std::chrono::time_point endTime = std::chrono::system_clock::now();
                std::chrono::duration<float> elapsedTime = endTime - startTime;
                std::string formattedTime = Printer::Precision(elapsedTime.count(), 4);

                if (LogFile.is_open())
                {
                    LogFile << "\n" << Configuration::GameNameShort << " generated in " << formattedTime << " seconds.";
                    LogFile.close();
                }

                Utils::Messagebox("Finished in " + formattedTime + " seconds, it is now safe to close the game.", "UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);
            }
        }
        else
        {
            Utils::Messagebox("Failed to create the desired directory, cannot generate an SDK at this time!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);
        }
    }

    void DumpInstances(bool dumpNames, bool dumpObjects)
    {
        if (Initialize(false))
        {
            std::filesystem::path fullDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort;
            std::filesystem::create_directory(Configuration::GeneratorDirectory);
            std::filesystem::create_directory(fullDirectory);

            if (std::filesystem::exists(fullDirectory))
            {
                uintptr_t entryPoint = Retrievers::GetEntryPoint();

                if (dumpNames)
                {
                    std::ofstream file(fullDirectory / "NameDump.txt");

                    uintptr_t offset = Retrievers::GetOffset(reinterpret_cast<uintptr_t>(GNames));

                    file << "Entry Point: " << Printer::Hex(entryPoint, sizeof(entryPoint)) << "\n";
                    file << "GNames: " << Printer::Hex(reinterpret_cast<uintptr_t>(GNames), sizeof(GNames)) << "\n";
                    file << "Offset: " << Printer::Hex(offset, sizeof(offset)) << "\n\n";

                    for (FNameEntry* nameEntry : *FName::Names())
                    {
                        if (nameEntry)
                        {
                            std::string name = nameEntry->ToString();

                            file << "Name[";
                            Printer::FillRight(file, '0', 6);
                            file << std::to_string(nameEntry->GetIndex()) << "] ";
                            file << name << " ";
                            Printer::FillRight(file, ' ', 50 - name.length());
                            file << Printer::Hex(reinterpret_cast<uintptr_t>(nameEntry), sizeof(nameEntry)) << "\n";
                        }
                    }

                    file.close();
                }

                if (dumpObjects)
                {
                    std::ofstream file(fullDirectory / "ObjectDump.txt");

                    uintptr_t offset = Retrievers::GetOffset(reinterpret_cast<uintptr_t>(GObjects));

                    file << "Entry Point: " << Printer::Hex(entryPoint, sizeof(entryPoint)) << "\n";
                    file << "GObjects: " << Printer::Hex(reinterpret_cast<uintptr_t>(GObjects), sizeof(GObjects)) << "\n";
                    file << "Offset: " << Printer::Hex(offset, sizeof(offset)) << "\n\n";

                    for (UObject* object : *UObject::GObjObjects())
                    {
                        if (object)
                        {
                            std::string objectName = object->GetFullName();

                            file << "UObject[";
                            Printer::FillRight(file, '0', 6);
                            file << std::to_string(object->ObjectInternalInteger) << "] ";
                            file << objectName << " ";
                            Printer::FillRight(file, ' ', 50 - objectName.length());
                            file << Printer::Hex(reinterpret_cast<uintptr_t>(object), sizeof(object)) << "\n";
                        }
                    }

                    file.close();
                }
            }
            else
            {
                Utils::Messagebox("Failed to create the desired directory, cannot dump instances at this time!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);
            }

            Utils::Messagebox("Finished dumping instances!", "UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);
        }
    }

    bool Initialize(bool createLogFile)
    {
        uintptr_t entryPoint = Retrievers::GetEntryPoint();

        if (!GlobalsInitialized)
        {
            if (!Configuration::UsingOffsets)
            {
                uintptr_t GObjectsAddress = Utils::FindPattern(GetModuleHandle(NULL), Configuration::GObjectsPattern, Configuration::GObjectsMask);
                uintptr_t GNamesAddress = Utils::FindPattern(GetModuleHandle(NULL), Configuration::GNamesPattern, Configuration::GNamesMask);
                GObjects = reinterpret_cast<TArray<UObject*>*>(GObjectsAddress);
                GNames = reinterpret_cast<TArray<FNameEntry*>*>(GNamesAddress);
            } 
            else
            {
                uintptr_t GObjectsAddress = entryPoint + Configuration::GObjectsOffset;
                uintptr_t GNamesAddress = entryPoint + Configuration::GNamesOffset;
                GObjects = reinterpret_cast<TArray<UObject*>*>(GObjectsAddress);
                GNames = reinterpret_cast<TArray<FNameEntry*>*>(GNamesAddress);
            }

            if (GObjects && GNames && Utils::AreGObjectsValid() && Utils::AreGNamesValid())
            {
                FNameEntry::RegIndex();
                FNameEntry::RegName();
                UObject::RegVfTableObject();
                UObject::RegObjectInternalInteger();
                UObject::RegOuter();
                UObject::RegName();
                UObject::RegClass();
                UField::RegNext();
                UEnum::RegNames();
                UConst::RegValue();
                UProperty::RegArrayDim();
                UProperty::RegElementSize();
                UProperty::RegPropertyFlags();
                UProperty::RegOffset();
                UStruct::RegSuperField();
                UStruct::RegChildren();
                UStruct::RegPropertySize();
                UFunction::RegFunctionFlags();
                UFunction::RegiNative();
                UStructProperty::RegStruct();
                UObjectProperty::RegPropertyClass();
                UClassProperty::RegMetaClass();
                UMapProperty::RegKey();
                UMapProperty::RegValue();
                UInterfaceProperty::RegInterfaceClass();
                UDelegateProperty::RegFunction(); // Not actually needed in sdk generation at the moment.
                UDelegateProperty::RegDelegateName(); // Not actually needed in sdk generation at the moment.
                UByteProperty::RegEnum();
                UBoolProperty::RegBitMask();
                UArrayProperty::RegInner();

                GlobalsInitialized = true;
            }
            else
            {
                Utils::Messagebox("Failed to validate GObject & GNames, please make sure you have them configured properly!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);   
                return false;
            }
        }

        if (GlobalsInitialized)
        {
            if (createLogFile)
            {
                std::filesystem::path fullDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort;
                std::filesystem::create_directory(Configuration::GeneratorDirectory);
                std::filesystem::create_directory(fullDirectory);

                if (std::filesystem::exists(fullDirectory))
                {
                    LogFile.open(fullDirectory / "UE3SDKGenerator.log");

                    LogFile << "Entry Point: " << Printer::Hex(entryPoint, sizeof(entryPoint)) << "\n";
                    LogFile << "GObjects: " << Printer::Hex(reinterpret_cast<uintptr_t>(GObjects), sizeof(GObjects)) << "\n";
                    LogFile << "GNames: " << Printer::Hex(reinterpret_cast<uintptr_t>(GNames), sizeof(GNames)) << "\n";
                    LogFile.flush();
                }
                else
                {
                    Utils::Messagebox("Failed to create the log file, might not have the right permissions!", "UE3SDKGenerator", MB_ICONERROR | MB_OK);
                    return false;
                }
            }
            
            return true;
        }

        return false;
    }
}

void OnAttach(HMODULE hModule)
{
    DisableThreadLibraryCalls(hModule);
    Generator::GenerateSDK();
    Generator::DumpInstances(true, true);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(OnAttach), nullptr, 0, nullptr);
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}