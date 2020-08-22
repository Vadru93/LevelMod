script start_main_menu
  SetFrontEndPad 0
  Request_Level level = Load_SkateShop
  SetGameType SkateShop
  SetGameState on
  reset_skateshop_skater
  HideLoadingScreen
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01" play_hold
  spawnscript main_menu_first_frame
endscript

DoAutoLoad = 1
ver = ""
motd = ""
HudIsOn = 1
AutoLoadRetryContinueVisible = 0

script ShatterAndDie
  Shatter <...>
  Kill <...>
endscript

script AutoLoad
  LoadFont "thps4.fnt"
  if PAL
    Change MinimumSpaceNeeded = 65
  endif
  if MemCardFileExists name = "Options and Pros" Type = OptionsAndPros
    DisplayMessage MessageScript = Message_LoadingData PauseLength = MemCardMessageShortPause
    Change MemCardLoadFailed = 0
    if LoadFromMemoryCard name = "Options and Pros" Type = OptionsAndPros
    else
      Change MemCardLoadFailed = 1
    endif
    if MemCardFileExists name = "Network settings" Type = NetworkSettings
      if LoadFromMemoryCard name = "Network settings" Type = NetworkSettings
      else
        Change MemCardLoadFailed = 1
      endif
    endif
    if CurrentSkaterIsPro
    else
      if CustomSkaterFilenameDefined
        GetCustomSkaterFilename
        if LoadFromMemoryCard name = <string> Type = cas
        else
          Change MemCardLoadFailed = 1
        endif
      endif
    endif
    RemoveAllSkaters
    ToggleSkater
    FireMenuEvent Type = refreshdecktextures Id = deck_menu target = deck_menu
    PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01" play_hold
    if istrue MemCardLoadFailed
      DisplayMessage MessageScript = Message_LoadFailed PauseLength = MemCardMessageErrorPause
    else
      movies_menu_create
      DisplayMessage MessageScript = Message_LoadSuccessful
    endif
  endif
  SwitchToMenu menu = frontend_menu DoNotMakeRoot
  Change DoAutoLoad = 0
  goto main_menu_first_frame
endscript

script _ReSpawnAutoload
  EnableBackEvent
  spawnscript AutoLoad
endscript

script _BackToMainMenuFirstFrame
  EnableBackEvent
  spawnscript BackToMainMenuFirstFrame
endscript

script BackToMainMenuFirstFrame
  SwitchToMenu menu = frontend_menu DoNotMakeRoot
  Change DoAutoLoad = 0
  goto main_menu_first_frame
endscript

script main_menu_first_frame
  if istrue run_viewer
    Change DoAutoLoad = 0
  endif
  if istrue DoAutoLoad
    goto AutoLoad
  endif
  Change FirstTimeInSplitscreen = 1
  Change FirstTimeFromSkateshop = 1
  if DoDeferredInternetConnect
  else
    if DeferredLaunchServer
    else
      if istrue AutoLinkToCareerScreen
        Change AutoLinkToCareerScreen = 0
        SetCareerMode
        SwitchToMenu menu = career_menu DoNotMakeRoot
      endif
    endif
  endif
endscript

script reset_skateshop_skater
  if CurrentSkaterProfileIs 0
    PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01" play_hold
  endif
  MakeSkaterGoto skater = 0 SkateShopDummyAI
  MakeSkaterGoto skater = 1 SkateShopDummyAI
endscript

script exit_main_menu
  UninitializeFrontEndSprites
endscript

script SkateShopDummyAI
  SwitchOnBoard
  SwitchOffAtomic special_item
  ClearExceptions
  SetQueueTricks NoTricks
  DisablePlayerInput
  SetRollingFriction 10000
  BlendPeriodOut 0
  begin
    if ProfileEquals is_named = wolverine
      PlayAnim Anim = WolverineIdle Cycle NoRestart Blendperiod = 0.0
    else
      if ProfileEquals is_named = demoness
        PlayAnim Anim = DemonessIdle Cycle NoRestart Blendperiod = 0.0
      else
        if ProfileEquals is_named = maul
          if flipped
          else
            flip
          endif
          SwitchOffAtomic special_item
          PlayAnim Anim = MaulIdle Cycle NoRestart Blendperiod = 0.0
          Wait 60 frames
          SwitchOnAtomic special_item
          PlaySound SaberHIT1
        else
          if ProfileEquals is_named = slater
            PlayAnim Anim = SlaterIdle Cycle NoRestart Blendperiod = 0.0
          else
            if ProfileEquals is_named = ollie
              PlayAnim Anim = OllieIdle Cycle NoRestart Blendperiod = 0.0
            else
              if ProfileEquals is_named = dick
                goto DickIdle
              else
                if ProfileEquals is_named = steamer
                  goto StandIdle
                else
                  if ProfileEquals is_named = muska
                    if flipped
                    else
                      flip
                    endif
                    GotoRandomScript [ MuskaIdle ]
                  else
                    if ProfileEquals is_named = margera
                      PlayAnim random( @Anim = BoardIdle @Anim = BoardIdle @Anim = BoardIdle @Anim = BoardIdle @Anim = BamScratch @Anim = BoardLook @Anim = BoardMove ) Blendperiod = 0.0
                    else
                      if ProfileEquals body_type = female
                        GotoRandomScript [ FemaleIdle1 FemaleIdle2 ]
                      else
                        GotoRandomScript [ StandIdle OtherIdle BoardIdle ]
                      endif
                    endif
                  endif
                endif
              endif
            endif
          endif
        endif
      endif
    endif
    WaitAnimFinished
  repeat
endscript

script StandIdle
  begin
    PlayAnim Anim = StandIdle Blendperiod = 0.0
    WaitAnimFinished
    PlayAnim random( @Anim = StandIdle @Anim = StandIdle @Anim = StandIdle @Anim = StandIdle @Anim = StandLeftRight @Anim = StandCheckBoard @Anim = StandShoulder ) Blendperiod = 0.0
    if AnimEquals StandSwap
      FlipAfter
      BlendPeriodOut 0.0
    endif
    WaitAnimFinished
  repeat
endscript

script MuskaIdle
  SwitchOnAtomic special_item
  PlayAnim Anim = MuskaIdle Cycle Blendperiod = 0.0 Speed = 0.7
endscript

script FemaleIdle1
  PlayAnim Anim = FemaleIdle Cycle Blendperiod = 0.0
endscript

script FemaleIdle2
  PlayAnim Anim = LeanIdle Cycle Blendperiod = 0.0
endscript

script BoardIdle
  begin
    PlayAnim Anim = random( @BoardIdle @BoardIdle @BoardIdle @BoardIdle @BoardIdle @BoardMove @BoardLook ) Blendperiod = 0.0
    if CurrentSkaterProfileIs 0
      if AnimEquals BoardMove
      endif
    endif
    WaitAnimFinished
  repeat
endscript

script BoardMove_SFX
  begin
    if FrameIS 30
      PlaySound skater_boarddown vol = 30
    endif
    if FrameIS 110
      PlaySound skater_boardup vol = 40
      break
    endif
    Wait 1 Frame
  repeat
endscript

script OtherIdle
  PlayAnim Anim = StartIdle Cycle Blendperiod = 0.0
endscript

script DickIdle
  begin
    if AnimEquals DickBeckon
      PlayAnim Anim = DickIdle Blendperiod = 0.0
    else
      PlayAnim Anim = random( @DickIdle @DickIdle @DickBeckon ) Blendperiod = 0.0
    endif
    WaitAnimFinished
  repeat
endscript

script reset_parkeditor_skater
  printf "reset park editor skater"
  MakeSkaterGoto skater = 0 FreezeSkater
  MakeSkaterGoto skater = 1 FreezeSkater
endscript

script MainMenuCam
  PlaySkaterCamAnim skater = 0 name = "Cam_Default01" play_hold
endscript

script MainMenuToOptionsCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_Options01_Anim" play_hold
endscript

script FrontEndFlash
  DoFlash duration = 1.0 start_r = 255 start_g = 255 start_b = 255 start_a = 255 end_r = 255 end_g = 255 end_b = 255 end_a = 0
endscript

script OptionsToMainMenuCamAnim
  MoveMenu Id = net_network_setup_menu x = 45
  if istrue ReturnedFromLevelRecords
    Change ReturnedFromLevelRecords = 0
  else
    PlaySkaterCamAnim skater = 0 name = "Cam_Options01_Back" play_hold
  endif
endscript

script MainMenuToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01_Anim" play_hold
  EnableSkaterRotation
  SetCurrentSkaterProfile 0
endscript

script Player1ToMainMenuCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu01_BackAnim" play_hold
  ResetSkaterRotation
  DisableSkaterRotation
endscript

script LevelSelectToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect01_BackAnim" play_hold
endscript

script Player1ToLevelSelectCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect01_Anim" play_hold
  BuildCassetteMenu
  killspawnedscript name = WaitThenEnableCassettes
  spawnscript WaitThenEnableCassettes Params = <...>
endscript

script Player1ToPlayer2CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu02_Anim" play_hold
  ResetSkaterRotation
endscript

script Player2ToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_MainMenu02_BackAnim" play_hold
  ResetSkaterRotation
endscript

script Player2ToLevelSelectCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect02_Anim" play_hold
  BuildCassetteMenu
  killspawnedscript name = WaitThenEnableCassettes
  spawnscript WaitThenEnableCassettes
  DisableSkaterRotation
endscript

script LevelSelectToPlayer2CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_LevelSelect02_BackAnim" play_hold
  ResetSkaterRotation
  EnableSkaterRotation
endscript

script Player1ToBoardShopCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect01_Anim" play_hold
endscript

script BoardShopToPlayer1CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect01_BackAnim" play_hold
endscript

script Player2ToBoardShopCamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect02_Anim" play_hold
endscript

script BoardShopToPlayer2CamAnim
  PlaySkaterCamAnim skater = 0 name = "Cam_BoardSelect02_BackAnim" play_hold
endscript

script WaitThenEnableCassettes
  DisableSkaterRotation
  PollUntilFinished func = SkaterCamAnimHeld skater = 0
  if menuisshown cassette_menu
    FireMenuEvent Type = MAKECASSETTESFANOUT Id = cassette_menu target = cassette_menu
  endif
endscript

ShouldShowCareerInfo = 1
AutoLinkToCareerScreen = 0
script SetCareerMode
  SetNetworkMode
  SetGameType Career
  MainMenuToPlayer1CamAnim
  Change ShouldShowCareerInfo = 1
endscript

script SetSingleSessionMode
  SetNetworkMode
  SetGameType SingleSession
  MainMenuToPlayer1CamAnim
  Change ShouldShowCareerInfo = 0
endscript

script SetFreeSkateMode
  SetNetworkMode
  SetGameType FreeSkate
  MainMenuToPlayer1CamAnim
  Change ShouldShowCareerInfo = 0
endscript

script SetFreeSkate2pMode
  SetNetworkMode
  SetGameType FreeSkate2p
  MainMenuToPlayer1CamAnim
  Change ShouldShowCareerInfo = 0
  SetSkaterRotationPad 0
  SyncPlayer2Profile
endscript

script SetNetworkPlayMode
  test_network_settings
  Change ShouldShowCareerInfo = 0
endscript

script Player1ToPlayer2
  Player1ToPlayer2CamAnim
  SetFrontEndPad 1
  SetSkaterRotationPad 1
  SetCurrentSkaterProfile 1
  SliderShow
  reload_cas
endscript

script Player2ToPlayer1
  Player2ToPlayer1CamAnim
  SetFrontEndPad 0
  SetSkaterRotationPad 0
  SetCurrentSkaterProfile 0
  SliderHide
endscript

script Player2ToLevelSelect
  UseBothPadsInFrontEnd
  Player2ToLevelSelectCamAnim
endscript

script Player1ToBoardshop
  Player1ToBoardShopCamAnim
  MakeSkaterGoto MoveToCounter skater = 0
endscript

script Player2ToBoardshop
  Player2ToBoardShopCamAnim
  MakeSkaterGoto MoveToCounter skater = 1
endscript

script Player1ToChangeAppearance
  SetCurrentSkaterProfile 0
endscript

script Player2ToChangeAppearance
  SetCurrentSkaterProfile 1
endscript

script TutorialLaunch
  SetNetworkMode
  SetGameType FreeSkate
  SetCurrentGameType
  SetCurrentSkaterProfile 0
  LevelSelectStartGame level = Load_Tut
endscript

script ParkEditorLaunch
  SetFrontEndPad 0
  SetNetworkMode
  AutoLaunch level = custom_park game = parkeditor
endscript

script player_one_menu_on_show
  SetFrontEndPad 0
  SliderShow
endscript

script player_two_menu_on_show
  update_pro_display_info <...>
  SliderShow
endscript

script frontend_menu_on_show
  if CD
    DestroyElement Id = mainmenu_debug
  endif
  UseBothPadsInFrontEnd
  SetSkaterRotationPad Both
  
  //Add Option needs to be called inbetween InitLevelMod
  //this is because first time it will init ini file and other stuff
  //and second time it will change a bool so you cannot add options anymore
  //this is to make sure options will not get added twice
  //so all options should only be added here, inbetween these two init calls
  InitLevelMod
  AddOptions
  InitLevelMod
endscript

script CareerMenuBack
  Player1ToMainMenuCamAnim
endscript

script free_skate_menu_on_show
  update_pro_display_info <...>
  SliderShow
endscript

script MenuQuitGame
  QuitGame
endscript

script single_session_menu_on_show
  update_pro_display_info <...>
  SliderShow
endscript

script invokeUIScreen
  InvokeScreen <...>
endscript

script get_version_number
  GetVersionNumber <...>
endscript

script main_menu_create
  CreateAndAttachMenu { Type = verticalmenu Id = frontend_menu parent = contain1 x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "frontend_menu_on_show" } children = [ { Type = textmenuelement auto_id text = "Main Menu" static dont_gray drawer = title dont_gray } { Type = textmenuelement auto_id text = "Career" link = career_menu target = "SetCareerMode" } { Type = textmenuelement auto_id text = "Single Session" link = single_session_menu target = "SetSingleSessionMode" } { Type = textmenuelement auto_id text = "Free Skate" link = free_skate_menu target = "SetFreeSkateMode" } { Type = textmenuelement Id = network_play_option text = "Network Play" target = "SetNetworkPlayMode" } { Type = textmenuelement auto_id text = "Create-a-Skater" link = pre_cas_main_menu target = "link_to_cas" } { Type = textmenuelement auto_id text = "Park Editor" target = "ParkEditorLaunch" } { Type = textmenuelement auto_id text = "Tutorials" target = "TutorialLaunch" } { Type = textmenuelement auto_id text = "Options" target = "MainMenuToOptionsCamAnim" link = options_main_menu } { Type = textmenuelement auto_id text = " " x = -45.0 y = 330.0 w = 200 drawer = keyboard_property static dont_gray just_left_x dontdrawrect lock_layout lock_width eventhandler = { Type = showeventhandler target = "get_version_number" } } { Type = textmenuelement auto_id text = motd x = 0.0 y = 300.0 w = 1024 drawer = keyboard_property static dont_gray just_left_x dontdrawrect lock_layout } { Type = textmenuelement auto_id text = "LevelMod 2013-2020" x = 0.0 y = 305.0 w = 580 drawer = keyboard_property static dont_gray dontdrawrect just_right_x just_center_y lock_layout lock_width } { Type = textmenuelement auto_id text = "by DCx Demo*, Vadru'Qc" x = 0.0 y = 330.0 w = 580 drawer = keyboard_property static dont_gray dontdrawrect just_right_x just_center_y lock_layout lock_width } { Type = textmenuelement auto_id text = "Quit" link = menuquitgame_yesno_menu } { Type = textmenuelement Id = mainmenu_debug text = "Debug" link = debug_main_menu } ] }
  CreateAndAttachMenu { Type = verticalmenu Id = menuquitgame_yesno_menu x = 170.0 y = 150.0 w = 300.0 h = 336.0 just_center_x just_center_y blue_top eventhandler = { Type = showeventhandler target = "Select_no" } parent = contain1 children = [ { Type = textmenuelement auto_id text = "Quit?" static dont_gray drawer = title } { Type = textmenuelement Id = quit_yes text = "Yes" target = "MenuQuitGame" } { Type = textmenuelement Id = quit_no text = "No" link = frontend_menu } ] }
  createmenu { Type = verticalmenu Id = player_one_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "Player1ToMainMenuCamAnim" } { Type = showeventhandler target = "player_one_menu_on_show" } { Type = showeventhandler target = "update_pro_display_info" Params = { name_menu_item = pro_current_name_player_one } } { Type = hideeventhandler target = "SliderHide" } ] children = [ { Type = textmenuelement auto_id text = "Player 1" static dont_gray drawer = title dont_gray } { Type = textmenuelement auto_id text = "Continue" link = player_two_menu target = "Player1ToPlayer2" } { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" } { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" } { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container } { Type = textmenuelement auto_id text = "-------" static } { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu } { Type = textmenuelement auto_id text = "Load Skater" target = "_Player1LoadCAS" } ] }
  createmenu { Type = verticalmenu Id = player_two_menu x = 295.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "Player2ToPlayer1" } { Type = showeventhandler target = "player_two_menu_on_show" Params = { name_menu_item = pro_current_name_player_two } } { Type = hideeventhandler target = "SliderHide" } ] children = [ { Type = textmenuelement auto_id text = "Player 2" static dont_gray drawer = title dont_gray } { Type = textmenuelement auto_id text = "Continue" link = cassette_menu target = "Player2ToLevelSelect" } { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player2ToChangeAppearance" } { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player2ToBoardshop" } { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container } { Type = textmenuelement auto_id text = "-------" static } { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu } { Type = textmenuelement auto_id text = "Load Skater" target = "_Player2LoadCAS" } ] }
  createmenu { Type = verticalmenu Id = career_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "CareerMenuBack" } { Type = showeventhandler target = "CareerMenuShow" } { Type = hideeventhandler target = "CareerMenuBackOut" } { Type = showeventhandler target = "update_pro_display_info" Params = { name_menu_item = pro_current_name_career } } ] children = [ { Type = textmenuelement auto_id text = "Career" static dont_gray drawer = title } { Type = textmenuelement auto_id text = "Play Level" link = cassette_menu target = "Player1ToLevelSelectCamAnim" } { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" } { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" } { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container } { Type = textmenuelement auto_id text = "-------" static } { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu } { Type = textmenuelement auto_id text = "Load Skater" target = "_CareerMenuLoadCAS" } { Type = textmenuelement auto_id text = "Done" target = "go_back" Params = { Id = career_menu } } ] }
  if istrue run_viewer
    createmenu { Type = verticalmenu Id = single_session_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "Player1ToMainMenuCamAnim" } { Type = showeventhandler target = "update_pro_display_info" Params = { name_menu_item = pro_current_name_single_session } } ] children = [ { Type = textmenuelement auto_id text = "Session" static dont_gray drawer = title } { Type = textmenuelement auto_id text = "Play Level" link = level_main_menu } { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" } { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" } { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container } { Type = textmenuelement auto_id text = "-------" static } { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu } { Type = textmenuelement auto_id text = "Load Skater" target = "_SingleSessionLoadCAS" } { Type = textmenuelement auto_id text = "Done" target = "go_back" Params = { Id = single_session_menu } } ] }
  else
    createmenu { Type = verticalmenu Id = single_session_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "Player1ToMainMenuCamAnim" } { Type = showeventhandler target = "single_session_menu_on_show" Params = { name_menu_item = pro_current_name_single_session } } { Type = hideeventhandler target = "SliderHide" } ] children = [ { Type = textmenuelement auto_id text = "Session" static dont_gray drawer = title } { Type = textmenuelement auto_id text = "Play Level" link = cassette_menu target = "Player1ToLevelSelectCamAnim" } { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" } { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" } { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container } { Type = textmenuelement auto_id text = "-------" static } { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu } { Type = textmenuelement auto_id text = "Load Skater" target = "_SingleSessionLoadCAS" } { Type = textmenuelement auto_id text = "Done" target = "go_back" Params = { Id = single_session_menu } } ] }
  endif
  if istrue run_viewer
    createmenu { Type = verticalmenu Id = free_skate_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "Player1ToMainMenuCamAnim" } { Type = showeventhandler target = "update_pro_display_info" Params = { name_menu_item = pro_current_name_free_skate } } ] children = [ { Type = textmenuelement auto_id text = "Free Skate" static dont_gray drawer = title } { Type = textmenuelement auto_id text = "Play Level" link = level_main_menu } { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" } { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" } { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container } { Type = textmenuelement auto_id text = "-------" static } { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu } { Type = textmenuelement auto_id text = "Load Skater" target = "_FreeSkateLoadCAS" } { Type = textmenuelement auto_id text = "Done" target = "go_back" Params = { Id = free_skate_menu } } ] }
  else
    createmenu { Type = verticalmenu Id = free_skate_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { Type = backeventhandler target = "Player1ToMainMenuCamAnim" } { Type = showeventhandler target = "free_skate_menu_on_show" Params = { name_menu_item = pro_current_name_free_skate } } { Type = hideeventhandler target = "SliderHide" } ] children = [ { Type = textmenuelement auto_id text = "Free Skate" static dont_gray drawer = title } { Type = textmenuelement auto_id text = "Play Level" link = cassette_menu target = "Player1ToLevelSelectCamAnim" } { Type = textmenuelement auto_id text = "Change Appearance" link = cas_menu_container target = "Player1ToChangeAppearance" } { Type = textmenuelement auto_id text = "Change Board" link = boardshop_menu target = "Player1ToBoardshop" } { Type = textmenuelement auto_id text = "Edit Tricks" link = trick_menu_container } { Type = textmenuelement auto_id text = "-------" static } { Type = textmenuelement auto_id text = "Choose Skater" link = pro_menu } { Type = textmenuelement auto_id text = "Load Skater" target = "_FreeSkateLoadCAS" } { Type = textmenuelement auto_id text = "Done" target = "go_back" Params = { Id = free_skate_menu } } ] }
  endif
  createmenu { Type = verticalmenu Id = load_career_menu x = 45.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "Load Skater" static dont_gray drawer = title } { Type = textmenuelement auto_id text = "Tony Hawk 1" } { Type = textmenuelement auto_id text = "Tony Hawk 2" } { Type = textmenuelement auto_id text = "Rune Glifberg" } { Type = textmenuelement auto_id text = "Created 1" } { Type = textmenuelement auto_id text = "Created 2" } ] }
  createmenu { Type = verticalmenu Id = career_info x = 45.0 y = 290.0 w = 300.0 h = 400.0 just_center_x just_center_y static dont_gray children = [ { Type = textmenuelement Id = career_info_line_1 text = "Poo poo pants" static dont_gray drawer = goals_text drawer = title_medium truncate_to_width } { Type = textmenuelement Id = career_info_line_2 text = "Goals: 12 of 35" static dont_gray drawer = goals_text } { Type = horizontalmenu Id = career_info_horizontal_menu static dont_gray children = [ { Type = textmenuelement Id = career_info_line_3 text = "Medals:" static dont_gray drawer = goals_text lock_layout x = 40 y = 8 } { Type = iconmenuelement Id = career_info_rio_medal_1 image = "panelsprites\medal_1.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_Gold dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_rio_medal_2 image = "panelsprites\medal_2.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_Silver dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_rio_medal_3 image = "panelsprites\medal_3.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_Bronze dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_rio_medal_4 image = "panelsprites\medal_locked.png" rectScaleX = 1.3 lock_layout x = 120 y = 2 IconColor_NoMedal dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_si_medal_1 image = "panelsprites\medal_1.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_Gold dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_si_medal_2 image = "panelsprites\medal_2.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_Silver dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_si_medal_3 image = "panelsprites\medal_3.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_Bronze dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_si_medal_4 image = "panelsprites\medal_locked.png" rectScaleX = 1.3 lock_layout x = 180 y = 2 IconColor_NoMedal dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_tokyo_medal_1 image = "panelsprites\medal_1.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_Gold dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_tokyo_medal_2 image = "panelsprites\medal_2.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_Silver dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_tokyo_medal_3 image = "panelsprites\medal_3.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_Bronze dontdrawrect static dont_gray } { Type = iconmenuelement Id = career_info_tokyo_medal_4 image = "panelsprites\medal_locked.png" rectScaleX = 1.3 lock_layout x = 240 y = 2 IconColor_NoMedal dontdrawrect static dont_gray } ] } ] }
  createmenu { Type = verticalmenu Id = slider_info x = 45.0 y = 290.0 w = 300.0 h = 400.0 just_center_x just_center_y children = [ { Type = textmenuelement Id = slider_info_line_1 text = "Poo poo pants" static dont_gray drawer = goals_text drawer = title_medium truncate_to_width } { Type = textmenuelement Id = slider_info_line_2 text = "Trickstyle: Vert" static dont_gray drawer = goals_text } { Type = textmenuelement Id = slider_info_line_3 text = "Stance: Goofy" static dont_gray drawer = goals_text } ] }
  CreateBoardShopMenu
  CreateDeckMenu
  CreateWheelColorMenu
  pre_cas_menu_create
  attachchild parent = contain1 child = career_menu
  attachchild parent = contain1 child = single_session_menu
  attachchild parent = contain1 child = free_skate_menu
  attachchild parent = contain1 child = load_career_menu
  attachchild parent = contain1 child = player_one_menu
  attachchild parent = contain1 child = player_two_menu
  attachchild parent = contain1 child = boardshop_menu
  attachchild parent = contain1 child = pre_cas_main_menu
  attachchild parent = contain1 child = deck_menu
  attachchild parent = contain1 child = career_info
  attachchild parent = contain1 child = slider_info
  attachchild parent = contain1 child = wheel_color_menu
  helper_menu_create
  pro_menu_create
  cassette_menu_create
endscript

script LevelSelectBackOut
  ClearCassetteMenu
  if CurrentSkaterProfileIs 1
    LevelSelectToPlayer2CamAnim
    SetFrontEndPad 1
  else
    LevelSelectToPlayer1CamAnim
  endif
  ResetSkaterRotation
  EnableSkaterRotation
  InitTVScreensVisibility
endscript

script InitTVScreensVisibility
  invisible prefix = "ss_go_TV_"
  visible name = ss_go_TV_THPScreen
endscript

script LevelSelectStartGame
  if CurrentSkaterProfileIs 1
    SetGameType trickattack
    UseBothPadsInFrontEnd
  else
  endif
  SetCurrentSkaterProfile 0
  SetSkaterRotationPad 0
  immediately_start_game <...>
endscript

ParkEditorCassetteTurnRate = -0.05
script SwitchOffCassetteMenuHeading
  FireMenuEvent Type = HIDE Id = cassette_menu_level_name target = cassette_menu_level_name
  FireMenuEvent Type = HIDE Id = cassette_menu_dummy_line target = cassette_menu_dummy_line
endscript

script SwitchOnCassetteMenuHeading
  FireMenuEvent Type = SHOW Id = cassette_menu_level_name target = cassette_menu_level_name
  FireMenuEvent Type = SHOW Id = cassette_menu_dummy_line target = cassette_menu_dummy_line
endscript

script PlayCustomParkFromCassetteMenu
  ClearCassetteMenu
  immediately_start_game <...>
endscript

script UpdateParkNameInMenuTitles
  GetParkName
  SetMenuElementText Id = cassette_menu_park_editor_menu_title <ParkName>
  SetMenuElementText Id = cassette_menu_dummy_line <ParkName>
endscript

script cassette_menu_create
  createmenu { Type = cassettemenu Id = cassette_menu eventhandlers = [ { Type = backeventhandler target = "LevelSelectBackOut" } { Type = showeventhandler target = "UpdateParkNameInMenuTitles" } ] AppearanceParams = CassetteMenuParams }
  createmenu { Type = verticalmenu Id = cassette_menu_park_editor_menu x = 255.0 y = 50.0 w = 360.0 h = 230.0 just_center_x just_center_y blue_top eventhandler = { Type = backeventhandler target = "SwitchOnCassetteMenuHeading" } children = [ { Type = textmenuelement Id = cassette_menu_park_editor_menu_title text = " " static dont_gray drawer = title truncate_to_width } { Type = textmenuelement auto_id text = "Play Park" target = "PlayCustomParkFromCassetteMenu" Params = { level = custom_park string = "Play custom park" } } { Type = textmenuelement auto_id text = "Load Custom Park" target = "_CassetteMenuParkEditorLoad" } { Type = textmenuelement auto_id text = "Pre-made parks" link = cassette_menu_parked_load_menu } { Type = textmenuelement auto_id text = "Back" target = "go_back" Params = { Id = cassette_menu_park_editor_menu } } ] }
  attachchild parent = contain1 child = cassette_menu
  attachchild parent = contain1 child = cassette_menu_park_editor_menu
endscript

script _LoadParkFromDisc
  SwitchOffMenu Id = cassette_menu_parked_load_menu
  spawnscript LoadParkFromDisc Params = <...>
endscript

script LoadParkFromDisc
  Message_LoadingParkFromDisc
  if ParkEditorCommand command = accessDisk file = <file> DoNotCleanupAndInitialize
    Message_LoadSuccessful
  else
    Message_LoadParkFromDiscFailed
  endif
  UpdateParkNameInMenuTitles
  Wait 0.5 seconds
  SwitchToMenu DoNotMakeRoot menu = cassette_menu_park_editor_menu
endscript

script ClearCassetteMenu
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
endscript

UnlockRequirements =
[ { level = LevelNum_Canada 3 Goals } { level = LevelNum_Rio 10 Goals } { level = LevelNum_Suburbia 1 Medal } { level = LevelNum_Airport 18 Goals } { level = LevelNum_SkaterIsland 26 Goals } { level = LevelNum_LA 2 Medals } { level = LevelNum_Tokyo 35 Goals } { level = LevelNum_Ship 3 Medals } ]
script BuildCassetteMenu
  createmenu { Type = verticalmenu Id = cassette_menu_heading x = 280.0 y = 50.0 w = 300.0 h = 230.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement Id = cassette_menu_level_name text = "Custom Park" static dont_gray drawer = title } { Type = textmenuelement Id = cassette_menu_dummy_line text = " " static dont_gray drawer = goals_text } ] }
  createmenu { Type = textmenuelement Id = cassette_menu_stat_points_text text = "Stat points:" just_center_x lock_layout static dont_gray drawer = goals_text x = 280 y = 80 w = 150 }
  createmenu { Type = statsmenuelement Id = cassette_menu_stat_points_pips text = "---" implemented_property just_center_x x = 430 y = 80 w = 70 h = 23 ghosted dont_gray pip_spacing = cas_pip_spacing lower = 0 upper = 5 start_value = 0 }
  createmenu { Type = textmenuelement Id = cassette_menu_got_deck_text text = "Deck: Yes" just_center_x lock_layout x = 500 y = 80 w = 80 h = 23 static dont_gray drawer = goals_text }
  createmenu { Type = textmenuelement Id = cassette_menu_line_1 text = "Goals completed:" static dont_gray drawer = goals_text just_center_x x = 280 y = 103 w = 150 }
  createmenu { Type = textmenuelement Id = cassette_menu_line_2 text = " " static dont_gray drawer = goals_text just_center_x x = 430 y = 103 w = 150 }
  createmenu { Type = textmenuelement Id = cassette_menu_best_medal_text text = "Best medal:" static dont_gray drawer = goals_text lock_layout just_center_x x = 320 y = 115 w = 150 }
  createmenu { Type = iconmenuelement Id = cassette_menu_best_medal_gold image = "panelsprites\medal_1.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_Gold dontdrawrect }
  createmenu { Type = iconmenuelement Id = cassette_menu_best_medal_silver image = "panelsprites\medal_2.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_Silver dontdrawrect }
  createmenu { Type = iconmenuelement Id = cassette_menu_best_medal_bronze image = "panelsprites\medal_3.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_Bronze dontdrawrect }
  createmenu { Type = iconmenuelement Id = cassette_menu_best_medal_none image = "panelsprites\medal_locked.png" rectScaleX = 1.3 x = 490 y = 110 IconColor_NoMedal dontdrawrect }
  createmenu { Type = verticalmenu Id = cassette_menu_goals_left x = 280.0 y = 170.0 w = 296.0 h = 250.0 just_center_x just_center_y children = [ { Type = textmenuelement Id = cassette_goals_line_1 text = "Level is locked !!!" static dont_gray drawer = goals_text } { Type = textmenuelement Id = cassette_goals_line_2 text = " " static dont_gray drawer = goals_text } ] }
  createmenu { x = 280 y = 80 w = 300 just_center_x just_center_y drawer = goals_text Type = textmenuelement Id = cassette_menu_records_1 text = "Records:" static dont_gray }
  createmenu { x = 280 y = 102 w = 300 just_center_x just_center_y drawer = goals_text Type = textmenuelement Id = cassette_menu_records_2 text = " " static dont_gray }
  createmenu { Type = verticalmenu Id = cassette_menu_parked_load_menu x = 255.0 y = 50.0 w = 360.0 h = 230.0 just_center_x just_center_y blue_top children = [ { Type = textmenuelement auto_id text = "Pre-Made Parks" static dont_gray drawer = title } { Type = scrollingmenu auto_id x = 0.0 y = 0.0 w = 350.0 h = 400.0 just_center_x just_center_y num_visible = 9 children = [ { Type = textmenuelement Id = parked_load1 text = "Park 1" target = "_LoadParkFromDisc" Params = { file = 1 } } { Type = textmenuelement Id = parked_load2 text = "Park 2" target = "_LoadParkFromDisc" Params = { file = 2 } } { Type = textmenuelement Id = parked_load3 text = "Park 3" target = "_LoadParkFromDisc" Params = { file = 3 } } { Type = textmenuelement Id = parked_load4 text = "Park 4" target = "_LoadParkFromDisc" Params = { file = 4 } } { Type = textmenuelement Id = parked_load5 text = "Park 5" target = "_LoadParkFromDisc" Params = { file = 5 } } { Type = textmenuelement Id = parked_load6 text = "Park 6" target = "_LoadParkFromDisc" Params = { file = 6 } } { Type = textmenuelement Id = parked_load7 text = "Park 7" target = "_LoadParkFromDisc" Params = { file = 7 } } { Type = textmenuelement Id = parked_load8 text = "Park 8" target = "_LoadParkFromDisc" Params = { file = 8 } } { Type = textmenuelement Id = parked_load9 text = "Park 9" target = "_LoadParkFromDisc" Params = { file = 9 } } { Type = textmenuelement Id = parked_load10 text = "Park 10" target = "_LoadParkFromDisc" Params = { file = 10 } } { Type = textmenuelement Id = parked_load11 text = "Park 11" target = "_LoadParkFromDisc" Params = { file = 11 } } { Type = textmenuelement Id = parked_load12 text = "Park 12" target = "_LoadParkFromDisc" Params = { file = 12 } } { Type = textmenuelement Id = parked_load13 text = "Park 13" target = "_LoadParkFromDisc" Params = { file = 13 } } { Type = textmenuelement Id = parked_load14 text = "Park 14" target = "_LoadParkFromDisc" Params = { file = 14 } } { Type = textmenuelement Id = parked_load15 text = "Park 15" target = "_LoadParkFromDisc" Params = { file = 15 } } { Type = textmenuelement Id = parked_load16 text = "Park 16" target = "_LoadParkFromDisc" Params = { file = 16 } } { Type = textmenuelement Id = parked_load17 text = "Park 17" target = "_LoadParkFromDisc" Params = { file = 17 } } { Type = textmenuelement Id = parked_load18 text = "Park 18" target = "_LoadParkFromDisc" Params = { file = 18 } } { Type = textmenuelement Id = parked_load19 text = "Park 19" target = "_LoadParkFromDisc" Params = { file = 19 } } { Type = textmenuelement Id = parked_load20 text = "Park 20" target = "_LoadParkFromDisc" Params = { file = 20 } } ] } ] }
  attachchild parent = contain1 child = cassette_menu_heading
  attachchild parent = contain1 child = cassette_menu_goals_left
  attachchild parent = contain1 child = cassette_menu_records_1
  attachchild parent = contain1 child = cassette_menu_records_2
  attachchild parent = contain1 child = cassette_menu_stat_points_text
  attachchild parent = contain1 child = cassette_menu_stat_points_pips
  attachchild parent = contain1 child = cassette_menu_got_deck_text
  attachchild parent = contain1 child = cassette_menu_line_1
  attachchild parent = contain1 child = cassette_menu_line_2
  attachchild parent = contain1 child = cassette_menu_best_medal_text
  attachchild parent = contain1 child = cassette_menu_best_medal_gold
  attachchild parent = contain1 child = cassette_menu_best_medal_silver
  attachchild parent = contain1 child = cassette_menu_best_medal_bronze
  attachchild parent = contain1 child = cassette_menu_best_medal_none
  attachchild parent = contain1 child = cassette_menu_parked_load_menu
  spawnscript PositionStatPips
  SetGlobalFlag flag = LEVEL_UNLOCKED_FOUNDRY
  if TestRequestedGameType Career
  else
    createmenu { Type = cassettemenuelement Id = cassette_editor ModelName = Editor TVImageName = ss_go_TV_editor LockedTVImageName = ss_go_TV_editor LevelName = '' LevelNumber = 0 parkeditor eventhandler = { Type = ChooseEventHandler target = "SwitchOffCassetteMenuHeading" link = cassette_menu_park_editor_menu } }
    attachchild child = cassette_editor parent = cassette_menu
  endif
  createmenu { Type = cassettemenuelement Id = cassette_foundry ModelName = Foundry TVImageName = ss_go_TV_foundry LockedTVImageName = ss_go_TV_foundry_l LevelName = 'Foundry' LevelNumber = LevelNum_Foundry GoalParams = FoundryGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Foun } } }
  attachchild child = cassette_foundry parent = cassette_menu
  createmenu { Type = cassettemenuelement Id = cassette_canada ModelName = Canada TVImageName = ss_go_TV_Can LockedTVImageName = ss_go_TV_Can_l LevelName = 'Canada' LevelNumber = LevelNum_Canada GlobalFlag = LEVEL_UNLOCKED_CANADA GoalParams = CanadaGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Can } } }
  attachchild child = cassette_canada parent = cassette_menu
  createmenu { Type = cassettemenuelement Id = cassette_rio ModelName = Rio TVImageName = ss_go_TV_Rio LockedTVImageName = ss_go_TV_Rio_l LevelName = 'Rio' LevelNumber = LevelNum_Rio GlobalFlag = LEVEL_UNLOCKED_RIO GoalParams = DefaultGoalParams CompetitionLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Rio } } }
  attachchild child = cassette_rio parent = cassette_menu
  createmenu { Type = cassettemenuelement Id = cassette_suburbia ModelName = Suburbia TVImageName = ss_go_TV_Sub LockedTVImageName = ss_go_TV_Sub_l LevelName = 'Suburbia' LevelNumber = LevelNum_Suburbia GlobalFlag = LEVEL_UNLOCKED_SUBURBIA GoalParams = SuburbiaGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Sub } } }
  attachchild child = cassette_suburbia parent = cassette_menu
  createmenu { Type = cassettemenuelement Id = cassette_airport ModelName = Airport TVImageName = ss_go_TV_ap LockedTVImageName = ss_go_TV_ap_l LevelName = 'Airport' LevelNumber = LevelNum_Airport GlobalFlag = LEVEL_UNLOCKED_AIRPORT GoalParams = AirportGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Ap } } }
  attachchild child = cassette_airport parent = cassette_menu
  createmenu { Type = cassettemenuelement Id = cassette_skater_island ModelName = Skater_Island TVImageName = ss_go_TV_si LockedTVImageName = ss_go_TV_si_l LevelName = 'Skater Island' LevelNumber = LevelNum_SkaterIsland GlobalFlag = LEVEL_UNLOCKED_SKATERISLAND GoalParams = DefaultGoalParams CompetitionLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Si } } }
  attachchild child = cassette_skater_island parent = cassette_menu
  createmenu { Type = cassettemenuelement Id = cassette_los_angeles ModelName = Los_Angeles TVImageName = ss_go_TV_la LockedTVImageName = ss_go_TV_la_l LevelName = 'Los Angeles' LevelNumber = LevelNum_LA GlobalFlag = LEVEL_UNLOCKED_LOSANGELES GoalParams = LosAngelesGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_La } } }
  attachchild child = cassette_los_angeles parent = cassette_menu
  createmenu { Type = cassettemenuelement Id = cassette_tokyo ModelName = Tokyo TVImageName = ss_go_TV_tok LockedTVImageName = ss_go_TV_tok_l LevelName = 'Tokyo' LevelNumber = LevelNum_Tokyo GlobalFlag = LEVEL_UNLOCKED_TOKYO GoalParams = DefaultGoalParams CompetitionLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Tok } } }
  attachchild child = cassette_tokyo parent = cassette_menu
  if istrue AllLevelsUnlocked
    CreateShipCassette
  else
    if GetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
      CreateShipCassette
    endif
  endif
  if TestRequestedGameType Career
  else
    if istrue AllLevelsUnlocked
      CreateWarehouseCassette
      CreateBurnsideCassette
      CreateRoswellCassette
    else
      if GetGlobalFlag flag = LEVEL_UNLOCKED_WAREHOUSE
        CreateWarehouseCassette
      endif
      if GetGlobalFlag flag = LEVEL_UNLOCKED_BURNSIDE
        CreateBurnsideCassette
      endif
      if GetGlobalFlag flag = LEVEL_UNLOCKED_ROSWELL
        CreateRoswellCassette
      endif
    endif
  endif
  if istrue XBox
    if TestRequestedGameType Career
    else
      createmenu { Type = cassettemenuelement Id = cassette_oil ModelName = Ship TVImageName = ss_go_TV_THPScreen LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Oil Rig' GoalParams = DefaultGoalParams SpecialLevel LevelNumber = 0 eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Oil } } }
      attachchild child = cassette_oil parent = cassette_menu
    endif
  endif
  if istrue GameCube
    if TestRequestedGameType Career
    else
      createmenu { Type = cassettemenuelement Id = cassette_rooftops ModelName = Ship TVImageName = ss_go_TV_THPScreen LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Rooftops' GoalParams = DefaultGoalParams SpecialLevel LevelNumber = 0 eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Roof } } }
      attachchild child = cassette_rooftops parent = cassette_menu
    endif
  endif
endscript

script CreateShipCassette
  createmenu { Type = cassettemenuelement Id = cassette_ship ModelName = Ship TVImageName = ss_go_TV_ship LockedTVImageName = ss_go_TV_ship_l LevelName = 'Cruise Ship' LevelNumber = LevelNum_Ship GlobalFlag = LEVEL_UNLOCKED_SHIP GoalParams = DefaultGoalParams eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Shp } } }
  attachchild child = cassette_ship parent = cassette_menu
endscript

script CreateWarehouseCassette
  createmenu { Type = cassettemenuelement Id = cassette_warehouse ModelName = Special1_WH TVImageName = ss_go_TV_sp1 LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Warehouse' LevelNumber = LevelNum_Warehouse GlobalFlag = LEVEL_UNLOCKED_WAREHOUSE GoalParams = DefaultGoalParams SpecialLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Ware } } }
  attachchild child = cassette_warehouse parent = cassette_menu
endscript

script CreateBurnsideCassette
  createmenu { Type = cassettemenuelement Id = cassette_burnside ModelName = Special2_BS TVImageName = ss_go_TV_sp2 LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Burnside' LevelNumber = LevelNum_Burnside GlobalFlag = LEVEL_UNLOCKED_BURNSIDE GoalParams = DefaultGoalParams SpecialLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Burn } } }
  attachchild child = cassette_burnside parent = cassette_menu
endscript

script CreateRoswellCassette
  createmenu { Type = cassettemenuelement Id = cassette_roswell ModelName = Special3_ros TVImageName = ss_go_TV_sp3 LockedTVImageName = ss_go_TV_THPScreen LevelName = 'Roswell' LevelNumber = LevelNum_Roswell GlobalFlag = LEVEL_UNLOCKED_ROSWELL GoalParams = DefaultGoalParams SpecialLevel eventhandler = { Type = ChooseEventHandler target = "LevelSelectStartGame" kill_menu use_loading_screen Params = { level = Load_Ros } } }
  attachchild child = cassette_roswell parent = cassette_menu
endscript

script PositionStatPips
  MoveMenu Id = cassette_menu_stat_points_pips x = 430 y = 80 w = 70 h = 23
endscript

CassetteMenuParams =
{ SourceNodeName = TRG_Videxplode NumFramesToFanOut = 20 FanOutPause = 1 NumFramesToFanIn = 20 FanInPause = 1 NumVisible = 6 XSeparation = 90 NumFramesToMoveSideways = 10 YCoord = 330 UnselectedZ = 40 SelectedZ = 20 NumFramesToFlip = 50 BigX = 320 BigY = 240 BigZ = 5 NumFramesToGetBig = 20 }
GoalInfoTopY = 60
GoalInfoDY = 35
script CareerMenuShow
  if menuisshown pro_menu
  else
    MakeMenuPopOffScreen Id = career_info Left
  endif
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
  spawnscript MaybeWhizz
endscript

script MaybeWhizz
  if ControllerPressed Left
    MoveMenu Id = career_info x = 45
  else
    if ControllerPressed Right
      MoveMenu Id = career_info x = 45
    else
      MakeMenuZoomOn Id = career_info FromLeft
    endif
  endif
endscript

script CareerMenuBackOut
  FireMenuEvent Type = HIDE Id = career_info target = career_info
endscript

script SetDefaultSliderPos
  if CurrentSkaterProfileIs 1
    MoveMenu Id = slider_info x = 295
  else
    MoveMenu Id = slider_info x = 45
  endif
endscript

script SliderShow
  SetDefaultSliderPos
  if menuisshown pro_menu
  else
    if CurrentSkaterProfileIs 1
      MakeMenuPopOffScreen Id = slider_info Right
    else
      MakeMenuPopOffScreen Id = slider_info Left
    endif
  endif
  FireMenuEvent Type = SHOW Id = slider_info target = slider_info
  UpdateSliderInfoMenu
  spawnscript SliderMaybeWhizz
endscript

script SliderMaybeWhizz
  if ControllerPressed Left
    SetDefaultSliderPos
  else
    if ControllerPressed Right
      SetDefaultSliderPos
    else
      if CurrentSkaterProfileIs 1
        MakeMenuPopOffScreen Id = slider_info Right
        MakeMenuZoomOn Id = slider_info FromRight
      else
        MakeMenuZoomOn Id = slider_info FromLeft
      endif
    endif
  endif
endscript

script SliderHide
  FireMenuEvent Type = HIDE Id = slider_info target = slider_info
endscript

TotalGoals = 54
IconColor_Gold = { r = 234 g = 190 b = 92 a = 115 }
IconColor_Silver = { r = 167 g = 225 b = 236 a = 115 }
IconColor_Bronze = { r = 118 g = 72 b = 21 a = 115 }
IconColor_NoMedal = { r = 67 g = 125 b = 176 a = 115 }
LeftArrowIconElement = { Type = iconmenuelement image = "panelsprites\arrow_menu.png" rotate = -90 x = 350.0 y = 180.0 lock_layout dontdrawrect dont_gray r = 255 g = 255 b = 255 a = 115 }
RightArrowIconElement = { Type = iconmenuelement image = "panelsprites\arrow_menu.png" rotate = 90 x = 530.0 y = 180.0 lock_layout dontdrawrect dont_gray r = 255 g = 255 b = 255 a = 115 }
script attach_back_button
  CreateAndAttachMenu { Type = textmenuelement auto_id parent = <menu_id> text = "Back" target = "go_back" Params = { Id = <menu_id> } }
endscript

