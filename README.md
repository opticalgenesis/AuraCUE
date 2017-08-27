## AuraCUE - RGB SDKs Simplified ##

AuraCUE is a .dll extension, with accompanying .h and .lib files that aims to unify the two released RGB hardware SDKs for ASUS' Aura Sync; and Corsair's CUE.

**THIS PROJECT IS ON HOLD INDEFINITELY UNTIL A REFACTOR IS COMPLETE**

**_Compile from source_**

Installation instructions assume Visual Studio 2017 installed a default directory; with the Projects folder located at %USERPROFILE%\Documents\Visual Studio 2017\Projects. 

Clone the repo with git clone https://github.com/opticalgenesis/AuraCUE in your Projects directory.

Open "AuraCUE.sln" in VS2017. Once opened use "Ctrl+Shift+B" to build the solution. The .dll will be located in "~\Debug", along with the .lib.

**_Basic usage instructions_**

The AuraCUE.dll and CorsairSdk.dll appropriate for your build architecture must be included in the same directory as your program's .exe.

Equally, the AuraCUE.lib file must be added as a reference in your project.

First, obtain a reference to the SDK Functions:

AuraCUE::Functions funcs;

Once the reference has been obtained, run the Initialize(bool, bool) function. true, true will initialise both SDKs; true, false initialises only CUE; and false, true initialises only Aura.

A std::vector of CUE devices can be obtained with funcs.GetCueDevices().
Similarly, funcs.GetAuraDevices() returns all Aura enabled devices.

If you just want a std::vector of ALL RGB enabled devices, use funcs.GetNormalizedDevices().

**_Other information_**

The necessary CUE SDK .dlls and Aura Sync .cpp/.h files are included in "~\RGB SDKs". The .dlls are identical to the ones provided by Corsair. 
Feel free to swap the ones from this project with the official ones, if you don't feel comfortable using the ones provided. The RogAuraService.* files 
are as provided by ASUS, but I have modified them to correct a typo.
