#pragma once
#include <string>

// These are global variables, make changes in the cpp file only!

namespace Configuration
{
	extern bool UsingConstants;
	extern bool UsingOffsets;
	extern bool UsingDetours;
	extern bool UsingEnumClasses;

	extern int CommentSpacer;
	extern int ConstSpacer;
	extern int EnumSpacer;
	extern int StructSpacer;
	extern int FunctionSpacer;
	extern int ClassSpacer;
	extern const unsigned char Alignment;

	extern int ProcessEventIndex;
	extern std::string ProcessEventString;
	extern const unsigned char* ProcessEventPattern;
	extern const char* ProcessEventMask;
	extern uintptr_t ProcessEventOffset;

	extern std::string GObjectsString;
	extern const unsigned char* GObjectsPattern;
	extern const char* GObjectsMask;
	extern uintptr_t GObjectsOffset;

	extern std::string GNamesString;
	extern const unsigned char* GNamesPattern;
	extern const char* GNamesMask;
	extern uintptr_t GNamesOffset;

	extern std::string GameName;
	extern std::string GameNameShort;
	extern std::string GameVersion;
	extern std::string GeneratorVersion;
	extern std::string GeneratorCredits;
	extern std::string GeneratorLinks;
	extern std::string GeneratorDirectory;
}