#pragma once
#include "GameDefines.h"

/*
# ========================================================================================= #
# Object Functions																			#
# ========================================================================================= #
*/

TArray<class UObject*>* UObject::GObjObjects()
{
	TArray<class UObject*>* objectArray = reinterpret_cast<TArray<class UObject*>*>(GObjects);
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
		UClass* uClass = reinterpret_cast<UClass*>(this);

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

			uClass = reinterpret_cast<UClass*>(uClass->SuperField);
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
	if (this->Class && this->Outer)
	{
		std::string fullName = this->GetName();

		for (UObject* uOuter = this->Outer; uOuter; uOuter = uOuter->Outer)
		{
			fullName = uOuter->GetName() + "." + fullName;
		}

		fullName = this->Class->GetName() + " " + fullName;

		return fullName;
	}

	return "null";
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

template<typename T>
T* UObject::FindObject(const std::string& objectFullName)
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
static uint32_t UObject::CountObject(const std::string& objectName)
{
	static std::map<std::string, int> countCache;

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

UClass* UObject::FindClass(const std::string& classFullName)
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
					foundClasses[objectFullName] = reinterpret_cast<UClass*>(uObject);
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

bool UObject::IsA(UClass* uClass)
{
	for (UClass* uSuperClass = this->Class; uSuperClass; uSuperClass = reinterpret_cast<UClass*>(uSuperClass->SuperField))
	{
		if (uSuperClass == uClass)
		{
			return true;
		}
	}

	return false;
}

bool UObject::IsA(int objInternalInteger)
{
	UClass* uClass = UObject::GObjObjects()->At(objInternalInteger)->Class;

	if (uClass)
	{
		return this->IsA(uClass);
	}

	return false;
}

UFunction* UFunction::FindFunction(const std::string& functionFullName)
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
					foundFunctions[objectFullName] = reinterpret_cast<UFunction*>(uObject);
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