#include "Configuration.h"

namespace Configuration
{
    bool UsingConstants = false; // If you want to use constant variables for everything, instead of FindClass (very efficient, but need to generate new sdk everytime the game updates)
    bool UsingOffsets = false; // If you are using offsets or FindPattern to find GObjects & GNames
    bool UsingDetours = true; // If you're goning to be detouring/using the VfTable for process event change this to true, if you're using virtual voids change this to false
    bool UsingEnumClasses = true; // If you want to use enum classes over just normal enums

    int CommentSpacer = 30;
    int ConstSpacer = 50;
    int EnumSpacer = 50;
    int StructSpacer = 50;
    int FunctionSpacer = 50;
    int ClassSpacer = 50;
    const unsigned char Alignment = 0x8; // 0x4 for x32 bit, 0x8 for x64 bit

    int ProcessEventIndex = 67;
    std::string ProcessEventString = "null";
    const unsigned char* ProcessEventPattern = (const unsigned char*)"null";
    const char* ProcessEventMask = (const char*)"null";
    uintptr_t ProcessEventOffset = (uintptr_t)0x0;

    std::string GObjectsString = "null";
    const unsigned char* GObjectsPattern = (const unsigned char*)"null";
    const char* GObjectsMask = (const char*)"null";
    uintptr_t GObjectsOffset = (uintptr_t)0x0;

    std::string GNamesString = "null";
    const unsigned char* GNamesPattern = (const unsigned char*)"null";
    const char* GNamesMask = (const char*)"null";
    uintptr_t GNamesOffset = (uintptr_t)0x0;

    std::string GameName = "Games Full Name";
    std::string GameNameShort = "GAMESDK";
    std::string GameVersion = "1.0.0";
    std::string GeneratorVersion = "v2.0.9";
    std::string GeneratorCredits = "TheFeckless, ItsBranK";
    std::string GeneratorLinks = "www.github.com/ItsBranK/UE3SDKGenerator, www.twitter.com/ItsBranK";
    std::string GeneratorDirectory = "UE3SDKGenerator";
}