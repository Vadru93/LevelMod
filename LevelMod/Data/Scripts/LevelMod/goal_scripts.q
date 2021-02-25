
GOAL_BRONZE = 0
GOAL_SILVER = 1
GOAL_GOLD = 2
GOAL_HIGHSCORE = 0
GOAL_PROSCORE = 1
GOAL_SICKSCORE = 2
GOAL_SKATE = 3
GOAL_TRICKSPOT = 4
GOAL_TAPE = 5
GOAL_SCRIPTED1 = 6
GOAL_SCRIPTED2 = 7
GOAL_SCRIPTED3 = 8
AWARD_TRICKSLOT = 63
GOAL_ALL_GOALS = 62
GOAL_LETTER_S = 61
GOAL_LETTER_K = 60
GOAL_LETTER_A = 59
GOAL_LETTER_T = 58
GOAL_LETTER_E = 57
GOAL_DECK = 56
GOAL_STAT_POINT1 = 55
GOAL_STAT_POINT2 = 54
GOAL_STAT_POINT3 = 53
GOAL_STAT_POINT4 = 52
GOAL_STAT_POINT5 = 51
GOAL_STAT_POINTS = 50
GOAL_ALL_GAPS = 49
GOAL_BOGUS = 48
PACER = 47
SKATE_LINE1 = 46
SKATE_LINE2 = 45
SKATE_LINE3 = 44
DECK_ICON1 = 43
DECK_ICON2 = 42
DECK_ICON3 = 41
STAT_SET_A = 40
STAT_SET_B = 39
STAT_SET_C = 38
STAT_SET_D = 37
WAIT_FOR_HIGHSCORE = 36
WAIT_FOR_PROSCORE = 35
WAIT_FOR_SICKSCORE = 34
WAIT_FOR_SKATE = 33
WAIT_FOR_TAPE = 32
WAIT_FOR_TRICKSPOT = 31
WAIT_FOR_SCRIPTED1 = 30
WAIT_FOR_SCRIPTED2 = 29
WAIT_FOR_SCRIPTED3 = 28
STAT_POINT1 = 0
STAT_POINT2 = 1
STAT_POINT3 = 2
STAT_POINT4 = 3
STAT_POINT5 = 4
DefaultGoalParams = { Goal_Start_Script = NullScript Goal_Start_End_Script = NullScript Goal_Complete_Start_Script = NullScript NetPlayScript = NullScript CareerPlayScript = NullScript CameraStart = "Cam_Goals" CameraTape = "Cam_Goal_04" CameraTrickspotVert = "Cam_Goal_05vert" CameraTrickspotVertComplete = "Cam_Goal_05vert" CameraTrickspotStreet = "Cam_Goal_05street" CameraTrickspotStreetComplete = "Cam_Goal_05street" CameraScripted1 = "Cam_Goal_06" CameraScripted1Complete = "Cam_Goal_Complete_06" CameraScripted2 = "Cam_Goal_07" CameraScripted2Complete = "Cam_Goal_Complete_07" CameraScripted3 = "Cam_Goal_07" CameraScripted3Complete = "Cam_Goals" CameraLevelEnd = "Cam_Goals" Goal_HighScore_Text = "Get a HIGH SCORE: XXXX Points" Goal_ProScore_Text = "Get a PRO SCORE: YYYY Points" Goal_SickScore_Text = "Get a SICK SCORE: ZZZ Points" Goal_SKATE_Text = "Collect S-K-A-T-E" Goal_Tape_Text = "Get the Secret Tape" Goal_TrickspotStreet_Text = "Street Trick X at Location Y" Goal_TrickspotVert_Text = "Vert Trick X at Location Y" Goal_Scripted1_Text = "Scripted Goal 1" Goal_Scripted2_Text = "Scripted Goal 2" Goal_Scripted3_Text = "Scripted Goal 3" Goal_Trickspot_Start_Script = NullScript Goal_TrickspotVert_Complete_Start_Script = NullScript Goal_TrickspotStreet_Complete_Start_Script = NullScript Goal_Scripted1_Start_Script = NullScript Goal_Scripted1_End_Script = NullScript Goal_Scripted1_Complete_Start_Script = NullScript Goal_Scripted1_Complete_End_Script = NullScript Goal_Scripted2_Start_Script = NullScript Goal_Scripted2_End_Script = NullScript Goal_Scripted2_Complete_Start_Script = NullScript Goal_Scripted2_Complete_End_Script = NullScript Goal_Scripted3_Start_Script = NullScript Goal_Scripted3_End_Script = NullScript Goal_Scripted3_Complete_Start_Script = NullScript Goal_Scripted3_Complete_End_Script = NullScript Goal_End_Script = NullScript Goal_Complete_End_Script = NullScript }

script WaitForControllerPressedX
  ResetKeyState
  begin
    wait 1.0 frame
    if ControllerPressed X
      break
    endif
  repeat
  ControllerDebounce X time = 0.3
endscript

script WaitForControllerPressedXNoDebounce
  ResetKeyState
  begin
    wait 1.0 frame
    if ControllerPressed X
      break
    endif
  repeat
endscript

script WaitForPressX
  ResetKeyState
  begin
    wait 1.0 frame
    if Held X
      break
    endif
  repeat
  Debounce X time = 0.3
endscript

script WaitForReleaseX
  ResetKeyState
  begin
    wait 1.0 frame
    if Released X
      break
    endif
  repeat
  Debounce X time = 0.3
endscript

script WaitForPressAndReleaseX
  ResetKeyState
  begin
    wait 1.0 frame
    if Held X
      begin
        wait 1 frame
        if Released X
          UnPauseSkaters
          break
        endif
      repeat
      break
    endif
  repeat
  Debounce X time = 0.3
endscript

script UnSetWaitForGoalFlags
  UnSetFlag flag = WAIT_FOR_HIGHSCORE
  UnSetFlag flag = WAIT_FOR_PROSCORE
  UnSetFlag flag = WAIT_FOR_SICKSCORE
  UnSetFlag flag = WAIT_FOR_SKATE
  UnSetFlag flag = WAIT_FOR_TAPE
  UnSetFlag flag = WAIT_FOR_SCRIPTED1
  UnSetFlag flag = WAIT_FOR_SCRIPTED2
  UnSetFlag flag = WAIT_FOR_SCRIPTED3
endscript

script GameStart DefaultGoalParams
  KillMessages
  if IsCareerMode
    PauseSkaters
    MoveSkaterZ dist = -2000
    UnSetFlag flag = WAIT_FOR_HIGHSCORE
    UnSetFlag flag = WAIT_FOR_PROSCORE
    UnSetFlag flag = WAIT_FOR_SICKSCORE
    UnSetFlag flag = WAIT_FOR_SKATE
    UnSetFlag flag = WAIT_FOR_TAPE
    UnSetFlag flag = WAIT_FOR_SCRIPTED1
    UnSetFlag flag = WAIT_FOR_SCRIPTED2
    UnSetFlag flag = WAIT_FOR_SCRIPTED3
    Kill prefix = "TRG_Goal_Letter"
    Kill prefix = "TRG_Goal_Secret_T"
    Kill prefix = "TRG_Stat"
    PauseMusic 1
    PauseStream 0
     <Goal_Start_Script>
    if CareerLevelIs LevelNum_Foundry
      LaunchLocalMessage id = ScreenTitle "The FOUNDRY" LevelTitle
    endif
    if CareerLevelIs LevelNum_Canada
      LaunchLocalMessage id = ScreenTitle "CANADA" LevelTitle
    endif
    if CareerLevelIs LevelNum_Suburbia
      LaunchLocalMessage id = ScreenTitle "SUBURBIA" LevelTitle
    endif
    if CareerLevelIs LevelNum_SkaterIsland
      LaunchLocalMessage id = ScreenTitle "SKATER ISLAND" LevelTitle
    endif
    if CareerLevelIs LevelNum_Airport
      LaunchLocalMessage id = ScreenTitle "The AIRPORT" LevelTitle
    endif
    if CareerLevelIs LevelNum_Rio
      LaunchLocalMessage id = ScreenTitle "RIO DE JANEIRO" LevelTitle
    endif
    if CareerLevelIs LevelNum_LA
      LaunchLocalMessage id = ScreenTitle "LOS ANGELES" LevelTitle
    endif
    if CareerLevelIs LevelNum_Tokyo
      LaunchLocalMessage id = ScreenTitle "TOKYO" LevelTitle
    endif
    if CareerLevelIs LevelNum_Ship
      LaunchLocalMessage id = ScreenTitle "CRUISE SHIP" LevelTitle
    endif
    PlaySkaterCamAnim skater = 0 name = <CameraStart> Play_Hold
    GoalViewAllGoalCompleteCheck From_movies
     <CareerPlayScript>
    SetUpSkaterDefaults
    CreateSkateLines2
    CreateSecretTape
    CreateTrickPoints2
    if GetGoal goal = GOAL_HIGHSCORE
    else
      PlaySound goaldone Vol = 100
      LaunchLocalMessage <Goal_HighScore_Text> panel_message_goal
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 120
    endif
    Debounce X time = 0.3
    if GetGoal goal = GOAL_PROSCORE
    else
      PlaySound goaldone Vol = 100
      LaunchLocalMessage <Goal_ProScore_Text> panel_message_goal
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 120
    endif
    Debounce X time = 0.3
    if GetGoal goal = GOAL_SICKSCORE
    else
      PlaySound goaldone Vol = 100
      LaunchLocalMessage <Goal_SickScore_Text> panel_message_goal
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 120
    endif
    Debounce X time = 0.3
    if GetGoal goal = GOAL_SKATE
    else
      PlaySound goaldone Vol = 100
      LaunchLocalMessage <Goal_SKATE_Text> panel_message_goal
      wait 1 gameframe
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 120
    endif
     <Goal_Start_End_Script>
    Debounce X time = 0.3
    if GetGoal goal = GOAL_TAPE
    else
      PlaySkaterCamAnim skater = 0 name = <CameraTape>
      PlaySound goaldone Vol = 100
      LaunchLocalMessage <Goal_Tape_Text> panel_message_goal
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraTape>
          break
        endif
        wait 1 gameframe
      repeat 200
    endif
    Debounce X time = 0.3
    if GetGoal goal = GOAL_TRICKSPOT
    else
      CreatePhotoGuy
      PauseSkaters
       <Goal_Trickspot_Start_Script>
      if ProfileEquals trickstyle = street
        PlaySkaterCamAnim skater = 0 name = <CameraTrickspotStreet>
        PlaySound goaldone Vol = 100
        LaunchLocalMessage <Goal_TrickspotStreet_Text> panel_message_goal
        wait 1 frames
        SetSkaterCamAnimSkippable skater = 0 skippable = 1
        begin
          if SkaterCamAnimFinished skater = 0 name = <CameraTrickspotStreet>
            break
          endif
          wait 1 gameframe
        repeat 150
      endif
      if ProfileEquals trickstyle = vert
        PlaySkaterCamAnim skater = 0 name = <CameraTrickspotVert>
        PlaySound goaldone Vol = 100
        LaunchLocalMessage <Goal_TrickspotVert_Text> panel_message_goal
        wait 1 frames
        SetSkaterCamAnimSkippable skater = 0 skippable = 1
        begin
          if SkaterCamAnimFinished skater = 0 name = <CameraTrickspotVert>
            break
          endif
          wait 1 gameframe
        repeat 150
      endif
    endif
    Debounce X time = 0.3
    KillPhotoGuy
    if GetGoal goal = GOAL_SCRIPTED3
    else
      LaunchLocalMessage <Goal_Scripted3_Text> panel_message_goal
      PlaySkaterCamAnim skater = 0 name = <CameraScripted3>
      PlaySound goaldone Vol = 100
       <Goal_Scripted3_Start_Script>
      wait 1 frames
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraScripted3>
          break
        endif
        wait 1 gameframe
      repeat
       <Goal_Scripted3_End_Script>
    endif
    Debounce X time = 0.3
    if GetGoal goal = GOAL_SCRIPTED1
    else
      LaunchLocalMessage <Goal_Scripted1_Text> panel_message_goal
      PlaySkaterCamAnim skater = 0 name = <CameraScripted1>
      PlaySound goaldone Vol = 100
       <Goal_Scripted1_Start_Script>
      wait 1 frames
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraScripted1>
          break
        endif
        wait 1 gameframe
      repeat
       <Goal_Scripted1_End_Script>
    endif
    Debounce X time = 0.3
    if GetGoal goal = GOAL_SCRIPTED2
    else
      LaunchLocalMessage <Goal_Scripted2_Text> panel_message_goal
      PlaySkaterCamAnim skater = 0 name = <CameraScripted2>
      PlaySound goaldone Vol = 100
       <Goal_Scripted2_Start_Script>
      wait 1 frames
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraScripted2>
          break
        endif
        wait 1 gameframe
      repeat
       <Goal_Scripted2_End_Script>
    endif
    KillMessages
    Kill prefix = "TRG_Goal_Letter"
    Kill prefix = "TRG_Goal_Secret_T"
    Kill prefix = "TRG_Stat"
    UnPauseSkaters
    MoveSkaterZ dist = 2000
    Debounce X time = 8.0
    PlaySkaterCamAnim skater = 0 name = <CameraLevelEnd> Play_Hold
  else
    if InMultiplayerGame
    endif
  endif
endscript

script NullScript
endscript
FoundryScoreParams = { Goal_HighScore_Text = "Get a HIGH SCORE:| 10,000 Points" Goal_ProScore_Text = "Get a PRO SCORE:| 30,000 Points" Goal_SickScore_Text = "Get a SICK SCORE:| 60,000 Points" }
CanadaScoreParams = { Goal_HighScore_Text = "Get a HIGH SCORE:| 35,000 Points" Goal_ProScore_Text = "Get a PRO SCORE:| 70,000 Points" Goal_SickScore_Text = "Get a SICK SCORE:| 120,000 Points" }
SuburbiaScoreParams = { Goal_HighScore_Text = "Get a HIGH SCORE:| 55,000 Points" Goal_ProScore_Text = "Get a PRO SCORE:| 110,000 Points" Goal_SickScore_Text = "Get a SICK SCORE:| 200,000 Points" }
AirportScoreParams = { Goal_HighScore_Text = "Get a HIGH SCORE:| 75,000 Points" Goal_ProScore_Text = "Get a PRO SCORE:| 150,000 Points" Goal_SickScore_Text = "Get a SICK SCORE:| 300,000 Points" }
LosAngelesScoreParams = { Goal_HighScore_Text = "Get a HIGH SCORE:| 100,000 Points" Goal_ProScore_Text = "Get a PRO SCORE:| 190,000 Points" Goal_SickScore_Text = "Get a SICK SCORE:| 400,000 Points" }
ShipScoreParams = { Goal_HighScore_Text = "Get a HIGH SCORE:| 150,000 Points" Goal_ProScore_Text = "Get a PRO SCORE:| 225,000 Points" Goal_SickScore_Text = "Get a SICK SCORE:| 500,000 Points" }

script StartRunScript
  ClearScoreGoals
  if CareerLevelIs LevelNum_Foundry
    SetScoreGoal Score = 10000 GoalScript = Got_HighScore goal = GOAL_HIGHSCORE
    SetScoreGoal Score = 30000 GoalScript = Got_ProScore goal = GOAL_PROSCORE
    SetScoreGoal Score = 60000 GoalScript = Got_SickScore goal = GOAL_SICKSCORE
    GameStart FoundryGoalParams FoundryScoreParams
  endif
  if CareerLevelIs LevelNum_Canada
    SetScoreGoal Score = 35000 GoalScript = Got_HighScore goal = GOAL_HIGHSCORE
    SetScoreGoal Score = 70000 GoalScript = Got_ProScore goal = GOAL_PROSCORE
    SetScoreGoal Score = 120000 GoalScript = Got_SickScore goal = GOAL_SICKSCORE
    GameStart CanadaGoalParams CanadaScoreParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    SetScoreGoal Score = 55000 GoalScript = Got_HighScore goal = GOAL_HIGHSCORE
    SetScoreGoal Score = 110000 GoalScript = Got_ProScore goal = GOAL_PROSCORE
    SetScoreGoal Score = 200000 GoalScript = Got_SickScore goal = GOAL_SICKSCORE
    GameStart SuburbiaGoalParams SuburbiaScoreParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    CompIntroSequence SICompParams
  endif
  if CareerLevelIs LevelNum_Airport
    SetScoreGoal Score = 75000 GoalScript = Got_HighScore goal = GOAL_HIGHSCORE
    SetScoreGoal Score = 150000 GoalScript = Got_ProScore goal = GOAL_PROSCORE
    SetScoreGoal Score = 300000 GoalScript = Got_SickScore goal = GOAL_SICKSCORE
    GameStart AirportGoalParams AirportScoreParams
  endif
  if CareerLevelIs LevelNum_Rio
    CompIntroSequence RioCompParams
  endif
  if CareerLevelIs LevelNum_LA
    SetScoreGoal Score = 100000 GoalScript = Got_HighScore goal = GOAL_HIGHSCORE
    SetScoreGoal Score = 190000 GoalScript = Got_ProScore goal = GOAL_PROSCORE
    SetScoreGoal Score = 400000 GoalScript = Got_SickScore goal = GOAL_SICKSCORE
    GameStart LosAngelesGoalParams LosAngelesScoreParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    CompIntroSequence TokCompParams
  endif
  if CareerLevelIs LevelNum_Ship
    SetScoreGoal Score = 150000 GoalScript = Got_HighScore goal = GOAL_HIGHSCORE
    SetScoreGoal Score = 225000 GoalScript = Got_ProScore goal = GOAL_PROSCORE
    SetScoreGoal Score = 500000 GoalScript = Got_SickScore goal = GOAL_SICKSCORE
    GameStart ShipGoalParams ShipScoreParams
  endif
  if GotParam From_menu
    SwitchToMenu menu = end_run_menu
  endif
endscript

script EndRunScript
  KillMessages
  if CareerLevelIs LevelNum_Foundry
    EndGame FoundryGoalParams FoundryScoreParams
  endif
  if CareerLevelIs LevelNum_Canada
    EndGame CanadaGoalParams CanadaScoreParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    EndGame SuburbiaGoalParams SuburbiaScoreParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    CompEndSequence SICompParams
  endif
  if CareerLevelIs LevelNum_Airport
    EndGame AirportGoalParams AirportScoreParams
  endif
  if CareerLevelIs LevelNum_Rio
    CompEndSequence RioCompParams
  endif
  if CareerLevelIs LevelNum_LA
    EndGame LosAngelesGoalParams LosAngelesScoreParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    CompEndSequence TokyoGoalParams
  endif
  if CareerLevelIs LevelNum_Ship
    EndGame ShipGoalParams ShipScoreParams
  endif
endscript

script SetUpSkaterDefaults
  if IsCareerMode
    UnSetFlag flag = SKATE_LINE1
    UnSetFlag flag = SKATE_LINE2
    UnSetFlag flag = SKATE_LINE3
    UnSetFlag flag = DECK_ICON1
    UnSetFlag flag = DECK_ICON2
    UnSetFlag flag = DECK_ICON3
    UnSetFlag flag = STAT_SET_A
    UnSetFlag flag = STAT_SET_B
    UnSetFlag flag = STAT_SET_C
    UnSetFlag flag = STAT_SET_D
    if ProfileEquals is_custom
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_D
    endif
    if ProfileEquals is_named = hawk
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_A
    endif
    if ProfileEquals is_named = caballero
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_B
    endif
    if ProfileEquals is_named = campbell
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_C
    endif
    if ProfileEquals is_named = glifberg
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_D
    endif
    if ProfileEquals is_named = koston
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_A
    endif
    if ProfileEquals is_named = lasek
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_B
    endif
    if ProfileEquals is_named = margera
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_C
    endif
    if ProfileEquals is_named = mullen
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_D
    endif
    if ProfileEquals is_named = muska
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_A
    endif
    if ProfileEquals is_named = reynolds
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_B
    endif
    if ProfileEquals is_named = rowley
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_C
    endif
    if ProfileEquals is_named = steamer
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_D
    endif
    if ProfileEquals is_named = thomas
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_A
    endif
    if ProfileEquals is_named = carrera
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_B
    endif
    if ProfileEquals is_named = shaft
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_C
    endif
    if ProfileEquals is_named = mohawkman
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_D
    endif
    if ProfileEquals is_named = yourdaddy
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_A
    endif
    if ProfileEquals is_named = jewett
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_B
    endif
    if ProfileEquals is_named = Dick
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_C
    endif
    if ProfileEquals is_named = demoness
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_D
    endif
    if ProfileEquals is_named = wolverine
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_A
    endif
    if ProfileEquals is_named = maul
      SetFlag flag = SKATE_LINE1
      SetFlag flag = DECK_ICON1
      SetFlag flag = STAT_SET_B
    endif
    if ProfileEquals is_named = slater
      SetFlag flag = SKATE_LINE2
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_C
    endif
    if ProfileEquals is_named = ollie
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON2
      SetFlag flag = STAT_SET_D
    endif
    if ProfileEquals is_named = eyeball
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_A
    endif
    if ProfileEquals is_named = doomguy
      SetFlag flag = SKATE_LINE3
      SetFlag flag = DECK_ICON3
      SetFlag flag = STAT_SET_A
    endif
  endif
endscript

script CreateSkateLines
endscript

script CreateSkateLines2
  LaunchLocalMessage id = S " " panel_message_letterS
  LaunchLocalMessage id = K " " panel_message_letterK
  LaunchLocalMessage id = A " " panel_message_letterA
  LaunchLocalMessage id = T " " panel_message_letterT
  LaunchLocalMessage id = E " " panel_message_letterE
  if IsCareerMode
    if GetGoal goal = GOAL_SKATE
    else
      UnSetFlag flag = GOAL_LETTER_S
      UnSetFlag flag = GOAL_LETTER_K
      UnSetFlag flag = GOAL_LETTER_A
      UnSetFlag flag = GOAL_LETTER_T
      UnSetFlag flag = GOAL_LETTER_E
      if GetFlag flag = SKATE_LINE1
        Create prefix = "TRG_Goal_LetterA"
      endif
      if GetFlag flag = SKATE_LINE2
        Create prefix = "TRG_Goal_LetterB"
      endif
      if GetFlag flag = SKATE_LINE3
        Create prefix = "TRG_Goal_LetterC"
      endif
    endif
  endif
endscript

script CreateSecretTape
  if IsCareerMode
    Kill prefix = "TRG_Goal_Secret_T"
    if GetGoal goal = GOAL_TAPE
    else
      Create name = TRG_Goal_Secret_Tape
    endif
  endif
endscript

script CreateDeckIcon
  if IsCareerMode
    if GetFlag flag = GOAL_DECK
    else
      Kill prefix = "TRG_Goal_Deck"
      if GetFlag flag = DECK_ICON1
        Create name = TRG_Goal_Deck_1
      endif
      if GetFlag flag = DECK_ICON2
        Create name = TRG_Goal_Deck_2
      endif
      if GetFlag flag = DECK_ICON3
        Create name = TRG_Goal_Deck_3
      endif
    endif
  endif
endscript

script KillTrickPoints
  if InMultiplayerGame
  else
    if IsCareerMode
      Kill prefix = "TRG_Stat_Point_"
    endif
  endif
endscript

script CreateTrickPoints
endscript

script CreateTrickPoints2
  if InMultiplayerGame
  else
    if IsCareerMode
      if GetFlag flag = STAT_SET_A
        if GetFlag flag = GOAL_STAT_POINT1
        else
          Create name = TRG_Stat_Point_A01
        endif
        if GetFlag flag = GOAL_STAT_POINT2
        else
          Create name = TRG_Stat_Point_A02
        endif
        if GetFlag flag = GOAL_STAT_POINT3
        else
          Create name = TRG_Stat_Point_A03
        endif
        if GetFlag flag = GOAL_STAT_POINT4
        else
          Create name = TRG_Stat_Point_A04
        endif
        if GetFlag flag = GOAL_STAT_POINT5
        else
          Create name = TRG_Stat_Point_A05
        endif
      endif
      if GetFlag flag = STAT_SET_B
        if GetFlag flag = GOAL_STAT_POINT1
        else
          Create name = TRG_Stat_Point_B01
        endif
        if GetFlag flag = GOAL_STAT_POINT2
        else
          Create name = TRG_Stat_Point_B02
        endif
        if GetFlag flag = GOAL_STAT_POINT3
        else
          Create name = TRG_Stat_Point_B03
        endif
        if GetFlag flag = GOAL_STAT_POINT4
        else
          Create name = TRG_Stat_Point_B04
        endif
        if GetFlag flag = GOAL_STAT_POINT5
        else
          Create name = TRG_Stat_Point_B05
        endif
      endif
      if GetFlag flag = STAT_SET_C
        if GetFlag flag = GOAL_STAT_POINT1
        else
          Create name = TRG_Stat_Point_C01
        endif
        if GetFlag flag = GOAL_STAT_POINT2
        else
          Create name = TRG_Stat_Point_C02
        endif
        if GetFlag flag = GOAL_STAT_POINT3
        else
          Create name = TRG_Stat_Point_C03
        endif
        if GetFlag flag = GOAL_STAT_POINT4
        else
          Create name = TRG_Stat_Point_C04
        endif
        if GetFlag flag = GOAL_STAT_POINT5
        else
          Create name = TRG_Stat_Point_C05
        endif
      endif
      if GetFlag flag = STAT_SET_D
        if GetFlag flag = GOAL_STAT_POINT1
        else
          Create name = TRG_Stat_Point_D01
        endif
        if GetFlag flag = GOAL_STAT_POINT2
        else
          Create name = TRG_Stat_Point_D02
        endif
        if GetFlag flag = GOAL_STAT_POINT3
        else
          Create name = TRG_Stat_Point_D03
        endif
        if GetFlag flag = GOAL_STAT_POINT4
        else
          Create name = TRG_Stat_Point_D04
        endif
        if GetFlag flag = GOAL_STAT_POINT5
        else
          Create name = TRG_Stat_Point_D05
        endif
      endif
    endif
  endif
endscript

script Goal_SKATE_Letter_S
  if IsCareerMode
  else
    Die
  endif
  Obj_RotY speed = 200
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_SKATE_Letter_S
  begin
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script Got_SKATE_Letter_S
  Obj_ClearExceptions
  PlaySound gapsound Vol = 100
  LaunchLocalMessage id = S "S" panel_message_letterS
  SetFlag flag = GOAL_LETTER_S
  GOAL_SKATE_Check
endscript

script Goal_SKATE_Letter_K
  if IsCareerMode
  else
    Die
  endif
  Obj_RotY speed = 200
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_SKATE_Letter_K
  begin
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script Got_SKATE_Letter_K
  Obj_ClearExceptions
  PlaySound gapsound Vol = 100
  LaunchLocalMessage id = K "K" panel_message_letterK
  SetFlag flag = GOAL_LETTER_K
  GOAL_SKATE_Check
endscript

script Goal_SKATE_Letter_A
  if IsCareerMode
  else
    Die
  endif
  Obj_RotY speed = 200
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_SKATE_Letter_A
  begin
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script Got_SKATE_Letter_A
  Obj_ClearExceptions
  PlaySound gapsound Vol = 100
  LaunchLocalMessage id = A "A" panel_message_letterA
  SetFlag flag = GOAL_LETTER_A
  GOAL_SKATE_Check
endscript

script Goal_SKATE_Letter_T
  if IsCareerMode
  else
    Die
  endif
  Obj_RotY speed = 200
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_SKATE_Letter_T
  begin
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script Got_SKATE_Letter_T
  Obj_ClearExceptions
  PlaySound gapsound Vol = 100
  LaunchLocalMessage id = T "T" panel_message_letterT
  SetFlag flag = GOAL_LETTER_T
  GOAL_SKATE_Check
endscript

script Goal_SKATE_Letter_E
  if IsCareerMode
  else
    Die
  endif
  Obj_RotY speed = 200
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_SKATE_Letter_E
  begin
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script Got_SKATE_Letter_E
  Obj_ClearExceptions
  PlaySound gapsound Vol = 100
  LaunchLocalMessage id = E "E" panel_message_letterE
  SetFlag flag = GOAL_LETTER_E
  GOAL_SKATE_Check
endscript

script Goal_STAT_POINT
  if IsCareerMode
  else
    Die
  endif
  if GotParam StatPoint1
    Obj_SetFlag STAT_POINT1
  endif
  if GotParam StatPoint2
    Obj_SetFlag STAT_POINT2
  endif
  if GotParam StatPoint3
    Obj_SetFlag STAT_POINT3
  endif
  if GotParam StatPoint4
    Obj_SetFlag STAT_POINT4
  endif
  if GotParam StatPoint5
    Obj_SetFlag STAT_POINT5
  endif
  Obj_RotX angle = 90 speed = 20000
  Obj_RotZ speed = 250
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_STAT_POINT
  begin
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script Goal_TRICK_POINT
  Die
endscript

script Got_STAT_POINT
  Obj_ClearExceptions
  PlaySound gapsound Vol = 100
  Obj_VarSet var = 0 value = 0
  SetGlobalFlag flag = PROMPT_FOR_SAVE
  if Obj_FlagSet STAT_POINT1
    SetFlag flag = GOAL_STAT_POINT1
  endif
  if Obj_FlagSet STAT_POINT2
    SetFlag flag = GOAL_STAT_POINT2
  endif
  if Obj_FlagSet STAT_POINT3
    SetFlag flag = GOAL_STAT_POINT3
  endif
  if Obj_FlagSet STAT_POINT4
    SetFlag flag = GOAL_STAT_POINT4
  endif
  if Obj_FlagSet STAT_POINT5
    SetFlag flag = GOAL_STAT_POINT5
  endif
  if GetFlag flag = GOAL_STAT_POINT1
    Obj_VarInc var = 0
  endif
  if GetFlag flag = GOAL_STAT_POINT2
    Obj_VarInc var = 0
  endif
  if GetFlag flag = GOAL_STAT_POINT3
    Obj_VarInc var = 0
  endif
  if GetFlag flag = GOAL_STAT_POINT4
    Obj_VarInc var = 0
  endif
  if GetFlag flag = GOAL_STAT_POINT5
    Obj_VarInc var = 0
  endif
  if ProfileEquals is_named = demoness
    StatCounter
    LaunchLocalMessage id = GoalText "Possession Points" panel_message_statpickup
  else
    AwardStatPoint
    if ProfileEquals is_named = carrera
      StatCounter
      LaunchLocalMessage id = GoalText "Demerit Points" panel_message_statpickup
    else
      if ProfileEquals is_named = jewett
        StatCounter
        LaunchLocalMessage id = GoalText "Mullet Points" panel_message_statpickup
      else
        if ProfileEquals is_named = Dick
          StatCounter
          LaunchLocalMessage id = GoalText "Donut Points" panel_message_statpickup
        else
          if ProfileEquals is_named = wolverine
            StatCounter
            LaunchLocalMessage id = GoalText "Mutation Points" panel_message_statpickup
          else
            if ProfileEquals is_named = maul
              StatCounter
              LaunchLocalMessage id = GoalText "Force Points" panel_message_statpickup
            else
              if ProfileEquals is_named = ollie
                StatCounter
                LaunchLocalMessage id = GoalText "Puke Points" panel_message_statpickup
              else
                if ProfileEquals is_named = slater
                  StatCounter
                  LaunchLocalMessage id = GoalText "Break Points" panel_message_statpickup
                else
                  if Obj_VarEQ var = 0 value = 1
                    LaunchLocalMessage id = StatCount "1 of 5 Stat Points" panel_message_statpickup
                  endif
                  if Obj_VarEQ var = 0 value = 2
                    LaunchLocalMessage id = StatCount "2 of 5 Stat Points" panel_message_statpickup
                  endif
                  if Obj_VarEQ var = 0 value = 3
                    LaunchLocalMessage id = StatCount "3 of 5 Stat Points" panel_message_statpickup
                  endif
                  if Obj_VarEQ var = 0 value = 4
                    LaunchLocalMessage id = StatCount "4 of 5 Stat Points" panel_message_statpickup
                  endif
                  if Obj_VarEQ var = 0 value = 5
                    LaunchLocalMessage id = StatCount "5 of 5 Stat Points" panel_message_statpickup
                  endif
                endif
              endif
            endif
          endif
        endif
      endif
    endif
  endif
  Die
endscript

script StatCounter
  if Obj_VarEQ var = 0 value = 1
    LaunchLocalMessage id = StatCount "1 of 5" panel_message_statpickup base_pos = (0, -30)
  endif
  if Obj_VarEQ var = 0 value = 2
    LaunchLocalMessage id = StatCount "2 of 5" panel_message_statpickup base_pos = (0, -30)
  endif
  if Obj_VarEQ var = 0 value = 3
    LaunchLocalMessage id = StatCount "3 of 5" panel_message_statpickup base_pos = (0, -30)
  endif
  if Obj_VarEQ var = 0 value = 4
    LaunchLocalMessage id = StatCount "4 of 5" panel_message_statpickup base_pos = (0, -30)
  endif
  if Obj_VarEQ var = 0 value = 5
    LaunchLocalMessage id = StatCount "5 of 5" panel_message_statpickup base_pos = (0, -30)
  endif
endscript

script Goal_Deck_Icon
  if IsCareerMode
  else
    Die
  endif
  wait 1 frame
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_Deck_Icon
  begin
    Obj_RotX time = 0.8 speed = 200
    Obj_RotY time = 0.8 speed = 20
    Obj_RotZ time = 0.8 speed = 20
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
    Obj_RotX time = 0.8 speed = 20
    Obj_RotY time = 0.8 speed = 200
    Obj_RotZ time = 0.8 speed = 20
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
    Obj_RotX time = 0.8 speed = 20
    Obj_RotY time = 0.8 speed = 20
    Obj_RotZ time = 0.8 speed = 200
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script Got_Deck_Icon
  Obj_ClearExceptions
  LaunchLocalMessage id = GoalText "New Deck" panel_message_DeckIcon_100
  PlaySound gapsound Vol = 100
  SetGlobalFlag flag = PROMPT_FOR_SAVE
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_DECK
  SetFlag flag = GOAL_DECK
  MidGameCheckAllGoals
  Die
endscript

script Goal_Secret_Tape
  if IsCareerMode
  else
    Die
  endif
  Obj_RotX angle = 90 speed = 900
  wait 2 frames
  Obj_RotZ speed = 200
  Obj_SetInnerRadius 8
  Obj_SetException ex = SkaterInRadius scr = Got_Secret_Tape
  begin
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, 1, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -6, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -2, 0) time = 0.1
    Obj_WaitMove
    Obj_MoveToRelPos (0, -1, 0) time = 0.1
    Obj_WaitMove
  repeat
endscript

script WaitForGoalCompletionTextFree
  begin
    if GetFlag flag = WAIT_FOR_HIGHSCORE
      wait 1 frame
    else
      if GetFlag flag = WAIT_FOR_PROSCORE
        wait 1 frame
      else
        if GetFlag flag = WAIT_FOR_SICKSCORE
          wait 1 frame
        else
          if GetFlag flag = WAIT_FOR_SKATE
            wait 1 frame
          else
            if GetFlag flag = WAIT_FOR_TAPE
              wait 1 frame
            else
              if GetFlag flag = WAIT_FOR_TRICKSPOT
                wait 1 frame
              else
                if GetFlag flag = WAIT_FOR_SCRIPTED1
                  wait 1 frame
                else
                  if GetFlag flag = WAIT_FOR_SCRIPTED2
                    wait 1 frame
                  else
                    if GetFlag flag = WAIT_FOR_SCRIPTED3
                      wait 1 frame
                    else
                      break
                    endif
                  endif
                endif
              endif
            endif
          endif
        endif
      endif
    endif
  repeat
endscript

script Got_HighScore
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_HIGHSCORE
  PlaySound goaldone Vol = 100
  LaunchLocalMessage id = GoalName "High Score!" panel_message_goalcomplete
  wait 1 frame
  LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_HIGHSCORE
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  MidGameCheckGoals
endscript

script Got_ProScore
  wait 1 frame
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_PROSCORE
  PlaySound goaldone Vol = 100
  LaunchLocalMessage id = GoalName "Pro Score!" panel_message_goalcomplete
  LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_PROSCORE
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  MidGameCheckGoals
endscript

script Got_SickScore
  wait 2 frames
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_SICKSCORE
  PlaySound goaldone Vol = 100
  LaunchLocalMessage id = GoalName "Sick Score!" panel_message_goalcomplete
  LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_SICKSCORE
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  MidGameCheckGoals
endscript

script GOAL_SKATE_Check
  Obj_ClearExceptions
  SpawnScript GOAL_SKATE_Check2
  Die
endscript

script GOAL_SKATE_Check2
  if GetFlag flag = GOAL_LETTER_E
    if GetFlag flag = GOAL_LETTER_T
      if GetFlag flag = GOAL_LETTER_A
        if GetFlag flag = GOAL_LETTER_K
          if GetFlag flag = GOAL_LETTER_S
            SetGoal goal = GOAL_SKATE
            wait 3 frames
            WaitForGoalCompletionTextFree
            SetFlag flag = WAIT_FOR_SKATE
            PlaySound goaldone Vol = 100
            LaunchLocalMessage id = GoalText "Collect S-K-A-T-E" panel_message_goalcomplete
            LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
            SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
            wait 2 seconds
            UnSetFlag flag = WAIT_FOR_SKATE
            MidGameCheckGoals
          endif
        endif
      endif
    endif
  endif
endscript

script Got_Secret_Tape
  Obj_ClearExceptions
  SpawnScript Got_Secret_Tape2
  Die
endscript

script Got_Secret_Tape2
  SetGoal goal = GOAL_TAPE
  wait 4 frames
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_TAPE
  LaunchLocalMessage id = GoalName "Secret Tape!" panel_message_goalcomplete
  LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  PlaySound goaldone Vol = 100
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_TAPE
  MidGameCheckGoals
endscript

script Got_TrickSpot
  if IsCareerMode
    if GetGoal goal = GOAL_TRICKSPOT
    else
      SetGoal goal = GOAL_TRICKSPOT
      SpawnScript Got_TrickSpot2
      PlayStream random( @photo_27a @photo_28 ) 
    endif
  endif
endscript

script Got_TrickSpot2
  if CareerLevelIs LevelNum_Foundry
    Got_LevelSpecificTrickspot FoundryGoalParams
  endif
  if CareerLevelIs LevelNum_Canada
    Got_LevelSpecificTrickspot CanadaGoalParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    Got_LevelSpecificTrickspot SuburbiaGoalParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    Got_LevelSpecificTrickspot SkaterIslandGoalParams
  endif
  if CareerLevelIs LevelNum_Airport
    Got_LevelSpecificTrickspot AirportGoalParams
  endif
  if CareerLevelIs LevelNum_Rio
    Got_LevelSpecificTrickspot RioGoalParams
  endif
  if CareerLevelIs LevelNum_LA
    Got_LevelSpecificTrickspot LosAngelesGoalParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    Got_LevelSpecificTrickspot TokyoGoalParams
  endif
  if CareerLevelIs LevelNum_Ship
    Got_LevelSpecificTrickspot ShipGoalParams
  endif
endscript

script Got_LevelSpecificTrickspot
  wait 5 frames
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_TRICKSPOT
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  if ProfileEquals trickstyle = street
    LaunchLocalMessage id = GoalText <Goal_TrickspotStreet_Text> panel_message_goalcomplete
    LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  endif
  if ProfileEquals trickstyle = vert
    LaunchLocalMessage id = GoalText <Goal_TrickspotVert_Text> panel_message_goalcomplete
    LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  endif
  PlaySound goaldone Vol = 100
  wait 1 second
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_TRICKSPOT
  MidGameCheckGoals
endscript

script Got_Scripted1
  if IsCareerMode
    if GetGoal goal = GOAL_SCRIPTED1
    else
      SetGoal goal = GOAL_SCRIPTED1
      SpawnScript Got_LevelSpecificScripted1_2
    endif
  endif
endscript

script Got_LevelSpecificScripted1_2
  if CareerLevelIs LevelNum_Foundry
    Got_LevelSpecificScripted1 FoundryGoalParams
  endif
  if CareerLevelIs LevelNum_Canada
    Got_LevelSpecificScripted1 CanadaGoalParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    Got_LevelSpecificScripted1 SuburbiaGoalParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    Got_LevelSpecificScripted1 SkaterIslandGoalParams
  endif
  if CareerLevelIs LevelNum_Airport
    Got_LevelSpecificScripted1 AirportGoalParams
  endif
  if CareerLevelIs LevelNum_Rio
    Got_LevelSpecificScripted1 RioGoalParams
  endif
  if CareerLevelIs LevelNum_LA
    Got_LevelSpecificScripted1 LosAngelesGoalParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    Got_LevelSpecificScripted1 TokyoGoalParams
  endif
  if CareerLevelIs LevelNum_Ship
    Got_LevelSpecificScripted1 ShipGoalParams
  endif
endscript

script Got_LevelSpecificScripted1
  wait 6 frames
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_SCRIPTED1
  LaunchLocalMessage id = GoalText <Goal_Scripted1_Text> panel_message_goalcomplete
  LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  PlaySound goaldone Vol = 100
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_SCRIPTED1
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  MidGameCheckGoals
endscript

script Got_Scripted2
  if IsCareerMode
    if GetGoal goal = GOAL_SCRIPTED2
    else
      SetGoal goal = GOAL_SCRIPTED2
      SpawnScript Got_LevelSpecificScripted2_2
    endif
  endif
endscript

script Got_LevelSpecificScripted2_2
  if CareerLevelIs LevelNum_Foundry
    Got_LevelSpecificScripted2 FoundryGoalParams
  endif
  if CareerLevelIs LevelNum_Canada
    Got_LevelSpecificScripted2 CanadaGoalParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    Got_LevelSpecificScripted2 SuburbiaGoalParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    Got_LevelSpecificScripted2 SkaterIslandGoalParams
  endif
  if CareerLevelIs LevelNum_Airport
    Got_LevelSpecificScripted2 AirportGoalParams
  endif
  if CareerLevelIs LevelNum_Rio
    Got_LevelSpecificScripted2 RioGoalParams
  endif
  if CareerLevelIs LevelNum_LA
    Got_LevelSpecificScripted2 LosAngelesGoalParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    Got_LevelSpecificScripted2 TokyoGoalParams
  endif
  if CareerLevelIs LevelNum_Ship
    Got_LevelSpecificScripted2 ShipGoalParams
  endif
endscript

script Got_LevelSpecificScripted2
  wait 7 frames
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_SCRIPTED2
  LaunchLocalMessage id = GoalText <Goal_Scripted2_Text> panel_message_goalcomplete
  LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  PlaySound goaldone Vol = 100
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_SCRIPTED2
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  MidGameCheckGoals
endscript

script Got_Scripted3
  if IsCareerMode
    if GetGoal goal = GOAL_SCRIPTED3
    else
      SetGoal goal = GOAL_SCRIPTED3
      SpawnScript Got_LevelSpecificScripted3_2
    endif
  endif
endscript

script Got_LevelSpecificScripted3_2
  if CareerLevelIs LevelNum_Foundry
    Got_LevelSpecificScripted3 FoundryGoalParams
  endif
  if CareerLevelIs LevelNum_Canada
    Got_LevelSpecificScripted3 CanadaGoalParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    Got_LevelSpecificScripted3 SuburbiaGoalParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    Got_LevelSpecificScripted3 SkaterIslandGoalParams
  endif
  if CareerLevelIs LevelNum_Airport
    Got_LevelSpecificScripted3 AirportGoalParams
  endif
  if CareerLevelIs LevelNum_Rio
    Got_LevelSpecificScripted3 RioGoalParams
  endif
  if CareerLevelIs LevelNum_LA
    Got_LevelSpecificScripted3 LosAngelesGoalParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    Got_LevelSpecificScripted3 TokyoGoalParams
  endif
  if CareerLevelIs LevelNum_Ship
    Got_LevelSpecificScripted3 ShipGoalParams
  endif
endscript

script Got_LevelSpecificScripted3
  wait 8 frames
  WaitForGoalCompletionTextFree
  SetFlag flag = WAIT_FOR_SCRIPTED3
  LaunchLocalMessage id = GoalText <Goal_Scripted3_Text> panel_message_goalcomplete
  LaunchLocalMessage id = complete "Complete" panel_message_goalcompleteline2
  PlaySound goaldone Vol = 100
  wait 2 seconds
  UnSetFlag flag = WAIT_FOR_SCRIPTED3
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_GOAL
  MidGameCheckGoals
endscript

script MidGameCheckStatPoints
  if GetFlag flag = GOAL_STAT_POINT1
    if GetFlag flag = GOAL_STAT_POINT2
      if GetFlag flag = GOAL_STAT_POINT3
        if GetFlag flag = GOAL_STAT_POINT4
          if GetFlag flag = GOAL_STAT_POINT5
            SetFlag flag = GOAL_STAT_POINTS
            if ProfileEquals is_named = demoness
              LaunchLocalMessage id = GoalText "All Possession Points" panel_message_TrickPoints_100
            else
              if ProfileEquals is_named = carrera
                LaunchLocalMessage id = GoalText "All Demerit Points" panel_message_TrickPoints_100
              else
                if ProfileEquals is_named = jewett
                  LaunchLocalMessage id = GoalText "All Mullet Points" panel_message_TrickPoints_100
                else
                  if ProfileEquals is_named = Dick
                    LaunchLocalMessage id = GoalText "All Donut Points" panel_message_TrickPoints_100
                  else
                    if ProfileEquals is_named = wolverine
                      LaunchLocalMessage id = GoalText "All Mutation Points" panel_message_TrickPoints_100
                    else
                      LaunchLocalMessage id = GoalText "All Stat Points" panel_message_TrickPoints_100
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
endscript

script MidGameCheckGoals
  if GetGoal goal = GOAL_SCRIPTED3
    if GetGoal goal = GOAL_SCRIPTED2
      if GetGoal goal = GOAL_SCRIPTED1
        if GetGoal goal = GOAL_TRICKSPOT
          if GetGoal goal = GOAL_TAPE
            if GetGoal goal = GOAL_SKATE
              if GetGoal goal = GOAL_SICKSCORE
                if GetGoal goal = GOAL_PROSCORE
                  if GetGoal goal = GOAL_HIGHSCORE
                    SetFlag flag = GOAL_ALL_GOALS
                  endif
                endif
              endif
            endif
          endif
        endif
      endif
    endif
  endif
  if JustGotFlag flag = GOAL_ALL_GOALS
    PlaySound goaldone Vol = 150
    LaunchLocalMessage id = GoalText "All Goals| Complete" panel_message_Goals_100
  endif
endscript

script MidGameCheckAllGoals
  if GetGoal goal = GOAL_SCRIPTED3
    if GetGoal goal = GOAL_SCRIPTED2
      if GetGoal goal = GOAL_SCRIPTED1
        if GetGoal goal = GOAL_TRICKSPOT
          if GetGoal goal = GOAL_TAPE
            if GetGoal goal = GOAL_SKATE
              if GetGoal goal = GOAL_SICKSCORE
                if GetGoal goal = GOAL_PROSCORE
                  if GetGoal goal = GOAL_HIGHSCORE
                    SetFlag flag = GOAL_ALL_GOALS
                    if GetFlag flag = GOAL_DECK
                      if GetFlag flag = GOAL_STAT_POINTS
                        if GetFlag flag = GOAL_ALL_GAPS
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
    endif
  endif
endscript
panel_message_goal = { font = "title.fnt" just = (0, 0) dims = (320, 112) colors = [ { (128, 128, 128) alpha = 70 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.3 time = 0 }
    { pos = (320, 205) alpha = 20 scale = 1.05 time = 0.1 }
    { pos = (320, 209) alpha = 50 scale = 0.95 time = 0.2 }
    { pos = (320, 211) alpha = 70 scale = 0.8 time = 0.3 }
    { pos = (320, 212) alpha = 130 scale = 1.01 time = 0.4 }
    { time = 1.8 }
    { pos = (320, 214) alpha = 100 scale = 0.85 time = 2.0 }
    { pos = (320, 217) alpha = 40 scale = 0.55 time = 2.1 }
    { pos = (320, 222) alpha = 0 scale = 0.25 time = 2.2 } ] }
panel_message_goalcompletemid = { font = "large.fnt" just = (0, 0) dims = (320, 112) colors = [ { (120, 20, 20) alpha = 70 } ] key_points = [ { pos = (335, 260) alpha = 0 scale = 0.3 time = 0 }
    { pos = (335, 260) alpha = 0 scale = 0.3 time = 0.5 }
    { pos = (335, 265) alpha = 20 scale = 2.05 time = 0.1 }
    { pos = (335, 269) alpha = 50 scale = 1.95 time = 0.2 }
    { pos = (335, 271) alpha = 70 scale = 1.8 time = 0.3 }
    { pos = (335, 272) alpha = 130 scale = 2.01 time = 0.4 }
    { time = 1.4 }
    { pos = (335, 274) alpha = 100 scale = 0.85 time = 1.6 }
    { pos = (335, 277) alpha = 40 scale = 0.55 time = 1.7 }
    { pos = (335, 282) alpha = 0 scale = 0.25 time = 1.9 } ] }
panel_message_newlevel = { font = "large.fnt" just = (0, 0) dims = (320, 112) colors = [ { (90, 90, 90) alpha = 70 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.3 time = 0 }
    { pos = (320, 205) alpha = 20 scale = 1.05 time = 0.1 }
    { pos = (320, 209) alpha = 50 scale = 1.95 time = 0.2 }
    { pos = (320, 211) alpha = 70 scale = 1.3 time = 0.3 }
    { pos = (320, 212) alpha = 130 scale = 1.21 time = 0.4 }
    { time = 2.0 }
    { pos = (320, 214) alpha = 100 scale = 1.35 time = 2.2 }
    { pos = (320, 217) alpha = 40 scale = 0.55 time = 2.3 }
    { pos = (320, 222) alpha = 0 scale = 0.25 time = 2.4 } ] }
panel_message_newlevelopen = { font = "title.fnt" just = (0, 0) dims = (320, 112) colors = [ { (20, 20, 120) alpha = 70 } ] key_points = [ { pos = (335, 260) alpha = 0 scale = 0.3 time = 0 }
    { pos = (335, 260) alpha = 0 scale = 0.3 time = 0.5 }
    { pos = (335, 265) alpha = 20 scale = 2.05 time = 0.1 }
    { pos = (335, 269) alpha = 50 scale = 2.15 time = 0.2 }
    { pos = (335, 271) alpha = 70 scale = 2.3 time = 0.3 }
    { pos = (335, 272) alpha = 130 scale = 2.51 time = 0.4 }
    { time = 2.0 }
    { pos = (335, 274) alpha = 100 scale = 1.85 time = 2.2 }
    { pos = (335, 277) alpha = 40 scale = 0.95 time = 2.3 }
    { pos = (335, 282) alpha = 0 scale = 0.25 time = 2.4 } ] }
panel_message_goalcomplete1 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 112) alpha = 0 scale = 1.2 time = 0 }
    { alpha = 128 scale = 1.01 time = 0.5 }
    { alpha = 128 scale = 1.01 time = 0.9 }
    { alpha = 0 scale = 0.85 time = 1 } ] }
panel_message_goalcomplete = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (320, 112) alpha = 128 scale = 1.01 time = 0 }
    { time = 1.7 } ] }
panel_message_goalcomplete2 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 132) alpha = 0 scale = 1.2 time = 0 }
    { alpha = 128 scale = 1.01 time = 0.5 }
    { alpha = 128 scale = 1.01 time = 0.9 }
    { alpha = 0 scale = 0.85 time = 1 } ] }
panel_message_goalcomplete3 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 152) alpha = 0 scale = 1.2 time = 0 }
    { alpha = 128 scale = 1.01 time = 0.5 }
    { alpha = 128 scale = 1.01 time = 0.9 }
    { alpha = 0 scale = 0.85 time = 1 } ] }
panel_message_goalcomplete4 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 172) alpha = 0 scale = 1.2 time = 0 }
    { alpha = 128 scale = 1.01 time = 0.5 }
    { alpha = 128 scale = 1.01 time = 0.9 }
    { alpha = 0 scale = 0.85 time = 1 } ] }
panel_message_goalcomplete5 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 192) alpha = 0 scale = 1.2 time = 0 }
    { alpha = 128 scale = 1.01 time = 0.5 }
    { alpha = 128 scale = 1.01 time = 0.9 }
    { alpha = 0 scale = 0.85 time = 1 } ] }
panel_message_goalcomplete6 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 212) alpha = 0 scale = 1.2 time = 0 }
    { alpha = 128 scale = 1.01 time = 0.5 }
    { alpha = 128 scale = 1.01 time = 0.9 }
    { alpha = 0 scale = 0.85 time = 1 } ] }
panel_message_goalcomplete7 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 232) alpha = 0 scale = 1.2 time = 0 }
    { alpha = 128 scale = 1.01 time = 0.5 }
    { alpha = 128 scale = 1.01 time = 0.9 }
    { alpha = 0 scale = 0.85 time = 1 } ] }
panel_message_goalcompleteline2 = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (320, 132) alpha = 0 scale = 1.01 time = 0 }
    { pos = (320, 132) alpha = 0 scale = 1.01 time = 0.1 }
    { pos = (320, 132) alpha = 128 scale = 1.01 time = 0.2 }
    { time = 1.7 } ] }
panel_message_goalcomprule = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (320, 224) alpha = 128 scale = 1.25 time = 0 }
    { time = 2 } ] }
panel_message_secondarypickup = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (320, 312) alpha = 128 scale = 1.01 time = 0 }
    { time = 1.7 } ] }
panel_message_statpickup = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (177, 88, 26) alpha = 128 } ] key_points = [ { pos = (320, 312) alpha = 128 scale = 1.01 time = 0 }
    { time = 1.7 } ] }
panel_message_PickUp = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (105, 105, 150) alpha = 115 } ] key_points = [ { pos = (0, 180) alpha = 0 scale = 1.9 time = 0 }
    { pos = (25, 180) alpha = 70 scale = 1.9 time = 0.15 }
    { pos = (40, 180) alpha = 100 scale = 1.9 time = 0.3 }
    { pos = (50, 180) alpha = 115 scale = 1.9 time = 0.4 }
    { pos = (50, 180) alpha = 255 scale = 2.5 time = 0.6 }
    { pos = (50, 180) alpha = 115 scale = 1.9 time = 1.0 }
    { time = 1200 } ] }
panel_message_PickUpDropped = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (105, 105, 150) alpha = 115 } ] key_points = [ { pos = (50, 180) alpha = 115 scale = 1.9 time = 0 }
    { pos = (50, 190) alpha = 115 scale = 1.9 time = 0.3 }
    { pos = (50, 210) alpha = 115 scale = 1.9 time = 0.4 }
    { pos = (50, 260) alpha = 115 scale = 1.9 time = 0.5 }
    { pos = (50, 500) alpha = 115 scale = 1.9 time = 0.6 } ] }
panel_message_PickUpVoided = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (105, 105, 150) alpha = 115 } ] key_points = [ { pos = (50, 180) alpha = 115 scale = 1.9 time = 0 }
    { pos = (50, 180) alpha = 215 scale = 2.2 time = 0.3 }
    { pos = (50, 180) alpha = 255 scale = 2.5 time = 0.4 }
    { pos = (50, 180) alpha = 115 scale = 1.9 time = 0.5 }
    { pos = (50, 180) alpha = 65 scale = 1.9 time = 0.6 }
    { pos = (50, 180) alpha = 0 scale = 1.9 time = 0.7 } ] }
panel_message_letterS = { font = "large.fnt" just = (1, 0) dims = (600, 112) colors = [ { (145, 105, 10) alpha = 115 } ] key_points = [ { pos = (695, 150) alpha = 0 scale = 1.5 time = 0 }
    { pos = (665, 150) alpha = 70 scale = 1.5 time = 0.15 }
    { pos = (650, 150) alpha = 100 scale = 1.5 time = 0.3 }
    { pos = (645, 150) alpha = 115 scale = 1.5 time = 0.4 }
    { time = 1200 } ] }
panel_message_letterK = { font = "large.fnt" just = (1, 0) dims = (600, 112) colors = [ { (145, 105, 10) alpha = 115 } ] key_points = [ { pos = (698, 180) alpha = 0 scale = 1.5 time = 0 }
    { pos = (668, 180) alpha = 70 scale = 1.5 time = 0.15 }
    { pos = (653, 180) alpha = 100 scale = 1.5 time = 0.3 }
    { pos = (648, 180) alpha = 115 scale = 1.5 time = 0.4 }
    { time = 1200 } ] }
panel_message_letterA = { font = "large.fnt" just = (1, 0) dims = (600, 112) colors = [ { (145, 105, 10) alpha = 115 } ] key_points = [ { pos = (696, 210) alpha = 0 scale = 1.5 time = 0 }
    { pos = (666, 210) alpha = 70 scale = 1.5 time = 0.15 }
    { pos = (651, 210) alpha = 100 scale = 1.5 time = 0.3 }
    { pos = (646, 210) alpha = 115 scale = 1.5 time = 0.4 }
    { time = 1200 } ] }
panel_message_letterT = { font = "large.fnt" just = (1, 0) dims = (600, 112) colors = [ { (145, 105, 10) alpha = 115 } ] key_points = [ { pos = (693, 241) alpha = 0 scale = 1.5 time = 0 }
    { pos = (663, 241) alpha = 70 scale = 1.5 time = 0.15 }
    { pos = (648, 241) alpha = 100 scale = 1.5 time = 0.3 }
    { pos = (643, 241) alpha = 115 scale = 1.5 time = 0.4 }
    { time = 1200 } ] }
panel_message_letterE = { font = "large.fnt" just = (1, 0) dims = (600, 112) colors = [ { (145, 105, 10) alpha = 115 } ] key_points = [ { pos = (693, 270) alpha = 0 scale = 1.5 time = 0 }
    { pos = (663, 270) alpha = 70 scale = 1.5 time = 0.15 }
    { pos = (648, 270) alpha = 100 scale = 1.5 time = 0.3 }
    { pos = (643, 270) alpha = 115 scale = 1.5 time = 0.4 }
    { time = 1200 } ] }
panel_message_goal1_all = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 126) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 121) alpha = 20 scale = 0.6 time = 0.1 }
    { pos = (320, 117) alpha = 50 scale = 0.8 time = 0.2 }
    { pos = (320, 114) alpha = 70 scale = 0.92 time = 0.3 }
    { pos = (320, 112) alpha = 115 scale = 1.01 time = 0.4 }
    { time = 6.4 }
    { alpha = 100 scale = 0.98 time = 6.6 }
    { alpha = 70 scale = 0.95 time = 6.8 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal2_all = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 146) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 146) alpha = 0 scale = 0.2 time = 0.5 }
    { pos = (320, 141) alpha = 20 scale = 0.6 time = 0.6 }
    { pos = (320, 137) alpha = 50 scale = 0.8 time = 0.7 }
    { pos = (320, 134) alpha = 70 scale = 0.92 time = 0.8 }
    { pos = (320, 132) alpha = 115 scale = 1.01 time = 0.9 }
    { time = 6.4 }
    { alpha = 100 scale = 0.98 time = 6.6 }
    { alpha = 70 scale = 0.95 time = 6.8 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal3_all = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 166) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 166) alpha = 0 scale = 0.2 time = 1.0 }
    { pos = (320, 161) alpha = 20 scale = 0.6 time = 1.1 }
    { pos = (320, 157) alpha = 50 scale = 0.8 time = 1.2 }
    { pos = (320, 154) alpha = 70 scale = 0.92 time = 1.3 }
    { pos = (320, 152) alpha = 115 scale = 1.01 time = 1.4 }
    { time = 6.4 }
    { alpha = 100 scale = 0.98 time = 6.6 }
    { alpha = 70 scale = 0.95 time = 6.8 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal4_all = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 186) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 186) alpha = 0 scale = 0.2 time = 1.5 }
    { pos = (320, 181) alpha = 20 scale = 0.6 time = 1.6 }
    { pos = (320, 177) alpha = 50 scale = 0.8 time = 1.7 }
    { pos = (320, 174) alpha = 70 scale = 0.92 time = 1.8 }
    { pos = (320, 172) alpha = 115 scale = 1.01 time = 1.9 }
    { time = 6.4 }
    { alpha = 100 scale = 0.98 time = 6.6 }
    { alpha = 70 scale = 0.95 time = 6.8 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal5_all = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 206) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 206) alpha = 0 scale = 0.2 time = 2.0 }
    { pos = (320, 201) alpha = 20 scale = 0.6 time = 2.1 }
    { pos = (320, 197) alpha = 50 scale = 0.8 time = 2.2 }
    { pos = (320, 194) alpha = 70 scale = 0.92 time = 2.3 }
    { pos = (320, 192) alpha = 115 scale = 1.01 time = 2.4 }
    { time = 6.4 }
    { alpha = 100 scale = 0.98 time = 6.6 }
    { alpha = 70 scale = 0.95 time = 6.8 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal6_all = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 226) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 226) alpha = 0 scale = 0.2 time = 2.5 }
    { pos = (320, 221) alpha = 20 scale = 0.6 time = 2.6 }
    { pos = (320, 217) alpha = 50 scale = 0.8 time = 2.7 }
    { pos = (320, 214) alpha = 70 scale = 0.92 time = 2.8 }
    { pos = (320, 212) alpha = 115 scale = 1.01 time = 2.9 }
    { time = 6.4 }
    { alpha = 100 scale = 0.98 time = 6.6 }
    { alpha = 70 scale = 0.95 time = 6.8 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal7_all = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 246) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 246) alpha = 0 scale = 0.2 time = 3.0 }
    { pos = (320, 241) alpha = 20 scale = 0.6 time = 3.1 }
    { pos = (320, 237) alpha = 50 scale = 0.8 time = 3.2 }
    { pos = (320, 234) alpha = 70 scale = 0.92 time = 3.3 }
    { pos = (320, 232) alpha = 115 scale = 1.01 time = 3.4 }
    { time = 6.4 }
    { alpha = 100 scale = 0.98 time = 6.6 }
    { alpha = 70 scale = 0.95 time = 6.8 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal0_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 80) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal1_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 110) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 110) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal2_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 140) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 140) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal3_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 170) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 170) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal4_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal5_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 230) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 230) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal6_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 260) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 260) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal7_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 290) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 290) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal8_got = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 90, 90) alpha = 35 } ] key_points = [ { pos = (320, 320) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 320) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal0_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 80) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 80) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal0a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 80) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 80) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 80) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal1_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 110) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 110) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 110) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal1a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 110) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 110) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 110) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 110) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal2_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 140) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 140) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 140) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal2a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 140) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 140) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 140) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 140) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal3_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 170) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 170) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 170) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal3a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 170) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 170) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 170) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 170) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal4_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 200) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal4a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 200) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 200) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal5_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 230) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 230) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 230) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal5a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 230) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 230) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 230) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 230) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal6_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 260) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 260) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 260) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal6a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 260) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 260) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 260) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 260) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal7_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 290) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 290) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 290) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal7a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 290) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 290) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 290) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 290) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal8_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 320) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 320) alpha = 128 scale = 1.01 time = 0.5 }
    { pos = (320, 320) alpha = 0 scale = 1.01 time = 1.0 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal8a_justgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (200, 60, 60) alpha = 65 } ] key_points = [ { pos = (320, 320) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 320) alpha = 8 scale = 1.01 time = 0.5 }
    { pos = (320, 320) alpha = 128 scale = 1.01 time = 1.0 }
    { pos = (320, 320) alpha = 40 scale = 1.01 time = 1.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal0_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 80) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal1_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 110) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 110) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal2_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 140) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 140) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal3_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 170) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 170) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal4_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal5_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 230) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 230) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal6_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 260) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 260) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal7_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 290) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 290) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goal8_notgot = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (320, 320) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 320) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_Goals_100 = { font = "large.fnt" just = (0, 0) dims = (640, 112) colors = [ { (128, 128, 128) alpha = 65 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.5 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.9 }
    { pos = (320, 200) alpha = 0 scale = 0.2 time = 1.5 } ] }
panel_message_Gaps_100 = { font = "large.fnt" just = (0, 0) dims = (640, 112) colors = [ { (128, 128, 128) alpha = 65 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.5 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.9 }
    { pos = (320, 200) alpha = 0 scale = 0.2 time = 1.5 } ] }
panel_message_TrickPoints_100 = { font = "large.fnt" just = (0, 0) dims = (640, 112) colors = [ { (128, 128, 128) alpha = 65 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.5 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.9 }
    { pos = (320, 200) alpha = 0 scale = 0.2 time = 1.5 } ] }
panel_message_DeckIcon_100 = { font = "large.fnt" just = (0, 0) dims = (640, 112) colors = [ { (128, 128, 128) alpha = 65 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.5 }
    { pos = (320, 200) alpha = 128 scale = 1.51 time = 0.9 }
    { pos = (320, 200) alpha = 0 scale = 0.2 time = 1.5 } ] }
panel_message_Goals_notGot = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (128, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (128, 360) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_Gaps_notGot = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (256, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (256, 360) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_TrickPoints_notGot = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (384, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (384, 360) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_DeckIcon_notGot = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (0, 128, 0) alpha = 65 } ] key_points = [ { pos = (512, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (512, 360) alpha = 128 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_Goals_Got = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (200, 90, 90) alpha = 65 } ] key_points = [ { pos = (128, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (128, 360) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_Gaps_Got = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (200, 90, 90) alpha = 65 } ] key_points = [ { pos = (256, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (256, 360) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_TrickPoints_Got = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (200, 90, 90) alpha = 65 } ] key_points = [ { pos = (384, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (384, 360) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_DeckIcon_Got = { font = "large.fnt" just = (0, 0) dims = (60, 112) colors = [ { (200, 90, 90) alpha = 65 } ] key_points = [ { pos = (512, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (512, 360) alpha = 30 scale = 1.01 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_goalTitle = { font = "Title.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (330, 40) alpha = 0 scale = 0.2 time = 0 }
    { pos = (330, 40) alpha = 128 scale = 1.41 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
panel_message_XtoContinue = { font = "small.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (330, 410) alpha = 0 scale = 0.2 time = 0 }
    { pos = (330, 410) alpha = 128 scale = 1.05 time = 0.2 }
    { time = 1000000000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
SideListBox = { box_color = { (40, 40, 40) alpha = 64 } box_pos_rel = (0, 0) box_w = 320 box_h = 34 }
SideListBoxSelected = { box_color = { (128, 40, 40) alpha = 100 } box_pos_rel = (0, 0) box_w = 220 box_h = 34 }
LevelTitle = { font = "Title.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (320, 50) alpha = 0 scale = 0.2 time = 0 }
    { pos = (320, 50) alpha = 128 scale = 1.41 time = 0.5 }
    { time = 6000 }
    { alpha = 0 scale = 0.9 time = 7.0 } ] }
SelectedGoalTitle = { font = "title.fnt" just = (0, 0) dims = (320, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (400, 202) alpha = 128 scale = 1.01 time = 0 }
    { time = 6000 } ] }
SideList_JustGotBlowOut = { SideListCompletedHeader key_points = [ { pos = (320, 190) alpha = 0 scale = 1.01 time = 0 }
    { time = 0.4 }
    { pos = (320, 202) alpha = 64 scale = 1.01 time = 0.4 }
    { pos = (320, 204) alpha = 0 scale = 5 time = 1.2 }
    { time = 4 } ] }
SideList_JustGotBlowOutInList = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (160, 40, 20) alpha = 128 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.8 time = 0 }
    { time = 0.4 }
    { pos = (320, 80) alpha = 64 scale = 0.8 time = 0.4 }
    { pos = (320, 80) alpha = 0 scale = 2 time = 1 } ] }
SideListJustGot2 = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (160, 40, 20) alpha = 128 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.81 time = 0 }
    { pos = (320, 80) alpha = 128 scale = 0.81 time = 0.2 }
    { time = 6000 } ] }
SideListJustGotPulse = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (160, 40, 20) alpha = 128 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.8 time = 0 }
    { pos = (320, 80) alpha = 128 scale = 1.2 time = 0.2 }
    { pos = (320, 80) alpha = 128 scale = 0.81 time = 0.6 }
    { time = 6000 } ] }
SideListCompleted = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (128, 128, 128) alpha = 0 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.81 time = 0 }
    { pos = (320, 80) alpha = 32 scale = 0.81 time = 0.2 }
    { time = 6000 } ] }
SideListCompletedQuick = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (128, 128, 128) alpha = 0 } ] key_points = [ { pos = (320, 80) alpha = 32 scale = 1 time = 0 }
    { pos = (320, 80) alpha = 32 scale = 0.8 time = 0.2 }
    { time = 6000 } ] }
SideListNotComplete = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (320, 80) alpha = 0 scale = 0.81 time = 0 }
    { pos = (320, 80) alpha = 100 scale = 0.81 time = 0.2 }
    { time = 6000 } ] }
SideListUnselected = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (320, 80) alpha = 100 scale = 0.8 time = 0 }
    { time = 6000 } ] }
SideListSelected = { font = "large.fnt" just = (0, 0) dims = (200, 112) colors = [ { (128, 128, 128) alpha = 128 scale = 1.2 } ] key_points = [ { pos = (320, 80) alpha = 128 scale = 0.8 time = 0 }
    { scale = 1.2 time = 0.1 }
    { scale = 1 time = 0.3 }
    { time = 6000 } ] }

script SetPacer
  wait 2 seconds
  SetFlag flag = PACER
endscript

script DisplayJustGotGoal
  LaunchLocalMessage id = <id> <GoalText> SideListJustGotPulse base_pos = <pos> SideListBox
endscript

script DisplayKilledGoal
  LaunchLocalMessage id = <id> <GoalText> SideListCompleted base_pos = <pos> SideListBox
endscript

script DisplayTodoGoal
  LaunchLocalMessage id = <id> <GoalText> SideListNotComplete base_pos = <pos> SideListBox
endscript

script DisplaySelected
  LaunchLocalMessage id = <id> <GoalText> SideListSelected base_pos = <pos> SideListBoxSelected
endscript

script DisplayUnselected
  LaunchLocalMessage id = <id> <GoalText> SideListUnselected base_pos = <pos> SideListBox
endscript

script RemoveSkaterFromMovies
  Obj_MoveToRel
endscript

script EndGame DefaultGoalParams
  if IsCareerMode
    PauseMusic 1
    SwitchOnBoard
    PauseSkaters
    TeleportSkaterDown
     <Goal_Complete_Start_Script>
    KillPhotoGuy
    KillGoalMessage id = GoalText " "
    KillGoalMessage id = complete " "
    PlaySkaterCamAnim skater = 0 name = <CameraStart> loop
    if JustGotGoal goal = GOAL_HIGHSCORE
      PlaySkaterCamAnim skater = 0 name = <CameraStart>
    else
      if JustGotGoal goal = GOAL_PROSCORE
        PlaySkaterCamAnim skater = 0 name = <CameraStart>
      else
        if JustGotGoal goal = GOAL_SICKSCORE
          PlaySkaterCamAnim skater = 0 name = <CameraStart>
        else
          if JustGotGoal goal = GOAL_SKATE
            PlaySkaterCamAnim skater = 0 name = <CameraStart>
          else
            if JustGotGoal goal = GOAL_TAPE
              PlaySkaterCamAnim skater = 0 name = <CameraStart>
            else
            endif
          endif
        endif
      endif
    endif
    if JustGotGoal goal = GOAL_HIGHSCORE
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_HighScore_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 100
    endif
    Debounce X time = 0.3
    if JustGotGoal goal = GOAL_PROSCORE
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_ProScore_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 100
    endif
    Debounce X time = 0.3
    if JustGotGoal goal = GOAL_SICKSCORE
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_SickScore_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 100
    endif
    Debounce X time = 0.3
    if JustGotGoal goal = GOAL_SKATE
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_SKATE_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 100
    endif
    Debounce X time = 0.3
    if JustGotGoal goal = GOAL_TAPE
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_Tape_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraStart>
          break
        endif
        wait 1 gameframe
      repeat 100
    endif
    Debounce X time = 0.3
    if JustGotGoal goal = GOAL_TRICKSPOT
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      SwitchOnBoard
      if CareerLevelIs LevelNum_LA
        CPF_NewTrickspotStuff LosAngelesGoalParams LosAngelesScoreParams
      else
        if CareerLevelIs LevelNum_Airport
          CPF_NewTrickspotStuff AirportGoalParams AirportScoreParams
          SparksOn railnotrequired
        else
          if CareerLevelIs LevelNum_Canada
            CPF_NewTrickspotStuff CanadaGoalParams CanadaScoreParams
            SparksOn railnotrequired
          else
            if CareerLevelIs LevelNum_Ship
              CPF_NewTrickspotStuff ShipGoalParams ShipScoreParams
            else
              if CareerLevelIs LevelNum_Suburbia
                CPF_NewTrickspotStuff SuburbiaGoalParams SuburbiaScoreParams
              else
                if CareerLevelIs LevelNum_Foundry
                  CPF_NewTrickspotStuff FoundryGoalParams FoundryScoreParams
                  SparksOn railnotrequired
                else
                  if ProfileEquals trickstyle = vert
                    PlaySkaterCamAnim skater = 0 name = <CameraTrickspotVertComplete> Play_Hold
                    LaunchLocalMessage id = GoalText <Goal_TrickspotVert_Text> panel_message_goal base_pos = (0, -150)
                    LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid base_pos = (0, 100)
                    wait 20 frames
                    begin
                      if ControllerPressed X
                        break
                      endif
                      wait 1 gameframe
                    repeat 200
                  endif
                  if ProfileEquals trickstyle = street
                    PlaySkaterCamAnim skater = 0 name = <CameraTrickspotStreetComplete> Play_Hold
                    LaunchLocalMessage id = GoalText <Goal_TrickspotStreet_Text> panel_message_goal base_pos = (0, -150)
                    LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid base_pos = (0, 100)
                    wait 20 frames
                    begin
                      if ControllerPressed X
                        break
                      endif
                      wait 1 gameframe
                    repeat 200
                  endif
                endif
              endif
            endif
          endif
        endif
      endif
    endif
    SparksOff
    ControllerDebounce X time = 0.3
    if JustGotGoal goal = GOAL_SCRIPTED3
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySkaterCamAnim skater = 0 name = <CameraScripted3Complete>
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_Scripted3_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
       <Goal_Scripted3_Complete_Start_Script>
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraScripted3Complete>
          break
        endif
        wait 1 gameframe
      repeat
       <Goal_Scripted3_Complete_End_Script>
    endif
    ControllerDebounce X time = 0.3
    if JustGotGoal goal = GOAL_SCRIPTED1
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySkaterCamAnim skater = 0 name = <CameraScripted1Complete>
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_Scripted1_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
       <Goal_Scripted1_Complete_Start_Script>
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraScripted1Complete>
          break
        endif
        wait 2 gameframe
      repeat
       <Goal_Scripted1_Complete_End_Script>
    endif
    ControllerDebounce X time = 0.3
    if JustGotGoal goal = GOAL_SCRIPTED2
      SetGlobalFlag flag = PROMPT_FOR_SAVE
      PlaySkaterCamAnim skater = 0 name = <CameraScripted2Complete>
      PlaySound goaldone
      LaunchLocalMessage id = GoalText <Goal_Scripted2_Text> panel_message_goal
      LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid
       <Goal_Scripted2_Complete_Start_Script>
      SetSkaterCamAnimSkippable skater = 0 skippable = 1
      begin
        if SkaterCamAnimFinished skater = 0 name = <CameraScripted2Complete>
          break
        endif
        wait 1 gameframe
      repeat
       <Goal_Scripted2_Complete_End_Script>
    endif
    KillGoalMessage id = GoalText " "
    KillGoalMessage id = complete " "
    ControllerDebounce X time = 0.3
    PlaySkaterCamAnim skater = 0 name = <CameraLevelEnd> Play_Hold
    EndRun_CheckForLevelsOpen
    GoalViewAllGoalCompleteCheck From_movies
    if GetFlag flag = GOAL_ALL_GOALS
      if GetFlag flag = AWARD_TRICKSLOT
      else
        Goal_AwardTrickSlot
        SetFlag flag = AWARD_TRICKSLOT
      endif
      Goal_UnlockingSecrets
    else
      ListAllGoals
      ControllerDebounce X time = 0.3
      LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
      WaitForControllerPressedX
    endif
    ClearEndMessagesSlowly
    TeleportSkaterUp
    UnPauseSkaters
  else
    if InMultiplayerGame
    endif
  endif
endscript
TrickspotLocLeft = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (20, 224) alpha = 0 scale = 1.2 time = 0 }
    { time = 5 } ] }
TrickspotLocTop = { font = "large.fnt" just = (0, 0) dims = (600, 112) colors = [ { (100, 100, 100) alpha = 128 } ] key_points = [ { pos = (320, 20) alpha = 0 scale = 1.2 time = 0 }
    { time = 5 } ] }
TrickspotBoxVert = { box_color = { (20, 20, 20) alpha = 255 } box_pos_rel = (0, 0) box_w = 40 box_h = 448 }
TrickspotBoxHoriz = { box_color = { (20, 20, 20) alpha = 255 } box_pos_rel = (0, 0) box_w = 560 box_h = 40 }

script TrickspotCompleteFlipCheck
  if Flipped
    Flip
  endif
endscript

script CPF_NewTrickspotStuff
  KillMessages
  UnPauseGame
  printf "switching on board -------------------------------------------------------"
  Makeskatergoto NullScript
  KillSpawnedScript name = BailBoardControl
  SwitchOnBoard
  SpawnSkaterScript TrickspotCompleteFlipCheck
  DoFlash duration = 0.07 start_r = 0 start_g = 0 start_b = 0 start_a = 128 end_r = 0 end_g = 0 end_b = 0 end_a = 128
  wait 5 frames
  DoFlash duration = 0.05 start_r = 28 start_g = 28 start_b = 28 start_a = 128 end_r = 28 end_g = 28 end_b = 28 end_a = 128 texture = "photo_effect_32.png"
  if ProfileEquals trickstyle = vert
    PlaySkaterCamAnim skater = 0 name = <CameraTrickspotVertComplete> Play_Hold
    wait 1 gameframe
    PauseGame
    UnPauseSkaters
    Obj_MoveToNode name = TRG_Trickspot_Vert01 orient
    PlayAnim Anim = <Goal_Trickspot_VertAnimName> from = <Goal_Trickspot_VertAnimFrame1> Blendperiod = 0
    wait 1 frame
    SwitchOnBoard
    PauseSkaters
    PlaySound hud_jumpgap Vol = 300
    wait 15 frame
    UnPauseGame
    DoFlash duration = 0.07 start_r = 0 start_g = 0 start_b = 0 start_a = 128 end_r = 0 end_g = 0 end_b = 0 end_a = 128
    wait 5 frames
    DoFlash duration = 0.05 start_r = 28 start_g = 28 start_b = 28 start_a = 128 end_r = 28 end_g = 28 end_b = 28 end_a = 128 texture = "photo_effect_32.png"
    wait 1 frame
    PauseGame
    UnPauseSkaters
    Obj_MoveToNode name = TRG_Trickspot_Vert02 orient
    PlayAnim Anim = <Goal_Trickspot_VertAnimName> from = <Goal_Trickspot_VertAnimFrame2> Blendperiod = 0
    wait 1 frame
    SwitchOnBoard
    PauseSkaters
    PlaySound hud_jumpgap Vol = 300
    wait 15 frame
    UnPauseGame
    DoFlash duration = 0.07 start_r = 0 start_g = 0 start_b = 0 start_a = 128 end_r = 0 end_g = 0 end_b = 0 end_a = 128
    wait 5 frames
    DoFlash duration = 0.05 start_r = 28 start_g = 28 start_b = 28 start_a = 128 end_r = 28 end_g = 28 end_b = 28 end_a = 128 texture = "photo_effect_32.png"
    wait 1 gameframe
    SwitchOnBoard
    PauseGame
    UnPauseSkaters
    Obj_MoveToNode name = TRG_Trickspot_Vert03 orient
    PlayAnim Anim = <Goal_Trickspot_VertAnimName> from = <Goal_Trickspot_VertAnimFrame3> Blendperiod = 0
    wait 1 frame
    SwitchOnBoard
    PauseSkaters
    PlaySound hud_jumpgap Vol = 300
    PlaySound goaldone
    LaunchLocalMessage id = GoalText <Goal_TrickspotVert_Text> panel_message_goal base_pos = (0, -150)
    LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid base_pos = (0, 100)
    PauseStream 0
    StopStream
    PlayStream random( @photo_27a @photo_28 ) 
    begin
      if ControllerPressed X
        break
      endif
      wait 1 gameframe
    repeat 200
    UnPauseGame
    TeleportSkaterDown
    PauseSkaters
  endif
  if ProfileEquals trickstyle = street
    PlaySkaterCamAnim skater = 0 name = <CameraTrickspotStreetComplete> Play_Hold
    wait 1 gameframe
    PauseGame
    UnPauseSkaters
    Obj_MoveToNode name = TRG_Trickspot_Street01 orient
    PlayAnim Anim = <Goal_Trickspot_StreetAnimName> from = <Goal_Trickspot_StreetAnimFrame1> Blendperiod = 0
    wait 1 frame
    SwitchOnBoard
    PauseSkaters
    PlaySound hud_jumpgap Vol = 300
    wait 20 frame
    UnPauseSkaters
    Obj_MoveToNode name = TRG_Trickspot_Street02 orient
    PlayAnim Anim = <Goal_Trickspot_StreetAnimName> from = <Goal_Trickspot_StreetAnimFrame2> Blendperiod = 0
    wait 1 frame
    SwitchOnBoard
    PauseSkaters
    PlaySound hud_jumpgap Vol = 300
    wait 20 frame
    UnPauseSkaters
    Obj_MoveToNode name = TRG_Trickspot_Street03 orient
    PlayAnim Anim = <Goal_Trickspot_StreetAnimName> from = <Goal_Trickspot_StreetAnimFrame3> Blendperiod = 0
    wait 1 frame
    SwitchOnBoard
    PauseSkaters
    PlaySound hud_jumpgap Vol = 300
    PlaySound goaldone
    LaunchLocalMessage id = GoalText <Goal_TrickspotStreet_Text> panel_message_goal base_pos = (0, -150)
    LaunchLocalMessage id = complete "Complete" panel_message_goalcompletemid base_pos = (0, 100)
    PauseStream 0
    StopStream
    PlayStream random( @photo_27a @photo_28 ) 
    begin
      if ControllerPressed X
        break
      endif
      wait 1 gameframe
    repeat 200
    UnPauseGame
    TeleportSkaterDown
    PauseSkaters
  endif
endscript
JUST_UNLOCKED_LEVEL = 0
PROMPT_FOR_SAVE = 1
SHOW_CREDITS = 2
IN_GOAL_MOVIES = 3
LEVEL_UNLOCKED_FOUNDRY = 10
LEVEL_UNLOCKED_CANADA = 11
LEVEL_UNLOCKED_RIO = 12
LEVEL_UNLOCKED_SUBURBIA = 13
LEVEL_UNLOCKED_AIRPORT = 14
LEVEL_UNLOCKED_SKATERISLAND = 15
LEVEL_UNLOCKED_LOSANGELES = 16
LEVEL_UNLOCKED_TOKYO = 17
LEVEL_UNLOCKED_SHIP = 18
GOT_GOLD_RIO = 19
GOT_GOLD_SI = 20
GOT_GOLD_TOKYO = 21
SHOWN_GOLDS_MOVIE = 22
UNLOCKED_SECRET = 23
SKATESHOP_JUST_GOT_BACK = 48
SKATESHOP_JUST_GOT_GOAL = 49
SKATESHOP_JUST_GOT_MEDAL_BRONZE = 50
SKATESHOP_JUST_GOT_MEDAL_SILVER = 51
SKATESHOP_JUST_GOT_MEDAL_GOLD = 52
SKATESHOP_JUST_GOT_LEVEL = 53
SKATESHOP_JUST_GOT_DECK = 54
SKATESHOP_JUST_GOT_SECRET_CHAR = 55
SKATESHOP_JUST_GOT_SECRET_LEVEL = 56
SKATESHOP_JUST_GOT_SECRET_CHEAT = 57
SKATESHOP_JUST_GOT_LEVEL1_GOAL = 58
SKATESHOP_JUST_GOT_LEVEL2_GOAL = 59
SKATESHOP_JUST_GOT_LEVEL4_GOAL = 60
SKATESHOP_JUST_GOT_LEVEL5_GOAL = 61
SKATESHOP_JUST_GOT_LEVEL7_GOAL = 62
SKATESHOP_JUST_GOT_3_MEDALS = 63
SKATESHOP_JUST_GOT_EVERYTHING = 64
SKATESHOP_RETURN_FROM_CAREER = 65
SKATESHOP_RETURN_FROM_SINGLE = 66
SKATESHOP_RETURN_FROM_FREESKATE = 67
SKATESHOP_RETURN_FROM_TUTORIALS = 68
SKATESHOP_RETURN_FROM_PARKEDIT = 69
SKATESHOP_RETURN_FROM_CAS = 70
SKATESHOP_RETURN_FROM_ONLINE = 71
FIRST_SHARED_GLOBAL_FLAG = 100
LEVEL_UNLOCKED_WAREHOUSE = 100
LEVEL_UNLOCKED_BURNSIDE = 101
LEVEL_UNLOCKED_ROSWELL = 102
SKATER_UNLOCKED_MAUL = 103
SKATER_UNLOCKED_WOLVERINE = 104
SKATER_UNLOCKED_DICK = 105
SKATER_UNLOCKED_CARRERA = 106
SKATER_UNLOCKED_BUM = 107
SKATER_UNLOCKED_SLATER = 108
SKATER_UNLOCKED_DEMONESS = 109
SKATER_UNLOCKED_EYEBALL = 110
SHIP_SECRET_UNLOCKED = 111
ALL_SECRETS_UNLOCKED = 112
ALREADY_EDITED_CAS = 113
SKATESHOP_FIRST_TIME_DONE = 114
CHEAT_UNLOCKED_1 = 120
CHEAT_UNLOCKED_2 = 121
CHEAT_UNLOCKED_3 = 122
CHEAT_UNLOCKED_4 = 123
CHEAT_UNLOCKED_5 = 124
CHEAT_UNLOCKED_6 = 125
CHEAT_UNLOCKED_7 = 126
CHEAT_UNLOCKED_8 = 127
CHEAT_UNLOCKED_9 = 128
CHEAT_UNLOCKED_10 = 129
CHEAT_UNLOCKED_11 = 130
CHEAT_UNLOCKED_12 = 131
CHEAT_UNLOCKED_13 = 132
CHEAT_UNLOCKED_14 = 133
CHEAT_UNLOCKED_15 = 134
MOVIE_UNLOCKED_HAWK = 135
MOVIE_UNLOCKED_CABALLERO = 136
MOVIE_UNLOCKED_CAMPBELL = 137
MOVIE_UNLOCKED_GLIFBERG = 138
MOVIE_UNLOCKED_KOSTON = 139
MOVIE_UNLOCKED_LASEK = 140
MOVIE_UNLOCKED_MARGERA = 141
MOVIE_UNLOCKED_MULLEN = 142
MOVIE_UNLOCKED_MUSKA = 143
MOVIE_UNLOCKED_REYNOLDS = 144
MOVIE_UNLOCKED_ROWLEY = 145
MOVIE_UNLOCKED_STEAMER = 146
MOVIE_UNLOCKED_THOMAS = 147
MOVIE_UNLOCKED_JEDIKNIGHT = 148
MOVIE_UNLOCKED_WOLVERINE = 149
MOVIE_UNLOCKED_CAS = 150
MOVIE_UNLOCKED_DICK = 151
MOVIE_UNLOCKED_CARRERA = 152
MOVIE_UNLOCKED_SLATER = 153
MOVIE_UNLOCKED_BUM = 154
MOVIE_UNLOCKED_DEMONESS = 155
MOVIE_UNLOCKED_Eyeball = 156
MOVIE_UNLOCKED_PRO_BAILS1 = 157
SPECIAL_CAS_CAREER_DONE = 158
SPECIAL_HAS_SEEN_SHIP = 159
SPECIAL_HAS_SEEN_TUTORIALS = 160
SECRET_UNLOCK_01 = 161
SECRET_UNLOCK_02 = 162
SECRET_UNLOCK_03 = 163
SECRET_UNLOCK_04 = 164
SECRET_UNLOCK_05 = 165
SECRET_UNLOCK_06 = 166
SECRET_UNLOCK_07 = 167
SECRET_UNLOCK_08 = 168
SECRET_UNLOCK_09 = 169
SECRET_UNLOCK_10 = 170
SECRET_UNLOCK_11 = 171
SECRET_UNLOCK_12 = 172
SECRET_UNLOCK_13 = 173
SECRET_UNLOCK_14 = 174
SECRET_UNLOCK_15 = 175
SECRET_UNLOCK_16 = 176
SECRET_UNLOCK_17 = 177
SECRET_UNLOCK_18 = 178
SECRET_UNLOCK_19 = 179
SECRET_UNLOCK_20 = 180
SECRET_UNLOCK_21 = 181
SECRET_UNLOCK_22 = 182
CHEAT_ON_1 = 185
CHEAT_ON_2 = 186
CHEAT_ON_3 = 187
CHEAT_ON_4 = 188
CHEAT_ON_5 = 189
CHEAT_ON_6 = 190
CHEAT_ON_7 = 191
CHEAT_ON_8 = 192
CHEAT_ON_9 = 193
CHEAT_ON_10 = 194
CHEAT_ON_11 = 195
CHEAT_ON_12 = 196
CHEAT_ON_13 = 197
CHEAT_ON_14 = 198
CHEAT_ON_15 = 199
SKATER_UNLOCKED_DOOMGUY = 200
SECRET_UNLOCKED_01 = 201
SecretScripts = [ THPS3_secret_1 THPS3_secret_2 THPS3_secret_3 THPS3_secret_4 THPS3_secret_5 THPS3_secret_6 THPS3_secret_7 THPS3_secret_8 THPS3_secret_9 THPS3_secret_10 THPS3_secret_11 THPS3_secret_12 THPS3_secret_13 THPS3_secret_14 THPS3_secret_15 THPS3_secret_16 THPS3_secret_17 THPS3_secret_18 THPS3_secret_19 THPS3_secret_20 THPS3_secret_21 THPS3_secret_22 ]

script THPS3_secretScript_1
  LaunchLocalMessage id = LegealStuff "Darth Maul ® 2001 Lucasfilm Ltd and TM or ® as indicated. |All rights reserved. Used under authorization.|Courtesy of LucasArts Entertainment Company LLC" panel_message_legalmumbojumbo
  UnlockSecretChar S_Name = "Darth Maul" S_Flag = SKATER_UNLOCKED_MAUL
endscript

script THPS3_secretScript_2
  LaunchLocalMessage id = LegealStuff "Marvel and Wolverine: TM and ® 2001 Marvel Characters, Inc. |All rights reserved. www.marvel.com. " panel_message_legalmumbojumbo
  UnlockSecretChar S_Name = "Wolverine" S_Flag = SKATER_UNLOCKED_WOLVERINE
endscript

script THPS3_secretScript_3
  UnlockSecretLvl S_Name = "Warehouse" S_Flag = LEVEL_UNLOCKED_WAREHOUSE
endscript

script THPS3_secretScript_4
  UnlockSecretChar S_Name = "Officer Dick" S_Flag = SKATER_UNLOCKED_DICK
endscript

script THPS3_secretScript_5
  UnlockSecretChar S_Name = "Private Carrera" S_Flag = SKATER_UNLOCKED_CARRERA
endscript

script THPS3_secretScript_6
  UnlockSecretLvl S_Name = "Burnside" S_Flag = LEVEL_UNLOCKED_BURNSIDE
endscript

script THPS3_secretScript_7
  UnlockSecretChar S_Name = "Ollie the Bum" S_Flag = SKATER_UNLOCKED_BUM
endscript

script THPS3_secretScript_8
  UnlockSecretChar S_Name = "Kelly Slater" S_Flag = SKATER_UNLOCKED_SLATER
endscript

script THPS3_secretScript_9
  UnlockSecretLvl S_Name = "Roswell" S_Flag = LEVEL_UNLOCKED_ROSWELL
endscript

script THPS3_secretScript_10
  UnlockSecretChar S_Name = "The Demoness" S_Flag = SKATER_UNLOCKED_DEMONESS
endscript

script THPS3_secretScript_11
  LaunchLocalMessage id = LegealStuff "                 Cheat Menu Unlocked                 | Pause Game to Access Cheats" panel_message_CheatsUnlocked
  UnlockSecretCheat S_Name = "SNOWBOARD MODE" S_Flag = CHEAT_UNLOCKED_1
endscript

script THPS3_secretScript_12
  UnlockSecretCheat S_Name = "ALWAYS SPECIAL|MODE" S_Flag = CHEAT_UNLOCKED_2
endscript

script THPS3_secretScript_13
  UnlockSecretCheat S_Name = "PERFECT RAIL| BALANCE MODE" S_Flag = CHEAT_UNLOCKED_3
endscript

script THPS3_secretScript_14
  UnlockSecretCheat S_Name = "SUPER STATS| MODE" S_Flag = CHEAT_UNLOCKED_4
endscript

script THPS3_secretScript_15
  UnlockSecretCheat S_Name = "GIANT MODE" S_Flag = CHEAT_UNLOCKED_5
endscript

script THPS3_secretScript_16
  UnlockSecretCheat S_Name = "SLOWMO MODE" S_Flag = CHEAT_UNLOCKED_6
endscript

script THPS3_secretScript_17
  UnlockSecretCheat S_Name = "PERFECT MANUAL| BALANCE MODE" S_Flag = CHEAT_UNLOCKED_7
endscript

script THPS3_secretScript_18
  UnlockSecretCheat S_Name = "TINY MODE" S_Flag = CHEAT_UNLOCKED_8
endscript

script THPS3_secretScript_19
  UnlockSecretCheat S_Name = "MOON PHYSICS| MODE" S_Flag = CHEAT_UNLOCKED_9
endscript

script THPS3_secretScript_20
  UnlockSecretCheat S_Name = "EXPERT MODE" S_Flag = CHEAT_UNLOCKED_10
endscript

script THPS3_secretScript_21
  UnlockSecretChar S_Name = "Neversoft Eyeball" S_Flag = SKATER_UNLOCKED_EYEBALL
endscript

script THPS3_secretScript_22
  UnlockSecretCheat S_Name = "FIRST PERSON| MODE" S_Flag = CHEAT_UNLOCKED_11
  if GetGlobalFlag flag = ALL_SECRETS_UNLOCKED
  else
    SetGlobalFlag flag = ALL_SECRETS_UNLOCKED
    SetGlobalFlag flag = SKATESHOP_JUST_GOT_EVERYTHING
    LaunchLocalMessage id = GoalText "Entire Game" panel_message_newlevel
    LaunchLocalMessage id = complete "COMPLETE!!!" panel_message_newlevelopen
    PlayStream unlock_secret
    wait 2.5 seconds
    THPS3_secret_Everything
  endif
endscript

script THPS3_secretScript_23
  LaunchLocalMessage id = LegealStuff "DOOM character (c) 1999-2002 Id Software, Inc. Used under license. All Rights Reserved." panel_message_legalmumbojumbo
  UnlockSecretChar S_Name = "DOOM Guy" S_Flag = SKATER_UNLOCKED_DOOMGUY
endscript

script THPS3_secret_Everything
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage id = BottomInfo " " panel_message_XtoContinue
endscript
panel_message_legalmumbojumbo = { font = "small.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (330, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (330, 360) alpha = 0 scale = 0.2 time = 0.5 }
    { pos = (330, 360) alpha = 128 scale = 0.81 time = 0.7 }
    { time = 1000000000 } ] }
panel_message_CheatsUnlocked = { font = "small.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 } ] key_points = [ { pos = (330, 360) alpha = 0 scale = 0.2 time = 0 }
    { pos = (330, 360) alpha = 0 scale = 0.2 time = 0.5 }
    { pos = (330, 360) alpha = 128 scale = 1.21 time = 0.7 }
    { time = 1000000000 } ] }
panel_message_newThing01 = { font = "large.fnt" just = (0, 0) dims = (320, 112) colors = [ { (90, 90, 90) alpha = 70 } ] key_points = [ { pos = (320, 212) alpha = 0 scale = 0.3 time = 0 }
    { pos = (320, 200) alpha = 128 scale = 1.21 time = 0.4 }
    { time = 1000000000 } ] }
panel_message_newThing02 = { font = "large.fnt" just = (0, 0) dims = (320, 112) colors = [ { (90, 90, 90) alpha = 70 } ] key_points = [ { pos = (320, 200) alpha = 128 scale = 1.21 time = 0 }
    { pos = (320, 212) alpha = 0 scale = 0.3 time = 0.4 } ] }
panel_message_newlevelThingName01 = { font = "title.fnt" just = (0, 0) dims = (320, 112) colors = [ { (20, 20, 120) alpha = 70 } ] key_points = [ { pos = (335, 260) alpha = 0 scale = 0.3 time = 0 }
    { pos = (335, 260) alpha = 0 scale = 0.3 time = 0.5 }
    { pos = (335, 265) alpha = 20 scale = 2.05 time = 0.1 }
    { pos = (335, 272) alpha = 130 scale = 1.81 time = 0.4 }
    { time = 1000000000 } ] }
panel_message_newlevelThingName02 = { font = "title.fnt" just = (0, 0) dims = (320, 112) colors = [ { (20, 20, 120) alpha = 70 } ] key_points = [ { pos = (335, 272) alpha = 130 scale = 1.81 time = 0 }
    { pos = (335, 260) alpha = 0 scale = 0.3 time = 0.5 } ] }

script UnlockSecretChar S_Name = "Chad" S_Flag = SKATER_UNLOCKED_MAUL
  PauseStream 0
  PlayStream unlock_secret
  LaunchLocalMessage id = GoalText "Secret Character Unlocked" panel_message_newThing01
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName01
  SetGlobalFlag flag = <S_Flag>
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_SECRET_CHAR
  wait 0.5 seconds
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage id = GoalText "Secret Character Unlocked" panel_message_newThing02
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName02
  LaunchLocalMessage id = BottomInfo " " panel_message_XtoContinue
  LaunchLocalMessage id = LegealStuff " " panel_message_legalmumbojumbo
  wait 0.5 seconds
  PlayCredits
endscript

script UnlockSecretCheat S_Name = "Poo Poo" S_Flag = CHEAT_UNLOCKED_1
  PauseStream 0
  PlayStream unlock_secret
  LaunchLocalMessage id = GoalText "Secret Cheat Unlocked" panel_message_newThing01
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName01
  SetGlobalFlag flag = <S_Flag>
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_SECRET_CHEAT
  wait 0.5 seconds
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage id = GoalText "Secret Cheat Unlocked" panel_message_newThing02
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName02
  LaunchLocalMessage id = BottomInfo " " panel_message_XtoContinue
  LaunchLocalMessage id = LegealStuff " " panel_message_legalmumbojumbo
  LaunchLocalMessage id = LegealStuff " " panel_message_CheatsUnlocked
  wait 0.5 seconds
  PlayCredits
endscript

script UnlockSecretLvl S_Name = "Hollywood" S_Flag = LEVEL_UNLOCKED_BURNSIDE
  PauseStream 0
  PlayStream unlock_secret
  LaunchLocalMessage id = GoalText "Secret Level Unlocked" panel_message_newThing01
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName01
  SetGlobalFlag flag = <S_Flag>
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_SECRET_LEVEL
  wait 0.5 seconds
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage id = GoalText "Secret Level Unlocked" panel_message_newThing02
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName02
  LaunchLocalMessage id = BottomInfo " " panel_message_XtoContinue
  LaunchLocalMessage id = LegealStuff " " panel_message_legalmumbojumbo
  wait 0.5 seconds
  PlayCredits
endscript

script UnlockNormalLvl S_Name = "Hollywood" S_Flag = LEVEL_UNLOCKED_FOUNDRY
  SetGlobalFlag flag = JUST_UNLOCKED_LEVEL
  PauseStream 0
  PlayStream unlock_level
  LaunchLocalMessage id = GoalText "New Level Unlocked" panel_message_newThing01
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName01
  SetGlobalFlag flag = <S_Flag>
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_LEVEL
  wait 0.5 seconds
  LaunchPanelMessage " " Properties = Panel_Message_XMessage
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage id = GoalText "New Level Unlocked" panel_message_newThing02
  LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName02
  LaunchLocalMessage id = BottomInfo " " panel_message_XtoContinue
  LaunchLocalMessage id = LegealStuff " " panel_message_legalmumbojumbo
  wait 0.5 seconds
endscript

script UnlockMovie S_Name = "Hollywood" S_Flag = MOVIE_UNLOCKED_HAWK S_Movie = "movies\kflip"
  if GetGlobalFlag flag = <S_Flag>
  else
    PauseStream 0
    PlayStream unlock_level
    LaunchLocalMessage id = GoalText "New Movie Unlocked" panel_message_newThing01
    LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName01
    SetGlobalFlag flag = <S_Flag>
    wait 1 seconds
    LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
    WaitForControllerPressedX
    LaunchLocalMessage id = GoalText "New Movie Unlocked" panel_message_newThing02
    LaunchLocalMessage id = complete <S_Name> panel_message_newlevelThingName02
    LaunchLocalMessage id = BottomInfo " " panel_message_XtoContinue
    LaunchLocalMessage id = LegealStuff " " panel_message_legalmumbojumbo
    wait 0.5 seconds
    if CD
      ingame_play_movie <S_Movie>
    endif
    wait 0.5 seconds
  endif
endscript

script PlayCredits
  SetGlobalFlag flag = SHOW_CREDITS
endscript

script MovieSpawn
  SpawnScript SpawnedMovie
endscript

script SpawnedMovie
  ingame_play_movie "movies\kflip"
endscript

script UnlockMovie_Pro
  if ProfileEquals is_custom
    UnlockMovie S_Name = "Pro Bails 2" S_Flag = MOVIE_UNLOCKED_CAS S_Movie = "movies\bails02"
  endif
  if ProfileEquals is_named = hawk
    UnlockMovie S_Name = "Tony Hawk" S_Flag = MOVIE_UNLOCKED_HAWK S_Movie = "movies\tony"
  endif
  if ProfileEquals is_named = caballero
    UnlockMovie S_Name = "Steve Caballero" S_Flag = MOVIE_UNLOCKED_CABALLERO S_Movie = "movies\cab"
  endif
  if ProfileEquals is_named = campbell
    UnlockMovie S_Name = "Kareem Campbell" S_Flag = MOVIE_UNLOCKED_CAMPBELL S_Movie = "movies\kareem"
  endif
  if ProfileEquals is_named = glifberg
    UnlockMovie S_Name = "Rune Glifberg" S_Flag = MOVIE_UNLOCKED_GLIFBERG S_Movie = "movies\rune"
  endif
  if ProfileEquals is_named = koston
    UnlockMovie S_Name = "Eric Koston" S_Flag = MOVIE_UNLOCKED_KOSTON S_Movie = "movies\koston"
  endif
  if ProfileEquals is_named = lasek
    UnlockMovie S_Name = "Bucky Lasek" S_Flag = MOVIE_UNLOCKED_LASEK S_Movie = "movies\bucky"
  endif
  if ProfileEquals is_named = margera
    UnlockMovie S_Name = "Bam Margera" S_Flag = MOVIE_UNLOCKED_MARGERA S_Movie = "movies\bam"
  endif
  if ProfileEquals is_named = mullen
    UnlockMovie S_Name = "Rodney Mullen" S_Flag = MOVIE_UNLOCKED_MULLEN S_Movie = "movies\mullen"
  endif
  if ProfileEquals is_named = muska
    UnlockMovie S_Name = "Chad Muska" S_Flag = MOVIE_UNLOCKED_MUSKA S_Movie = "movies\muska"
  endif
  if ProfileEquals is_named = reynolds
    UnlockMovie S_Name = "Andrew Reynolds" S_Flag = MOVIE_UNLOCKED_REYNOLDS S_Movie = "movies\andrew"
  endif
  if ProfileEquals is_named = rowley
    UnlockMovie S_Name = "Geoff Rowley" S_Flag = MOVIE_UNLOCKED_ROWLEY S_Movie = "movies\rowley"
  endif
  if ProfileEquals is_named = steamer
    UnlockMovie S_Name = "Elissa Steamer" S_Flag = MOVIE_UNLOCKED_STEAMER S_Movie = "movies\elissa"
  endif
  if ProfileEquals is_named = thomas
    UnlockMovie S_Name = "Jamie Thomas" S_Flag = MOVIE_UNLOCKED_THOMAS S_Movie = "movies\thomas"
  endif
  if ProfileEquals is_named = carrera
    UnlockMovie S_Name = "Kickflip |Contest" S_Flag = MOVIE_UNLOCKED_CARRERA S_Movie = "movies\kflip"
  endif
  if ProfileEquals is_named = Dick
    UnlockMovie S_Name = "Pro Retro" S_Flag = MOVIE_UNLOCKED_DICK S_Movie = "movies\proret"
  endif
  if ProfileEquals is_named = demoness
    UnlockMovie S_Name = "Neversoft |Retro" S_Flag = MOVIE_UNLOCKED_DEMONESS S_Movie = "movies\nsret"
  endif
  if ProfileEquals is_named = wolverine
    UnlockMovie S_Name = "Neversoft |Makes" S_Flag = MOVIE_UNLOCKED_WOLVERINE S_Movie = "movies\nsmakes"
  endif
  if ProfileEquals is_named = maul
    UnlockMovie S_Name = "Neversoft |Bails" S_Flag = MOVIE_UNLOCKED_JEDIKNIGHT S_Movie = "movies\nsbails"
  endif
  if ProfileEquals is_named = slater
    UnlockMovie S_Name = "Slater" S_Flag = MOVIE_UNLOCKED_SLATER S_Movie = "movies\slater"
  endif
  if ProfileEquals is_named = ollie
    UnlockMovie S_Name = "Neversoft |Friends" S_Flag = MOVIE_UNLOCKED_BUM S_Movie = "movies\friends"
  endif
endscript

script EndRun_CheckForLevelsOpen
  KillMessages
  UnSetGlobalFlag flag = JUST_UNLOCKED_LEVEL
  if GoalsGreaterThan 2
    if GetGlobalFlag flag = LEVEL_UNLOCKED_CANADA
    else
      UnlockNormalLvl S_Name = "CANADA" S_Flag = LEVEL_UNLOCKED_CANADA
    endif
    if GoalsGreaterThan 9
      if GetGlobalFlag flag = LEVEL_UNLOCKED_RIO
      else
        UnlockNormalLvl S_Name = "RIO" S_Flag = LEVEL_UNLOCKED_RIO
      endif
      if MedalsGreaterThan 0
        if GetGlobalFlag flag = LEVEL_UNLOCKED_SUBURBIA
        else
          UnlockNormalLvl S_Name = "SUBURBIA" S_Flag = LEVEL_UNLOCKED_SUBURBIA
        endif
        if GoalsGreaterThan 17
          if GetGlobalFlag flag = LEVEL_UNLOCKED_AIRPORT
          else
            UnlockNormalLvl S_Name = "The Airport" S_Flag = LEVEL_UNLOCKED_AIRPORT
          endif
          if GoalsGreaterThan 25
            if GetGlobalFlag flag = LEVEL_UNLOCKED_SKATERISLAND
            else
              UnlockNormalLvl S_Name = "Skater Island" S_Flag = LEVEL_UNLOCKED_SKATERISLAND
            endif
            if MedalsGreaterThan 1
              if GetGlobalFlag flag = LEVEL_UNLOCKED_LOSANGELES
              else
                UnlockNormalLvl S_Name = "LOS ANGELES" S_Flag = LEVEL_UNLOCKED_LOSANGELES
              endif
              if GoalsGreaterThan 34
                if GetGlobalFlag flag = LEVEL_UNLOCKED_TOKYO
                else
                  UnlockNormalLvl S_Name = "Tokyo" S_Flag = LEVEL_UNLOCKED_TOKYO
                endif
                if MedalsGreaterThan 2
                  if GoalsGreaterThan 44
                  endif
                endif
              endif
            endif
          endif
        endif
      endif
    endif
  endif
endscript

script MidLevel_GoalCheck
  DisablePause
  TogglePanel 0
  KillGoalMessage id = GoalText " "
  KillGoalMessage id = complete " "
  PlaySound goaldone
  if GotParam from_pause_menu
    PauseGame
  endif
  GoalViewAllGoalCompleteCheck from_list
  wait 1 frame
  ListAllGoals
  WaitForControllerPressedXNoDebounce
  EnablePause
  LaunchLocalMessage id = BottomInfo " " panel_message_XtoContinue
  TogglePanel 1
  ClearEndMessages
  wait 2 frames
  if GotParam from_pause_menu
    UnPauseGame
    SwitchToMenu menu = game_menu
  endif
endscript

script ListAllGoals
  PlaySound goaldone
  if CareerLevelIs LevelNum_Foundry
    ListAllGoalsLevelSpecific FoundryGoalParams FoundryScoreParams
  endif
  if CareerLevelIs LevelNum_Canada
    ListAllGoalsLevelSpecific CanadaGoalParams CanadaScoreParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    ListAllGoalsLevelSpecific SuburbiaGoalParams SuburbiaScoreParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    ListAllGoalsLevelSpecific SICompParams
  endif
  if CareerLevelIs LevelNum_Airport
    ListAllGoalsLevelSpecific AirportGoalParams AirportScoreParams
  endif
  if CareerLevelIs LevelNum_Rio
    ListAllGoalsLevelSpecific RioCompParams
  endif
  if CareerLevelIs LevelNum_LA
    ListAllGoalsLevelSpecific LosAngelesGoalParams LosAngelesScoreParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    ListAllGoalsLevelSpecific TokyoGoalParams
  endif
  if CareerLevelIs LevelNum_Ship
    ListAllGoalsLevelSpecific ShipGoalParams ShipScoreParams
  endif
endscript

script ListAllGoalsLevelSpecific
  launchconsolemessage "checking lists"
  if JustGotGoal goal = GOAL_HIGHSCORE
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot01 GoalText = <Goal_HighScore_Text> id = mess01 pos = (0, 0) }
  else
    if GetGoal goal = GOAL_HIGHSCORE
      DisplayKilledGoal GoalText = <Goal_HighScore_Text> id = mess01 pos = (0, 0)
    else
      DisplayTodoGoal GoalText = <Goal_HighScore_Text> id = mess01 pos = (0, 0)
    endif
  endif
  if JustGotGoal goal = GOAL_PROSCORE
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot02 GoalText = <Goal_ProScore_Text> id = mess02 pos = (0, 35) }
  else
    if GetGoal goal = GOAL_PROSCORE
      DisplayKilledGoal GoalText = <Goal_ProScore_Text> id = mess02 pos = (0, 35)
    else
      DisplayTodoGoal GoalText = <Goal_ProScore_Text> id = mess02 pos = (0, 35)
    endif
  endif
  if JustGotGoal goal = GOAL_SICKSCORE
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot03 GoalText = <Goal_SickScore_Text> id = mess03 pos = (0, 70) }
  else
    if GetGoal goal = GOAL_SICKSCORE
      DisplayKilledGoal GoalText = <Goal_SickScore_Text> id = mess03 pos = (0, 70)
    else
      DisplayTodoGoal GoalText = <Goal_SickScore_Text> id = mess03 pos = (0, 70)
    endif
  endif
  if JustGotGoal goal = GOAL_SKATE
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot04 GoalText = <Goal_SKATE_Text> id = mess04 pos = (0, 105) }
  else
    if GetGoal goal = GOAL_SKATE
      DisplayKilledGoal GoalText = <Goal_SKATE_Text> id = mess04 pos = (0, 105)
    else
      DisplayTodoGoal GoalText = <Goal_SKATE_Text> id = mess04 pos = (0, 105)
    endif
  endif
  if JustGotGoal goal = GOAL_TAPE
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot05 GoalText = <Goal_Tape_Text> id = mess05 pos = (0, 140) }
  else
    if GetGoal goal = GOAL_TAPE
      DisplayKilledGoal GoalText = <Goal_Tape_Text> id = mess05 pos = (0, 140)
    else
      DisplayTodoGoal GoalText = <Goal_Tape_Text> id = mess05 pos = (0, 140)
    endif
  endif
  if JustGotGoal goal = GOAL_TRICKSPOT
    if ProfileEquals trickstyle = vert
      SpawnScript DisplayJustGotGoal params = { format = SideListJustGot06 GoalText = <Goal_TrickspotVert_Text> id = mess06 pos = (0, 175) }
    else
      SpawnScript DisplayJustGotGoal params = { format = SideListJustGot06 GoalText = <Goal_TrickspotStreet_Text> id = mess06 pos = (0, 175) }
    endif
  else
    if GetGoal goal = GOAL_TRICKSPOT
      if ProfileEquals trickstyle = vert
        DisplayKilledGoal GoalText = <Goal_TrickspotVert_Text> id = mess06 pos = (0, 175)
      else
        DisplayKilledGoal GoalText = <Goal_TrickspotStreet_Text> id = mess06 pos = (0, 175)
      endif
    else
      if ProfileEquals trickstyle = vert
        DisplayTodoGoal GoalText = <Goal_TrickspotVert_Text> id = mess06 pos = (0, 175)
      else
        DisplayTodoGoal GoalText = <Goal_TrickspotStreet_Text> id = mess06 pos = (0, 175)
      endif
    endif
  endif
  if JustGotGoal goal = GOAL_SCRIPTED3
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot07 GoalText = <Goal_Scripted3_Text> id = mess07 pos = (0, 210) }
  else
    if GetGoal goal = GOAL_SCRIPTED3
      DisplayKilledGoal GoalText = <Goal_Scripted3_Text> id = mess07 pos = (0, 210)
    else
      DisplayTodoGoal GoalText = <Goal_Scripted3_Text> id = mess07 pos = (0, 210)
    endif
  endif
  if JustGotGoal goal = GOAL_SCRIPTED1
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot08 GoalText = <Goal_Scripted1_Text> id = mess08 pos = (0, 245) }
  else
    if GetGoal goal = GOAL_SCRIPTED1
      DisplayKilledGoal GoalText = <Goal_Scripted1_Text> id = mess08 pos = (0, 245)
    else
      DisplayTodoGoal GoalText = <Goal_Scripted1_Text> id = mess08 pos = (0, 245)
    endif
  endif
  if JustGotGoal goal = GOAL_SCRIPTED2
    SpawnScript DisplayJustGotGoal params = { format = SideListJustGot09 GoalText = <Goal_Scripted2_Text> id = mess09 pos = (0, 280) }
  else
    if GetGoal goal = GOAL_SCRIPTED2
      DisplayKilledGoal GoalText = <Goal_Scripted2_Text> id = mess09 pos = (0, 280)
    else
      DisplayTodoGoal GoalText = <Goal_Scripted2_Text> id = mess09 pos = (0, 280)
    endif
  endif
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
endscript

script GoalViewAllGoalCompleteCheck
  if GotParam from_list
    launchconsolemessage "From List"
  endif
  if GotParam From_movies
  endif
  if GetGoal goal = GOAL_SCRIPTED3
    if GetGoal goal = GOAL_SCRIPTED2
      if GetGoal goal = GOAL_SCRIPTED1
        if GetGoal goal = GOAL_TRICKSPOT
          if GetGoal goal = GOAL_TAPE
            if GetGoal goal = GOAL_SKATE
              if GetGoal goal = GOAL_SICKSCORE
                if GetGoal goal = GOAL_PROSCORE
                  if GetGoal goal = GOAL_HIGHSCORE
                    SetFlag flag = GOAL_ALL_GOALS
                    if GotParam From_movies
                      ClearEndMessages
                      EndGameGoalsCompleteLevelSpecific From_movies
                    else
                      SetFlag flag = GOAL_ALL_GOALS
                      ClearEndMessages
                      EndGameGoalsComplete
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
endscript

script EndGameGoalsCompleteLevelSpecific
  if CareerLevelIs LevelNum_Foundry
    EndGameGoalsComplete FoundryGoalParams FoundryScoreParams
  endif
  if CareerLevelIs LevelNum_Canada
    EndGameGoalsComplete CanadaGoalParams CanadaScoreParams
  endif
  if CareerLevelIs LevelNum_Suburbia
    EndGameGoalsComplete SuburbiaGoalParams SuburbiaScoreParams
  endif
  if CareerLevelIs LevelNum_SkaterIsland
    EndGameGoalsComplete SICompParams
  endif
  if CareerLevelIs LevelNum_Airport
    EndGameGoalsComplete AirportGoalParams AirportScoreParams
  endif
  if CareerLevelIs LevelNum_Rio
    EndGameGoalsComplete RioCompParams
  endif
  if CareerLevelIs LevelNum_LA
    EndGameGoalsComplete LosAngelesGoalParams LosAngelesScoreParams
  endif
  if CareerLevelIs LevelNum_Tokyo
    EndGameGoalsComplete TokyoGoalParams
  endif
  if CareerLevelIs LevelNum_Ship
    EndGameGoalsComplete ShipGoalParams ShipScoreParams
  endif
endscript

script EndGameGoalsComplete
  wait 1 gameframe
  if GotParam From_movies
    PlaySkaterCamAnim skater = 0 name = <CameraStart>
    SetSkaterCamAnimSkippable skater = 0 skippable = 1
    begin
      if SkaterCamAnimFinished skater = 0 name = <CameraStart>
        SwitchToMenu menu = end_run_menu
        break
      endif
      wait 1 gameframe
    repeat
  endif
  PlaySound goaldone Vol = 150
  LaunchLocalMessage id = complete "All Goals| Complete" panel_message_specialTrick01
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  wait 0.1 seconds
  WaitForControllerPressedXNoDebounce
  LaunchLocalMessage id = complete "All Goals| Complete" panel_message_specialTrick02
  wait 0.5 seconds
endscript
panel_message_specialTrick01 = { font = "large.fnt" just = (0, 0) dims = (320, 112) colors = [ { (90, 90, 90) alpha = 70 } ] key_points = [ { pos = (320, 200) alpha = 0 scale = 0.3 time = 0 }
    { pos = (320, 205) alpha = 20 scale = 1.05 time = 0.1 }
    { pos = (320, 209) alpha = 50 scale = 1.95 time = 0.2 }
    { pos = (320, 211) alpha = 70 scale = 1.3 time = 0.3 }
    { pos = (320, 212) alpha = 130 scale = 1.21 time = 0.4 }
    { time = 1000000000 } ] }
panel_message_specialTrick02 = { font = "large.fnt" just = (0, 0) dims = (320, 112) colors = [ { (90, 90, 90) alpha = 70 } ] key_points = [ { pos = (320, 214) alpha = 100 scale = 1.35 time = 0 }
    { pos = (320, 217) alpha = 40 scale = 0.55 time = 0.3 }
    { pos = (320, 222) alpha = 0 scale = 0.25 time = 0.4 } ] }

script Goal_AwardTrickSlot
  PlaySound goaldone Vol = 150
  LaunchLocalMessage id = GoalText "New Special Trick Slot|Unlocked" panel_message_specialTrick01
  AwardSpecialTrickSlot
  wait 0.5 seconds
  LaunchLocalMessage id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage id = GoalText "New Special Trick Slot|Unlocked" panel_message_specialTrick02
  wait 0.5 seconds
endscript

script Goal_CheckProVideoUnlock
  PauseSkaters
  if MedalsGreaterThan 2
    if GetGlobalFlag flag = MOVIE_UNLOCKED_PRO_BAILS1
    else
      SetGlobalFlag flag = SKATESHOP_JUST_GOT_3_MEDALS
      UnlockMovie S_Name = "Pro Bails" S_Flag = MOVIE_UNLOCKED_PRO_BAILS1 S_Movie = "movies\bails01"
    endif
  endif
  if GetGlobalFlag flag = GOT_GOLD_RIO
    if GetGlobalFlag flag = GOT_GOLD_SI
      if GetGlobalFlag flag = GOT_GOLD_TOKYO
        UnlockMovie_Pro
      endif
    endif
  endif
  if MedalsGreaterThan 2
    if GetGlobalFlag flag = LEVEL_UNLOCKED_SHIP
    else
      UnlockNormalLvl S_Name = "The Cruise Ship" S_Flag = LEVEL_UNLOCKED_SHIP
      SetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
    endif
  endif
endscript

script Goal_UnlockingSecrets
  if GetGlobalFlag flag = GOT_GOLD_RIO
    if GetGlobalFlag flag = GOT_GOLD_SI
      if GetGlobalFlag flag = GOT_GOLD_TOKYO
        if GoalsGreaterThan 53
          if ProfileEquals is_custom
            if GetGlobalFlag flag = SPECIAL_CAS_CAREER_DONE
            else
              SetGlobalFlag flag = SPECIAL_CAS_CAREER_DONE
              UnlockNextSecret
              ExecuteNextSecret
            endif
          else
            if GetGlobalFlag flag = UNLOCKED_SECRET
            else
              SetGlobalFlag flag = UNLOCKED_SECRET
              UnlockNextSecret
              ExecuteNextSecret
            endif
          endif
        endif
      endif
    endif
  endif
endscript

script KillGoalMessage
  ChangeLocalMessage <...> { key_points = [ { alpha = 0 time = 0 } ] }
endscript

script ClearEndMessages
  KillGoalMessage id = mess01
  KillGoalMessage id = mess02
  KillGoalMessage id = mess03
  KillGoalMessage id = mess04
  KillGoalMessage id = mess05
  KillGoalMessage id = mess06
  KillGoalMessage id = mess07
  KillGoalMessage id = mess08
  KillGoalMessage id = mess09
  KillGoalMessage id = ScreenTitle
  KillGoalMessage id = BottomInfo
  KillGoalMessage id = complete
endscript

script ClearEndMessagesSlowly
  KillGoalMessage id = mess01
  wait 0.01 second
  KillGoalMessage id = mess02
  wait 0.01 second
  KillGoalMessage id = mess03
  wait 0.01 second
  KillGoalMessage id = mess04
  wait 0.01 second
  KillGoalMessage id = mess05
  wait 0.01 second
  KillGoalMessage id = mess06
  wait 0.01 second
  KillGoalMessage id = mess07
  wait 0.01 second
  KillGoalMessage id = mess08
  wait 0.01 second
  KillGoalMessage id = mess09
  wait 0.01 second
  KillGoalMessage id = ScreenTitle
  wait 0.01 second
  KillGoalMessage id = BottomInfo
  wait 0.01 second
  KillGoalMessage id = complete
  wait 0.01 second
endscript

script WaitForSkippable
  wait 1 second
endscript

script DebounceForSkippable
  Debounce X time = 1
endscript

script THPS3_secret_1
  SetGlobalFlag flag = SECRET_UNLOCK_01
endscript

script THPS3_secret_2
  SetGlobalFlag flag = SECRET_UNLOCK_02
endscript

script THPS3_secret_3
  SetGlobalFlag flag = SECRET_UNLOCK_03
endscript

script THPS3_secret_4
  SetGlobalFlag flag = SECRET_UNLOCK_04
endscript

script THPS3_secret_5
  SetGlobalFlag flag = SECRET_UNLOCK_05
endscript

script THPS3_secret_6
  SetGlobalFlag flag = SECRET_UNLOCK_06
endscript

script THPS3_secret_7
  SetGlobalFlag flag = SECRET_UNLOCK_07
endscript

script THPS3_secret_8
  SetGlobalFlag flag = SECRET_UNLOCK_08
endscript

script THPS3_secret_9
  SetGlobalFlag flag = SECRET_UNLOCK_09
endscript

script THPS3_secret_10
  SetGlobalFlag flag = SECRET_UNLOCK_10
endscript

script THPS3_secret_11
  SetGlobalFlag flag = SECRET_UNLOCK_11
endscript

script THPS3_secret_12
  SetGlobalFlag flag = SECRET_UNLOCK_12
endscript

script THPS3_secret_13
  SetGlobalFlag flag = SECRET_UNLOCK_13
endscript

script THPS3_secret_14
  SetGlobalFlag flag = SECRET_UNLOCK_14
endscript

script THPS3_secret_15
  SetGlobalFlag flag = SECRET_UNLOCK_15
endscript

script THPS3_secret_16
  SetGlobalFlag flag = SECRET_UNLOCK_16
endscript

script THPS3_secret_17
  SetGlobalFlag flag = SECRET_UNLOCK_17
endscript

script THPS3_secret_18
  SetGlobalFlag flag = SECRET_UNLOCK_18
endscript

script THPS3_secret_19
  SetGlobalFlag flag = SECRET_UNLOCK_19
endscript

script THPS3_secret_20
  SetGlobalFlag flag = SECRET_UNLOCK_20
endscript

script THPS3_secret_21
  SetGlobalFlag flag = SECRET_UNLOCK_21
endscript

script THPS3_secret_22
  SetGlobalFlag flag = SECRET_UNLOCK_22
endscript

script ClearSecretFlags
  UnSetGlobalFlag flag = SECRET_UNLOCK_01
  UnSetFlag flag = SECRET_UNLOCK_02
  UnSetFlag flag = SECRET_UNLOCK_03
  UnSetFlag flag = SECRET_UNLOCK_04
  UnSetFlag flag = SECRET_UNLOCK_05
  UnSetFlag flag = SECRET_UNLOCK_06
  UnSetFlag flag = SECRET_UNLOCK_07
  UnSetFlag flag = SECRET_UNLOCK_08
  UnSetFlag flag = SECRET_UNLOCK_09
  UnSetFlag flag = SECRET_UNLOCK_10
  UnSetFlag flag = SECRET_UNLOCK_11
  UnSetFlag flag = SECRET_UNLOCK_12
  UnSetFlag flag = SECRET_UNLOCK_13
  UnSetFlag flag = SECRET_UNLOCK_14
  UnSetFlag flag = SECRET_UNLOCK_15
  UnSetFlag flag = SECRET_UNLOCK_16
  UnSetFlag flag = SECRET_UNLOCK_17
  UnSetFlag flag = SECRET_UNLOCK_18
  UnSetFlag flag = SECRET_UNLOCK_19
  UnSetFlag flag = SECRET_UNLOCK_20
endscript

script ExecuteNextSecret
  if GetGlobalFlag flag = SECRET_UNLOCK_20
    ExecuteNextSecret_20to29
  else
    if GetGlobalFlag flag = SECRET_UNLOCK_10
      ExecuteNextSecret_10to19
    else
      ExecuteNextSecret_1to9
    endif
  endif
endscript

script ExecuteNextSecret_1to9
  if GetGlobalFlag flag = SECRET_UNLOCK_09
    THPS3_secretScript_9
  else
    if GetGlobalFlag flag = SECRET_UNLOCK_08
      THPS3_secretScript_8
    else
      if GetGlobalFlag flag = SECRET_UNLOCK_07
        THPS3_secretScript_7
      else
        if GetGlobalFlag flag = SECRET_UNLOCK_06
          THPS3_secretScript_6
        else
          if GetGlobalFlag flag = SECRET_UNLOCK_05
            THPS3_secretScript_5
          else
            if GetGlobalFlag flag = SECRET_UNLOCK_04
              THPS3_secretScript_4
            else
              if GetGlobalFlag flag = SECRET_UNLOCK_03
                THPS3_secretScript_3
              else
                if GetGlobalFlag flag = SECRET_UNLOCK_02
                  THPS3_secretScript_2
                else
                  if GetGlobalFlag flag = SECRET_UNLOCK_01
                    THPS3_secretScript_1
                  else
                  endif
                endif
              endif
            endif
          endif
        endif
      endif
    endif
  endif
endscript

script ExecuteNextSecret_10to19
  wait 5 frames
  if GetGlobalFlag flag = SECRET_UNLOCK_19
    THPS3_secretScript_19
  else
    if GetGlobalFlag flag = SECRET_UNLOCK_18
      THPS3_secretScript_18
    else
      if GetGlobalFlag flag = SECRET_UNLOCK_17
        THPS3_secretScript_17
      else
        if GetGlobalFlag flag = SECRET_UNLOCK_16
          THPS3_secretScript_16
        else
          if GetGlobalFlag flag = SECRET_UNLOCK_15
            THPS3_secretScript_15
          else
            if GetGlobalFlag flag = SECRET_UNLOCK_14
              THPS3_secretScript_14
            else
              if GetGlobalFlag flag = SECRET_UNLOCK_13
                THPS3_secretScript_13
              else
                if GetGlobalFlag flag = SECRET_UNLOCK_12
                  THPS3_secretScript_12
                else
                  if GetGlobalFlag flag = SECRET_UNLOCK_11
                    THPS3_secretScript_11
                  else
                    if GetGlobalFlag flag = SECRET_UNLOCK_10
                      THPS3_secretScript_10
                    else
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
endscript

script ExecuteNextSecret_20to29
  if GetGlobalFlag flag = SECRET_UNLOCK_22
    THPS3_secretScript_22
  else
    if GetGlobalFlag flag = SECRET_UNLOCK_21
      THPS3_secretScript_21
    else
      if GetGlobalFlag flag = SECRET_UNLOCK_20
        THPS3_secretScript_20
      else
      endif
    endif
  endif
endscript

script CreatePhotoGuy
  if IsCareerMode
    if GetGoal goal = GOAL_TRICKSPOT
    else
      if ProfileEquals trickstyle = vert
        if CareerLevelIs LevelNum_Foundry
          Create name = TRG_Photographer_Vert
        endif
        if CareerLevelIs LevelNum_Canada
          Create name = TRG_Photographer_Vert
        endif
        if CareerLevelIs LevelNum_Suburbia
          Create name = TRG_Photographer_Vert
        endif
        if CareerLevelIs LevelNum_Airport
          Create name = TRG_Photographer_Vert
        endif
        if CareerLevelIs LevelNum_LA
          Create name = TRG_Photographer_Vert
        endif
        if CareerLevelIs LevelNum_Ship
          Create name = TRG_Photographer_Vert
        endif
      endif
      if ProfileEquals trickstyle = street
        if CareerLevelIs LevelNum_Foundry
          Create name = TRG_Photographer_Street
        endif
        if CareerLevelIs LevelNum_Canada
          Create name = TRG_Photographer_Street
        endif
        if CareerLevelIs LevelNum_Suburbia
          Create name = TRG_Photographer_Street
        endif
        if CareerLevelIs LevelNum_Airport
          Create name = TRG_Photographer_Street
        endif
        if CareerLevelIs LevelNum_LA
          Create name = TRG_Photographer_Street
        endif
        if CareerLevelIs LevelNum_Ship
          Create name = TRG_Photographer_Street
        endif
      endif
    endif
  endif
endscript

script KillPhotoGuy
  if IsCareerMode
    Kill prefix = "TRG_Photographer_"
  endif
endscript
