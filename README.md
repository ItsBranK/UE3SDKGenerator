# UE3SDKGenerator v2.3.0

### About

A modern C++17 Unreal Engine 3 SDK generator, originally based off of TheFeckless's UE3 SDK Generator.

**This project is now deprecated and is no longer being maintained, this repository now only exists for archival purposes! For an updated and maintained version I have created a [separate project here](https://github.com/CodeRedModding/CodeRed-Generator/)!

### Features

- **Accessibility**
Generated sdk is plug and play, just `#include "SdkHeaders.hpp"` in your project, initialize globals, and you're ready to go.

- **Global offsets**
Generate an sdk using either offsets or patterns for GObjects and GNames.

- **Constant variables**
Have the option to use constant variables for classes and functions instead of FindClass/FindObject.

- **Process Event options**
Have the option of using virtual voids for Process Event, or just use an index number for UObject's VfTable instead.

- **Custom class alignment**
Full support for both x32 bit and x64 bit games, just change the `Alignment` value in `Configuration.cpp`.

- **Enum classes**
Have the option to generate normal enums or enum classes as well as their underlying type.

- **Custom spacer widths**
Customize the spacing for comments, constants, structs, enums, functions, classes, and even the log file.

### Requirements

- ISO C++17 Standard.
- Visual Studio or another Windows based compiler (For Windows header files, along with the PSAPI library).

### Configuration

To get started in generating an sdk, copy  and paste the `Template` folder included in the Engine folder and rename it to the game you would like to use. The `GameDefines.hpp` file will need to be reversed by hand because every game is different, there is no way to automate this process. In `Configuration.hpp` uncomment which character type your game uses, it will either be wide char (UTF16) or const char (UTF8).

![](https://i.imgur.com/gbIfB3R.png)

Hard coded fields in `GameDefines.hpp` are dynamically generated in the final sdk, along with their offsets. In order for the offsets to be correct you must properly "register" the field, the template includes all fields needed for sdk generation; anything other than that you do NOT need to register as long as it's not part of the `EFieldIds` enum.

Here is an example of how to register fields, there are more examples in the `Template` folder, as well as comments throughout to help you if you encounter any errors with the template.

![](https://i.imgur.com/qbTOPWd.png)

Once you have the necessary classes filled out all that's left to do is make the desired changes in the `Configuration.cpp` file (don't forget to set the GeneratorDirectory path) and make sure you have the right files included for your game in `Engine.hpp`. When you inject the compiled dll into your game you will be prompted with a message saying that sdk generation has started, do not close your game until you recieve another message confirming generation is completed.

### Finalization

Once your sdk has been generated you might need to make a minor change to it. Depending on the game the header files in `SdkHeaders.hpp` could be placed out of order, if they are make sure to swap it out in the order of `Core` first, then `Engine`; here is an example:

```cpp
#include "GameDefines.hpp"
#include "SDK_HEADERS\Core_structs.hpp"
#include "SDK_HEADERS\Core_classes.hpp"
#include "SDK_HEADERS\Core_parameters.hpp"
#include "SDK_HEADERS\Engine_structs.hpp"
#include "SDK_HEADERS\Engine_classes.hpp"
#include "SDK_HEADERS\Engine_parameters.hpp"
```
