#include "Configuration.hpp"

/*
# ========================================================================================= #
# Configuration
# ========================================================================================= #
*/

namespace Configuration
{
    const bool UsingConstants =                         false; // If you want to use constant variables for everything, instead of FindClass (very efficient, but need to generate new sdk everytime the game updates),
    const bool UsingOffsets =                           false; // If you're using offsets or FindPattern to find GObjects & GNames,
    const bool UsingDetours =                           true; // If you're goning to be detouring/using the VfTable for process event change this to true, if you're using virtual voids change this to false,
    const bool UsingEnumClasses =                       true; // If you want to use enum classes over just normal enums.
    const std::string EnumClassType =                   "uint8_t"; // Underlying field type if you set UsingEnumClasses to true.

    const int32_t CommentSpacer =                       30;
    const int32_t ConstSpacer =                         50;
    const int32_t EnumSpacer =                          50;
    const int32_t StructSpacer =                        50;
    const int32_t FunctionSpacer =                      50;
    const int32_t ClassSpacer =                         50;
    const EAlignment Alignment =                        EAlignment::X64BIT;

    const int32_t ProcessEventIndex =                   0; // Position where the Process Event function is in UObject's VfTable.
    const std::string ProcessEventString =              "null";
    const uint8_t* ProcessEventPattern =                (uint8_t*)"null";
    const char* ProcessEventMask =                      (char*)"null";

    const std::string GObjectsString =                  "null";
    const uint8_t* GObjectsPattern =                    (uint8_t*)"null";
    const char* GObjectsMask =                          (char*)"null";
    const uintptr_t GObjectsOffset =                    (uintptr_t)0x0;

    const std::string GNamesString =                    "null";
    const uint8_t* GNamesPattern =                      (uint8_t*)"null";
    const char* GNamesMask =                            (char*)"null";
    const uintptr_t GNamesOffset =                      (uintptr_t)0x0;

    const std::string GameName =                        "Template";
    const std::string GameNameShort =                   "TSDK";
    const std::string GameVersion =                     "1.0.0.0";
    const std::filesystem::path GeneratorDirectory =    "E:\\Steam\\steamapps\\common\\Template\\UE3SDKGenerator";
}

/*
# ========================================================================================= #
#
# ========================================================================================= #
*/