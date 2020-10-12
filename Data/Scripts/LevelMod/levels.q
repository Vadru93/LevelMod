
//level numbers
LevelNum_Skateshop = 0
LevelNum_Foundry = 1
LevelNum_Canada = 2
LevelNum_Rio = 3
LevelNum_Suburbia = 4
LevelNum_Airport = 5
LevelNum_SkaterIsland = 6
LevelNum_LA = 7
LevelNum_Tokyo = 8
LevelNum_Ship = 9
LevelNum_Oil = 10
LevelNum_Tutorials = 11
LevelNum_Warehouse = 12
LevelNum_Burnside = 13
LevelNum_Roswell = 14
LevelNum_New = 15

SCRIPT InitLevelLoadProcess
	printf "***********************InitLevelLoadProcess"
	UninitializeFrontEndSprites
	Cleanup
	StopMusic
	PauseMusic
	ClearMusicTrackList
	MouseShowCursor 0
ENDSCRIPT

SCRIPT PreFreezeScreen
	SetArenaSize 1
ENDSCRIPT

SCRIPT PostFreezeScreen
	SetArenaSize
ENDSCRIPT


SCRIPT SetupForClipPlane
	EnableFog
	SetClippingDistances near = 12 far = 20000
	SetBackgroundColor { r = 128 g = 128 b = 128 alpha = 0 }
	SetFogDistance distance = 1280
	SetFogColor r = 128 g = 128 b = 128
ENDSCRIPT

SCRIPT PreLevelLoad
	SetArenaSize 1
	SetUpBasicLights
	SetSfxReverb Instant
	ClearScoreGoals
	SetScoreGoal score = 10000 goalscript = DefaultHiScoreScript goal = GOAL_HIGHSCORE
	SetScoreGoal score = 50000 goalscript = DefaultProScoreScript goal = GOAL_PROSCORE
	ClearMusicTrackList
	IF GameModeEquals is_parkeditor
		IF InNetGame
			DisplayProperLoadingScreen net
		ELSE
			DisplayLoadingScreen "images\loadscrn_editor.png"
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT DisplayProperLoadingScreen
	printf "DisplayProperLoadingScreen"
	
	IF InMultiPlayerGame
	ELSE
		change FirstTimeFromSkateshop = 0
	ENDIF
	
	IF GotParam SkateShop
		DisplayLoadingScreen "images\loadscrn_ss.png"
	ELSE
		IF EnteringNetGame
			printf "Entering Net Game"
			IF OnServer
				DisplayLoadingScreen "images\loadscrn_Online.png"
			ELSE
				DisplayLoadingScreen "images\loadscrn_Online2.png"
			ENDIF
		ELSE
			IF IsTrue FirstTimeFromSkateshop
				DisplayLoadingScreen "images\loadscrn_2player.png"
			ELSE 
				IF GotParam loadscr
					DisplayLoadingScreen <loadscr>
				ELSE
					DisplayLoadingScreen <loadscr>
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	
	IF GotParam Simulate
		Wait <Simulate> seconds
	ENDIF
ENDSCRIPT

SCRIPT PostLevelLoad
    printf "PostLevelLoad"
	SetArenaSize
ENDSCRIPT


SCRIPT script_assert <...>
	printf "SCRIPT ASSERT!"
	printf <...>
	BEGIN
		wait 1 gameframe
	REPEAT
ENDSCRIPT

SCRIPT request_level
	printf "Running script: request_level"
	change wall_non_skatable_angle = 25
	IF GotParam level
		RequestLevel <level>
	ELSE
		script_assert "request_level needs a level param"
	ENDIF
ENDSCRIPT

SCRIPT LoadLevel
	printf "Running script: LoadLevel"
	change wall_non_skatable_angle = 25
	MemPushContext 0
	PreLevelLoad
	LaunchLevel <...>
	PostLevelLoad
	MemPopContext
	//ForEachIn LevelModOptions do = OptionsOnChangeLevel params = <...>
ENDSCRIPT

SCRIPT FireUpGame
	ToggleSkater On
	SetGameState On
	LaunchMenuScreen screen = game_menu
ENDSCRIPT


SCRIPT DistinguishLevels
	SetCompetition LevelNum_Rio
	SetCompetition LevelNum_SkaterIsland
	SetCompetition LevelNum_Tokyo
ENDSCRIPT



SCRIPT Load_La
	AddMusicTrack "ambience\la"
	SetUpLosAngelesLights
	Load_Level_Func levelnum = LevelNum_LA lev_bsp = "Levels\la\la.bsp" lev_sky = "Levels\la_Sky\la_Sky.bsp" lev_qb = "Levels\la\la.qb" loadscr = "images\loadscrn_la.png"
	PrepareLevelFog r = 158 g = 202 b = 220 a = 0 cnear = 13 cfar = 14000
	La_Startup
ENDSCRIPT


SCRIPT Load_Shp
	AddMusicTrack "ambience\ship"
	SetUpShipLights
	Load_Level_Func lev_bsp = "Levels\shp\shp.bsp" lev_sky = "Levels\shp_Sky\shp_Sky.bsp" lev_qb = "Levels\shp\shp.qb" loadscr = "images\loadscrn_ship.png"
	CareerStartLevel level = LevelNum_Ship
	PrepareLevelFog r = 143 g = 205 b = 233 a = 0 cnear = 13 cfar = 32000
	SetMovementVelocity 1500
	SetRotateVelocity 120
	LoadTerrain
	SHP_Startup
ENDSCRIPT

SCRIPT Load_Si
	AddMusicTrack "ambience\si"
	SetUpSkatersIslandLights
	Load_Level_Func lev_bsp = "Levels\si\si.bsp" lev_sky = "Levels\si_Sky\si_Sky.bsp" lev_qb = "Levels\si\si.qb" loadscr = "images\loadscrn_si.png" loadscr_comp = "images\loadscrn_si_comp.png"
	CareerStartLevel level = LevelNum_SkaterIsland competition
	PrepareLevelFog r = 158 g = 211 b = 255 a = 0 cnear = 13 cfar = 12000
	LoadTerrain
	Si_Startup
ENDSCRIPT

SCRIPT Load_Tok
	AddMusicTrack "ambience\tok"
	SetUpTokyoLights
	Load_Level_Func lev_bsp = "Levels\tok\tok.bsp" lev_sky = "Levels\tok_Sky\tok_Sky.bsp" lev_qb = "Levels\tok\tok.qb" loadscr = "images\loadscrn_tok.png" loadscr_comp = "images\loadscrn_tok_comp.png"
	CareerStartLevel level = LevelNum_Tokyo competition
	PrepareLevelFog r = 16 g = 17 b = 26 a = 0 cnear = 13 cfar = 17000
	SetMovementVelocity 1000
	SetRotateVelocity 120
	LoadTerrain
	Tok_Startup
ENDSCRIPT

SCRIPT Load_Oil
	AddMusicTrack "ambience\oil"
	SetUpOilLights
	Load_Level_Func lev_bsp = "Levels\oil\oil.bsp" lev_sky = "Levels\oil\oil_Sky.bsp" lev_qb = "Levels\oil\oil.qb" loadscr = "images\loadscrn_oil.png"
	CareerStartLevel level = LevelNum_Oil
	PrepareLevelFog r = 190 g = 205 b = 238 a = 0 cnear = 13 cfar = 30000
	SetMovementVelocity 1000
	SetRotateVelocity 120
	LoadTerrain
	SetRenderModeVU
	Oil_Startup
ENDSCRIPT

SCRIPT Load_Roof
	AddMusicTrack "ambience\burn"
	SetUpSuburbiaLights
	Load_Level_Func lev_bsp = "Levels\par\par.bsp" lev_sky = "Levels\par_Sky\par_Sky.bsp" lev_qb = "Levels\par\par.qb" loadscr = "images\loadscrn_paris.png"
	CareerStartLevel level = LevelNum_New
	PrepareLevelFog r = 165 g = 180 b = 202 a = 0 cnear = 13 cfar = 20000
	LoadTerrain
ENDSCRIPT


SCRIPT Load_Tut
	DisplayProperLoadingScreen loadscr = "images\loadscrn_tutorials.png"
	CareerStartLevel level = LevelNum_Tutorials
	SetUpLosAngelesLights
	LoadLevelGeometry level = "Levels\tut\tut.bsp" Sky = "Levels\Sk3Ed_Bch_Sky\Sk3Ed_Bch_Sky.bsp" Pre_set = "Tut"
	LoadNodeArray "levels\tut\tut.qb"
	IF ClipPlaneEnabled
		SetupForClipPlane
	ELSE
		SetBackgroundColor { r = 128 g = 128 b = 128 alpha = 0 }
		SetClippingDistances near = 12 far = 20000
	ENDIF
	SetBSPAmbientColor { r = 84 g = 84 b = 84 alpha = 0 }
	LoadTerrain
	Tut_Startup
	UnloadPreFile "Tut.pre" dont_assert
ENDSCRIPT

SCRIPT Load_SkateShop
	DisplayProperLoadingScreen SkateShop
	CareerStartLevel level = LevelNum_Skateshop
	SetUpSkateShopLights
	LoadLevelGeometry level = "Levels\skateshop\skateshop.bsp" Sky = "" Pre_set = "SkateShop" KeepTexturesInMemory
	LoadNodeArray "levels\skateshop\skateshop.qb"
	LoadTerrain
	IF ClipPlaneEnabled
		SetupForClipPlane
	ELSE
		SetBackgroundColor { r = 0 g = 0 b = 32 alpha = 0 }
		SetClippingDistances near = 12 far = 12000
	ENDIF
	SkateShop_Startup
	UnloadPreFile "SkateShop.pre" dont_assert
ENDSCRIPT

//unused
SCRIPT Load_Foo
	SetUpFoundryLights
	LoadLevelGeometry level = "Levels\foo\foo.bsp" Sky = "" Pre_set = "Foo"
	LoadNodeArray "levels\foo\foo.qb"
	LoadTerrain
	SetRenderModeVU
	SetBackgroundColor { r = 16 g = 16 b = 32 alpha = 0 }
	SetClippingDistances near = 12 far = 22000
	Foo_Startup
	UnloadPreFile "Foo.pre" dont_assert
ENDSCRIPT


//=========================================THUG2===============================================

SCRIPT Load_AU_ug2
	AddMusicTrack "ambience\thug2\AU"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\AU\AU.bsp" lev_sky = "levels_ug2\AU\AU_sky.bsp" lev_qb = "levels_ug2\AU\AU.qb" loadscr = "images\load\ug2_au.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_NO_ug2
	AddMusicTrack "ambience\thug2\NO"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\NO\NO.bsp" lev_sky = "levels_ug2\NO\NO_sky.bsp" lev_qb = "levels_ug2\NO\NO.qb" loadscr = "images\load\ug2_no.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BA_ug2
	AddMusicTrack "ambience\thug2\BA"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\BA\BA.bsp" lev_sky = "levels_ug2\BA\BA_sky.bsp" lev_qb = "levels_ug2\BA\BA.qb" loadscr = "images\load\ug2_ba.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BE_ug2
	AddMusicTrack "ambience\thug2\BE"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\BE\BE.bsp" lev_sky = "levels_ug2\BE\BE_sky.bsp" lev_qb = "levels_ug2\BE\BE.qb" loadscr = "images\load\ug2_be.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_BO_ug2
	AddMusicTrack "ambience\thug2\BO"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\BO\BO.bsp" lev_sky = "levels_ug2\BO\BO_sky.bsp" lev_qb = "levels_ug2\BO\BO.qb" loadscr = "images\load\ug2_bo.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE_ug2
	AddMusicTrack "ambience\thug2\se"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\se\se.bsp" lev_sky = "levels_ug2\se\se_sky.bsp" lev_qb = "levels_ug2\se\se.qb" loadscr = "images\load\ug2_se.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE2_ug2
	AddMusicTrack "ambience\thug2\SE2"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\SE2\SE2.bsp" lev_sky = "levels_ug2\SE2\SE2_sky.bsp" lev_qb = "levels_ug2\SE2\SE2.qb" loadscr = "images\load\ug2_se2.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_ST_ug2
	AddMusicTrack "ambience\thug2\ST"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug2\ST\ST.bsp" lev_sky = "levels_ug2\ST\ST_sky.bsp" lev_qb = "levels_ug2\ST\ST.qb" loadscr = "images\load\ug2_st.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT


//don't think default params are actually needed
//if it crashes, fix the struct. we don't want to like load foundry if something's wrong.
SCRIPT Load_Level_Func 
	printf "Load_Level_Func begins..."

	IF IsCareerMode
		IF GotParam loadscr_comp
			DisplayProperLoadingScreen { loadscr = <loadscr_comp> }
		ELSE
			DisplayProperLoadingScreen { loadscr = <loadscr> }
		ENDIF
	ELSE
		DisplayProperLoadingScreen { loadscr = <loadscr> }
	ENDIF

	//this sets levelnum for career mode, so bails check doesnt work if skipped
	//make sure levelnum = LevelNum_New is set for levelmod levels
	IF GotParam levelnum
		IF GotParam competition
			CareerStartLevel { level = <levelnum> competition }
		ELSE
			CareerStartLevel { level = <levelnum> }
		ENDIF
	ENDIF

	IF GotParam lev_amb
		AddMusicTrack <lev_amb>
	ENDIF

	IF GotParam lev_lights
		<lev_lights>
	ELSE
		SetUpRioDeJaneiroLights
	ENDIF


	printf "Loading Geometry..."
	LoadLevelGeometry { level = <lev_bsp> Sky = <lev_sky> }
	
	IF InNetGame
		IF #"Not" IsOptionOn LM_GameOption_bNetSky
			LoadLevelGeometry { Sky = "" }
		ENDIF
	ENDIF
	
	printf "Loading NodeArray..."
	LoadNodeArray <lev_qb>
	
	LoadTerrain
	
	PrepareLevelFog <...>
	
	//this should be uncommented when all def structs are fixed
	LM_MaybeSetTh2Physics <...>

	IF GotParam startup_func
		<startup_func>
	ENDIF

	printf "...finished loading."
ENDSCRIPT


DEFAULT_WALL_SKATABLE_ANGLE = 25
LM_TH2_PHYSICS = 0


SCRIPT LM_MaybeSetTh2Physics
	//reset to default angle
	Change Wall_Non_Skatable_Angle = DEFAULT_WALL_SKATABLE_ANGLE
	Change LM_TH2_PHYSICS = 0
	
	if GotParam th1_level
		Change LM_TH2_PHYSICS = 1
	endif
	
	if GotParam th2_level
		Change LM_TH2_PHYSICS = 1
	endif
	
	if GotParam th2x_level
		Change LM_TH2_PHYSICS = 1
	endif
	
	//this is here for th1 levels in th3 (warehouse, burnside, roswell)
	if GotParam ignore_th2_physics
		Change LM_TH2_PHYSICS = 0
	endif
	
	if IsTrue LM_TH2_PHYSICS
		printf "need to fix vert angle!"
		Change Wall_Non_Skatable_Angle = 0
	endif
ENDSCRIPT

SCRIPT PrepareLevelFog r = 128 b = 128 b = 128 a = 128 cnear = 12 cfar = 10000
	//IF ClipPlaneEnabled
	//	SetupForClipPlane
	//ELSE
	//	SetBackgroundColor <...>
	//	SetClippingDistances { near = <cnear> far = <cfar> }
	//ENDIF
	
	SetBackgroundColor <...>
	
	//IF ClipPlaneEnabled
	IF IsOptionOn LM_GameOption_bFog
		EnableFog
		SetClippingDistances { near = 12 far = 2500 }
		SetFogDistance { distance = 1250 }
		SetFogColor <...>
	ELSE
		DisableFog
		SetClippingDistances { near = <cnear> far = <cfar> }
	ENDIF
ENDSCRIPT


OnlineModes = { 
	supports_ctf 
	supports_own 
	supports_bball 
}


//=============================Master Level List==============================

//================THPS1===================

//THPS1 WAREHOUSE

Def_Ware = { 
	level_name = "Warehouse" 
	location = "Woodland Hills"
	level_id = warehouse_id 
	levelnum = LevelNum_Warehouse	
	unlock_flag = LEVEL_UNLOCKED_WAREHOUSE 

	th1_level regular_level NoCareer OnlineModes ignore_th2_physics

	lev_bsp = "levels\ware\ware.bsp"
	lev_qb = "levels\ware\ware.qb"
	lev_amb = "ambience\thps1\ware"
	loadscr = "images\loadscrn_warehouse.png"
	lev_lights = SetUpWareLights

	r = 0 g = 0 b = 0 a = 0 
	cnear = 13 cfar = 20000
	
	startup_func = Ware_Startup
	load_script = Load_Ware 
}

SCRIPT Load_Ware
	Load_Level_Func { Def_Ware }
ENDSCRIPT

//THPS1 SCHOOL

Def_Sc1 = { 
	level_name = "School" 
	location = "Miami"
	level_id = sc1_id 
	levelnum = LevelNum_New

	th1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th1\sc1\sc1.bsp" 
	lev_sky = "levels_th1\sc1\sc1_sky.bsp" 
	lev_qb = "levels_th1\sc1\sc1.qb" 
	lev_amb = "ambience\thps1\sc1"
	loadscr = "images\load\th1_sc1.png"

	r = 50 g = 84 b = 149 a = 0 
	cnear = 12 cfar = 13000

	load_script = Load_Sc1
} 

SCRIPT Load_Sc1
	Load_Level_Func { Def_Sc1 }
ENDSCRIPT

//THPS1 MALL

Def_Mall = { 
	level_name = "The Mall" 
	location = "New York"
	level_id = mall_id 
	levelnum = LevelNum_New

	th1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th1\mall\mall.bsp" 
	lev_sky = "levels_th1\mall\mall_sky.bsp" 
	lev_qb = "levels_th1\mall\mall.qb" 
	lev_amb = "ambience\thps1\mall"
	loadscr = "images\load\th1_mall.png"

	r = 0 g = 0 b = 0 a = 0 
	cnear = 12 cfar = 20000

	load_script = Load_Mall
} 

SCRIPT Load_Mall
	Load_Level_Func { Def_Mall }
ENDSCRIPT

//THPS1 VANS

Def_Vans = { 
	level_name = "Skate Park" 
	location = "Chicago"
	level_id = vans_id 
	levelnum = LevelNum_New

	th1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th1\vans\vans.bsp" 
	lev_sky = "levels_th1\vans\vans_sky.bsp" 
	lev_qb = "levels_th1\vans\vans.qb" 
	lev_amb = "ambience\thps1\vans"
	loadscr = "images\load\th1_vans.png"

	r = 191 g = 230 b = 255 a = 0 
	cnear = 12 cfar = 5000

	load_script = Load_Vans
} 

SCRIPT Load_Vans
	Load_Level_Func { Def_Vans }
ENDSCRIPT

//THPS1 DOWNTOWN

Def_Down = { 
	level_name = "Downtown" 
	location = "Minneapolis"
	level_id = down_id 
	levelnum = LevelNum_New

	th1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th1\down\down.bsp" 
	lev_sky = "levels_th1\down\down_sky.bsp" 
	lev_qb = "levels_th1\down\down.qb" 
	lev_amb = "ambience\thps1\down"
	loadscr = "images\load\th1_down.png"
	lev_lights = SetUpTokyoLights

	r = 0 g = 8 b = 19 a = 0 
	cnear = 12 cfar = 10000

	load_script = Load_Down
} 

SCRIPT Load_Down
	Load_Level_Func { Def_Down }
ENDSCRIPT

//THPS1 DOWNHILL JAM

Def_Jam = { 
	level_name = "Downhill Jam" 
	location = "Phoenix"
	level_id = jam_id 
	levelnum = LevelNum_New

	th1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th1\jam\jam.bsp" 
	lev_sky = "levels_th1\jam\jam_sky.bsp" 
	lev_qb = "levels_th1\jam\jam.qb" 
	lev_amb = "ambience\thps1\jam"
	loadscr = "images\load\th1_jam.png"
	lev_lights = SetUpSuburbiaLights

	r = 121 g = 118 b = 140 a = 0 
	cnear = 12 cfar = 15000

	load_script = Load_Jam
} 

SCRIPT Load_Jam
	Load_Level_Func { Def_Jam }
ENDSCRIPT

//THPS1 BURNSIDE

Def_Burn = { 
	level_name = "Burnside" 
	location = "Portland"
	level_id = burnside_id 
	levelnum = LevelNum_Burnside
	unlock_flag = LEVEL_UNLOCKED_BURNSIDE

	th1_level regular_level NoCareer OnlineModes ignore_th2_physics

	lev_bsp = "levels\burn\burn.bsp" 
	lev_sky = "levels\burn\burn_sky.bsp" 
	lev_qb = "levels\burn\burn.qb" 
	lev_amb = "ambience\thps1\burn"
	loadscr = "images\loadscrn_burnside.png"
	lev_lights = SetUpBurnLights

	r = 159 g = 183 b = 214 a = 0 
	cnear = 13 cfar = 9000

	startup_func = Burn_Startup
	load_script = Load_Burn
} 

SCRIPT Load_Burn
	Load_Level_Func { Def_Burn }
ENDSCRIPT

//THPS1 STREETS

Def_SF1 = { 
	level_name = "Streets" 
	location = "San Francisco"
	level_id = sf1_id 
	levelnum = LevelNum_New

	th1_level regular_level NoCareer OnlineModes ignore_th2_physics

	lev_bsp = "levels_th1\sf1\sf1.bsp" 
	lev_sky = "levels_th1\sf1\sf1_sky.bsp" 
	lev_qb = "levels_th1\sf1\sf1.qb" 
	lev_amb = "ambience\thps1\sf1"
	loadscr = "images\load\th1_sf1.png"

	r = 124 g = 149 b = 172 a = 0 
	cnear = 12 cfar = 20000

	load_script = Load_SF1
}

SCRIPT Load_SF1
	Load_Level_Func { Def_SF1 }
ENDSCRIPT

//THPS1 ROSWELL

Def_Ros = {
	level_name = "Roswell" 
	location = "New Mexico"
	level_id = roswell_id 
	levelnum = LevelNum_Roswell
	unlock_flag = LEVEL_UNLOCKED_ROSWELL

	th1_level regular_level NoCareer OnlineModes ignore_th2_physics

	lev_bsp = "levels\ros\ros.bsp" 
	lev_sky = "levels\ros\ros_sky.bsp" 
	lev_qb = "levels\ros\ros.qb" 
	lev_amb = "ambience\thps1\ros"
	loadscr = "images\loadscrn_roswell.png"
	lev_lights = SetUpRoswellLights

	r = 41 g = 34 b = 52 a = 0 
	cnear = 13 cfar = 9000

	load_script = Load_Ros
}

SCRIPT Load_Ros
	Load_Level_Func { Def_Ros }
ENDSCRIPT

//================THPS2===================

//THPS2 HANGAR

Def_Han = { 
	level_name = "Hangar" 
	location = "Mullet Falls MT"
	level_id = han_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\Han\han.bsp" 
	lev_sky = "levels_th2\han\han_sky.bsp" 
	lev_qb = "levels_th2\han\han.qb" 
	lev_amb = "ambience\thps2\han"
	loadscr = "images\load\th2_han.png"
	lev_lights = SetUpTokyoLights
	
	r = 6 g = 8 b = 12 a = 0 
	cnear = 12 cfar = 10000
	
	load_script = Load_Han 
} 

SCRIPT Load_Han
	Load_Level_Func { Def_Han }
ENDSCRIPT

//THPS2 SCHOOL 2

Def_Sc2 = { 
	level_name = "School II" 
	location = "California"
	level_id = sc2_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\sc2\sc2.bsp"
	lev_sky = "levels_th2\sc2\sc2_sky.bsp" 
	lev_qb = "levels_th2\sc2\sc2.qb" 
	lev_amb = "ambience\thps2\sc2"
	loadscr = "images\load\th2_sc2.png"
	lev_lights = SetUpLosAngelesLights
	
	r = 145 g = 201 b = 242 a = 0 
	cnear = 12 cfar = 13000
	
	load_script = Load_Sc2 
} 

SCRIPT Load_Sc2
	Load_Level_Func { Def_Sc2 }
ENDSCRIPT

//THPS2 MARSEILLE

Def_Mar = { 
	level_name = "Marseille" 
	location = "France"
	level_id = mar_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\mar\mar.bsp"
	lev_sky = "levels_th2\mar\mar_sky.bsp" 
	lev_qb = "levels_th2\mar\mar.qb" 
	lev_amb = "ambience\thps2\mar"
	loadscr = "images\load\th2_mar.png"
	lev_lights = SetUpSuburbiaLights
	
	r = 187 g = 132 b = 98 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_Mar
}

SCRIPT Load_Mar
	Load_Level_Func { Def_Mar }
ENDSCRIPT

//THPS2 NEW YORK CITY

Def_NY1 = { 
	level_name = "New York" 
	location = "NY City"
	level_id = ny1_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\ny\ny.bsp" 
	lev_sky = "levels_th2\ny\ny_Sky.bsp" 
	lev_qb = "levels_th2\ny\ny.qb" 
	lev_amb = "ambience\thps2\ny"
	loadscr = "images\load\th2_ny.png"
	lev_lights = SetUpTokyoLights	
	
	r = 4 g = 8 b = 12 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_NY1 
}

SCRIPT Load_NY1
	Load_Level_Func { Def_NY1 }
ENDSCRIPT

//THPS2 VENICE BEACH

Def_Ven = { 
	level_name = "Venice Beach" 
	location = "California"
	level_id = ven_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\ven\ven.bsp" 
	lev_sky = "levels_th2\ven\ven_Sky.bsp" 
	lev_qb = "levels_th2\ven\ven.qb" 
	lev_amb = "ambience\thps2\ven"
	loadscr = "images\load\th2_ven.png"
	lev_lights = SetUpShipLights
	
	r = 145 g = 201 b = 242 a = 0 
	cnear = 12 cfar = 18000
	
	load_script = Load_Ven
}

SCRIPT Load_Ven
	Load_Level_Func { Def_Ven }
ENDSCRIPT

//THPS2 SKATESTREET

Def_SSV = { 
	level_name = "Skatestreet" 
	location = "Ventura"
	level_id = ssv_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\ssv\ssv.bsp" 
	lev_sky = "levels_th2\ssv\ssv_Sky.bsp" 
	lev_qb = "levels_th2\ssv\ssv.qb" 
	lev_amb = "ambience\thps2\ssv"
	loadscr = "images\load\th2_ssv.png"
	lev_lights = SetUpSkatersIslandLights
	
	r = 145 g = 201 b = 242 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_SSV
}

SCRIPT Load_SSV
	Load_Level_Func { Def_SSV }
ENDSCRIPT

//THPS2 PHILADELPHIA

Def_Ph = { 
	level_name = "Philadelphia" 
	location = "Pennsylvania"
	level_id = ph_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\ph\ph.bsp" 
	lev_sky = "levels_th2\ph\ph_Sky.bsp" 
	lev_qb = "levels_th2\ph\ph.qb" 
	lev_amb = "ambience\thps2\ph"
	loadscr = "images\load\th2_philly.png"
	lev_lights = SetUpLosAngelesLights
	
	r = 145 g = 201 b = 242 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_Ph
}

SCRIPT Load_Ph
	Load_Level_Func { Def_Ph }
ENDSCRIPT

//THPS2 BULLRING

Def_Bul = { 
	level_name = "Bullring" 
	location = "New Mexico"
	level_id = bul_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\bul\bul.bsp" 
	lev_sky = "levels_th2\bul\bul_Sky.bsp" 
	lev_qb = "levels_th2\bul\bul.qb" 
	lev_amb = "ambience\thps2\bul"
	loadscr = "images\load\th2_bul.png"
	lev_lights = SetUpSuburbiaLights
	
	r = 145 g = 201 b = 242 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_Bul
}

SCRIPT Load_Bul
	Load_Level_Func { Def_Bul }
ENDSCRIPT

//THPS2 SKATE HEAVEN

Def_Hvn = { 
	level_name = "Skate Heaven" 
	location = "Paradise"
	level_id = hvn_id 
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\hvn\hvn.bsp" 
	lev_sky = "levels_th2\hvn\hvn_Sky.bsp" 
	lev_qb = "levels_th2\hvn\hvn.qb" 
	lev_amb = "ambience\thps2\hvn"
	loadscr = "images\load\th2_hvn.png"
	lev_lights = SetUpTokyoLights
	
	r = 1 g = 4 b = 8
	a = 0 cnear = 12 cfar = 17000
	
	load_script = Load_Hvn
}

SCRIPT Load_Hvn
	Load_Level_Func { Def_Hvn }
ENDSCRIPT

//THPS2 CHOPPER DROP

Def_Drop = {
	level_name = "Chopper Drop"
	location = "Hawaii"
	level_id = drop_id
	levelnum = LevelNum_New
	NoCareer th2_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\Drop\Drop.bsp" 
	lev_sky = "levels_th2\Drop\Drop_sky.bsp" 
	lev_qb = "levels_th2\Drop\Drop.qb" 
	lev_amb = "ambience\thps2\drop"
	loadscr = "images\load\th2_drop.png"
	lev_lights = SetUpOilLights
	
	r = 12 g = 8 b = 9 a = 0 
	cnear = 12 cfar = 10000
	
	load_script = Load_Drop
}

SCRIPT Load_Drop
	Load_Level_Func { Def_Drop }
ENDSCRIPT


//================THPS2X===================

//THPS2X CONSTRUCTION SITE

Def_Cons = { 
	level_id = cons_id
	levelnum = LevelNum_New
	level_name = "Construction Site"
	
	NoCareer th2x_level regular_level OnlineModes
	
	lev_bsp = "levels_th2\cons\cons.bsp" 
	lev_sky = "levels_th2\cons\cons_sky.bsp" 
	lev_qb = "levels_th2\cons\cons.qb" 
	lev_amb = "ambience\thps2\cons"
	loadscr = "images\load\th2_cons.png"
	
	r = 106 g = 97 b = 72 a = 0 
	cnear = 12 cfar = 20000
	
	load_script = Load_Cons 
}

SCRIPT Load_Cons
	Load_Level_Func { Def_Cons }
ENDSCRIPT

//THPS2X CLUB

Def_Club = { 
	level_id = club_id 
	levelnum = LevelNum_New
	
	level_name = "Club" 
	location = "London"

	lev_bsp = "levels_th2\Club\Club.bsp" 
	lev_qb = "levels_th2\Club\Club.qb" 
	lev_amb = "ambience\thps2\club"
	loadscr = "images\load\th2_club.png"
	
	NoCareer th2x_level regular_level OnlineModes
	
	r = 0 g = 0 b = 0 a = 0 
	cnear = 12 cfar = 20000 
	
	load_script = Load_Club 
}

SCRIPT Load_Club
	Load_Level_Func { Def_Club }
ENDSCRIPT

//THPS2X SUBWAY

Def_Sway = { 
	level_name = "Subway" 
	load_script = Load_Sway 
	level_id = sway_id 
	
	lev_bsp = "levels_th2\Sway\Sway.bsp" 
	lev_qb = "levels_th2\Sway\Sway.qb" 
	lev_amb = "ambience\thps2\sway"
	loadscr = "images\load\th2_sway.png"
	
	NoCareer th2x_level regular_level OnlineModes
	
	r = 0 g = 0 b = 0 a = 0 
	cnear = 12 cfar = 20000 
}

SCRIPT Load_Sway
	Load_Level_Func { Def_Sway }
ENDSCRIPT

//THPS2X TAMPA

Def_Flor = { 
	level_name = "Skate Park Tampa"
	load_script = Load_Flor 
	level_id = flor_id 
	
	//unlock_flag = FIX_LEVELMOD_FLAG 
	
	lev_bsp = "levels_th2\flor\flor.bsp" 
	lev_sky = "levels_th2\flor\flor_sky.bsp" 
	lev_qb = "levels_th2\flor\flor.qb" 
	lev_amb = "ambience\thps2\flor"
	loadscr = "images\load\th2_flor.png"
	
	NoCareer th2x_level regular_level OnlineModes 
	
	r = 149 g = 192 b = 232 a = 0 
	cnear = 12 cfar = 20000 
}

SCRIPT Load_Flor
	Load_Level_Func { Def_Flor }
ENDSCRIPT

//THPS2X SKY LINES

Def_Sky = { 
	level_name = "Sky Lines" 
	load_script = Load_Sky 
	level_id = skylines_id 
	
	lev_bsp = "levels_th2\roof\roof.bsp" 
	lev_sky = "levels_th2\roof\roof_sky.bsp" 
	lev_qb = "levels_th2\roof\roof.qb" 
	lev_amb = "ambience\thps2\sky"
	loadscr = "images\load\th2_sky.png"
	
	NoCareer th2x_level regular_level OnlineModes

	r = 32 g = 36 b = 63 a = 0 
	cnear = 12 cfar = 20000 
}

SCRIPT Load_Sky
	Load_Level_Func { Def_Sky }
ENDSCRIPT


//================THPS3===================

//THPS3 FOUNDRY

Def_Foun = {
	level_id = foundry_id 
	levelnum = LevelNum_Foundry 
	level_name = "Foundry" 
	location = "Woodland Hills" 
	
	lev_lights = SetUpFoundryLights
	
	regular_level th3_level OnlineModes
	
	once_on_startup = StartRunScript 
	once_on_exit = EndRunScript 
	load_script = Load_Foun 
	startup_func = Foun_Startup

	lev_bsp = "Levels\foun\foun.bsp" 
	lev_sky = "Levels\foun_Sky\foun_Sky.bsp" 
	lev_qb = "Levels\foun\foun.qb"
	lev_amb = "ambience\th3\foun"
	loadscr = "images\loadscrn_foundry.png"
	
	r = 0 g = 0 b = 0 a = 0 
	cnear = 13 cfar = 20000
}

SCRIPT Load_Foun
	Load_Level_Func { Def_Foun }
ENDSCRIPT

//THPS3 CANADA

Def_Can = { 
	level_id = canada_id 
	levelnum = LevelNum_Canada 
	unlock_flag = LEVEL_UNLOCKED_CANADA 
	level_name = "Canada" 
	location = "Canada"
	lev_lights = SetUpCanadaLights
	
	regular_level th3_level OnlineModes
	
	once_on_startup = StartRunScript 
	once_on_exit = EndRunScript 
	load_script = Load_Can
	startup_func = Can_Startup
	
	lev_bsp = "Levels\can\can.bsp" 
	lev_sky = "Levels\can_Sky\can_Sky.bsp" 
	lev_qb = "Levels\can\can.qb" 
	lev_amb = "ambience\th3\can"
	loadscr = "images\loadscrn_can.png"
	
	r = 157 g = 170 b = 190 a = 0 
	cnear = 13 cfar = 16000
}

SCRIPT Load_Can
	Load_Level_Func { Def_Can }
	SetMovementVelocity 1500
	SetRotateVelocity 120
ENDSCRIPT

//THPS3 RIO

Def_Rio = {
	level_id = rio_id 
	levelnum = LevelNum_Rio 
	level_name = "Rio"
	location = "Brazil"
	unlock_flag = LEVEL_UNLOCKED_RIO 
	
	lev_lights = SetUpRioDeJaneiroLights
	
	regular_level th3_level OnlineModes competition
	
	once_on_startup = StartRunScript 
	once_on_exit = EndRunScript 
	load_script = Load_Rio 
	startup_func = Rio_Startup

	lev_bsp = "Levels\rio\rio.bsp" 
	lev_sky = "Levels\rio_Sky\rio_Sky.bsp" 
	lev_qb = "Levels\rio\rio.qb" 
	le_amb = "ambience\th3\rio"
	loadscr = "images\loadscrn_rio.png"
	loadscr_comp = "images\loadscrn_rio_comp.png"

	r = 124 g = 158 b = 253 a = 0 
	cnear = 13 cfar = 30000
}

SCRIPT Load_Rio
	Load_Level_Func { Def_Rio }
	SetMovementVelocity 2000
	SetRotateVelocity 120
ENDSCRIPT

//THPS3 SUBURBIA

Def_Sub = {
	level_id = suburbia_id 
	levelnum = LevelNum_Suburbia 
	level_name = "Suburbia"
	location = "Jacksonville?"
	unlock_flag = LEVEL_UNLOCKED_SUBURBIA 
	
	lev_lights = SetUpSuburbiaLights
	
	regular_level th3_level OnlineModes 
	
	once_on_startup = StartRunScript 
	once_on_exit = EndRunScript 
	load_script = Load_Sub
	startup_func = Sub_Startup

	lev_bsp = "Levels\sub\sub.bsp" 
	lev_sky = "Levels\sub_Sky\sub_Sky.bsp" 
	lev_qb = "Levels\sub\sub.qb" 
	le_amb = "ambience\th3\sub"
	loadscr = "images\loadscrn_sub.png"

	r = 121 g = 118 b = 140 a = 0 
	cnear = 13 cfar = 13500
}

SCRIPT Load_Sub
	Load_Level_Func { Def_Sub }
ENDSCRIPT

//THPS3 AIRPORT

Def_Ap = {
	level_id = airport_id 
	levelnum = LevelNum_Airport
	level_name = "Airport"
	location = "Hawaii?"
	unlock_flag = LEVEL_UNLOCKED_AIRPORT 
	
	lev_lights = SetUpAirportLights
	
	regular_level th3_level OnlineModes 
	
	once_on_startup = StartRunScript 
	once_on_exit = EndRunScript 
	load_script = Load_Ap
	startup_func = Ap_Startup

	lev_bsp = "Levels\ap\ap.bsp" 
	lev_sky = "Levels\ap_Sky\ap_Sky.bsp" 
	lev_qb = "Levels\ap\ap.qb" 
	le_amb = "ambience\th3\ap"
	loadscr = "images\loadscrn_ap.png"

	r = 134 g = 168 b = 190 a = 0 
	cnear = 13 
	cfar = 20000
}

SCRIPT Load_Ap
	Load_Level_Func { Def_Ap }
ENDSCRIPT


//================THPS4===================

//THPS4 TRAINING

Def_Trn = { 
	level_name = "Training" 
	location = "unknown"
	level_id = Trn_id 
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Trn\Trn.bsp" 
	lev_sky = "levels_th4\Trn\Trn_sky.bsp" 
	lev_qb = "levels_th4\Trn\Trn.qb" 
	lev_amb = "ambience\thps4\Trn"
	loadscr = "images\load\th4_mot.png"

	r = 179 g = 184 b = 192 a = 0 
	cnear = 12 cfar = 20000

	load_script = Load_Trn
} 

SCRIPT Load_Trn
	Load_Level_Func { Def_Trn }
ENDSCRIPT

//THPS4 COLLEGE

Def_Sch = { 
	level_name = "College" 
	location = "California"
	level_id = college_id 
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Sch\Sch.bsp" 
	lev_sky = "levels_th4\Sch\Sch_sky.bsp" 
	lev_qb = "levels_th4\Sch\Sch.qb" 
	lev_amb = "ambience\thps4\Sch"
	loadscr = "images\load\th4_col.png"

	r = 140 g = 168 b = 228 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Sch
} 

SCRIPT Load_Sch
	Load_Level_Func { Def_Sch }
ENDSCRIPT

//THPS4 SAN FRANCISCO

Def_SF2 = { 
	level_name = "San Francisco" 
	location = "California"
	level_id = sanfran2_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Sf2\Sf2.bsp"
	lev_sky = "levels_th4\Sf2\Sf2_sky.bsp"
	lev_qb = "levels_th4\Sf2\Sf2.qb"
	lev_amb = "ambience\thps4\Sf2"
	loadscr = "images\load\th4_sf2.png"

	r = 179 g = 184 b = 192 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_SF2
}

SCRIPT Load_SF2
	Load_Level_Func { Def_Sf2 }
ENDSCRIPT

//THPS4 ALCATRAZ

Def_Alc = { 
	level_name = "Alcatraz" 
	location = "California Bay"
	level_id = alcatraz_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\alc\alc.bsp"
	lev_sky = "levels_th4\alc\alc_sky.bsp"
	lev_qb = "levels_th4\alc\alc.qb"
	lev_amb = "ambience\thps4\alc"
	loadscr = "images\load\th4_alc.png"

	r = 179 g = 184 b = 192 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Alc
}

SCRIPT Load_Alc
	Load_Level_Func { Def_Alc }
ENDSCRIPT

//THPS4 KONA USA

Def_Kon = { 
	level_name = "Kona USA" 
	location = "Jacksonville"
	level_id = kona_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Kon\Kon.bsp"
	lev_sky = "levels_th4\Kon\Kon_sky.bsp"
	lev_qb = "levels_th4\Kon\Kon.qb"
	lev_amb = "ambience\thps4\Kon"
	loadscr = "images\load\th4_Kon.png"

	r = 137 g = 163 b = 186 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Kon
}

SCRIPT Load_Kon
	Load_Level_Func { Def_Kon }
ENDSCRIPT

//THPS4 SHIPYARD

Def_Jnk = { 
	level_name = "Shipyard" 
	location = "unknown"
	level_id = shipyard_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Jnk\Jnk.bsp"
	lev_sky = "levels_th4\Jnk\Jnk_sky.bsp"
	lev_qb = "levels_th4\Jnk\Jnk.qb"
	lev_amb = "ambience\thps4\Jnk"
	loadscr = "images\load\th4_Jnk.png"

	r = 112 g = 145 b = 172 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Jnk
}

SCRIPT Load_Jnk
	Load_Level_Func { Def_Jnk }
ENDSCRIPT

//THPS4 LONDON

Def_Lon = { 
	level_name = "London" 
	location = "United Kingdom"
	level_id = london_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Lon\Lon.bsp"
	lev_sky = "levels_th4\Lon\Lon_sky.bsp"
	lev_qb = "levels_th4\Lon\Lon.qb"
	lev_amb = "ambience\thps4\Lon"
	loadscr = "images\load\th4_Lon.png"

	r = 130 g = 154 b = 182 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Lon
}

SCRIPT Load_Lon
	Load_Level_Func { Def_Lon }
ENDSCRIPT

//THPS4 ZOO

Def_Zoo = { 
	level_name = "Zoo" 
	location = "United Kingdom"
	level_id = zoo_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Zoo\Zoo.bsp"
	lev_sky = "levels_th4\Zoo\Zoo_sky.bsp"
	lev_qb = "levels_th4\Zoo\Zoo.qb"
	lev_amb = "ambience\thps4\Zoo"
	loadscr = "images\load\th4_Zoo.png"

	r = 130 g = 154 b = 182 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Zoo
}

SCRIPT Load_Zoo
	Load_Level_Func { Def_Zoo }
ENDSCRIPT

//THPS4 CARNIVAL

Def_Cnv = { 
	level_name = "Carnival" 
	location = "unknown (USA)"
	level_id = carnival_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Cnv\Cnv.bsp"
	lev_sky = "levels_th4\Cnv\Cnv_sky.bsp"
	lev_qb = "levels_th4\Cnv\Cnv.qb"
	lev_amb = "ambience\thps4\Cnv"
	loadscr = "images\load\th4_Cnv.png"

	r = 180 g = 146 b = 119 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Cnv
}

SCRIPT Load_Cnv
	Load_Level_Func { Def_Cnv }
ENDSCRIPT

//THPS4 CHICAGO

Def_Hof = { 
	level_name = "Chicago" 
	location = "Illinois"
	level_id = hoffman_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Hof\Hof.bsp"
	lev_sky = "levels_th4\Hof\Hof_sky.bsp"
	lev_qb = "levels_th4\Hof\Hof.qb"
	lev_amb = "ambience\thps4\Hof"
	loadscr = "images\load\th4_Hof.png"

	r = 165 g = 180 b = 202 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Hof
}

SCRIPT Load_Hof
	Load_Level_Func { Def_Hof }
ENDSCRIPT

//THPS4 MOTOX

Def_Mot = { 
	level_name = "MotoX" 
	location = "unknown"
	level_id = motox_id
	levelnum = Levelnum_New

	th4_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_th4\Mot\Mot.bsp"
	lev_sky = "levels_th4\Mot\Mot_sky.bsp"
	lev_qb = "levels_th4\Mot\Mot.qb"
	lev_amb = "ambience\thps4\Mot"
	loadscr = "images\load\th4_Mot.png"

	r = 165 g = 180 b = 202 a = 0 
	cnear = 12 cfar = 30000

	load_script = Load_Mot
}

SCRIPT Load_Mot
	Load_Level_Func { Def_Mot }
ENDSCRIPT



//=========================================THUG1===============================================

//THUG1 NEW JERSEY

Def_NJ = { 
	level_name = "New Jersey" 
	location = "Trenton"
	level_id = jersey_id
	levelnum = Levelnum_New

	ug1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_ug1\nj\nj.bsp" 
	lev_sky = "levels_ug1\nj\nj_sky.bsp" 
	lev_qb = "levels_ug1\nj\nj.qb" 
	lev_amb = "ambience\thug1\nj"
	loadscr = "images\load\ug1_nj.png"

	r = 165 g = 180 b = 202 a = 0 
	cnear = 12 cfar = 20000

	load_script = Load_NJ
}

SCRIPT Load_NJ
	Load_Level_Func { Def_NJ }
ENDSCRIPT

//THUG1 MANHATTAN

Def_NY = { 
	level_name = "Manhattan" 
	location = "New York"
	level_id = manhattan_id
	levelnum = Levelnum_New

	ug1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_ug1\ny\ny.bsp" 
	lev_sky = "levels_ug1\ny\ny_sky.bsp" 
	lev_qb = "levels_ug1\ny\ny.qb" 
	lev_amb = "ambience\thug1\ny"
	loadscr = "images\load\ug1_ny.png"

	r = 165 g = 180 b = 202 a = 0 
	cnear = 12 cfar = 20000

	load_script = Load_NY
}

SCRIPT Load_NY
	Load_Level_Func { Def_NY }
ENDSCRIPT

//THUG1 TAMPA

Def_FL = { 
	level_name = "Tampa" 
	location = "Florida"
	level_id = tampa_id
	levelnum = Levelnum_New

	ug1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_ug1\fl\fl.bsp" 
	lev_sky = "levels_ug1\fl\fl_sky.bsp" 
	lev_qb = "levels_ug1\fl\fl.qb" 
	lev_amb = "ambience\thug1\fl"
	loadscr = "images\load\ug1_fl.png"

	r = 165 g = 180 b = 202 a = 0 
	cnear = 12 cfar = 20000

	load_script = Load_FL
}

SCRIPT Load_FL
	Load_Level_Func { Def_FL }
ENDSCRIPT

//THUG1 SAN DIEGO

Def_SD = { 
	level_name = "San Diego" 
	location = "USA"
	level_id = sandiego_id
	levelnum = Levelnum_New

	ug1_level regular_level NoCareer OnlineModes

	lev_bsp = "levels_ug1\sd\sd.bsp" 
	lev_sky = "levels_ug1\sd\sd_sky.bsp" 
	lev_qb = "levels_ug1\sd\sd.qb" 
	lev_amb = "ambience\thug1\sd"
	loadscr = "images\load\ug1_sd.png"

	r = 165 g = 180 b = 202 a = 0 
	cnear = 12 cfar = 20000

	load_script = Load_SD
}
SCRIPT Load_SD
	Load_Level_Func { Def_SD }
ENDSCRIPT

SCRIPT Load_HI
	AddMusicTrack "ambience\thug1\hi"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\hi\hi.bsp" lev_sky = "levels_ug1\hi\hi_sky.bsp" lev_qb = "levels_ug1\hi\hi.qb" loadscr = "images\load\ug1_hi.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_VC
	AddMusicTrack "ambience\thug1\vc"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\vc\vc.bsp" lev_sky = "levels_ug1\vc\vc_sky.bsp" lev_qb = "levels_ug1\vc\vc.qb" loadscr = "images\load\ug1_vc.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SJ
	AddMusicTrack "ambience\thug1\sj"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\sj\sj.bsp" lev_sky = "levels_ug1\sj\sj_sky.bsp" lev_qb = "levels_ug1\sj\sj.qb" loadscr = "images\load\ug1_sj.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_RU
	AddMusicTrack "ambience\thug1\ru"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\ru\ru.bsp" lev_sky = "levels_ug1\ru\ru_sky.bsp" lev_qb = "levels_ug1\ru\ru.qb" loadscr = "images\load\ug1_ru.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT

SCRIPT Load_SE
	AddMusicTrack "ambience\thug1\se"
	SetUpRioDeJaneiroLights
	Load_Level_Func levelnum = LevelNum_New lev_bsp = "levels_ug1\se\se.bsp" lev_sky = "levels_ug1\se\se_sky.bsp" lev_qb = "levels_ug1\se\se.qb" loadscr = "images\load\ug1_se.png"
	PrepareLevelFog r = 84 g = 84 b = 150 a = 0 cnear = 13 cfar = 20000
ENDSCRIPT



master_level_list = [
	//THPS3
	{ Def_Foun } { Def_Can } { Def_Rio } { Def_Sub } { Def_Ap }
	{ level_name = "Skater Island" load_script = Load_Si level_id = skatersisland_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_SKATERISLAND supports_ctf supports_own supports_bball }
	{ level_name = "Los Angeles" load_script = Load_La level_id = la_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_LOSANGELES supports_ctf supports_own supports_bball }
	{ level_name = "Tokyo" load_script = Load_Tok level_id = tokyo_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_TOKYO supports_ctf supports_own supports_bball }
	{ level_name = "Cruise Ship" load_script = Load_Shp level_id = ship_id regular_level th3_level once_on_startup = StartRunScript once_on_exit = EndRunScript unlock_flag = LEVEL_UNLOCKED_SHIP special_flag = SPECIAL_HAS_SEEN_SHIP supports_ctf supports_own supports_bball }
	{ level_name = "Oil Rig" load_script = Load_Oil level_id = oil_id NoCareer regular_level th3_level supports_ctf supports_own supports_bball }
	{ level_name = "Paris" load_script = Load_Roof level_id = rooftops_id NoCareer regular_level th3_level supports_ctf supports_own supports_bball }
	{ level_name = "Play custom park" load_script = custom_park level_id = custom_park debug_level }
	{ level_name = "Tutorials" load_script = Load_Tut level_id = tutorials_id NoCareer once_on_startup = CPF_Tut_LoadingScript }
	{ level_name = "Foo" load_script = Load_Foo level_id = foo_id debug_level }
	{ level_name = "Skate Shop" load_script = Load_SkateShop level_id = skateshop_id debug_level }

	//THPS1
	{ Def_Ware } { Def_Sc1 } { Def_Mall } { Def_Vans } { Def_Down } { Def_Jam } { Def_Burn } { Def_SF1 } { Def_Ros }

	//THPS2
	{ Def_Han } { Def_Sc2 } { Def_Mar } { Def_NY1 } { Def_Ven } { Def_Ssv } { Def_Ph } { Def_Bul } { Def_Drop } { Def_Hvn }

	//THPS2X
	{ Def_Club } { Def_Cons } { Def_Flor } { Def_Sway } { Def_Sky }

	//THPS4	
	{ Def_Trn } { Def_Sch } { Def_Sf2 } { Def_Alc } { Def_Kon } { Def_Jnk } { Def_Lon } { Def_Zoo } { Def_Cnv } { Def_Hof } { Def_Mot }

	//THUG
	{ Def_NJ } { Def_NY } { Def_FL } { Def_SD }
	{ level_name = "Hawaii" load_script = Load_HI level_id = hawaii_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Vancouver" load_script = Load_VC level_id = vancouver_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Slam City Jam" load_script = Load_SJ level_id = slam_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Moscow" load_script = Load_RU level_id = moscow_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }
	{ level_name = "Hotter Than Hell" load_script = Load_SE level_id = hotter_id NoCareer regular_level ug1_level supports_ctf supports_own supports_bball }

	//THUG2
	{ level_name = "Boston" load_script = Load_BO_ug2 level_id = BO_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Barcelona" load_script = Load_BA_ug2 level_id = BA_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Berlin" load_script = Load_BE_ug2 level_id = BE_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Australia" load_script = Load_AU_ug2 level_id = AU_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "New Orleans" load_script = Load_NO_ug2 level_id = NO_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }	
	{ level_name = "Skatopia" load_script = Load_ST_ug2 level_id = ST_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "Pro Skater" load_script = Load_SE_ug2 level_id = SE_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
	{ level_name = "The Triangle" load_script = Load_SE2_ug2 level_id = SE2_id NoCareer regular_level ug2_level supports_ctf supports_own supports_bball }
]
