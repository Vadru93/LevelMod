script start_game
  printf "here 1"
  set_network_mode NO_NET_MODE
  printf "here 2"
  SetServerMode On
  printf "here 3"
  setcurrentgametype
  printf "here 4"
  SetGameState On
  printf "here 5"
  LaunchMenuScreen screen = game_menu not_active
  printf "here 6"
endscript

script immediately_start_game
  Request_Level level = <level>
  start_game
  printf "script immediately_start_game\n"
endscript

script start_replay_game
  setReplayMode REPLAY_MODE_PLAY_SAVED
  start_game
endscript

script change_level
  if InNetGame
    HideMenu
  endif
  if InFrontEnd
    SetGameType freeskate
    immediately_start_game <...>
  else
    if istrue run_viewer
      if FirstTimeThisIsCalled
        SetGameType freeskate
        immediately_start_game <...>
      else
        Request_Level level = <level>
        ChangeLevel <...>
      endif
    else
      Request_Level level = <level>
      ChangeLevel <...>
    endif
  endif
endscript

script level_menu_create
  CreateAndAttachMenu { type = verticalmenu id = level_main_menu x = 300.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { type = backeventhandler target = "LevelSelectToPlayer1CamAnim" }
      { type = showeventhandler target = "populate_level_menu" params = { choose_script = "change_level" parent_menu = level_main_menu } } ] children = [ { type = textmenuelement auto_id text = "Levels" static dont_gray drawer = title } ] parent = contain1 }
endscript

script add_level_menu_item
  if GotParam notPS2_level
    if cd
    else
      add_level_menu_item2 <...>
    endif
  else
    if GotParam unlock_flag
      if GetGlobalFlag flag = <unlock_flag>
        add_level_menu_item2 <...>
      else
        if istrue AllLevelsUnlocked
          add_level_menu_item2 <...>
        else
          if GotParam special_flag
            if GetGlobalFlag flag = SPECIAL_HAS_SEEN_SHIP
              if IsCareerMode
                add_level_menu_item2 <...> static
              else
                CheckForAnyPro_Unlock <...>
              endif
            else
            endif
          else
            if GotParam secret_level
            else
              if IsCareerMode
                add_level_menu_item2 <...> static
              else
                CheckForAnyPro_Unlock <...>
              endif
            endif
          endif
        endif
      endif
    else
      add_level_menu_item2 <...>
    endif
  endif
endscript

script CheckForAnyPro_Unlock
  if GotParam unlock_flag
    if GetGlobalFlag flag = <unlock_flag> AnyPro
      printf "some other pro has unlocked this level"
      add_level_menu_item2 <...>
    else
      printf "NO pro has unlocked this level"
      add_level_menu_item2 <...> static
    endif
  else
    add_level_menu_item2 <...>
  endif
endscript

script add_level_menu_item2
  if GotParam <mask>
    VerifyParam param = level_name func = add_level_menu_item <...>
    VerifyParam param = load_script func = add_level_menu_item <...>
    VerifyParam param = target_script func = add_level_menu_item <...>
    VerifyParam param = parent_menu func = add_level_menu_item <...>
    DestroyElement id = <level_id>
	
	//If file don't exists make menu static
    if NotTrue FileExists <lev_bsp>
	    AddParam static
	endif
	
    if GotParam static
      CreateAndAttachMenu { type = textmenuelement id = <level_id> text = <level_name> target = <target_script> static kill_menu params = { level = <load_script> string = <level_name> } parent = <parent_menu> }
    else
      if IsCareerMode
        if GotParam NoCareer
        else
          CreateAndAttachMenu { type = textmenuelement id = <level_id> text = <level_name> target = <target_script> kill_menu params = { level = <load_script> string = <level_name> } parent = <parent_menu> }
        endif
      else
        CreateAndAttachMenu { type = textmenuelement id = <level_id> text = <level_name> target = <target_script> params = { level = <load_script> string = <level_name> } parent = <parent_menu> }
      endif
    endif
  endif
endscript