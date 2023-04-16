SCRIPT start_main_menu
  SetFrontEndPad 0
  request_level level = Load_SkateShop
  SetGameType SkateShop
  SetGameState on
  reset_skateshop_skater
  HideLoadingScreen
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01" play_hold
  SpawnScript main_menu_first_frame
ENDSCRIPT

DoAutoLoad = 1
ver = ""
motd = ""
HudIsOn = 1
AutoLoadRetryContinueVisible = 0

//this is probably should be moved to framework qb?
SCRIPT ShatterAndDie
  Shatter <...>
  Kill <...>
ENDSCRIPT

SCRIPT AutoLoad
  LoadFont "thps4.fnt"
  IF PAL
    change MinimumSpaceNeeded = 65
  ENDIF
  IF MemCardFileExists name = "Options and Pros" Type = OptionsAndPros
    DisplayMessage MessageScript = Message_LoadingData PauseLength = MemCardMessageShortPause
    change MemCardLoadFailed = 0
    IF LoadFromMemoryCard name = "Options and Pros" Type = OptionsAndPros
    ELSE
      change MemCardLoadFailed = 1
    ENDIF
    IF MemCardFileExists name = "Network settings" Type = NetworkSettings
      IF LoadFromMemoryCard name = "Network settings" Type = NetworkSettings
      ELSE
        change MemCardLoadFailed = 1
      ENDIF
    ENDIF
    IF CurrentSkaterIsPro
    ELSE
      IF CustomSkaterFilenameDefined
        GetCustomSkaterFilename
        IF LoadFromMemoryCard name = <string> Type = cas
        ELSE
          change MemCardLoadFailed = 1
        ENDIF
      ENDIF
    ENDIF
    RemoveAllSkaters
    ToggleSkater
    FireMenuEvent Type = refreshdecktextures Id = deck_menu target = deck_menu
    PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01" play_hold
    IF IsTrue MemCardLoadFailed
      DisplayMessage MessageScript = Message_LoadFailed PauseLength = MemCardMessageErrorPause
    ELSE
      movies_menu_create
      DisplayMessage MessageScript = Message_LoadSuccessful
    ENDIF
  ENDIF
  SwitchToMenu menu = frontend_menu DoNotMakeRoot
  change DoAutoLoad = 0
  //populate level menu after save file is loaded, else th3 levels will be locked
  LM_PopulateLevelMenu
  goto main_menu_first_frame
ENDSCRIPT

SCRIPT _ReSpawnAutoload
  EnableBackEvent
  SpawnScript AutoLoad
ENDSCRIPT

SCRIPT _BackToMainMenuFirstFrame
  EnableBackEvent
  SpawnScript BackToMainMenuFirstFrame
ENDSCRIPT

SCRIPT BackToMainMenuFirstFrame
  SwitchToMenu menu = frontend_menu DoNotMakeRoot
  change DoAutoLoad = 0
  goto main_menu_first_frame
ENDSCRIPT

SCRIPT main_menu_first_frame
  IF IsTrue run_viewer
    change DoAutoLoad = 0
  ENDIF
  IF IsTrue DoAutoLoad
    goto AutoLoad
  ENDIF
  change FirstTimeInSplitScreen = 1
  change FirstTimeFromSkateshop = 1
  IF DoDeferredInternetConnect
  ELSE
    IF DeferredLaunchServer
    ELSE
      IF IsTrue AutoLinkToCareerScreen
        change AutoLinkToCareerScreen = 0
        SetCareerMode
        SwitchToMenu menu = career_menu DoNotMakeRoot
      ENDIF
    ENDIF
  ENDIF
ENDSCRIPT

SCRIPT reset_skateshop_skater
  IF CurrentSkaterProfileIs 0
    PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01" play_hold
  ENDIF
  MakeSkaterGoto skater = 0 SkateShopDummyAI
  MakeSkaterGoto skater = 1 SkateShopDummyAI
ENDSCRIPT

SCRIPT exit_main_menu
  UninitializeFrontEndSprites
ENDSCRIPT

SCRIPT SkateShopDummyAI
  SwitchOnBoard
  SwitchOffAtomic special_item
  ClearExceptions
  SetQueueTricks NoTricks
  DisablePlayerInput
  SetRollingFriction 10000
  BlendperiodOut 0
  BEGIN
    IF ProfileEquals is_named = wolverine
      PlayAnim Anim = WolverineIdle Cycle NoRestart BlendPeriod = 0.0
    ELSE
      IF ProfileEquals is_named = demoness
        PlayAnim Anim = DemonessIdle Cycle NoRestart BlendPeriod = 0.0
      ELSE
        IF ProfileEquals is_named = maul
          IF Flipped
          ELSE
            flip
          ENDIF
          SwitchOffAtomic special_item
          PlayAnim Anim = MaulIdle Cycle NoRestart BlendPeriod = 0.0
          Wait 60 frames
          SwitchOnAtomic special_item
          PlaySound SaberHIT1
        ELSE
          IF ProfileEquals is_named = slater
            PlayAnim Anim = SlaterIdle Cycle NoRestart BlendPeriod = 0.0
          ELSE
            IF ProfileEquals is_named = Ollie
              PlayAnim Anim = OllieIdle Cycle NoRestart BlendPeriod = 0.0
            ELSE
              IF ProfileEquals is_named = dick
                goto DickIdle
              ELSE
                IF ProfileEquals is_named = steamer
                  goto StandIdle
                ELSE
                  IF ProfileEquals is_named = muska
                    IF Flipped
                    ELSE
                      flip
                    ENDIF
                    GotoRandomScript [ MuskaIdle ]
                  ELSE
                    IF ProfileEquals is_named = margera
                      PlayAnim random( @Anim = BoardIdle @Anim = BoardIdle @Anim = BoardIdle @Anim = BoardIdle @Anim = BamScratch @Anim = BoardLook @Anim = BoardMove  ) BlendPeriod = 0.0
                    ELSE
                      IF ProfileEquals body_type = female
                        GotoRandomScript [ FemaleIdle1 FemaleIdle2 ]
                      ELSE
                        GotoRandomScript [ StandIdle OtherIdle BoardIdle ]
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
    WaitAnimFinished
  REPEAT
ENDSCRIPT

SCRIPT StandIdle
  BEGIN
    PlayAnim Anim = StandIdle BlendPeriod = 0.0
    WaitAnimFinished
    PlayAnim Anim = random( @StandIdle @StandIdle @StandIdle @StandIdle @StandLeftRight @StandCheckBoard @StandShoulder ) BlendPeriod = 0.0
    IF AnimEquals StandSwap
      FlipAfter
      BlendperiodOut 0.0
    ENDIF
    WaitAnimFinished
  REPEAT
ENDSCRIPT

SCRIPT MuskaIdle
  SwitchOnAtomic special_item
  PlayAnim Anim = MuskaIdle Cycle BlendPeriod = 0.0 speed = 0.7
ENDSCRIPT

SCRIPT FemaleIdle1
  PlayAnim Anim = FemaleIdle Cycle BlendPeriod = 0.0
ENDSCRIPT

SCRIPT FemaleIdle2
  PlayAnim Anim = LeanIdle Cycle BlendPeriod = 0.0
ENDSCRIPT

SCRIPT BoardIdle
  BEGIN
    PlayAnim Anim = random( @BoardIdle @BoardIdle @BoardIdle @BoardIdle @BoardIdle @BoardMove @BoardLook  ) BlendPeriod = 0.0
    IF CurrentSkaterProfileIs 0
      IF AnimEquals BoardMove
      ENDIF
    ENDIF
    WaitAnimFinished
  REPEAT
ENDSCRIPT

SCRIPT BoardMove_SFX
  BEGIN
    IF FrameIS 30
      PlaySound skater_boarddown vol = 30
    ENDIF
    IF FrameIS 110
      PlaySound skater_boardup vol = 40
      BREAK
    ENDIF
    Wait 1 Frame
  REPEAT
ENDSCRIPT

SCRIPT OtherIdle
  PlayAnim Anim = StartIdle Cycle BlendPeriod = 0.0
ENDSCRIPT

SCRIPT DickIdle
  BEGIN
    IF AnimEquals DickBeckon
      PlayAnim Anim = DickIdle BlendPeriod = 0.0
    ELSE
      PlayAnim Anim = random( @DickIdle @DickIdle @DickBeckon  ) BlendPeriod = 0.0
    ENDIF
    WaitAnimFinished
  REPEAT
ENDSCRIPT

SCRIPT reset_parkeditor_skater
  printf "reset park editor skater"
  MakeSkaterGoto skater = 0 FreezeSkater
  MakeSkaterGoto skater = 1 FreezeSkater
ENDSCRIPT

SCRIPT MainMenuCam
  PlaySkaterCamAnim skater = 0 name = "Cam_Default01" play_hold
ENDSCRIPT

SCRIPT MainMenuToOptionsCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_Options01_Anim" play_hold
ENDSCRIPT

SCRIPT FrontEndFlash
  DoFlash Duration = 1.0 start_r = 255 start_g = 255 start_b = 255 start_a = 255 end_r = 255 end_g = 255 end_b = 255 end_a = 0
ENDSCRIPT

SCRIPT OptionsToMainMenuCamAnim
  MoveMenu Id = net_network_setup_menu x = 45
  IF IsTrue ReturnedFromLevelRecords
    change ReturnedFromLevelRecords = 0
  ELSE
    PlaySkaterCamAnim skater = 0 name = "Cam_Options01_Back" play_hold
  ENDIF
ENDSCRIPT

SCRIPT MainMenuToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01_Anim" play_hold
  EnableSkaterRotation
  SetCurrentSkaterProfile 0
ENDSCRIPT

SCRIPT Player1ToMainMenuCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01_BackAnim" play_hold
  ResetSkaterRotation
  DisableSkaterRotation
ENDSCRIPT

SCRIPT LevelSelectToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect01_BackAnim" play_hold
ENDSCRIPT

SCRIPT Player1ToLevelSelectCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect01_Anim" play_hold
  BuildCassetteMenu
  KillSpawnedScript name = WaitThenEnableCassettes
  SpawnScript WaitThenEnableCassettes params = <...>
ENDSCRIPT

SCRIPT Player1ToPlayer2CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu02_Anim" play_hold
  ResetSkaterRotation
ENDSCRIPT

SCRIPT Player2ToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu02_BackAnim" play_hold
  ResetSkaterRotation
ENDSCRIPT

SCRIPT Player2ToLevelSelectCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect02_Anim" play_hold
  BuildCassetteMenu
  KillSpawnedScript name = WaitThenEnableCassettes
  SpawnScript WaitThenEnableCassettes
  DisableSkaterRotation
ENDSCRIPT

SCRIPT LevelSelectToPlayer2CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect02_BackAnim" play_hold
  ResetSkaterRotation
  EnableSkaterRotation
ENDSCRIPT

SCRIPT Player1ToBoardShopCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect01_Anim" play_hold
ENDSCRIPT

SCRIPT BoardShopToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect01_BackAnim" play_hold
ENDSCRIPT

SCRIPT Player2ToBoardShopCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect02_Anim" play_hold
ENDSCRIPT

SCRIPT BoardShopToPlayer2CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect02_BackAnim" play_hold
ENDSCRIPT

SCRIPT WaitThenEnableCassettes
  DisableSkaterRotation
  PollUntilFinished func = SkaterCamAnimHeld skater = 0
  IF menuisshown cassette_menu
    FireMenuEvent Type = MAKECASSETTESFANOUT Id = cassette_menu target = cassette_menu
  ENDIF
ENDSCRIPT

ShouldShowCareerInfo = 1
AutoLinkToCareerScreen = 0

SCRIPT SetCareerMode
  SetNetworkMode
  SetGameType Career
  MainMenuToPlayer1CamAnim
  change ShouldShowCareerInfo = 1
ENDSCRIPT

SCRIPT SetSingleSessionMode
  SetNetworkMode
  SetGameType SingleSession
  MainMenuToPlayer1CamAnim
  change ShouldShowCareerInfo = 0
ENDSCRIPT

SCRIPT SetFreeSkateMode
  SetNetworkMode
  SetGameType FreeSkate
  MainMenuToPlayer1CamAnim
  change ShouldShowCareerInfo = 0
ENDSCRIPT

SCRIPT SetFreeSkate2pMode
  SetNetworkMode
  SetGameType FreeSkate2p
  MainMenuToPlayer1CamAnim
  change ShouldShowCareerInfo = 0
  SetSkaterRotationPad 0
  SyncPlayer2Profile
ENDSCRIPT

SCRIPT SetNetworkPlayMode
  test_network_settings
  change ShouldShowCareerInfo = 0
ENDSCRIPT

SCRIPT Player1ToPlayer2
  Player1ToPlayer2CamAnim
  SetFrontEndPad 1
  SetSkaterRotationPad 1
  SetCurrentSkaterProfile 1
  SliderShow
  //reload_cas //this function here crashes the game
ENDSCRIPT

SCRIPT Player2ToPlayer1
  Player2ToPlayer1CamAnim
  SetFrontEndPad 0
  SetSkaterRotationPad 0
  SetCurrentSkaterProfile 0
  SliderHide
ENDSCRIPT

SCRIPT Player2ToLevelSelect
  UseBothPadsInFrontEnd
  Player2ToLevelSelectCamAnim
ENDSCRIPT

SCRIPT Player1ToBoardshop
  Player1ToBoardShopCamAnim
  MakeSkaterGoto MoveToCounter skater = 0
ENDSCRIPT

SCRIPT Player2ToBoardshop
  Player2ToBoardShopCamAnim
  MakeSkaterGoto MoveToCounter skater = 1
ENDSCRIPT

SCRIPT Player1ToChangeAppearance
  SetCurrentSkaterProfile 0
ENDSCRIPT

SCRIPT Player2ToChangeAppearance
  SetCurrentSkaterProfile 1
ENDSCRIPT

SCRIPT TutorialLaunch
  SetNetworkMode
  SetGameType FreeSkate
  SetCurrentGameType
  SetCurrentSkaterProfile 0
  LevelSelectStartGame level = Load_Tut
ENDSCRIPT

SCRIPT ParkEditorLaunch
  SetFrontEndPad 0
  SetNetworkMode
  AutoLaunch level = custom_park Game = parkeditor
ENDSCRIPT

SCRIPT player_one_menu_on_show
  SetFrontEndPad 0
  SliderShow
ENDSCRIPT

SCRIPT player_two_menu_on_show
  update_pro_display_info <...>
  SliderShow
ENDSCRIPT

SCRIPT CareerMenuBack
  Player1ToMainMenuCamAnim
ENDSCRIPT

SCRIPT free_skate_menu_on_show
  update_pro_display_info <...>
  SliderShow
ENDSCRIPT

SCRIPT MenuQuitGame
  QuitGame
ENDSCRIPT

SCRIPT single_session_menu_on_show
  update_pro_display_info <...>
  SliderShow
ENDSCRIPT

SCRIPT invokeUIScreen
  InvokeScreen <...>
ENDSCRIPT

SCRIPT get_version_number
  GetVersionNumber <...>
ENDSCRIPT

menuitem_separator = { Type = textmenuelement auto_id text = "-------" static }

frontend_menu = { 
  Type = verticalmenu 
  Id = frontend_menu 
  parent = contain1 
  x = 45.0 y = 90.0 w = 300.0 h = 400.0 
  just_center_x just_center_y blue_top 
  eventhandler = { Type = showeventhandler target = "frontend_menu_on_show" } 
  children = frontend_menu_children
}

SCRIPT frontend_menu_on_show
  IF CD
    DestroyElement Id = mainmenu_debug
  ENDIF
  UseBothPadsInFrontEnd
  SetSkaterRotationPad Both
  InitLevelMod
  AddOptions
  InitLevelMod
ENDSCRIPT


frontend_menu_children = [ 
  { Type = textmenuelement auto_id text = "Main Menu" static dont_gray drawer = title dont_gray }
  { Type = textmenuelement auto_id text = "Career" link = career_menu target = "SetCareerMode"  }
  { Type = textmenuelement auto_id text = "Single Session" link = single_session_menu target = "SetSingleSessionMode" }
  { Type = textmenuelement auto_id text = "Free Skate" link = free_skate_menu target = "SetFreeSkateMode" }
  //{ Type = textmenuelement auto_id text = "2 Players" link = player_one_menu target = "SetFreeSkate2pMode" } //splitscreen 2 player menu mode
  { Type = textmenuelement Id = network_play_option text = "Network Play" target = "SetNetworkPlayMode" }
  { Type = textmenuelement auto_id text = "Create-a-Skater" link = pre_cas_main_menu target = "link_to_cas" }
  { Type = textmenuelement auto_id text = "Park Editor" target = "ParkEditorLaunch" }
  { Type = textmenuelement auto_id text = "Tutorials" target = "TutorialLaunch" }
  { Type = textmenuelement auto_id text = "Options"  link = options_main_menu target = "MainMenuToOptionsCamAnim" }

  //additional onscreen text in main menu
  { Type = textmenuelement auto_id text = " " x = -45.0 y = 330.0 w = 200 drawer = keyboard_property static dont_gray just_left_x dontdrawrect lock_layout lock_width eventhandler = { Type = showeventhandler target = "get_version_number" } }
  { Type = textmenuelement auto_id text = motd x = 0.0 y = 300.0 w = 1024 drawer = keyboard_property static dont_gray just_left_x dontdrawrect lock_layout }
  { Type = textmenuelement auto_id text = "LevelMod 2013-2020" x = 0.0 y = 305.0 w = 580 drawer = keyboard_property static dont_gray dontdrawrect just_right_x just_center_y lock_layout lock_width }
  { Type = textmenuelement auto_id text = "by DCxDemo*, Vadru'Qc" x = 0.0 y = 330.0 w = 580 drawer = keyboard_property static dont_gray dontdrawrect just_right_x just_center_y lock_layout lock_width }
  
  { Type = textmenuelement Id = mainmenu_debug text = "Debug" link = debug_main_menu } 
  { Type = textmenuelement auto_id text = "Quit" link = menuquitgame_yesno_menu }
]


menuquitgame_yesno_menu = { 
  Type = verticalmenu 
  Id = menuquitgame_yesno_menu 
  parent = contain1 
  x = 170.0 y = 150.0 w = 300.0 h = 336.0 
  just_center_x just_center_y blue_top 
  eventhandler = { Type = showeventhandler target = "Select_no" } 
  children = [ 
    { Type = textmenuelement auto_id text = "Quit?" static dont_gray drawer = title }
    { Type = textmenuelement Id = quit_yes text = "Yes" target = "MenuQuitGame" }
    { Type = textmenuelement Id = quit_no text = "No" link = frontend_menu } 
  ] 
}


player_one_menu = { 
  Type = verticalmenu 
  Id = player_one_menu 
  parent = contain1 
  x = 45.0 y = 90.0 w = 300.0 h = 400.0 
  just_center_x just_center_y blue_top 
  eventhandlers = player_one_menu_eventhandlers
  children = player_one_menu_children 
}

player_one_menu_eventhandlers = [ 
  { Type = backeventhandler target = "Player1ToMainMenuCamAnim" }
  { Type = showeventhandler target = "player_one_menu_on_show" }
  { Type = showeventhandler target = "update_pro_display_info" params = { name_menu_item = pro_current_name_player_one } }
  { Type = hideeventhandler target = "SliderHide" } 
] 

player_one_menu_children = [ 
  { Type = textmenuelement auto_id text = "Player 1" static dont_gray drawer = title }
  { Type = textmenuelement auto_id text = "Continue" link = player_two_menu target = "Player1ToPlayer2" }
  { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" }
  { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" }
  { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container }
  { menuitem_separator }
  { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu }
  { Type = textmenuelement auto_id text = "Load Skater" target = "_Player1LoadCAS" } 
  { Type = textmenuelement auto_id text = "Done" target = "go_back" params = { Id = player_one_menu } } 
]

//splitscreen menu. reload_cas function crashes the menu on loading, commented that out
//now it loads in the game, but changing anything in the player 2 menu crashes the game.
player_two_menu = { 
  Type = verticalmenu 
  Id = player_two_menu 
  parent = contain1 
  x = 295.0 y = 90.0 w = 300.0 h = 400.0 
  just_center_x just_center_y blue_top 
  eventhandlers = player_two_menu_eventhandlers 
  children = player_two_menu_children
}

player_two_menu_eventhandlers = [ 
  { Type = backeventhandler target = "Player2ToPlayer1" }
  { Type = showeventhandler target = "player_two_menu_on_show" params = { name_menu_item = pro_current_name_player_two } }
  { Type = hideeventhandler target = "SliderHide" } 
]

player_two_menu_children = [ 
  { Type = textmenuelement auto_id text = "Player 2" static dont_gray drawer = title }
  { Type = textmenuelement auto_id text = "Continue" link = cassette_menu target = "Player2ToLevelSelect" }
  { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player2ToChangeAppearance" }
  { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player2ToBoardshop" }
  { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container }
  { menuitem_separator }
  { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu }
  { Type = textmenuelement auto_id text = "Load Skater" target = "_Player2LoadCAS" } 
  { Type = textmenuelement auto_id text = "Done" target = "go_back" params = { Id = player_two_menu } } 
]


career_menu = { 
  Type = verticalmenu 
  Id = career_menu 
  parent = contain1 
  x = 45.0 y = 90.0 w = 300.0 h = 400.0 
  just_center_x just_center_y blue_top 
  eventhandlers = career_menu_eventhandlers
  children = career_menu_children
}

career_menu_eventhandlers = [ 
  { Type = backeventhandler target = "CareerMenuBack" }
  { Type = showeventhandler target = "CareerMenuShow" }
  { Type = hideeventhandler target = "CareerMenuBackOut" }
  { Type = showeventhandler target = "update_pro_display_info" params = { name_menu_item = pro_current_name_career } } 
]

career_menu_children = [ 
  { Type = textmenuelement auto_id text = "Career" static dont_gray drawer = title }
  { Type = textmenuelement auto_id text = "Play Level" link = cassette_menu target = "Player1ToLevelSelectCamAnim" }
  { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" }
  { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" }
  { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container }
  { menuitem_separator }
  { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu }
  { Type = textmenuelement auto_id text = "Load Skater" target = "_CareerMenuLoadCAS" }
  { Type = textmenuelement auto_id text = "Done" target = "go_back" params = { Id = career_menu } } 
] 

single_session_menu = { 
  Type = verticalmenu 
  Id = single_session_menu 
  parent = contain1
  x = 45.0 y = 90.0 w = 300.0 h = 400.0 
  just_center_x just_center_y blue_top 
  eventhandlers = single_session_menu_eventhandlers
  children = single_session_menu_children
}

single_session_menu_eventhandlers = [ 
  { Type = backeventhandler target = "Player1ToMainMenuCamAnim" }
  { Type = showeventhandler target = "single_session_menu_on_show" params = { name_menu_item = pro_current_name_single_session } }
  { Type = hideeventhandler target = "SliderHide" } 
] 

single_session_menu_children =  [ 
  { Type = textmenuelement auto_id text = "Session" static dont_gray drawer = title }
  //{ Type = textmenuelement auto_id text = "Play Level" link = level_main_menu } //originally IF IsTrue run_viewer this would be used
  //{ Type = textmenuelement auto_id text = "Play Level" link = cassette_menu target = "Player1ToLevelSelectCamAnim" }
  { Type = textmenuelement auto_id text = "Play Level" link = GameChangeLevelMenu target = "Player1ToLevelSelectCamAnim" }
  { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" }
  { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" }
  { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container }
  { menuitem_separator }
  { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu }
  { Type = textmenuelement auto_id text = "Load Skater" target = "_SingleSessionLoadCAS" }
  { Type = textmenuelement auto_id text = "Done" target = "go_back" params = { Id = single_session_menu } } 
] 


free_skate_menu = { 
  Type = verticalmenu 
  Id = free_skate_menu 
  parent = contain1
  x = 45.0 y = 90.0 w = 300.0 h = 400.0 
  just_center_x just_center_y blue_top 
  eventhandlers = free_skate_menu_eventhandlers
  children = free_skate_menu_children
}

free_skate_menu_eventhandlers = [ 
  { Type = backeventhandler target = "Player1ToMainMenuCamAnim" }
  { Type = showeventhandler target = "free_skate_menu_on_show" params = { name_menu_item = pro_current_name_free_skate } }
  { Type = hideeventhandler target = "SliderHide" } 
] 

free_skate_menu_children = [ 
  { Type = textmenuelement auto_id text = "Free Skate" static dont_gray drawer = title }
  //{ Type = textmenuelement auto_id text = "Play Level" link = level_main_menu } //if istrue run_viewer
  //{ Type = textmenuelement auto_id text = "Play Level" link = cassette_menu target = "Player1ToLevelSelectCamAnim" }
  { Type = textmenuelement auto_id text = "Play Level" link = GameChangeLevelMenu target = "Player1ToLevelSelectCamAnim" }
  { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" }
  { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" }
  { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container }
  { menuitem_separator }
  { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu }
  { Type = textmenuelement auto_id text = "Load Skater" target = "_FreeSkateLoadCAS" }
  { Type = textmenuelement auto_id text = "Done" target = "go_back" params = { Id = free_skate_menu } } 
] 

SCRIPT main_menu_create
  CreateAndAttachMenu { frontend_menu }
  CreateAndAttachMenu { menuquitgame_yesno_menu } 
  CreateAndAttachMenu { player_one_menu }
  CreateAndAttachMenu { player_two_menu }
  CreateAndAttachMenu { career_menu }
  CreateAndAttachMenu { single_session_menu }
  CreateAndAttachMenu { free_skate_menu }

  //what's this?
  CreateMenu { 
    Type = verticalmenu 
    Id = load_career_menu 
    x = 45.0 y = 90.0 w = 300.0 h = 400.0 
    just_center_x just_center_y blue_top 
    children = [ 
      { Type = textmenuelement auto_id text = "Load Skater" static dont_gray drawer = title }
      { Type = textmenuelement auto_id text = "Tony Hawk 1" }
      { Type = textmenuelement auto_id text = "Tony Hawk 2" }
      { Type = textmenuelement auto_id text = "Rune Glifberg" }
      { Type = textmenuelement auto_id text = "Created 1" }
      { Type = textmenuelement auto_id text = "Created 2" } 
    ] 
  }
  
  CreateMenu {
    Type = verticalmenu 
    Id = career_info 
    x = 45.0 y = 290.0 w = 300.0 h = 400.0 
    just_center_x just_center_y static dont_gray 
    children = [ 
      { Type = textmenuelement Id = career_info_line_1 text = "Poo poo pants" static dont_gray drawer = goals_text drawer = title_medium truncate_to_width }
      { Type = textmenuelement Id = career_info_line_2 text = "Goals: 12 of 35" static dont_gray drawer = goals_text }
      { 
        Type = horizontalmenu 
        Id = career_info_horizontal_menu 
        static dont_gray 
        children = [ 
          { Type = textmenuelement Id = career_info_line_3 text = "Medals:" static dont_gray drawer = goals_text lock_layout x = 40 y = 8 }
          { Type = iconmenuelement Id = career_info_rio_medal_1 image = "panelsprites\medal_1.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_Gold dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_rio_medal_2 image = "panelsprites\medal_2.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_Silver dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_rio_medal_3 image = "panelsprites\medal_3.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_Bronze dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_rio_medal_4 image = "panelsprites\medal_locked.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_NoMedal dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_si_medal_1 image = "panelsprites\medal_1.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_Gold dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_si_medal_2 image = "panelsprites\medal_2.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_Silver dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_si_medal_3 image = "panelsprites\medal_3.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_Bronze dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_si_medal_4 image = "panelsprites\medal_locked.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_NoMedal dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_tokyo_medal_1 image = "panelsprites\medal_1.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_Gold dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_tokyo_medal_2 image = "panelsprites\medal_2.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_Silver dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_tokyo_medal_3 image = "panelsprites\medal_3.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_Bronze dontdrawrect static dont_gray }
          { Type = iconmenuelement Id = career_info_tokyo_medal_4 image = "panelsprites\medal_locked.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_NoMedal dontdrawrect static dont_gray } 
        ] 
      } 
    ] 
  }
  
  CreateMenu { 
    Type = verticalmenu 
    Id = slider_info 
    x = 45.0 y = 290.0 w = 300.0 h = 400.0 
    just_center_x just_center_y 
    children = [ 
      { Type = textmenuelement Id = slider_info_line_1 text = "Poo poo pants" static dont_gray drawer = goals_text drawer = title_medium truncate_to_width }
      { Type = textmenuelement Id = slider_info_line_2 text = "Trickstyle: Vert" static dont_gray drawer = goals_text }
      { Type = textmenuelement Id = slider_info_line_3 text = "Stance: Goofy" static dont_gray drawer = goals_text } 
    ] 
  }
  
  CreateBoardShopMenu
  CreateDeckMenu
  CreateWheelColorMenu
  pre_cas_menu_create
  CreateNewChangeLevelMenu
  
  AttachChild parent = contain1 child = load_career_menu
  AttachChild parent = contain1 child = boardshop_menu
  AttachChild parent = contain1 child = pre_cas_main_menu
  AttachChild parent = contain1 child = deck_menu
  AttachChild parent = contain1 child = career_info
  AttachChild parent = contain1 child = slider_info
  AttachChild parent = contain1 child = wheel_color_menu
  
  helper_menu_create
  pro_menu_create
  cassette_menu_create
ENDSCRIPT

SCRIPT LevelSelectBackOut
  ClearCassetteMenu
  IF CurrentSkaterProfileIs 1
    LevelSelectToPlayer2CamAnim
    SetFrontEndPad 1
  ELSE
    LevelSelectToPlayer1CamAnim
  ENDIF
  ResetSkaterRotation
  EnableSkaterRotation
  InitTVScreensVisibility
ENDSCRIPT

SCRIPT InitTVScreensVisibility
  invisible prefix = "ss_go_TV_"
  visible name = ss_go_TV_THPScreen
ENDSCRIPT

SCRIPT LevelSelectStartGame
  IF CurrentSkaterProfileIs 1
    SetGameType trickattack
    UseBothPadsInFrontEnd
  ELSE
  ENDIF
  SetCurrentSkaterProfile 0
  SetSkaterRotationPad 0
  immediately_start_game <...>
ENDSCRIPT

ParkEditorCassetteTurnRate = -0.05

SCRIPT SwitchOffCassetteMenuHeading
  FireMenuEvent Type = HIDE Id = cassette_menu_level_name target = cassette_menu_level_name
  FireMenuEvent Type = HIDE Id = cassette_menu_dummy_line target = cassette_menu_dummy_line
ENDSCRIPT

SCRIPT SwitchOnCassetteMenuHeading
  FireMenuEvent Type = SHOW Id = cassette_menu_level_name target = cassette_menu_level_name
  FireMenuEvent Type = SHOW Id = cassette_menu_dummy_line target = cassette_menu_dummy_line
ENDSCRIPT

SCRIPT PlayCustomParkFromCassetteMenu
  ClearCassetteMenu
  immediately_start_game <...>
ENDSCRIPT

SCRIPT UpdateParkNameInMenuTitles
  GetParkName
  SetMenuElementText Id = cassette_menu_park_editor_menu_title <ParkName>
  SetMenuElementText Id = cassette_menu_dummy_line <ParkName>
ENDSCRIPT

SCRIPT cassette_menu_create
  CreateMenu { Type = cassettemenu Id = cassette_menu eventhandlers = [ { Type = backeventhandler target = "LevelSelectBackOut" }
      { Type = showeventhandler target = "UpdateParkNameInMenuTitles" } ] AppearanceParams = CassetteMenuParams }
  CreateMenu { Type = verticalmenu Id = cassette_menu_park_editor_menu x = 255.0 y = 50.0 w = 360.0 h = 230.0 just_center_x just_center_y blue_top eventhandler = { Type = backeventhandler target = "SwitchOnCassetteMenuHeading" } children = [ { Type = textmenuelement Id = cassette_menu_park_editor_menu_title text = " " static dont_gray drawer = title truncate_to_width }
      { Type = textmenuelement auto_id text = "Play Park" target = "PlayCustomParkFromCassetteMenu" params = { level = custom_park string = "Play custom park" } }
      { Type = textmenuelement auto_id text = "Load Custom Park" target = "_CassetteMenuParkEditorLoad" }
      { Type = textmenuelement auto_id text = "Pre-made parks" link = cassette_menu_parked_load_menu }
      { Type = textmenuelement auto_id text = "Back" target = "go_back" params = { Id = cassette_menu_park_editor_menu } } ] }
  AttachChild parent = contain1 child = cassette_menu
  AttachChild parent = contain1 child = cassette_menu_park_editor_menu
ENDSCRIPT

SCRIPT _LoadParkFromDisc
  SwitchOffMenu Id = cassette_menu_parked_load_menu
  SpawnScript LoadParkFromDisc params = <...>
ENDSCRIPT

SCRIPT LoadParkFromDisc
  Message_LoadingParkFromDisc
  IF ParkEditorCommand command = accessDisk file = <file> DoNotCleanupAndInitialize
    Message_LoadSuccessful
  ELSE
    Message_LoadParkFromDiscFailed
  ENDIF
  UpdateParkNameInMenuTitles
  Wait 0.5 seconds
  SwitchToMenu DoNotMakeRoot menu = cassette_menu_park_editor_menu
ENDSCRIPT

SCRIPT ClearCassetteMenu
  DestroyElement Id = cassette_menu_heading
  DestroyElement Id = cassette_menu_goals_left
  DestroyElement Id = cassette_menu_records_1
  DestroyElement Id = cassette_menu_records_2
  DestroyElement Id = cassette_menu_stat_points_text
  DestroyElement Id = cassette_menu_stat_points_pips
  DestroyElement Id = cassette_menu_got_deck_text
  DestroyElement Id = cassette_menu_line_1
  DestroyElement Id = cassette_menu_line_2
  DestroyElement Id = cassette_menu_best_medal_text
  DestroyElement Id = cassette_menu_best_medal_gold
  DestroyElement Id = cassette_menu_best_medal_silver
  DestroyElement Id = cassette_menu_best_medal_bronze
  DestroyElement Id = cassette_menu_best_medal_none
  DestroyElement Id = cassette_menu_parked_load_menu
  DestroyElement Id = cassette_editor
  DestroyElement Id = cassette_foundry
  DestroyElement Id = cassette_canada
  DestroyElement Id = cassette_rio
  DestroyElement Id = cassette_suburbia
  DestroyElement Id = cassette_airport
  DestroyElement Id = cassette_skater_island
  DestroyElement Id = cassette_los_angeles
  DestroyElement Id = cassette_tokyo
  DestroyElement Id = cassette_oil
  DestroyElement Id = cassette_rooftops
  DestroyElement Id = cassette_ship
  DestroyElement Id = cassette_warehouse
  DestroyElement Id = cassette_burnside
  DestroyElement Id = cassette_roswell
ENDSCRIPT

UnlockRequirements = [ 
  { level = LevelNum_Canada 3 Goals }
  { level = LevelNum_Rio 10 Goals }
  { level = LevelNum_Suburbia 1 Medal }
  { level = LevelNum_Airport 18 Goals }
  { level = LevelNum_SkaterIsland 26 Goals }
  { level = LevelNum_LA 2 Medals }
  { level = LevelNum_Tokyo 35 Goals }
  { level = LevelNum_Ship 3 Medals } 
]

SCRIPT BuildCassetteMenu
  CreateMenu { Type = verticalmenu Id = cassette_menu_heading x = 280.0 y = 50.0 w = 300.0 h = 230.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement Id = cassette_menu_level_name text = "Custom Park" static dont_gray drawer = title }
      { Type = textmenuelement Id = cassette_menu_dummy_line text = " " static dont_gray drawer = goals_text } ] }
  CreateMenu { Type = textmenuelement Id = cassette_menu_stat_points_text text = "Stat points:" just_center_x lock_layout static dont_gray drawer = goals_text x = 280 y = 80 w = 150 }
  CreateMenu { Type = statsmenuelement Id = cassette_menu_stat_points_pips text = "---" implemented_property just_center_x x = 430 y = 80 w = 70 h = 23 ghosted dont_gray pip_spacing = cas_pip_spacing lower = 0 upper = 5 start_value = 0 }
  CreateMenu { Type = textmenuelement Id = cassette_menu_got_deck_text text = "Deck: Yes" just_center_x lock_layout x = 500 y = 80 w = 80 h = 23 static dont_gray drawer = goals_text }
  CreateMenu { Type = textmenuelement Id = cassette_menu_line_1 text = "Goals completed:" static dont_gray drawer = goals_text just_center_x x = 280 y = 103 w = 150 }
  CreateMenu { Type = textmenuelement Id = cassette_menu_line_2 text = " " static dont_gray drawer = goals_text just_center_x x = 430 y = 103 w = 150 }
  CreateMenu { Type = textmenuelement Id = cassette_menu_best_medal_text text = "Best medal:" static dont_gray drawer = goals_text lock_layout just_center_x x = 320 y = 115 w = 150 }
  CreateMenu { Type = iconmenuelement Id = cassette_menu_best_medal_gold image = "panelsprites\medal_1.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_Gold dontdrawrect }
  CreateMenu { Type = iconmenuelement Id = cassette_menu_best_medal_silver image = "panelsprites\medal_2.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_Silver dontdrawrect }
  CreateMenu { Type = iconmenuelement Id = cassette_menu_best_medal_bronze image = "panelsprites\medal_3.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_Bronze dontdrawrect }
  CreateMenu { Type = iconmenuelement Id = cassette_menu_best_medal_none image = "panelsprites\medal_locked.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_NoMedal dontdrawrect }
  CreateMenu { Type = verticalmenu Id = cassette_menu_goals_left x = 280.0 y = 170.0 w = 296.0 h = 250.0 just_center_x just_center_y children = [ { Type = textmenuelement Id = cassette_goals_line_1 text = "Level is locked !!!" static dont_gray drawer = goals_text }
      { Type = textmenuelement Id = cassette_goals_line_2 text = " " static dont_gray drawer = goals_text } ] }
  CreateMenu { x = 280 y = 80 w = 300 just_center_x just_center_y drawer = goals_text Type = textmenuelement Id = cassette_menu_records_1 text = "Records:" static dont_gray }
  CreateMenu { x = 280 y = 102 w = 300 just_center_x just_center_y drawer = goals_text Type = textmenuelement Id = cassette_menu_records_2 text = " " static dont_gray }
  CreateMenu { Type = verticalmenu Id = cassette_menu_parked_load_menu x = 255.0 y = 50.0 w = 360.0 h = 230.0 just_center_x just_center_y blue_top children = [ 
    { Type = textmenuelement auto_id text = "Pre-Made Parks" static dont_gray drawer = title }
    { Type = scrollingmenu auto_id x = 0.0 y = 0.0 w = 350.0 h = 400.0 just_center_x just_center_y num_visible = 9 children = [ 
    { Type = textmenuelement Id = parked_load1 text = "Park 1" target = "_LoadParkFromDisc" params = { file = 1 } }
      { Type = textmenuelement Id = parked_load2 text = "Park 2" target = "_LoadParkFromDisc" params = { file = 2 } }
      { Type = textmenuelement Id = parked_load3 text = "Park 3" target = "_LoadParkFromDisc" params = { file = 3 } }
      { Type = textmenuelement Id = parked_load4 text = "Park 4" target = "_LoadParkFromDisc" params = { file = 4 } }
      { Type = textmenuelement Id = parked_load5 text = "Park 5" target = "_LoadParkFromDisc" params = { file = 5 } }
      { Type = textmenuelement Id = parked_load6 text = "Park 6" target = "_LoadParkFromDisc" params = { file = 6 } }
      { Type = textmenuelement Id = parked_load7 text = "Park 7" target = "_LoadParkFromDisc" params = { file = 7 } }
      { Type = textmenuelement Id = parked_load8 text = "Park 8" target = "_LoadParkFromDisc" params = { file = 8 } }
      { Type = textmenuelement Id = parked_load9 text = "Park 9" target = "_LoadParkFromDisc" params = { file = 9 } }
      { Type = textmenuelement Id = parked_load10 text = "Park 10" target = "_LoadParkFromDisc" params = { file = 10 } }
      { Type = textmenuelement Id = parked_load11 text = "Park 11" target = "_LoadParkFromDisc" params = { file = 11 } }
      { Type = textmenuelement Id = parked_load12 text = "Park 12" target = "_LoadParkFromDisc" params = { file = 12 } }
      { Type = textmenuelement Id = parked_load13 text = "Park 13" target = "_LoadParkFromDisc" params = { file = 13 } }
      { Type = textmenuelement Id = parked_load14 text = "Park 14" target = "_LoadParkFromDisc" params = { file = 14 } }
      { Type = textmenuelement Id = parked_load15 text = "Park 15" target = "_LoadParkFromDisc" params = { file = 15 } }
      { Type = textmenuelement Id = parked_load16 text = "Park 16" target = "_LoadParkFromDisc" params = { file = 16 } }
      { Type = textmenuelement Id = parked_load17 text = "Park 17" target = "_LoadParkFromDisc" params = { file = 17 } }
      { Type = textmenuelement Id = parked_load18 text = "Park 18" target = "_LoadParkFromDisc" params = { file = 18 } }
      { Type = textmenuelement Id = parked_load19 text = "Park 19" target = "_LoadParkFromDisc" params = { file = 19 } }
      { Type = textmenuelement Id = parked_load20 text = "Park 20" target = "_LoadParkFromDisc" params = { file = 20 } } 
    ] } 
  ] }
  AttachChild parent = contain1 child = cassette_menu_heading
  AttachChild parent = contain1 child = cassette_menu_goals_left
  AttachChild parent = contain1 child = cassette_menu_records_1
  AttachChild parent = contain1 child = cassette_menu_records_2
  AttachChild parent = contain1 child = cassette_menu_stat_points_text
  AttachChild parent = contain1 child = cassette_menu_stat_points_pips
  AttachChild parent = contain1 child = cassette_menu_got_deck_text
  AttachChild parent = contain1 child = cassette_menu_line_1
  AttachChild parent = contain1 child = cassette_menu_line_2
  AttachChild parent = contain1 child = cassette_menu_best_medal_text
  AttachChild parent = contain1 child = cassette_menu_best_medal_gold
  AttachChild parent = contain1 child = cassette_menu_best_medal_silver
  AttachChild parent = contain1 child = cassette_menu_best_medal_bronze
  AttachChild parent = contain1 child = cassette_menu_best_medal_none
  AttachChild parent = contain1 child = cassette_menu_parked_load_menu
  SpawnScript PositionStatPips
  SetGlobalFlag flag = LEVEL_UNLOCKED_FOUNDRY
  IF TestRequestedGameType Career
  ELSE
    CreateMenu { Type = cassettemenuelement Id = cassette_editor ModelName = Editor TVImageName = ss_go_TV_editor LockedTVImageName = ss_go_TV_editor LevelName = '' LevelNumber = 0 parkeditor eventhandler = { Type = ChooseEventHandler target = "SwitchOffCassetteMenuHeading" link = cassette_menu_park_editor_menu } }
    AttachChild child = cassette_editor parent = cassette_menu
  ENDIF
  CreateMenu { Type = cassettemenuelement Id = cassette_foundry ModelName = Foundry TVImageName = ss_go_TV_foundry LockedTVImageName = ss_go_TV_foundry_l LevelName = 'Foundry' LevelNumber = LevelNum_Foundry GoalParams = FoundryGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Foun } } }
  AttachChild child = cassette_foundry parent = cassette_menu
  CreateMenu { Type = cassettemenuelement Id = cassette_canada ModelName = Canada TVImageName = ss_go_TV_Can LockedTVImageName = ss_go_TV_Can_l LevelName = 'Canada' LevelNumber = LevelNum_Canada GlobalFlag = LEVEL_UNLOCKED_CANADA GoalParams = CanadaGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Can } } }
  AttachChild child = cassette_canada parent = cassette_menu
  CreateMenu { Type = cassettemenuelement Id = cassette_rio ModelName = Rio TVImageName = ss_go_TV_Rio LockedTVImageName = ss_go_TV_Rio_l LevelName = 'Rio' LevelNumber = LevelNum_Rio GlobalFlag = LEVEL_UNLOCKED_RIO GoalParams = DefaultGoalParams CompetitionLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Rio } } }
  AttachChild child = cassette_rio parent = cassette_menu
  CreateMenu { Type = cassettemenuelement Id = cassette_suburbia ModelName = Suburbia TVImageName = ss_go_TV_Sub LockedTVImageName = ss_go_TV_Sub_l LevelName = 'Suburbia' LevelNumber = LevelNum_Suburbia GlobalFlag = LEVEL_UNLOCKED_SUBURBIA GoalParams = SuburbiaGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Sub } } }
  AttachChild child = cassette_suburbia parent = cassette_menu
  CreateMenu { Type = cassettemenuelement Id = cassette_airport ModelName = Airport TVImageName = ss_go_TV_ap LockedTVImageName = ss_go_TV_ap_l LevelName = 'Airport' LevelNumber = LevelNum_Airport GlobalFlag = LEVEL_UNLOCKED_AIRPORT GoalParams = AirportGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Ap } } }
  AttachChild child = cassette_airport parent = cassette_menu
  CreateMenu { Type = cassettemenuelement Id = cassette_skater_island ModelName = Skater_Island TVImageName = ss_go_TV_si LockedTVImageName = ss_go_TV_si_l LevelName = 'Skater Island' LevelNumber = LevelNum_SkaterIsland GlobalFlag = LEVEL_UNLOCKED_SKATERISLAND GoalParams = DefaultGoalParams CompetitionLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Si } } }
  AttachChild child = cassette_skater_island parent = cassette_menu
  CreateMenu { Type = cassettemenuelement Id = cassette_los_angeles ModelName = Los_Angeles TVImageName = ss_go_TV_la LockedTVImageName = ss_go_TV_la_l LevelName = 'Los Angeles' LevelNumber = LevelNum_LA GlobalFlag = LEVEL_UNLOCKED_LOSANGELES GoalParams = LosAngelesGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_La } } }
  AttachChild child = cassette_los_angeles parent = cassette_menu
  CreateMenu { Type = cassettemenuelement Id = cassette_tokyo ModelName = Tokyo TVImageName = ss_go_TV_tok LockedTVImageName = ss_go_TV_tok_l LevelName = 'Tokyo' LevelNumber = LevelNum_Tokyo GlobalFlag = LEVEL_UNLOCKED_TOKYO GoalParams = DefaultGoalParams CompetitionLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Tok } } }
  AttachChild child = cassette_tokyo parent = cassette_menu
  IF IsTrue AllLevelsUnlocked
    CreateShipCassette
  ELSE
    IF GetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
      CreateShipCassette
    ENDIF
  ENDIF
  IF TestRequestedGameType Career
  ELSE
    IF IsTrue AllLevelsUnlocked
      CreateWarehouseCassette
      CreateBurnsideCassette
      CreateRoswellCassette
    ELSE
      IF GetGlobalFlag flag = LEVEL_UNLOCKED_WAREHOUSE
        CreateWarehouseCassette
      ENDIF
      IF GetGlobalFlag flag = LEVEL_UNLOCKED_BURNSIDE
        CreateBurnsideCassette
      ENDIF
      IF GetGlobalFlag flag = LEVEL_UNLOCKED_ROSWELL
        CreateRoswellCassette
      ENDIF
    ENDIF
  ENDIF
  IF IsTrue XBox
    IF TestRequestedGameType Career
    ELSE
      CreateMenu { Type = cassettemenuelement Id = cassette_oil ModelName = Ship TVImageName = ss_go_TV_THPScreen LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Oil Rig' GoalParams = DefaultGoalParams SpecialLevel LevelNumber = 0 eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Oil } } }
      AttachChild child = cassette_oil parent = cassette_menu
    ENDIF
  ENDIF
  IF IsTrue GameCube
    IF TestRequestedGameType Career
    ELSE
      CreateMenu { Type = cassettemenuelement Id = cassette_rooftops ModelName = Ship TVImageName = ss_go_TV_THPScreen LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Rooftops' GoalParams = DefaultGoalParams SpecialLevel LevelNumber = 0 eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Roof } } }
      AttachChild child = cassette_rooftops parent = cassette_menu
    ENDIF
  ENDIF
ENDSCRIPT

SCRIPT CreateShipCassette
  CreateMenu { Type = cassettemenuelement Id = cassette_ship ModelName = Ship TVImageName = ss_go_TV_ship LockedTVImageName = ss_go_TV_ship_l LevelName = 'Cruise Ship' LevelNumber = LevelNum_Ship GlobalFlag = LEVEL_UNLOCKED_SHIP GoalParams = DefaultGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Shp } } }
  AttachChild child = cassette_ship parent = cassette_menu
ENDSCRIPT

SCRIPT CreateWarehouseCassette
  CreateMenu { Type = cassettemenuelement Id = cassette_warehouse ModelName = Special1_WH TVImageName = ss_go_TV_sp1 LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Warehouse' LevelNumber = LevelNum_Warehouse GlobalFlag = LEVEL_UNLOCKED_WAREHOUSE GoalParams = DefaultGoalParams SpecialLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Ware } } }
  AttachChild child = cassette_warehouse parent = cassette_menu
ENDSCRIPT

SCRIPT CreateBurnsideCassette
  CreateMenu { Type = cassettemenuelement Id = cassette_burnside ModelName = Special2_BS TVImageName = ss_go_TV_sp2 LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Burnside' LevelNumber = LevelNum_Burnside GlobalFlag = LEVEL_UNLOCKED_BURNSIDE GoalParams = DefaultGoalParams SpecialLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Burn } } }
  AttachChild child = cassette_burnside parent = cassette_menu
ENDSCRIPT

SCRIPT CreateRoswellCassette
  CreateMenu { Type = cassettemenuelement Id = cassette_roswell ModelName = Special3_ros TVImageName = ss_go_TV_sp3 LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Roswell' LevelNumber = LevelNum_Roswell GlobalFlag = LEVEL_UNLOCKED_ROSWELL GoalParams = DefaultGoalParams SpecialLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen params = { level = Load_Ros } } }
  AttachChild child = cassette_roswell parent = cassette_menu
ENDSCRIPT

SCRIPT PositionStatPips
  MoveMenu Id = cassette_menu_stat_points_pips x = 430 y = 80 w = 70 h = 23
ENDSCRIPT

CassetteMenuParams = { 
  SourceNodeName = TRG_Videxplode 
  NumFramesToFanOut = 20 
  FanOutPause = 1 
  NumFramesToFanIn = 20 
  FanInPause = 1 
  NumVisible = 6 
  XSeparation = 90 
  NumFramesToMoveSideways = 10 
  YCoord = 330 
  UnselectedZ = 40 
  SelectedZ = 20 
  NumFramesToFlip = 50 
  BigX = 320 
  BigY = 240 
  BigZ = 5 
  NumFramesToGetBig = 20 
}

GoalInfoTopY = 60
GoalInfoDY = 35

SCRIPT CareerMenuShow
  IF menuisshown pro_menu
  ELSE
    MakeMenuPopOffScreen Id = career_info Left
  ENDIF
  FireMenuEvent Type = SHOW Id = career_info target = career_info
  FireMenuEvent Type = HIDE Id = career_info_rio_medal_1 target = career_info_rio_medal_1
  FireMenuEvent Type = HIDE Id = career_info_rio_medal_2 target = career_info_rio_medal_2
  FireMenuEvent Type = HIDE Id = career_info_rio_medal_3 target = career_info_rio_medal_3
  FireMenuEvent Type = HIDE Id = career_info_rio_medal_4 target = career_info_rio_medal_4
  FireMenuEvent Type = HIDE Id = career_info_si_medal_1 target = career_info_si_medal_1
  FireMenuEvent Type = HIDE Id = career_info_si_medal_2 target = career_info_si_medal_2
  FireMenuEvent Type = HIDE Id = career_info_si_medal_3 target = career_info_si_medal_3
  FireMenuEvent Type = HIDE Id = career_info_si_medal_4 target = career_info_si_medal_4
  FireMenuEvent Type = HIDE Id = career_info_tokyo_medal_1 target = career_info_tokyo_medal_1
  FireMenuEvent Type = HIDE Id = career_info_tokyo_medal_2 target = career_info_tokyo_medal_2
  FireMenuEvent Type = HIDE Id = career_info_tokyo_medal_3 target = career_info_tokyo_medal_3
  FireMenuEvent Type = HIDE Id = career_info_tokyo_medal_4 target = career_info_tokyo_medal_4
  UpdateCareerInfoMenu
  SpawnScript MaybeWhizz
ENDSCRIPT

SCRIPT MaybeWhizz
  IF ControllerPressed Left
    MoveMenu Id = career_info x = 45
  ELSE
    IF ControllerPressed Right
      MoveMenu Id = career_info x = 45
    ELSE
      MakeMenuZoomOn Id = career_info FromLeft
    ENDIF
  ENDIF
ENDSCRIPT

SCRIPT CareerMenuBackOut
  FireMenuEvent Type = HIDE Id = career_info target = career_info
ENDSCRIPT

SCRIPT SetDefaultSliderPos
  IF CurrentSkaterProfileIs 1
    MoveMenu Id = slider_info x = 295
  ELSE
    MoveMenu Id = slider_info x = 45
  ENDIF
ENDSCRIPT

SCRIPT SliderShow
  SetDefaultSliderPos
  IF menuisshown pro_menu
  ELSE
    IF CurrentSkaterProfileIs 1
      MakeMenuPopOffScreen Id = slider_info Right
    ELSE
      MakeMenuPopOffScreen Id = slider_info Left
    ENDIF
  ENDIF
  FireMenuEvent Type = SHOW Id = slider_info target = slider_info
  UpdateSliderInfoMenu
  SpawnScript SliderMaybeWhizz
ENDSCRIPT

SCRIPT SliderMaybeWhizz
  IF ControllerPressed Left
    SetDefaultSliderPos
  ELSE
    IF ControllerPressed Right
      SetDefaultSliderPos
    ELSE
      IF CurrentSkaterProfileIs 1
        MakeMenuPopOffScreen Id = slider_info Right
        MakeMenuZoomOn Id = slider_info FromRight
      ELSE
        MakeMenuZoomOn Id = slider_info FromLeft
      ENDIF
    ENDIF
  ENDIF
ENDSCRIPT

SCRIPT SliderHide
  FireMenuEvent Type = HIDE Id = slider_info target = slider_info
ENDSCRIPT

TotalGoals = 54
IconColor_Gold = { r = 234 g = 190 b = 92 a = 115 }
IconColor_Silver = { r = 167 g = 225 b = 236 a = 115 }
IconColor_Bronze = { r = 118 g = 72 b = 21 a = 115 }
IconColor_NoMedal = { r = 67 g = 125 b = 176 a = 115 }
LeftArrowIconElement = { Type = iconmenuelement image = "panelsprites\arrow_menu.png" rotate = -90 x = 350.0 y = 180.0 lock_layout dontdrawrect dont_gray r = 255 g = 255 b = 255 a = 115 }
RightArrowIconElement = { Type = iconmenuelement image = "panelsprites\arrow_menu.png" rotate = 90 x = 530.0 y = 180.0 lock_layout dontdrawrect dont_gray r = 255 g = 255 b = 255 a = 115 }

SCRIPT attach_back_button text = "Back"
  CreateAndAttachMenu { Type = textmenuelement auto_id parent = <menu_id> text = <text> target = "go_back" params = { Id = <menu_id> } }
ENDSCRIPT
