
FirstTimeInSplitscreen = 1
FirstTimeFromSkateshop = 0

//original th3 graffiti colors
graffitiColors = [
  (192, 0, 0)     //red
  (0, 0, 192)     //blue
  (192, 192, 0)   //yellow
  (0, 127, 0)     //green
  (48, 192, 192)  //cyan
  (192, 96, 0)    //orange
  (140, 97, 35)   //brown << this and orange are almost same, lol
  (192, 0, 192)   //magenta
]

//th4 colors are darker, can we make it toggleable somehow?
graffitiColors_thps4 = [
    (102, 26, 8)    //red
    (17, 64, 116)   //blue
    (9, 107, 36)    //green
    (123, 99, 3)    //brown
    (0, 128, 128)   //cyan
    (128, 0, 128)   //magenta
    (30, 50, 80)    //dark blue << this is bad, better be yellow or violet
    (88, 105, 112)  //grayish
]

//unused stuff? can remove? 0.8 like they intended hsv here instead of rgb?
__graffitiColors = [
    (0, 1, 1)
    (240, 1, 1)
    (60, 1, 1)
    (120, 0.8, 0.7)
    (45, 1, 1)
    (225, 1, 1)
    (135, 1, 1)
    (315, 1, 1)
]

splitscreen_game_type_info = [
  { name = "Free Skate" checksum = FreeSkate2p }
  { name = "Trick Attack" checksum = trickattack }
  { name = "Graffiti" checksum = graffiti }
  { name = "Slap!" checksum = slap }
  { name = "Horse" checksum = horse }
  { name = "King of the Hill" checksum = king }
]

//this defines time limit for online modes, added more options here
time_limit_options = [
    { name = "None" checksum = time_0 time = 0 }
    { name = "5 Seconds" checksum = time_5 time = 5 }
    { name = "10 Seconds" checksum = time_10 time = 10 }
    { name = "30 Seconds" checksum = time_30 time = 30 }
    { name = "1 Minute" checksum = time_60 time = 60 }
    { name = "2 Minutes" checksum = time_120 time = 120 }
    { name = "5 Minutes" checksum = time_300 time = 300 }
    { name = "10 Minutes" checksum = time_600 time = 600 }
    { name = "30 Minutes" checksum = time_1800 time = 1800 }
    { name = "1 Hour" checksum = time_3600 time = 3600 }
    { name = "2 Hours" checksum = time_7200 time = 7200 }
    { name = "Devil Special" checksum = time_40019 time = 40019 }
]

horse_time_limit_options = [
    { name = "10 seconds" checksum = horse_time_10 time = 10 }
    { name = "20 seconds" checksum = horse_time_20 time = 20 }
    { name = "30 seconds" checksum = horse_time_30 time = 30 }
]

player_handicap_options = [
    { name = "+5 to stats" checksum = player_handicap_5 time = 5 }
    { name = "+4 to stats" checksum = player_handicap_4 time = 4 }
    { name = "+3 to stats" checksum = player_handicap_3 time = 3 }
    { name = "+2 to stats" checksum = player_handicap_2 time = 2 }
    { name = "+1 to stats" checksum = player_handicap_1 time = 1 }
    { name = "No change to stats" checksum = player_handicap_0 time = 0 }
    { name = "-1 to stats" checksum = player_handicap_minus_1 time = -1 }
    { name = "-2 to stats" checksum = player_handicap_minus_2 time = -2 }
    { name = "-3 to stats" checksum = player_handicap_minus_3 time = -3 }
    { name = "-4 to stats" checksum = player_handicap_minus_4 time = -4 }
    { name = "-5 to stats" checksum = player_handicap_minus_5 time = -5 }
]

//apparently unused on pc due to missing splitscreen
viewport_options = [
    { name = "Vertical" checksum = viewport_type_vertical }
    { name = "Horizontal" checksum = viewport_type_horizontal }
]

default_splitscreen_preferences = {
    game_type = { ui_string = "Trick Attack" checksum = trickattack }
    time_limit = { ui_string = "2 minutes" checksum = time_120 time = 120 }
    horse_time_limit = { ui_string = "10 seconds" checksum = horse_time_10 time = 10 }
    player1_handicap = { ui_string = "No change to stats" checksum = player_handicap_0 time = 0 }
    player2_handicap = { ui_string = "No change to stats" checksum = player_handicap_0 time = 0 }
    viewport_type = { ui_string = "Vertical" checksum = viewport_type_vertical }
    horse_word = { ui_string = "Horse" }
}

script do_victorycondition_highestscore
    OverrideGameModeOptions { victory_conditions = [ { type = highest_score } ] }
endscript

script do_victorycondition_target10000
    OverrideGameModeOptions { victory_conditions = [ { type = target_score Score = 10000 } ] }
endscript

script do_victorycondition_target50000
  OverrideGameModeOptions { victory_conditions = [ { type = target_score Score = 50000 } ] }
endscript

script StartSplitScreenGame
    ApplySplitScreenOptions
    unpause_game_flow
    if IsTrue FirstTimeInSplitscreen
    else
        do_backend_retry
    endif
endscript

script RefreshSplitScreenMenu
  DestroyElement id = ssm_time_limit_title
  DestroyElement id = ssm_time_limit_display
  DestroyElement id = ssm_player1_handicap_title
  DestroyElement id = ssm_player1_handicap_display
  DestroyElement id = ssm_player2_handicap_title
  DestroyElement id = ssm_player2_handicap_display
  DestroyElement id = ssm_screenmode_title
  DestroyElement id = ssm_screenmode_display
  DestroyElement id = ssm_horse_time_limit_title
  DestroyElement id = ssm_horse_time_limit_display
  DestroyElement id = ssm_horse_word_title
  DestroyElement id = ssm_horse_word_display
  DestroyElement id = ssm_start_game
  if PreferenceEquals prefs = splitscreen field = game_type equals = FreeSkate2p
  else
    if PreferenceEquals prefs = splitscreen field = game_type equals = horse
      AddLine { id = ssm_horse_time_limit_title parent = splitscreen_setup_menu text = "Time Limit" link = horse_time_limit_menu }
      AddLine { id = ssm_horse_time_limit_display parent = splitscreen_setup_menu text = "Dummy Horse Time Limit" static dont_gray drawer = keyboard_property }
      AttachEventHandler { object = ssm_horse_time_limit_display type = showeventhandler target = "set_ui_from_preferences" params = { prefs = splitscreen field = "horse_time_limit" } }
    else
      AddLine { id = ssm_time_limit_title parent = splitscreen_setup_menu text = "Time Limit" link = splitscreen_time_limit_menu }
      AddLine { id = ssm_time_limit_display parent = splitscreen_setup_menu text = "Dummy Time Limit" static dont_gray drawer = keyboard_property }
      AttachEventHandler { object = ssm_time_limit_display type = showeventhandler target = "set_ui_from_preferences" params = { prefs = splitscreen field = "time_limit" } }
    endif
  endif
  AddLine { id = ssm_player1_handicap_title parent = splitscreen_setup_menu text = "Player 1 Handicap" link = splitscreen_player1_handicap_menu }
  AddLine { id = ssm_player1_handicap_display parent = splitscreen_setup_menu text = "Dummy Handicap 1" static dont_gray drawer = keyboard_property }
  AttachEventHandler { object = ssm_player1_handicap_display type = showeventhandler target = "set_ui_from_preferences" params = { prefs = splitscreen field = "player1_handicap" } }
  AddLine { id = ssm_player2_handicap_title parent = splitscreen_setup_menu text = "Player 2 Handicap" link = splitscreen_player2_handicap_menu }
  AddLine { id = ssm_player2_handicap_display parent = splitscreen_setup_menu text = "Dummy Handicap 2" static dont_gray drawer = keyboard_property }
  AttachEventHandler { object = ssm_player2_handicap_display type = showeventhandler target = "set_ui_from_preferences" params = { prefs = splitscreen field = "player2_handicap" } }
  if PreferenceEquals prefs = splitscreen field = game_type equals = horse
  else
    AddLine { id = ssm_screenmode_title parent = splitscreen_setup_menu text = "Screen Mode" link = splitscreen_viewport_type_menu }
    AddLine { id = ssm_screenmode_display parent = splitscreen_setup_menu text = "Dummy Screen Mode" static dont_gray drawer = keyboard_property }
    AttachEventHandler { object = ssm_screenmode_display type = showeventhandler target = "set_ui_from_preferences" params = { prefs = splitscreen field = "viewport_type" } }
  endif
  if PreferenceEquals prefs = splitscreen field = game_type equals = horse
    AddLine { id = ssm_horse_word_title parent = splitscreen_setup_menu text = "Horse Word" link = horse_keyboard_control }
    AddLine { id = ssm_horse_word_display parent = splitscreen_setup_menu text = "Dummy Horse Word" static dont_gray drawer = keyboard_property }
    AttachEventHandler { object = ssm_horse_word_display type = showeventhandler target = "set_ui_from_preferences" params = { prefs = splitscreen field = "horse_word" } }
  else
  endif
  AddLine { id = ssm_start_game parent = splitscreen_setup_menu text = "Start Game" target = "StartSplitScreenGame" kill_menu }
  if IsTrue FirstTimeInSplitscreen
    EndofRun_helper
  else
    helper_select_choose_back_centered
  endif
endscript

script create_splitscreen_menu
  CreateAndAttachMenu { type = verticalmenu id = splitscreen_setup_menu parent = contain1 x = 170.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y blue_top eventhandlers = [ { type = showeventhandler target = "RefreshSplitScreenMenu" } ] children = [ { type = textmenuelement auto_id text = "2 Player Setup" static dont_gray drawer = title }
      { type = textmenuelement auto_id text = "Game Type" link = splitscreen_game_type_menu }
      { type = textmenuelement id = game_type_display text = "Current Game" static dont_gray drawer = keyboard_property eventhandler = { type = showeventhandler target = "set_ui_from_preferences" params = { prefs = splitscreen field = "game_type" } } } ] }
  create_and_attach_generic_array_menu { menu_id = splitscreen_game_type_menu array_name = splitscreen_game_type_info field = game_type title = "Game Type" prefs = splitscreen x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
  AttachEventHandler { object = splitscreen_game_type_menu type = showeventhandler target = "helper_select_choose_back_centered" }
  create_and_attach_generic_array_menu { menu_id = splitscreen_time_limit_menu array_name = time_limit_options field = time_limit title = "Time Limit" prefs = splitscreen x = 170.0 y = 90.0 w = 300.0 h = 400.0 }
  AttachEventHandler { object = splitscreen_time_limit_menu type = showeventhandler target = "helper_select_choose_back_centered" }
  create_and_attach_generic_array_menu { menu_id = splitscreen_player1_handicap_menu array_name = player_handicap_options field = player1_handicap title = "Player 1 Handicap" prefs = splitscreen x = 135.0 y = 90.0 w = 370.0 h = 400.0 }
  AttachEventHandler { object = splitscreen_player1_handicap_menu type = showeventhandler target = "helper_select_choose_back_centered" }
  create_and_attach_generic_array_menu { menu_id = splitscreen_player2_handicap_menu array_name = player_handicap_options field = player2_handicap title = "Player 2 Handicap" prefs = splitscreen x = 135.0 y = 90.0 w = 370.0 h = 400.0 }
  AttachEventHandler { object = splitscreen_player2_handicap_menu type = showeventhandler target = "helper_select_choose_back_centered" }
  CreateAndAttachMenu { type = verticalmenu id = splitscreen_viewport_type_menu x = 170.0 y = 90.0 w = 300.0 h = 400.0 just_center_x just_center_y parent = contain1 }
  AttachEventHandler { object = splitscreen_viewport_type_menu type = showeventhandler target = "helper_select_choose_back_centered" }
endscript
