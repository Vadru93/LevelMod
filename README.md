![LevelMod](levelmod_logo.png)

**LevelMod** is a global modification for the game **Tony Hawk's Pro Skater 3**. The original mod was in development in between 2011 and 2013. Its main goal was to provide the hardcore THPS3 community at chapter-3.net with new levels from other games to play on and it was the first mod to feature a properly playable ports.

The new iteration of the mod is a hobby project for die hard fans of THPS3. It aims to accumulate all the fixes the community has created for the game over the years or even only wished for in a single mod. It also aims to be fully customizable, allowing to maintain the feel of the original game by being able to toggle most of the introduced changes.

LevelMod features:
- playable levels from various Tony Hawk's games
- multiple crash fixes
- mod settings menu with multiple features to toggle
- trick system overhaul to add features from later games (extra tricks, wallplant/wallieplant, etc).
- intergrated patches like 251x or 32 tag counter limit.

### How to install
**Important!** The mod requires original game to be installed on your computer. You cannot play the mod without original files.
You also need the 1.01 patch [.Links](#links)
1. Make sure you have the last LevelMod Full release installed from chapter-3.net thread
2. Look for Levelmod.flt file and remove it if found
3. Download latest realease [.Links](#links)
4. Copy to your game folder and replace all the files if asked. However, for the first install it shouldn't replace any existing files
5. Make sure you have Visual Studio Redist x86 2019 [.Links](#links)
6. For people with windows 10, make sure you set full rights to the thps3 folder to(aka remove read-only property) and run game as admin.

If you want for some reason to play the original game, you can just rename d3d8.dll and the game becomes vanilla untouched. All the mod scripts are now stored in a LevelMod subfolder.

If you want to use latest commit, you can do so by downloading d3d8.dll and scripts folder directly from the repo.

### DLL code changes
LevelMod.flt is now deprecated. Now you only need d3d8.dll and you should remove all instances of levelmod.flt including copies. If game finds flt file in same folder as skate3.exe or data/MILES, it will try to load it and since it's already loaded inside d3d8.dll it will crash.

Also Skate3.exe is no longer needed, because d3d8.dll is loaded earlier so I change everything in memory instead.

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

2011-2020, DCxDemo* and Vadru'Qc.
