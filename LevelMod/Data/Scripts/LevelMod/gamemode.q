
//ever used? some leftover?

//script launchgamemode
//  SetGameType <...>
//  SetGameState On
//  printf "script launchgamemode\n"
//  LaunchMenuScreen screen = game_menu
//endscript

//script launchgamemode_freeskate
//  launchgamemode FreeSkate
//endscript

//script launchgamemode_singlesession
//  launchgamemode SingleSession
//endscript

//script launchgamemode_trickattack
//  launchgamemode trickattack
//endscript

//script gamemode_menu_create
//  CreateMenu { type = verticalmenu id = gamemode_menu x = 160.0 y = 68.0 w = 320.0 h = 236.0 just_center_x just_center_y drawer = viewertext children = [ { type = textmenuelement id = freeskate_id text = "Free skate" eventhandler = { type = ChooseEventHandler target = "launchgamemode_freeskate" kill_menu } }
//      { type = textmenuelement id = singlesession_id text = "Single Session" eventhandler = { type = ChooseEventHandler target = "launchgamemode_singlesession" kill_menu } }
//      { type = textmenuelement id = trickattack_id text = "Trick Attack" eventhandler = { type = ChooseEventHandler target = "launchgamemode_trickattack" kill_menu } } ] }
//  AttachChild parent = contain1 child = gamemode_menu
//endscript

mode_info = [
  { name = "Free Skate" checksum = FreeSkate definition = "mode_freeskate" }
  { name = "2 Player Free Skate" checksum = FreeSkate2p definition = "mode_freeskate2p" }
  { name = "Single Session" checksum = SingleSession definition = "mode_singlesession" }
  { name = "Trick Attack" checksum = trickattack definition = "mode_trickattack" }
  { name = "Graffiti" checksum = graffiti definition = "mode_graffiti" }
  { name = "Slap!" checksum = slap definition = "mode_slap" }
  { name = "King of the Hill" checksum = king definition = "mode_king" }
  { name = "Horse" checksum = horse definition = "mode_horse" }
  { name = "Hamburger" checksum = hamburger definition = "mode_hamburger" }
  { name = "Strip Skate" checksum = stripskate definition = "mode_stripskate" }
  { name = "Career" checksum = Career definition = "mode_career" }
  { name = "Contest" checksum = contest definition = "mode_contest" }
  { name = "Lobby" checksum = netlobby definition = "mode_netlobby" }
  { name = "Trick Attack" checksum = nettrickattack definition = "mode_nettrickattack" }
  { name = "Graffiti" checksum = netgraffiti definition = "mode_netgraffiti" }
  { name = "Slap!" checksum = netslap definition = "mode_netslap" }
  { name = "King of the Hill" checksum = netking definition = "mode_netking" }
  { name = "Skate Shop" checksum = SkateShop definition = "mode_skateshop" }
  { name = "Park Editor" checksum = parkeditor definition = "mode_parkeditor" }
  { name = "Keep-Away" checksum = beachball definition = "mode_beachball" }
  { name = "Control Zone" checksum = ownthezone definition = "mode_ownthezone" }
  { name = "Capture the Flag" checksum = ctf definition = "mode_ctf" }
]

mode_freeskate = { name = FreeSkate initial_players = 1 max_players = 1 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 0 track_trick_score = 1 degrade_score = 0 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 1 should_run_intro_camera_noncd = 1 show_ranking_screen = 0 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_freeskate2p = { name = FreeSkate initial_players = 2 max_players = 2 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 0 track_trick_score = 1 degrade_score = 0 is_lobby = 0 is_frontend = 0 screenmode = splitscreen should_run_intro_camera = 1 should_run_intro_camera_noncd = 1 show_ranking_screen = 0 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_singlesession = { name = SingleSession initial_players = 1 max_players = 1 time_limit_type = fixed default_time_limit = 120 victory_condition_type = fixed accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 0 is_career = 0 is_singlesession = 1 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_career = { name = Career initial_players = 1 max_players = 1 time_limit_type = fixed default_time_limit = 120 victory_condition_type = fixed accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 1 should_run_intro_camera_noncd = 1 show_ranking_screen = 0 is_career = 1 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_trickattack = { name = trickattack initial_players = 2 max_players = 2 time_limit_type = config default_time_limit = 120 victory_condition_type = config accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = splitscreen victory_conditions = [ { type = highest_score } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 1 show_leader_messages = 1 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_nettrickattack = { name = nettrickattack initial_players = 1 max_players = 32 time_limit_type = config default_time_limit = 90 victory_condition_type = config accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single victory_conditions = [ { type = highest_score } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 1 show_leader_messages = 1 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_graffiti = { name = graffiti initial_players = 2 max_players = 2 time_limit_type = config default_time_limit = 120 victory_condition_type = config accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = splitscreen victory_conditions = [ { type = highest_score } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 1 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 1 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_netgraffiti = { name = netgraffiti initial_players = 1 max_players = 32 time_limit_type = config default_time_limit = 90 victory_condition_type = config accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single victory_conditions = [ { type = highest_score } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 1 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 1 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_slap = { name = slap initial_players = 2 max_players = 2 time_limit_type = config default_time_limit = 120 victory_condition_type = config accumulate_score = 1 track_trick_score = 0 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = splitscreen victory_conditions = [ { type = highest_score } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 1 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_netslap = { name = netslap initial_players = 1 max_players = 32 time_limit_type = config default_time_limit = 90 victory_condition_type = config accumulate_score = 1 track_trick_score = 0 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single victory_conditions = [ { type = highest_score } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 1 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }
mode_netking = { name = netking initial_players = 1 max_players = 32 time_limit_type = config default_time_limit = 0 victory_condition_type = config accumulate_score = 1 track_trick_score = 0 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single victory_conditions = [ { type = target_score Score = 120000 } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 1 is_trick_attack = 0 show_leader_messages = 1 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_king = { name = king initial_players = 2 max_players = 2 time_limit_type = config default_time_limit = 0 victory_condition_type = config accumulate_score = 1 track_trick_score = 0 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = splitscreen victory_conditions = [ { type = target_score Score = 120000 } ] should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 1 is_trick_attack = 0 show_leader_messages = 1 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
targetScoreArray = [ { type = target_score Score = 10000 } ]
mode_netlobby = { name = netlobby initial_players = 1 max_players = 8 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 0 track_trick_score = 1 degrade_score = 0 is_lobby = 1 is_frontend = 0 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 0 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_skateshop = { name = SkateShop initial_players = 1 max_players = 1 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 0 track_trick_score = 0 degrade_score = 0 is_lobby = 0 is_frontend = 1 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 0 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_parkeditor = { name = parkeditor initial_players = 1 max_players = 0 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 0 track_trick_score = 1 degrade_score = 0 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 0 is_career = 0 is_singlesession = 0 is_parkeditor = 1 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_horse = { name = horse initial_players = 2 max_players = 2 time_limit_type = config default_time_limit = 10 victory_condition_type = fixed accumulate_score = 1 track_trick_score = 1 degrade_score = 0 is_lobby = 0 is_frontend = 0 screenmode = horse should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 0 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 1 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_stagingarea = { name = stagingarea initial_players = 1 max_players = 32 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 0 track_trick_score = 0 degrade_score = 0 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 0 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 0 teamplay_type = fixed is_teamplay = 1 }
mode_beachball = { name = beachball initial_players = 1 max_players = 32 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 1 timer_beeps = 1 teamplay_type = config is_teamplay = 0 }
mode_ownthezone = { name = ownthezone initial_players = 1 max_players = 32 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 1 teamplay_type = config is_teamplay = 1 }
mode_ctf = { name = ctf initial_players = 1 max_players = 32 time_limit_type = fixed default_time_limit = 0 victory_condition_type = fixed accumulate_score = 1 track_trick_score = 1 degrade_score = 1 is_lobby = 0 is_frontend = 0 screenmode = single should_run_intro_camera = 0 should_run_intro_camera_noncd = 0 show_ranking_screen = 1 is_career = 0 is_singlesession = 0 is_parkeditor = 0 should_modulate_color = 0 is_horse = 0 is_king = 0 is_trick_attack = 0 show_leader_messages = 0 timer_beeps = 1 teamplay_type = fixed is_teamplay = 1 }

teamplay_info = [
    { name = "Team Play" checksum = num_0 }
    { name = "Free for all" checksum = num_1 }
]

crown_info = {
    name = "crown"
    mesh = "models/crown/crown.dff"
    texdir = "models/crown"
    tex = "models/crown/crown.tex"
    node_type_checksum = crown
    autoreturn = 0
}

flag1_info = {
    name = "blue flag"
    mesh = "models/blue_flg/blue_flg.dff"
    texdir = "models/blue_flg"
    tex = "models/blue_flg/blue_flg.tex"
    node_type_checksum = ctf_1
    autoreturn = 30
    scale = 0.35
    rotate_speed = 1
}

flag2_info = {
    name = "red flag"
    mesh = "models/red_flg/red_flg.dff"
    texdir = "models/red_flg"
    tex = "models/red_flg/red_flg.tex"
    node_type_checksum = ctf_2
    autoreturn = 30
    scale = 0.35
    rotate_speed = 1
}

ball_info = {
    Bounciness = 0.75
    name = "beach ball"
    mesh = "models/beachball/beachball.dff"
    texdir = "models/beachball"
    tex = "models/beachball/beachball.tex"
    node_type_checksum = crown
    BounceSound = beach_ball_bounce
    autoreturn = 30
    scale = 0.7
    rotate_speed = 2
}

key_info = {
    name = "Zone Key"
    mesh = "models/key/key.dff"
    texdir = "models/key"
    tex = "models/key/key.tex"
    node_type_checksum = zone_key
    autoreturn = 15
    scale = 0.4
    rotate_speed = 1
    offset = (-40, 0, 0)
    Bounciness = 0.25
}

zone_info = {
    node_type_checksum = zone
    alt_node_type_checksum = crown
    model_info = [
        { mesh = "models/cpblue/cpblue.dff" tex = "models/cpblue/cpblue.tex" }
        { mesh = "models/cpred/cpred.dff" tex = "models/cpred/cpred.tex" }
        { mesh = "models/cpwhite/cpwhite.dff" tex = "models/cpwhite/cpwhite.tex" }
    ]
}