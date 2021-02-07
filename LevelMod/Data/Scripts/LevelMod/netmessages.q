
SCRIPT NewScoreLeaderYou
	LaunchPanelMessage "&C1You have taken the lead!" properties = netmessageprops
	PlaySound HUD_specialtrickAA Vol = 100
ENDSCRIPT

SCRIPT NewScoreLeaderOther
	LaunchPanelMessage "&c1%s0 &c2has taken the lead!" s0 = <String0> properties = netmessageprops
	PlaySound HUD_specialtrickAA Vol = 100
ENDSCRIPT

SCRIPT timer_runout_beep
	PlaySound TimeoutA Vol = 75 pitch = 110
ENDSCRIPT

SCRIPT GraffitiStealYou
	LaunchPanelMessage "You stole from &c1%s0&c2!" s0 = <String0> properties = netmessageprops
	PlaySound HUD_specialtrickAA Vol = 100
ENDSCRIPT

SCRIPT GraffitiStealOther
	LaunchPanelMessage "&c1%s0&c2 stole from you!" s0 = <String0> properties = netmessageprops
	PlaySound HUD_specialtrickAA Vol = 100
ENDSCRIPT

SCRIPT SkaterCollideBail
	InBail
	PlaySound HUDtrickslopC
	IF InNetGame
		LaunchPanelMessage properties = netmessageprops s0 = <String0> random( 
		@"&c2You got smacked by &c1%s0"
		@"&c1%s0 &c2just dropped the hammer on you"
		@"&c1%s0 &c2is taking all comers"
		@"&c1%s0 &c2caught you wankin'"
		@"&c1%s0 &c2shows you to the kiddie park "
		@"&c1%s0 &c2says this is THPS3, not THP YOU SUCK"
		@"&c2Sorry, &c1%s0 &c2meant to kick your face"
		@"&c2Another classic neck breaker by &c1%s0"
		@"&c1%s0 &c2is SO much better than you, it's sick"
		@"&c1%s0 &c2says stick to scooters"
		@"&C2You gonna let &c1%s0 &c2treat you like that?"
		@"&c2Hint, stay away from &c1%s0"
		@"&c2Face plant, courtesy of &c1%s0"
		@"&c1%s0 &c2set you up the bomb"
		@"&c2All your base are belong to &c1%s0"
		@"&c1%s0 &c2wipes you off his shoe"
		@"&c1%s0 &c2rejected you like a baboon heart"
		@"&c1%s0 &c2wipes you off his shoe"
		@"&c1%s0 &c2offers you some inline skates"
		@"&c1%s0 &c2served you up a backhand"
		@"&c1%s0 &c2shanked you coldblooded"
		@"&c2You are &c1%s0's &c2prison boy"
		@"&c1%s0&c2 1, you 0"
		@"&c2Head slap, compliments of &c1%s0"
		@"&c1%s0 &c2kicked your ass. Plain and simple"
		@"&c1%s0 &c2is never soft"
	) ENDIF
	PlaySound bodysmackA
	IF InSlapGame
		ClearPanel_Bailed
		MoveToRandomRestart
	ENDIF
	goto NoseManualBail
ENDSCRIPT

SCRIPT MadeOtherSkaterBail
	SetTrickName "Head Smack"
	SetTrickScore 2000
	IF InNetGame
		IF ProfileEquals is_named = maul
			LaunchPanelMessage s0 = <String0> properties = netmessageprops random( 
			@"You force pushed &c1%s0"
			@"Lightsaber attack on &c1%s0"
			) PlaySound Saberhit1
		ELSE
			LaunchPanelMessage s0 = <String0> properties = netmessageprops random( 
			@"You smacked &c1%s0"
			@"You punked &c1%s0"
			@"You shanked &c1%s0"
			@"You slapped &c1%s0 &C0silly"
			@"You showed the door to &c1%s0"
			@"You dropped &c1%s0 &c0like a bad habit"
			@"You made &c1%s0 &c0lick the floor"
			@"You are &c1%s0's &c0daddy"
		) ENDIF
	ENDIF
	Display Blockspin
	OnGroundExceptions
	Clearexception Ex = RunHasEnded
	PlayAnim Anim = SlapRight Blendperiod = 0.3
	PlaySound bitchslap2
	WaitAnimWhilstChecking AndManuals
	CheckGapTricks
	ClearGapTricks
	ClearPanel_Landed
	goto OnGroundAi
ENDSCRIPT

SCRIPT MadeOtherSkaterBailAir
	SetTrickName "Head Smack"
	SetTrickScore 2000
	IF InNetGame
		IF ProfileEquals is_named = maul
			LaunchPanelMessage s0 = <String0> properties = netmessageprops random( 
			@"You force pushed &c1%s0"
			@"Lightsaber attack on &c1%s0"
			) PlaySound Saberhit1
		ELSE
			LaunchPanelMessage "Airborne face smack to &c1%s0!" s0 = <String0> properties = netmessageprops
		ENDIF
	ELSE
	ENDIF
	Display Blockspin
	IF doingtrick
	ELSE
		InAirExceptions
		PlayAnim Anim = SlapAir Blendperiod = 0.3
		PlaySound bodysmackA
		WaitAnimWhilstChecking
		goto Airborne
	ENDIF
ENDSCRIPT

SCRIPT MadeOtherSkaterBail_Called
	SetTrickName "Head Smack"
	SetTrickScore 2000
	IF InNetGame
		LaunchPanelMessage "Face smack to &c1%s0!" s0 = <String0> properties = netmessageprops
	ELSE
	ENDIF
	Display Blockspin
ENDSCRIPT

SCRIPT NetIdle
	IF ShouldMongo
		PlayAnim Anim = MongoBrakeIdle Cycle NoRestart
	ELSE
		PlayAnim Anim = BrakeIdle Cycle NoRestart
	ENDIF
ENDSCRIPT

SCRIPT server_enter_free_skate
	LaunchPanelMessage "Entering Free Skate." properties = netstatusprops
	LoadPendingPlayers
ENDSCRIPT

SCRIPT client_enter_free_skate
	LaunchPanelMessage "Entering Free Skate" properties = netstatusprops
ENDSCRIPT
props_shared_params = {
	font = "small.fnt"
	just = (0, 0)
	dims = (600, 112)
	colors = [
		{ (50, 80, 128) alpha = 128 }
		{ (180, 160, 0) alpha = 75 }
		{ (110, 50, 50) alpha = 75 }
		{ (128, 128, 128) alpha = 75 }
	]
}
netmessageprops = { font = "trick.fnt" just = (0, 0) dims = (600, 112) colors = [ { (50, 80, 128) alpha = 128 }
		{ (128, 128, 128) alpha = 75 }
		{ (110, 50, 50) alpha = 75 }
		{ (180, 160, 0) alpha = 75 } ] key_points = [ { pos = (320, 250) alpha = 0 scale = 0.2 time = 0 }
		{ alpha = 128 scale = 1.01 time = 0.09 }
		{ alpha = 128 scale = 1.01 time = 5 }
		{ alpha = 0 scale = 0.2 time = 5.09 } ] }
netstatusprops = { font = "trick.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 }
		{ (50, 80, 128) alpha = 75 }
		{ (110, 50, 50) alpha = 75 }
		{ (180, 160, 0) alpha = 75 } ] key_points = [ { pos = (320, 285) alpha = 0 scale = 0.2 time = 0 }
		{ alpha = 128 scale = 1.01 time = 0.09 }
		{ alpha = 128 scale = 1.01 time = 5 }
		{ alpha = 0 scale = 0.2 time = 5.09 } ] }
netexceptionprops = { font = "trick.fnt" just = (0, 0) dims = (600, 112) colors = [ { (128, 128, 128) alpha = 128 }
		{ (50, 80, 128) alpha = 75 }
		{ (110, 50, 50) alpha = 75 }
		{ (180, 160, 0) alpha = 75 } ] key_points = [ { pos = (320, 305) alpha = 0 scale = 0.2 time = 0 }
		{ alpha = 128 scale = 1.01 time = 0.09 }
		{ alpha = 128 scale = 1.01 time = 5 }
		{ alpha = 0 scale = 0.2 time = 5.09 } ] }
chat1props = {
	props_shared_params
	key_points = [
		{ pos = (164, 260) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 260) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat2props = {
	props_shared_params
	key_points = [
		{ pos = (164, 270) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 270) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat3props = {
	props_shared_params
	key_points = [
		{ pos = (164, 280) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 280) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat4props = {
	props_shared_params
	key_points = [
		{ pos = (164, 290) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 290) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat5props = {
	props_shared_params
	key_points = [
		{ pos = (164, 300) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 300) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat6props = {
	props_shared_params
	key_points = [
		{ pos = (164, 310) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 310) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat7props = {
	props_shared_params
	key_points = [
		{ pos = (164, 320) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 320) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat8props = {
	props_shared_params
	key_points = [
		{ pos = (164, 330) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 330) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
chat9props = {
	props_shared_params
	key_points = [
		{ pos = (164, 340) alpha = 0 scale = 0.1 time = 0 }
		{ alpha = 128 scale = 0.755 time = 0.2 }
		{ alpha = 128 scale = 0.655 time = 7 }
		{ pos = (-564, 340) alpha = 0 scale = 0.2 time = 7.5 }
	]
}
net_error_msg = 'Error'
net_notice_msg = 'Notice'
net_status_msg = 'Status'
net_refused_msg = 'Refused'
net_error_not_connected = 'Your network device is not properly connected to the network. Check your cables or contact your network administrator.'
net_error_not_detected = 'Could not detect any compatible network devices. Check your connections. See the THPS3 manual for troubleshooting.'
net_error_dhcp_error = 'Could not automatically detect network settings. Check connections and DHCP server or choose a static IP.'
net_error_changed_device = 'You have changed network device settings and must restart your system for this change to take effect.'
net_error_general_error = 'Your network device is not configured properly or may not be properly connected. See the THPS3 manual for troubleshooting.'
net_error_cant_change_device = 'You must restart your system before changing this option.'
net_reason_full_observers = 'The server will not allow any more observers at this time.'
net_reason_banned = 'You have been banned from this server.'
net_reason_full = 'The server is full.'
net_reason_wrong_password = 'Incorrect password.'
net_reason_version = 'Incompatible version.'
net_reason_general = 'Connection refused.'
net_message_game_in_progress = 'This game is in progress. Would you like to observe the remainder of this game and join automatically afterwards?'
net_message_waiting_for_players = 'Waiting for some players to fully load... Would you like to wait for them?'
net_message_server_removed_you = 'The server has chosen to remove you from this game.'
net_message_server_moved_on = 'The server decided to start the game without you.'
net_message_no_servers_found = 'No Servers Found'
net_message_new_king_other = '&C1%s0 &C0is king of the hill!'
net_message_new_king_you = 'You are king of the hill!'
net_message_new_player = '&C1%s0 &C0is joining the game'
net_message_dropped_crown_you = 'You dropped your crown!'
net_message_dropped_crown_other = 'The king has dropped his crown!'
net_message_changing_levels = 'Get Ready! Changing levels to &C1%s0....'
net_message_game_will_start = 'The game will start when all players are fully-loaded.'
net_message_join_pending = '&C1%s0 &C0is waiting to join.'
net_message_observing = '&C1%s0 &C0is now observing the game.'
net_message_joining = '&C1%s0 &C0is joining the game.'
net_message_starting_game = 'Get Ready! Starting a &C1%s0&C0 game....'
net_message_player_quit = '&C1%s0 &C0has left the game.'
net_message_player_timed_out = '&C1%s0 &C0has timed out.'
net_message_player_now_observing = '&C1%s0 &C0has chosen to observe.'
net_message_player_banned = 'Server has removed &C1%s0 &C0from the game.'
net_message_player_left_out = 'Server has dropped &C1%s0 &C0from the game.'
net_message_player_dropped = '&C1%s0 &C0had a bad connection and was dropped.'
net_status_nomap = 'Map missing.'
net_status_connecting = 'Connecting to Server....'
net_status_joining = 'Joining game....'
net_status_trying_password = 'Trying password....'
net_status_lost_connection = 'You have lost connection to the server.'
net_status_server_quit = 'The server has quit.'
net_status_join_timeout = 'Timed out joining server. Refresh your server list or try again. If this problem persists, check your firewall settings.'
net_status_not_posted = 'Your game was not posted on Gamespy. Check your network cables as well as any Firewall settings you may have.'
net_status_checking_motd = 'Connecting to master server....'
net_status_retry_motd = 'Reattempting to Connect to master server....'
net_status_getting_lobbies = 'Getting lobby list....'
net_status_gamespy_no_dns = 'Failed to connect to Gamespy. Check your Gateway and/or Firewall settings. See the THPS3 manual for troubleshooting.'
net_status_gamespy_no_connect = 'Failed to connect to Gamespy. Check your connection and settings. See the THPS3 manual for troubleshooting.'
net_status_motd_failed = 'Failed to connect to master server.'
net_status_testing_settings = 'Testing network settings....'
net_status_need_to_setup = 'You have yet to setup your network device and connection. Would you like to do so now?'
net_status_need_to_setup_dialup = 'You have yet to setup your connection settings. Would you like to do so now?'
net_modem_state_dialing = 'Dialing'
net_modem_state_connected = 'Connected. Logging in....'
net_modem_state_logged_in = 'Connected'
net_modem_state_disconnecting = 'Disconnecting....'
net_modem_state_hanging_up = 'Hanging up....'
net_modem_state_disconnected = 'Disconnected.'
net_modem_state_conencting = 'Connecting to ISP'
net_modem_error_no_modem = 'No compatible modems are attached.'
net_modem_error_timeout = 'Timed out connecting to service provider. Check your connection settings.'
net_modem_error_busy = 'The phone number you dialed is busy,'
net_modem_error_no_connect = 'Could not connect to ISP. Please check your ISP phone number.'
net_modem_error_no_dialtone = 'No dialtone detected. Make sure your phone line is properly connected.'
net_modem_error_during_connect = 'Connected, but could not log in. Check your username and password in your connection settings.'
score_title_trick_attack = 'Score'
score_title_graffiti = 'Tags'
score_title_king = 'Time'
score_title_slap = 'Slaps'
sort_title_list = 'Server List'
sort_title_name = 'Name'
sort_title_ping = 'Ping'
sort_title_players = 'Players'
sort_title_observers = 'Observers'
sort_title_mode = 'Mode'
sort_title_level = 'Level'
sort_title_skill = 'Skill'
lobby_full_title = 'Full'
lobby_lan_title = 'LAN Games'
net_message_has_respawned = 'The %s has respawned!'
net_message_starting_game_error = '&C1%s0&C0 not supported on this map.'
net_message_ctf_grab_you = 'You grabbed the flag!'
net_message_ctf_grab_teammate = '&C%d%s&C0 grabbed the flag!'
net_message_ctf_grab_other = '&C%d%s&C0 grabbed your flag!'
net_message_ctf_capture_you = 'You captured the flag!'
net_message_ctf_capture_teammate = '&C%d%s&C0 captured the flag!'
net_message_ctf_capture_other = '&C%d%s&C0 captured your flag!'
net_message_ctf_return_you = 'You returned the flag!'
net_message_ctf_return_teammate = '&C%d%s&C0 returned your flag!'
net_message_ctf_return_other = '&C%d%s&C0 returned the flag!'
net_message_ctf_drop_you = 'You dropped the flag!'
net_message_ctf_drop_teammate = '&C%d%s&C0 dropped the flag!'
net_message_ctf_drop_other = '&C%d%s&C0 dropped your flag!'
net_message_zone_you_lost_to = 'You lost a %dx zone to &c%d%s&c0!'
net_message_zone_you_lost = 'You lost a %dx zone!'
net_message_zone_you_own_from = 'You own a %dx zone from &c%d%s&c0!'
net_message_zone_1_lost_to_2 = '&c%d%s&c0 lost a %dx zone to &c%d%s&c0!'
net_message_zone_lost = '&c%d%s&c0 lost a %dx zone!'
net_message_zone_you_own = 'You own a %dx zone!'
net_message_zone_own = '&c%d%s&c0 owns a %dx zone!'
net_message_zone_all = 'Your team owned all zones and scores!'
net_message_zone_other = 'The %s owned all zones and scores!'
net_message_grab_key_other = '&C1%s &C0got the zone key!'
net_message_grab_key_you = 'You got the zone key!'
net_message_dropped_key_you = 'You dropped the zone key!'
net_message_dropped_key_other = '&C1%s &C0has dropped the zone key!'
net_message_you_joined = 'You joined the %s!'
net_message_joined = '%s joined the %s!'
net_message_new_ball_other = '&C1%s &C0got the ball!'
net_message_new_ball_you = 'You got the ball!'
net_message_dropped_ball_you = 'You dropped the ball!'
net_message_dropped_ball_other = '&C1%s &C0has dropped the ball!'
ss_message_left = '%s has left the game.'
ss_message_joined = '%s has joined the game.'
ss_message_level_changed = 'Level changed to %s.'
ss_message_gamemode_changed = 'Game mode changed to %s.'
ss_message_time_changed = 'Time limit changed to %s.'
ss_message_bad_msg_target = 'Bad private message.'
ss_message_bad_msg_format = 'Bad message format.'
ss_message_bad_player = 'Player %s not found.'
ss_message_msg_self = 'Cannot send private message to yourself!'
ss_message_system = '>* %s'
ss_message_private = '>*%s*: %s'
ss_message_normal = '>%s: %s'
ss_message_action = '>%s %s'
gs_message_init = 'Initializing GameSpy...'
gs_message_conn = 'Connecting to GameSpy...'
gs_message_connected = 'Connected to GameSpy!'
gs_message_plist = 'Updating player list...'
gs_message_plist_done = 'Finished updating players list.'
gs_message_slist = 'Retrieving server list...'
gs_message_joined = '%s has joined.'
gs_message_left = '%s has left.'
gs_message_err_disconnected = 'Disconnected: %s'
gs_message_err_init_failed = 'GameSpy initialization failed.'
gs_message_err_msg = 'Unknown error parsing message.'
gs_message_err_not_conn = 'Not connected to GameSpy.  Please reconnect.'
gs_message_err_couldnt_conn = 'Unable to connect to GameSpy.  Please retry in a few minutes.'
gs_message_err_nick_inuse = 'Error: Nick <%s> is in use.'
gs_message_err_nick_invalid = 'Error: Nick <%s> is invalid.'
gs_message_err_invalid_char = 'Error: Character <%c> in nick is invalid.'
gs_message_err_bad_ip = 'Bad connect ip address.'
gs_message_your_ip = 'Your IP address is: %s.'
gs_command_me = 'me'
gs_command_tell = 'tell'
gs_command_connect = 'connect'
gs_command_ip = 'ip'
gs_message_nickchanged = 'Nick changed.'
gs_message_nicknotchanged = 'Unable to change nick.'
gs_wintitle_filters = 'Filters'
gs_wintitle_options = 'Options'
gs_wintitle_not_con = 'Not Connected'
gs_wintitle_staging = 'Staging'
gs_wintitle_create = 'Create'
gs_wintitle_si = 'Server Info'
gs_wintitle_slist = 'Server List'