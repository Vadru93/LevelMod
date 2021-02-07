
SK3SFX_TABLE_WHEELROLL = 0
SK3SFX_TABLE_GRIND = 1
SK3SFX_TABLE_JUMP = 2
SK3SFX_TABLE_LAND = 3
SK3SFX_TABLE_BONK = 4
SK3SFX_TABLE_GRINDJUMP = 5
SK3SFX_TABLE_GRINDLAND = 6
SK3SFX_TABLE_SLIDE = 7
SK3SFX_TABLE_SLIDEJUMP = 8
SK3SFX_TABLE_SLIDELAND = 9
SK3SFX_TABLE_CESS = 10

MusicVolume = 60

//all songs in the game, there is static limit of 25 in exe
//maybe we can show playing song title in the game somehow?
master_song_list = [
	{ path = "music\afi" Title = "A.F.I.: The Boy Who Destroyed The World" }
	{ path = "music\adol" Title = "Adolescents: Amoeba" }
	{ path = "music\antfarm" Title = "Alien Ant Farm: Wish" }
	{ path = "music\bodyjar" Title = "Bodyjar: Not The Same" }
	{ path = "music\cky" Title = "CKY: 96 Quite Bitter Beings" }
	{ path = "music\del" Title = "Del The Funky Homosapien: If You Must" }
	{ path = "music\gutter" Title = "Guttermouth: I'm Destroying The World" }
	{ path = "music\pain" Title = "House of Pain: I'm A Swing It" }
	{ path = "music\krs" Title = "KRS One: Hush" }
	{ path = "music\mad" Title = "Mad Capsule Markets: N Pulse" }
	{ path = "music\ace" Title = "Motorhead: Ace Of Spades" }
	{ path = "music\next" Title = "Nextmen: Amongst Madness" }
	{ path = "music\ozo" Title = "Ozomatli: Cut Chemist Suite" }
	{ path = "music\ramones" Title = "Ramones: Blitzkrieg Bop" }
	{ path = "music\chilis" Title = "Red Hot Chili Peppers: Fight Like A Brave" }
	{ path = "music\redman" Title = "Redman: Let's Get Dirty" }
	{ path = "music\rev" Title = "Reverend Horton Heat: I Can't Surf" }
	{ path = "music\rollins" Title = "Rollins Band: What's The Matter Man" }
	{ path = "music\Xzibit" Title = "Xzibit: Paparazzi" }
	{ path = "music\zebra" Title = "Zebrahead: Check" }
]

master_sfx_list = [
	{ path = "Skater\wheels01" vol = 10 }
	{ path = "Skater\bail04" vol = 50 }
	{ path = "Skater\bail05" vol = 50 }
	{ path = "Skater\bodysmackA" vol = 50 }
	{ path = "Skater\headsmackB" }
	{ path = "Skater\HUD_jumpgap" }
	{ path = "Skater\HUD_specialtrickAA" }
	{ path = "Skater\hitdumpsterX" }
	{ path = "Skater\hitvehicle3" }
	{ path = "Skater\boneless09" }
	{ path = "Skater\boardbail01" vol = 50 }
	{ path = "Skater\boardbail02" vol = 50 }
	{ path = "Skater\foleymove01" vol = 50 }
	{ path = "Skater\foleyslide04" vol = 50 }
	{ path = "Skater\footstep03" vol = 50 }
	{ path = "Skater\hitblood01" vol = 50 }
	{ path = "Skater\hitblood02" vol = 50 }
	{ path = "Skater\hitblood04" vol = 50 }
	{ path = "Skater\hitblood05" vol = 50 }
	{ path = "Skater\hitbody03" vol = 50 }
	{ path = "Skater\hitbody04" vol = 50 }
	{ path = "Skater\hitface01" vol = 50 }
	{ path = "Skater\hitface02" vol = 50 }
	{ path = "Skater\bail_knee1" }
	{ path = "Skater\bail_backward1" }
	{ path = "Skater\Bailrunoutflip" }
	{ path = "Skater\Nosemanualbail" }
	{ path = "Skater\bitchslap2" }
	{ path = "Skater\HUDtrickslopC" }
	{ path = "Skater\HUDtrickperfect" pitch = 80 }
	{ path = "Skater\hitbody02" }
	{ path = "Skater\OllieMetalFuture" }
	{ path = "Skater\select2b" }
	{ path = "Skater\RevertWood" }
	{ path = "Skater\Saberhit1" }
	{ path = "Skater\SaberTwirl5" }
	{ path = "Skater\GUI_stat03" }
	{ path = "Skater\timeoutA" vol = 50 }
	{ path = "Skater\copinghit3" vol = 100 }
	{ path = "Skater\pedprops" vol = 100 }
	{ path = "Skater\GUI_click06" vol = 100 }
	{ path = "Skater\GUI_type02" vol = 100 }
	{ path = "Skater\horselose" vol = 100 }
	{ path = "Skater\GUI_buzzer01" vol = 100 }
	
	//extra sounds used in LevelMod
	{ path = "LM\ExtraTrick" vol = 100 }
]

SCRIPT LoadPermSounds
	ForEachIn master_sfx_list do = LoadSfx params = { <...> }
	ForEachIn master_song_list do = LoadSong params = { <...> }
ENDSCRIPT

script LoadSfx
	if LM_GotParam path
		GetParam path
		if LM_GotParam vol
			GetParam vol
			LoadSound <path> FLAG_PERM vol = <vol>
		else
			if LM_GotParam pitch
				GetParam pitch
				LoadSound <path> FLAG_PERM pitch = <pitch>
			else
				LoadSound <path> FLAG_PERM
			endif
		endif
	else
		printf "LoadSfx: no path given"
	endif
endscript

script LoadSong
	if LM_GotParam path
		GetParam path
		if LM_GotParam Title
			GetParam Title
			AddMusicTrack <path> FLAG_PERM TrackTitle = <Title>
		endif
	else
		printf "LoadSong: no path given"
	endif
endscript
