#include "Configuration.hpp"

/*
# ========================================================================================= #
# Configuration
# ========================================================================================= #
*/

namespace Configuration
{
    const bool UsingConstants = false; // If you want to use constant variables for everything, instead of FindClass (very efficient, but need to generate new sdk everytime the game updates)
    const bool UsingOffsets = false; // If you are using offsets or FindPattern to find GObjects & GNames
    const bool UsingDetours = true; // If you're goning to be detouring/using the VfTable for process event change this to true, if you're using virtual voids change this to false
    const bool UsingEnumClasses = true; // If you want to use enum classes over just normal enums

    const int32_t CommentSpacer = 30;
    const int32_t ConstSpacer = 50;
    const int32_t EnumSpacer = 50;
    const int32_t StructSpacer = 50;
    const int32_t FunctionSpacer = 50;
    const int32_t ClassSpacer = 50;
    const EAlignment Alignment = EAlignment::X64BIT;

    const int32_t ProcessEventIndex = 0;
    const std::string ProcessEventString = "null";
    const uint8_t* ProcessEventPattern = (uint8_t*)"null";
    const char* ProcessEventMask = (char*)"null";
    const uintptr_t ProcessEventOffset = (uintptr_t)0x0;

    const std::string GObjectsString = "null";
    const uint8_t* GObjectsPattern = (uint8_t*)"null";
    const char* GObjectsMask = (char*)"null";
    const uintptr_t GObjectsOffset = (uintptr_t)0x0;

    const std::string GNamesString = "null";
    const uint8_t* GNamesPattern = (uint8_t*)"null";
    const char* GNamesMask = (char*)"null";
    const uintptr_t GNamesOffset = (uintptr_t)0x0;

    const std::string GameName = "Template Name";
    const std::string GameNameShort = "TSDK";
    const std::string GameVersion = "0.0.0";
}