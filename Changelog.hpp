#pragma once

/*
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