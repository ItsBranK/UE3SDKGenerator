#include "dllmain.hpp"

namespace Utils
{
    MODULEINFO GetModuleInfo(LPCTSTR lpModuleName)
    {
        MODULEINFO miInfos = { NULL };

        HMODULE hmModule = GetModuleHandle(lpModuleName);

        if (hmModule)
        {
            GetModuleInformation(GetCurrentProcess(), hmModule, &miInfos, sizeof(MODULEINFO));
        }

        return miInfos;
    }

    uintptr_t FindPattern(HMODULE module, const uint8_t* pattern, const char* mask)
    {
        MODULEINFO info = { };
        GetModuleInformation(GetCurrentProcess(), module, &info, sizeof(MODULEINFO));

        uintptr_t start = reinterpret_cast<uintptr_t>(module);
        size_t length = info.SizeOfImage;

        size_t pos = 0;
        size_t maskLength = std::strlen(mask) - 1;

        for (uintptr_t retAddress = start; retAddress < start + length; retAddress++)
        {
            if (*reinterpret_cast<unsigned char*>(retAddress) == pattern[pos] || mask[pos] == '?')
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

    bool SortPropertyPair(std::pair<class UProperty*, std::string> uPropertyA, std::pair<class UProperty*, std::string> uPropertyB)
    {
        if (uPropertyA.first->Offset == uPropertyB.first->Offset && uPropertyA.first->IsA(UBoolProperty::StaticClass()) && uPropertyB.first->IsA(UBoolProperty::StaticClass()))
        {
            return (reinterpret_cast<UBoolProperty*>(uPropertyA.first)->BitMask < reinterpret_cast<UBoolProperty*>(uPropertyB.first)->BitMask);
        }
       
        return (uPropertyA.first->Offset < uPropertyB.first->Offset);
    }

    bool SortProperty(class UProperty* uPropertyA, class UProperty* uPropertyB)
    {
        if (uPropertyA->Offset == uPropertyB->Offset && uPropertyA->IsA(UBoolProperty::StaticClass()) && uPropertyB->IsA(UBoolProperty::StaticClass()))
        {
            return (reinterpret_cast<UBoolProperty*>(uPropertyA)->BitMask < reinterpret_cast<UBoolProperty*>(uPropertyB)->BitMask);
        }
       
        return (uPropertyA->Offset < uPropertyB->Offset);
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
            if (FName(0).ToString() == "None")
            {
                return true;
            }
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

    EPropertyTypes GetPropertyType(class UProperty* uProperty, std::string& propertyType, bool returnFunction)
    {
        if (uProperty)
        {
            if (uProperty->IsA(UStructProperty::StaticClass()))
            {
                UStructProperty* structProperty = reinterpret_cast<UStructProperty*>(uProperty);

                if (structProperty->Struct)
                {
                    uint32_t propertyCount = UObject::CountObject<UScriptStruct>(structProperty->Struct->GetName());

                    if (propertyCount > 1)
                    {
                        if (structProperty->Struct->Outer)
                        {
                            propertyType = "struct " + Generator::GenerateValidName(structProperty->Struct->Outer->GetNameCPP() + "_" + Generator::GenerateValidName(structProperty->Struct->GetNameCPP()));
                        }
                        else
                        {
                            propertyType = "struct " + Generator::GenerateValidName(structProperty->Struct->GetNameCPP() + "_Outer" + std::to_string(propertyCount));
                        }
                    }
                    else
                    {
                        propertyType = "struct " + Generator::GenerateValidName(structProperty->Struct->GetNameCPP());
                    }

                    return EPropertyTypes::TYPE_FSTRUCT;
                }
            }
            else if (uProperty->IsA(UStrProperty::StaticClass()))
            {
                propertyType = "class FString";
                return EPropertyTypes::TYPE_FSTRING;
            }
            else if (uProperty->IsA(UQWordProperty::StaticClass()))
            {
                propertyType = "uint64_t";
                return EPropertyTypes::TYPE_UINT64;
            }
            else if (uProperty->IsA(UObjectProperty::StaticClass()))
            { 
                UObjectProperty* objectProperty = reinterpret_cast<UObjectProperty*>(uProperty);

                if (objectProperty->PropertyClass)
                {
                    propertyType = "class " + Generator::GenerateValidName(objectProperty->PropertyClass->GetNameCPP()) + "*";
                    return EPropertyTypes::TYPE_UPOINTER;
                }
            }
            else if (uProperty->IsA(UClassProperty::StaticClass()))
            {
                UClassProperty* classProperty = reinterpret_cast<UClassProperty*>(uProperty);

                if (classProperty->MetaClass)
                {
                    propertyType = "class " + Generator::GenerateValidName(classProperty->MetaClass->GetNameCPP()) + "*";
                    return EPropertyTypes::TYPE_UPOINTER;
                }
            }
            else if (uProperty->IsA(UNameProperty::StaticClass()))
            {
                propertyType = "struct FName";
                return EPropertyTypes::TYPE_FNAME;
            }
            else if (uProperty->IsA(UMapProperty::StaticClass()))
            {
                std::string propertyTypeKey;
                std::string propertyTypeValue;

                if (GetPropertyType(reinterpret_cast<UMapProperty*>(uProperty)->Key, propertyTypeKey, returnFunction) != EPropertyTypes::TYPE_UNKNOWN && GetPropertyType(reinterpret_cast<UMapProperty*>(uProperty)->Value, propertyTypeValue, returnFunction) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    propertyType = "TMap<" + propertyTypeKey + ", " + propertyTypeValue + ">";

                    return EPropertyTypes::TYPE_TMAP;
                }
                else
                {
                    return EPropertyTypes::TYPE_UNKNOWN;
                }
            }
            else if (uProperty->IsA(UIntProperty::StaticClass()))
            {
                propertyType = "int32_t";
                return EPropertyTypes::TYPE_INT32;
            }
            else if (uProperty->IsA(UInterfaceProperty::StaticClass()))
            {
                UInterfaceProperty* interfaceProperty = reinterpret_cast<UInterfaceProperty*>(uProperty);

                if (interfaceProperty->InterfaceClass)
                {
                    propertyType = "class " + Generator::GenerateValidName(interfaceProperty->InterfaceClass->GetNameCPP()) + "*";
                }

                return EPropertyTypes::TYPE_UPOINTER;
            }
            else if (uProperty->IsA(UFloatProperty::StaticClass()))
            {
                propertyType = "float";
                return EPropertyTypes::TYPE_FLOAT;   
            }
            else if (uProperty->IsA(UDelegateProperty::StaticClass()))
            {
                propertyType = "struct FScriptDelegate";
                return EPropertyTypes::TYPE_FSCRIPTDELEGATE;
            }
            else if (uProperty->IsA(UByteProperty::StaticClass()))
            {
                propertyType = "uint8_t";
                return EPropertyTypes::TYPE_UINT8;
            }
            else if (uProperty->IsA(UBoolProperty::StaticClass()))
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
            else if (uProperty->IsA(UArrayProperty::StaticClass()))
            {
                UArrayProperty* arrayProperty = reinterpret_cast<UArrayProperty*>(uProperty);

                if (arrayProperty->Inner)
                {
                    std::string innerProperty;

                    if (GetPropertyType(arrayProperty, innerProperty, returnFunction) != EPropertyTypes::TYPE_UNKNOWN)
                    {
                        propertyType = "TArray<" + innerProperty + ">";
                        return EPropertyTypes::TYPE_TARRAY;
                    }
                    else
                    {
                        return EPropertyTypes::TYPE_UNKNOWN;
                    }
                }
            }
        }
        
        return EPropertyTypes::TYPE_UNKNOWN;
    }

    size_t GetPropertySize(UProperty* uProperty)
    {
        if (uProperty)
        {
            if (uProperty->IsA(UStructProperty::StaticClass()))
            {
                return uProperty->ElementSize;
            }
            else if (uProperty->IsA(UStrProperty::StaticClass()))
            {
                return sizeof(FString);
            }
            else if (uProperty->IsA(UQWordProperty::StaticClass()))
            {
                return sizeof(uint64_t);
            }
            else if (uProperty->IsA(UObjectProperty::StaticClass()))
            {
                return sizeof(uintptr_t);
            }
            else if (uProperty->IsA(UClassProperty::StaticClass()))
            {
                return sizeof(uintptr_t);
            }
            else if (uProperty->IsA(UNameProperty::StaticClass()))
            {
                return sizeof(FName);
            }
            else if (uProperty->IsA(UMapProperty::StaticClass()))
            {
                return uProperty->ElementSize;
            }
            else if (uProperty->IsA(UIntProperty::StaticClass()))
            {
                return sizeof(int32_t);
            }
            else if (uProperty->IsA(UInterfaceProperty::StaticClass()))
            {
                return sizeof(uintptr_t);
            }
            else if (uProperty->IsA(UFloatProperty::StaticClass()))
            {
                return sizeof(float);
            }
            else if (uProperty->IsA(UDelegateProperty::StaticClass()))
            {
                return sizeof(FScriptDelegate);
            }
            else if (uProperty->IsA(UByteProperty::StaticClass()))
            {
                return sizeof(uint8_t);
            }
            else if (uProperty->IsA(UBoolProperty::StaticClass()))
            {
                return sizeof(unsigned long);
            }
            else if (uProperty->IsA(UArrayProperty::StaticClass()))
            {
                return sizeof(TArray<uintptr_t>);
            }
        }
        
        return 0;
    }
}

namespace StructGenerator
{
    UScriptStruct* FindLargestStruct(const std::string& structFullName)
    {
        unsigned long propertySize = 0;
        UScriptStruct* largestStruct = nullptr;

        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject && uObject->IsA(UScriptStruct::StaticClass()))
            {
                if (uObject->GetFullName() == structFullName)
                {
                    UScriptStruct* uStruct = reinterpret_cast<UScriptStruct*>(uObject);

                    if (uStruct->PropertySize >= propertySize)
                    {
                        largestStruct = uStruct;
                        propertySize = uStruct->PropertySize;
                    }
                }
            }
        }

        return largestStruct;
    }

    void GenerateStruct(File& file, class UScriptStruct* uScriptStruct)
    {
        std::ostringstream structStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        std::string structFullName = uScriptStruct->GetFullName();
        std::string structName = Generator::GenerateValidName(uScriptStruct->GetName());
        std::string structNameCPP = Generator::GenerateValidName(uScriptStruct->GetNameCPP());
        std::string structOuterNameCPP = Generator::GenerateValidName(uScriptStruct->Outer->GetNameCPP());

        if (structNameCPP == "FPointer")
        {
            return;
        }

        std::string propertyTitle = "ScriptStruct: ";
        Generator::LogFile.Write(propertyTitle);
        Generator::LogFile.Pad(' ', structName.size(), false);
        Generator::LogFile.Write(structName);
        Generator::LogFile.Pad(' ', 75 - (structName.size() + propertyTitle.size()), false);
        Generator::LogFile.Write(" - Instance: ");
        Generator::LogFile.Hex(reinterpret_cast<uintptr_t>(uScriptStruct), static_cast<int32_t>(Configuration::Alignment));
        Generator::LogFile.NewLine();

        structStream << "// " << structFullName << "\n";

        int32_t size = 0;
        int32_t lastOffset = 0;
        int32_t missedOffset = 0;

        UScriptStruct* superField = reinterpret_cast<UScriptStruct*>(uScriptStruct->SuperField);
        uint32_t structCount = UObject::CountObject<UScriptStruct>(structName);

        if (superField && superField != uScriptStruct)
        {
            size = uScriptStruct->PropertySize - superField->PropertySize;
            lastOffset = superField->PropertySize;

            std::string fieldName = Generator::GenerateValidName(superField->GetName());
            std::string fieldNameCPP = Generator::GenerateValidName(superField->GetNameCPP());
            std::string fieldOuterNameCPP = Generator::GenerateValidName(superField->Outer->GetNameCPP());

            uint32_t fieldStructCount = UObject::CountObject<UScriptStruct>(fieldName);

            structStream << "// ";
            Printers::MakeHex(structStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            structStream << " (";
            Printers::MakeHex(structStream, superField->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            structStream << " - ";
            Printers::MakeHex(structStream, uScriptStruct->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            structStream << ")" << "\n";

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
            size = uScriptStruct->PropertySize;

            structStream << "// ";
            Printers::MakeHex(structStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            structStream << "\n";

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

        for (UProperty* uProperty = reinterpret_cast<UProperty*>(uScriptStruct->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
        {
            if (uProperty->ElementSize > 0 && !uProperty->IsA(UScriptStruct::StaticClass()))
            {
                vProperty.push_back(uProperty);
            }
        }

        std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

        std::map<std::string, uint32_t> propertyNameMap;
        uint32_t unknownDataIndex = 0;

        for (size_t i = 0; i < vProperty.size(); i++)
        {
            UProperty* uProperty = vProperty[i];

            if (lastOffset < uProperty->Offset)
            {
                missedOffset = uProperty->Offset - lastOffset;

                if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                    propertyStream << "];";

                    structStream << "\t";
                    Printers::MakeSpacer(structStream, Configuration::StructSpacer, "uint8_t", true);
                    structStream << " ";
                    Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                    structStream << "\t\t// ";
                    Printers::MakeHex(structStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << " (";
                    Printers::MakeHex(structStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << ") MISSED OFFSET\n";

                    Printers::EmptyStream(propertyStream);

                    unknownDataIndex++;
                }
            }

            std::string propertyType;

            if (Retrievers::GetPropertyType(uProperty, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
            {
                size_t correctElementSize = Retrievers::GetPropertySize(uProperty);

                std::string propertyName = Generator::GenerateValidName(uProperty->GetName());

                if (propertyNameMap.count(propertyName) == 0)
                {
                    propertyNameMap[propertyName] = 1;
                    propertyStream << propertyName;
                }
                else
                {
                    propertyStream << propertyName;
                    Printers::MakeDecimal(propertyStream, propertyNameMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                    propertyNameMap[propertyName]++;
                }

                if (uProperty->ArrayDim > 1)
                {
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, uProperty->ArrayDim, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                    propertyStream << "]";
                }

                if (uProperty->IsA(UBoolProperty::StaticClass()))
                {
                    propertyStream << " : 1";
                }

                propertyStream << ";";

                Retrievers::GetAllPropertyFlags(flagStream, uProperty->PropertyFlags);

                structStream << "\t";
                Printers::MakeSpacer(structStream, Configuration::StructSpacer, propertyType, true);
                structStream << " ";
                Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                structStream << "\t\t// ";
                Printers::MakeHex(structStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                structStream << " (";
                Printers::MakeHex(structStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                structStream << ") [";
                Printers::MakeHex(structStream, uProperty->PropertyFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_PROPERTY));
                structStream << "] ";

                if (uProperty->IsA(UBoolProperty::StaticClass()))
                {
                    structStream << "[";
                    Printers::MakeHex(structStream, reinterpret_cast<UBoolProperty*>(uProperty)->BitMask, static_cast<uint32_t>(EWidthTypes::WIDTH_BITMASK));
                    structStream << "] ";
                }
                else
                {
                    Printers::MakeSpacer(structStream, 14);
                }

                structStream << flagStream.str() << "\n";

                Printers::EmptyStream(propertyStream);
                Printers::EmptyStream(flagStream);

                int32_t offsetError = (uProperty->ElementSize * uProperty->ArrayDim) - (correctElementSize * uProperty->ArrayDim);

                if (offsetError > 0)
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                    propertyStream<< "];";

                    structStream << "\t";
                    Printers::MakeSpacer(structStream, Configuration::StructSpacer, "uint8_t", true);
                    structStream << " ";
                    Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                    structStream << "// ";
                    Printers::MakeHex(structStream, (uProperty->Offset + offsetError), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << " (";
                    Printers::MakeHex(structStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << ") FIX WRONG SIZE OF PREVIOUS PROPERTY [Original: ";
                    Printers::MakeHex(structStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << ", Missing: ";
                    Printers::MakeHex(structStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    structStream << "]\n";

                    Printers::EmptyStream(propertyStream);

                    unknownDataIndex++;
                }
            }
            else
            {
                propertyStream << "UnknownData";
                Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                propertyStream << "[";
                Printers::MakeHex(propertyStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                propertyStream << "];";

                structStream << "\t";
                Printers::MakeSpacer(structStream, Configuration::StructSpacer, "uint8_t", true);
                structStream << " ";
                Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                structStream << "//";
                Printers::MakeHex(structStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                structStream << " (";
                Printers::MakeHex(structStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                structStream << ") UNKNOWN PROPERTY: " << uProperty->GetFullName() << "\n";

                Printers::EmptyStream(propertyStream);

                unknownDataIndex++;
            }

            lastOffset = uProperty->Offset + (uProperty->ElementSize * uProperty->ArrayDim);
        }

        if (lastOffset < uScriptStruct->PropertySize)
        {
            missedOffset = uScriptStruct->PropertySize - lastOffset;

            if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
            {
                propertyStream << "UnknownData";
                Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                propertyStream << "[";
                Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                propertyStream << "];";

                structStream << "\t";
                Printers::MakeSpacer(structStream, Configuration::StructSpacer, "uint8_t", true);
                structStream << " ";
                Printers::MakeSpacer(structStream, propertyStream, Configuration::StructSpacer);
                structStream << "\t\t// ";
                Printers::MakeHex(propertyStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                structStream << " (";
                Printers::MakeHex(structStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                structStream << ") MISSED OFFSET\n";

                Printers::EmptyStream(propertyStream);
            }
        }

        structStream << "};\n\n";

        file.Write(structStream);
    }

    void GenerateStructProperties(File& file, class UScriptStruct* uScriptStruct, class UObject* uPackageObj)
    {
        UObject* packageObject = uScriptStruct->GetPackageObj();

        if (packageObject && packageObject == uPackageObj)
        {
            static std::vector<std::string> generatedStructs;

            std::string structFullName = uScriptStruct->GetFullName();
            std::string structName = uScriptStruct->GetName();

            if (structName.find("Default__") != std::string::npos
                && structName.find("<uninitialized>") != std::string::npos)
            {
                return;
            }

            std::string propertyType;

            if (find(generatedStructs.begin(), generatedStructs.end(), structFullName) == generatedStructs.end())
            {
                uScriptStruct = FindLargestStruct(structFullName);

                if (uScriptStruct->SuperField && uScriptStruct->SuperField != uScriptStruct && std::find(generatedStructs.begin(), generatedStructs.end(), (reinterpret_cast<UScriptStruct*>(uScriptStruct->SuperField))->GetFullName()) == generatedStructs.end())
                {
                    GenerateStructProperties(file, reinterpret_cast<UScriptStruct*>(uScriptStruct->SuperField), uPackageObj);
                }

                for (UProperty* uStructProperty = reinterpret_cast<UProperty*>(uScriptStruct->Children); uStructProperty; uStructProperty = reinterpret_cast<UProperty*>(uStructProperty->Next))
                {
                    EPropertyTypes typeResult = Retrievers::GetPropertyType(uStructProperty, propertyType, false);

                    if (typeResult == EPropertyTypes::TYPE_FSTRUCT)
                    {
                        UScriptStruct* propertyStruct = reinterpret_cast<UScriptStruct*>(reinterpret_cast<UStructProperty*>(uStructProperty)->Struct);

                        if (propertyStruct && propertyStruct != uScriptStruct && std::find(generatedStructs.begin(), generatedStructs.end(), propertyStruct->GetFullName()) == generatedStructs.end())
                        {
                            GenerateStructProperties(file, propertyStruct, uPackageObj);
                        }
                    }

                    if (typeResult == EPropertyTypes::TYPE_TARRAY)
                    {
                        UScriptStruct* propertyStruct = reinterpret_cast<UScriptStruct*>(reinterpret_cast<UStructProperty*>(reinterpret_cast<UArrayProperty*>(uStructProperty)->Inner)->Struct);

                        if (propertyStruct != uScriptStruct &&
                            Retrievers::GetPropertyType(reinterpret_cast<UArrayProperty*>(uStructProperty)->Inner, propertyType, false) == EPropertyTypes::TYPE_FSTRUCT
                            && find(generatedStructs.begin(), generatedStructs.end(), propertyStruct->GetFullName()) == generatedStructs.end())
                        {
                            GenerateStructProperties(file, propertyStruct, uPackageObj);
                        }
                    }
                }

                GenerateStruct(file, uScriptStruct);
                generatedStructs.push_back(structFullName);
            }   
        }
    }

    void ProcessStructs(File& file, UObject* uPackageObj)
    {
        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject)
            {
                UObject* packageObject = uObject->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == uPackageObj && uObject->IsA(UScriptStruct::StaticClass()))
                    {
                        GenerateStructProperties(file, reinterpret_cast<UScriptStruct*>(uObject), packageObject);
                    }
                }
            }
        }
    }
}

namespace ConstGenerator
{
    void GenerateConst(File& file, class UConst* uConst)
    {
        std::ostringstream constStream;
        std::ostringstream valueStream;

        static std::multimap<std::string, std::string> nameValueMap;
        std::string constName = Generator::GenerateValidName(uConst->GetName());

        if (constName.find("Default__") == std::string::npos)
        {
            std::string propertyTitle = "Const: ";
            Generator::LogFile.Write(propertyTitle);
            Generator::LogFile.Pad(' ', constName.size(), false);
            Generator::LogFile.Write(constName);
            Generator::LogFile.Pad(' ', 75 - (constName.size() + propertyTitle.size()), false);
            Generator::LogFile.Write(" - Instance: ");
            Generator::LogFile.Hex(reinterpret_cast<uintptr_t>(uConst), static_cast<int32_t>(Configuration::Alignment));
            Generator::LogFile.NewLine();

            std::string constValue = uConst->Value.ToString();
            size_t mapSize = nameValueMap.count(constName);

            if (mapSize == 0)
            {
                nameValueMap.insert(std::make_pair(constName, constValue));

                constStream << "#define CONST_";
                Printers::MakeSpacer(constStream, Configuration::ConstSpacer, constName, true);
                constStream << " " << constValue << "\n";

                file.Write(constStream);
            }
            else if (!Utils::MapExists(nameValueMap, constName, constValue))
            {
                nameValueMap.insert(std::make_pair(constName, constValue));

                valueStream << constName;
                Printers::MakeDecimal(valueStream, mapSize, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));

                constStream << "#define CONST_";
                Printers::MakeSpacer(constStream, valueStream, Configuration::ConstSpacer);
                constStream << " " << constValue << "\n";

                file.Write(constStream);
                Printers::EmptyStream(valueStream);
            }   
        }
    }

    void ProcessConsts(File& file, class UObject* uPackageObj)
    {
        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject)
            {
                UObject* packageObject = uObject->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == uPackageObj && uObject->IsA(UConst::StaticClass()))
                    {
                        GenerateConst(file, reinterpret_cast<UConst*>(uObject));
                    }
                }
            }
        }
    }
}

namespace EnumGenerator
{
    void GenerateEnum(File& file, class UEnum* uEnum)
    {
        std::ostringstream enumStream;
        std::ostringstream propertyStream;

        std::string enumFullName = uEnum->GetFullName();
        std::string enumName = Generator::GenerateValidName(uEnum->GetName());

        if (enumName.find("Default__") == std::string::npos)
        {
            std::string propertyTitle = "Enum: ";
            Generator::LogFile.Write(propertyTitle);
            Generator::LogFile.Pad(' ', enumName.size(), false);
            Generator::LogFile.Write(enumName);
            Generator::LogFile.Pad(' ', 75 - (enumName.size() + propertyTitle.size()), false);
            Generator::LogFile.Write(" - Instance: ");
            Generator::LogFile.Hex(reinterpret_cast<uintptr_t>(uEnum), static_cast<int32_t>(Configuration::Alignment));
            Generator::LogFile.NewLine();

            uint32_t enumCount = UObject::CountObject<UEnum>(uEnum->GetName());

            if (enumCount > 1)
            {
                enumName = Generator::GenerateValidName(uEnum->Outer->GetName()) + "_" + enumName;
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
                std::string propertyName = Generator::GenerateValidName(uEnum->Names[i].ToString());

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
                    propertyStream << propertyName;
                    Printers::MakeDecimal(propertyStream, propertiesMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                    propertiesMap[propertyName]++;
                }

                enumStream << "\t";
                Printers::MakeSpacer(enumStream, propertyStream, Configuration::EnumSpacer);
                enumStream << " = " << i;

                if (i != uEnum->Names.Num() - 1)
                {
                    enumStream << "," << "\n";
                }
                else
                {
                    enumStream << "\n";
                }

                Printers::EmptyStream(propertyStream);
            }

            enumStream << "};\n\n";

            file.Write(enumStream);
        }
    }

    void ProcessEnums(File& file, class UObject* uPackageObj)
    {
        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject)
            {
                UObject* packageObject = uObject->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == uPackageObj && uObject->IsA(UEnum::StaticClass()))
                    {
                        GenerateEnum(file, reinterpret_cast<UEnum*>(uObject));
                    }
                }
            }
        }
    }
}

namespace ClassGenerator
{
    void GenerateFields(std::ostringstream& classStream, class UClass* uClass, EClassTypes classType)
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

                for (const auto& field : fields)
                {
                    if ((lastOffset + field.second.Size) < field.second.Offset)
                    {
                        missedOffset = field.second.Offset - lastOffset;

                        if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                        {
                            propertyStream << "\tuint8_t UnknownData";
                            Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                            propertyStream << "[";
                            Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                            propertyStream << "];";

                            classStream << propertyStream.str() << " // ";
                            Printers::MakeHex(classStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                            classStream << " (";
                            Printers::MakeHex(classStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                            classStream << ") DYNAMIC FIELD PADDING\n";

                            Printers::EmptyStream(propertyStream);

                            unknownDataIndex++;
                        }
                    }

                    classStream << "\t" << field.second.Type << " // ";
                    Printers::MakeHex(classStream, field.second.Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << " (";
                    Printers::MakeHex(classStream, field.second.Size, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << ")\n";

                    lastOffset = field.second.Offset + field.second.Size;
                }

                if (lastOffset < uClass->PropertySize)
                {
                    missedOffset = uClass->PropertySize - lastOffset;

                    if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                    {
                        propertyStream << "\tuint8_t UnknownData";
                        Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                        propertyStream << "];";

                        classStream << propertyStream.str() << " // ";
                        Printers::MakeHex(classStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (";
                        Printers::MakeHex(classStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << ") DYNAMIC FIELD PADDING\n";
                    }
                }
            }
            else
            {
                Generator::LogFile.WriteLine("Error: INCORRECT CLASS SIZE DETECTED FOR ECLASSTYPE " + std::to_string(static_cast<int32_t>(classType)));
                Generator::LogFile.WriteLine("Error: ECLASSTYPE LOCAL SIZE: " + std::to_string(localSize));
                Generator::LogFile.WriteLine("Error: ECLASSTYPE ACTUAL SIZE: " + std::to_string(uClass->PropertySize));
                MessageBox(NULL, (LPCWSTR)L"Error: Incorrect class size detected, check the log for more details!", (LPCWSTR)L"UE3SDKGenerator", MB_ICONERROR | MB_OK);
            }
        }
        else
        {
            Generator::LogFile.WriteLine("Error: NO FIELDS FOUND FOR ECLASSTYPE " + std::to_string(static_cast<int32_t>(classType)));
            MessageBox(NULL, (LPCWSTR)L"Error: No fields found, check the log for more details!", (LPCWSTR)L"UE3SDKGenerator", MB_ICONERROR | MB_OK);
        }
    }

    void GenerateClass(File& file, class UClass* uClass)
    {
        std::ostringstream classStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        int32_t size = 0;
        int32_t lastOffset = 0;
        int32_t missedOffset = 0;

        UClass* uSuperClass = reinterpret_cast<UClass*>(uClass->SuperField);

        std::string className = Generator::GenerateValidName(uClass->GetName());
        std::string classNameCPP = Generator::GenerateValidName(uClass->GetNameCPP());
        std::string classFullName = uClass->GetFullName();

        std::string propertyTitle = "Class: ";
        Generator::LogFile.Write(propertyTitle);
        Generator::LogFile.Pad(' ', className.size(), false);
        Generator::LogFile.Write(className);
        Generator::LogFile.Pad(' ', 75 - (className.size() + propertyTitle.size()), false);
        Generator::LogFile.Write(" - Instance: ");
        Generator::LogFile.Hex(reinterpret_cast<uintptr_t>(uClass), static_cast<int32_t>(Configuration::Alignment));
        Generator::LogFile.NewLine();

        std::vector<UProperty*> vProperty;

        for (UProperty* uProperty = reinterpret_cast<UProperty*>(uClass->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
        {
            if (!uProperty->IsA(UFunction::StaticClass())
                && !uProperty->IsA(UConst::StaticClass())
                && !uProperty->IsA(UEnum::StaticClass())
                && !uProperty->IsA(UScriptStruct::StaticClass())
                && uProperty->ElementSize > 0
                && (!uSuperClass || (uSuperClass && uSuperClass != uClass && uProperty->Offset >= uSuperClass->PropertySize)))
            {
                vProperty.push_back(uProperty);
            }
        }

        std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

        classStream << "// " << classFullName << "\n";

        if (uSuperClass && uSuperClass != uClass)
        {
            size = uClass->PropertySize - uSuperClass->PropertySize;
            lastOffset = uSuperClass->PropertySize;

            std::string superClassNameCPP = Generator::GenerateValidName(uSuperClass->GetNameCPP());

            classStream << "// ";
            Printers::MakeHex(classStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            classStream << " (";
            Printers::MakeHex(classStream, uSuperClass->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            classStream << " - ";
            Printers::MakeHex(classStream, uClass->PropertySize, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            classStream << ")\n" << "class " << classNameCPP << " : public " << superClassNameCPP;
        }
        else
        {
            size = uClass->PropertySize;

            classStream << "// ";
            Printers::MakeHex(classStream, size, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
            classStream << "\n" << "class " << classNameCPP;
        }

        classStream << "\n{\npublic:\n";

        if (uClass == UField::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UFIELD); }
        else if (uClass == UEnum::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UENUM); }
        else if (uClass == UConst::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UCONST); }
        else if (uClass == UProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UPROPERTY); }
        else if (uClass == UStruct::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_USTRUCT); }
        else if (uClass == UFunction::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UFUNCTION); }
        else if (uClass == UStructProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_USTRUCTPROPERTY); }
        else if (uClass == UObjectProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UOBJECTPROPERTY); }
        else if (uClass == UMapProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UMAPPROPERTY); }
        else if (uClass == UInterfaceProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UINTERFACEPROPERTY); }
        else if (uClass == UDelegateProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UDELEGATEPROPERTY); }
        else if (uClass == UByteProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UBYTEPROPERTY); }
        else if (uClass == UBoolProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UBOOLPROPERTY); }
        else if (uClass == UArrayProperty::StaticClass()) { GenerateFields(classStream, uClass, EClassTypes::CLASS_UARRAYPROPERTY); }
        else
        {
            std::map<std::string, uint32_t> propertyNameMap;
            uint32_t unknownDataIndex = 0;

            for (size_t i = 0; i < vProperty.size(); i++)
            {
                UProperty* uProperty = vProperty[i];

                std::string propertyName = Generator::GenerateValidName(uProperty->GetName());

                if (uClass == UObject::StaticClass())
                {
                    if (!Configuration::UsingDetours && propertyName.find("VfTable") != std::string::npos)
                    {
                        lastOffset = uProperty->Offset + (uProperty->ElementSize * uProperty->ArrayDim);
                        continue;
                    }
                }

                if (lastOffset < uProperty->Offset)
                {
                    missedOffset = uProperty->Offset - lastOffset;

                    if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                    {
                        propertyStream << "UnknownData";
                        Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                        propertyStream << "];";

                        classStream << "\t";
                        Printers::MakeSpacer(classStream, Configuration::ClassSpacer, "uint8_t", true);
                        classStream << " ";
                        Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer - 3);
                        classStream << "// ";
                        Printers::MakeHex(classStream, lastOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (";
                        Printers::MakeHex(classStream, missedOffset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << ") MISSED OFFSET\n";

                        Printers::EmptyStream(propertyStream);

                        unknownDataIndex++;
                    }
                }

                std::string propertyType;

                if (Retrievers::GetPropertyType(uProperty, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    size_t correctElementSize = Retrievers::GetPropertySize(uProperty);

                    if (propertyNameMap.count(propertyName) == 0)
                    {
                        propertyNameMap[propertyName] = 1;
                        propertyStream << propertyName;
                    }
                    else
                    {
                        propertyStream << propertyName;
                        Printers::MakeDecimal(propertyStream, propertyNameMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                        propertyNameMap[propertyName]++;
                    }

                    if (uProperty->ArrayDim > 1)
                    {
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, uProperty->ArrayDim, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                        propertyStream << "]";
                    }

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                    {
                        propertyStream << " : 1";
                    }

                    Retrievers::GetAllPropertyFlags(flagStream, uProperty->PropertyFlags);

                    int32_t offsetError = (uProperty->ElementSize * uProperty->ArrayDim) - (correctElementSize * uProperty->ArrayDim);

                    classStream << "\t";

                    Printers::MakeSpacer(classStream, Configuration::ClassSpacer, propertyType, true);
                    classStream << " " << propertyStream.str() << ";";
                    Printers::MakeSpacer(classStream, Configuration::ClassSpacer - (propertyStream.str().size() + 1), "// ", false);

                    Printers::MakeHex(classStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << " (";
                    Printers::MakeHex(classStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << ") [";
                    Printers::MakeHex(classStream, uProperty->PropertyFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_PROPERTY));
                    classStream << "] ";

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                    {
                        classStream << "[";
                        Printers::MakeHex(classStream, reinterpret_cast<UBoolProperty*>(uProperty)->BitMask, static_cast<uint32_t>(EWidthTypes::WIDTH_BITMASK));
                        classStream << "] ";
                    }
                    else
                    {
                        Printers::MakeSpacer(classStream, 14);
                    }

                    classStream << flagStream.str() << "\n";

                    if (offsetError > 0)
                    {
                        propertyStream << "_UnknownData";
                        Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                        propertyStream << "];";

                        classStream << "\t";
                        Printers::MakeSpacer(classStream, Configuration::ClassSpacer, "uint8_t", true);
                        classStream << " ";
                        Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer - 3);
                        classStream << "// ";
                        Printers::MakeHex(classStream, (uProperty->Offset + offsetError), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << " (";
                        Printers::MakeHex(classStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << ") FIX WRONG SIZE OF PREVIOUS PROPERTY [Original: ";
                        Printers::MakeHex(classStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << ", Missing: ";
                        Printers::MakeHex(classStream, offsetError, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        classStream << "]\n";

                        Printers::EmptyStream(propertyStream);

                        unknownDataIndex++;
                    }

                    Printers::EmptyStream(propertyStream);
                    Printers::EmptyStream(flagStream);
                }
                else
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                    propertyStream << "];";

                    classStream << "\t";
                    Printers::MakeSpacer(classStream, Configuration::ClassSpacer, "uint8_t", true);
                    classStream << " ";
                    Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer - 3);
                    classStream << "// ";
                    Printers::MakeHex(classStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << " (";
                    Printers::MakeHex(classStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                    classStream << ") UNKNOWN PROPERTY: " << uProperty->GetFullName() << "\n";

                    Printers::EmptyStream(propertyStream);

                    unknownDataIndex++;
                }

                lastOffset = uProperty->Offset + (uProperty->ElementSize * uProperty->ArrayDim);
            }

            if (lastOffset < uClass->PropertySize)
            {
                missedOffset = uClass->PropertySize - lastOffset;

                if (missedOffset >= static_cast<int32_t>(Configuration::Alignment))
                {
                    propertyStream << "UnknownData";
                    Printers::MakeDecimal(propertyStream, unknownDataIndex, static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                    propertyStream << "[";
                    Printers::MakeHex(propertyStream, missedOffset, 0);
                    propertyStream << "];";

                    classStream << "\t";
                    Printers::MakeSpacer(classStream, Configuration::ClassSpacer, "uint8_t", true);
                    classStream << " ";
                    Printers::MakeSpacer(classStream, propertyStream, Configuration::ClassSpacer - 3);
                    classStream << "// ";
                    Printers::MakeHex(classStream, lastOffset, 4);
                    classStream << " (";
                    Printers::MakeHex(classStream, missedOffset, 4);
                    classStream << ") MISSED OFFSET\n";

                    Printers::EmptyStream(propertyStream);
                }
            }
        }

        classStream << "public:\n";

        if (uClass == UObject::StaticClass())
        {
            classStream << PiecesOfCode::UObject_FunctionDescriptions;
        }

        if (Configuration::UsingConstants)
        {
            classStream << "\tstatic UClass* StaticClass()\n"
                << "\t{\n"
                << "\t\tstatic UClass* uClassPointer = nullptr;\n\n"
                << "\t\tif (!uClassPointer)\n"
                << "\t\t{\n"
                << "\t\t\tuClassPointer = reinterpret_cast<UClass*>(UObject::GObjObjects()->At(" << Generator::GenerateValidName(Generator::GenerateIndexName(uClass, true)) << "));\n"
                << "\t\t}\n\n"
                << "\t\treturn uClassPointer;\n"
                << "\t};\n\n";
        }
        else
        {
            classStream << "\tstatic UClass* StaticClass()\n"
                << "\t{\n"
                << "\t\tstatic UClass* uClassPointer = nullptr;\n\n"
                << "\t\tif (!uClassPointer)\n"
                << "\t\t{\n"
                << "\t\t\tuClassPointer = UObject::FindClass(\"" << classFullName << "\");\n"
                << "\t\t}\n\n"
                << "\t\treturn uClassPointer;\n"
                << "\t};\n\n";
        }

        file.Write(classStream);

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

        file.Write(classStream);
    }

    void GenerateClassProperties(File& file, class UClass* uClass, class UObject* uPackageObj)
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

            std::string className = Generator::GenerateValidName(uClass->GetName());
            std::string classFullName = uClass->GetFullName();

            if (className.find("Default__") == std::string::npos)
            {
                if (classNames.find(classFullName) == classNames.end())
                {
                    if (uClass->SuperField && uClass->SuperField != uClass)
                    {
                        if (classNames.find(uClass->SuperField->GetFullName()) == classNames.end())
                        {
                            GenerateClassProperties(file, reinterpret_cast<UClass*>(uClass->SuperField), uPackageObj);
                        }
                    }

                    GenerateClass(file, uClass);
                    classNames.insert(std::make_pair(classFullName, uClass->ObjectInternalInteger));
                }   
            }
        }
    }

    void ProcessClasses(File& file, class UObject* uPackageObj)
    {
        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject)
            {
                UObject* packageObject = uObject->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == uPackageObj && uObject->IsA(UClass::StaticClass()))
                    {
                        GenerateClassProperties(file, reinterpret_cast<UClass*>(uObject), packageObject);
                    }
                }
            }
        }
    }
}

namespace ParameterGenerator
{
    void GenerateParameter(File& file, class UClass* uClass)
    {
        std::ostringstream parameterStream;
        std::ostringstream propertyStream;
        std::ostringstream flagStream;

        std::vector<UFunction*> vFunction;

        for (UProperty* uProperty = reinterpret_cast<UProperty*>(uClass->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
        {
            if (uProperty->IsA(UFunction::StaticClass()))
            {
                vFunction.push_back(reinterpret_cast<UFunction*>(uProperty));
            }
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* uFunction = vFunction[i];

            std::string functionFullName = uFunction->GetFullName();
            std::string functionName = Generator::GenerateValidName(uFunction->GetName());
            std::string classNameCPP = Generator::GenerateValidName(uClass->GetNameCPP());

            parameterStream << "// " << functionFullName << "\n";
            parameterStream << "// [";
            Printers::MakeHex(parameterStream, uFunction->FunctionFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_FUNCTION));
            parameterStream << "] ";

            if ((uFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && uFunction->iNative)
            {
                parameterStream << " iNative[";
                Printers::MakeHex(parameterStream, uFunction->iNative, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                parameterStream << "]";
            }

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { propertyStream << "exec"; }
            else if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { propertyStream << "event"; }
            else { propertyStream << "exec"; }

            Generator::MakeWindowsFunction(functionName);

            parameterStream << "\nstruct " << classNameCPP << "_" << propertyStream.str() << functionName << "_Params\n" << "{\n";

            Printers::EmptyStream(propertyStream);

            std::vector<UProperty*> vProperty;

            for (UProperty* uProperty = reinterpret_cast<UProperty*>(uFunction->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
            {
                if (uProperty->ElementSize > 0)
                {
                    vProperty.push_back(uProperty);
                }
            }

            std::sort(vProperty.begin(), vProperty.end(), Utils::SortProperty);

            std::map<std::string, uint32_t> propertyNameMap;

            for (size_t i = 0; i < vProperty.size(); i++)
            {
                UProperty* uProperty = vProperty[i];

                std::string propertyType;

                if (Retrievers::GetPropertyType(uProperty, propertyType, true) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    std::string propertyName = Generator::GenerateValidName(uProperty->GetName());

                    if (propertyNameMap.count(propertyName) == 0)
                    {
                        propertyNameMap[propertyName] = 1;
                        propertyStream << propertyName;
                    }
                    else
                    {
                        propertyStream << propertyName;
                        Printers::MakeDecimal(propertyStream, propertyNameMap[propertyName], static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                        propertyNameMap[propertyName]++;
                    }

                    if (uProperty->ArrayDim > 1)
                    {
                        propertyStream << "[";
                        Printers::MakeHex(propertyStream, uProperty->ArrayDim, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                        propertyStream << "]";
                    }

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                    {
                        propertyStream << " : 1";
                    }

                    propertyStream << ";";

                    Retrievers::GetAllPropertyFlags(flagStream, uProperty->PropertyFlags);

                    if (uProperty->PropertyFlags & EPropertyFlags::CPF_Parm)
                    {
                        parameterStream << "\t";
                        Printers::MakeSpacer(parameterStream, Configuration::FunctionSpacer, propertyType, true);
                        parameterStream << " ";
                        Printers::MakeSpacer(parameterStream, propertyStream, Configuration::FunctionSpacer);
                        parameterStream << "\t\t// ";
                        Printers::MakeHex(parameterStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        parameterStream << " (";
                        Printers::MakeHex(parameterStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        parameterStream << ") [";
                        Printers::MakeHex(parameterStream, uProperty->PropertyFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_PROPERTY));
                        parameterStream << "] ";
                    }
                    else
                    {
                        parameterStream << "\t// ";
                        Printers::MakeSpacer(parameterStream, Configuration::FunctionSpacer, propertyType, true);
                        parameterStream << " ";
                        Printers::MakeSpacer(parameterStream, propertyStream, Configuration::FunctionSpacer);
                        parameterStream << "\t\t// ";
                        Printers::MakeHex(parameterStream, uProperty->Offset, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        parameterStream << " (";
                        Printers::MakeHex(parameterStream, (uProperty->ElementSize * uProperty->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                        parameterStream << ") [";
                        Printers::MakeHex(parameterStream, uProperty->PropertyFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_PROPERTY));
                        parameterStream << "] ";
                    }

                    if (uProperty->IsA(UBoolProperty::StaticClass()))
                    {
                        parameterStream << "[";
                        Printers::MakeHex(parameterStream, reinterpret_cast<UBoolProperty*>(uProperty)->BitMask, static_cast<uint32_t>(EWidthTypes::WIDTH_BITMASK));
                        parameterStream << "] ";
                    }
                    else
                    {
                        Printers::MakeSpacer(parameterStream, 14);
                    }

                    parameterStream << flagStream.str() << "\n";

                    Printers::EmptyStream(propertyStream);
                    Printers::EmptyStream(flagStream);
                }
                else
                {
                    std::string sPropertyFullName = uProperty->GetFullName();

                    parameterStream << "\t// UNKNOWN PROPERTY: " << sPropertyFullName << "\n";
                }
            }

            parameterStream << "};\n\n";
        }

        file.Write(parameterStream);
    }

    void ProcessParameters(File& file, UObject* uPackageObj)
    {
        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject)
            {
                UObject* packageObject = uObject->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == uPackageObj && uObject->IsA(UClass::StaticClass()))
                    {
                        GenerateParameter(file, reinterpret_cast<UClass*>(uObject));
                    }
                }
            }
        }
    }
}

namespace FunctionGenerator
{
    void GenerateVirtualFunctions(File& file)
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
            file.NewLine();
            file.WriteLine("\t// Virtual Functions\n");

            for (int32_t index = 0; index < 256; index++)
            {
                uintptr_t virtualFunction = reinterpret_cast<uintptr_t*>(UObject::StaticClass()->VfTableObject.Dummy)[index];

                if (virtualFunction == processEventAddress)
                {
                    file.Write("\tvirtual void ProcessEvent(class UFunction* uFunction, void* uParams, void* uResult = nullptr);\t\t\t// ");
                    file.Hex(virtualFunction, static_cast<int32_t>(Configuration::Alignment));
                    file.NewLine();
                    break;
                }
                else
                {
                    file.Write("\tvirtual void VirtualFunction" + std::to_string(index) + "();\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t// ");
                    file.Hex(virtualFunction, static_cast<int32_t>(Configuration::Alignment));
                    file.NewLine();
                }
            }
        }
        else
        {
            file.NewLine();
            file.Write("\t// FIX PROCESS EVENT IN CONFIGURATION.CPP");
        }
    }

    void GenerateFunctionCode(File& file, class UClass* uClass)
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
                codeStream << "template<typename T>\n"
                << "T GetVirtualFunction(const void* instance, std::size_t index)\n"
                << "{\n"
                << "\tauto vtable = *static_cast<const void***>(const_cast<void*>(instance));\n"
                << "\treturn reinterpret_cast<T>(vtable[index]);\n"
                << "}\n\n";

                codeStream << "void UObject::ProcessEvent(class UFunction* uFunction, void* uParams, void* uResult = nullptr)\n"
                << "{\n"
                << "\tGetVirtualFunction<void(*)(class UObject*, class UFunction*, void*)>(this, " << Configuration::ProcessEventIndex << ")(this, uFunction, uParams);\n"
                << "}\n\n";
            }

            objectFunctions = true;
        }

        if (!functionFunctions && uClass == UFunction::StaticClass())
        {
            codeStream << PiecesOfCode::UFunction_Functions;

            functionFunctions = true;
        }

        for (UProperty* uProperty = reinterpret_cast<UProperty*>(uClass->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
        {
            if (uProperty->IsA(UFunction::StaticClass()))
            {
                vFunction.push_back(reinterpret_cast<UFunction*>(uProperty));
            }
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* uFunction = vFunction[i];

            std::string functionFullName = uFunction->GetFullName();
            std::string functionName = Generator::GenerateValidName(uFunction->GetName());
            std::string classNameCPP = Generator::GenerateValidName(uClass->GetNameCPP());

            Retrievers::GetAllFunctionFlags(functionStream, uFunction->FunctionFlags);
            codeStream << "// " << functionFullName << "\n" << "// [";
            Printers::MakeHex(codeStream, uFunction->FunctionFlags, static_cast<uint32_t>(EWidthTypes::WIDTH_FUNCTION));
            codeStream << "] "<< functionStream.str();
            Printers::EmptyStream(functionStream);

            if ((uFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && uFunction->iNative)
            {
                codeStream << " iNative [";
                Printers::MakeHex(codeStream, uFunction->iNative, static_cast<uint32_t>(EWidthTypes::WIDTH_SIZE));
                codeStream << "]";
            }

            std::vector<std::pair<UProperty*, std::string>> propertyParams;
            std::vector<std::pair<UProperty*, std::string>> propertyOutParams;
            std::vector<std::pair<UProperty*, std::string>> propertySpecialParams;
            std::pair<UProperty*, std::string> propertyReturnParm;

            std::map<std::string, uint32_t> propertyNameMap;

            for (UProperty* uProperty = reinterpret_cast<UProperty*>(uFunction->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
            {
                if (uProperty->ElementSize == 0)
                {
                    continue;
                }

                std::string propertyNameBuffer = Generator::GenerateValidName(uProperty->GetName());
                std::string propertyNameUnique;

                if (propertyNameMap.count(propertyNameBuffer) == 0)
                {
                    propertyNameMap[propertyNameBuffer] = 1;
                    propertyNameUnique = propertyNameBuffer;
                }
                else
                {
                    functionStream << propertyNameBuffer;
                    Printers::MakeDecimal(functionStream, propertyNameMap[propertyNameBuffer], static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));
                    propertyNameUnique = functionStream.str();
                    Printers::EmptyStream(functionStream);
                    propertyNameMap[propertyNameBuffer]++;
                }

                if (uProperty->PropertyFlags & EPropertyFlags::CPF_ReturnParm)
                {
                    propertyReturnParm = make_pair(uProperty, propertyNameUnique);
                }
                else if ((uProperty->PropertyFlags & EPropertyFlags::CPF_Parm) && (uProperty->PropertyFlags & EPropertyFlags::CPF_OutParm))
                {
                    propertyOutParams.push_back(make_pair(uProperty, propertyNameUnique));
                    propertySpecialParams.push_back(make_pair(uProperty, propertyNameUnique));
                }
                else if (uProperty->PropertyFlags & EPropertyFlags::CPF_Parm)
                {
                    propertyParams.push_back(make_pair(uProperty, propertyNameUnique));
                }
            }

            sort(propertyParams.begin(), propertyParams.end(), Utils::SortPropertyPair);
            sort(propertyOutParams.begin(), propertyOutParams.end(), Utils::SortPropertyPair);
            sort(propertySpecialParams.begin(), propertySpecialParams.end(), Utils::SortPropertyPair);

            codeStream << "\n// Parameter info:\n";

            std::string propertyType;

            if (propertyReturnParm.first && Retrievers::GetPropertyType(propertyReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UNKNOWN)
            {
                Retrievers::GetAllPropertyFlags(functionStream, propertyReturnParm.first->PropertyFlags);
                codeStream << "// ";
                Printers::MakeSpacer(codeStream, Configuration::CommentSpacer, propertyType, true);
                codeStream << " ";
                Printers::MakeSpacer(codeStream, Configuration::CommentSpacer, propertyReturnParm.second, true);
                codeStream << " " << functionStream.str() << "\n";
                Printers::EmptyStream(functionStream);
            }

            for (size_t i = 0; i < propertyParams.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(propertyParams[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    Retrievers::GetAllPropertyFlags(functionStream, uProperty.first->PropertyFlags);
                    codeStream << "// ";
                    Printers::MakeSpacer(codeStream, Configuration::CommentSpacer, propertyType, true);
                    codeStream << " ";
                    Printers::MakeSpacer(codeStream, Configuration::CommentSpacer, uProperty.second, true);
                    codeStream << " " << functionStream.str() << "\n";
                    Printers::EmptyStream(functionStream);
                }
            }

            for (size_t i = 0; i < propertyOutParams.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(propertyOutParams[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    Retrievers::GetAllPropertyFlags(functionStream, uProperty.first->PropertyFlags);
                    codeStream << "// ";
                    Printers::MakeSpacer(codeStream, Configuration::CommentSpacer, propertyType, true);
                    codeStream << " ";
                    Printers::MakeSpacer(codeStream, Configuration::CommentSpacer, uProperty.second, true);
                    codeStream << " " << functionStream.str() << "\n";
                    Printers::EmptyStream(functionStream);
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

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { codeStream << " " << classNameCPP << "::" << functionName << "("; }
            else if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { codeStream << " " << classNameCPP << "::event" << functionName << "("; }
            else { codeStream << " " << classNameCPP << "::" << functionName << "("; }

            bool printComma = false;

            for (size_t i = 0; i < propertyParams.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(propertyParams[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (printComma)
                    {
                        codeStream << ", ";
                    }

                    codeStream << propertyType << " " << uProperty.second;

                    printComma = true;
                }
            }

            for (size_t i = 0; i < propertyOutParams.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(propertyOutParams[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (uProperty.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                    {
                        if (printComma)
                        {
                            codeStream << ", ";
                        }

                        codeStream << propertyType << "& " << uProperty.second;

                        printComma = true;
                    }
                }
            }

            if (Configuration::UsingConstants)
            {
                codeStream << ")\n{\n"
                << "\tstatic UFunction* uFn" << functionName << " = nullptr;\n\n"
                << "\tif (!uFn" << functionName << ")\n"
                << "\t{\n"
                << "\t\tuFn" << functionName << " = reinterpret_cast<UFunction*>(UObject::GObjObjects()->At(" << Generator::GenerateValidName(Generator::GenerateIndexName(uFunction, true)) << "));\n"
                << "\t}\n\n"
                << "\t" << classNameCPP << "_";
            }
            else
            {
                codeStream << ")\n{\n"
                << "\tstatic UFunction* uFn" << functionName << " = nullptr;\n\n"
                << "\tif (!uFn" << functionName << ")\n"
                << "\t{\n"
                << "\t\tuFn" << functionName << " = UFunction::FindFunction(\"" << functionFullName << "\");\n"
                << "\t}\n\n"
                << "\t" << classNameCPP << "_";
            }

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { codeStream << "exec"; }
            else if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { codeStream << "event"; }
            else { codeStream << "exec"; }

            codeStream << functionName << "_Params " << functionName << "_Params;\n";

            for (size_t i = 0; i < propertyParams.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(propertyParams[i]);

                EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(uProperty.first, propertyType, false);

                if (uProperty.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                {
                    EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(uProperty.first, propertyType, false);

                    if (propertyTypeResult != EPropertyTypes::TYPE_UNKNOWN)
                    {
                        if (!Utils::IsBitField(propertyTypeResult) || !Utils::IsBitField(uProperty.first->ArrayDim))
                        {
                            codeStream << "\tmemcpy_s(&" << functionName << "_Params." << uProperty.second << ", ";
                            Printers::MakeHex(codeStream, (uProperty.first->ElementSize * uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ", &" << uProperty.second << ", ";
                            Printers::MakeHex(codeStream, (uProperty.first->ElementSize * uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ");\n";
                        }
                        else if (!Utils::IsStructProperty(propertyTypeResult))
                        {
                            codeStream << "\t" << functionName << "_Params." << uProperty.second << " = " << uProperty.second << ";\n";
                        }
                    }
                }
            }

            for (size_t i = 0; i < propertySpecialParams.size(); i++)
            {
                std::pair<UProperty*, std::string> uProperty(propertySpecialParams[i]);

                EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(uProperty.first, propertyType, false);

                if (uProperty.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                {
                    EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(uProperty.first, propertyType, false);

                    if (propertyTypeResult != EPropertyTypes::TYPE_UNKNOWN)
                    {
                        if (!Utils::IsBitField(propertyTypeResult) || !Utils::IsBitField(uProperty.first->ArrayDim))
                        {
                            codeStream << "\tmemcpy_s(&" << functionName << "_Params." << uProperty.second << ", ";
                            Printers::MakeHex(codeStream, (uProperty.first->ElementSize * uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ", &" << uProperty.second << ", ";
                            Printers::MakeHex(codeStream, (uProperty.first->ElementSize * uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                            codeStream << ");\n";
                        }
                        else if (!Utils::IsStructProperty(propertyTypeResult))
                        {
                            codeStream << "\t" << uProperty.second << " = " << functionName << "_Params." << uProperty.second << ";\n";
                        }
                    }
                }
            }

            if ((uFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && uFunction->iNative)
            {
                codeStream << "\n\tunsigned short iNative = uFn" << functionName << "->iNative;\n\tuFn" << functionName << "->iNative = 0;\n";
            }

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Native)
            {
                codeStream << "\n\tuFn" << functionName << "->FunctionFlags |= ~";
                Printers::MakeHex(codeStream, EFunctionFlags::FUNC_Native, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                codeStream << ";\n";
            }

            codeStream << "\n\tthis->ProcessEvent(uFn" << functionName << ", &" << functionName << "_Params, nullptr);\n";

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Native)
            {
                codeStream << "\n\tuFn" << functionName << "->FunctionFlags |= ";
                Printers::MakeHex(codeStream, EFunctionFlags::FUNC_Native, static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                codeStream << ";\n";
            }

            if ((uFunction->FunctionFlags & EFunctionFlags::FUNC_Native) && uFunction->iNative)
            {
                codeStream << "\n\tuFn" << functionName << "->iNative = iNative;\n";
            }

            if (propertyOutParams.size() > 0)
            {
                codeStream << "\n";

                for (size_t i = 0; i < propertyOutParams.size(); i++)
                {
                    std::pair<UProperty*, std::string> uProperty(propertyOutParams[i]);

                    if (uProperty.first->PropertyFlags & EPropertyFlags::CPF_OutParm)
                    {
                        EPropertyTypes propertyTypeResult = Retrievers::GetPropertyType(uProperty.first, propertyType, false);

                        if (propertyTypeResult != EPropertyTypes::TYPE_UNKNOWN)
                        {
                            if (!Utils::IsBitField(propertyTypeResult) || !Utils::IsBitField(uProperty.first->ArrayDim))
                            {
                                codeStream << "\tmemcpy_s(&" << uProperty.second << ", ";
                                Printers::MakeHex(codeStream, (uProperty.first->ElementSize * uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                                codeStream << ", &" << functionName << "_Params." << uProperty.second << ", ";
                                Printers::MakeHex(codeStream, (uProperty.first->ElementSize * uProperty.first->ArrayDim), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
                                codeStream << ");\n";
                            }
                            else if (!Utils::IsStructProperty(propertyTypeResult))
                            {
                                codeStream << "\t" << uProperty.second << " = " << functionName << "_Params." << uProperty.second << ";\n";
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

        file.Write(codeStream);
    }

    void GenerateFunctionDescription(File& file, class UClass* uClass)
    {
        std::ostringstream functionStream;
        std::ostringstream propertyStream;

        std::vector<UFunction*> vFunction;

        for (UProperty* uProperty = reinterpret_cast<UProperty*>(uClass->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
        {
            if (uProperty->IsA(UFunction::StaticClass()))
            {
                vFunction.push_back(reinterpret_cast<UFunction*>(uProperty));
            }
        }

        for (size_t i = 0; i < vFunction.size(); i++)
        {
            UFunction* uFunction = vFunction[i];

            std::string functionName = Generator::GenerateValidName(uFunction->GetName());

            std::vector<std::pair<UProperty*, std::string>> propertyParams;
            std::vector<std::pair<UProperty*, std::string>> propertyOutParams;
            std::pair<UProperty*, std::string> propertyReturnParm;

            std::map<std::string, uint32_t> propertyNameMap;

            for (UProperty* uProperty = reinterpret_cast<UProperty*>(uFunction->Children); uProperty; uProperty = reinterpret_cast<UProperty*>(uProperty->Next))
            {
                if (uProperty->ElementSize == 0)
                {
                    continue;
                }

                std::string propertyNameBuffer = Generator::GenerateValidName(uProperty->GetName());
                std::string propertyNameUnique;

                if (propertyNameMap.count(propertyNameBuffer) == 0)
                {
                    propertyNameMap[propertyNameBuffer] = 1;
                    propertyNameUnique = propertyNameBuffer;
                }
                else
                {
                    propertyStream << propertyNameBuffer;
                    Printers::MakeDecimal(propertyStream, propertyNameMap[propertyNameBuffer], static_cast<uint32_t>(EWidthTypes::WIDTH_BYTE));

                    propertyNameUnique = propertyStream.str();
                    Printers::EmptyStream(propertyStream);
                    propertyNameMap[propertyNameBuffer]++;
                }

                if (uProperty->PropertyFlags & EPropertyFlags::CPF_ReturnParm)
                {
                    propertyReturnParm = std::make_pair(uProperty, propertyNameUnique);
                }
                else if (uProperty->PropertyFlags & EPropertyFlags::CPF_OutParm)
                {
                    propertyOutParams.push_back(std::make_pair(uProperty, propertyNameUnique));
                }
                else if (uProperty->PropertyFlags & EPropertyFlags::CPF_Parm)
                {
                    propertyParams.push_back(std::make_pair(uProperty, propertyNameUnique));
                }
            }

            std::sort(propertyParams.begin(), propertyParams.end(), Utils::SortPropertyPair);
            std::sort(propertyOutParams.begin(), propertyOutParams.end(), Utils::SortPropertyPair);

            std::string propertyType;

            if (propertyReturnParm.first && Retrievers::GetPropertyType(propertyReturnParm.first, propertyType, true) != EPropertyTypes::TYPE_UNKNOWN)
            {
                functionStream << "\t" << propertyType;
            }
            else
            {
                functionStream << "\tvoid";
            }

            Generator::MakeWindowsFunction(functionName);

            if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Exec) { functionStream << " " << functionName << "("; }
            else if (uFunction->FunctionFlags & EFunctionFlags::FUNC_Event) { functionStream << " event" << functionName << "("; }
            else { functionStream << " " << functionName << "("; }

            bool printComma = false;

            for (size_t i = 0; i < propertyParams.size(); i++ )
            {
                std::pair<UProperty*, std::string> uProperty(propertyParams[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (printComma)
                    {
                        functionStream << ", ";
                    }

                    functionStream << propertyType << " " << uProperty.second;

                    printComma = true;
                }
            }

            for (size_t i = 0; i < propertyOutParams.size(); i++ )
            {
                std::pair<UProperty*, std::string> uProperty(propertyOutParams[i]);

                if (Retrievers::GetPropertyType(uProperty.first, propertyType, false) != EPropertyTypes::TYPE_UNKNOWN)
                {
                    if (uProperty.first->PropertyFlags & EPropertyFlags::CPF_Parm)
                    {
                        if (printComma)
                        {
                            functionStream << ", ";
                        }

                        functionStream << propertyType << "& " << uProperty.second;

                        printComma = true;
                    }
                }
            }

            functionStream << ");\n";
        }

        file.Write(functionStream);
    }

    void ProcessFunctions(File& file, class UObject* uPackageObj)
    {
        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject)
            {
                UObject* packageObject = uObject->GetPackageObj();

                if (packageObject)
                {
                    if (packageObject == uPackageObj && uObject->IsA(UClass::StaticClass()))
                    {
                        GenerateFunctionCode(file, reinterpret_cast<UClass*>(uObject));
                    }
                }
            }
        }
    }
}

namespace Generator
{
    File LogFile;
    std::vector<std::pair<std::string, int32_t>> vConstants{};
    std::vector<class UObject*> vIncludes{};

    std::string GenerateValidName(const std::string& invalidName)
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

    std::string GenerateUniqueName(class UClass* uClass)
    {
        std::ostringstream nameBuffer;

        UClass* superClass = reinterpret_cast<UClass*>(uClass->SuperField);
        std::string classNameCPP = GenerateValidName(uClass->GetNameCPP());

        for (size_t i = 0; i < classNameCPP.size(); i++)
        {
            classNameCPP[i] = std::toupper(classNameCPP[i]);
        }

        nameBuffer << classNameCPP;

        if (superClass)
        {
            std::string superClassNameCPP = GenerateValidName(superClass->GetNameCPP());

            for (size_t i = 0; i < superClassNameCPP.size(); i++)
            {
                superClassNameCPP[i] = std::toupper(superClassNameCPP[i]);
            }

            nameBuffer << "_" << superClassNameCPP;
        }

        return nameBuffer.str();
    }

    std::string GenerateUniqueName(class UFunction* uFunction, class UClass* uClass)
    {
        std::ostringstream nameBuffer;

        std::string functionName = GenerateValidName(uFunction->GetName());
        std::string classNameCPP = GenerateValidName(uClass->GetNameCPP());

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

    std::string GenerateIndexName(class UObject* uObject, bool pushBack)
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
            File file;
            file.Create(Configuration::GeneratorDirectory / Configuration::GameNameShort, "SdkConstants.hpp");

            file.WriteLine("#pragma once");

            for (UObject* uObject : *UObject::GObjObjects())
            {
                if (uObject)
                {
                    if (uObject->IsA(UFunction::StaticClass()) || uObject->IsA(UClass::StaticClass()))
                    {
                        std::string objectFullName = GenerateIndexName(uObject, false);
                        objectFullName = GenerateValidName(objectFullName);

                        std::pair<std::string, int> pConstant = std::make_pair(objectFullName, uObject->ObjectInternalInteger);

                        if (std::find(vConstants.begin(), vConstants.end(), pConstant) == vConstants.end())
                        {
                            vConstants.push_back(pConstant);
                        }
                    }
                }
            }

            for (size_t i = 0; i < vConstants.size(); i++)
            {
                file.Write("#define " + vConstants[i].first);
                file.Pad(' ', 150 - vConstants[i].first.size(), false);
                file.WriteLine(std::to_string(vConstants[i].second));
            }

            file.Close();
        }
    }

    void GenerateHeaders()
    {
        File file;
        file.Create(Configuration::GeneratorDirectory / Configuration::GameNameShort, "SdkHeaders.hpp");

        Printers::PrintHeader(file, "SdkHeaders", "hpp", false);
        file.WriteLine("#pragma once");

        Printers::PrintSection(file, "Includes");

        file.WriteLine("#include \"GameDefines.hpp\"");

        for (size_t i = 0; i < vIncludes.size(); i++)
        {
            file.WriteLine("#include \"SDK_HEADERS\\" + vIncludes[i]->GetName() + "_structs.hpp\"");
            file.WriteLine("#include \"SDK_HEADERS\\" + vIncludes[i]->GetName() + "_classes.hpp\"");
            file.WriteLine("#include \"SDK_HEADERS\\" + vIncludes[i]->GetName() + "_parameters.hpp\"");
        }

        file.NewLine();

        Printers::PrintFooter(file, false);

        file.Close();
    }

    void GenerateDefines()
    {
        File file;
        file.Create(Configuration::GeneratorDirectory / Configuration::GameNameShort, "GameDefines.hpp");

        Printers::PrintHeader(file, "GameDefines", "hpp", false);

        file.WriteLine("#pragma once");
        file.WriteLine("#include <algorithm>");
        file.WriteLine("#include <locale>");
        file.WriteLine("#include <stdlib.h>");
        file.WriteLine("#include <xlocale>");
        file.WriteLine("#include <ctype.h>");
        file.WriteLine("#include <chrono>");
        file.WriteLine("#include <thread>");
        file.WriteLine("#include <vector>");
        file.WriteLine("#include <map>");
        file.NewLine();

        if (Configuration::UsingOffsets)
        {
            file.WriteLine("// GObjects");
            file.Write("#define GObjects_Offset\t\t(uintptr_t)");
            file.Hex(Configuration::GObjectsOffset, sizeof(Configuration::GObjectsOffset));
            file.NewLine();

            file.WriteLine("// GNames");
            file.Write("#define GNames_Offset\t\t(uintptr_t)");
            file.Hex(Configuration::GNamesOffset, sizeof(Configuration::GNamesOffset));
            file.NewLine();
        }
        else
        {
            file.WriteLine("// GObjects");
            file.WriteLine("#define GObjects_Pattern\t\t(const uint8_t*)\"" + Configuration::GObjectsString + "\"");
            file.WriteLine("#define GObjects_Mask\t\t\t(const char*)\"" + std::string(Configuration::GObjectsMask) + "\"");

            file.WriteLine("// GNames");
            file.WriteLine("#define GNames_Pattern\t\t\t(const uint8_t*)\"" + Configuration::GNamesString + "\"");
            file.WriteLine("#define GNames_Mask\t\t\t\t(const char*)\"" + std::string(Configuration::GNamesMask) + "\"");

            file.WriteLine("// Process Event");
            file.WriteLine("#define ProcessEvent_Pattern\t(const uint8_t*)\"" + Configuration::ProcessEventString + "\"");
            file.WriteLine("#define ProcessEvent_Mask\t\t(const char*)\"" + std::string(Configuration::ProcessEventMask) + "\"");
        }

        Printers::PrintSection(file, "Defines");

        file.Write(PiecesOfCode::TArray_Iterator);
        file.Write(PiecesOfCode::TArray_Class);
        file.Write(PiecesOfCode::TMap_Class);

        Printers::PrintSection(file, "Globals");

        file.WriteLine("extern TArray<class UObject*>* GObjects;");
        file.WriteLine("extern TArray<struct FNameEntry*>* GNames;");

        Printers::PrintSection(file, "Structs");

#ifdef CHARACTER_UTF16
        file.WriteLine(PiecesOfCode::FNameEntry_UTF16);
        file.WriteLine(PiecesOfCode::FName_UTF16);
        file.WriteLine(PiecesOfCode::FString_UTF16);
#endif

#ifdef CHARACTER_UTF8
        file.WriteLine(PiecesOfCode::FNameEntry_UTF8);
        file.WriteLine(PiecesOfCode::FName_UTF8);
        file.WriteLine(PiecesOfCode::FString_UTF8);
#endif

        std::ostringstream stream;
        stream << "\tuint8_t UnknownData00[";
        Printers::MakeHex(stream, sizeof(FScriptDelegate), static_cast<uint32_t>(EWidthTypes::WIDTH_NONE));
        stream << "];";

        file.WriteLine("struct FScriptDelegate\n");
        file.WriteLine("{");
        file.WriteLine(stream.str());
        file.WriteLine("}");

        file.WriteLine(PiecesOfCode::FPointer_Struct);
        file.WriteLine(PiecesOfCode::FQWord_Struct);

        Printers::PrintFooter(file, false);

        file.Close();

        file.Create(Configuration::GeneratorDirectory / Configuration::GameNameShort, "GameDefines.cpp");

        Printers::PrintHeader(file, "GameDefines", "cpp", false);

        file.WriteLine("#include \"GameDefines.hpp\"");
       
        Printers::PrintSection(file, "Initialize Global Variables");

        file.WriteLine("TArray<class UObject*>* GObjects{};");
        file.Write("TArray<struct FNameEntry*>* GNames{};");

        file.Close();
    }

    void ProcessPackages()
    {
        std::vector<UObject*> vPackages;

        std::filesystem::path fullDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort / "SDK_HEADERS";

        for (UObject* uObject : *UObject::GObjObjects())
        {
            if (uObject)
            {
                if (uObject->IsA(UClass::StaticClass()))
                {
                    UObject* packageObject = uObject->GetPackageObj();

                    if (packageObject)
                    {
                        if (find(vPackages.begin(), vPackages.end(), packageObject) == vPackages.end())
                        {
                            Generator::LogFile.NewLine();
                            Generator::LogFile.Write("Processing Package: " + packageObject->GetName());
                            Generator::LogFile.NewLine();
                            Generator::LogFile.NewLine();

                            vPackages.push_back(packageObject);

                            File file;

                            // Structs
                            file.Create(fullDirectory, packageObject->GetName() + "_structs.hpp");

                            Printers::PrintHeader(file, packageObject->GetName() + "_structs", "hpp", true);

                            Printers::PrintSection(file, "Script Structs");
                            StructGenerator::ProcessStructs(file, packageObject);

                            Printers::PrintFooter(file, true);                
                            file.Close();

                            // Classes
                            file.Create(fullDirectory, packageObject->GetName() + "_classes.hpp");

                            Printers::PrintHeader(file, packageObject->GetName() + "_classes", "hpp", true);

                            Printers::PrintSection(file, "Constants");
                            ConstGenerator::ProcessConsts(file, packageObject);

                            Printers::PrintSection(file, "Enums");
                            EnumGenerator::ProcessEnums(file, packageObject);

                            Printers::PrintSection(file, "Classes");
                            ClassGenerator::ProcessClasses(file, packageObject);

                            Printers::PrintFooter(file, true);                 
                            file.Close();

                            // Parameters
                            file.Create(fullDirectory, packageObject->GetName() + "_parameters.hpp");

                            Printers::PrintHeader(file, packageObject->GetName() + "_parameters", "hpp", true);

                            Printers::PrintSection(file, "Function Parameters");
                            ParameterGenerator::ProcessParameters(file, packageObject);

                            Printers::PrintFooter(file, true);
                            file.Close();

                            // Functions
                            file.Create(fullDirectory, packageObject->GetName() + "_classes.cpp");

                            Printers::PrintHeader(file, packageObject->GetName() + "_classes", "cpp", true);

                            Printers::PrintSection(file, "Functions");
                            FunctionGenerator::ProcessFunctions(file, packageObject);

                            Printers::PrintFooter(file, true);
                            file.Close();
                        }   
                    }
                }
            }
        }
    }

    void GenerateSDK()
    {
        std::filesystem::path fullDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort;
        std::filesystem::path headerDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort / "SDK_HEADERS";

        std::filesystem::create_directory(Configuration::GeneratorDirectory);
        std::filesystem::create_directory(fullDirectory);
        std::filesystem::create_directory(headerDirectory);

        if (std::filesystem::exists(headerDirectory))
        {
            std::chrono::time_point startTime = std::chrono::system_clock::now();

            MessageBox(NULL, (LPCWSTR)L"SDK generation has started, do not close the game until prompted to do so!", (LPCWSTR)L"UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);
            Initialize(true);

            if (Utils::AreGObjectsValid() && Utils::AreGNamesValid())
            {
                ProcessPackages();
                GenerateHeaders();
                GenerateDefines();
                GenerateConstants();

                std::chrono::time_point endTime = std::chrono::system_clock::now();
                std::chrono::duration<float> elapsedTime = endTime - startTime;

                Generator::LogFile.NewLine();
                Generator::LogFile.Write(Configuration::GameNameShort +  " generated in  ");
                Generator::LogFile.Precision(4, elapsedTime.count());
                Generator::LogFile.Write(" seconds.");
                Generator::LogFile.Close();

                MessageBox(NULL, (LPCWSTR)L"Finished! It is now safe to close the game.", (LPCWSTR)L"UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);
            }
            else
            {
                Generator::LogFile.Close();
                MessageBox(NULL, (LPCWSTR)L"Failed to validate GObject & GNames, please make sure you have them configured properly! SDK generation has been canceled.", (LPCWSTR)L"UE3SDKGenerator", MB_ICONERROR | MB_OK);
            }
        }
        else
        {
            MessageBox(NULL, (LPCWSTR)L"Failed to create the desired directory, cannot generate an SDK at this time!", (LPCWSTR)L"UE3SDKGenerator", MB_ICONERROR | MB_OK);
        }
    }

    void DumpInstances(bool dumpGNames, bool dumpGObjects, bool dumpGObjectsFull)
    {
        Initialize(true);

        std::filesystem::path fullDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort;
        std::filesystem::create_directory(Configuration::GeneratorDirectory);
        std::filesystem::create_directory(fullDirectory);

        if (std::filesystem::exists(fullDirectory))
        {
            uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

            if (dumpGNames)
            {
                std::ofstream stim;

                stim.close();

                File file;
                file.Create(fullDirectory, "NameDump.txt");

                file.Write("Base: ");
                file.Hex(baseAddress, static_cast<int32_t>(Configuration::Alignment));
                file.NewLine();
                file.Write("GNames: ");
                file.Hex(reinterpret_cast<uintptr_t>(GNames), static_cast<int32_t>(Configuration::Alignment));
                file.NewLine();
                file.NewLine();

                for (auto& name : *FName::Names())
                {
                    if (name)
                    {
                        file.Write("Name[");
                        file.Pad('0', 6, false);
                        file.Write(std::to_string(name->GetIndex()));
                        file.Write("] " + name->ToString() + " ");
                        file.Pad(' ', 50 - name->ToString().length(), false);
                        file.Hex(reinterpret_cast<uintptr_t>(name), static_cast<int32_t>(Configuration::Alignment));
                        file.NewLine();
                    }
                }

                file.Close();
            }

            if (dumpGObjects)
            {
                File file;
                file.Create(fullDirectory, "ObjectDump.txt");

                file.Write("Base: ");
                file.Hex(baseAddress, static_cast<int32_t>(Configuration::Alignment));
                file.NewLine();
                file.Write("GObjects: ");
                file.Hex(reinterpret_cast<uintptr_t>(GObjects), static_cast<int32_t>(Configuration::Alignment));
                file.NewLine();
                file.NewLine();

                for (UObject* uObject : *UObject::GObjObjects())
                {
                    if (uObject)
                    {
                        std::string objectName = uObject->GetName();

                        file.Write("UObject[");
                        file.Pad('0', 6, false);
                        file.Write(std::to_string(uObject->ObjectInternalInteger));
                        file.Write("] " + objectName + " ");
                        file.Pad(' ', 50 - objectName.length(), false);
                        file.Hex(reinterpret_cast<uintptr_t>(uObject), static_cast<int32_t>(Configuration::Alignment));
                        file.NewLine();
                    }
                }

                file.Close();
            }

            if (dumpGObjectsFull)
            {
                File file;
                file.Create(fullDirectory, "FullObjectDump.txt");

                file.Write("Base: ");
                file.Hex(baseAddress, static_cast<int32_t>(Configuration::Alignment));
                file.NewLine();
                file.Write("GObjects: ");
                file.Hex(reinterpret_cast<uintptr_t>(GObjects), static_cast<int32_t>(Configuration::Alignment));
                file.NewLine();
                file.NewLine();

                for (UObject* uObject : *UObject::GObjObjects())
                {
                    if (uObject)
                    {
                        std::string objectFullName = uObject->GetFullName();

                        file.Write("UObject[");
                        file.Pad('0', 6, false);
                        file.Write(std::to_string(uObject->ObjectInternalInteger));
                        file.Write("] " + objectFullName + " ");
                        file.Pad(' ', 50 - objectFullName.length(), false);
                        file.Hex(reinterpret_cast<uintptr_t>(uObject), static_cast<int32_t>(Configuration::Alignment));
                        file.NewLine();
                    }
                }

                file.Close();
            }
        }
        else
        {
            MessageBox(NULL, (LPCWSTR)L"Failed to create the desired directory, cannot dump instances at this time!", (LPCWSTR)L"UE3SDKGenerator", MB_ICONERROR | MB_OK);
        }

        MessageBox(NULL, (LPCWSTR)L"Finished dumping instances!", (LPCWSTR)L"UE3SDKGenerator", MB_ICONINFORMATION | MB_OK);
    }

    void Initialize(bool createLogFile)
    {
        static bool initialized = false;
        uintptr_t baseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

        if (!initialized)
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
                uintptr_t GObjectsAddress = baseAddress + Configuration::GObjectsOffset;
                uintptr_t GNamesAddress = baseAddress + Configuration::GNamesOffset;
                GObjects = reinterpret_cast<TArray<UObject*>*>(GObjectsAddress);
                GNames = reinterpret_cast<TArray<FNameEntry*>*>(GNamesAddress);
            }

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
            //UDelegateProperty::RegFunction(); // Not actually needed in sdk generation at the moment.
            //UDelegateProperty::RegDelegateName(); // Not actually needed in sdk generation at the moment.
            UByteProperty::RegEnum();
            UBoolProperty::RegBitMask();
            UArrayProperty::RegInner();

            initialized = true;
        }

        if (createLogFile && initialized)
        {
            std::filesystem::path fullDirectory = Configuration::GeneratorDirectory / Configuration::GameNameShort;

            std::filesystem::create_directory(Configuration::GeneratorDirectory);
            std::filesystem::create_directory(fullDirectory);
            Generator::LogFile.Create(fullDirectory, "UE3SDKGenerator.log");

            Generator::LogFile.Write("Base: ");
            Generator::LogFile.Hex(baseAddress, static_cast<int32_t>(Configuration::Alignment));
            Generator::LogFile.NewLine();
            Generator::LogFile.Write("GObjects: ");
            Generator::LogFile.Hex(reinterpret_cast<uintptr_t>(GObjects), static_cast<int32_t>(Configuration::Alignment));
            Generator::LogFile.NewLine();
            Generator::LogFile.Write("GNames: ");
            Generator::LogFile.Hex(reinterpret_cast<uintptr_t>(GNames), static_cast<int32_t>(Configuration::Alignment));
            Generator::LogFile.NewLine();
        }
    }
}

void OnAttach(HMODULE hModule)
{
    DisableThreadLibraryCalls(hModule);
    //Generator::GenerateSDK();
    Generator::DumpInstances(true, false, true);
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