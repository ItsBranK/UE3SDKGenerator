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
#include <functional>
#include "Configuration.hpp"

/*
# ========================================================================================= #
# Game Defines
# ========================================================================================= #
*/

// Function Flags
// https://docs.unrealengine.com/en-US/API/Runtime/CoreUObject/UObject/EFunctionFlags/index.html
enum EFunctionFlags
{
	FUNC_None =                             0x00000000,
	FUNC_Final =                            0x00000001,
	FUNC_RequiredAPI =                      0x00000002,
	FUNC_BlueprintAuthorityOnly =           0x00000004,
	FUNC_BlueprintCosmetic =                0x00000008,
	FUNC_Net =                              0x00000040,
	FUNC_NetReliable =                      0x00000080,
	FUNC_NetRequest =                       0x00000100,
	FUNC_Exec =                             0x00000200,
	FUNC_Native =                           0x00000400,
	FUNC_Event =                            0x00000800,
	FUNC_NetResponse =                      0x00001000,
	FUNC_Static =                           0x00002000,
	FUNC_NetMulticast =                     0x00004000,
	FUNC_UbergraphFunction =                0x00008000,
	FUNC_MulticastDelegate =                0x00010000,
	FUNC_Public =                           0x00020000,
	FUNC_Private =                          0x00040000,
	FUNC_Protected =                        0x00080000,
	FUNC_Delegate =                         0x00100000,
	FUNC_NetServer =                        0x00200000,
	FUNC_HasOutParms =                      0x00400000,
	FUNC_HasDefaults =                      0x00800000,
	FUNC_NetClient =                        0x01000000,
	FUNC_DLLImport =                        0x02000000,
	FUNC_BlueprintCallable =                0x04000000,
	FUNC_BlueprintEvent =                   0x08000000,
	FUNC_BlueprintPure =                    0x10000000,
	FUNC_EditorOnly =                       0x20000000,
	FUNC_Const =                            0x40000000,
	FUNC_NetValidate =                      0x80000000,
	FUNC_AllFlags =                         0xFFFFFFFF
};

// Proprerty Flags
// https://docs.unrealengine.com/en-US/API/Runtime/CoreUObject/UObject/EPropertyFlags/index.html (The ones in this link are UE4 specific, so I had to modify accordingly here.)
enum EPropertyFlags
{
	CPF_Edit =								0x0000000000000001,	// Property is user-settable in the editor.
	CPF_Const =								0x0000000000000002,	// Actor's property always matches class's default actor property.
	CPF_Input =							    0x0000000000000004,	// Variable is writable by the input system.
	CPF_ExportObject =						0x0000000000000008,	// Object can be exported with actor.
	CPF_OptionalParm =						0x0000000000000010,	// Optional parameter (if CPF_Param is set).
	CPF_Net =								0x0000000000000020,	// Property is relevant to network replication.
	CPF_EditConstArray =					0x0000000000000040,	// Prevent adding/removing of items from dynamic a array in the editor.
	CPF_Parm =								0x0000000000000080,	// Function/When call parameter.
	CPF_OutParm =							0x0000000000000100,	// Value is copied out after function call.
	CPF_SkipParm =							0x0000000000000200,	// Property is a short-circuitable evaluation function parm.
	CPF_ReturnParm =						0x0000000000000400,	// Return value.
	CPF_CoerceParm =						0x0000000000000800,	// Coerce args into this function parameter.
	CPF_Native =							0x0000000000001000,	// Property is native: C++ code is responsible for serializing it.
	CPF_Transient =							0x0000000000002000,	// Property is transient: shouldn't be saved, zero-filled at load time.
	CPF_Config =							0x0000000000004000,	// Property should be loaded/saved as permanent profile.
	CPF_Localized =							0x0000000000008000,	// Property should be loaded as localizable text.
	CPF_Travel =							0x0000000000010000,	// Property travels across levels/servers.
	CPF_EditConst =							0x0000000000020000,	// Property is uneditable in the editor.
	CPF_GlobalConfig =						0x0000000000040000,	// Load config from base class, not subclass.
	CPF_Component =							0x0000000000080000,	// Property containts component references.
	CPF_NeedCtorLink =						0x0000000000400000,	// Fields need construction/destruction.
	CPF_NoExport =							0x0000000000800000,	// Property should not be exported to the native class header file.
	CPF_NoClear =							0x0000000002000000,	// Hide clear (and browse) button.
	CPF_EditInline =						0x0000000004000000,	// Edit this object reference inline.
	CPF_EdFindable =						0x0000000008000000,	// References are set by clicking on actors in the editor viewports.
	CPF_EditInlineUse =						0x0000000010000000,	// EditInline with Use button.
	CPF_Deprecated =						0x0000000020000000,	// Property is deprecated.  Read it from an archive, but don't save it.
	CPF_EditInlineNotify =					0x0000000040000000,	// EditInline, notify outer object on editor change.
	CPF_RepNotify =							0x0000000100000000,	// Notify actors when a property is replicated
	CPF_Interp =							0x0000000200000000,	// interpolatable property for use with matinee
	CPF_NonTransactional =					0x0000000400000000,	// Property isn't transacted
	CPF_EditorOnly =						0x0000000800000000,	// Property should only be loaded in the editor.
	CPF_NoDestructor =						0x0000001000000000,	// No destructor.
	CPF_AutoWeak =							0x0000004000000000,	// CPF_ = 0x0000002000000000, ///<.
	CPF_ContainsInstancedReference =        0x0000008000000000,	// Property contains component refuerences.
	CPF_AssetRegistrySearchable =           0x0000010000000000,	// Asset instances will add properties with this flag to the asset registry automatically
	CPF_SimpleDisplay =						0x0000020000000000,	// The property is visible by default in the editor details view.
	CPF_AdvancedDisplay =					0x0000040000000000,	// The property is advanced and not visible by default in the editor details view.
	CPF_Protected =							0x0000080000000000,	// Property is protected from the perspective of scrip
	CPF_BlueprintCallable =					0x0000100000000000,	// MC Delegates only. Property should be exposed for calling in blueprint code.
	CPF_BlueprintAuthorityOnly =			0x0000200000000000,	// MC Delegates only. This delegate accepts (only in blueprint) only events with BlueprintAuthorityOnly.
	CPF_TextExportTransient =				0x0000400000000000,	// Property shouldn't be exported to text format (e.g. copy/paste)
	CPF_NonPIEDuplicateTransient =			0x0000800000000000,	// Property should only be copied in PIE.
	CPF_ExposeOnSpawn =						0x0001000000000000,	// Property is exposed on spawn.
	CPF_PersistentInstance =				0x0002000000000000,	// A object referenced by the property is duplicated like a component. (Each actor should have an own instance.)
	CPF_UObjectWrapper =					0x0004000000000000,	// Property was parsed as a wrapper class like TSubclassOf , FScriptInterface etc., rather than a USomething*.
	CPF_HasGetValueTypeHash =				0x0008000000000000,	// This property can generate a meaningful hash value.
	CPF_NativeAccessSpecifierPublic =		0x0010000000000000,	// Public native access specifier.
	CPF_NativeAccessSpecifierProtected =	0x0020000000000000,	// Protected native access specifier.
	CPF_NativeAccessSpecifierPrivate =		0x0040000000000000,	// Private native access specifier.
	CPF_SkipSerialization =					0x0080000000000000	// Property shouldn't be serialized, can still be exported to text.
};

// Object Flags
// https://docs.unrealengine.com/4.26/en-US/API/Runtime/CoreUObject/UObject/EObjectFlags/
enum EObjectFlags
{
	RF_NoFlags =							0x00000000,
	RF_Public =								0x00000001,
	RF_Standalone =							0x00000002,
	RF_MarkAsNative =						0x00000004,
	RF_Transactional =						0x00000008,
	RF_ClassDefaultObject =					0x00000010,
	RF_ArchetypeObject =					0x00000020,
	RF_Transient =							0x00000040,
	RF_MarkAsRootSet =						0x00000080,
	RF_TagGarbageTemp =						0x00000100,
	RF_NeedInitialization =					0x00000200,
	RF_NeedLoad =							0x00000400,
	RF_KeepForCooker =						0x00000800,
	RF_NeedPostLoad =						0x00001000,
	RF_NeedPostLoadSubobjects =				0x00002000,
	RF_NewerVersionExists =					0x00004000,
	RF_BeginDestroyed =						0x00008000,
	RF_FinishDestroyed =					0x00010000,
	RF_BeingRegenerated =					0x00020000,
	RF_DefaultSubObject =					0x00040000,
	RF_WasLoaded =							0x00080000,
	RF_TextExportTransient =				0x00100000,
	RF_LoadCompleted =						0x00200000,
	RF_InheritableComponentTemplate =		0x00400000,
	RF_DuplicateTransient =					0x00800000,
	RF_StrongRefOnFrame =					0x01000000,
	RF_NonPIEDuplicateTransient =			0x02000000,
	RF_Dynamic =							0x04000000,
	RF_WillBeLoaded =						0x08000000,
};

// Class Types
enum class EClassTypes : uint8_t
{
	CLASS_UNKNOWN,
	CLASS_FNAMEENTRY,
	CLASS_UOBJECT,
	CLASS_UFIELD,
	CLASS_UENUM,
	CLASS_UCONST,
	CLASS_UPROPERTY,
	CLASS_USTRUCT,
	CLASS_UFUNCTION,
	CLASS_USCRIPTSTRUCT,
	CLASS_USTATE,
	CLASS_UCLASS,
	CLASS_USTRUCTPROPERTY,
	CLASS_USTRPROPERTY,
	CLASS_UQWORDPROPERTY,
	CLASS_UOBJECTPROPERTY,
	CLASS_UCOMPONENTPROPERTY,
	CLASS_UCLASSPROPERTY,
	CLASS_UNAMEPROPERTY,
	CLASS_UMAPPROPERTY,
	CLASS_UINTPROPERTY,
	CLASS_UINTERFACEPROPERTY,
	CLASS_UFLOATPROPERTY,
	CLASS_UDELEGATEPROPERTY,
	CLASS_UBYTEPROPERTY,
	CLASS_UBOOLPROPERTY,
	CLASS_UARRAYPROPERTY
};

// Field Identifiers
enum class EFieldIds : uint8_t
{
	UNKNOWN,
	FNAMEENTRY_INDEX,
	FNAMEENTRY_NAME_UTF16,
	FNAMEENTRY_NAME_UTF8,
	UOBJECT_VFTABLE,
	UOBJECT_INDEX,
	UOBJECT_OUTER,
	UOBJECT_NAME,
	UOBJECT_CLASS,
	UFIELD_NEXT,
	UFIELD_SUPERFIELD,
	UENUM_NAMES,
	UCONST_VALUE,
	UPROPERTY_DIMENSION,
	UPROPERTY_SIZE,
	UPROPERTY_FLAGS,
	UPROPERTY_OFFSET,
	USTRUCT_SUPERFIELD,
	USTRUCT_CHILDREN,
	USTRUCT_SIZE,
	UFUNCTION_FLAGS,
	UFUNCTION_NATIVE,
	USTRUCTPROPERTY_STRUCT,
	UOBJECTPROPERTY_PROPERTY,
	UCLASSPROPERTY_METACLASS,
	UMAPPROPERTY_KEY,
	UMAPPROPERTY_VALUE,
	UINTERFACEPROPERTY_CLASS,
	UDELEGATEPROPERTY_FUNCTION,
	UDELEGATEPROPERTY_NAME,
	UBYTEPROPERTY_ENUM,
	UBOOLPROPERTY_BITMASK,
	UARRAYPROPERTY_INNTER
};

// Property Types
enum class EPropertyTypes : uint16_t
{
	TYPE_UNKNOWN,
	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_INT64,
	TYPE_UINT8,
	TYPE_UINT16,
	TYPE_UINT32,
	TYPE_UINT64,
	TYPE_LONG,
	TYPE_ULONG,
	TYPE_DOUBLE,
	TYPE_FLOAT,
	TYPE_BOOL,
	TYPE_TARRAY,
	TYPE_TMAP,
	TYPE_FNAMEENTRY,
	TYPE_FNAME,
	TYPE_FSTRING,
	TYPE_FSCRIPTDELEGATE,
	TYPE_FPOINTER,
	TYPE_FSTRUCT,
	TYPE_UPOINTER
};

// Width Types
enum class EWidthTypes : uint64_t
{
	WIDTH_NONE = 0,
	WIDTH_BYTE = 2,
	WIDTH_SIZE = 4,
	WIDTH_BITMASK = 8,
	WIDTH_FUNCTION = 8,
	WIDTH_FIELD = 14,
	WIDTH_PROPERTY = 16
};

/*
# ========================================================================================= #
# Structs
# ========================================================================================= #
*/

template<typename TArray>
class TIterator
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

public:
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
class TArray
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
	TArray() : ArrayData(nullptr), ArrayCount(0), ArrayMax(0)
	{
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
		return ArrayData[index];
	}

	ElementReference operator[](int32_t index)
	{
		return ArrayData[index];
	}

	ElementConstReference At(int32_t index) const
	{
		return ArrayData[index];
	}

	ElementReference At(int32_t index)
	{
		return ArrayData[index];
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

// THIS CLASS CAN BE GAME SPECIFIC, MOST GAMES WILL GENERATE A STRUCT MIRROR; JUST PASTE THE FIELDS HERE IF SO.
template<typename TKey, typename TValue>
class TMap
{
private:
	class TPair
	{
	public:
		TKey Key;
		TValue Value;
		int32_t* HashNext;
	};

public:
	using ElementType = TPair;
	using ElementPointer = ElementType*;
	using ElementReference = ElementType&;
	using ElementConstReference = const ElementType&;
	using Iterator = TIterator<TMap<TKey, TValue>>;

public:
	ElementPointer ElementData;				// 0x0000 (0x0008)
	int32_t ElementCount;					// 0x0008 (0x0004)
	int32_t ElementMax;						// 0x000C (0x0004)
	uintptr_t IndirectData;					// 0x0010 (0x0008)
	int32_t InlineData[0x4];                // 0x0018 (0x0010)
	int32_t NumBits;                        // 0x0028 (0x0004)
	int32_t MaxBits;                        // 0x002C (0x0004)
	int32_t FirstFreeIndex;                 // 0x0030 (0x0004)
	int32_t NumFreeIndices;                 // 0x0034 (0x0004)
	int64_t InlineHash;                     // 0x0038 (0x0008)
	int32_t* Hash;                          // 0x0040 (0x0008)
	int32_t HashCount;                      // 0x0048 (0x0004)

public:
	TMap()
	{
		ElementData = nullptr;
		ElementCount = 0;
		ElementMax = 0;
		IndirectData = NULL;
		NumBits = 0;
		MaxBits = 0;
		FirstFreeIndex = 0;
		NumFreeIndices = 0;
		InlineHash = 0;
		Hash = nullptr;
		HashCount = 0;
	}

	TMap(struct FMap_Mirror& fMap)
	{
		*this = *reinterpret_cast<TMap<TKey, TValue>*>(&fMap);
	}

	~TMap() {}

public:
	ElementConstReference operator[](int32_t index) const
	{
		return ElementData[index];
	}

	ElementReference operator[](int32_t index)
	{
		return ElementData[index];
	}

	const TValue& operator[](const TKey& key) const
	{
		for (int32_t i = 0; i < Num(); i++)
		{
			const TPair& pair = ElementData[i];

			if (pair.Key == key)
			{
				return pair.Value;
			}
		}
	}

	TValue& operator[](const TKey& key)
	{
		for (int32_t i = 0; i < Num(); i++)
		{
			TPair& pair = ElementData[i];

			if (pair.Key == key)
			{
				return pair.Value;
			}
		}
	}

	TMap<TKey, TValue>& operator=(const struct FMap_Mirror& fMap)
	{
		*this = *reinterpret_cast<TMap<TKey, TValue>*>(&fMap);
		return *this;
	}

	ElementConstReference At(int32_t index) const
	{
		return ElementData[index];
	}

	ElementReference At(int32_t index)
	{
		return ElementData[index];
	}

	int32_t Num() const
	{
		return ElementCount;
	}

	int32_t Max() const
	{
		return ElementMax;
	}

	Iterator begin()
	{
		return Iterator(ElementData);
	}

	Iterator end()
	{
		return Iterator(ElementData + ElementCount);
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
# Fields
# ========================================================================================= #
*/

class ClassField
{
public:
	EFieldIds Id;
	std::string Type;
	uintptr_t Offset;
	size_t Size;

public:
	ClassField();
	ClassField(EFieldIds id, size_t size);
	~ClassField();

public:
	ClassField operator=(const ClassField& other);
};

namespace Fields
{
	extern std::vector<std::function<void()>> FieldMethods;
	extern std::map<EFieldIds, std::string> IdToType;
	extern std::map<EFieldIds, ClassField> GlobalFields;

	uintptr_t GetOffset(EFieldIds fieldId);
	std::map<uintptr_t, ClassField> GetOrderedFields(EClassTypes classType, size_t& classSize, size_t& startOffset);
	void AssertField(const ClassField& classField);
}

#define REGISTER_FIELD(type, method, id) static void Reg##method(){ Fields::AssertField(ClassField(id, sizeof(type))); }

/*
# ========================================================================================= #
# Structs
# ========================================================================================= #
*/

// FNameEntry
// (0x0000 - 0x000C)
struct FNameEntry
{
public:
	uint64_t Flags;							// 0x0000 (0x08)
	int32_t Index;							REGISTER_FIELD(int32_t, Index, EFieldIds::FNAMEENTRY_INDEX)						// 0x0008 (0x04)

#ifdef CHARACTER_UTF16
	wchar_t			Name[0x400];			REGISTER_FIELD(wchar_t, Name, EFieldIds::FNAMEENTRY_NAME_UTF16)					// 0x000C (0x00)
#endif
#ifdef CHARACTER_UTF8
	char			Name[0x400];			REGISTER_FIELD(wchar_t, Name, EFieldIds::FNAMEENTRY_NAME_UTF8)					// 0x000C (0x00)
#endif

public:
	FNameEntry() : Flags(0), Index(-1) {}
	~FNameEntry() {}

public:
	int32_t GetIndex() const
	{
		return Index;
	}

#ifdef CHARACTER_UTF16
	std::string ToString() const
	{
		std::wstring ws(Name);
		std::string str(ws.begin(), ws.end());
		return str;
	}
#endif

#ifdef CHARACTER_UTF8
	std::string ToString() const
	{
		return std::string(Name);
	}
#endif

#ifdef CHARACTER_UTF16
	const wchar_t* GetWideName() const
	{
		return Name;
	}
#endif

#ifdef CHARACTER_UTF8
	const char* GetAnsiName() const
	{
		return Name;
	}
#endif
};

// FName
// (0x0000 - 0x0008)
struct FName
{
public:
#ifdef CHARACTER_UTF16
	using ElementType = const wchar_t;
#endif
#ifdef CHARACTER_UTF8
	using ElementType = const char;
#endif
	using ElementPointer = ElementType*;

private:
	int32_t			FNameEntryId;									// 0x0000 (0x04)
	int32_t			InstanceNumber;									// 0x0004 (0x04)

public:
	FName() : FNameEntryId(-1), InstanceNumber(0) {}

	FName(int32_t id) : FNameEntryId(id), InstanceNumber(0) {}

#ifdef CHARACTER_UTF16
	FName(const ElementPointer nameToFind) : FNameEntryId(0), InstanceNumber(0)
	{
		static std::vector<int32_t> nameCache{};

		for (int32_t entryId : nameCache)
		{
			if (Names()->At(entryId))
			{
				if (!wcscmp(Names()->At(entryId)->Name, nameToFind))
				{
					FNameEntryId = entryId;
					return;
				}
			}
		}

		for (int32_t i = 0; i < Names()->Num(); i++)
		{
			if (Names()->At(i))
			{
				if (!wcscmp(Names()->At(i)->Name, nameToFind))
				{
					nameCache.push_back(i);
					FNameEntryId = i;
				}
			}
		}
	}
#endif

#ifdef CHARACTER_UTF8
	FName(ElementPointer nameToFind) : FNameEntryId(0), InstanceNumber(0)
	{
		static std::vector<int32_t> nameCache{};

		for (int32_t entryId : nameCache)
		{
			if (Names()->At(entryId))
			{
				if (!strcmp(Names()->At(entryId)->Name, nameToFind))
				{
					FNameEntryId = entryId;
					return;
				}
			}
		}

		for (int32_t i = 0; i < Names()->Num(); i++)
		{
			if (Names()->At(i))
			{
				if (!strcmp(Names()->At(i)->Name, nameToFind))
				{
					nameCache.push_back(i);
					FNameEntryId = i;
				}
			}
		}
	}
#endif

	~FName() {}

public:
	static class TArray<struct FNameEntry*>* Names()
	{
		TArray<FNameEntry*>* GNamesArray = reinterpret_cast<TArray<FNameEntry*>*>(GNames);
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

		return FNameEntry();
	}

	struct FNameEntry* GetEntry()
	{
		if (IsValid())
		{
			return Names()->At(FNameEntryId);
		}

		return nullptr;
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
			return GetDisplayNameEntry().ToString();
		}

		return std::string("UnknownName");
	}

	bool IsValid() const
	{
		if (FNameEntryId < 0 || FNameEntryId > Names()->Num())
		{
			return false;
		}

		return true;
	}

public:
	struct FName& operator=(const struct FName& other)
	{
		FNameEntryId = other.FNameEntryId;
		InstanceNumber = other.InstanceNumber;
		return *this;
	}

	bool operator==(const struct FName& other) const
	{
		return (FNameEntryId == other.FNameEntryId);
	}

	bool operator!=(const struct FName& other) const
	{
		return (FNameEntryId != other.FNameEntryId);
	}
};

// FString
// (0x0000 - 0x0010)
class FString
{
public:
#ifdef CHARACTER_UTF16
	using ElementType = const wchar_t;
#endif
#ifdef CHARACTER_UTF8
	using ElementType = const char;
#endif
	using ElementPointer = ElementType*;

private:
	ElementPointer	ArrayData;										// 0x0000 (0x08)
	int32_t			ArrayCount;										// 0x0008 (0x04)
	int32_t			ArrayMax;										// 0x000C (0x04)

public:
	FString() : ArrayData(nullptr), ArrayCount(0), ArrayMax(0) {}

#ifdef CHARACTER_UTF16
	FString(ElementPointer other) : ArrayData(nullptr), ArrayCount(0), ArrayMax(0)
	{
		ArrayMax = ArrayCount = *other ? (wcslen(other) + 1) : 0;

		if (ArrayCount > 0)
		{
			ArrayData = other;
		}
	}
#endif

#ifdef CHARACTER_UTF8
	FString(ElementPointer other) : ArrayData(nullptr), ArrayCount(0), ArrayMax(0)
	{
		ArrayMax = ArrayCount = *other ? (strlen(other) + 1) : 0;

		if (ArrayCount > 0)
		{
			ArrayData = other;
		}
	}
#endif

	~FString() {}

public:
#ifdef CHARACTER_UTF16
	std::string ToString() const
	{
		if (IsValid())
		{
			std::wstring wideStr(ArrayData);
			std::string str(wideStr.begin(), wideStr.end());
			return str;
		}

		return "null";
	}
#endif

#ifdef CHARACTER_UTF8
	std::string ToString() const
	{
		if (IsValid())
		{
			return std::string(ArrayData);
		}

		return "null";
	}
#endif

	bool IsValid() const
	{
		return (!!ArrayData);
	}

#ifdef CHARACTER_UTF16
	class FString& operator=(ElementPointer other)
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
#endif

#ifdef CHARACTER_UTF8
	class FString& operator=(ElementPointer other)
	{
		if (ArrayData != other)
		{
			ArrayMax = ArrayCount = *other ? (strlen(other) + 1) : 0;

			if (ArrayCount > 0)
			{
				ArrayData = other;
			}
		}

		return *this;
	}
#endif

public:
#ifdef CHARACTER_UTF16
	bool operator==(const FString& other)
	{
		return (!wcscmp(ArrayData, other.ArrayData));
	}

	bool operator!=(const FString& other)
	{
		return (wcscmp(ArrayData, other.ArrayData));
	}
#endif

#ifdef CHARACTER_UTF8
	bool operator==(const FString& other)
	{
		return (!strcmp(ArrayData, other.ArrayData));
	}

	bool operator!=(const FString& other)
	{
		return (strcmp(ArrayData, other.ArrayData));
	}
#endif
};

// FScriptDelegate [This struct is game dependent, don't forget to reverse its contents or just its size!]
// (0x0000 - 0x0009)
struct FScriptDelegate
{
	class UObject* Object;				// 0x0000 (0x08)
	uint8_t UnknownData[0x1];			// 0x0008 (0x01)
};

struct FPointer
{
	uintptr_t Dummy;
};

struct FQWord
{
	int32_t	A;
	int32_t B;
};

/*
# ========================================================================================= #
# Classes
# ========================================================================================= #
*/

// Class Core.Object
// (0x0000 - 0x0034)
class UObject
{
public:
	struct FPointer VfTableObject;		REGISTER_FIELD(FPointer, VfTableObject, EFieldIds::UOBJECT_VFTABLE)				// 0x0000 (0x08)
	uint8_t UnknownData00[0x10];		// Example of padding, you do not need to register this because offsets are all automatically calculated.
	int32_t ObjectInternalInteger;		REGISTER_FIELD(int32_t, ObjectInternalInteger, EFieldIds::UOBJECT_INDEX)		// 0x0018 (0x04)
	class UObject* Outer;				REGISTER_FIELD(class UObject*, Outer, EFieldIds::UOBJECT_OUTER)					// 0x001C (0x08)
	struct FName Name;					REGISTER_FIELD(FName, Name, EFieldIds::UOBJECT_NAME)							// 0x0024 (0x08)
	class UClass* Class;				REGISTER_FIELD(class UClass*, Class, EFieldIds::UOBJECT_CLASS)					// 0x002C (0x08)

public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;

		if (!uClassPointer)
		{
			uClassPointer = UObject::FindClass("Class Core.Object");
		}

		return uClassPointer;
	}

	static TArray<class UObject*>* GObjObjects();

	std::string GetName();
	std::string GetNameCPP();
	std::string GetFullName();
	std::string GetPackageName();
	class UObject* GetPackageObj();
	template<typename T> static T* FindObject(const std::string& objectFullName)
	{
		for (UObject* uObject : *UObject::GObjObjects())
		{
			if (uObject && uObject->IsA(T::StaticClass()))
			{
				if (uObject->GetFullName() == objectFullName)
				{
					return static_cast<T*>(uObject);
				}
			}
		}

		return nullptr;
	}
	template<typename T> static uint32_t CountObject(const std::string& objectName)
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
	template<typename T> bool IsA()
	{
		if (std::is_base_of<UObject, T>::value)
		{
			return IsA(T::StaticClass());
		}
	}
	bool IsA(class UClass* uClass);
	bool IsA(int32_t objInternalInteger);
};

 //Class Core.Field
// 0x0010 (0x0034 - 0x0044)
class UField : public UObject
{
public:
	class UField* Next;					REGISTER_FIELD(class UField*, Next, EFieldIds::UFIELD_NEXT)						// 0x0034 (0x08)
	class UField* SuperField;			REGISTER_FIELD(class UField*, SuperField, EFieldIds::UFIELD_SUPERFIELD)			// 0x003C (0x08) [SUPERFIELD CAN EITHER BE HERE, OR IN USTRUCT DPENDING ON THE GAME. COMMENT OUT ACCORDINGLY!]

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
// 0x0010 (0x0044 - 0x0054)
class UEnum : public UField
{
public:
	TArray<struct FName> Names;			REGISTER_FIELD(TArray<struct FName>, Names, EFieldIds::UENUM_NAMES)				// 0x0044 (0x10)

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
// 0x0010 (0x0044 - 0x0054)
class UConst : public UField
{
public:
	class FString Value;				REGISTER_FIELD(FString, Value, EFieldIds::UCONST_VALUE)							// 0x0044 (0x10)

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
// 0x0014 (0x0044 - 0x0058)
class UProperty : public UField
{
public:
	unsigned long ArrayDim;				REGISTER_FIELD(unsigned long, ArrayDim, EFieldIds::UPROPERTY_DIMENSION)			// 0x0044 (0x04)
	unsigned long ElementSize;			REGISTER_FIELD(unsigned long, ElementSize, EFieldIds::UPROPERTY_SIZE)			// 0x0048 (0x04)
	uint64_t PropertyFlags;				REGISTER_FIELD(unsigned long, PropertyFlags, EFieldIds::UPROPERTY_FLAGS)		// 0x004C (0x08)
	unsigned long Offset;				REGISTER_FIELD(unsigned long, Offset, EFieldIds::UPROPERTY_OFFSET)				// 0x0054 (0x04)

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
// 0x0014 (0x0044 - 0x0058)
class UStruct : public UField
{
public:
	//class UField* SuperField;			REGISTER_FIELD(class UField*, SuperField, EFieldIds::USTRUCT_SUPERFIELD)		// 0x0044 (0x08) [SUPERFIELD CAN EITHER BE HERE, OR IN UFIELD DPENDING ON THE GAME. COMMENT OUT ACCORDINGLY!]
	class UField* Children;				REGISTER_FIELD(class UField*, Children, EFieldIds::USTRUCT_CHILDREN)			// 0x004C (0x08)
	unsigned long PropertySize;			REGISTER_FIELD(unsigned long, PropertySize, EFieldIds::USTRUCT_SIZE)			// 0x0054 (0x04)

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
// 0x000A (0x0058 - 0x0062)
class UFunction : public UStruct
{
public:
	uint64_t FunctionFlags;				REGISTER_FIELD(uint64_t, FunctionFlags, EFieldIds::UFUNCTION_FLAGS)				// 0x0058 (0x08)
	uint16_t iNative;					REGISTER_FIELD(uint16_t, iNative, EFieldIds::UFUNCTION_NATIVE)					// 0x0060 (0x02)

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
// 0x0001 (0x0058 - 0x0059)
class UScriptStruct : public UStruct
{
public:
	uint8_t UnknownData00[0x01]; // 0x0058 (0x01) [USE THIS CLASSES PROPERTYSIZE IN RECLASS TO DETERMINE THE SIZE OF THE UNKNOWNDATA]
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
// 0x0001 (0x0058 - 0x0059)
class UState : public UStruct
{
public:
	uint8_t UnknownData00[0x01]; // 0x0058 (0x01) [USE THIS CLASSES PROPERTYSIZE IN RECLASS TO DETERMINE THE SIZE OF THE UNKNOWNDATA]
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
// 0x0001 (0x0058 - 0x0059)
class UClass : public UState
{
public:
	uint8_t UnknownData00[0x01]; // 0x0058 (0x00) [USE THIS CLASSES PROPERTYSIZE IN RECLASS TO DETERMINE THE SIZE OF THE UNKNOWNDATA]
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
// 0x0008 (0x0058 - 0x0060)
class UStructProperty : public UProperty
{
public:
	class UStruct* Struct;				REGISTER_FIELD(class UStruct*, Struct, EFieldIds::USTRUCTPROPERTY_STRUCT)			// 0x0058 (0x08)

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
// 0x0000 (0x0058 - 0x0058)
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
// 0x0000 (0x0058 - 0x0058)
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
// 0x0008 (0x0058 - 0x0060)
class UObjectProperty : public UProperty
{
public:
	class UClass* PropertyClass;		REGISTER_FIELD(class UClass*, PropertyClass, EFieldIds::UOBJECTPROPERTY_PROPERTY)	// 0x0058 (0x08)

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
// 0x0000 (0x0058 - 0x0058)
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
// 0x0008 (0x0058 - 0x0060)
class UClassProperty : public UObjectProperty
{
public:
	class UClass* MetaClass;			REGISTER_FIELD(class UClass*, MetaClass, EFieldIds::UCLASSPROPERTY_METACLASS)		// 0x0058 (0x08)

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
// 0x0000 (0x0058 - 0x0000)
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
// 0x0010 (0x0058 - 0x0068)
class UMapProperty : public UProperty
{
public:
	class UProperty* Key;				REGISTER_FIELD(class UProperty*, Key, EFieldIds::UMAPPROPERTY_KEY)					// 0x0058 (0x08)
	class UProperty* Value;				REGISTER_FIELD(class UProperty*, Value, EFieldIds::UMAPPROPERTY_VALUE)				// 0x0060 (0x08)

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
// 0x0000 (0x0058 - 0x0058)
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
// 0x0008 (0x0058 - 0x0060)
class UInterfaceProperty : public UProperty
{
public:
	class UClass* InterfaceClass;		REGISTER_FIELD(class UClass*, InterfaceClass, EFieldIds::UINTERFACEPROPERTY_CLASS)	// 0x0058 (0x08)

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
// 0x0000 (0x0058 - 0x0058)
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
// 0x0010 (0x0058 - 0x0068)
class UDelegateProperty : public UProperty
{
public:
	class UFuncton* Function;			REGISTER_FIELD(class UFuncton*, Function, EFieldIds::UDELEGATEPROPERTY_FUNCTION)		// 0x0058 (0x08)
	struct FName DelegateName;			REGISTER_FIELD(FName, DelegateName, EFieldIds::UDELEGATEPROPERTY_NAME)					// 0x0060 (0x08)

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
// 0x0008 (0x0058 - 0x0060)
class UByteProperty : public UProperty
{
public:
	class UEnum* Enum;					REGISTER_FIELD(class UEnum*, Enum, EFieldIds::UBYTEPROPERTY_ENUM)						// 0x0058 (0x08)

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
// 0x0008 (0x0058 - 0x0060)
class UBoolProperty : public UProperty
{
public:
	uint64_t BitMask;					REGISTER_FIELD(uint64_t, BitMask, EFieldIds::UBOOLPROPERTY_BITMASK)						// 0x0058 (0x08)

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
// 0x0008 (0x0058 - 0x0060)
class UArrayProperty : public UProperty
{
public:
	class UProperty* Inner;				REGISTER_FIELD(class UProperty*, Inner, EFieldIds::UARRAYPROPERTY_INNTER)				// 0x0058 (0x08)

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