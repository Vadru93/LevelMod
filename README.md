![LevelMod](levelmod_logo.png)

**LevelMod** is a global modification for the game **Tony Hawk's Pro Skater 3**. The original mod was in development in between 2011 and 2013. Its main goal was to provide the hardcore THPS3 community at chapter-3.net with new levels from other games to play on and it was the first mod to feature a properly playable ports.

The new iteration of the mod is a hobby project for die hard fans of THPS3. It aims to accumulate all the fixes the community has created for the game over the years or even only wished for in a single mod. It also aims to be fully customizable, allowing to maintain the feel of the original game by being able to toggle most of the introduced changes.

LevelMod features:
- playable levels from various Tony Hawk's games
- multiple crash & bug fixes
- some optimizations to game engine
- mod settings menu with multiple features to toggle
- trick system overhaul to add features from later games (extra tricks, wallplant/wallieplant, etc).
- integrated patches like 251x or 32 tag counter limit.
- graphics enhancement(extra blendmodes, shaders, mipmaps, AA, etc)

[_Complete list of features_](https://github.com/Vadru93/LevelMod/wiki/Settings-and-Features)

### How to install
**Important!** *The mod requires original game to be installed on your computer. You cannot play the mod without original files. You also need the [1.01 patch](#links)*
1. Make sure you have the latest LevelMod Full release and lost levels installed from [Chapter-3.net thread](#links)
2. Look for Levelmod.flt file and remove it if found
3. Download [latest release from repo](#links)
4. Extract to your game folder and replace all the files if asked. However, for the first install it shouldn't require to replace any existing files
5. Make sure you have [Visual Studio Redist x86 2019](#links) installed
6. For people with enabled UAC, run as admin, or make sure you have write permissions for THPS3 folder and folder is not set to read-only

* *If you want to restore the original game, you can just rename d3d8.dll and the game becomes vanilla untouched. All the mod scripts are now stored in a LevelMod subfolder.*
* *If you want to use latest commit, you can do so by downloading the [LevelMod folder](https://github.com/Vadru93/LevelMod/tree/master/LevelMod) and copy it's content to game folder.*

**Recommended additional downloads**
1. [Widescreen patch by thirteenAG](#links) - adds custom resolutions and proper widescreen support
2. [X360CE For DirectInput gamepads](#links) - converts DirectInput calls to XInput, ensuring proper L2-R2 separation

### DLL code changes
Additional code used to be loaded in LevelMod.flt by integrated MSS plugin system, but this method is deprecated now. Now you only need d3d8.dll, hence you must remove all instances of legacy levelmod.flt, including copies/backups. If the game finds flt file in same folder as skate3.exe or data/MILES, it will try to load it and since it's already loaded inside d3d8.dll it will crash the game. This also means you don't need a custom game executable anymore, since all the patches were moved to d3d8.dll.

**DLL Compilation notes**
1. Include Extension.h and add the d3d8.lib to your project.
2. If you use VS you need to set USE MULTI BYTE CHARSET.
3. Set Export target to .flt instead of .dll and add your flt file to data/MILES folder.
4. Remember to add the scripts and the latest built d3d8.dll to not make your game crash.

### Links
* [Legacy THPS Discord](https://discord.gg/vTWucHS)
* [Latest LevelMod release](https://github.com/Vadru93/LevelMod/releases/latest)
* [LevelMod wiki](https://github.com/Vadru93/LevelMod/wiki)
* [Original LevelMod thread at Chapter-3.net](http://chapter-3.net/thps3/v2/showthread.php?tid=3141)
* [Visual Studio Redist x86 2019](https://aka.ms/vs/16/release/vc_redist.x86.exe)
* [Official 1.01 patch](https://community.pcgamingwiki.com/files/file/339-tony-hawks-pro-skater-3-patch/)
* [THPS3 Widescreen patch by thirteenAG](https://github.com/ThirteenAG/WidescreenFixesPack/releases/tag/thps3)
* [X360CE For DirectInput gamepads](https://github.com/x360ce/x360ce/releases/latest)

2011-2021, DCxDemo* and Vadru'Qc.
