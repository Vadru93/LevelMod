
load_icon_properties = { x = 50.0 y = 50.0 anim_delay = 16 images = [ "images/panelsprites/wheel_001.png" "images/panelsprites/wheel_002.png" "images/panelsprites/wheel_003.png" "images/panelsprites/wheel_004.png" "images/panelsprites/wheel_005.png" "images/panelsprites/wheel_006.png" ] }
game_menu_properties = { x = 150.0 y = 90.0 w = 320.0 h = 336.0 just_center_x just_center_y }
EnteringStatsMenuFromGameFlow = 0

SCRIPT PauseMusicAndStreams
	PauseMusic 1
	PauseStream 1
ENDSCRIPT

SCRIPT UnPauseMusicAndStreams
	PauseMusic 0
	PauseStream 0
ENDSCRIPT

SCRIPT do_backend_retry
	spawnscript do_screen_freeze
ENDSCRIPT

SCRIPT do_screen_freeze
	PauseGame
	IF InNetGame
		StartNetworkLobby
	ENDIF
	Wait 2 gameframe
	DisplayLoadingScreen CurrentScreen
	unpausegame
	retry
ENDSCRIPT

SCRIPT do_backend_end_run
	IF InSplitScreenGame
		UseBothPadsInFrontEnd
	ENDIF
	EndRun
ENDSCRIPT

SCRIPT Paused_menu_backevent
	IF CareerLevelIs LevelNum_Tutorials
		PauseStream 0
	ELSE
		UnPauseMusicAndStreams
	ENDIF
ENDSCRIPT

SCRIPT do_backend_continue
	IF CareerLevelIs LevelNum_Tutorials
		PauseStream 0
	ELSE
		UnPauseMusicAndStreams
	ENDIF
ENDSCRIPT

SCRIPT do_backend_continue_delay
	Wait 0.2 seconds
ENDSCRIPT

SCRIPT View_Goal_movies
	printf "viewing movies -----------------------"
	SpawnSkaterScript StartRunScript params = { from_menu }
ENDSCRIPT

SCRIPT View_ListAllGoals
	SpawnSkaterScript MidLevel_GoalCheck params = { from_pause_menu }
ENDSCRIPT

SCRIPT ResetLevelGoals
	UnSetGoal goal = GOAL_HIGHSCORE
	UnSetGoal goal = GOAL_PROSCORE
	UnSetGoal goal = GOAL_SICKSCORE
	UnSetGoal goal = GOAL_SKATE
	UnSetGoal goal = GOAL_TRICKSPOT
	UnSetGoal goal = GOAL_TAPE
	UnSetGoal goal = GOAL_SCRIPTED1
	UnSetGoal goal = GOAL_SCRIPTED2
	UnSetGoal goal = GOAL_SCRIPTED3
	UnSetFlag flag = GOAL_STAT_POINT1
	UnSetFlag flag = GOAL_STAT_POINT2
	UnSetFlag flag = GOAL_STAT_POINT3
	UnSetFlag flag = GOAL_STAT_POINT4
	UnSetFlag flag = GOAL_STAT_POINT5
	UnSetFlag flag = GOAL_STAT_POINTS
	UnSetFlag flag = GOAL_DECK
	LaunchLocalMessage "Goals reset"
	IF CareerLevelIs LevelNum_Canada
		UnSetFlag flag = LEVEL_FLAG_AJC_CAN_LOG_LOADER_SEQUENCE_PLAYED_ALREADY
		UnSetFlag flag = LEVEL_FLAG_AJC_CAN_LOG_LOADER_IS_UP
		SetFlag flag = LEVEL_FLAG_AJC_CAN_LOG_LOADER_IS_DOWN
	ENDIF
	IF CareerLevelIs LevelNum_Ship
		UnSetFlag flag = BDJ_SHP_POOL
		UnSetFlag flag = BDJ_SHP_FERRY
		UnSetFlag flag = BDJ_SHP_NETS
		BDJ_SHP_RestartSetup
	ENDIF
ENDSCRIPT

SCRIPT do_end_comp
	EndCompetition
ENDSCRIPT

SCRIPT test_fragmentation
	BEGIN
		Wait 0.05 second
		printf "Launching Message"
		LaunchLocalMessage id = BottomInfo "Press X to Continue" panel_message_XtoContinue
	REPEAT
ENDSCRIPT

SCRIPT RefreshGameMenu_Replay
	PauseMusicAndStreams
	AddLine parent = game_menu id = backend_continue text = "Continue" target = "do_backend_continue" kill_menu
	AddLine parent = end_run_menu id = backend_watchreplay text = "Restart Replay" target = "WatchEndOfRunReplay" kill_menu
	IF ReplayModeEquals REPLAY_MODE_PLAY_AFTER_RUN
		AddLine parent = game_menu id = backend_retry text = "Retry" target = "do_backend_retry" kill_menu
		AddLine parent = game_menu id = save_replay_from_pause text = "Save Replay" link = "_EndRunSaveReplay"
		AddLine parent = game_menu id = exit_replay_from_pause text = "Exit" link = "ExitReplayAfterEndOfRun"
	ELSE
		AddLine parent = game_menu id = exit_replay_from_pause text = "Exit" link = "ExitSavedReplay"
	ENDIF
ENDSCRIPT
bPrintDebug = 0
bShowConsole = 0
SlapIsOn = 1

SCRIPT ToggleSlap
	IF IsTrue SlapIsOn
		Change InternetClientCollRadius = 0.0
		Change InternetServerCollRadius = 0.0
		Change SlapIsOn = 0
	ELSE
		Change InternetClientCollRadius = 120.0
		Change InternetServerCollRadius = 94.0
		Change SlapIsOn = 1
	ENDIF
	UpdateSlapText
	//UpdateLevelModSettings
ENDSCRIPT

SCRIPT UpdateSlapText
	IF IsTrue SlapIsOn
		SetMenuElementText id = slap_toggle "Collision: On"
	ELSE
		SetMenuElementText id = slap_toggle "Collision: Off"
	ENDIF
ENDSCRIPT
//HUDIsOn = 1
//This is an option and should use IsOptionOn
SCRIPT ToggleHUD
    ToggleOption LM_GUI_bShowHud
	UpdateShowHud
	UpdateHUDText
	//UpdateLevelModSettings
ENDSCRIPT

SCRIPT UpdateHUDText
	IF IsOptionOn LM_GUI_bShowHud
		SetMenuElementText id = HUD_Toggle "HUD: On"
	ELSE
		SetMenuElementText id = HUD_Toggle "HUD: Off"
	ENDIF
ENDSCRIPT
NetNameIsOn = 1

SCRIPT ToggleNetName
	IF IsTrue NetNameIsOn
		TogglePlayerNames 0
		Change NetNameIsOn = 0
	ELSE
		TogglePlayerNames 1
		Change NetNameIsOn = 1
	ENDIF
	UpdateNetNameText
	//UpdateLevelModSettings
ENDSCRIPT

SCRIPT UpdateNetNameText
	IF IsTrue NetNameIsOn
		SetMenuElementText id = NetName_Toggle "Player names: On"
	ELSE
		SetMenuElementText id = NetName_Toggle "Player names: Off"
	ENDIF
ENDSCRIPT
//ExtraLayersDisabled = 0

//SCRIPT ToggleLayers
//	ToggleOption LM_GameOption_bExtraLayer
//	UpdateExtraLayer
//	UpdateLayersText
//ENDSCRIPT

//SCRIPT UpdateLayersText
//	IF IsOptionOn LM_GameOption_bExtraLayer
//		SetMenuElementText id = Layers_Toggle "Layers: On"
//	ELSE
//		SetMenuElementText id = Layers_Toggle "Layers: Off"
//	ENDIF
//ENDSCRIPT

//This is an option and should use IsOptionOn
//SCRIPT ToggleGrass
//    ToggleOption LM_GameOption_bGrass
//	UpdateGrass
//	UpdateGrassText
//ENDSCRIPT

//SCRIPT UpdateGrassText
//	IF IsOptionOn LM_GameOption_bGrass
//		SetMenuElementText id = Grass_Toggle "Grass: On"
//	ELSE
//		SetMenuElementText id = Grass_Toggle "Grass: Off"
//	ENDIF
//ENDSCRIPT

SCRIPT nextobsrv
	ObserveNext
ENDSCRIPT

SCRIPT enterobsrv
	EnterObserveMode2
ENDSCRIPT

SCRIPT Leaveobsrv
	LeaveObserveMode2
ENDSCRIPT
TH4CompDisabled = 1

SCRIPT ToggleTH4Comp
	IF IsTrue TH4CompDisabled
		Change TH4CompDisabled = 0
		UpdateTH4CompText
		Create prefix = "G_Comp_"
		Create prefix = "TRG_G_COMP_"
		Kill prefix = "G_COMPNOT_"
		Kill prefix = "TRG_G_COMPNOT_"
	ELSE
		Change TH4CompDisabled = 1
		UpdateTH4CompText
		Kill prefix = "G_Comp_"
		Kill prefix = "TRG_G_COMP_"
		Create prefix = "G_COMPNOT_"
		Create prefix = "TRG_G_COMPNOT_"
	ENDIF
ENDSCRIPT

SCRIPT UpdateTH4CompText
	IF IsTrue TH4CompDisabled
		SetMenuElementText id = TH4Comp_Toggle "Competition objects: Off"
	ELSE
		SetMenuElementText id = TH4Comp_Toggle "Competition objects: On"
	ENDIF
ENDSCRIPT
TH4ProDisabled = 1

SCRIPT ToggleTH4Pro
	IF IsTrue TH4ProDisabled
		Change TH4ProDisabled = 0
		UpdateTH4ProText
		Create prefix = "Proset"
		Create prefix = "TRG_Proset"
		Kill prefix = "PROSET1NOT_"
		Kill prefix = "PROSET2NOT_"
		Kill prefix = "PROSET3NOT_"
		Kill prefix = "PROSET4NOT_"
		Kill prefix = "PROSET5NOT_"
		Kill prefix = "PROSET6NOT_"
		Kill prefix = "PROSET7NOT_"
		Kill prefix = "TRG_PROSET1NOT_"
		Kill prefix = "TRG_PROSET2NOT_"
		Kill prefix = "TRG_PROSET3NOT_"
		Kill prefix = "TRG_PROSET4NOT_"
		Kill prefix = "TRG_PROSET5NOT_"
		Kill prefix = "TRG_PROSET6NOT_"
		Kill prefix = "TRG_PROSET7NOT_"
	ELSE
		Change TH4ProDisabled = 1
		UpdateTH4ProText
		Kill prefix = "Proset"
		Kill prefix = "TRG_Proset"
		Create prefix = "PROSET1NOT_"
		Create prefix = "PROSET2NOT_"
		Create prefix = "PROSET3NOT_"
		Create prefix = "PROSET4NOT_"
		Create prefix = "PROSET5NOT_"
		Create prefix = "PROSET6NOT_"
		Create prefix = "PROSET7NOT_"
		Create prefix = "TRG_PROSET1NOT_"
		Create prefix = "TRG_PROSET2NOT_"
		Create prefix = "TRG_PROSET3NOT_"
		Create prefix = "TRG_PROSET4NOT_"
		Create prefix = "TRG_PROSET5NOT_"
		Create prefix = "TRG_PROSET6NOT_"
		Create prefix = "TRG_PROSET7NOT_"
	ENDIF
ENDSCRIPT

SCRIPT UpdateTH4ProText
	IF IsTrue TH4ProDisabled
		SetMenuElementText id = TH4Pro_Toggle "Pro Trick objects: Off"
	ELSE
		SetMenuElementText id = TH4Pro_Toggle "Pro Trick objects: On"
	ENDIF
ENDSCRIPT

//SCRIPT ToggleSky
//	IF IsTrue NetSkyIsOn
//		loadlevelgeometry level = <lev_bsp> Sky = ""
//		Change NetSkyIsOn = 0
//	ELSE
//		loadlevelgeometry level = <lev_bsp> Sky = lev_sky
//		Change NetSkyIsOn = 1
//	ENDIF
//	UpdateNetSkyText
//	//UpdateLevelModSettings
//ENDSCRIPT

//SCRIPT UpdateNetSkyText
//	IF IsTrue NetSkyIsOn
//		SetMenuElementText id = Sky_Toggle "Sky: On"
//	ELSE
//		SetMenuElementText id = Sky_Toggle "Sky: Off"
//	ENDIF
//ENDSCRIPT

SCRIPT UpdateOptionsMenu
    AddLine parent = new_net_options_menu Type = textmenuelement id = levelmod_settings_item text = "LevelMod Settings" eventhandler = { Type = ChooseEventHandler link = Levelmod_menu_root }
	AddLine parent = new_net_options_menu Type = textmenuelement id = options_menu_edit_tricks_item text = "Edit Tricks" eventhandler = { Type = ChooseEventHandler link = trick_menu_container }
	AddLine parent = new_net_options_menu Type = textmenuelement id = options_menu_sound_options_item text = "Sound Options" eventhandler = { Type = ChooseEventHandler link = sfx_menu_container }
	AddLine parent = new_net_options_menu Type = textmenuelement id = options_menu_graf_pos_item text = "Tag counter options" eventhandler = { Type = ChooseEventHandler link = graf_counter_menu }
	AddLine parent = new_net_options_menu id = NetName_Toggle text = "Player names: --" target = "ToggleNetName"
	UpdateNetNameText
	AddLine parent = new_net_options_menu id = HUD_Toggle text = "HUD: --" target = "ToggleHUD"
	UpdateHUDText
	
	//options moved to levelmod menu
	//AddLine parent = new_net_options_menu id = Sky_Toggle text = "Sky: --" target = "ToggleSky"
	//UpdateNetSkyText
	//AddLine parent = new_net_options_menu id = Layers_Toggle text = "Layers: --" target = "ToggleLayers"
	//UpdateLayersText
	//AddLine parent = new_net_options_menu id = Grass_Toggle text = "Grass: --" target = "ToggleGrass"
	//UpdateGrassText
	
	//AddLine parent = new_net_options_menu id = TH4Pro_Toggle text = "Pro Trick Objects: --" target = "ToggleTH4Pro"
	//UpdateTH4ProText
	//AddLine parent = new_net_options_menu id = TH4Comp_Toggle text = "Competition objects: --" target = "ToggleTH4Comp"
	//UpdateTH4CompText
	AddLine parent = new_net_options_menu id = options_menu_back_item text = "Back" target = "go_back" params = { id = new_net_options_menu }
ENDSCRIPT

SCRIPT RefreshGameMenu
	IF InNetGame
		EnablePause
		AddLine parent = game_menu id = game_menu_settings_window text = "Esc" kill_menu eventhandler = { Type = ChooseEventHandler target = "invokeUIScreen" params = { screen = ss_main_window } }
		AddLine parent = game_menu id = game_menu_continue_item text = "Continue" target = "do_backend_continue" kill_menu
		IF OnServer
			AddLine parent = game_menu Type = textmenuelement id = game_menu_game_options_item text = "Host Options" eventhandler = { Type = ChooseEventHandler link = net_game_options_menu }
			AddLine parent = game_menu Type = textmenuelement id = Start_game_menu_item text = "Start Game" eventhandler = { Type = ChooseEventHandler link = server_pause_menu }
			AddLine parent = game_menu Type = textmenuelement id = game_menu_change_level_item text = "Change Level" eventhandler = { Type = ChooseEventHandler link = GameChangeLevelMenu }
		ENDIF
		AddLine parent = game_menu Type = textmenuelement id = wtff1 text = "Set Cust Restart" target = "SetCustRestart" kill_menu
		AddLine parent = game_menu Type = textmenuelement id = wtf2 text = "Goto Cust Restart" target = "GotoCustRestart" kill_menu
		AddLine parent = game_menu Type = textmenuelement id = wtf3 text = "Enter Obs" target = "enterobsrv" kill_menu
		AddLine parent = game_menu Type = textmenuelement id = wtf4 text = "next" target = "nextobsrv" kill_menu
		AddLine parent = game_menu Type = textmenuelement id = wtf5 text = "Leave Obs" target = "Leaveobsrv" kill_menu
		AddLine parent = game_menu Type = textmenuelement id = net_options_game_menu text = "Options" eventhandler = { Type = ChooseEventHandler link = new_net_options_menu }
		AddLine parent = game_menu Type = textmenuelement id = game_menu_choose_team_item text = "Choose Team" eventhandlers = [ { Type = ChooseEventHandler link = game_menu_choose_team_menu } ]
		AddLine parent = game_menu Type = textmenuelement id = game_menu_enter_chat_item text = "Chat Message" eventhandler = { Type = ChooseEventHandler link = chat_message_control }
		AddLine parent = game_menu Type = textmenuelement id = game_menu_quit_item text = "Quit" eventhandler = { Type = ChooseEventHandler link = server_quitgame_yesno_menu }
	ELSE
		printf "refreshing"
		helper_select_choose_back_centered
		PauseMusicAndStreams
		AddLine parent = game_menu id = backend_continue text = "Continue" target = "do_backend_continue" kill_menu
		AddLine parent = game_menu id = backend_retry text = "Retry" target = "do_backend_retry" kill_menu
		IF CareerLevelIs LevelNum_Tutorials
		ELSE
			IF IsCareerMode
				AddLine parent = game_menu id = backend_change_level_career text = "Change Level" link = career_change_level_menu
			ELSE
				AddLine parent = game_menu id = backend_change_level_not_career text = "Change Level" link = GameChangeLevelMenu
			ENDIF
		ENDIF
		IF InSplitScreenGame
			DestroyElement id = goto_trickset_menu
		ELSE
			AddLine parent = game_menu id = goto_trickset_menu text = "Edit Tricks" link = trick_menu_container
		ENDIF
		IF IsCareerMode
			IF IsCompetition
				DestroyElement id = backend_resetgoals
				DestroyElement id = ListAllGoals
			ELSE
				AddLine parent = game_menu id = ListAllGoals text = "View Goals List" target = "View_ListAllGoals" kill_menu
				IF CD
					DestroyElement id = backend_resetgoals
				ELSE
					AddLine parent = game_menu id = backend_resetgoals text = "Reset Level Goals" target = "ResetLevelGoals" kill_menu
				ENDIF
			ENDIF
		ELSE
			DestroyElement id = backend_resetgoals
			DestroyElement id = ListAllGoals
		ENDIF
		AddLine parent = game_menu id = backend_sound_options text = "Sound Options" link = sfx_menu_container
		DestroyElement id = backend_cheat_menu
		IF GetGlobalFlag flag = CHEAT_UNLOCKED_1
			AddLine parent = game_menu id = backend_cheat_menu text = "Cheats" link = cheat_menu
		ENDIF
		AddLine parent = game_menu id = goto_restart_menu text = "Skip to Restart" link = restart_menu
		AddLine parent = game_menu Type = textmenuelement id = levelmod_settings_item text = "LevelMod Settings" eventhandler = { Type = ChooseEventHandler link = Levelmod_menu_root }
		AddLine parent = game_menu id = backend_end_run text = "End Run" target = "do_backend_end_run" kill_menu
		IF IsCompetition
			AddLine parent = game_menu id = end_comp text = "End Competition" target = "do_end_comp" kill_menu
		ELSE
			DestroyElement id = end_comp
		ENDIF
		SelectElement id = backend_continue
		AddLine parent = game_menu id = game_menu_debug text = "Debug/Options" link = debug_main_menu
		DestroyElement id = game_menu_debug
		IF CareerLevelIs LevelNum_Tutorials
			IF GetFlag flag = TUTORIAL_RUNNING
				AddLine parent = game_menu id = end_tutorial_item text = "Tutorials Menu" target = "EndCurrentTutorial" kill_menu
			ENDIF
			DestroyElement id = goto_trickset_menu
			DestroyElement id = backend_cheat_menu
			DestroyElement id = backend_retry
			DestroyElement id = goto_restart_menu
			DestroyElement id = game_menu_debug
			DestroyElement id = backend_end_run
		ELSE
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT EndofRun_helper
	helper_hide
	helper_set_positions x1 = 190 x2 = 330
	SetMenuElementText id = helper_menu_1 "ЎҐ Select"
	SetMenuElementText id = helper_menu_2 "¬ Accept"
ENDSCRIPT

SCRIPT Refresh_EndofRunMenu
	EndofRun_helper
	IF InNetGame
	ELSE
		PauseMusicAndStreams
	ENDIF
	IF IsCareerMode
		IF GetGlobalFlag flag = JUST_UNLOCKED_LEVEL
			SelectElement id = backend_change_level
		ELSE
			SelectElement id = backend_retry2
		ENDIF
	ELSE
		SelectElement id = backend_retry2
	ENDIF
	IF IsCompetition
		AddLine parent = end_run_menu id = backend_retry2 text = "Restart Comp" target = "do_backend_retry" kill_menu
	ELSE
		AddLine parent = end_run_menu id = backend_retry2 text = "Retry" target = "do_backend_retry" kill_menu
	ENDIF
	IF InSplitScreenGame
		AddLine parent = end_run_menu id = gameoptions_item text = "Game options" link = splitscreen_setup_menu
	ELSE
		DestroyElement id = gameoptions_item
	ENDIF
	IF IsCareerMode
		IF IsCompetition
			DestroyElement id = backend_watchreplay
		ELSE
			AddLine parent = end_run_menu id = backend_watchreplay text = "Watch Replay" target = "WatchEndOfRunReplay" kill_menu
		ENDIF
	ELSE
		IF GameModeEquals is_singlesession
			IF IsCustomPark
				DestroyElement id = backend_watchreplay
			ELSE
				AddLine parent = end_run_menu id = backend_watchreplay text = "Watch Replay" target = "WatchEndOfRunReplay" kill_menu
			ENDIF
		ELSE
			DestroyElement id = backend_watchreplay
		ENDIF
	ENDIF
	IF IsTrue ShouldDisableReplays
		DestroyElement id = backend_watchreplay
	ENDIF
	IF IsCareerMode
		IF IsCompetition
			DestroyElement id = view_goals_movies_element
		ELSE
			AddLine parent = end_run_menu id = view_goals_movies_element text = "View Goal Movies" target = "View_Goal_movies" kill_menu
		ENDIF
	ELSE
		DestroyElement id = view_goals_movies_element
	ENDIF
	IF InSplitScreenGame
		DestroyElement id = goto_trickset_menu
	ELSE
	ENDIF
	IF IsCareerMode
		AddLine parent = end_run_menu id = backend_change_level text = "Change Level" link = career_change_level_menu
	ELSE
		AddLine parent = end_run_menu id = goto_server_menu text = "Change Level" link = GameChangeLevelMenu
	ENDIF
	IF IsCustomPark
		DestroyElement id = backend_change_level
	ENDIF
	IF InSplitScreenGame
		DestroyElement id = edit_stats
		DestroyElement id = goto_trickset_menu
		DestroyElement id = save_game
	ELSE
		AddLine parent = end_run_menu id = edit_stats text = "Edit stats" link = stats_menu
		AddLine parent = end_run_menu id = goto_trickset_menu text = "Edit Tricks" link = trick_menu_container
		AddLine parent = end_run_menu id = save_game text = "Save game" target = "InGameSave"
	ENDIF
	DestroyElement id = endrun_cheat_menu
	IF InNetGame
	ELSE
		IF GetGlobalFlag flag = CHEAT_UNLOCKED_1
			AddLine parent = end_run_menu id = endrun_cheat_menu text = "Cheats" link = cheat_menu
		ENDIF
	ENDIF
	AddLine parent = end_run_menu id = main_menu text = "Quit" link = quitgame_yesno_menu
ENDSCRIPT

SCRIPT goto_skateshop
	Change AutoLinkToCareerScreen = 1
	chosen_leave_server
ENDSCRIPT

SCRIPT InGameEndMemCardSaveLoad
	SwitchToMenu menu = end_run_menu
ENDSCRIPT

SCRIPT _InGameEndMemCardSaveLoad
	spawnscript InGameEndMemCardSaveLoad params = <...>
ENDSCRIPT

SCRIPT InGameSave
	helper_hide
	spawnscript SaveOptionsAndPros params = { EndScr = InGameEndMemCardSaveLoad }
ENDSCRIPT

SCRIPT Continue_to_endofrun
	SwitchToMenu menu = end_run_menu
ENDSCRIPT

SCRIPT front_end_set_inactive
	FrontEndSetInactive
	IF CareerLevelIs LevelNum_Tutorials
		PauseStream 0
	ELSE
		UnPauseMusicAndStreams
	ENDIF
ENDSCRIPT

SCRIPT Select_No
	SelectElement id = quit_no
	helper_select_choose_back_centered
ENDSCRIPT

SCRIPT Server_Select_No
	SelectElement id = server_quit_no
	helper_select_choose_back_centered
ENDSCRIPT

SCRIPT Client_Select_No
	SelectElement id = client_quit_no
	helper_select_choose_back_centered
ENDSCRIPT

SCRIPT pop_th1
	DepopulateMenu id = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th1_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
ENDSCRIPT

SCRIPT pop_th2
	DepopulateMenu id = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th2_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
ENDSCRIPT

SCRIPT pop_th2x
	DepopulateMenu id = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th2x_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
ENDSCRIPT

SCRIPT pop_th3
	DepopulateMenu id = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th3_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
ENDSCRIPT

SCRIPT pop_th4
	DepopulateMenu id = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th4_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
ENDSCRIPT

SCRIPT pop_ug1
	DepopulateMenu id = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = ug1_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
ENDSCRIPT

SCRIPT pop_ug2
	DepopulateMenu id = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = ug2_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
ENDSCRIPT

SCRIPT setup_backend_menu
	setdefaultproperty Type = textdrawer name = main
	CreateMenu { Type = menucontainer id = contain1 dynamic_children = [ { id = sfx_menu_container target_script = sfx_menu_create }
			{ id = trick_menu_container target_script = trick_menu_create } ] }
	createandattachmenu { Type = verticalmenu id = game_menu game_menu_properties blue_top eventhandlers = [ { Type = showeventhandler target = "RefreshGameMenu" }
			{ Type = backeventhandler target = "front_end_set_inactive" } ] children = [ { Type = textmenuelement auto_id text = "paused" static dont_gray drawer = title } ] parent = contain1 }
	createandattachmenu { Type = scrollingmenu id = restart_menu num_visible = 14 x = 100.0 y = 70.0 w = 410.0 h = 224.0 parent = contain1 }
	createandattachmenu { Type = verticalmenu id = savegame_yesno_menu x = 170.0 y = 150.0 w = 300.0 h = 336.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "EndofRun_helper" } parent = contain1 children = [ { Type = textmenuelement auto_id text = "Save Game" static dont_gray drawer = title }
			{ Type = textmenuelement id = savegame_yes text = "Yes" target = "InGameSave" }
			{ Type = textmenuelement id = savegame_no text = "No" target = "Continue_to_endofrun" kill_menu } ] }
	createandattachmenu { Type = verticalmenu id = quitgame_yesno_menu x = 170.0 y = 150.0 w = 300.0 h = 336.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "Select_no" } parent = contain1 children = [ { Type = textmenuelement auto_id text = "Quit?" static dont_gray drawer = title }
			{ Type = textmenuelement id = quit_yes text = "Yes" target = "chosen_leave_server" kill_menu }
			{ Type = textmenuelement id = quit_no text = "No" link = end_run_menu } ] }
	IF ENGLISH
		createandattachmenu { Type = verticalmenu id = end_run_menu game_menu_properties blue_top eventhandler = { Type = showeventhandler target = "Refresh_EndofRunMenu" } children = [ { Type = textmenuelement auto_id text = "end of run" static dont_gray drawer = title } ] parent = contain1 }
	ELSE
		createandattachmenu { Type = verticalmenu id = end_run_menu game_menu_properties w = 340 x = 150 blue_top eventhandler = { Type = showeventhandler target = "Refresh_EndofRunMenu" } children = [ { Type = textmenuelement auto_id text = "end of run" static dont_gray drawer = title } ] parent = contain1 }
	ENDIF
	createandattachmenu { Type = verticalmenu id = victorycondition_menu x = 120.0 y = 70.0 w = 420.0 h = 336.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "set victory" static dont_gray drawer = title }
			{ Type = textmenuelement id = victorycondition_highestscore text = "Highest Score Wins" target = "do_victorycondition_highestscore" adds_task }
			{ Type = textmenuelement id = victorycondition_target10000 text = "First Skater to 10000 Wins" target = "do_victorycondition_target10000" adds_task }
			{ Type = textmenuelement id = victorycondition_target50000 text = "First Skater to 50000 Wins" target = "do_victorycondition_target50000" adds_task } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = gameoptions_menu x = 120.0 y = 70.0 w = 400.0 h = 336.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "game options" static dont_gray drawer = title }
			{ Type = textmenuelement id = gameoptions_victorycondition text = "Set Victory Conditions" link = victorycondition_menu } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = trickset_menu game_menu_properties blue_top children = [ { Type = textmenuelement auto_id text = "trickset" static dont_gray drawer = title }
			{ Type = textmenuelement id = airtricks1 text = "Left+Circle" target = "change_airtricks1" kill_menu }
			{ Type = textmenuelement id = airtricks2 text = "Right+Circle" target = "change_airtricks2" kill_menu } ] parent = contain1 }
	create_splitscreen_menu
	create_and_attach_generic_array_menu { menu_id = network_time_limit_menu array_name = time_limit_options field = time_limit title = "Time Limit" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = network_end_game_zone array_name = choices_endgame_zone field = game_end_type title = "Game End Type" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = network_end_game_cond_control_zone array_name = choices_zone_control_limit field = game_end_cond title = "Game End Condition" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = network_end_game_cond_score_zone array_name = choices_zone_score_limit field = game_end_cond title = "Game End Condition" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = network_end_game_ctf array_name = choices_endgame_ctf field = game_end_type title = "Game End Type" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = network_end_game_cond_ctf array_name = choices_capture_limit field = game_end_cond title = "Game End Condition" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = network_end_game_other array_name = choices_endgame_other field = game_end_type title = "Game End Type" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = network_end_game_cond_other array_name = choices_score_limit field = game_end_cond title = "Game End Condition" prefs = network x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
	createandattachmenu { Type = verticalmenu id = GameChangeLevelMenu x = 40.0 y = 100.0 w = 560.0 h = 224.0 blue_top children = [ { Type = textmenuelement auto_id text = "Change Level" static dont_gray drawer = title }
			{ Type = menucontainer id = level_menu_container show_all_children eventhandler = { Type = leftrightcontroller Left = lmc_level_menu_container Right = rmc_level_menu_container } } ] parent = contain1 }
	CreateMenu { Type = menucontainer id = lmc_level_menu_container }
	attachchild parent = level_menu_container child = lmc_level_menu_container
	CreateMenu { Type = menucontainer id = rmc_level_menu_container }
	attachchild parent = level_menu_container child = rmc_level_menu_container
	CreateMenu { Type = verticalmenu id = GameList x = 0.0 y = 0.0 w = 280.0 h = 224.0 not_rounded children = [ { Type = textmenuelement auto_id text = "THPS1" target = "pop_th1" link = ChangeLevelMenuList no_visit }
			{ Type = textmenuelement auto_id text = "THPS2" target = "pop_th2" link = ChangeLevelMenuList no_visit }
			{ Type = textmenuelement auto_id text = "THPS2x" target = "pop_th2x" link = ChangeLevelMenuList no_visit }
			{ Type = textmenuelement auto_id text = "THPS3" target = "pop_th3" link = ChangeLevelMenuList no_visit }
			{ Type = textmenuelement auto_id text = "THPS4" target = "pop_th4" link = ChangeLevelMenuList no_visit }
			{ Type = textmenuelement auto_id text = "THUG" target = "pop_ug1" link = ChangeLevelMenuList no_visit }
			{ Type = textmenuelement auto_id text = "THUG 2" target = "pop_ug2" link = ChangeLevelMenuList no_visit } ] }
	attachchild parent = lmc_level_menu_container child = GameList
	CreateMenu { Type = verticalmenu id = ChangeLevelMenuList x = 280.0 y = 0.0 w = 280.0 h = 224.0 not_rounded right_side_icon children = [ ] }
	attachchild parent = rmc_level_menu_container child = ChangeLevelMenuList
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th3_level target_script = "change_level" parent_menu = ChangeLevelMenuList }
	createandattachmenu { Type = verticalmenu id = server_pause_menu x = 120.0 y = 80.0 w = 400.0 h = 336.0 eventhandlers = [ { Type = showeventhandler target = "AddGameEndOptions" } ] blue_top children = [ { Type = textmenuelement auto_id text = "Start Game" static dont_gray drawer = title }
			{ Type = textmenuelement id = server_pause_menu_start_game_item text = "Ready" eventhandler = { Type = ChooseEventHandler } }
			{ Type = textmenuelement auto_id text = "Game Type" eventhandler = { Type = ChooseEventHandler link = opts_net_game_type_menu } }
			{ Type = textmenuelement auto_id text = "Current Game" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_type" } } }
			{ Type = textmenuelement auto_id text = "Time Limit" link = network_time_limit_menu }
			{ Type = textmenuelement auto_id text = "Current Time Limit" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "time_limit" } } } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = Start_game_menu game_menu_properties just_center_x just_center_y blue_top eventhandler = { Type = backeventhandler target = "front_end_set_inactive" } children = [ { Type = textmenuelement auto_id text = "Start game" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Ready" target = "GameFlow_StartRun" kill_menu }
			{ Type = textmenuelement auto_id text = "Game Type" eventhandler = { Type = ChooseEventHandler link = opts_net_game_type_menu } }
			{ Type = textmenuelement auto_id text = "Current Game" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_type" } } }
			{ Type = textmenuelement auto_id text = "Time Limit" link = network_time_limit_menu }
			{ Type = textmenuelement auto_id text = "Current Time Limit" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "time_limit" } } } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = new_net_options_menu game_menu_properties just_center_x just_center_y blue_top eventhandlers = [ { Type = showeventhandler target = "UpdateOptionsMenu" } ] children = [ { Type = textmenuelement auto_id text = "Options" static dont_gray drawer = title } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = graf_counter_menu game_menu_properties just_center_x just_center_y blue_top eventhandlers = [ { Type = showeventhandler target = "UpdateGrafCounterMenu" }
			{ Type = backeventhandler target = "RemoveGrafCounterPreview" } ] children = [ { Type = textmenuelement auto_id text = "Tag counter options" static dont_gray drawer = title }
			{ Type = textmenuelement id = graf_toggle_item text = "Count tags: on/off" target = "sToggleGrafCounter" }
			{ Type = slidermenuelement id = CounterPosX text = "Counter Pos X:" lower = 0 upper = 600 delta = 1 start = 547 wrap = 0 right_side_w = 80 eventhandlers = [ { Type = ContentsChangedEventHandler target = "PreviewCounter" } ] }
			{ Type = slidermenuelement id = CounterPosY text = "Counter Pos Y:" lower = 0 upper = 600 delta = 1 start = 286 wrap = 0 right_side_w = 80 eventhandlers = [ { Type = ContentsChangedEventHandler target = "PreviewCounter" } ] }
			{ Type = textmenuelement id = reset_graf_pos_item text = "Reset position" target = "ResetGrafCounterPos" }
			{ Type = textmenuelement auto_id text = "Back" target = "go_back" params = { id = graf_counter_menu } } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = game_menu_choose_team_menu x = 170.0 y = 80.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "Choose Team" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Automatic" eventhandler = { Type = ChooseEventHandler link = game_menu target = "do_choose_team" params = { automatic } } }
			{ Type = textmenuelement auto_id text = "Blue" eventhandler = { Type = ChooseEventHandler link = game_menu target = "do_choose_team" params = { blue } } }
			{ Type = textmenuelement auto_id text = "Red" eventhandler = { Type = ChooseEventHandler link = game_menu target = "do_choose_team" params = { red } } } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = server_choose_team_menu x = 170.0 y = 80.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "Choose Team" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Automatic" eventhandler = { Type = ChooseEventHandler link = server_pause_menu target = "do_choose_team" params = { automatic } } }
			{ Type = textmenuelement auto_id text = "Blue" eventhandler = { Type = ChooseEventHandler link = server_pause_menu target = "do_choose_team" params = { blue } } }
			{ Type = textmenuelement auto_id text = "Red" eventhandler = { Type = ChooseEventHandler link = server_pause_menu target = "do_choose_team" params = { red } } } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = client_choose_team_menu x = 170.0 y = 80.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "Choose Team" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Automatic" eventhandler = { Type = ChooseEventHandler link = server_pause_menu target = "do_choose_team" params = { automatic } } }
			{ Type = textmenuelement auto_id text = "Blue" eventhandler = { Type = ChooseEventHandler link = server_pause_menu target = "do_choose_team" params = { blue } } }
			{ Type = textmenuelement auto_id text = "Red" eventhandler = { Type = ChooseEventHandler link = server_pause_menu target = "do_choose_team" params = { red } } } ] parent = contain1 }
	createandattachmenu { Type = scrollingmenu id = net_level_menu_th4 num_visible = 15 x = 170.0 y = 80.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = showeventhandler target = "populate_level_menu_th4" params = { choose_script = "change_level" parent_menu = net_level_menu_th4 } } ] children = [ { Type = textmenuelement auto_id text = "Levels" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Play Custom Park" target = "_NetMenuParkEditorLoad" } ] parent = contain1 }
	createandattachmenu { Type = scrollingmenu id = net_level_menu num_visible = 15 x = 170.0 y = 80.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = showeventhandler target = "populate_level_menu" params = { choose_script = "change_level" parent_menu = net_level_menu } } ] children = [ { Type = textmenuelement auto_id text = "Levels" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Play Custom Park" target = "_NetMenuParkEditorLoad" } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = client_pause_menu x = 120.0 y = 80.0 w = 400.0 h = 336.0 blue_top eventhandler = { Type = backeventhandler target = "front_end_set_inactive" } children = [ { Type = textmenuelement auto_id text = "Client Pause" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Settings (TEMP)" eventhandler = { kill_menu Type = ChooseEventHandler target = "invokeUIScreen" params = { screen = ss_main_window } } }
			{ Type = textmenuelement id = client_pause_menu_continue_item text = "Continue" eventhandler = { Type = ChooseEventHandler } }
			{ Type = textmenuelement id = client_choose_team_item text = "Choose Team" eventhandlers = [ { Type = ChooseEventHandler link = client_choose_team_menu } ] }
			{ Type = textmenuelement id = client_pause_menu_toggle_names_item text = "Toggle Player Names" }
			{ Type = textmenuelement id = client_pause_menu_toggle_scores_item text = "Toggle Scores" }
			{ Type = textmenuelement id = client_pause_menu_chat_item text = "Enter Chat Message" eventhandler = { Type = ChooseEventHandler link = chat_message_control } }
			{ Type = textmenuelement id = client_pause_sound_options text = "Sound Options" eventhandler = { Type = ChooseEventHandler link = sfx_menu_container } }
			{ Type = textmenuelement id = client_pause_menu_quit_item text = "Quit" eventhandler = { Type = ChooseEventHandler link = client_quitgame_yesno_menu } } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = server_quitgame_yesno_menu x = 170.0 y = 150.0 w = 300.0 h = 336.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "Server_Select_No" } parent = contain1 children = [ { Type = textmenuelement auto_id text = "Quit?" static dont_gray drawer = title }
			{ Type = textmenuelement id = server_quit_yes text = "Yes" target = "chosen_leave_server" }
			{ Type = textmenuelement id = server_quit_no text = "No" link = game_menu } ] }
	createandattachmenu { Type = verticalmenu id = client_quitgame_yesno_menu x = 170.0 y = 150.0 w = 300.0 h = 336.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "Client_Select_No" } parent = contain1 children = [ { Type = textmenuelement auto_id text = "Quit?" static dont_gray drawer = title }
			{ Type = textmenuelement id = client_quit_yes text = "Yes" target = "chosen_leave_server" }
			{ Type = textmenuelement id = client_quit_no text = "No" link = client_pause_menu } ] }
	createandattachmenu { Type = keyboardcontrol id = chat_message_control x = 160.0 y = 56.0 w = 320.0 h = 336.0 just_center_x just_center_y num_columns = 13 standard_charset allow_uppercase allow_lowercase min_chars = 0 max_chars = 150 title_string = "Enter chat message:" default_string = "" eventhandlers = [ { Type = showeventhandler target = "clear_keyboard_string" params = { control_id = chat_message_control } }
			{ Type = ContentsChangedEventHandler target = "send_chat_message" }
			{ Type = showeventhandler target = "helper_on_keyboard_control" } ] kill_on_close parent = contain1 }
	createandattachmenu { Type = keyboardcontrol id = kb_chat_message_control x = 160.0 y = 56.0 w = 320.0 h = 336.0 just_center_x just_center_y num_columns = 13 no_gui allow_uppercase allow_lowercase allow_slashes min_chars = 0 max_chars = 150 title_string = "Enter chat message:" default_string = "" eventhandlers = [ { Type = showeventhandler target = "clear_keyboard_string" params = { control_id = kb_chat_message_control } }
			{ Type = ContentsChangedEventHandler target = "send_chat_message" }
			{ Type = showeventhandler target = "helper_on_keyboard_control_no_gui" } ] kill_on_close parent = contain1 }
	createandattachmenu { Type = verticalmenu id = net_game_options_menu game_menu_properties just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "UpdateSlapText" } children = [ { Type = textmenuelement auto_id text = "Host Options" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Server Name" eventhandler = { Type = ChooseEventHandler link = opts_net_server_name_control } }
			{ Type = textmenuelement auto_id text = "Current Server Name" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "server_name" } } }
			{ Type = textmenuelement auto_id text = "Password" eventhandler = { Type = ChooseEventHandler link = opts_net_password_control } }
			{ Type = textmenuelement auto_id text = "Current Password" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "password" } } }
			{ Type = textmenuelement auto_id text = "Maximum Players" eventhandler = { Type = ChooseEventHandler link = opts_net_num_players_menu } }
			{ Type = textmenuelement auto_id text = "Current Players" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "num_players" } } }
			{ Type = textmenuelement auto_id text = "Maximum Observers" eventhandler = { Type = ChooseEventHandler link = opts_net_num_obs_menu } }
			{ Type = textmenuelement auto_id text = "Current Observers" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "num_observers" } } }
			{ Type = textmenuelement id = slap_toggle text = "Slap: --" target = "ToggleSlap" }
			{ Type = textmenuelement id = new_menu_ban text = "Ban Player..." eventhandler = { Type = ChooseEventHandler link = ban_player_menu } }
			{ Type = textmenuelement auto_id text = "Back" target = "go_back" params = { id = net_game_options_menu } } ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = net_game_options_menu x = 160.0 y = 56.0 w = 320.0 h = 336.0 just_center_x just_center_y id = ban_player_menu blue_top children = [ { Type = textmenuelement auto_id text = "Ban Player" static dont_gray drawer = title } ] parent = contain1 }
	create_and_attach_generic_array_menu { menu_id = opts_net_num_players_menu array_name = num_players_info field = num_players title = "Number of Players" prefs = network validationscript = "validate_player_option" x = 140.0 y = 120.0 w = 360.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = opts_net_num_obs_menu array_name = num_observers_info field = num_observers title = "Number of Observers" prefs = network x = 120.0 y = 120.0 w = 400.0 h = 400.0 }
	create_and_attach_generic_array_menu { menu_id = opts_net_game_type_menu array_name = game_type_info field = game_type title = "Game Type" prefs = network x = 170.0 y = 120.0 w = 300.0 h = 400.0 }
	createandattachmenu { Type = keyboardcontrol id = opts_net_password_control x = 160.0 y = 56.0 w = 320.0 h = 336.0 just_center_x just_center_y num_columns = 13 standard_charset min_chars = 0 max_chars = 60 title_string = "Enter password:" eventhandlers = [ { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "password" } }
			{ Type = ContentsChangedEventHandler target = "set_preferences_from_ui" params = { prefs = network field = "password" } }
			{ Type = showeventhandler target = "helper_on_keyboard_control" } ] parent = contain1 }
	createandattachmenu { Type = keyboardcontrol id = opts_net_server_name_control x = 160.0 y = 56.0 w = 320.0 h = 336.0 just_center_x just_center_y num_columns = 13 standard_charset min_chars = 0 max_chars = 60 title_string = "Enter server name:" eventhandlers = [ { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "server_name" } }
			{ Type = ContentsChangedEventHandler target = "set_preferences_from_ui" params = { prefs = network field = "server_name" } }
			{ Type = showeventhandler target = "helper_on_keyboard_control" } ] parent = contain1 }
	createandattachmenu { Type = scrollingmenu id = change_level_menu num_visible = 15 x = 160.0 y = 70.0 w = 300.0 h = 400.0 just_center_x just_center_y eventhandler = { Type = showeventhandler target = "populate_level_menu_th3" params = { choose_script = "change_level" parent_menu = change_level_menu } } children = [ ] parent = contain1 }
	createandattachmenu { Type = verticalmenu id = career_change_level_menu x = 140.0 y = 90.0 w = 360.0 h = 400.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "populate_level_menu" params = { choose_script = "change_level" parent_menu = career_level_names } } children = [ { Type = textmenuelement auto_id text = "Levels" static dont_gray drawer = title }
			{ Type = menucontainer id = career_levels_multi_container show_all_children } ] parent = contain1 }
	DestroyElement id = career_level_names
	DestroyElement id = career_level_goals
	CreateMenu { Type = verticalmenu id = career_level_names x = 0.0 y = 0.0 w = 280.0 h = 336.0 just_center_x just_center_y not_rounded children = [ { Type = textmenuelement auto_id text = "Name" static drawer = main_smaller } ] }
	CreateMenu { Type = verticalmenu id = career_level_goals x = 280.0 y = 0.0 w = 80.0 h = 336.0 just_center_x just_center_y not_rounded static eventhandler = { Type = showeventhandler target = "populate_goals_menu" } children = [ { Type = textmenuelement auto_id text = "Goals" static drawer = main_smaller } ] }
	attachchild parent = career_levels_multi_container child = career_level_names
	attachchild parent = career_levels_multi_container child = career_level_goals

	printf "Going through master level list... regular change level"
	attach_new_stats_menu { menu_id = stats_menu container_id = contain1 points_available_id = ingame_cas_points_available left_container_id = stats_left_container right_container_id = stats_right_container }
	AttachEventHandler { Type = showeventhandler object = stats_menu target = "helper_on_stat_menu" }
	helper_menu_create
	debug_menu_create
	level_menu_create
	stats_menu_create
	comp_menu_create
	horse_menu_create
	replay_menu_create
	cheat_menu_create
	replay_from_memcard_menu_create
	memcard_files_menu_create
	
	CreateLevelModMenus
ENDSCRIPT

SCRIPT grafcounter_create
	CreateMenu { Type = menucontainer id = grafcounter_container }
	createandattachmenu { Type = textmenuelement id = grafcounter x = 90.0 y = 350.0 w = 150.0 h = 250.0 just_center_x just_center_y text = "ЎўЈ¤Ґ¦§Ё©Є«¬" static dont_gray drawer = helper_text parent = grafcounter_container dontdrawrect }
ENDSCRIPT
exist = 1

SCRIPT checkifexist
	IF GotParam checksum
		IF PreferenceEquals prefs = network field = <theField> equals = <checksum>
			Change exist = 1
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT ResetGrafCounterPos
	ChangeValues Container = tags_message Values = { pos = (547, 286) }
	LaunchGrafCounter text = "Tags: 0"
ENDSCRIPT

SCRIPT sToggleGrafCounter
	ToggleGrafCounter
	UpdateGrafCounterMenu
ENDSCRIPT

SCRIPT RemoveGrafCounterPreview
	LaunchGrafCounter text = ""
ENDSCRIPT

SCRIPT UpdateGrafCounterMenu
	LaunchGrafCounter text = "Tags: 0"
	IF CounterIsOn
		SetMenuElementText id = graf_toggle_item "Count tags: On"
	ELSE
		SetMenuElementText id = graf_toggle_item "Count tags: Off"
	ENDIF
ENDSCRIPT

SCRIPT AddGameEndOptions
	DestroyElement id = end_game_type
	DestroyElement id = end_game_cond
	DestroyElement id = end_game_opts_type
	DestroyElement id = end_game_opts_cond
	DestroyElement id = back
	IF PreferenceEquals prefs = network field = game_type equals = ownthezone
		Change exist = 0
		ForEachIn choices_endgame_zone do = checkifexist params = { theField = game_end_type }
		IF IsTrue exist
		ELSE
			SetPreference prefs = network field = game_end_type name = "Timed" checksum = choice_timed value = empty_choices array_name = choices_endgame_zone params = { name = "Timed" ui_string = "Timed" checksum = choice_timed value = empty_choices }
		ENDIF
		AddLine Type = textmenuelement id = end_game_type text = "End Game Type" link = network_end_game_zone parent = server_pause_menu
		AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_type text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_type" } }
		IF #"Not" PreferenceEquals prefs = network field = game_end_type equals = choice_timed
			IF PreferenceEquals prefs = network field = game_end_type equals = zone_control_limit
				Change exist = 0
				ForEachIn choices_zone_control_limit do = checkifexist params = { theField = game_end_cond }
				IF IsTrue exist
				ELSE
					SetPreference prefs = network field = game_end_cond name = "1 Time" checksum = zone_control_limit_1 value = 1 array_name = choices_zone_control_limit params = { name = "1 Time" ui_string = "1 Time" checksum = zone_control_limit_1 value = 1 }
				ENDIF
				AddLine Type = textmenuelement id = end_game_cond text = "End Game Condition" link = network_end_game_cond_control_zone parent = server_pause_menu
				AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_cond text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_cond" } }
			ENDIF
			IF PreferenceEquals prefs = network field = game_end_type equals = zone_time_control_limit
				Change exist = 0
				ForEachIn choices_zone_score_limit do = checkifexist params = { theField = game_end_cond }
				IF IsTrue exist
				ELSE
					SetPreference prefs = network field = game_end_cond name = "500 pts" checksum = score_500 value = 500 array_name = choices_zone_score_limit params = { name = "500 pts" ui_string = "500 pts" checksum = score_500 value = 500 }
				ENDIF
				AddLine Type = textmenuelement id = end_game_cond text = "End Game Condition" link = network_end_game_cond_score_zone parent = server_pause_menu
				AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_cond text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_cond" } }
			ENDIF
		ENDIF
	ENDIF
	IF PreferenceEquals prefs = network field = game_type equals = ctf
		Change exist = 0
		ForEachIn choices_endgame_ctf do = checkifexist params = { theField = game_end_type }
		IF IsTrue exist
		ELSE
			SetPreference prefs = network field = game_end_type name = "Timed" checksum = choice_timed value = empty_choices array_name = choices_endgame_ctf params = { name = "Timed" ui_string = "Timed" checksum = choice_timed value = empty_choices }
		ENDIF
		AddLine Type = textmenuelement id = end_game_type text = "End Game Type" link = network_end_game_ctf parent = server_pause_menu
		AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_type text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_type" } }
		IF #"Not" PreferenceEquals prefs = network field = game_end_type equals = choice_timed
			Change exist = 0
			ForEachIn choices_capture_limit do = checkifexist params = { theField = game_end_cond }
			IF IsTrue exist
			ELSE
				SetPreference prefs = network field = game_end_cond name = "5 Captures" checksum = capture_limit_5 value = 5 array_name = choices_capture_limit params = { name = "5 Captures" ui_string = "5 Captures" checksum = capture_limit_5 value = 5 }
			ENDIF
			AddLine Type = textmenuelement id = end_game_cond text = "End Game Condition" link = network_end_game_cond_ctf parent = server_pause_menu
			AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_cond text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_cond" } }
		ENDIF
	ENDIF
	IF PreferenceEquals prefs = network field = game_type equals = nettrickattack
		Change exist = 0
		ForEachIn choices_endgame_other do = checkifexist params = { theField = game_end_type }
		IF IsTrue exist
		ELSE
			SetPreference prefs = network field = game_end_type name = "Timed" checksum = choice_timed value = empty_choices array_name = choices_endgame_other params = { name = "Timed" ui_string = "Timed" checksum = choice_timed value = empty_choices }
		ENDIF
		AddLine Type = textmenuelement id = end_game_type text = "End Game Type" link = network_end_game_other parent = server_pause_menu
		AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_type text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_type" } }
		IF #"Not" PreferenceEquals prefs = network field = game_end_type equals = choice_timed
			Change exist = 0
			ForEachIn choices_score_limit do = checkifexist params = { theField = game_end_cond }
			IF IsTrue exist
			ELSE
				SetPreference prefs = network field = game_end_cond name = "250,000 pts" checksum = score_250k value = 250000 array_name = choices_score_limit params = { name = "250,000 pts" ui_string = "250,000 pts" checksum = score_250k value = 250000 }
			ENDIF
			AddLine Type = textmenuelement id = end_game_cond text = "End Game Condition" link = network_end_game_cond_other parent = server_pause_menu
			AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_cond text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_cond" } }
		ENDIF
	ENDIF
	IF PreferenceEquals prefs = network field = game_type equals = beachball
		Change exist = 0
		ForEachIn choices_endgame_other do = checkifexist params = { theField = game_end_type }
		IF IsTrue exist
		ELSE
			SetPreference prefs = network field = game_end_type name = "Timed" checksum = choice_timed value = empty_choices array_name = choices_endgame_other params = { name = "Timed" ui_string = "Timed" checksum = choice_timed value = empty_choices }
		ENDIF
		AddLine Type = textmenuelement id = end_game_type text = "End Game Type" link = network_end_game_other parent = server_pause_menu
		AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_type text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_type" } }
		IF #"Not" PreferenceEquals prefs = network field = game_end_type equals = choice_timed
			Change exist = 0
			ForEachIn choices_score_limit do = checkifexist params = { theField = game_end_cond }
			IF IsTrue exist
			ELSE
				SetPreference prefs = network field = game_end_cond name = "250,000 pts" checksum = score_250k value = 250000 array_name = choices_score_limit params = { name = "250,000 pts" ui_string = "250,000 pts" checksum = score_250k value = 250000 }
			ENDIF
			AddLine Type = textmenuelement id = end_game_cond text = "End Game Condition" link = network_end_game_cond_other parent = server_pause_menu
			AddLine parent = server_pause_menu Type = textmenuelement id = end_game_opts_cond text = "Current End Game Options" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_ui_from_preferences" params = { prefs = network field = "game_end_cond" } }
		ENDIF
	ENDIF
	AddLine parent = server_pause_menu Type = textmenuelement id = back text = "Back" target = "go_back" params = { id = server_pause_menu }
ENDSCRIPT

SCRIPT populate_goals_menu
	DestroyElement id = level2
	DestroyElement id = level3
	DestroyElement id = level4
	DestroyElement id = level5
	DestroyElement id = level6
	DestroyElement id = level7
	DestroyElement id = level8
	DestroyElement id = level9
	GetGoalsCompleted LevelNum_Foundry
	AddLine parent = career_level_goals id = level1 text = <GoalsCompleted>
	GetGoalsCompleted LevelNum_Canada
	IF GetGlobalFlag flag = LEVEL_UNLOCKED_CANADA
		AddLine parent = career_level_goals id = level2 text = <GoalsCompleted>
	ELSE
		AddLine parent = career_level_goals id = level2 text = <GoalsCompleted> static
	ENDIF
	GetGoalsCompleted LevelNum_Rio
	IF GetGlobalFlag flag = LEVEL_UNLOCKED_RIO
		AddLine parent = career_level_goals id = level3 text = <BestMedal>
	ELSE
		printf "--------------------- Printing best medal for Rio"
		AddLine parent = career_level_goals id = level3 text = <BestMedal> static
	ENDIF
	GetGoalsCompleted LevelNum_Suburbia
	IF GetGlobalFlag flag = LEVEL_UNLOCKED_SUBURBIA
		AddLine parent = career_level_goals id = level4 text = <GoalsCompleted>
	ELSE
		AddLine parent = career_level_goals id = level4 text = <GoalsCompleted> static
	ENDIF
	GetGoalsCompleted LevelNum_Airport
	IF GetGlobalFlag flag = LEVEL_UNLOCKED_AIRPORT
		AddLine parent = career_level_goals id = level5 text = <GoalsCompleted>
	ELSE
		AddLine parent = career_level_goals id = level5 text = <GoalsCompleted> static
	ENDIF
	GetGoalsCompleted LevelNum_SkaterIsland
	IF GetGlobalFlag flag = LEVEL_UNLOCKED_SKATERISLAND
		AddLine parent = career_level_goals id = level6 text = <BestMedal>
	ELSE
		AddLine parent = career_level_goals id = level6 text = <BestMedal> static
	ENDIF
	GetGoalsCompleted LevelNum_LA
	IF GetGlobalFlag flag = LEVEL_UNLOCKED_LOSANGELES
		AddLine parent = career_level_goals id = level7 text = <GoalsCompleted>
	ELSE
		AddLine parent = career_level_goals id = level7 text = <GoalsCompleted> static
	ENDIF
	GetGoalsCompleted LevelNum_Tokyo
	IF GetGlobalFlag flag = LEVEL_UNLOCKED_TOKYO
		AddLine parent = career_level_goals id = level8 text = <BestMedal>
	ELSE
		AddLine parent = career_level_goals id = level8 text = <BestMedal> static
	ENDIF
	IF GetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
		GetGoalsCompleted LevelNum_Ship
		IF GetGlobalFlag flag = LEVEL_UNLOCKED_SHIP
			AddLine parent = career_level_goals id = level9 text = <GoalsCompleted>
		ELSE
			AddLine parent = career_level_goals id = level9 text = <GoalsCompleted> static
		ENDIF
	ELSE
		DestroyElement id = level9
	ENDIF
ENDSCRIPT

SCRIPT wait_th4
	Wait 10 gameframes
	SwitchToMenu menu = net_level_menu_th4
ENDSCRIPT

SCRIPT wait_and_populate_th4
	spawnscript wait_th4
ENDSCRIPT

SCRIPT wait_th3
	Wait 10 gameframes
	SwitchToMenu menu = net_level_menu
ENDSCRIPT

SCRIPT wait_and_populate_th3
	spawnscript wait_th3
ENDSCRIPT

SCRIPT populate_level_menu_th3
	helper_hide
	helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
	DestroyElement id = next_level_at
	AddLine parent = <parent_menu> id = THPS3_Levels text = "THPS3 Levels" static
	AddLine parent = <parent_menu> id = THPS4_Levels text = "THPS4 Levels" eventhandler = { kill_menu Type = ChooseEventHandler target = "wait_and_populate_th4" }
	VerifyParam param = parent_menu func = populate_level_menu_th3 <...>
	VerifyParam param = choose_script func = populate_level_menu_th3 <...>
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th3_level target_script = <choose_script> parent_menu = <parent_menu> }
ENDSCRIPT

SCRIPT populate_level_menu_th4
	helper_hide
	helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
	DestroyElement id = next_level_at
	AddLine parent = <parent_menu> id = THPS3_Levels text = "THPS3 Levels" eventhandler = { Type = ChooseEventHandler target = "wait_and_populate_th3" }
	AddLine parent = <parent_menu> id = THPS4_Levels text = "THPS4 Levels" static
	VerifyParam param = parent_menu func = populate_level_menu_th4 <...>
	VerifyParam param = choose_script func = populate_level_menu_th4 <...>
	ForEachIn master_level_list do = add_level_menu_item params = { mask = th4_level target_script = <choose_script> parent_menu = <parent_menu> }
ENDSCRIPT

SCRIPT populate_level_menu
	helper_hide
	helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
	IF IsCareerMode
		populate_goals_menu
		GetNextLevelRequirements
		IF GotParam friggina
			IF GetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
				DestroyElement id = next_level_at
				IF GetGlobalFlag flag = LEVEL_UNLOCKED_SHIP
					IF PAL
						AddLine parent = career_change_level_menu id = next_level_at text = "All levels unlocked" static drawer = main_smaller lock_layout y = 228
					ELSE
						AddLine parent = career_change_level_menu id = next_level_at text = "All levels unlocked" static drawer = main_smaller lock_layout y = 220
					ENDIF
				ELSE
					IF PAL
						AddLine parent = career_change_level_menu id = next_level_at text = <NextLevelRequirements> static drawer = main_smaller lock_layout y = 228
					ELSE
						AddLine parent = career_change_level_menu id = next_level_at text = <NextLevelRequirements> static drawer = main_smaller lock_layout y = 220
					ENDIF
				ENDIF
			ELSE
				IF PAL
					AddLine parent = career_change_level_menu id = next_level_at text = <NextLevelRequirements> static drawer = main_smaller lock_layout y = 209
				ELSE
					AddLine parent = career_change_level_menu id = next_level_at text = <NextLevelRequirements> static drawer = main_smaller lock_layout y = 201
				ENDIF
			ENDIF
		ELSE
			IF GetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
				DestroyElement id = next_level_at
				IF GetGlobalFlag flag = LEVEL_UNLOCKED_SHIP
					AddLine parent = career_change_level_menu id = next_level_at text = "All levels unlocked" static drawer = main_smaller lock_layout y = 238
				ELSE
					AddLine parent = career_change_level_menu id = next_level_at text = <NextLevelRequirements> static drawer = main_smaller lock_layout y = 238
				ENDIF
			ELSE
				AddLine parent = career_change_level_menu id = next_level_at text = <NextLevelRequirements> static drawer = main_smaller lock_layout y = 214
			ENDIF
		ENDIF
	ELSE
		DestroyElement id = next_level_at
	ENDIF
	VerifyParam param = parent_menu func = populate_level_menu <...>
	VerifyParam param = choose_script func = populate_level_menu <...>
	ForEachIn master_level_list do = add_level_menu_item params = { mask = regular_level target_script = <choose_script> parent_menu = <parent_menu> }
ENDSCRIPT

SCRIPT toggle_player_names
	TogglePlayerNames
ENDSCRIPT

SCRIPT toggle_scores
	ToggleScores
ENDSCRIPT

SCRIPT enter_observer_mode
	EnterObserverMode
ENDSCRIPT

SCRIPT enter_chat_mode
ENDSCRIPT

SCRIPT back_to_main
	printf "running back_to_main"
	LaunchMenuScreen screen = main_menu
ENDSCRIPT

SCRIPT change_airtricks1
	Change Air_CircleL = Trick_Stiffy
ENDSCRIPT

SCRIPT change_airtricks2
	Change Air_CircleR = Trick_Judo
ENDSCRIPT

SCRIPT change_airtricks3
	Change Airtricks = Airtricks3
	Change JumpTricks = Jumptricks3
ENDSCRIPT

SCRIPT enter_kb_chat
	SwitchToMenu menu = kb_chat_message_control DoNotPause
ENDSCRIPT

SCRIPT attach_new_stats_menu stats_left_width = 150 stats_right_width = 160
	VerifyParam param = menu_id func = attach_new_stats_menu <...>
	VerifyParam param = container_id func = attach_new_stats_menu <...>
	VerifyParam param = left_container_id func = attach_new_stats_menu <...>
	VerifyParam param = right_container_id func = attach_new_stats_menu <...>
	createandattachmenu { Type = verticalmenu id = <menu_id> parent = <container_id> game_menu_properties blue_top eventhandlers = [ { Type = showeventhandler target = "update_stats_elements" } ] }
	IF GotParam is_pro_menu
	ELSE
		createandattachmenu { Type = textmenuelement auto_id parent = <menu_id> text = "edit stats" static dont_gray drawer = title }
	ENDIF
	createandattachmenu { Type = menucontainer auto_id show_all_children parent = <menu_id> eventhandler = { Type = leftrightcontroller Left = <left_container_id> Right = <right_container_id> } children = [ { Type = verticalmenu cas_right_menu_properties y = 0.0 x = <stats_left_width> w = <stats_right_width> id = <right_container_id> }
			{ Type = verticalmenu cas_left_menu_properties y = 0.0 w = <stats_left_width> id = <left_container_id> } ] }
	IF GotParam is_pro_menu
	ELSE
		VerifyParam param = points_available_id func = attach_new_stats_menu <...>
		createandattachmenu { Type = textmenuelement id = <points_available_id> parent = <menu_id> text = "current points" static dont_gray drawer = points_available_property lock_layout y = 222.0 }
	ENDIF
	IF GotParam is_pro_menu
		ForEachIn cas_stats_display_children do = attach_new_right_stats_menu_item params = { container_id = <right_container_id> is_pro_menu stats_right_width = <stats_right_width> }
	ELSE
		ForEachIn cas_stats_display_children do = attach_new_right_stats_menu_item params = { container_id = <right_container_id> stats_right_width = <stats_right_width> }
	ENDIF
	IF GotParam is_pro_menu
		ForEachIn cas_stats_children do = attach_new_left_stats_menu_item params = { container_id = <left_container_id> is_pro_menu }
	ELSE
		ForEachIn cas_stats_children do = attach_new_left_stats_menu_item params = { container_id = <left_container_id> }
	ENDIF
ENDSCRIPT

SCRIPT update_stats_elements
	printf "Update Stats Elements"
	UpdateStatsElements
ENDSCRIPT

SCRIPT update_stats_ui
	UpdateStatsUI <...>
ENDSCRIPT
stats_left_menu_properties = { x = 0.0 y = 0.0 w = 150.0 h = 400.0 just_center_x just_center_y not_rounded }
stats_right_menu_properties = { x = 150.0 y = 0.0 w = 200.0 h = 400.0 just_center_x just_center_y not_rounded }

SCRIPT attach_new_left_stats_menu_item
	IF GotParam is_pro_menu
		createandattachmenu { <...> parent = <container_id> y = 0.0 w = 140.0 }
	ELSE
		createandattachmenu { <...> parent = <container_id> y = 0.0 }
	ENDIF
ENDSCRIPT

SCRIPT attach_new_right_stats_menu_item
	IF GotParam is_pro_menu
		VerifyParam param = pro_id func = attach_new_right_stats_menu_item <...>
		VerifyParam param = stats_right_width func = attach_new_right_stats_menu_item <...>
		createandattachmenu { <...> id = <pro_id> parent = <container_id> y = 0.0 w = <stats_right_width> static dont_gray r = 100 g = 65 b = 0 a = 115 }
	ELSE
		createandattachmenu { <...> parent = <container_id> y = 0.0 w = 170.0 }
		AttachEventHandler { Type = ChooseEventHandler object = <id> target = "go_back" params = { id = <container_id> } }
	ENDIF
ENDSCRIPT

SCRIPT send_chat_message
	printf "Sending chat message..."
	SendChatMessage <...>
ENDSCRIPT

SCRIPT clear_keyboard_string
	VerifyParam param = control_id func = clear_keyboard_string <...>
	IF GotParam string
		LaunchEvent Type = setcontents target = <control_id> string = <string>
	ELSE
		LaunchEvent Type = setcontents target = <control_id> string = ""
	ENDIF
ENDSCRIPT

SCRIPT PreRunQueuedScripts
	SetSlomo 100.0
ENDSCRIPT

SCRIPT PostRunQueuedScripts
	SetSlomo 1
ENDSCRIPT

SCRIPT choose_team_on_show
	IF InTeamGame
		MakeTextMenuElementStatic <ElementId> nonstatic = 1
	ELSE
		MakeTextMenuElementStatic <ElementId>
	ENDIF
ENDSCRIPT

SCRIPT do_choose_team
	ChooseTeam <...>
ENDSCRIPT
