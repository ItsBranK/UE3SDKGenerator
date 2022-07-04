#include "GameDefines.hpp"

/*
# ========================================================================================= #
# Fields
# ========================================================================================= #
*/

ClassField::ClassField() :
	Id(EFieldIds::UNKNOWN),
	Type("uint8_t UNKNOWN_CLASS_FIELD[0x1];"),
	Offset(0x0),
	Size(1)
{

}

ClassField::ClassField(EFieldIds id, size_t sizeOverride) :
	Id(id),
	Type(Fields::IdToType[id]),
	Offset(Fields::GetOffset(id)),
	Size(sizeOverride)
{

}

ClassField::~ClassField() { }

ClassField ClassField::operator=(const ClassField& other)
{
	Id = other.Id;
	Type = other.Type;
	Offset = other.Offset;
	Size = other.Size;
	return *this;
}

namespace Fields
{
	std::vector<std::function<void()>> FieldMethods{};

	std::map<EFieldIds, std::string> IdToType =
	{
		{ EFieldIds::FNAMEENTRY_INDEX, "int32_t Index;" },
		{ EFieldIds::FNAMEENTRY_NAME_UTF16, "wchar_t Name[0x400];" },
		{ EFieldIds::FNAMEENTRY_NAME_UTF8, "char Name[0x400];" },
		{ EFieldIds::UOBJECT_VFTABLE, "struct FPointer VfTableObject;" },
		{ EFieldIds::UOBJECT_INDEX, "int32_t ObjectInternalInteger;" },
		{ EFieldIds::UOBJECT_OUTER, "class UObject* Outer;" },
		{ EFieldIds::UOBJECT_NAME, "struct FName Name;" },
		{ EFieldIds::UOBJECT_CLASS, "class UClass* Class;" },
		{ EFieldIds::UFIELD_NEXT, "class UField* Next;" },
		{ EFieldIds::UFIELD_SUPERFIELD, "class UField* SuperField;" },
		{ EFieldIds::UENUM_NAMES, "TArray<struct FName> Names;" },
		{ EFieldIds::UCONST_VALUE, "class FString Value;" },
		{ EFieldIds::UPROPERTY_DIMENSION, "unsigned long ArrayDim;" },
		{ EFieldIds::UPROPERTY_SIZE, "unsigned long ElementSize;" },
		{ EFieldIds::UPROPERTY_FLAGS, "uint64_t PropertyFlags;" },
		{ EFieldIds::UPROPERTY_OFFSET, "unsigned long Offset;" },
		{ EFieldIds::USTRUCT_SUPERFIELD, "class UField* SuperField;" },
		{ EFieldIds::USTRUCT_CHILDREN, "class UField* Children;" },
		{ EFieldIds::USTRUCT_SIZE, "unsigned long PropertySize;" },
		{ EFieldIds::UFUNCTION_FLAGS, "uint64_t FunctionFlags;" },
		{ EFieldIds::UFUNCTION_NATIVE, "uint16_t iNative;" },
		{ EFieldIds::USTRUCTPROPERTY_STRUCT, "class UStruct* Struct;" },
		{ EFieldIds::UOBJECTPROPERTY_PROPERTY, "class UClass* PropertyClass;" },
		{ EFieldIds::UCLASSPROPERTY_METACLASS, "class UClass* MetaClass;" },
		{ EFieldIds::UMAPPROPERTY_KEY, "class UProperty* Key;" },
		{ EFieldIds::UMAPPROPERTY_VALUE, "class UProperty* Value;" },
		{ EFieldIds::UINTERFACEPROPERTY_CLASS, "class UClass* InterfaceClass;" },
		{ EFieldIds::UDELEGATEPROPERTY_FUNCTION, "class UFuncton* Function;" },
		{ EFieldIds::UDELEGATEPROPERTY_NAME, "struct FName Name;" },
		{ EFieldIds::UBYTEPROPERTY_ENUM, "class UEnum* Enum;" },
		{ EFieldIds::UBOOLPROPERTY_BITMASK, "uint64_t BitMask;" },
		{ EFieldIds::UARRAYPROPERTY_INNTER, "class UProperty* Inner;" }
	};

	std::map<EFieldIds, ClassField> GlobalFields{};

	std::uintptr_t GetOffset(EFieldIds fieldId)
	{
		switch (fieldId)
		{
		case EFieldIds::FNAMEENTRY_INDEX:
			return offsetof(FNameEntry, Index);
		case EFieldIds::FNAMEENTRY_NAME_UTF16:
			return offsetof(FNameEntry, Name);
		case EFieldIds::FNAMEENTRY_NAME_UTF8:
			return offsetof(FNameEntry, Name);
		case EFieldIds::UOBJECT_VFTABLE:
			return offsetof(UObject, VfTableObject);
		case EFieldIds::UOBJECT_INDEX:
			return offsetof(UObject, ObjectInternalInteger);
		case EFieldIds::UOBJECT_OUTER:
			return offsetof(UObject, Outer);
		case EFieldIds::UOBJECT_NAME:
			return offsetof(UObject, Name);
		case EFieldIds::UOBJECT_CLASS:
			return offsetof(UObject, Class);
		case EFieldIds::UFIELD_NEXT:
			return offsetof(UField, Next);
		case EFieldIds::UFIELD_SUPERFIELD:
			return offsetof(UStruct, SuperField);
		case EFieldIds::UENUM_NAMES:
			return offsetof(UEnum, Names);
		case EFieldIds::UCONST_VALUE:
			return offsetof(UConst, Value);
		case EFieldIds::UPROPERTY_DIMENSION:
			return offsetof(UProperty, ArrayDim);
		case EFieldIds::UPROPERTY_SIZE:
			return offsetof(UProperty, ElementSize);
		case EFieldIds::UPROPERTY_FLAGS:
			return offsetof(UProperty, PropertyFlags);
		case EFieldIds::UPROPERTY_OFFSET:
			return offsetof(UProperty, Offset);
		case EFieldIds::USTRUCT_SUPERFIELD:
			return offsetof(UStruct, SuperField);
		case EFieldIds::USTRUCT_CHILDREN:
			return offsetof(UStruct, Children);
		case EFieldIds::USTRUCT_SIZE:
			return offsetof(UStruct, PropertySize);
		case EFieldIds::UFUNCTION_FLAGS:
			return offsetof(UFunction, FunctionFlags);
		case EFieldIds::UFUNCTION_NATIVE:
			return offsetof(UFunction, iNative);
		case EFieldIds::USTRUCTPROPERTY_STRUCT:
			return offsetof(UStructProperty, Struct);
		case EFieldIds::UOBJECTPROPERTY_PROPERTY:
			return offsetof(UObjectProperty, PropertyClass);
		case EFieldIds::UCLASSPROPERTY_METACLASS:
			return offsetof(UClassProperty, MetaClass);
		case EFieldIds::UMAPPROPERTY_KEY:
			return offsetof(UMapProperty, Key);
		case EFieldIds::UMAPPROPERTY_VALUE:
			return offsetof(UMapProperty, Value);
		case EFieldIds::UINTERFACEPROPERTY_CLASS:
			return offsetof(UInterfaceProperty, InterfaceClass);
		case EFieldIds::UDELEGATEPROPERTY_FUNCTION:
			return offsetof(UDelegateProperty, Function);
		case EFieldIds::UDELEGATEPROPERTY_NAME:
			return offsetof(UDelegateProperty, DelegateName);
		case EFieldIds::UBYTEPROPERTY_ENUM:
			return offsetof(UByteProperty, Enum);
		case EFieldIds::UBOOLPROPERTY_BITMASK:
			return offsetof(UBoolProperty, BitMask);
		case EFieldIds::UARRAYPROPERTY_INNTER:
			return offsetof(UArrayProperty, Inner);
		default:
			return 0x0;
		}
	}

	std::map<std::uintptr_t, ClassField> GetOrderedFields(EClassTypes classType, size_t& classSize, size_t& startOffset)
	{
		std::map<std::uintptr_t, ClassField> returnMap;

		switch (classType)
		{
		case EClassTypes::CLASS_FNAMEENTRY:
			returnMap.emplace(GlobalFields[EFieldIds::FNAMEENTRY_INDEX].Offset, GlobalFields[EFieldIds::FNAMEENTRY_INDEX]);

#ifdef CHARACTER_UTF16
			returnMap.emplace(GlobalFields[EFieldIds::FNAMEENTRY_NAME_UTF16].Offset, GlobalFields[EFieldIds::FNAMEENTRY_NAME_UTF16]);
#endif
#ifdef CHARACTER_UTF8
			returnMap.emplace(GlobalFields[EFieldIds::FNAMEENTRY_NAME_UTF8].Offset, GlobalFields[EFieldIds::FNAMEENTRY_NAME_UTF8]);
#endif
			classSize = sizeof(FNameEntry);
			startOffset = 0;
			break;
		case EClassTypes::CLASS_UOBJECT:
			returnMap.emplace(GlobalFields[EFieldIds::UOBJECT_VFTABLE].Offset, GlobalFields[EFieldIds::UOBJECT_VFTABLE]);
			returnMap.emplace(GlobalFields[EFieldIds::UOBJECT_INDEX].Offset, GlobalFields[EFieldIds::UOBJECT_INDEX]);
			returnMap.emplace(GlobalFields[EFieldIds::UOBJECT_OUTER].Offset, GlobalFields[EFieldIds::UOBJECT_OUTER]);
			returnMap.emplace(GlobalFields[EFieldIds::UOBJECT_NAME].Offset, GlobalFields[EFieldIds::UOBJECT_NAME]);
			returnMap.emplace(GlobalFields[EFieldIds::UOBJECT_CLASS].Offset, GlobalFields[EFieldIds::UOBJECT_CLASS]);
			classSize = sizeof(UObject);
			startOffset = 0;
			break;
		case EClassTypes::CLASS_UFIELD:
			returnMap.emplace(GlobalFields[EFieldIds::UFIELD_NEXT].Offset, GlobalFields[EFieldIds::UFIELD_NEXT]);
			if (GlobalFields.find(EFieldIds::UFIELD_SUPERFIELD) != GlobalFields.end()) { returnMap.emplace(GlobalFields[EFieldIds::UFIELD_SUPERFIELD].Offset, GlobalFields[EFieldIds::UFIELD_SUPERFIELD]); }
			classSize = sizeof(UField);
			startOffset = sizeof(UObject);
			break;
		case EClassTypes::CLASS_UENUM:
			returnMap.emplace(GlobalFields[EFieldIds::UENUM_NAMES].Offset, GlobalFields[EFieldIds::UENUM_NAMES]);
			classSize = sizeof(UEnum);
			startOffset = sizeof(UField);
			break;
		case EClassTypes::CLASS_UCONST:
			returnMap.emplace(GlobalFields[EFieldIds::UCONST_VALUE].Offset, GlobalFields[EFieldIds::UCONST_VALUE]);
			classSize = sizeof(UConst);
			startOffset = sizeof(UField);
			break;
		case EClassTypes::CLASS_UPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UPROPERTY_DIMENSION].Offset, GlobalFields[EFieldIds::UPROPERTY_DIMENSION]);
			returnMap.emplace(GlobalFields[EFieldIds::UPROPERTY_SIZE].Offset, GlobalFields[EFieldIds::UPROPERTY_SIZE]);
			returnMap.emplace(GlobalFields[EFieldIds::UPROPERTY_FLAGS].Offset, GlobalFields[EFieldIds::UPROPERTY_FLAGS]);
			returnMap.emplace(GlobalFields[EFieldIds::UPROPERTY_OFFSET].Offset, GlobalFields[EFieldIds::UPROPERTY_OFFSET]);
			classSize = sizeof(UProperty);
			startOffset = sizeof(UField);
			break;
		case EClassTypes::CLASS_USTRUCT:
			if (GlobalFields.find(EFieldIds::USTRUCT_SUPERFIELD) != GlobalFields.end()) { returnMap.emplace(GlobalFields[EFieldIds::USTRUCT_SUPERFIELD].Offset, GlobalFields[EFieldIds::USTRUCT_SUPERFIELD]); }
			returnMap.emplace(GlobalFields[EFieldIds::USTRUCT_CHILDREN].Offset, GlobalFields[EFieldIds::USTRUCT_CHILDREN]);
			returnMap.emplace(GlobalFields[EFieldIds::USTRUCT_SIZE].Offset, GlobalFields[EFieldIds::USTRUCT_SIZE]);
			classSize = sizeof(UStruct);
			startOffset = sizeof(UField);
			break;
		case EClassTypes::CLASS_UFUNCTION:
			returnMap.emplace(GlobalFields[EFieldIds::UFUNCTION_FLAGS].Offset, GlobalFields[EFieldIds::UFUNCTION_FLAGS]);
			returnMap.emplace(GlobalFields[EFieldIds::UFUNCTION_NATIVE].Offset, GlobalFields[EFieldIds::UFUNCTION_NATIVE]);
			classSize = sizeof(UFunction);
			startOffset = sizeof(UStruct);
			break;
		case EClassTypes::CLASS_USTRUCTPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::USTRUCTPROPERTY_STRUCT].Offset, GlobalFields[EFieldIds::USTRUCTPROPERTY_STRUCT]);
			classSize = sizeof(UStructProperty);
			startOffset = sizeof(UProperty);
			break;
		case EClassTypes::CLASS_UOBJECTPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UOBJECTPROPERTY_PROPERTY].Offset, GlobalFields[EFieldIds::UOBJECTPROPERTY_PROPERTY]);
			classSize = sizeof(UObjectProperty);
			startOffset = sizeof(UObject);
			break;
		case EClassTypes::CLASS_UCLASSPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UCLASSPROPERTY_METACLASS].Offset, GlobalFields[EFieldIds::UCLASSPROPERTY_METACLASS]);
			classSize = sizeof(UClassProperty);
			startOffset = sizeof(UProperty);
			break;
		case EClassTypes::CLASS_UMAPPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UMAPPROPERTY_KEY].Offset, GlobalFields[EFieldIds::UMAPPROPERTY_KEY]);
			returnMap.emplace(GlobalFields[EFieldIds::UMAPPROPERTY_VALUE].Offset, GlobalFields[EFieldIds::UMAPPROPERTY_VALUE]);
			classSize = sizeof(UMapProperty);
			startOffset = sizeof(UProperty);
			break;
		case EClassTypes::CLASS_UINTERFACEPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UINTERFACEPROPERTY_CLASS].Offset, GlobalFields[EFieldIds::UINTERFACEPROPERTY_CLASS]);
			classSize = sizeof(UInterfaceProperty);
			startOffset = sizeof(UProperty);
			break;
		case EClassTypes::CLASS_UDELEGATEPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UDELEGATEPROPERTY_FUNCTION].Offset, GlobalFields[EFieldIds::UDELEGATEPROPERTY_FUNCTION]);
			returnMap.emplace(GlobalFields[EFieldIds::UDELEGATEPROPERTY_NAME].Offset, GlobalFields[EFieldIds::UDELEGATEPROPERTY_NAME]);
			classSize = sizeof(UDelegateProperty);
			startOffset = sizeof(UProperty);
			break;
		case EClassTypes::CLASS_UBYTEPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UBYTEPROPERTY_ENUM].Offset, GlobalFields[EFieldIds::UBYTEPROPERTY_ENUM]);
			classSize = sizeof(UByteProperty);
			startOffset = sizeof(UProperty);
			break;
		case EClassTypes::CLASS_UBOOLPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UBOOLPROPERTY_BITMASK].Offset, GlobalFields[EFieldIds::UBOOLPROPERTY_BITMASK]);
			classSize = sizeof(UBoolProperty);
			startOffset = sizeof(UProperty);
			break;
		case EClassTypes::CLASS_UARRAYPROPERTY:
			returnMap.emplace(GlobalFields[EFieldIds::UARRAYPROPERTY_INNTER].Offset, GlobalFields[EFieldIds::UARRAYPROPERTY_INNTER]);
			classSize = sizeof(UArrayProperty);
			startOffset = sizeof(UProperty);
			break;
		}

		return returnMap;
	}

	void AssertField(const ClassField& classField)
	{
		GlobalFields[classField.Id] = classField;
	}
}

/*
# ========================================================================================= #
# Initialize Globals
# ========================================================================================= #
*/

TArray<class UObject*>* GObjects{};
TArray<struct FNameEntry*>* GNames{};

/*
# ========================================================================================= #
# Class Functions
# ========================================================================================= #
*/

TArray<class UObject*>* UObject::GObjObjects()
{
	TArray<UObject*>* objectArray = reinterpret_cast<TArray<UObject*>*>(GObjects);
	return objectArray;
}

std::string UObject::GetName()
{
	return this->Name.ToString();
}

std::string UObject::GetNameCPP()
{
	std::string nameCPP;

	if (this->IsA(UClass::StaticClass()))
	{
		UClass* uClass = static_cast<UClass*>(this);

		while (uClass)
		{
			std::string className = uClass->GetName();

			if (className == "Actor")
			{
				nameCPP += "A";
				break;
			}
			else if (className == "Object")
			{
				nameCPP += "U";
				break;
			}

			uClass = static_cast<UClass*>(uClass->SuperField);
		}
	}
	else
	{
		nameCPP += "F";
	}

	nameCPP += this->GetName();

	return nameCPP;
}

std::string UObject::GetFullName()
{
	std::string fullName = this->GetName();

	for (UObject* uOuter = this->Outer; uOuter; uOuter = uOuter->Outer)
	{
		fullName = uOuter->GetName() + "." + fullName;
	}

	fullName = this->Class->GetName() + " " + fullName;

	return fullName;
}

std::string UObject::GetPackageName()
{
	UObject* uPackageObj = this->GetPackageObj();

	if (uPackageObj)
	{
		static std::string packageName = uPackageObj->GetName();

		return packageName;
	}

	return "null";
}

UObject* UObject::GetPackageObj()
{
	UObject* uPackage = nullptr;

	for (UObject* uOuter = this->Outer; uOuter; uOuter = uOuter->Outer)
	{
		uPackage = uOuter;
	}

	return uPackage;
}

class UClass* UObject::FindClass(const std::string& classFullName)
{
	static bool initialized = false;
	static std::map<std::string, UClass*> foundClasses{};

	if (!initialized)
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject)
			{
				std::string objectFullName = uObject->GetFullName();

				if (objectFullName.find("Class") == 0)
				{
					foundClasses[objectFullName] = static_cast<UClass*>(uObject);
				}
			}
		}

		initialized = true;
	}

	if (foundClasses.find(classFullName) != foundClasses.end())
	{
		return foundClasses[classFullName];
	}

	return nullptr;
}

bool UObject::IsA(class UClass* uClass)
{
	for (UClass* uSuperClass = this->Class; uSuperClass; uSuperClass = static_cast<UClass*>(uSuperClass->SuperField))
	{
		if (uSuperClass == uClass)
		{
			return true;
		}
	}

	return false;
}

bool UObject::IsA(int32_t objInternalInteger)
{
	UClass* uClass = UObject::GObjObjects()->At(objInternalInteger)->Class;

	if (uClass)
	{
		return this->IsA(uClass);
	}

	return false;
}

class UFunction* UFunction::FindFunction(const std::string& functionFullName)
{
	static bool initialized = false;
	static std::map<std::string, UFunction*> foundFunctions{};

	if (!initialized)
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject)
			{
				std::string objectFullName = uObject->GetFullName();

				if (objectFullName.find("Function") == 0)
				{
					foundFunctions[objectFullName] = static_cast<UFunction*>(uObject);
				}
			}
		}

		initialized = true;
	}

	if (foundFunctions.find(functionFullName) != foundFunctions.end())
	{
		return foundFunctions[functionFullName];
	}

	return nullptr;
}

/*
# ========================================================================================= #
#
# ========================================================================================= #
*/