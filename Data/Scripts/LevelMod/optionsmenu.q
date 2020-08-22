SCRIPT MoveNetSetup
	MoveMenu id = net_network_setup_menu x = 150
ENDSCRIPT

SCRIPT Options_ShowEvent
	helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
ENDSCRIPT

SCRIPT Palmer_ShowEvent
	SelectElement id = return_THPS3
ENDSCRIPT

SCRIPT RunCreditMovies
	playmovie_script "movies\credits"
ENDSCRIPT

SCRIPT set_primary_controller_name
	printf "show detected"
	SetPrimaryControllerName <...>
ENDSCRIPT

SCRIPT controller_list_options
	helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
ENDSCRIPT

SCRIPT SetCustRestart
	StoreSkaterPos
ENDSCRIPT

SCRIPT GotoCustRestart
	SetSpeed 0
	SetState Ground
	DisablePlayerInput AllowCameraControl
	IF OnRail
		StopBalanceTrick
		MakeSkaterGoto GroundGone
	ENDIF
	CleanUpSpecialItems
	StopBalanceTrick
	CheckGapTricks
	ClearGapTricks
	ClearPanel_Landed
	ClearExceptions
	ClearTrickQueues
	ClearEventBuffer
	TurnToFaceVelocity
	RestartSkaterExceptions
	SetRollingFriction 10000
	PausePhysics
	PauseClock
	PauseSkaters
	GotoStoredPos
	SetSpeed 0
	ClearPanel_Bailed
	UnpauseClock
	UnpauseSkaters
	WaitOneGameFrame
	UnPausePhysics
	GotoStoredPos
	SetSpeed 0
	TurnToFaceVelocity
	SetRollingFriction 100
	WaitOneGameFrame
	SetRollingFriction 18
	EnablePlayerInput
ENDSCRIPT

SCRIPT LoadLevelModSettings
	IF UsingNewMenu
		SetMenuElementText id = new_menu "New Menu: On"
	ELSE
		SetMenuElementText id = new_menu "New Menu: Off"
	ENDIF
	IF UsingSoundFix
		SetMenuElementText id = sound_fix "Sound Fix: On"
	ELSE
		SetMenuElementText id = sound_fix "Sound Fix: Off"
	ENDIF
	IF UsingTeleFix
		SetMenuElementText id = tele_fix "Tele Switch Fix: On"
	ELSE
		SetMenuElementText id = tele_fix "Tele Switch Fix: Off"
	ENDIF
	IF SetTagLimit
		SetMenuElementText id = tag_fix "Unlimited Tags: Off"
	ELSE
		SetMenuElementText id = tag_fix "Unlimited Tags: On"
	ENDIF
	IF IsTrue GrassLayersDisabled
		SetMenuElementText id = grass_3d "3D Grass: Off"
	ELSE
		SetMenuElementText id = grass_3d "3D Grass: On"
	ENDIF
	IF IsTrue bPrintDebug
		SetMenuElementText id = print_debug "Print Debug: On"
	ELSE
		SetMenuElementText id = print_debug "Print Debug: Off"
	ENDIF
	IF IsTrue bShowConsole
		SetMenuElementText id = show_console "Console: Show"
	ELSE
		SetMenuElementText id = show_console "Console: Hide"
	ENDIF
	IF strstr s1 = spine_button_text s2 = "Revert"
		AddLine Type = textmenuelement id = spine_button_id text = "Revert" static dont_gray drawer = keyboard_property parent = menu_levelmod_settings
	ENDIF
	IF strstr s1 = spine_button_text s2 = "Nollie"
		AddLine Type = textmenuelement id = spine_button_id text = "Nollie" static dont_gray drawer = keyboard_property parent = menu_levelmod_settings
	ENDIF
	IF strstr s1 = spine_button_text s2 = "Left"
		AddLine Type = textmenuelement id = spine_button_id text = "Left Spine Button" static dont_gray drawer = keyboard_property parent = menu_levelmod_settings
	ENDIF
	IF strstr s1 = spine_button_text s2 = "Right"
		AddLine Type = textmenuelement id = spine_button_id text = "Right Spin Button" static dont_gray drawer = keyboard_property parent = menu_levelmod_settings
	ENDIF
	AddLine Type = textmenuelement id = options_done_lm text = "Done" parent = menu_levelmod_settings target = "go_back" Params = { id = menu_levelmod_settings }
ENDSCRIPT

SCRIPT UpdateConsoleText
	IF IsTrue bShowConsole
		SetMenuElementText id = show_console "Console: Show"
	ELSE
		SetMenuElementText id = show_console "Console: Hide"
	ENDIF
ENDSCRIPT

SCRIPT UpdateDebugText
	IF IsTrue bPrintDebug
		SetMenuElementText id = print_debug "Print Debug: On"
	ELSE
		SetMenuElementText id = print_debug "Print Debug: Off"
	ENDIF
ENDSCRIPT

SCRIPT sToggleTagLimit
	IF SetTagLimit
		SetTagLimit 200
	ELSE
		SetTagLimit 32
	ENDIF
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT sToggleShowConsole
	IF IsTrue bShowConsole
		Change bShowConsole = 0
		HideConsoleScript
	ELSE
		Change bShowConsole = 1
		ShowConsoleScript
	ENDIF
	UpdateConsoleText
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT sTogglePrintDebug
	IF IsTrue bPrintDebug
		Change bPrintDebug = 0
		UnhookDebugMessages
	ELSE
		Change bPrintDebug = 1
		HookDebugMessages
	ENDIF
	UpdateDebugText
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT sToggleNewMenu
	ToggleNewMenu
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT sToggleTeleFix
	ToggleTeleFix
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT sToggleSoundFix
	ToggleSoundFix
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT sToggle3DGrass
	IF IsTrue GrassLayersDisabled
		Change GrassLayersDisabled = 0
	ELSE
		Change GrassLayersDisabled = 1
	ENDIF
	LoadLevelModSettings
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT PreviewCounter posX = 100.0 posY = 70.0
	GetSliderValue id = CounterPosX name = posX
	GetSliderValue id = CounterPosY name = posY
	CreatePair name = "pair" x = <posX> y = <posY>
	ChangeValues Container = tags_message Values = { pos = <pair> }
	LaunchPanelMessage "Tags: 0" properties = tags_message
ENDSCRIPT
spine_button_text = "don't touch this..."

SCRIPT SetSpineButton
	IF GotParam revert
		ChangeString #"string" = spine_button_text value = "Revert"
	ENDIF
	IF GotParam nollie
		ChangeString #"string" = spine_button_text value = "Nollie"
	ENDIF
	IF GotParam LeftSpin
		ChangeString #"string" = spine_button_text value = "Left Spin Button"
	ENDIF
	IF GotParam RightSpin
		ChangeString #"string" = spine_button_text value = "Right Spin Button"
	ENDIF
	UpdateLevelModSettings
ENDSCRIPT

SCRIPT Spine_Button_OnShow
ENDSCRIPT








SCRIPT options_menu_create
	CreateMenu { Type = verticalmenu id = options_main_menu x = 170.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "OptionsToMainMenuCamAnim" }
			{ Type = showeventhandler target = "Options_ShowEvent" } ] children = [ { Type = textmenuelement auto_id text = "Options" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "LevelMod Settings" link = menu_levelmod_settings }
			{ Type = textmenuelement id = soundfx_menu text = "Sound Options" link = sfx_menu_container }
			{ Type = textmenuelement auto_id text = "Movies" link = movies_menu }
			{ Type = textmenuelement auto_id text = "Save Game" target = "OptionsMenuSave" }
			{ Type = textmenuelement auto_id text = "Load Game" target = "OptionsMenuLoad" }
			{ Type = textmenuelement auto_id text = "Load Replay" target = "_OptionsMenuLoadReplay" }
			{ Type = textmenuelement auto_id text = "Level Records" target = "Options_RecordsDisplay" }
			{ Type = textmenuelement auto_id text = "Controller Config" link = controller_config_menu }
			{ Type = textmenuelement auto_id text = "Cheats" link = cheat_keyboard_control }
			{ Type = textmenuelement auto_id text = "Credits" target = "RunCreditMovies" }
			{ Type = textmenuelement id = options_done text = "Done" target = "go_back" Params = { id = options_main_menu } } ] }

	//adds levelmod menu
	CreateLevelModMenus
	CreateMenu { Type = verticalmenu id = palmer_warning x = 130.0 y = 120.0 w = 380.0 h = 400.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "Palmer_ShowEvent" } children = [ { Type = textmenuelement auto_id text = "Warning!" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "You will lose any" static dont_gray }
			{ Type = textmenuelement auto_id text = "unsaved THPS3 changes" static dont_gray }
			{ Type = textmenuelement auto_id text = "if you exit to the" static dont_gray }
			{ Type = textmenuelement auto_id text = "Shaun Palmer demo." static dont_gray }
			{ Type = textmenuelement auto_id text = " " static dont_gray }
			{ Type = textmenuelement auto_id text = "Play Demo" target = "palmer_demo" }
			{ Type = textmenuelement id = return_THPS3 text = "Return to THPS3" link = options_main_menu } ] }
	attachchild parent = contain1 child = palmer_warning
	create_cheat_keyboard
	IF IsTrue XBox
	ELSE
		DestroyElement id = xbox_gamma
	ENDIF
	attachchild parent = contain1 child = options_main_menu
	CreateMenu { Type = verticalmenu id = gamma_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "Gamma" static dont_gray drawer = title }
			{ Type = slidermenuelement id = gamma_r text = "red" lower = 0 upper = 100 delta = 1 start = 20 wrap = 0 right_side_w = 80 arrow_spacing = 30.0 eventhandlers = [ { Type = ContentsChangedEventHandler target = "change_gamma" } ] }
			{ Type = slidermenuelement id = gamma_g text = "green" lower = 0 upper = 100 delta = 1 start = 20 right_side_w = 80 arrow_spacing = 30.0 eventhandlers = [ { Type = ContentsChangedEventHandler target = "change_gamma" } ] }
			{ Type = slidermenuelement id = gamma_b text = "blue" lower = 0 upper = 100 delta = 1 start = 20 right_side_w = 80 arrow_spacing = 30.0 eventhandlers = [ { Type = ContentsChangedEventHandler target = "change_gamma" } ] } ] }
	attachchild parent = contain1 child = gamma_menu
	memcard_files_menu_create
	CreateMenu { Type = verticalmenu id = controller_config_menu x = 150.0 y = 120.0 w = 340.0 h = 400.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "controller_config_menu_on_show" } children = [ { Type = textmenuelement auto_id text = "Control Config" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Primary Controller" link = controller_select_primary }
			{ Type = textmenuelement id = controller_display_id text = "Default" static dont_gray drawer = keyboard_property eventhandler = { Type = showeventhandler target = "set_primary_controller_name" } }
			{ Type = textmenuelement id = controller_primary_profile_select text = "Gamepad Profile" link = controller_select_profile }
			{ Type = textmenuelement id = controller_profile_id text = "Default" static dont_gray drawer = keyboard_property }
			{ Type = textmenuelement id = controller_kb_profile_select text = "Keyboard Profile" link = controller_select_kb_profile }
			{ Type = textmenuelement id = controller_kb_profile_id text = "Default" static dont_gray drawer = keyboard_property }
			{ Type = textmenuelement id = controller_config_1_option text = "Settings" link = controller_config_1 }
			{ Type = textmenuelement id = controller_config_done text = "Done" target = "go_back" Params = { id = controller_config_menu } } ] }
	attachchild parent = contain1 child = controller_config_menu
	CreateMenu { Type = verticalmenu id = controller_config_1 x = 120.0 y = 120.0 w = 400.0 h = 400.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "controller_config_1_on_show" } children = [ { Type = textmenuelement auto_id text = "Settings" static dont_gray drawer = title }
			{ Type = textmenuelement id = controller_config_1_autokick text = "Foo" target = "ToggleAutoKick" Params = { Controller1 } }
			{ Type = textmenuelement id = controller_config_1_spintaps text = "Foo" target = "ToggleSpinTaps" Params = { Controller1 } }
			{ Type = textmenuelement id = controller_config_1_done text = "Done" target = "go_back" Params = { id = controller_config_1 } } ] }
	attachchild parent = contain1 child = controller_config_1
	CreateMenu { Type = verticalmenu id = controller_config_2 x = 150.0 y = 120.0 w = 340.0 h = 400.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "controller_config_2_on_show" } children = [ { Type = textmenuelement auto_id text = "Controller 2" static dont_gray drawer = title }
			{ Type = textmenuelement id = controller_config_2_vibration text = "Foo" target = "ToggleVibration" Params = { Controller2 } }
			{ Type = textmenuelement id = controller_config_2_autokick text = "Foo" target = "ToggleAutoKick" Params = { Controller2 } }
			{ Type = textmenuelement id = controller_config_2_spintaps text = "Foo" target = "ToggleSpinTaps" Params = { Controller2 } }
			{ Type = textmenuelement id = controller_config_2_done text = "Done" target = "go_back" Params = { id = controller_config_2 } } ] }
	attachchild parent = contain1 child = controller_config_2
	CreateMenu { Type = verticalmenu id = controller_select_primary x = 120.0 y = 120.0 w = 400.0 h = 400.0 just_center_x just_center_y blue_top }
	attachchild parent = contain1 child = controller_select_primary
	CreateMenu { Type = verticalmenu id = controller_select_profile x = 120.0 y = 120.0 w = 400.0 h = 400.0 just_center_x just_center_y blue_top }
	attachchild parent = contain1 child = controller_select_profile
	CreateMenu { Type = verticalmenu id = controller_select_kb_profile x = 120.0 y = 120.0 w = 400.0 h = 400.0 just_center_x just_center_y blue_top }
	attachchild parent = contain1 child = controller_select_kb_profile
	InitializeControllerMenu
ENDSCRIPT
opts_str_profile = 'Profile'
opts_str_controllers = 'Controllers'

SCRIPT controller_config_menu_on_show
	SetFrontEndPad 0
	UseBothPadsInFrontEnd
ENDSCRIPT

SCRIPT controller_config_1_on_show
	SetFrontEndPad 0
	IF VibrationIsOn 0
		SetMenuElementText id = controller_config_1_vibration "Vibration: On"
	ELSE
		SetMenuElementText id = controller_config_1_vibration "Vibration: Off"
	ENDIF
	IF AutoKickIsOn 0
		SetMenuElementText id = controller_config_1_autokick "AutoKick: On"
	ELSE
		SetMenuElementText id = controller_config_1_autokick "AutoKick: Off"
	ENDIF
	IF SpinTapsAreOn 0
		SetMenuElementText id = controller_config_1_spintaps "180 Spin Taps: On"
	ELSE
		SetMenuElementText id = controller_config_1_spintaps "180 Spin Taps: Off"
	ENDIF
ENDSCRIPT

SCRIPT controller_config_2_on_show
	SetFrontEndPad 1
	IF VibrationIsOn 1
		SetMenuElementText id = controller_config_2_vibration "Vibration: On"
	ELSE
		SetMenuElementText id = controller_config_2_vibration "Vibration: Off"
	ENDIF
	IF AutoKickIsOn 1
		SetMenuElementText id = controller_config_2_autokick "AutoKick: On"
	ELSE
		SetMenuElementText id = controller_config_2_autokick "AutoKick: Off"
	ENDIF
	IF SpinTapsAreOn 1
		SetMenuElementText id = controller_config_2_spintaps "180 Spin Taps: On"
	ELSE
		SetMenuElementText id = controller_config_2_spintaps "180 Spin Taps: Off"
	ENDIF
ENDSCRIPT

SCRIPT ToggleVibration
	IF GotParam Controller1
		IF VibrationIsOn 0
			VibrationOff 0
		ELSE
			VibrationOn 0
		ENDIF
		controller_config_1_on_show
	ENDIF
	IF GotParam Controller2
		IF VibrationIsOn 1
			VibrationOff 1
		ELSE
			VibrationOn 1
		ENDIF
		controller_config_2_on_show
	ENDIF
ENDSCRIPT

SCRIPT ToggleAutoKick
	IF GotParam Controller1
		IF AutoKickIsOn 0
			AutoKickOff 0
		ELSE
			AutoKickOn 0
		ENDIF
		controller_config_1_on_show
	ENDIF
	IF GotParam Controller2
		IF AutoKickIsOn 1
			AutoKickOff 1
		ELSE
			AutoKickOn 1
		ENDIF
		controller_config_2_on_show
	ENDIF
ENDSCRIPT

SCRIPT ToggleSpintaps
	IF GotParam Controller1
		IF SpinTapsAreOn 0
			SpinTapsOff 0
		ELSE
			SpinTapsOn 0
		ENDIF
		controller_config_1_on_show
	ENDIF
	IF GotParam Controller2
		IF SpinTapsAreOn 1
			SpinTapsOff 1
		ELSE
			SpinTapsOn 1
		ENDIF
		controller_config_2_on_show
	ENDIF
ENDSCRIPT

SCRIPT change_gamma
	ApplyChangeGamma
ENDSCRIPT
ken_line_col_1 = 180
ken_line_col_2 = 435
ken_line_col_3 = 565
ReturnedFromLevelRecords = 0

SCRIPT WaitForButtonRelease
	BEGIN
		IF ControllerPressed <Button>
		ELSE
			BREAK
		ENDIF
		Wait 1 gameframe
	REPEAT
ENDSCRIPT

SCRIPT Options_RecordsDisplay
	DisableBackEvent
	DisableEvent id = options_main_menu EventType = back
	CreatePanels
	KillMessages
	SpawnSkaterScript Skater_RecordsDisplay
	SwitchOffMenu id = options_main_menu
	Spawnscript DisplayHelper
ENDSCRIPT

SCRIPT DisplayHelper
	Wait 2 gameframe
	IF MenuIsShown options_main_menu
	ELSE
		helper_level_records
	ENDIF
ENDSCRIPT

SCRIPT Skater_RecordsDisplay level = LevelNum_Foundry Heading = "Foundry"
	GetRecordText level = <level>
	records_display x_col_1 = ken_line_col_1 x_col_2 = ken_line_col_2 x_col_3 = ken_line_col_3 Heading = <Heading>
	WaitForButtonRelease Button = Right
	WaitForButtonRelease Button = Left
	WaitForButtonRelease Button = Up
	WaitForButtonRelease Button = Down
	BEGIN
		IF ControllerPressed Right
			ShowNextRecordRight <...>
		ENDIF
		IF ControllerPressed Up
			ShowNextRecordRight <...>
		ENDIF
		IF ControllerPressed Left
			ShowNextRecordLeft <...>
		ENDIF
		IF ControllerPressed Down
			ShowNextRecordLeft <...>
		ENDIF
		IF ControllerPressed Triangle
			KillMessages
			DeletePanel
			helper_hide
			Change ReturnedFromLevelRecords = 1
			SwitchToMenu DoNotMakeRoot menu = options_main_menu
			EnableBackEvent
			EnableEvent id = options_main_menu EventType = back
			BREAK
		ENDIF
		Wait 1 gameframe
	REPEAT
ENDSCRIPT

SCRIPT MaybeShowShip
	Goto Skater_RecordsDisplay Params = { level = LevelNum_Ship Heading = "Ship" }
ENDSCRIPT

SCRIPT MaybeShowWarehouse
	Goto Skater_RecordsDisplay Params = { level = LevelNum_Warehouse Heading = "Warehouse" }
ENDSCRIPT

SCRIPT MaybeShowBurnside
	Goto Skater_RecordsDisplay Params = { level = LevelNum_Burnside Heading = "Burnside" }
ENDSCRIPT

SCRIPT MaybeShowRoswell
	Goto Skater_RecordsDisplay Params = { level = LevelNum_Roswell Heading = "Roswell" }
ENDSCRIPT

SCRIPT ShowNextRecordRight
	IF IntegerEquals a = <level> b = LevelNum_Foundry
		Goto Skater_RecordsDisplay Params = { level = LevelNum_Canada Heading = "Canada" }
	ELSE
		IF IntegerEquals a = <level> b = LevelNum_Canada
			Goto Skater_RecordsDisplay Params = { level = LevelNum_Rio Heading = "Rio" }
		ELSE
			IF IntegerEquals a = <level> b = LevelNum_Rio
				Goto Skater_RecordsDisplay Params = { level = LevelNum_Suburbia Heading = "Suburbia" }
			ELSE
				IF IntegerEquals a = <level> b = LevelNum_Suburbia
					Goto Skater_RecordsDisplay Params = { level = LevelNum_Airport Heading = "Airport" }
				ELSE
					IF IntegerEquals a = <level> b = LevelNum_Airport
						Goto Skater_RecordsDisplay Params = { level = LevelNum_SkaterIsland Heading = "Skater Island" }
					ELSE
						IF IntegerEquals a = <level> b = LevelNum_SkaterIsland
							Goto Skater_RecordsDisplay Params = { level = LevelNum_LA Heading = "Los Angeles" }
						ELSE
							IF IntegerEquals a = <level> b = LevelNum_LA
								Goto Skater_RecordsDisplay Params = { level = LevelNum_Tokyo Heading = "Tokyo" }
							ELSE
								IF IntegerEquals a = <level> b = LevelNum_Tokyo
									Goto Skater_RecordsDisplay Params = { level = LevelNum_Ship Heading = "Ship" }
								ELSE
									IF IntegerEquals a = <level> b = LevelNum_Ship
										Goto Skater_RecordsDisplay Params = { level = LevelNum_Warehouse Heading = "Warehouse" }
									ELSE
										IF IntegerEquals a = <level> b = LevelNum_Warehouse
											Goto Skater_RecordsDisplay Params = { level = LevelNum_Burnside Heading = "Burnside" }
										ELSE
											IF IntegerEquals a = <level> b = LevelNum_Burnside
												Goto Skater_RecordsDisplay Params = { level = LevelNum_Roswell Heading = "Roswell" }
											ELSE
												IF IntegerEquals a = <level> b = LevelNum_Roswell
													Goto Skater_RecordsDisplay Params = { level = LevelNum_Oil Heading = "Oil Rig" }
												ENDIF
											ENDIF
										ENDIF
									ENDIF
								ENDIF
							ENDIF
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT ShowNextRecordLeft
	IF IntegerEquals a = <level> b = LevelNum_Canada
		Goto Skater_RecordsDisplay Params = { level = LevelNum_Foundry Heading = "Foundry" }
	ELSE
		IF IntegerEquals a = <level> b = LevelNum_Rio
			Goto Skater_RecordsDisplay Params = { level = LevelNum_Canada Heading = "Canada" }
		ELSE
			IF IntegerEquals a = <level> b = LevelNum_Suburbia
				Goto Skater_RecordsDisplay Params = { level = LevelNum_Rio Heading = "Rio" }
			ELSE
				IF IntegerEquals a = <level> b = LevelNum_Airport
					Goto Skater_RecordsDisplay Params = { level = LevelNum_Suburbia Heading = "Suburbia" }
				ELSE
					IF IntegerEquals a = <level> b = LevelNum_SkaterIsland
						Goto Skater_RecordsDisplay Params = { level = LevelNum_Airport Heading = "Airport" }
					ELSE
						IF IntegerEquals a = <level> b = LevelNum_LA
							Goto Skater_RecordsDisplay Params = { level = LevelNum_SkaterIsland Heading = "Skater Island" }
						ELSE
							IF IntegerEquals a = <level> b = LevelNum_Tokyo
								Goto Skater_RecordsDisplay Params = { level = LevelNum_LA Heading = "Los Angeles" }
							ELSE
								IF IntegerEquals a = <level> b = LevelNum_Ship
									Goto Skater_RecordsDisplay Params = { level = LevelNum_Tokyo Heading = "Tokyo" }
								ELSE
									IF IntegerEquals a = <level> b = LevelNum_Warehouse
										Goto Skater_RecordsDisplay Params = { level = LevelNum_Ship Heading = "Cruise Ship" }
									ELSE
										IF IntegerEquals a = <level> b = LevelNum_Burnside
											Goto Skater_RecordsDisplay Params = { level = LevelNum_Warehouse Heading = "Warehouse" }
										ELSE
											IF IntegerEquals a = <level> b = LevelNum_Roswell
												Goto Skater_RecordsDisplay Params = { level = LevelNum_Burnside Heading = "Burnside" }
											ELSE
												IF IntegerEquals a = <level> b = LevelNum_Oil
													Goto Skater_RecordsDisplay Params = { level = LevelNum_Roswell Heading = "Roswell" }
												ENDIF
											ENDIF
										ENDIF
									ENDIF
								ENDIF
							ENDIF
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT create_cheat_keyboard
	DestroyElement id = cheat_keyboard_control
	createandattachmenu { Type = keyboardcontrol id = cheat_keyboard_control x = 160.0 y = 56.0 w = 320.0 h = 336.0 just_center_x just_center_y num_columns = 13 alphanumeric_charset min_chars = 0 max_chars = 28 default_to_accept title_string = "Enter Cheat Code" default_string = "" hide_password eventhandlers = [ { Type = showeventhandler target = "start_cheat" Params = { prefs = records field = "cheat" } }
			{ Type = ContentsChangedEventHandler target = "end_cheat" Params = { prefs = records field = "cheat" } } ] parent = contain1 }
ENDSCRIPT

SCRIPT start_cheat
	LaunchEvent Type = setcontents target = cheat_keyboard_control #"string" = ""
ENDSCRIPT

SCRIPT end_cheat
	printf <#"string">
	TryCheatString #"string" = <#"string">
ENDSCRIPT

SCRIPT palmer_demo
	StopMusic
	stopstream
	LoadExecPS2 elf = "cdrom0:\DEMO\SLUS_201.99"
ENDSCRIPT
