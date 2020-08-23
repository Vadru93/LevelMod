script WaitFrameLoop
  begin
    Wait 1 gameframe
  repeat
endscript
script InitializeGameFlow
  if OnServer
    printf "*** START SERVER HERE ***"
    SetServerMode
    StartServer
  endif
  if InFrontEnd
  else
    LaunchMenuScreen screen = game_menu not_active
  endif
  if OnServer
    printf "*** JOIN SERVER HERE ***"
    JoinServer <...>
    CreatePanels
  endif
  printf "***************************    LOADING LEVEL ****************************************************"
  LoadRequestedLevel
  if InFrontEnd
    InitializeFrontEndSprites
    InitTVScreensVisibility
    LoadSkaters
    SpawnScript helper_scanner
  endif
  StandardGameFlow
endscript
script ChangeLevelGameFlow
  change FirstTimeInSplitScreen = 1
  LoadRequestedLevel
  ReinsertSkaters
  if InNetGame
    SetGameType netlobby
    SetCurrentGameType
  endif
  begin
    if InFrontEnd
      if SkatersAreReady
        break
      endif
      Wait 1 gameframe
    else
      Wait 1 gameframe
      if SkatersAreReady
        break
      endif
    endif
  repeat
  WaitFrameLoop
endscript
script RestartGameFlow
  StandardGameFlow
endscript
script pause_game_flow
  printf "Pausing game flow"
  PauseGameFlow
  Wait 1 gameframe
endscript
script unpause_game_flow
  printf "Unpausing game flow"
  UnpauseGameFlow
endscript
FirstTimeInGameFlow_Startup = 1
script GameFlow_Startup
  printf "Running standard game flow"
  if GameModeEquals is_career
    SetGlobalFlag flag = SKATESHOP_RETURN_FROM_CAREER
  else
    if GameModeEquals is_singlesession
      SetGlobalFlag flag = SKATESHOP_RETURN_FROM_SINGLE
    else
      if GameModeEquals is_parkeditor
        SetGlobalFlag flag = SKATESHOP_RETURN_FROM_PARKEDIT
      else
        if InNetGame
          SetGlobalFlag flag = SKATESHOP_RETURN_FROM_ONLINE
        else
          SetGlobalFlag flag = SKATESHOP_RETURN_FROM_FREESKATE
        endif
      endif
    endif
  endif
  DisablePause
  UnSetWaitForGoalFlags
  SetScreenMode one_camera
  begin
    if InFrontEnd
      if SkatersAreReady
        break
      endif
      Wait 1 gameframe
    else
      Wait 1 gameframe
      if SkatersAreReady
        break
      endif
    endif
  repeat
  HideClock
  if GameModeEquals is_parkeditor
    printf "freezing skater"
    Ed_RemoveSkater
  endif
  RestartLevel
  if InFrontEnd
    WaitFrameLoop
  endif
  if GameModeEquals is_parkeditor
  else
    RefreshGameMenu
  endif
  InitializeSkaters
  KillMessages
  TogglePanel 0
  if CareerLevelIs LevelNum_Tutorials
    PauseMusic 1
  else
  endif
  PauseStream 0
  HideLoadingScreen
  if InNetGame
    if GameModeEquals is_lobby
      if OnServer
        server_enter_free_skate
      else
        client_enter_free_skate
      endif
    endif
  endif
  if IsTrue FirstTimeFromSkateshop
    change FirstTimeFromSkateshop = 0
  else
  endif
  if InSplitScreenGame
    ResetSkaters
    if IsTrue FirstTimeInSplitScreen
      MakeSkaterGosub remove_skater_from_world skater = 0
      MakeSkaterGosub remove_skater_from_world skater = 1
      SwitchToMenu menu = splitscreen_setup_menu
      pause_game_flow
      change FirstTimeInSplitScreen = 0
    endif
    ApplySplitScreenOptions
    MakeSkaterGosub add_skater_to_world skater = 0
    MakeSkaterGosub add_skater_to_world skater = 1
  endif
  SetScreenModeFromGameMode
  if ShouldRunIntroScript
    DisablePause
    SpawnLevelScript target = once_on_startup callback = unpause_game_flow
  endif
  begin
    if IntroScriptFinished
      break
    endif
    Wait 1 gameframe
  repeat
  if IsCompetition
    if CareerLevelIs LevelNum_Rio
      StartCompetition RioCompParams RioScoreParams
    endif
    if CareerLevelIs LevelNum_SkaterIsland
      StartCompetition SICompParams SIScoreParams
    endif
    if CareerLevelIs LevelNum_Tokyo
      StartCompetition TokCompParams TokScoreParams
    endif
  endif
  if GameModeEquals is_horse
    StartHorse
  endif
endscript
script GameFlow_StartRun
  if IsCompetition
    StartCompetitionRun
    PlaySkaterCamAnim skater = 0 stop
    KillMessages
    DisablePause
    if CareerLevelIs LevelNum_Rio
      printf "going to spawn comp_wait_runstart"
      Comp_Wait_RunStart_Setup RioCompParams
    endif
    if CareerLevelIs LevelNum_SkaterIsland
      Comp_Wait_RunStart_Setup SICompParams
    endif
    if CareerLevelIs LevelNum_Tokyo
      Comp_Wait_RunStart_Setup TokCompParams
    endif
    pause_game_flow
  else
    PlaySkaterCamAnim skater = 0 stop
  endif
  TogglePanel 1
  DisablePause
  if ReplayModeEquals REPLAY_MODE_PLAY_SAVED
    SwitchToMenu menu = replay_from_memcard_menu DoNotOpen
    PauseStream 0
  else
    if ReplayModeEquals REPLAY_MODE_PLAY_AFTER_RUN
      SwitchToMenu menu = replay_menu DoNotOpen
      PauseStream 0
    endif
  endif
  enable_replays
  if InMultiPlayerGame
  else
  endif
  ReplayInit
  if TestGameType parkeditor
  else
    ResetLevel
  endif
  if IsCareerMode
    KillTrickPoints
    SetUpSkaterDefaults
    CreateTrickPoints2
    CreateSkateLines2
    CreateDeckIcon
    CreatePhotoGuy
    UnSetGlobalFlag flag = PROMPT_FOR_SAVE
  endif
  if CareerLevelIs LevelNum_Tutorials
    PauseMusic 1
  else
    printf "starting a run....skip tracks and crank up the music"
    if GameModeEquals is_horse
    else
      SkipMusicTrack
    endif
    PauseMusic 0
  endif
  if GameModeEquals default_time_limit
    ResetClock
    UnpauseClock
    ShowClock
  else
    HideClock
  endif
  SetTeamMode 0
  if GameModeEquals is_king
    SpawnCrown
    SpawnCompass
  endif
  if GameModeEquals is_horse
    horse_start_run
  endif
  if GameModeChecksumEquals ctf
    SpawnCTF
    SetTeamMode 1
    SpawnCompass
  endif
  if GameModeChecksumEquals ownthezone
    SpawnZones
    SetTeamMode 1
    SpawnCompass
  endif
  if GameModeChecksumEquals beachball
    SpawnBeachball
    SpawnCompass
  endif
  SetTeamMode 2
  ResetSkaters
  if InMultiPlayerGame
    SpawnScript Splitscreen_StartupScript
  endif
  if InNetGame
    if OnServer
    else
      LaunchQueuedScripts
      if IsObserving
        ShowAllObjects
      endif
    endif
  endif
  if InNetGame
    LoadMultiplayerSounds
    if GameModeEquals is_lobby
      if IsTrue FirstTimeInGameFlow_Startup
        change FirstTimeInGameFlow_Startup = 0
        InvokeScreen screen = ss_main_window
      else
        DoDeferredStart
      endif
    endif
  endif
  IF GameModeChecksumEquals netgraffiti
     GrafStarted
  ENDIF
  LeaveObserveMode2
  printf "going through option list"
  ForEachIn LevelModOptions do = OptionsOnStartGame params = <...>
endscript
script GameFlow_PlayRun
  Wait 10 gameframe
  if InMultiPlayerGame
    UseBothPadsInFrontEnd
  else
    UseOnePadInFrontEnd
  endif
  EnableActuators
  EnablePause
  begin
    if ShouldEndRun
      break
    endif
    if IsCompetition
      if CompetitionEnded
        break
      endif
    endif
    if GameModeEquals is_horse
      if FirstTrickStarted
        HideClock
        break
      endif
    endif
    Wait 1 gameframe
  repeat
endscript
script GameFlow_WaitEnd
  begin
    if EndRunSelected
      break
    endif
    if IsCompetition
      if CompetitionEnded
        break
      endif
    endif
    if AllSkatersAreIdle
      break
    endif
    Wait 1 gameframe
  repeat
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
endscript
script GameFlow_End
  if IsCompetition
    placing_screen
  endif
  begin
    if CalculateFinalScores
      break
    endif
    Wait 1 gameframe
  repeat
  SpawnLevelScript target = once_on_exit
  begin
    if IntroScriptFinished
      break
    endif
    Wait 1 gameframe
  repeat
  if IsCareerMode
    Goal_CheckProVideoUnlock
    if GetGlobalFlag flag = SHOW_CREDITS
      UnSetGlobalFlag flag = SHOW_CREDITS
      if CD
        Wait 1 gameframe
        ingame_play_movie "movies\credits"
      endif
    endif
    if IsCompetition
    else
      UpdateRecords
      Statistics_screen
    endif
  else
    if IsCustomPark
    else
      if GameModeEquals is_singlesession
        UpdateRecords
        Statistics_screen
      endif
    endif
  endif
  change EnteringStatsMenuFromGameFlow = 1
  if JustGotFlag flag = GOAL_STAT_POINT1
    printf "stat point"
    SwitchToMenu menu = stats_menu
    pause_game_flow
  else
    if JustGotFlag flag = GOAL_STAT_POINT2
      printf "stat point"
      SwitchToMenu menu = stats_menu
      pause_game_flow
    else
      if JustGotFlag flag = GOAL_STAT_POINT3
        printf "stat point"
        SwitchToMenu menu = stats_menu
        pause_game_flow
      else
        if JustGotFlag flag = GOAL_STAT_POINT4
          printf "stat point"
          SwitchToMenu menu = stats_menu
          pause_game_flow
        else
          if JustGotFlag flag = GOAL_STAT_POINT5
            printf "stat point"
            SwitchToMenu menu = stats_menu
            pause_game_flow
          endif
        endif
      endif
    endif
  endif
  change EnteringStatsMenuFromGameFlow = 0
  helper_select_choose_back_centered
  if InNetGame
    SwitchToMenu menu = end_run_menu DontPauseWhenActive
  else
    if IsCareerMode
      if GetGlobalFlag flag = PROMPT_FOR_SAVE
        SwitchToMenu menu = savegame_yesno_menu
      else
        SwitchToMenu menu = end_run_menu
      endif
    else
      SwitchToMenu menu = end_run_menu
    endif
  endif
  if GameModeEquals show_ranking_screen
    helper_hide
    KillMessages all_panels
    SwitchToMenu menu = winner_screen
  endif
  if InNetGame
    if OnServer
      Wait 5 gameframes
      LoadPendingPlayers
    endif
  endif
endscript
script StandardGameFlow
  GameFlow_Startup
  begin
    GameFlow_StartRun
    if ReplayModeEquals REPLAY_MODE_PLAY_SAVED
      GameFlow_PlayRun
      GameFlow_WaitEnd
      SwitchToMenu menu = replay_from_memcard_menu
      pause_game_flow
    else
      if ReplayModeEquals REPLAY_MODE_PLAY_AFTER_RUN
        GameFlow_PlayRun
        GameFlow_WaitEnd
        SwitchToMenu menu = replay_menu
        pause_game_flow
      else
        GameFlow_PlayRun
        GameFlow_WaitEnd
        if IsCompetition
          if CompetitionEnded
            EndCompetitionRun
            PauseMusic 1
            leader_screen_gameflow
            pause_game_flow
            break
          else
            EndCompetitionRun
            New_Judge_screen_gameflow
            pause_game_flow
            leader_screen_gameflow
            pause_game_flow
            if RoundIs 3
              break
            else
              InitializeSkaters
            endif
          endif
        else
          if GameModeEquals is_horse
            if EndRunSelected
              break
            endif
            horse_end_run
            if HorseEnded
              break
            else
              InitializeSkaters
            endif
          else
            break
          endif
        endif
      endif
    endif
  repeat
  horse_uninit
  GameFlow_End
  WaitFrameLoop
endscript
script spawn_movie
  SpawnScript play_movie_task Params = { <...> }
endscript
script play_movie_task
  playmovie_script <...>
endscript
script gf
  Cleanup
  request_level level = Load_Rio
  ChangeLevelGameFlow
endscript
script ShowAllObjects
  if CareerLevelIs LevelNum_Foundry
  endif
  if CareerLevelIs LevelNum_Canada
    AJC_Script_Can_Network_Startup
  endif
  if CareerLevelIs LevelNum_Suburbia
  endif
  if CareerLevelIs LevelNum_SkaterIsland
  endif
  if CareerLevelIs LevelNum_Airport
    CPF_AP_BeginAt_ShowAll
  endif
  if CareerLevelIs LevelNum_Rio
  endif
  if CareerLevelIs LevelNum_LA
  endif
  if CareerLevelIs LevelNum_Tokyo
    JS_Tok_NetObserver
  endif
  if CareerLevelIs LevelNum_Ship
    BDJ_SHP_NetObserverStartup
  endif
  if LevelIs Load_Ware
    JKU_Ware_IsObserver
  endif
endscript
script Splitscreen_StartupScript
  if LevelIs Load_Shp
    printf "Launching BDJ_SHP_LoadingScript................................."
    BDJ_SHP_LoadingScript
  endif
endscript
script LoadMultiplayerSounds
  if GameModeChecksumEquals ctf
    LoadSound "MP\ctf_flag_captured1_crowd" vol = 100
    LoadSound "MP\ctf_flag_captured2_crowd" vol = 100
    LoadSound "MP\ctf_flag_dropped" vol = 100
    LoadSound "MP\ctf_flag_returned" vol = 100
    LoadSound "MP\ctf_flag_taken1_crowd" vol = 100
    LoadSound "MP\ctf_flag_taken2_crowd" vol = 100
  endif
  if GameModeChecksumEquals ownthezone
    LoadSound "MP\zone_control_all" vol = 100
    LoadSound "MP\zone_control_zone" vol = 100
    LoadSound "MP\zone_drop_key" vol = 100
    LoadSound "MP\zone_get_key" vol = 100
    LoadSound "MP\zone_lose_all" vol = 100
    LoadSound "MP\zone_lose_zone" vol = 100
  endif
  if GameModeChecksumEquals beachball
    LoadSound "MP\beach_ball_bounce" vol = 100
    LoadSound "MP\beach_ball_dropped" vol = 100
    LoadSound "MP\beach_ball_taken" vol = 100
  endif
endscript
