#pragma once
#include <algorithm>
#include <stdlib.h>
#include <locale.h>
#include <xlocale>
#include <ctype.h>
#include <chrono>
#include <thread>
#include <vector>
#include <map>

/*
# ========================================================================================= #
# Game Defines
# ========================================================================================= #
*/

// Function Flags
enum EFunctionFlags
{
	FUNC_None =								0x00000000,
	FUNC_Final =							0x00000001,
	FUNC_RequiredAPI =						0x00000002,
	FUNC_BlueprintAuthorityOnly =			0x00000004,
	FUNC_BlueprintCosmetic =				0x00000008,
	FUNC_Net =								0x00000040,
	FUNC_NetReliable =						0x00000080,
	FUNC_NetRequest =						0x00000100,
	FUNC_Exec =								0x00000200,
	FUNC_Native =							0x00000400,
	FUNC_Event =							0x00000800,
	FUNC_NetResponse =						0x00001000,
	FUNC_Static =							0x00002000,
	FUNC_NetMulticast =						0x00004000,
	FUNC_UbergraphFunction =				0x00008000,
	FUNC_MulticastDelegate =				0x00010000,
	FUNC_Public =							0x00020000,
	FUNC_Private =							0x00040000,
	FUNC_Protected =						0x00080000,
	FUNC_Delegate =							0x00100000,
	FUNC_NetServer =						0x00200000,
	FUNC_HasOutParms =						0x00400000,
	FUNC_HasDefaults =						0x00800000,
	FUNC_NetClient =						0x01000000,
	FUNC_DLLImport =						0x02000000,
	FUNC_BlueprintCallable =				0x04000000,
	FUNC_BlueprintEvent =					0x08000000,
	FUNC_BlueprintPure =					0x10000000,
	FUNC_EditorOnly =						0x20000000,
	FUNC_Const =							0x40000000,
	FUNC_NetValidate =						0x80000000,
	FUNC_AllFlags =							0xFFFFFFFF
};

// Proprerty Flags
enum EPropertyFlags
{
	CPF_Edit =								0x0000000000000001,		// Property is user-settable in the editor.
	CPF_Const =								0x0000000000000002,		// Actor's property always matches class's default actor property.
	CPF_Input =							    0x0000000000000004,		// Variable is writable by the input system.
	CPF_ExportObject =						0x0000000000000008,		// Object can be exported with actor.
	CPF_OptionalParm =						0x0000000000000010,		// Optional parameter (if CPF_Param is set).
	CPF_Net =								0x0000000000000020,		// Property is relevant to network replication.
	CPF_EditConstArray =					0x0000000000000040,		// Prevent adding/removing of items from dynamic a array in the editor.
	CPF_Parm =								0x0000000000000080,		// Function/When call parameter.
	CPF_OutParm =							0x0000000000000100,		// Value is copied out after function call.
	CPF_SkipParm =							0x0000000000000200,		// Property is a short-circuitable evaluation function parm.
	CPF_ReturnParm =						0x0000000000000400,		// Return value.
	CPF_CoerceParm =						0x0000000000000800,		// Coerce args into this function parameter.
	CPF_Native =							0x0000000000001000,		// Property is native: C++ code is responsible for serializing it.
	CPF_Transient =							0x0000000000002000,		// Property is transient: shouldn't be saved, zero-filled at load time.
	CPF_Config =							0x0000000000004000,		// Property should be loaded/saved as permanent profile.
	CPF_Localized =							0x0000000000008000,		// Property should be loaded as localizable text.
	CPF_Travel =							0x0000000000010000,		// Property travels across levels/servers.
	CPF_EditConst =							0x0000000000020000,		// Property is uneditable in the editor.
	CPF_GlobalConfig =						0x0000000000040000,		// Load config from base class, not subclass.
	CPF_Component =							0x0000000000080000,		// Property containts component references.
	CPF_NeedCtorLink =						0x0000000000400000,		// Fields need construction/destruction.
	CPF_NoExport =							0x0000000000800000,		// Property should not be exported to the native class header file.
	CPF_NoClear =							0x0000000002000000,		// Hide clear (and browse) button.
	CPF_EditInline =						0x0000000004000000,		// Edit this object reference inline.
	CPF_EdFindable =						0x0000000008000000,		// References are set by clicking on actors in the editor viewports.
	CPF_EditInlineUse =						0x0000000010000000,		// EditInline with Use button.
	CPF_Deprecated =						0x0000000020000000,		// Property is deprecated.  Read it from an archive, but don't save it.
	CPF_EditInlineNotify =					0x0000000040000000,		// EditInline, notify outer object on editor change.
	CPF_RepNotify =							0x0000000100000000,		// Notify actors when a property is replicated
	CPF_Interp =							0x0000000200000000,		// interpolatable property for use with matinee
	CPF_NonTransactional =					0x0000000400000000,		// Property isn't transacted
	CPF_EditorOnly =						0x0000000800000000,		// Property should only be loaded in the editor.
	CPF_NoDestructor =						0x0000001000000000,		// No destructor.
	CPF_AutoWeak =							0x0000004000000000,		// CPF_ = 0x0000002000000000, ///<.
	CPF_ContainsInstancedReference =        0x0000008000000000,		// Property contains component refuerences.
	CPF_AssetRegistrySearchable =           0x0000010000000000,		// Asset instances will add properties with this flag to the asset registry automatically
	CPF_SimpleDisplay =						0x0000020000000000,		// The property is visible by default in the editor details view.
	CPF_AdvancedDisplay =					0x0000040000000000,		// The property is advanced and not visible by default in the editor details view.
	CPF_Protected =							0x0000080000000000,		// Property is protected from the perspective of scrip
	CPF_BlueprintCallable =					0x0000100000000000,		// MC Delegates only. Property should be exposed for calling in blueprint code.
	CPF_BlueprintAuthorityOnly =			0x0000200000000000,		// MC Delegates only. This delegate accepts (only in blueprint) only events with BlueprintAuthorityOnly.
	CPF_TextExportTransient =				0x0000400000000000,		// Property shouldn't be exported to text format (e.g. copy/paste)
	CPF_NonPIEDuplicateTransient =			0x0000800000000000,		// Property should only be copied in PIE.
	CPF_ExposeOnSpawn =						0x0001000000000000,		// Property is exposed on spawn.
	CPF_PersistentInstance =				0x0002000000000000,		// A object referenced by the property is duplicated like a component. (Each actor should have an own instance.)
	CPF_UObjectWrapper =					0x0004000000000000,		// Property was parsed as a wrapper class like TSubclassOf , FScriptInterface etc., rather than a USomething*.
	CPF_HasGetValueTypeHash =				0x0008000000000000,		// This property can generate a meaningful hash value.
	CPF_NativeAccessSpecifierPublic =		0x0010000000000000,		// Public native access specifier.
	CPF_NativeAccessSpecifierProtected =	0x0020000000000000,		// Protected native access specifier.
	CPF_NativeAccessSpecifierPrivate =		0x0040000000000000,		// Private native access specifier.
	CPF_SkipSerialization =					0x0080000000000000		// Property shouldn't be serialized, can still be exported to text.
};

// Property Types
enum class EPropertyTypes : uint16_t
{
	TYPE_UNKNOWN =				0,
	TYPE_INT8 =					1,
	TYPE_INT16 =				2,
	TYPE_INT32 =				3,
	TYPE_INT64 =				4,
	TYPE_UINT8 =				5,
	TYPE_UINT16 =				6,
	TYPE_UINT32 =				7,
	TYPE_UINT64 =				8,
	TYPE_LONG =					9,
	TYPE_ULONG =				10,
	TYPE_DOUBLE =				11,
	TYPE_FLOAT =				12,
	TYPE_BOOL =					13,
	TYPE_TARRAY =				14,
	TYPE_TMAP =					15,
	TYPE_FNAMEENTRY =			16,
	TYPE_FNAME =				17,
	TYPE_FSTRING =				18,
	TYPE_FSCRIPTDELEGATE =		19,
	TYPE_FPOINTER =				20,
	TYPE_FSTRUCT =				21,
	TYPE_UPOINTER =				22,
};

// Width Types
enum class EWidthTypes : uint16_t
{
	WIDTH_NONE =				0,
	WIDTH_BYTE =				2,
	WIDTH_SIZE =				4,
	WIDTH_BITMASK =				8,
	WIDTH_FUNCTION =			8,
	WIDTH_PROPERTY =			16
};

template<typename TArray>
struct TIterator
{
public:
	using ElementType = typename TArray::ElementType;
	using ElementPointer = ElementType*;
	using ElementReference = ElementType&;
	using ElementConstReference = const ElementType&;

private:
	ElementPointer IteratorData;

public:
	TIterator(ElementPointer inElementPointer)
	{
		IteratorData = inElementPointer;
	}

public:
	TIterator& operator++()
	{
		IteratorData++;
		return *this;
	}

	TIterator operator++(int32_t)
	{
		TIterator iteratorCopy = *this;
		++(*this);
		return iteratorCopy;
	}

	TIterator& operator--()
	{
		IteratorData--;
		return *this;
	}

	TIterator operator--(int32_t)
	{
		TIterator iteratorCopy = *this;
		--(*this);
		return iteratorCopy;
	}

	ElementReference operator[](int32_t index)
	{
		return *(IteratorData[index]);
	}

	ElementPointer operator->()
	{
		return IteratorData;
	}

	ElementReference operator*()
	{
		return *IteratorData;
	}

	bool operator==(const TIterator& other) const
	{
		return (IteratorData == other.IteratorData);
	}

	bool operator!=(const TIterator& other) const
	{
		return !(*this == other);
	}
};

template<typename InElementType>
struct TArray
{
public:
	using ElementType = InElementType;
	using ElementPointer = ElementType*;
	using ElementReference = ElementType&;
	using ElementConstReference = const ElementType&;
	using Iterator = TIterator<TArray<ElementType>>;

private:
	ElementPointer ArrayData;
	int32_t ArrayCount;
	int32_t ArrayMax;

public:
	TArray()
	{
		ArrayData = nullptr;
		ArrayCount = 0;
		ArrayMax = 0;

		//ReAllocate(sizeof(ElementType));
	}

	~TArray()
	{
		//Clear();
		//::operator delete(ArrayData, ArrayMax * sizeof(ElementType));
	}

public:
	ElementConstReference operator[](int32_t index) const
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		}
	}

	ElementReference operator[](int32_t index)
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		} 
	}

	ElementConstReference At(const int32_t index) const
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		} 
	}

	ElementReference At(const int32_t index)
	{
		if (index <= ArrayCount)
		{
			return ArrayData[index];
		} 
	}

	void Add(ElementConstReference newElement)
	{
		if (ArrayCount >= ArrayMax)
		{
			ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
		}

		new(&ArrayData[ArrayCount]) ElementType(newElement);
		ArrayCount++;
	}

	void Add(ElementReference& newElement)
	{
		if (ArrayCount >= ArrayMax)
		{
			ReAllocate(sizeof(ElementType) * (ArrayCount + 1));
		}

		new(&ArrayData[ArrayCount]) ElementType(newElement);
		ArrayCount++;
	}

	void PopBack()
	{
		if (ArrayCount > 0)
		{
			ArrayCount--;
			ArrayData[ArrayCount].~ElementType();
		}
	}

	void Clear()
	{
		for (int32_t i = 0; i < ArrayCount; i++)
		{
			ArrayData[i].~ElementType();
		}

		ArrayCount = 0;
	}

	int32_t Num() const
	{
		return ArrayCount;
	}

	int32_t Max() const
	{
		return ArrayMax;
	}

	Iterator begin()
	{
		return Iterator(ArrayData);
	}

	Iterator end()
	{
		return Iterator(ArrayData + ArrayCount);
	}

private:
	void ReAllocate(int32_t newArrayMax)
	{
		ElementPointer newArrayData = (ElementPointer)::operator new(newArrayMax * sizeof(ElementType));

		int32_t newNum = ArrayCount;

		if (newArrayMax < newNum)
		{
			newNum = newArrayMax;
		}

		for (int32_t i = 0; i < newNum; i++)
		{
			new(newArrayData + i) ElementType(std::move(ArrayData[i]));
		}

		for (int32_t i = 0; i < ArrayCount; i++)
		{
			ArrayData[i].~ElementType();
		}

		::operator delete(ArrayData, ArrayMax * sizeof(ElementType));
		ArrayData = newArrayData;
		ArrayMax = newArrayMax;
	}
};

/*
# ========================================================================================= #
# Globals
# ========================================================================================= #
*/

extern TArray<class UObject*>* GObjects;
extern TArray<struct FNameEntry*>* GNames;

/*
# ========================================================================================= #
# Structs
# ========================================================================================= #
*/

struct FNameEntry
{
public:
	int64_t			Flags;										// 0x0000 (0x08)
	int32_t			Index;										// 0x0008 (0x04)
	uint8_t			UnknownData00[0xC];							// 0x000C (0x0C)
	wchar_t			Name[0x400];								// 0x0018 (0x00) // Will either be char, or wchar_t depending on the game.
	//char			Name[0x400];								// 0x0018 (0x00)

public:
	int32_t GetIndex() const
	{
		return Index;
	}

	std::string GetName() const
	{
		std::wstring ws(Name);
		std::string str(ws.begin(), ws.end());
		return str;
	}

	//std::string GetName() const
	//{
	//	return Name;
	//}

	const wchar_t* GetWideName() const
	{
		return Name;
	}

	//const char* GetAnsiName() const
	//{
	//	return Name;
	//}
};

struct FName
{
public:
	using ElementType = const wchar_t; // Will either be const char, or const wchar_t depending on the game.
	using ElementPointer = ElementType*;

private:
	int32_t	FNameEntryId;
	int32_t	InstanceNumber;

public:
	FName()
	{
		FNameEntryId = 0;
		InstanceNumber = 0;
	}

	FName(const int32_t& i)
	{
		FNameEntryId = i;
		InstanceNumber = 0;
	}

	FName(ElementPointer nameToFind)
	{
		static std::vector<int32_t> nameCache{};

		FNameEntryId = 0;
		InstanceNumber = 0;

		for (int32_t name : nameCache)
		{
			if (Names()->At(name))
			{
				if (wcscmp(Names()->At(nameCache[name])->Name, nameToFind))
				{
					FNameEntryId = nameCache[name];
					return;
				}
			}
		}

		for (int32_t i = 0; i < Names()->Num(); i++)
		{
			if (Names()->At(i))
			{
				if (wcscmp(Names()->At(i)->Name, nameToFind))
				{
					nameCache.push_back(i);
					FNameEntryId = i;
				}
			}
		}
	}

	~FName() {}

public:
	static TArray<FNameEntry*>* Names()
	{
		TArray<struct FNameEntry*>* GNamesArray = reinterpret_cast<TArray<struct FNameEntry*>*>(GNames);
		return GNamesArray;
	}

	int32_t GetDisplayIndex() const
	{
		return FNameEntryId;
	}

	const struct FNameEntry GetDisplayNameEntry() const
	{
		if (IsValid())
		{
			return *Names()->At(FNameEntryId);
		}
	}

	struct FNameEntry* GetEntry()
	{
		if (IsValid())
		{
			return Names()->At(FNameEntryId);
		}
	}

	int32_t GetNumber() const
	{
		return InstanceNumber;
	}

	void SetNumber(int32_t newNumber)
	{
		InstanceNumber = newNumber;
	}

	std::string ToString() const
	{
		if (IsValid())
		{
			std::wstring ws(GetDisplayNameEntry().Name);
			std::string str(ws.begin(), ws.end());
			return str;
		}

		return std::string("UnknownName");
	}

	bool IsValid() const
	{
		if (FNameEntryId > Names()->Num() || !Names()->At(FNameEntryId))
		{
			return false;
		}

		return true;
	}

	FName operator=(const FName& other)
	{
		FNameEntryId = other.FNameEntryId;
		InstanceNumber = other.InstanceNumber;
		return *this;
	}

	bool operator==(const FName& other) const
	{
		return (FNameEntryId == other.FNameEntryId);
	}
};

struct FString
{
public:
	using ElementType = const wchar_t; // Will either be const char, or const wchar_t depending on the game.
	using ElementPointer = ElementType*;

private:
	ElementPointer ArrayData;
	int32_t ArrayCount;
	int32_t ArrayMax;

public:
	FString()
	{
		ArrayData = nullptr;
		ArrayCount = 0;
		ArrayMax = 0;
	}

	FString(ElementPointer other)
	{
		ArrayData = nullptr;
		ArrayCount = 0;
		ArrayMax = 0;

		ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

		if (ArrayCount > 0)
		{
			ArrayData = other;
		}
	}

	~FString() {}

public:
	std::string ToString() const
	{
		if (IsValid())
		{
			std::wstring wideStr(ArrayData);
			std::string str(wideStr.begin(), wideStr.end());
			return str;
		}

		return std::string("null");
	}

	bool IsValid() const
	{
		return !!ArrayData;
	}

	FString operator=(ElementPointer other)
	{
		if (ArrayData != other)
		{
			ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

			if (ArrayCount > 0)
			{
				ArrayData = other;
			}
		}

		return *this;
	}

	bool operator==(const FString& other)
	{
		return (!wcscmp(ArrayData, other.ArrayData));
	}

	bool operator!=(const FString& other)
	{
		return (wcscmp(ArrayData, other.ArrayData));
	}
};

struct FScriptDelegate
{
	uint8_t UnknownData00[0x1];
};

struct FPointer
{
	uintptr_t Dummy;
};

struct FQWord
{
	int32_t A;
	int32_t B;
};

/*
# ========================================================================================= #
# Classes
# ========================================================================================= #
*/

// Class Core.Object
// (0x0000 - 0x0000)
class UObject
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Object");
		}

		return uClassPointer;
	};

	static TArray<class UObject*>* GObjObjects();

	std::string GetName();
	std::string GetNameCPP();
	std::string GetFullName();
	std::string GetPackageName();
	class UObject* GetPackageObj();

	template<typename T>
	static T* FindObject(const std::string& objectFullName)
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject && uObject->IsA(T::StaticClass()))
			{
				if (uObject->GetFullName() == objectFullName)
				{
					return reinterpret_cast<T*>(uObject);
				}
			}
		}

		return nullptr;
	}

	template<typename T>
	static uint32_t CountObject(const std::string& objectName)
	{
		static std::map<std::string, int32_t> countCache;

		if (countCache.find(objectName) == countCache.end())
		{
			countCache[objectName] = 0;

			for (UObject* uObject : *UObject::GObjObjects())
			{
				if (uObject && uObject->IsA(T::StaticClass()))
				{
					if (uObject->GetName() == objectName)
					{
						countCache[uObject->GetName()]++;
					}
				}
			}
		}

		return countCache[objectName];
	}

	static class UClass* FindClass(const std::string& classFullName);

	bool IsA(class UClass* uClass);
	bool IsA(int objInternalInteger);
};

 //Class Core.Field
// (0x0000 - 0x0000)
class UField : public UObject
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Field");
		}

		return uClassPointer;
	};
};

// Class Core.Enum
// (0x0000 - 0x0000)
class UEnum : public UField
{
public:
	TArray<FName>		Names;									// 0x0000 (0x0C)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Enum");
		}

		return uClassPointer;
	};
};

// Class Core.Const
// (0x0000 - 0x0000)
class UConst : public UField
{
public:
	struct FString		Value;									// 0x0000 (0x10)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Const");
		}

		return uClassPointer;
	};
};

// Class Core.Property
// 0x0000 (0x0000 - 0x0000)
class UProperty : public UField
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Property");
		}

		return uClassPointer;
	};
};

// Class Core.Struct
// 0x0000 (0x0000 - 0x0000)
class UStruct : public UField
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Struct");
		}

		return uClassPointer;
	};
};

// Class Core.Function
// 0x0000 (0x0000 - 0x0000)
class UFunction : public UStruct
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Function");
		}

		return uClassPointer;
	};

	static UFunction* FindFunction(const std::string& functionFullName);
};

// Class Core.ScriptStruct
// 0x0000 (0x0000 - 0x0000)
class UScriptStruct : public UStruct
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.ScriptStruct");
		}

		return uClassPointer;
	};
};

// Class Core.State
// 0x0060 (0x0130 - 0x0190)
class UState : public UStruct
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.State");
		}

		return uClassPointer;
	};
};

// Class Core.Class
// 0x0228 (0x0190 - 0x03B8)
class UClass : public UState
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Class");
		}
		
		return uClassPointer;
	};
};

/*
# ========================================================================================= #
# Property Sub Classes
# ========================================================================================= #
*/

 //Class Core.StructProperty
// 0x0000 (0x0000 - 0x0000)
class UStructProperty : public UProperty
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.StructProperty");
		}

		return uClassPointer;
	};
};

// Class Core.StrProperty
// 0x0000 (0x0000 - 0x0000)
class UStrProperty : public UProperty
{
public:
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.StrProperty");
		}

		return uClassPointer;
	};
};

// Class Core.QWordProperty
// 0x0000 (0x0000 - 0x0000)
class UQWordProperty : public UProperty
{
public:
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.QWordProperty");
		}

		return uClassPointer;
	};
};

// Class Core.ObjectProperty
// 0x0000 (0x0000 - 0x0000)
class UObjectProperty : public UProperty
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.ObjectProperty");
		}

		return uClassPointer;
	};
};

// Class Core.ComponentProperty
// 0x0000 (0x0000 - 0x0000)
class UComponentProperty : public UObjectProperty
{
public:
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.ComponentProperty");
		}

		return uClassPointer;
	};
};

// Class Core.ClassProperty
// 0x0000 (0x0000 - 0x0000)
class UClassProperty : public UObjectProperty
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.ClassProperty");
		}

		return uClassPointer;
	};
};

// Class Core.NameProperty
// 0x00C8 (0x0000 - 0x0000)
class UNameProperty : public UProperty
{
public:
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.NameProperty");
		}

		return uClassPointer;
	};
};

// Class Core.MapProperty
// 0x0010 (0x0000 - 0x0000)
class UMapProperty : public UProperty
{
public:
	class UProperty*	Key;									// 0x00C8 (0x08)
	class UProperty*	Value;									// 0x00D0 (0x08)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.MapProperty");
		}

		return uClassPointer;
	};
};

// Class Core.IntProperty
//  0x00C8 (0x00C8 - 0x00C8)
class UIntProperty : public UProperty
{
public:
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.IntProperty");
		}

		return uClassPointer;
	};
};

// Class Core.InterfaceProperty
// 0x0008 (0x00C8 - 0x00D0)
class UInterfaceProperty : public UProperty
{
public:
	class UClass*		InterfaceClass;							// 0x00C8 (0x08)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.InterfaceProperty");
		}

		return uClassPointer;
	};
};

// Class Core.FloatProperty
// 0x0000 (0x0000 - 0x0000)
class UFloatProperty : public UProperty
{
public:
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.FloatProperty");
		}

		return uClassPointer;
	};
};

// Class Core.DelegateProperty
// 0x0000 (0x0000 - 0x0000)
class UDelegateProperty : public UProperty
{
public:
	uint8_t				UnknownData00[0x00];					// 0x0000 (0x00)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.DelegateProperty");
		}

		return uClassPointer;
	};
};

// Class Core.ByteProperty
// 0x0008 (0x0000 - 0x0000)
class UByteProperty : public UProperty
{
public:
	class UEnum*		Enum;									// 0x00C8 (0x08)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.ByteProperty");
		}

		return uClassPointer;
	};
};

// Class Core.BoolProperty
// 0x0008 (0x0000 - 0x0000)
class UBoolProperty : public UProperty
{
public:
	uint64_t			BitMask;								// 0x00C8 (0x08)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.BoolProperty");
		}

		return uClassPointer;
	};
};

// Class Core.ArrayProperty
// 0x0008 (0x0000 - 0x0000)
class UArrayProperty : public UProperty
{
public:
	class UProperty*	Inner;									// 0x00C8 (0x08)
public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.ArrayProperty");
		}

		return uClassPointer;
	};
};

/*
# ========================================================================================= #
#
# ========================================================================================= #
*/