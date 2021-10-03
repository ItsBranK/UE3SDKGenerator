#pragma once

/*
	Changes in v2.2.3:
	- Changed the format for some of the constructors in "GameDefines.hpp" as well in "PiecesOfCode.cpp".
	- Fixed the "IsValid" function for FNames in the generated sdk.

	Changes in v2.2.2:
	- Changed some cast types from reinterpret _cast to safer ones in some places.
	- Fixed the wrong "correctElementSize" value being set if there was an array of objects for generated structs, this is the same fix as the one in v2.1.9 I just forgot to make it apply to script structs.
	- Fixed UInterfaceProperty's being generated wrong in structs, again same fix as the one in v2.1.9 but applied to script structs.
	- Fixed a spacing issue with the includes in the generated "GameDefines.hpp" file as well as in generated script structs.

	Changes in v2.2.1:
	- Added a message box popup to tell the user if generating virtual functions did not work correctly.
	- Fixed functions being static if "FUNC_AllFlags" was set, it's not suppose to be static if that's the case. Thanks tim sweeney.
	- Fixed not properly clearing the iNative bitflags and setting them back for functions.

	Changes in v2.2.0:
	- Fixed the interface name being wrong if there was an array of them.
	- Replaced the "dumpObjects" parameter with just "dumpObjectsFull" internally, because why would you even need that option?
	- Increased the loop size for virtual functions from 256 to 512.

	Changes in v2.1.9:
	- Fixed the UInterfaceProperty being generated wrong in the final sdk, it no longer says wrong size of previous property.
	- Fixed the math for "correctElementSize" being wrong if there was an array of objects, it now takes into account the array dimensions.
	- Fixed the iNative flags being wrong after going through process event.
	- Added support for generating static functions in classes.
	- Added an option to print the flag enums to the final sdk in "Configuration.cpp".
	- Added the "EObjectFlags" enum in GameDefines.
	- Added an extra safety check for validating globals.
	- Changed how the "AreGNamesValid" function works.

	Changes in v2.1.8:
	- Fixed the start offset for UObjectProperty being wrong.
	- Fixed there not being a proper ";" for the generated "FScriptDelegate" struct.
	- Fixed the return string for the "GetName" functions in PiecesOfCode being const by mistake in the teamplate.
	- Fixed the TPair members being private by default in PiecesOfCode in the template.
	- Fixed the generated UObject_Functions being in the wrong order in the generated sdk.
	- Fixed the UDelegateProperty offsets being wrong in the template for the generated sdk.

	Changes in v2.1.7:
	- I forgot to make FNameEntry's fields dynamically generate (whoops), added field register ids for them now in "GameDefines.hpp".
	- Added the GetEntryPoint and GetOffset functions.
	- Added extra "std::filesystem::exists" safety checks, along with more null pointer checks.
	- Added more detailed messagebox information.
	- Added a WIDTH_FIELD size to the EWidthTypes enum.
	- Remade the entire "Printers" namespace along with adding some extra functions.
	- Changed where the validation of globals happens, it has now been moved to the Initialize function.
	- Everything now uses direct std::ofstream's now.
	- Added the offset for globals when dumping instances now.
	- Dumping instances alone no longer creates an "empty" log file.
	- Adjusted how the start and end time are calculated.
	- Adjusted the "PiecesOfCode.cpp" structs to remove some new lines.
	- Organized some namespaces.

	Changes in v2.1.6:
	- Added some safety/null checks for class fields.
	- Changed the AreGObjectsValid check to support a wider range of games.
	- You no longer need to comment out returning the offsetof in "GameDefines.cpp" for the class fields.
 
	(This is a major update for the generator, please read the new information in the README file in the repository as well as the comments/examples in the "Template" folder!)
	Changes in v2.1.5:
	- Remade the "Template" engine classes to include all classes that are required for generation.
	- Added an offset macro for fields in "GameDefines.hpp", this you'll never have to update the fields in "PiecesOfCode.cpp" again! It's now all dynamically generated.
	- Added CHARACTER_UTF16 and CHARACTER_UTF8 defines in "Configuration.cpp", which changes everything on the fly including the generated structs.
	- Added dynamic generation of the FScriptDelegate struct (original class still has to be reversed).
	- Added an option to set the underlying type of enums if UsingEnumClasses is set to true in "Configuration.cpp".
	- Added the TMap class, along with printing it to "GameDefines.hpp" in the generated sdk.
	- Moved the changelog from "dllmain.hpp" to this current file.
	- Moved the GeneratorDirectory field from "Engine.hpp" to "Configuration.hpp" and also made it a std::filesystem::path instead of a string.
	- Turned some structs into classes, like TIterator, TArray, and FString. 

	Changes in v2.1.4:
	- Fixed not correctly setting input values if the parameter was a bitfield.

	Changes in v2.1.3
	- Fixed parameters not correctly being copied if they were BOTH a CPF_Parm and CPF_OutParm.
	- Fixed not properly returning an out parameter they they were BOTH a CPF_Parm and CPF_OutParm.
	- Included some links in "GameDefines.hpp" for the function/enum flags, along with some extra comments.
	- Added some extra saftey checks for structs in "GameDefines.hpp" and fixed some spelling mistakes.
	- Added UDelegateProperty to PiecesOfCode and the generator.

	Changes in v2.1.2:
	- Added "SendMessage" to the MakeWindowsFunction function, and also redid the function itself.

	Changes in v2.1.1:
	- Moved the windows include headers and shit to "pch.hpp".
	- Added PiecesOfCode.cpp and made the ones in the header file externs.
	- Added Engine.hpp and Engine.cpp, generator info will be in here from now on as well as the includes for your engine files.
	- Got rid of ObjectFunctions and moved everything to GameDefines.cpp, this also means I had to move the static template functions in the GameDefines.hpp file.
	- Made the alignment value in Configuration.hpp/cpp an enum.
	- Fixed the version number being wrong in the last update.
	- Renamed h files to hpp cuz SEEE PLUS PLUSS!!!!

	Changes in v2....
	I erased my commit history and sadly I don't have any copies of the changelog from previous versions.
*/