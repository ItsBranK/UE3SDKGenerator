#pragma once
#include <string>
#include <filesystem>

/*
# ========================================================================================= #
# Configuration
# ========================================================================================= #
*/

// Comment & uncomment these depending on what your game uses!
#define CHARACTER_UTF16
//#define CHARACTER_UTF8
//#define NO_LOGGING // Uncomment if you want to disable logging to the log file.

// This CAN be customized to anything you want really, just remember "FinalAlignment" only applies to the generated sdk.
enum class EAlignment : int32_t
{
	NONE = 0x1,
	X32BIT = 0x4,
	X64BIT = 0x8
};

// These are global variables below, make changes in the cpp file only!

namespace Configuration
{
	extern const bool UsingConstants;
	extern const bool UsingOffsets;
	extern const bool UsingDetours;
	extern const bool UsingEnumClasses;
	extern const bool PrintFlagEnums;
	extern const bool RemoveNativeFlags;
	extern const std::string EnumClassType;

	extern const int32_t CommentSpacer;
	extern const int32_t ConstSpacer;
	extern const int32_t EnumSpacer;
	extern const int32_t StructSpacer;
	extern const int32_t FunctionSpacer;
	extern const int32_t ClassSpacer;
	extern const EAlignment Alignment;
	extern const int32_t FinalAlignment;

	extern const int32_t ProcessEventIndex;
	extern const std::string ProcessEventString;
	extern const uint8_t* ProcessEventPattern;
	extern const char* ProcessEventMask;

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
	extern const std::filesystem::path GeneratorDirectory;
}

/*
# ========================================================================================= #
#
# ========================================================================================= #
*/