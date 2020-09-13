![LevelMod](levelmod_logo.png)

# LevelMod Full Source


###BIG CHANGES###

LevelMod.flt is now deprecated. Now you only need d3d8.dll and you should remove all instances of levelmod.flt including copies. If game finds flt file in same folder as skate3.exe or data/MILES, it will try to load it and since it's already loaded inside d3d8.dll it will crash.

Also Skate3.exe is no longer needed, because d3d8.dll is loaded earlier so I change everything in memory instead.

1. Include Extension.h and add the d3d8.lib to your project.

2. If you use VS you need to set USE MULTI BYTE CHARSET.

3. Set Export target to .flt instead of .dll and add your flt file to data/MILES folder.

4. Remember to add the scripts and the latest built d3d8.dll to not make your game crash.




# List of All Functions that you can call from Script
* **SomeNotes** When you call a function that will add a value to your params that you can access with GLOBAL
* You need to have the value there in the params else it will not work
* **ChangeString** Example: `ChangeString string = StringToChange param = TherNewString` always returns true
* **strstr** Example: `strstr s1 = "Here is a string" s2 = "string"` returns true if s1 contains s2
* **GetZAngle** angle is stored in `GLOBAL.angle` and `GLOBAL.angle` need to exist before calling **GetZAngle** always returns true
* **GetSpeed** same as above but stored in `GLOBAL.speed` always returns true
* **GetSkaterPos** same as above but stored in `GLOBAL.pos` always returns true
* **GetSkaterLook** same as above but stored in `GLOBAL.pos` always returns true
* **CreatePair** Example: `CreatePair Name = "NameToStore" x = 1.0 y = 2.0` acces it with `GLOBAL.NameToStore`
* **TestForAcid** depricated
* **Wallplant** depricated
* **PreWallplant** depricated
* **EnterObserveMode2** Enter custom observe mode always returns true
* **LeaveObserveMode2** Leave custom observe mode always returns true
* **ObserveNext** Observe next player always returns true
* **StoreSkaterPos** Stores skaters position always returns true
* **GotoStoredPos** Goto the stored position always returns true
* **Not** Example: `IF Not IsOptionOn` will call CFunction IsOptionOn and return !
* **SubToGlobal** used in thps4+ levels always returns true
* **AddToGlobal** used in thps4+ levels always returns true
* **FreezeCamera** Freezes the camera always returns true
* **UnfreezeCamera** Unfreeze the camera always returns true
* **GrafStarted** Called from scripts when Graffiti is started 
* so client can check if server has enabled unlimited tags
* **ChangeValues** Example: `ChangeValues Container = AStruct values = BStruct` 
* Sets values from BStruct to AStruct returns true if find params, else false
* **GetSliderValue** Example: `GetSliderValue id = element_id ||name = NameToStore||` 
* acces it with `GLOBAL.NameToStore` if no name is passed acces with `GLOBAL.value` returns true on sucess
* **InitLevelMod** Inits levelmod stuff
* **MoveObject** Move an EnvironmentObject
* **KillMovingObject** Stop Moving an EnvironmentObject
* **ChangeParamToUnnamed**
* **IsOptionOn** check if an option is on, remember to add the option to the list first
* **AddOption** adds an option to the list, all options gets stored inside levelmod.ini use value = [number] to set a default value
* **ToggleOption** toggle an option
* **LM_GotParam** usage same as GotParam, but checks inside arrays and structs
* **GetParam** Moves a param from array/struct into the local stack



d3d8to9
=======

[![Build Status](https://ci.appveyor.com/api/projects/status/aqupdda60ixgenyd?svg=true)](https://ci.appveyor.com/project/crosire/d3d8to9)

This is a pseudo-driver module that intends to improve compatibility and stability in games using Direct3D 8 for rendering by converting all API calls and low-level shaders to equivalent Direct3D 9 ones. By that it also opens those games to new possibilities from proven modding tools written for Direct3D 9, including [ReShade](http://reshade.me).

## Building

You'll need Visual Studio 2013 or higher to build d3d8to9. It is recommended to install the old standalone DirectX end-user runtime, which is required for the D3DX libraries used for disassembling and assembling the shaders.

A quick overview of what some of the source code files contain:

|File                                                      |Description                                                                      |
|----------------------------------------------------------|---------------------------------------------------------------------------------|
|[d3d8to9.cpp](source/d3d8to9.cpp)                         | Definition of the main D3D8 entry point `Direct3DCreate8`                       |
|[d3d8to9_base.cpp](source/d3d8to9_base.cpp)               | Implementation of the `IDirect3D8` interface, including device creation         |
|[d3d8to9_device.cpp](source/d3d8to9_device.cpp)           | Implementation of the `IDirect3DDevice8` interface, including shader conversion |
|[d3d8types.hpp](source/d3d8types.hpp)                     | Declaration of all used D3D8 types one would otherwise find in d3d8.h           |
|[lookup_table.hpp](source/lookup_table.hpp)               | Table to map D3D9 interface addresses to their matching D3D8 implementations    |

## Contributing

Any contributions to the project are welcomed, it's recommended to use GitHub [pull requests](https://help.github.com/articles/using-pull-requests/).

A big shout-out to all the existing [contributors](https://github.com/crosire/d3d8to9/graphs/contributors) who worked on improving compatibility, especially [elishacloud](https://github.com/elishacloud)!

## License

All source code in this repository is licensed under a [BSD 2-clause license](LICENSE.md).
