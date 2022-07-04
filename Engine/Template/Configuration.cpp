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
    const bool PrintFlagEnums =                         true; // If you want the EFunctionFlags, EPropertyFlags, and EObjectFlags enum so be generated in the final sdk.
    const bool RemoveNativeFlags =                      false; // If you want to remove the FUNC_Native flags on functions before going through process event.
    const std::string EnumClassType =                   "uint8_t"; // Underlying field type if you set UsingEnumClasses to true.

    const int32_t CommentSpacer =                       30;
    const int32_t ConstSpacer =                         50;
    const int32_t EnumSpacer =                          50;
    const int32_t StructSpacer =                        50;
    const int32_t FunctionSpacer =                      50;
    const int32_t ClassSpacer =                         50;
    const EAlignment Alignment =                        EAlignment::X32BIT; // If you're using enum classes, it's recommended you set this to EAlignment::NONE unless you know what you're doing!

    // Alignment used in the final generated sdk, this defines class/structs field byte alignment.
    // Keep in mind when using "UsingEnumClasses" enum sizes will be defined by their underlying type, which is dependent on this final alignment!
    const int32_t FinalAlignment =                      0x4;

    const int32_t ProcessEventIndex =                   -1; // Position where the Process Event function is in UObject's VfTable.
    const std::string ProcessEventString =              "null";
    const uint8_t* ProcessEventPattern =                (uint8_t*)"null";
    const char* ProcessEventMask =                      (char*)"null";

    const std::string GObjectsString =                  "null";
    const uint8_t* GObjectsPattern =                    (uint8_t*)"null";
    const char* GObjectsMask =                          (char*)"null";
    const uintptr_t GObjectsOffset =                    NULL;

    const std::string GNamesString =                    "null";
    const uint8_t* GNamesPattern =                      (uint8_t*)"null";
    const char* GNamesMask =                            (char*)"null";
    const uintptr_t GNamesOffset =                      NULL;

    const std::string GameName =                        "Template";
    const std::string GameNameShort =                   "TSDK";
    const std::string GameVersion =                     "1.0.0.0";
    const std::filesystem::path GeneratorDirectory =    "I_FORGOT_TO_SET_A_PATH";
}

/*
# ========================================================================================= #
#
# ========================================================================================= #
*/