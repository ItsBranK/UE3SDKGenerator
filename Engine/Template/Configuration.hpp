#pragma once
#include <string>

/*
# ========================================================================================= #
# Configuration
# ========================================================================================= #
*/

// These are global variables, make changes in the cpp file only!

enum class EAlignment : uint8_t
{
	X32BIT = 0x4,
	X64BIT = 0x8
};

namespace Configuration
{
	extern const bool UsingConstants;
	extern const bool UsingOffsets;
	extern const bool UsingDetours;
	extern const bool UsingEnumClasses;

	extern const int32_t CommentSpacer;
	extern const int32_t ConstSpacer;
	extern const int32_t EnumSpacer;
	extern const int32_t StructSpacer;
	extern const int32_t FunctionSpacer;
	extern const int32_t ClassSpacer;
	extern const EAlignment Alignment;

	extern const int32_t ProcessEventIndex;
	extern const std::string ProcessEventString;
	extern const uint8_t* ProcessEventPattern;
	extern const char* ProcessEventMask;
	extern const uintptr_t ProcessEventOffset;

	extern const std::string GObjectsString;
	extern const uint8_t* GObjectsPattern;
	extern const char* GObjectsMask;
	extern const uintptr_t GObjectsOffset;

	extern const std::string GNamesString;
	extern const uint8_t* GNamesPattern;
	extern const char* GNamesMask;
	extern const uintptr_t GNamesOffset;

	extern const std::string GameName;
	extern const std::string GameNameShort;
	extern const std::string GameVersion;
}