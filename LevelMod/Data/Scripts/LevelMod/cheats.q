
CHEAT_SNOWBOARD = CHEAT_ON_1
CHEAT_ALWAYS_SPECIAL = CHEAT_ON_2
CHEAT_PERFECT_RAIL = CHEAT_ON_3
CHEAT_STATS_13 = CHEAT_ON_4
CHEAT_GIANT = CHEAT_ON_5
CHEAT_MATRIX = CHEAT_ON_6
CHEAT_PERFECT_MANUAL = CHEAT_ON_7
CHEAT_KID = CHEAT_ON_8
CHEAT_MOON = CHEAT_ON_9
CHEAT_SIM = CHEAT_ON_10
CHEAT_FIRST_PERSON = CHEAT_ON_11
CHEAT_NUMBER12 = CHEAT_ON_12

// initialize cheat menu
script cheat_menu_create
  CreateMenu cheat_menu
  AttachChild parent = contain1 child = cheat_menu
endscript

cheat_menu_shared = {
    type = textmenuelement
    text = "Foo"
    target = "ToggleCheatMenuLine"
}

cheat_menu = {
    type = verticalmenu
    Id = cheat_menu
    x = 125.0 y = 70.0 w = 380.0 h = 400.0
    just_center_x just_center_y blue_top
    eventhandler = { type = showeventhandler target = "cheat_menu_refresh" }
    children = [
        { type = textmenuelement auto_id text = "Cheat Menu" static dont_gray drawer = title }
        { cheat_menu_shared Id = menu_cheat_1 Params = { cheat = CHEAT_ON_1 unlocked = CHEAT_UNLOCKED_1 } }
        { cheat_menu_shared Id = menu_cheat_2 Params = { cheat = CHEAT_ON_2 unlocked = CHEAT_UNLOCKED_2 } }
        { cheat_menu_shared Id = menu_cheat_3 Params = { cheat = CHEAT_ON_3 unlocked = CHEAT_UNLOCKED_3 } }
        { cheat_menu_shared Id = menu_cheat_4 Params = { cheat = CHEAT_ON_4 unlocked = CHEAT_UNLOCKED_4 } }
        { cheat_menu_shared Id = menu_cheat_5 Params = { cheat = CHEAT_ON_5 unlocked = CHEAT_UNLOCKED_5 } }
        { cheat_menu_shared Id = menu_cheat_6 Params = { cheat = CHEAT_ON_6 unlocked = CHEAT_UNLOCKED_6 } }
        { cheat_menu_shared Id = menu_cheat_7 Params = { cheat = CHEAT_ON_7 unlocked = CHEAT_UNLOCKED_7 } }
        { cheat_menu_shared Id = menu_cheat_8 Params = { cheat = CHEAT_ON_8 unlocked = CHEAT_UNLOCKED_8 } }
        { cheat_menu_shared Id = menu_cheat_9 Params = { cheat = CHEAT_ON_9 unlocked = CHEAT_UNLOCKED_9 } }
        { cheat_menu_shared Id = menu_cheat_10 Params = { cheat = CHEAT_ON_10 unlocked = CHEAT_UNLOCKED_10 } }
        { cheat_menu_shared Id = menu_cheat_11 Params = { cheat = CHEAT_ON_11 unlocked = CHEAT_UNLOCKED_11 } }
        { type = textmenuelement Id = menu_cheat_done text = "Done" target = "go_back" Params = { Id = cheat_menu } }
    ]
}

// cheat menu entry click event handler
script ToggleCheatMenuLine
// expected params:
// cheat = cheat state to toggle
// unlocked = cheat unlock state flag
  if GetGlobalFlag flag = <unlocked>
    disable_replays

    printf "toggling flag......"

    if GetGlobalFlag flag = <cheat>
      printf "unsetting flag"
      UnsetGlobalFlag flag = <cheat>
    else
      printf "setting flag"
      SetGlobalFlag flag = <cheat>
    endif
  endif
  cheat_menu_refresh
endscript

// cheat_menu show event handler
script cheat_menu_refresh
  Cheat_menu_refresh_line cheat = CHEAT_ON_1 Id = menu_cheat_1 unlocked = CHEAT_UNLOCKED_1 
  Cheat_menu_refresh_line cheat = CHEAT_ON_2 Id = menu_cheat_2 unlocked = CHEAT_UNLOCKED_2
  Cheat_menu_refresh_line cheat = CHEAT_ON_3 Id = menu_cheat_3 unlocked = CHEAT_UNLOCKED_3
  Cheat_menu_refresh_line cheat = CHEAT_ON_4 Id = menu_cheat_4 unlocked = CHEAT_UNLOCKED_4
  Cheat_menu_refresh_line cheat = CHEAT_ON_5 Id = menu_cheat_5 unlocked = CHEAT_UNLOCKED_5
  Cheat_menu_refresh_line cheat = CHEAT_ON_6 Id = menu_cheat_6 unlocked = CHEAT_UNLOCKED_6
  Cheat_menu_refresh_line cheat = CHEAT_ON_7 Id = menu_cheat_7 unlocked = CHEAT_UNLOCKED_7
  Cheat_menu_refresh_line cheat = CHEAT_ON_8 Id = menu_cheat_8 unlocked = CHEAT_UNLOCKED_8
  Cheat_menu_refresh_line cheat = CHEAT_ON_9 Id = menu_cheat_9 unlocked = CHEAT_UNLOCKED_9
  Cheat_menu_refresh_line cheat = CHEAT_ON_10 Id = menu_cheat_10 unlocked = CHEAT_UNLOCKED_10
  Cheat_menu_refresh_line cheat = CHEAT_ON_11 Id = menu_cheat_11 unlocked = CHEAT_UNLOCKED_11
endscript

// updates cheat menu entries text
script Cheat_menu_refresh_line
// expected params:
// cheat = cheat enabled state flag
// id = menu element id
// unlocked = cheat unlock state flag
  Cheat_menu_maybe <...> maybe = CHEAT_SNOWBOARD on = "Snowboard: On" off = "Snowboard: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_ALWAYS_SPECIAL on = "Always Special: On" off = "Always Special: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_PERFECT_RAIL on = "Perfect Rail: On" off = "Perfect Rail: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_STATS_13 on = "Super Stats: On" off = "Super Stats: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_GIANT on = "Giant Mode: On" off = "Giant Mode: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_MATRIX on = "Slowmo: On" off = "Slowmo: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_PERFECT_MANUAL on = "Perfect Manual: On" off = "Perfect Manual: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_KID on = "Tiny Mode: On" off = "Tiny Mode: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_MOON on = "Moon Physics: On" off = "Moon Physics: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_SIM on = "Expert Mode: On" off = "Expert Mode: Off"
  Cheat_menu_maybe <...> maybe = CHEAT_FIRST_PERSON on = "First Person: On" off = "First Person: Off"
endscript

// updates cheat menu entry text, based on global cheat flag
script Cheat_menu_maybe
// expected params:
// id = menu element id
// maybe = global cheat flag to check
// cheat = cheat enabled state flag
// unlocked = cheat unlock state flag
// on = unlocked text
// off = locked text
  if IntegerEquals a = <cheat> b = <maybe>
    if GetGlobalFlag flag = <unlocked>
      if GetGlobalFlag flag = <cheat>
        SetMenuElementText Id = <Id> <on>
      else
        SetMenuElementText Id = <Id> <off>
      endif
    else
      SetMenuElementText Id = <Id> " - Cheat Locked - "
    endif
  endif
endscript

script done_with_cheats
endscript


//==================================================
// unlock scripts
//=================================================


script cleargame
  printf "clearing the game....."
  CareerStartLevel level = LevelNum_Rio
  ClearACompLevel
  CareerStartLevel level = LevelNum_SkaterIsland
  ClearACompLevel
  CareerStartLevel level = LevelNum_Tokyo
  ClearACompLevel
  SetGlobalFlag flag = GOT_GOLD_RIO
  SetGlobalFlag flag = GOT_GOLD_SI
  SetGlobalFlag flag = GOT_GOLD_TOKYO
  CareerStartLevel level = LevelNum_Foundry
  ClearALevel
  CareerStartLevel level = LevelNum_Canada
  ClearALevel
  CareerStartLevel level = LevelNum_Suburbia
  ClearALevel
  CareerStartLevel level = LevelNum_Airport
  ClearALevel
  CareerStartLevel level = LevelNum_LA
  ClearALevel
  CareerStartLevel level = LevelNum_Ship
  ClearALevel
  CareerStartLevel level = LevelNum_Skateshop
  SetGlobalFlag flag = LEVEL_UNLOCKED_FOUNDRY
  SetGlobalFlag flag = LEVEL_UNLOCKED_CANADA
  SetGlobalFlag flag = LEVEL_UNLOCKED_RIO
  SetGlobalFlag flag = LEVEL_UNLOCKED_SUBURBIA
  SetGlobalFlag flag = LEVEL_UNLOCKED_AIRPORT
  SetGlobalFlag flag = LEVEL_UNLOCKED_SKATERISLAND
  SetGlobalFlag flag = LEVEL_UNLOCKED_LOSANGELES
  SetGlobalFlag flag = LEVEL_UNLOCKED_TOKYO
  SetGlobalFlag flag = LEVEL_UNLOCKED_SHIP
  SetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
  //SetGlobalFlag flag = LEVEL_UNLOCKED_OIL   //xbox flag
endscript

script AllDecks
  CareerStartLevel level = LevelNum_Rio
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_SkaterIsland
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_Tokyo
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_Foundry
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_Canada
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_Suburbia
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_Airport
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_LA
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_Ship
  SetFlag flag = GOAL_DECK
  CareerStartLevel level = LevelNum_Skateshop
endscript

script AllStats
  CareerStartLevel level = LevelNum_Rio
  GiveLevelStats
  CareerStartLevel level = LevelNum_SkaterIsland
  GiveLevelStats
  CareerStartLevel level = LevelNum_Tokyo
  GiveLevelStats
  CareerStartLevel level = LevelNum_Foundry
  GiveLevelStats
  CareerStartLevel level = LevelNum_Canada
  GiveLevelStats
  CareerStartLevel level = LevelNum_Suburbia
  GiveLevelStats
  CareerStartLevel level = LevelNum_Airport
  GiveLevelStats
  CareerStartLevel level = LevelNum_LA
  GiveLevelStats
  CareerStartLevel level = LevelNum_Ship
  GiveLevelStats
  CareerStartLevel level = LevelNum_Skateshop
endscript

script unlock_all_cheats
  SetGlobalFlag flag = CHEAT_UNLOCKED_1
  SetGlobalFlag flag = CHEAT_UNLOCKED_2
  SetGlobalFlag flag = CHEAT_UNLOCKED_3
  SetGlobalFlag flag = CHEAT_UNLOCKED_4
  SetGlobalFlag flag = CHEAT_UNLOCKED_5
  SetGlobalFlag flag = CHEAT_UNLOCKED_6
  SetGlobalFlag flag = CHEAT_UNLOCKED_7
  SetGlobalFlag flag = CHEAT_UNLOCKED_8
  SetGlobalFlag flag = CHEAT_UNLOCKED_9
  SetGlobalFlag flag = CHEAT_UNLOCKED_10
  SetGlobalFlag flag = CHEAT_UNLOCKED_11
  SetGlobalFlag flag = CHEAT_UNLOCKED_12
endscript

script cheat_give_skaters
  SetGlobalFlag flag = SKATER_UNLOCKED_MAUL
  SetGlobalFlag flag = SKATER_UNLOCKED_WOLVERINE
  SetGlobalFlag flag = SKATER_UNLOCKED_DICK
  SetGlobalFlag flag = SKATER_UNLOCKED_CARRERA
  SetGlobalFlag flag = SKATER_UNLOCKED_BUM
  SetGlobalFlag flag = SKATER_UNLOCKED_SLATER
  SetGlobalFlag flag = SKATER_UNLOCKED_DEMONESS
  SetGlobalFlag flag = SKATER_UNLOCKED_EYEBALL
  //SetGlobalFlag flag = SKATER_UNLOCKED_SKATERX  //xbox flag
endscript

script cheat_give_skaterX
  SetGlobalFlag flag = SKATER_UNLOCKED_SKATERX
endscript

//this is cleargame + th1 levels and oil rig
script cheat_give_levels
  cleargame
  SetGlobalFlag flag = LEVEL_UNLOCKED_WAREHOUSE
  SetGlobalFlag flag = LEVEL_UNLOCKED_BURNSIDE
  SetGlobalFlag flag = LEVEL_UNLOCKED_ROSWELL
  //SetGlobalFlag flag = LEVEL_UNLOCKED_OIL   //xbox flag
endscript

script cheat_give_rig
  SetGlobalFlag flag = LEVEL_UNLOCKED_OIL
endscript

script cheat_give_everything
  cheat_give_skaters
  cheat_give_levels
  unlock_all_cheats
  cheat_unlockmovies
endscript

script cheat_togglemetrics
  Togglemetrics
endscript

script cheat_togglememmetrics
  ToggleMemMetrics
endscript

script cheat_unlockmovies
  SetGlobalFlag flag = MOVIE_UNLOCKED_HAWK
  SetGlobalFlag flag = MOVIE_UNLOCKED_CABALLERO
  SetGlobalFlag flag = MOVIE_UNLOCKED_CAMPBELL
  SetGlobalFlag flag = MOVIE_UNLOCKED_GLIFBERG
  SetGlobalFlag flag = MOVIE_UNLOCKED_KOSTON
  SetGlobalFlag flag = MOVIE_UNLOCKED_LASEK
  SetGlobalFlag flag = MOVIE_UNLOCKED_MARGERA
  SetGlobalFlag flag = MOVIE_UNLOCKED_MULLEN
  SetGlobalFlag flag = MOVIE_UNLOCKED_MUSKA
  SetGlobalFlag flag = MOVIE_UNLOCKED_REYNOLDS
  SetGlobalFlag flag = MOVIE_UNLOCKED_ROWLEY
  SetGlobalFlag flag = MOVIE_UNLOCKED_STEAMER
  SetGlobalFlag flag = MOVIE_UNLOCKED_THOMAS
  SetGlobalFlag flag = MOVIE_UNLOCKED_JEDIKNIGHT
  SetGlobalFlag flag = MOVIE_UNLOCKED_WOLVERINE
  SetGlobalFlag flag = MOVIE_UNLOCKED_CARRERA
  SetGlobalFlag flag = MOVIE_UNLOCKED_DICK
  SetGlobalFlag flag = MOVIE_UNLOCKED_SLATER
  SetGlobalFlag flag = MOVIE_UNLOCKED_BUM
  SetGlobalFlag flag = MOVIE_UNLOCKED_Eyeball
  SetGlobalFlag flag = MOVIE_UNLOCKED_PRO_BAILS1
  SetGlobalFlag flag = MOVIE_UNLOCKED_CAS
  SetGlobalFlag flag = MOVIE_UNLOCKED_DEMONESS
  //SetGlobalFlag flag = MOVIE_UNLOCKED_SKATERX //xbox flag
  movies_menu_create
endscript

script do_doomguy_stuff
  wait 10 gameframes
  SwitchOffMenu Id = options_main_menu
  wait 5 gameframes
  DisableBackEvent
  DisableEvent Id = options_main_menu EventType = back
  DisableEvent Id = options_main_menu EventType = choose
  FrontEndSetInactive
  CreatePanels
  KillMessages
  PauseStream 0
  PlayStream unlock_secret
  LaunchLocalMessage Id = LegealStuff "DOOM character (c) 1999-2002 Id Software, Inc. Used under license. All Rights Reserved." panel_message_legalmumbojumbo
  LaunchLocalMessage Id = GoalText "Secret Character Unlocked" panel_message_newThing01
  LaunchLocalMessage Id = complete "DOOM Guy" panel_message_newlevelThingName01
  SetGlobalFlag flag = SKATER_UNLOCKED_DOOMGUY
  SetGlobalFlag flag = SKATESHOP_JUST_GOT_SECRET_CHAR
  wait 0.5 seconds
  LaunchLocalMessage Id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage Id = GoalText "Secret Character Unlocked" panel_message_newThing02
  LaunchLocalMessage Id = complete "DOOM Guy" panel_message_newlevelThingName02
  LaunchLocalMessage Id = BottomInfo " " panel_message_XtoContinue
  LaunchLocalMessage Id = LegealStuff " " panel_message_legalmumbojumbo
  wait 0.5 seconds
  LaunchLocalMessage Id = GoalText "New Movie Unlocked" panel_message_newThing01
  LaunchLocalMessage Id = complete "Gearbox Tribute" panel_message_newlevelThingName01
  wait 1 seconds
  LaunchLocalMessage Id = BottomInfo "Press ¬ to Continue" panel_message_XtoContinue
  WaitForControllerPressedX
  LaunchLocalMessage Id = GoalText "New Movie Unlocked" panel_message_newThing02
  LaunchLocalMessage Id = complete "Gearbox Tribute" panel_message_newlevelThingName02
  LaunchLocalMessage Id = BottomInfo " " panel_message_XtoContinue
  LaunchLocalMessage Id = LegealStuff " " panel_message_legalmumbojumbo
  wait 0.5 seconds
  KillMessages
  DeletePanel
  ControllerDebounce x time = 0.2
  wait 15 gameframes
  SwitchOnMenu Id = options_main_menu
  EnableBackEvent
  EnableEvent Id = options_main_menu EventType = back
  EnableEvent Id = options_main_menu EventType = choose
  FrontEndSetActive
endscript

script cheat_unlockdoomguy
  if GetGlobalFlag flag = SKATER_UNLOCKED_DOOMGUY
  else
    SetGlobalFlag flag = SKATER_UNLOCKED_DOOMGUY
    MakeTextMenuElementStatic Movie_GBX_Tribute nonstatic = 1
    spawnscript do_doomguy_stuff
  endif
endscript

script ClearACompLevel
  SetGoal Goal = GOAL_BRONZE
  SetGoal Goal = GOAL_SILVER
  SetGoal Goal = GOAL_GOLD
  SetFlag flag = GOAL_DECK
  GiveLevelStats
  SetFlag flag = GOAL_STAT_POINT1
  SetFlag flag = GOAL_STAT_POINT2
  SetFlag flag = GOAL_STAT_POINT3
  SetFlag flag = GOAL_STAT_POINT4
  SetFlag flag = GOAL_STAT_POINT5
endscript

script ClearALevel
  SetFlag flag = GOAL_DECK
  GiveLevelStats
  SetGoal Goal = GOAL_HIGHSCORE
  SetGoal Goal = GOAL_PROSCORE
  SetGoal Goal = GOAL_SICKSCORE
  SetGoal Goal = GOAL_SKATE
  SetGoal Goal = GOAL_TRICKSPOT
  SetGoal Goal = GOAL_TAPE
  SetGoal Goal = GOAL_SCRIPTED1
  SetGoal Goal = GOAL_SCRIPTED2
  SetGoal Goal = GOAL_SCRIPTED3
endscript

script GiveLevelStats
  if GetFlag flag = GOAL_STAT_POINT1
  else
    SetFlag flag = GOAL_STAT_POINT1
    AwardStatPoint
  endif
  if GetFlag flag = GOAL_STAT_POINT2
  else
    SetFlag flag = GOAL_STAT_POINT2
    AwardStatPoint
  endif
  if GetFlag flag = GOAL_STAT_POINT3
  else
    SetFlag flag = GOAL_STAT_POINT3
    AwardStatPoint
  endif
  if GetFlag flag = GOAL_STAT_POINT4
  else
    SetFlag flag = GOAL_STAT_POINT4
    AwardStatPoint
  endif
  if GetFlag flag = GOAL_STAT_POINT5
  else
    SetFlag flag = GOAL_STAT_POINT5
    AwardStatPoint
  endif
endscript

script cheat_xxx
  printf "Cheat without a cheatscript"
endscript

script cheat_give_neversoft_skaters
  change give_neversoft_skaters = 1
endscript

script cheat_select_shift
  change select_shift = 1
endscript

//cheat OK sound !!!called from EXE!!!
script cheat_playsound_good
  PlaySound GUI_unlock1
endscript

//cheat failed sound !!!called from EXE!!!
script cheat_playsound_bad
  PlaySound GUI_buzzer01
endscript

script cheat_toggle_net_metrics
  ToggleNetMetrics
endscript

script cheat_reset
  ResetPS2
endscript

script cheat_resethd
  ResetHD
endscript

script default_non_cd_startup
  cheat_give_neversoft_skaters
  cheat_select_shift
endscript

//minimum cheat string length is 5

cheat_array = [

    //pc/ps2 cheats
    { c1 = 617143598 c2 = -1773057737 CheatScript = unlock_all_cheats CheatString = magicmissle }
    { c1 = 235273493 c2 = -488702967 CheatScript = cheat_give_everything CheatString = backdoor }
    { c1 = 379726526 c2 = 1631993579 CheatScript = cheat_give_levels CheatString = roadtrip }
    { c1 = 1906945534 c2 = 1661351313 CheatScript = cheat_give_skaters CheatString = yohomies }
    { c1 = -1732415189 c2 = 237830098 CheatScript = cheat_give_neversoft_skaters CheatString = weeatdirt }
    { c1 = 1617514200 c2 = 84318768 CheatScript = cleargame }
    { c1 = 156079685 c2 = 407536924 CheatScript = AllDecks CheatString = givemesomewood }
    { c1 = 652320665 c2 = 334460197 CheatScript = AllStats CheatString = pumpmeup }
    { c1 = 1552329878 c2 = -766892396 CheatScript = cheat_unlockmovies CheatString = peepshow }
    { c1 = -2038664901 c2 = 461662073 CheatScript = cheat_unlockdoomguy CheatString = idkfa }    //doomguy PC extra cheat

    //xbox
    { c1 = -37959188 c2 = 1748658267 CheatScript = unlock_all_cheats }
    { c1 = 181597736 c2 = 2026475500 CheatScript = cheat_give_everything }
    { c1 = 1050402718 c2 = -1165286410 CheatScript = cheat_give_levels CheatString = stiffcomp } //maybe its cleargame?
    { c1 = 1517769166 c2 = -2102684580 CheatScript = cheat_give_skaters CheatString = teamfreak }
    //{ c1 = -1732415189 c2 = 237830098 CheatScript = cheat_give_neversoft_skaters CheatString = weeatdirt } //this one is same
    { c1 = -870714115 c2 = -2014367527 CheatScript = cleargame }
    { c1 = -1047293409 c2 = -2139091335 CheatScript = AllDecks CheatString = neverboard }
    { c1 = -576160501 c2 = -626945426 CheatScript = AllStats CheatString = juice4me }
    { c1 = -1826212812 c2 = -1357437635 CheatScript = cheat_unlockmovies CheatString = rollit }
    { c1 = -1516901853 c2 = -1163887267 CheatScript = cheat_give_rig }    //unlock oil rig extra cheat
    { c1 = 1077935541 c2 = -2114267603 CheatScript = cheat_give_skaterX }   //unlock x-ray extra cheat

    //gamecube
    { c1 = 1060403491 c2 = 2115153090 CheatScript = unlock_all_cheats CheatString = markedcards }
    { c1 = 1394139785 c2 = -1265691956 CheatScript = cheat_give_everything CheatString = noneed2play }
    { c1 = 272215117 c2 = -187732418 CheatScript = cheat_give_levels CheatString = fieldtrip } //cheatstring doesnt work
    { c1 = -803154772 c2 = 752950656 CheatScript = cheat_give_skaters CheatString = freakshow }
    //{ c1 = -1732415189 c2 = 237830098 CheatScript = cheat_give_neversoft_skaters }  //this one is same
    { c1 = 1278439181 c2 = -58809515 CheatScript = cleargame CheatString = unlimited } //cheatstring doesnt work
    { c1 = 843781972 c2 = -1161699504 CheatScript = AllDecks }
    { c1 = 573732884 c2 = 260894528 CheatScript = AllStats CheatString = maxmeout }
    { c1 = -1120178649 c2 = 497637221 CheatScript = cheat_unlockmovies CheatString = popcorn }

]