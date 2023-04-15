
helper_string_accept = "¬ Accept"
helper_string_back = "Є Back"
helper_string_select_lr = "ЎҐ Select"
helper_string_select_ud = "§Ј Select"
helper_string_select_all = "ЎҐ§Ј Select"

//???
download_str = "LOLlololol"

script helper_menu_create
  CreateMenu { type = textmenuelement id = helper_menu_1 x = 90.0 y = 350.0 w = 150.0 h = 250.0 just_center_x just_center_y text = "waaaaaaaaaaaaaaaaaaaaaaaaaaaw" static dont_gray drawer = helper_text dontdrawrect }
    //???
  CreateMenu { type = textmenuelement id = levelmod_download x = 0.0 y = 300.0 w = 1024 h = 250.0 just_center_x just_center_y text = download_str static dont_gray drawer = small_helper dontdrawrect }
  CreateMenu { type = textmenuelement id = helper_menu_2 x = 250.0 y = 350.0 w = 150.0 h = 250.0 just_center_x just_center_y text = "ЎўЈ¤Ґ¦§Ё©Є«¬" static dont_gray drawer = helper_text dontdrawrect }
  CreateMenu { type = textmenuelement id = helper_menu_3 x = 410.0 y = 350.0 w = 150.0 h = 250.0 just_center_x just_center_y text = "ЎўЈ¤Ґ¦§Ё©Є«¬" static dont_gray drawer = helper_text dontdrawrect }
  CreateMenu { type = textmenuelement id = helper_menu_4 x = 410.0 y = 350.0 w = 150.0 h = 250.0 just_center_x just_center_y text = "ЎўЈ¤Ґ¦§Ё©Є«¬" static dont_gray drawer = helper_text dontdrawrect }
  CreateMenu { type = textmenuelement id = helper_menu_5 x = 410.0 y = 350.0 just_left_x just_center_y text = "Full" static dont_gray drawer = small_helper dontdrawrect }
  CreateMenu { type = textmenuelement id = helper_menu_6 x = 410.0 y = 350.0 just_left_x just_center_y text = "Empty" static dont_gray drawer = small_helper dontdrawrect }
  AttachChild parent = contain1 child = helper_menu_1
  AttachChild parent = contain1 child = helper_menu_2
  AttachChild parent = contain1 child = helper_menu_3
  AttachChild parent = contain1 child = helper_menu_4
  AttachChild parent = contain1 child = helper_menu_5
  AttachChild parent = contain1 child = helper_menu_6
endscript

script helper_set_positions y = 402 x1 = 45 w1 = 100 x2 = 160 w2 = 100 x3 = 270 w3 = 80
  MoveMenu id = helper_menu_1 x = <x1> y = <y> w = <w1>
  MoveMenu id = helper_menu_2 x = <x2> y = <y> w = <w2>
  MoveMenu id = helper_menu_3 x = <x3> y = <y> w = <w3>
  MoveMenu id = helper_menu_4 x = <x4> y = <y> w = <w4>
endscript

//replace global flag with levelmod setting later
SETTING_FLAG_HELPERS_ENABLED = 1

//levelmod stuff?
script ShowDownloadInfo
  FireShowEvent id = helper_menu_1 target = helper_menu_1
endscript

script helper_show
  FireShowEvent id = helper_menu_1
  FireShowEvent id = helper_menu_2
  FireShowEvent id = helper_menu_3
endscript

script helper_hide
  FireHideEvent id = helper_menu_1
  FireHideEvent id = helper_menu_2
  FireHideEvent id = helper_menu_3
  FireHideEvent id = helper_menu_4 //why 4 here, but not 4 in helper_show???
endscript

script FireMenuEventWrapper
  if GetGlobalFlag flag = SETTING_FLAG_HELPERS_ENABLED
    if GotParam id
        FireMenuEvent type = <type> id = <id> target = <id>
    endif
  endif
endscript

script FireShowEvent
    FireMenuEventWrapper type = show id = <id>
endscript

script FireHideEvent
    FireMenuEventWrapper type = hide id = <id>
endscript

script helper_park_editor_building
    helper_hide
    helper_show
    helper_set_positions x1 = 45 x2 = 220 x3 = 400
    SetMenuElementText id = helper_menu_1 "L1/L2 Raise/Lower"
    SetMenuElementText id = helper_menu_2 "D-Pad change piece"
    SetMenuElementText id = helper_menu_3 "¬ place piece"

    FireShowEvent id = helper_menu_4
    MoveMenu id = helper_menu_4 x = 515 y = 410 w = 80
    SetMenuElementText id = helper_menu_4 "Є erase"

    FireShowEvent id = helper_menu_5
    MoveMenu id = helper_menu_5 x = 565 y = 75 w = 80
    SetMenuElementText id = helper_menu_5 "full"

    FireShowEvent id = helper_menu_6
    MoveMenu id = helper_menu_6 x = 565 y = 390 w = 80
    SetMenuElementText id = helper_menu_6 "empty"
endscript

script helper_select_choose_back
  helper_show
  helper_set_positions <...>
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 helper_string_accept
  SetMenuElementText id = helper_menu_3 helper_string_back
endscript

//this is on pc, ever used?
script SetHelperText
  SetMenuElementText id = helper_menu_1 <text>
endscript

//this is on pc, ever used?
script SetHelperPos
  MoveMenu id = helper_menu_1 x = 90 y = 420 w = 2024
endscript

script helper_select_choose_back_delete
  helper_show
  helper_set_positions x1 = 122 x2 = 237 x3 = 347
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 helper_string_accept
  SetMenuElementText id = helper_menu_3 helper_string_back
  FireShowEvent id = helper_menu_4 target = helper_menu_4
  MoveMenu id = helper_menu_4 x = 425 y = 402
  SetMenuElementText id = helper_menu_4 "© Delete"
endscript

script helper_select_choose_rotate_back x1 = 67 x2 = 192 x3 = 345 x4 = 480
  printf "launching helper text helper_select_choose_rotate_back ------------------"
  helper_show
  helper_set_positions <...>
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 helper_string_accept
  SetMenuElementText id = helper_menu_3 "©/« Rotate Skater"
  FireShowEvent id = helper_menu_4 target = helper_menu_4
  SetMenuElementText id = helper_menu_4 helper_string_back
  if menuisshown SelectChooseBackRotateMenus
    WaitForMenuToDisappear SelectChooseBackRotateMenus
  endif
endscript

script helper_select_lr_choose_back_centered
  helper_show
  helper_set_positions x1 = 25 x2 = 120 x3 = 452 x4 = 235
  SetMenuElementText id = helper_menu_1 helper_string_accept
  SetMenuElementText id = helper_menu_2 helper_string_back
  SetMenuElementText id = helper_menu_3 "§ Change Skater Ј"
  FireShowEvent id = helper_menu_4 target = helper_menu_4
  SetMenuElementText id = helper_menu_4 "©/« Rotate Skater"
endscript

script helper_select_toggle_back
  helper_show
  helper_set_positions x1 = 100 x2 = 270 x3 = 430
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "¬ Toggle/Accept"
  SetMenuElementText id = helper_menu_3 helper_string_back
endscript

script helper_leftrightselect_choose_back
  helper_show
  helper_set_positions <...>
  SetMenuElementText id = helper_menu_1 helper_string_select_ud
  SetMenuElementText id = helper_menu_2 helper_string_accept
  SetMenuElementText id = helper_menu_3 helper_string_back
endscript

script helper_level_select
  helper_hide
  helper_show
  helper_set_positions x1 = 170 x2 = 285 x3 = 395
  SetMenuElementText id = helper_menu_1 helper_string_select_ud
  SetMenuElementText id = helper_menu_2 helper_string_accept
  SetMenuElementText id = helper_menu_3 helper_string_back
  if TestRequestedGameType Career
  else
    helper_set_positions x1 = 70 x2 = 190 x3 = 450 x4 = 290
    FireShowEvent id = helper_menu_4 target = helper_menu_4
    SetMenuElementText id = helper_menu_4 "ЎҐ Records"
  endif
endscript

script helper_pro_leftright
  FireShowEvent id = helper_menu_4 target = helper_menu_4
  MoveMenu id = helper_menu_4 x = 465 y = 410 w = 80
  SetMenuElementText id = helper_menu_4 "§ Change Skater Ј"
endscript

script helper_level_records
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  SetMenuElementText id = helper_menu_1 "§Ґ Select ЎЈ"
  SetMenuElementText id = helper_menu_2 helper_string_back
  helper_set_positions x1 = 130 x2 = 350
endscript

script helper_please_insert_card
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  SetMenuElementText id = helper_menu_1 helper_string_back
  MoveMenu id = helper_menu_1 x = 270 y = 402
endscript

script helper_on_trick_right_menu
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "¬ Choose Button Combo"
  SetMenuElementText id = helper_menu_3 helper_string_back
  helper_set_positions x1 = 80 x2 = 270 x3 = 450
  if InFrontEnd
    WaitForMenuToDisappear trick_right_menu
  endif
endscript

script helper_on_trick_available_menu
  printf "helper_on_trick_available_menu"
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "¬ Choose Trick"
  SetMenuElementText id = helper_menu_3 helper_string_back
  if InFrontEnd
    WaitForMenuToDisappear trick_available_menu
  endif
endscript

script helper_on_special_trickname_menu
  printf "helper_on_special_trickname_menu"
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  FireShowEvent id = helper_menu_4 target = helper_menu_4
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "¬ Choose Trick Slot"
  SetMenuElementText id = helper_menu_3 "© Unassign Trick"
  SetMenuElementText id = helper_menu_4 helper_string_back
  helper_set_positions x1 = 25 x2 = 180 x3 = 380 x4 = 480
  if InFrontEnd
    WaitForMenuToDisappear special_trickname_menu
  endif
endscript

script helper_on_available_specials_menu
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "¬ Choose Special Trick"
  SetMenuElementText id = helper_menu_3 helper_string_back
  helper_set_positions x1 = 80 x2 = 270 x3 = 450
  if InFrontEnd
    WaitForMenuToDisappear available_specials_menu callback = check_for_trickslots_menu
  endif
endscript

script check_for_trickslots_menu
  if menuisshown available_trickslots_menu
    Goto helper_on_available_trickslots_menu
  endif
endscript

script helper_on_available_trickslots_menu
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "¬ Choose Button Combo"
  SetMenuElementText id = helper_menu_3 helper_string_back
  helper_set_positions x1 = 80 x2 = 270 x3 = 450
  if InFrontEnd
    WaitForMenuToDisappear available_trickslots_menu
  endif
endscript

script helper_on_stat_menu
  if IsTrue EnteringStatsMenuFromGameFlow
    helper_on_cas_select_change_accept
  else
    helper_on_cas_select_change_accept_back
  endif
endscript

script helper_on_keyboard_control
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  helper_set_positions x1 = 167 x2 = 282 x3 = 392
  SetMenuElementText id = helper_menu_1 helper_string_select_all
  SetMenuElementText id = helper_menu_2 helper_string_accept
  SetMenuElementText id = helper_menu_3 helper_string_back
  if InFrontEnd
    WaitForMenuToDisappear KeyboardControlMenus
  else
    if EditingPark
      WaitForMenuToDisappear KeyboardControlMenus
    endif
  endif
endscript

script helper_on_keyboard_control_linear
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  helper_set_positions x1 = 190 x2 = 350
  SetMenuElementText id = helper_menu_1 helper_string_select_all
  SetMenuElementText id = helper_menu_2 helper_string_accept
  if InFrontEnd
    WaitForMenuToDisappear KeyboardControlLinearMenus
  endif
endscript

script helper_on_keyboard_control_no_gui
  helper_hide
endscript

SelectChooseBackMenus = [ pre_trick_menu ]
SelectChooseBackRotateMenus = [ pre_cas_main_menu career_menu single_session_menu free_skate_menu player_one_menu net_type_menu create_new_warning ]
SelectChooseBackCenteredMenus = [ palmer_warning net_pppoe_setup_menu net_broadband_setup_menu net_device_type_menu net_dialup_setup_menu net_advanced_menu net_dialup_authentication_control net_options_use_defaults_menu options_main_menu net_network_setup_menu cdsound_menu cdso_menu movies_menu controller_config_menu replay_menu controller_select_primary ]
CasSelectChooseBackMenus = [ cas_push_menu cas_stance_menu cas_sex_menu heads_menu hair_menu jaws_menu hats_menu hat_logos_menu glasses_menu torsos_menu front_logos_menu back_logos_menu backpacks_menu accessories_menu chest_tattoos_menu back_tattoos_menu left_arm_tattoos_menu right_arm_tattoos_menu legs_menu socks_menu shoes_menu left_leg_tattoos_menu right_leg_tattoos_menu helmets_menu helmet_logos_menu elbowpads_menu kneepads_menu ]
CasChangeBackMenus = [ cas_age_menu cas_height_menu cas_weight_menu hair_hsv_menu hats_hsv_menu jaws_hsv_menu torsos_hsv_menu backpacks_hsv_menu legs_hsv_menu helmets_hsv_menu elbowpads_hsv_menu kneepads_hsv_menu ]
SelectChooseBackChangeMenus = [ ]
KeyboardControlMenus = [ cas_name_keyboard_control cas_hometown_keyboard_control cheat_keyboard_control net_password_control net_ip_address_control net_gateway_control net_network_id_control net_server_name_control net_dialup_number_control net_dialup_username_control net_dialup_password_control net_subnet_control net_dns_control net_dns_control2 net_host_name_control net_domain_name_control gapname_keyboard_Control mem_card_keyboard_control park_editor_keyboard_control sfx_menu ]
KeyboardControlLinearMenus = [ enter_password_control ]
ToggleChooseBackMenus = [ controller_config_1 controller_config_2 playlist_menu ]

script helper_scanner
  helper_hide
  begin
    //this is on pc
    if MenuIsSelected rr
      Goto helper_on_server_list
    endif
    if menuisshown cassette_menu
      Goto helper_on_cassette_menu
    endif
    if menuisshown SelectChooseBackMenus
      Goto helper_on_main_menu
    endif
    if menuisshown frontend_menu
      Goto helper_on_fe_menu
    endif
    if menuisshown SelectChooseBackRotateMenus
      Goto helper_select_choose_rotate_back
    endif
    if menuisshown SelectChooseBackCenteredMenus
      Goto helper_on_main_menu_centered
    endif
    if IsTrue MemCardYesNoMenuVisible
      Goto helper_on_mem_card_yes_no
    endif
    if IsTrue AutoLoadRetryContinueVisible
      Goto helper_on_autoload_retry_continue
    endif
    if menuisshown memcard_files_menu
      Goto helper_on_mem_card_files_menu
    endif
    if menuisshown pro_menu
      Goto helper_on_pro_menu
    endif
    if menuisshown KeyboardControlMenus
      Goto helper_on_keyboard_control
    endif
    if menuisshown KeyboardControlLinearMenus
      Goto helper_on_keyboard_control_linear
    endif
    if menuisshown cas_menu_container
      if menuisshown CasSelectChooseBackMenus
        Goto helper_on_cas_select_choose_back
      endif
      if menuisshown CasChangeBackMenus
        helper_on_cas_change_back
        WaitForMenuToDisappear CasChangeBackMenus
      endif
      if menuisshown cas_stats_menu
        if MenuIsSelected cas_left_container
        else
          helper_on_cas_select_change_back
          WaitForMenuToSelect cas_left_container
        endif
      endif
      //this is on pc
      if menuisshown cas_multi_menu
        Goto helper_on_cas_select_choose_back_4_arrows
      endif
    endif
    if menuisshown SelectChooseBackChangeMenus
      Goto helper_on_career_menu
    endif
    if menuisshown ToggleChooseBackMenus
      Goto helper_on_controller_config_menus
    endif
    if menuisshown player_two_menu
      Goto helper_on_player_two_menu
    endif
    if menuisshown boardshop_menu
      Goto helper_on_boardshop_menu
    endif
    if menuisshown wheel_color_menu
      Goto helper_on_wheel_color_menu
    endif
    if menuisshown deck_menu
      Goto helper_on_deck_menu
    endif
    if MenuIsSelected trick_menu_container
      if menuisshown trick_right_menu
        Goto helper_on_trick_right_menu
      endif
      if menuisshown trick_available_menu
        Goto helper_on_trick_available_menu
      endif
      if menuisshown special_trickname_menu
        Goto helper_on_special_trickname_menu
      endif
      if menuisshown available_specials_menu
        Goto helper_on_available_specials_menu
      endif
    endif
    if menuisshown ParkEditorMenus
      Goto helper_on_park_editor_select_choose_back_centered
    endif
    //this is on xbox, what is it?
    if menuisshown multi_menu
      Goto helper_select_choose_back params = { x1 = 147 x2 = 282 x3 = 412 }
    endif
    wait 1 gameframe
  repeat
endscript

script helper_on_cassette_menu
  begin
    if SkaterCamAnimHeld Skater = 0
      helper_level_select
      break
    endif
    wait 1 gameframe
  repeat
  begin
    if menuisshown cassette_menu
      if SkaterCamAnimHeld Skater = 0
      else
        Goto helper_scanner
      endif
    else
      Goto helper_scanner
    endif
    wait 1 gameframe
  repeat
endscript

script WaitForMenuToDisappear
  begin
    if GotParam callback
       <callback>
    endif
    if menuisshown <...>
    else
      Goto helper_scanner
    endif
    wait 1 gameframe
  repeat
endscript

script WaitForMenuToSelect
  begin
    if MenuIsSelected <...>
      Goto helper_scanner
    endif
    wait 1 gameframe
  repeat
endscript

script WaitForMenuToDeSelect
  begin
    if MenuIsSelected <...>
    else
      Goto helper_scanner
    endif
    wait 1 gameframe
  repeat
endscript

script helper_on_main_menu
  helper_hide
  if menuisshown pre_trick_menu
    if CurrentSkaterProfileIs 1
      helper_select_choose_back x1 = 295 x2 = 410 x3 = 520
    else
      helper_select_choose_back
    endif
  else
    helper_select_choose_back
  endif
  WaitForMenuToDisappear SelectChooseBackMenus
endscript

script helper_on_fe_menu
  helper_hide
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  helper_set_positions x1 = 80 x2 = 210
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 helper_string_accept
  WaitForMenuToDisappear frontend_menu
endscript

script helper_on_pro_menu
  helper_select_lr_choose_back_centered
  WaitForMenuToDisappear pro_menu
endscript

script helper_select_choose_back_centered
  helper_hide
  helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
endscript

script helper_on_main_menu_centered
  helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
  WaitForMenuToDisappear SelectChooseBackCenteredMenus
endscript

script helper_on_mem_card_yes_no
  helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
  begin
    if MenuIsSelected [ ken_line4 ken_line5 ken_line6 ken_line8 ken_line9 ]
    else
      Change MemCardYesNoMenuVisible = 0
      Goto helper_scanner
    endif
    wait 1 gameframe
  repeat
endscript

script helper_on_autoload_retry_continue
  helper_select_choose_back x1 = 187 x2 = 332
  FireHideEvent id = helper_menu_3 target = helper_menu_3
  begin
    if MenuIsSelected [ ken_line10 ken_line11 ]
    else
      Change AutoLoadRetryContinueVisible = 0
      Goto helper_scanner
    endif
    wait 1 gameframe
  repeat
endscript

script helper_on_mem_card_files_menu
  if FilesMenuIsSave
    helper_select_choose_back_delete
  else
    helper_select_choose_back x1 = 167 x2 = 282 x3 = 392
  endif
  WaitForMenuToDisappear memcard_files_menu
endscript

script helper_on_cas_select_choose_back
  helper_select_choose_rotate_back x1 = 67 x2 = 192 x3 = 345
  WaitForMenuToDisappear CasSelectChooseBackMenus
endscript

script helper_on_cas_select_choose_back_4_arrows
  helper_select_choose_rotate_back
  SetMenuElementText id = helper_menu_1 helper_string_select_all
  begin
    if menuisshown CasSelectChooseBackMenus
      Goto helper_scanner
    else
      if menuisshown CasChangeBackMenus
        Goto helper_scanner
      else
        if menuisshown cas_stats_menu
          if MenuIsSelected cas_left_container
          else
            Goto helper_scanner
          endif
        endif
        if menuisshown cas_multi_menu
        else
          Goto helper_scanner
        endif
      endif
    endif
    wait 1 gameframe
  repeat
endscript

script helper_on_cas_change_back
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  helper_set_positions
  MoveMenu id = helper_menu_1 x = 90
  MoveMenu id = helper_menu_2 x = 260
  SetMenuElementText id = helper_menu_1 "§ Change Ј"
  SetMenuElementText id = helper_menu_2 helper_string_back
endscript

script helper_on_cas_select_change_back
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  helper_set_positions
  MoveMenu id = helper_menu_1 x = 90
  MoveMenu id = helper_menu_2 x = 221
  MoveMenu id = helper_menu_3 x = 335
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "§ Change Ј"
  SetMenuElementText id = helper_menu_3 helper_string_back
endscript

script helper_on_cas_select_change_accept
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  helper_set_positions x1 = 167 x2 = 282 x3 = 392
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "§ Change Ј"
  SetMenuElementText id = helper_menu_3 helper_string_accept
endscript

script helper_on_cas_select_change_accept_back
  FireShowEvent id = helper_menu_1 target = helper_menu_1
  FireShowEvent id = helper_menu_2 target = helper_menu_2
  FireShowEvent id = helper_menu_3 target = helper_menu_3
  FireShowEvent id = helper_menu_4 target = helper_menu_4
  helper_set_positions
  MoveMenu id = helper_menu_1 x = 90
  MoveMenu id = helper_menu_2 x = 221
  MoveMenu id = helper_menu_3 x = 335
  MoveMenu id = helper_menu_4 x = 400
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "§ Change Ј"
  SetMenuElementText id = helper_menu_3 helper_string_accept
  SetMenuElementText id = helper_menu_4 helper_string_back
endscript

script helper_on_career_menu
  helper_select_choose_back
  WaitForMenuToDisappear SelectChooseBackChangeMenus
endscript

script helper_on_player_two_menu
  helper_select_choose_rotate_back
  WaitForMenuToDisappear player_two_menu
endscript

script helper_on_controller_config_menus
  helper_select_toggle_back
  WaitForMenuToDisappear ToggleChooseBackMenus
endscript

script helper_on_boardshop_menu
  begin
    if SkaterCamAnimHeld Skater = 0
      helper_select_choose_back
      break
    endif
    wait 1 gameframe
  repeat
  helper_set_positions x1 = 221 x2 = 360 x3 = 470 y = 390
  WaitForMenuToDisappear boardshop_menu
endscript

script helper_on_deck_menu
  helper_show
  helper_set_positions x1 = 221 x2 = 360 x3 = 470 y = 390
  SetMenuElementText id = helper_menu_1 "§ЈЎҐ Select"
  SetMenuElementText id = helper_menu_2 helper_string_accept
  SetMenuElementText id = helper_menu_3 helper_string_back
  WaitForMenuToDisappear deck_menu
endscript

script helper_on_wheel_color_menu
  helper_show
  helper_set_positions x1 = 221 x4 = 335 x3 = 470 x2 = 50 y = 390
  SetMenuElementText id = helper_menu_1 helper_string_select_lr
  SetMenuElementText id = helper_menu_2 "§Ј Change"
  SetMenuElementText id = helper_menu_3 helper_string_back
  FireShowEvent id = helper_menu_4 target = helper_menu_4
  SetMenuElementText id = helper_menu_4 helper_string_accept
  WaitForMenuToDisappear wheel_color_menu
endscript

script helper_on_server_list
  helper_hide
  if InInternetMode
  endif
  if InInternetMode
    helper_set_positions x1 = 445 x2 = 525 y = 400
  else
    helper_set_positions x2 = 470 y = 400
  endif
  if InInternetMode
    SetMenuElementText id = helper_menu_1 "§Ј Sort"
  endif
  SetMenuElementText id = helper_menu_2 helper_string_back
  WaitForMenuToDeSelect rr
endscript

ParkEditorHelper1x = 20 ParkEditorHelper1y = 402
ParkEditorHelper2x = 190 ParkEditorHelper2y = 402
ParkEditorHelper3x = 375 ParkEditorHelper3y = 402
ParkEditorHelper4x = 515 ParkEditorHelper4y = 402
ParkEditorHelper5x = 575 ParkEditorHelper5y = 115
ParkEditorHelper6x = 555 ParkEditorHelper6y = 382
ParkEditorHelper0x = 220
ParkEditorHelper0y = 377
ParkEditorGapMoveText1 = "Select Piece to Gap from"
ParkEditorGapMoveText2 = "Select Piece to Gap to"
ParkEditorGapMoveHelper4x = 490
ParkEditorGapAdjustHelper1x = 25
ParkEditorGapAdjustHelper2x = 145
ParkEditorGapAdjustHelper3x = 295
ParkEditorGapAdjustHelper4x = 490
ParkEditorGapAdjustHelpery = 402
ParkEditorGapAdjustText = "Ѕ/ѕ/ї/є Scale Gap"

script helper_park_editor
  killspawnedscript id = helper_scanner
  spawnscript helper_scanner id = helper_scanner
endscript

ParkEditorMenus = [ parked_main_menu parked_load_menu parked_save_menu parked_clear_menu parked_theme_menu parked_gap_menu parked_gap_score_menu parked_test_menu parked_yesno_menu1 parked_yesno_menu2 parked_yesno_menu3 parked_yesno_menu4 parked_zone_menu parked_zone_multiplier_menu parked_modify_menu ]

script helper_on_park_editor_select_choose_back_centered
  helper_select_choose_back x1 = 210 x2 = 330
  FireHideEvent id = helper_menu_3 target = helper_menu_3
  if EditingPark
    DrawFullEmpty
  endif
  WaitForMenuToDisappear ParkEditorMenus
endscript

script DrawFullEmpty
  FireShowEvent id = helper_menu_5 target = helper_menu_5
  MoveMenu id = helper_menu_5 x = 578 y = 112
  FireShowEvent id = helper_menu_6 target = helper_menu_6
  MoveMenu id = helper_menu_6 x = 558 y = 387
endscript 