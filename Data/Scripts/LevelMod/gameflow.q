SCRIPT WaitFrameLoop
	BEGIN
		Wait 1 gameframe
	REPEAT
ENDSCRIPT

SCRIPT InitializeGameFlow
	IF OnServer
		printf "*** START SERVER HERE ***"
		SetServerMode
		StartServer
	ENDIF
	IF InFrontEnd
	ELSE
		LaunchMenuScreen screen = game_menu not_active
	ENDIF
	IF OnServer
		printf "*** JOIN SERVER HERE ***"
		JoinServer <...>
		CreatePanels
	ENDIF
	printf "***************************    LOADING LEVEL ****************************************************"
	LoadRequestedLevel
	IF InFrontEnd
		InitializeFrontEndSprites
		InitTVScreensVisibility
		LoadSkaters
		SpawnScript helper_scanner
	ENDIF
	StandardGameFlow
ENDSCRIPT

SCRIPT ChangeLevelGameFlow
	change FirstTimeInSplitScreen = 1
	LoadRequestedLevel
	ReinsertSkaters
	IF InNetGame
		SetGameType netlobby
		SetCurrentGameType
	ENDIF
	BEGIN
		IF InFrontEnd
			IF SkatersAreReady
				BREAK
			ENDIF
			Wait 1 gameframe
		ELSE
			Wait 1 gameframe
			IF SkatersAreReady
				BREAK
			ENDIF
		ENDIF
	REPEAT
	WaitFrameLoop
ENDSCRIPT

SCRIPT RestartGameFlow
	StandardGameFlow
ENDSCRIPT

SCRIPT pause_game_flow
	printf "Pausing game flow"
	PauseGameFlow
	Wait 1 gameframe
ENDSCRIPT

SCRIPT unpause_game_flow
	printf "Unpausing game flow"
	UnpauseGameFlow
ENDSCRIPT
FirstTimeInGameFlow_Startup = 1

SCRIPT GameFlow_Startup
	printf "Running standard game flow"
	IF GameModeEquals is_career
		SetGlobalFlag flag = SKATESHOP_RETURN_FROM_CAREER
	ELSE
		IF GameModeEquals is_singlesession
			SetGlobalFlag flag = SKATESHOP_RETURN_FROM_SINGLE
		ELSE
			IF GameModeEquals is_parkeditor
				SetGlobalFlag flag = SKATESHOP_RETURN_FROM_PARKEDIT
			ELSE
				IF InNetGame
					SetGlobalFlag flag = SKATESHOP_RETURN_FROM_ONLINE
				ELSE
					SetGlobalFlag flag = SKATESHOP_RETURN_FROM_FREESKATE
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	DisablePause
	UnSetWaitForGoalFlags
	SetScreenMode one_camera
	BEGIN
		IF InFrontEnd
			IF SkatersAreReady
				BREAK
			ENDIF
			Wait 1 gameframe
		ELSE
			Wait 1 gameframe
			IF SkatersAreReady
				BREAK
			ENDIF
		ENDIF
	REPEAT
	HideClock
	IF GameModeEquals is_parkeditor
		printf "freezing skater"
		Ed_RemoveSkater
	ENDIF
	RestartLevel
	IF InFrontEnd
		WaitFrameLoop
	ENDIF
	IF GameModeEquals is_parkeditor
	ELSE
		RefreshGameMenu
	ENDIF
	InitializeSkaters
	KillMessages
	TogglePanel 0
	IF CareerLevelIs LevelNum_Tutorials
		PauseMusic 1
	ELSE
	ENDIF
	PauseStream 0
	HideLoadingScreen
	IF InNetGame
		IF GameModeEquals is_lobby
			IF OnServer
				server_enter_free_skate
			ELSE
				client_enter_free_skate
			ENDIF
		ENDIF
	ENDIF
	IF IsTrue FirstTimeFromSkateshop
		change FirstTimeFromSkateshop = 0
	ELSE
	ENDIF
	IF InSplitScreenGame
		ResetSkaters
		IF IsTrue FirstTimeInSplitScreen
			MakeSkaterGosub remove_skater_from_world skater = 0
			MakeSkaterGosub remove_skater_from_world skater = 1
			SwitchToMenu menu = splitscreen_setup_menu
			pause_game_flow
			change FirstTimeInSplitScreen = 0
		ENDIF
		ApplySplitScreenOptions
		MakeSkaterGosub add_skater_to_world skater = 0
		MakeSkaterGosub add_skater_to_world skater = 1
	ENDIF
	SetScreenModeFromGameMode
	IF ShouldRunIntroScript
		DisablePause
		SpawnLevelScript target = once_on_startup callback = unpause_game_flow
	ENDIF
	BEGIN
		IF IntroScriptFinished
			BREAK
		ENDIF
		Wait 1 gameframe
	REPEAT
	IF IsCompetition
		IF CareerLevelIs LevelNum_Rio
			StartCompetition RioCompParams RioScoreParams
		ENDIF
		IF CareerLevelIs LevelNum_SkaterIsland
			StartCompetition SICompParams SIScoreParams
		ENDIF
		IF CareerLevelIs LevelNum_Tokyo
			StartCompetition TokCompParams TokScoreParams
		ENDIF
	ENDIF
	IF GameModeEquals is_horse
		StartHorse
	ENDIF
ENDSCRIPT

SCRIPT GameFlow_StartRun
	IF IsCompetition
		StartCompetitionRun
		PlaySkaterCamAnim skater = 0 stop
		KillMessages
		DisablePause
		IF CareerLevelIs LevelNum_Rio
			printf "going to spawn comp_wait_runstart"
			Comp_Wait_RunStart_Setup RioCompParams
		ENDIF
		IF CareerLevelIs LevelNum_SkaterIsland
			Comp_Wait_RunStart_Setup SICompParams
		ENDIF
		IF CareerLevelIs LevelNum_Tokyo
			Comp_Wait_RunStart_Setup TokCompParams
		ENDIF
		pause_game_flow
	ELSE
		PlaySkaterCamAnim skater = 0 stop
	ENDIF
	TogglePanel 1
	DisablePause
	IF ReplayModeEquals REPLAY_MODE_PLAY_SAVED
		SwitchToMenu menu = replay_from_memcard_menu DoNotOpen
		PauseStream 0
	ELSE
		IF ReplayModeEquals REPLAY_MODE_PLAY_AFTER_RUN
			SwitchToMenu menu = replay_menu DoNotOpen
			PauseStream 0
		ENDIF
	ENDIF
	enable_replays
	IF InMultiPlayerGame
	ELSE
	ENDIF
	ReplayInit
	IF TestGameType parkeditor
	ELSE
		ResetLevel
	ENDIF
	IF IsCareerMode
		KillTrickPoints
		SetUpSkaterDefaults
		CreateTrickPoints2
		CreateSkateLines2
		CreateDeckIcon
		CreatePhotoGuy
		UnSetGlobalFlag flag = PROMPT_FOR_SAVE
	ENDIF
	IF CareerLevelIs LevelNum_Tutorials
		PauseMusic 1
	ELSE
		printf "starting a run....skip tracks and crank up the music"
		LM_PrintInfo
		IF GameModeEquals is_horse
		ELSE
			SkipMusicTrack
		ENDIF
		PauseMusic 0
	ENDIF
	IF GameModeEquals default_time_limit
		ResetClock
		UnpauseClock
		ShowClock
	ELSE
		HideClock
	ENDIF
	SetTeamMode 0
	IF GameModeEquals is_king
		SpawnCrown
		SpawnCompass
	ENDIF
	IF GameModeEquals is_horse
		horse_start_run
	ENDIF
	IF GameModeChecksumEquals ctf
		SpawnCTF
		SetTeamMode 1
		SpawnCompass
	ENDIF
	IF GameModeChecksumEquals ownthezone
		SpawnZones
		SetTeamMode 1
		SpawnCompass
	ENDIF
	IF GameModeChecksumEquals beachball
		SpawnBeachball
		SpawnCompass
	ENDIF
	SetTeamMode 2
	ResetSkaters
	IF InMultiPlayerGame
		SpawnScript Splitscreen_StartupScript
	ENDIF
	IF InNetGame
		IF OnServer
		ELSE
			LaunchQueuedScripts
			IF IsObserving
				ShowAllObjects
			ENDIF
		ENDIF
	ENDIF
	IF InNetGame
		LoadMultiplayerSounds
		IF GameModeEquals is_lobby
			IF IsTrue FirstTimeInGameFlow_Startup
				change FirstTimeInGameFlow_Startup = 0
				InvokeScreen screen = ss_main_window
			ELSE
				DoDeferredStart
			ENDIF
		ENDIF
	ENDIF
	IF GameModeChecksumEquals netgraffiti
		GrafStarted
	ENDIF
	//Added to load custom shaders
	OnPostLevelLoad
	LeaveObserveMode2
	printf "going through option list"
	ForEachIn LevelModOptions do = OptionsOnStartGame params = <...>
ENDSCRIPT

SCRIPT GameFlow_PlayRun
	Wait 10 gameframe
	IF InMultiPlayerGame
		UseBothPadsInFrontEnd
	ELSE
		UseOnePadInFrontEnd
	ENDIF
	EnableActuators
	EnablePause
	BEGIN
		IF ShouldEndRun
			BREAK
		ENDIF
		IF IsCompetition
			IF CompetitionEnded
				BREAK
			ENDIF
		ENDIF
		IF GameModeEquals is_horse
			IF FirstTrickStarted
				HideClock
				BREAK
			ENDIF
		ENDIF
		Wait 1 gameframe
	REPEAT
ENDSCRIPT

SCRIPT GameFlow_WaitEnd
	BEGIN
		IF EndRunSelected
			BREAK
		ENDIF
		IF IsCompetition
			IF CompetitionEnded
				BREAK
			ENDIF
		ENDIF
		IF AllSkatersAreIdle
			BREAK
		ENDIF
		Wait 1 gameframe
	REPEAT
	EnableActuators 0
	printf "About to disable"
	DisablePause
	Wait 2 Game frames
	UnpauseGame
	DisablePause
	FrontEndSetInactive
	DisablePause
	Wait 2 Game frames
	TogglePanel 0
	KillMessages
	SetReplayMode REPLAY_MODE_OFF
	KillSpawnedScript name = SK3_Killskater_Finish
ENDSCRIPT

SCRIPT GameFlow_End
	IF IsCompetition
		placing_screen
	ENDIF
	BEGIN
		IF CalculateFinalScores
			BREAK
		ENDIF
		Wait 1 gameframe
	REPEAT
	SpawnLevelScript target = once_on_exit
	BEGIN
		IF IntroScriptFinished
			BREAK
		ENDIF
		Wait 1 gameframe
	REPEAT
	IF IsCareerMode
		Goal_CheckProVideoUnlock
		IF GetGlobalFlag flag = SHOW_CREDITS
			UnSetGlobalFlag flag = SHOW_CREDITS
			IF CD
				Wait 1 gameframe
				ingame_play_movie "movies\credits"
			ENDIF
		ENDIF
		IF IsCompetition
		ELSE
			UpdateRecords
			Statistics_screen
		ENDIF
	ELSE
		IF IsCustomPark
		ELSE
			IF GameModeEquals is_singlesession
				UpdateRecords
				Statistics_screen
			ENDIF
		ENDIF
	ENDIF
	change EnteringStatsMenuFromGameFlow = 1
	IF JustGotFlag flag = GOAL_STAT_POINT1
		printf "stat point"
		SwitchToMenu menu = stats_menu
		pause_game_flow
	ELSE
		IF JustGotFlag flag = GOAL_STAT_POINT2
			printf "stat point"
			SwitchToMenu menu = stats_menu
			pause_game_flow
		ELSE
			IF JustGotFlag flag = GOAL_STAT_POINT3
				printf "stat point"
				SwitchToMenu menu = stats_menu
				pause_game_flow
			ELSE
				IF JustGotFlag flag = GOAL_STAT_POINT4
					printf "stat point"
					SwitchToMenu menu = stats_menu
					pause_game_flow
				ELSE
					IF JustGotFlag flag = GOAL_STAT_POINT5
						printf "stat point"
						SwitchToMenu menu = stats_menu
						pause_game_flow
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	change EnteringStatsMenuFromGameFlow = 0
	helper_select_choose_back_centered
	IF InNetGame
		SwitchToMenu menu = end_run_menu DontPauseWhenActive
	ELSE
		IF IsCareerMode
			IF GetGlobalFlag flag = PROMPT_FOR_SAVE
				SwitchToMenu menu = savegame_yesno_menu
			ELSE
				SwitchToMenu menu = end_run_menu
			ENDIF
		ELSE
			SwitchToMenu menu = end_run_menu
		ENDIF
	ENDIF
	IF GameModeEquals show_ranking_screen
		helper_hide
		KillMessages all_panels
		SwitchToMenu menu = winner_screen
	ENDIF
	IF InNetGame
		IF OnServer
			Wait 5 gameframes
			LoadPendingPlayers
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT StandardGameFlow
	GameFlow_Startup
	BEGIN
		GameFlow_StartRun
		IF ReplayModeEquals REPLAY_MODE_PLAY_SAVED
			GameFlow_PlayRun
			GameFlow_WaitEnd
			SwitchToMenu menu = replay_from_memcard_menu
			pause_game_flow
		ELSE
			IF ReplayModeEquals REPLAY_MODE_PLAY_AFTER_RUN
				GameFlow_PlayRun
				GameFlow_WaitEnd
				SwitchToMenu menu = replay_menu
				pause_game_flow
			ELSE
				GameFlow_PlayRun
				GameFlow_WaitEnd
				IF IsCompetition
					IF CompetitionEnded
						EndCompetitionRun
						PauseMusic 1
						leader_screen_gameflow
						pause_game_flow
						BREAK
					ELSE
						EndCompetitionRun
						New_Judge_screen_gameflow
						pause_game_flow
						leader_screen_gameflow
						pause_game_flow
						IF RoundIs 3
							BREAK
						ELSE
							InitializeSkaters
						ENDIF
					ENDIF
				ELSE
					IF GameModeEquals is_horse
						IF EndRunSelected
							BREAK
						ENDIF
						horse_end_run
						IF HorseEnded
							BREAK
						ELSE
							InitializeSkaters
						ENDIF
					ELSE
						BREAK
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	REPEAT
	horse_uninit
	GameFlow_End
	WaitFrameLoop
ENDSCRIPT

SCRIPT spawn_movie
	SpawnScript play_movie_task params = { <...> }
ENDSCRIPT

SCRIPT play_movie_task
	playmovie_script <...>
ENDSCRIPT

SCRIPT gf
	Cleanup
	request_level level = Load_Rio
	ChangeLevelGameFlow
ENDSCRIPT

SCRIPT ShowAllObjects
	IF CareerLevelIs LevelNum_Foundry
	ENDIF
	IF CareerLevelIs LevelNum_Canada
		AJC_Script_Can_Network_Startup
	ENDIF
	IF CareerLevelIs LevelNum_Suburbia
	ENDIF
	IF CareerLevelIs LevelNum_SkaterIsland
	ENDIF
	IF CareerLevelIs LevelNum_Airport
		CPF_AP_BeginAt_ShowAll
	ENDIF
	IF CareerLevelIs LevelNum_Rio
	ENDIF
	IF CareerLevelIs LevelNum_LA
	ENDIF
	IF CareerLevelIs LevelNum_Tokyo
		JS_Tok_NetObserver
	ENDIF
	IF CareerLevelIs LevelNum_Ship
		BDJ_SHP_NetObserverStartup
	ENDIF
	IF LevelIs Load_Ware
		JKU_Ware_IsObserver
	ENDIF
ENDSCRIPT

SCRIPT Splitscreen_StartupScript
	IF LevelIs Load_Shp
		printf "Launching BDJ_SHP_LoadingScript................................."
		BDJ_SHP_LoadingScript
	ENDIF
ENDSCRIPT

SCRIPT LoadMultiplayerSounds
	IF GameModeChecksumEquals ctf
		LoadSound "MP\ctf_flag_captured1_crowd" vol = 100
		LoadSound "MP\ctf_flag_captured2_crowd" vol = 100
		LoadSound "MP\ctf_flag_dropped" vol = 100
		LoadSound "MP\ctf_flag_returned" vol = 100
		LoadSound "MP\ctf_flag_taken1_crowd" vol = 100
		LoadSound "MP\ctf_flag_taken2_crowd" vol = 100
	ENDIF
	IF GameModeChecksumEquals ownthezone
		LoadSound "MP\zone_control_all" vol = 100
		LoadSound "MP\zone_control_zone" vol = 100
		LoadSound "MP\zone_drop_key" vol = 100
		LoadSound "MP\zone_get_key" vol = 100
		LoadSound "MP\zone_lose_all" vol = 100
		LoadSound "MP\zone_lose_zone" vol = 100
	ENDIF
	IF GameModeChecksumEquals beachball
		LoadSound "MP\beach_ball_bounce" vol = 100
		LoadSound "MP\beach_ball_dropped" vol = 100
		LoadSound "MP\beach_ball_taken" vol = 100
	ENDIF
ENDSCRIPT
