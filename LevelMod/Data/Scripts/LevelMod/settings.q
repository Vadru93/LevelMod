
//Options in this list will get added autmatically
//These options will override the original option
//If no connection is found the client will use the Value parameter

lm_edit_keymap_keyboard_control = { 
type = keyboardcontrol 
id = lm_edit_keymap_keyboard_control 
x = 160.0 y = 115.0 w = 320.0 h = 336.0 
just_center_x just_center_y num_columns = 13 
gapname_charset min_chars = 0 max_chars = 16 
title_string = "Profile name:" eventhandlers = 
[ { type = ContentsChangedEventHandler target = 
"EditKeyMapControlAccept" params = 
{ field_id = display_name keyboard_id = lm_edit_keymap_keyboard_control  } }
] default_to_accept hide_on_close parent = contain1 }

LM_HostOption_MenuItem = { Type = textmenuelement text = "Foo" target = "LM_ToggleHostOption" }

LM_HostOptions = [
	{ Type = textmenuelement auto_id text = "LevelMod HostOptions" static dont_gray drawer = title }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bSpine Value = 1 id = LM_HostOption_bSpine_id override_true = LM_Control_bSpine params = { name = LM_HostOption_bSpine id = LM_HostOption_bSpine_id on = "Spine: Allowed" off = "Spine: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bBank Value = 1 id = LM_HostOption_bBank_id override_true = LM_Control_bBank params = { name = LM_HostOption_bBank id = LM_HostOption_bBank_id on = "Bank Drop: Allowed" off = "Bank Drop: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bAcid Value = 1 id = LM_HostOption_bAcid_id override_true = LM_Control_bAcid params = { name = LM_HostOption_bAcid id = LM_HostOption_bAcid_id on = "Acid Drop: Allowed" off = "Acid Drop: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bExtraTricks Value = 1 id = LM_HostOption_bExtraTricks_id override_true = LM_Control_bExtraTricks params = { name = LM_HostOption_bExtraTricks id = LM_HostOption_bExtraTricks_id on = "ExtraTricks: Allowed" off = "ExtraTricks: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bUnLimitTags Value = 1 id = LM_HostOption_bUnLimitTags_id override_true = LM_GameOption_bUnLimitTags  params = { name = LM_HostOption_bUnLimitTags id = LM_HostOption_bUnLimitTags_id on = "32 TagFix: Allowed" off = "32 TagFix: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bWallplant Value = 1 id = LM_HostOption_bWallplant_id override_true = LM_Control_bWallplant  params = { name = LM_HostOption_bWallplant id = LM_HostOption_bWallplant_id on = "Wallplant: Allowed" off = "Wallplant: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bWalliePlant Value = 1 id = LM_HostOption_bWalliePlant_id override_true = LM_Control_bWalliePlant  params = { name = LM_HostOption_bWalliePlant id = LM_HostOption_bWalliePlant_id on = "Wallieplant: Allowed" off = "Wallieplant: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bBoostPlant Value = 1 id = LM_HostOption_bBoostPlant_id override_true = LM_Control_bBoostPlant  params = { name = LM_HostOption_bBoostPlant id = LM_HostOption_bBoostPlant_id on = "Boostplant: Allowed" off = "Boostplant: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bButtSlap Value = 1 id = LM_HostOption_bButtSlap_id override_true = LM_Control_bButtSlap  params = { name = LM_HostOption_bButtSlap id = LM_HostOption_bButtSlap_id on = "Buttslap: Allowed" off = "Buttslap: Disallowed" } }
	
]

 
//maybe move to shader.q, when enough code for shaders??s

BlendModes = [
	//Oringial th4+ blendmodes, in order 0-15
	
	// ( 0 - 0 ) * 0 + Src
	//DIFFUSE = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_ONE dest_blend = D3DBLEND_ZERO }
	
	// ( Src - 0 ) * Src + Dst
	//ADD = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_SRCALPHA dest_blend = D3DBLEND_ONE }
	
	// ( Src - 0 ) * Fixed + Dst
	//ADD_FIXED = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_CONSTANTALPHA dest_blend = D3DBLEND_ONE }
	
	// ( 0 - Src ) * Src + Dst
	//SUBTRACT = { blend_op = D3DBLENDOP_REVSUBTRACT src_blend = D3DBLEND_SRCALPHA dest_blend = D3DBLEND_ONE }
	
	// ( 0 - Src ) * Fixed + Dst
	//SUB_FIXED = { blend_op = D3DBLENDOP_REVSUBTRACT src_blend = D3DBLEND_CONSTANTALPHA dest_blend = D3DBLEND_ONE }
	
	// ( Src - Dst ) * Src + Dst	
	//BLEND = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_SRCALPHA dest_blend = D3DBLEND_INVSRCALPHA }
	
	// ( Src - Dst ) * Fixed + Dst
	//BLEND_FIXED= { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_CONSTANTALPHA dest_blend = D3DBLEND_INVCONSTANTALPHA }
	
	// ( Dst - 0 ) * Src + 0
	//MODULATE = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_ZERO dest_blend = D3DBLEND_SRCALPHA }
	
	// ( Dst - 0 ) * Fixed + 0
	//MODULATE_FIXED = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_ZERO dest_blend = D3DBLEND_CONSTANTALPHA }
	
	// ( Dst - 0 ) * Src + Dst
	//BRIGHTEN = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_DESTCOLOR dest_blend = D3DBLEND_ONE }
	
	// ( Dst - 0 ) * Fixed + Dst
	//BRIGHTEN_FIXED = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_DESTCOLOR dest_blend = D3DBLEND_CONSTANTALPHA }
	
	// Treat as diffuse for now.
	//GLOSS_MAP = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_ONE dest_blend = D3DBLEND_ZERO }
	
	// ( Dst - 0 ) * Src(col) + 0 - specially for the shadow.
	//MODULATE_COLOR = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_ZERO dest_blend = D3DBLEND_SRCCOLOR }
	
	// Meaningless unless destination alpha is enabled.
	//BLEND_PREVIOUS_MASK = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_DESTALPHA dest_blend = D3DBLEND_INVDESTALPHA }
				
	// Meaningless unless destination alpha is enabled.
	//BLEND_INVERSE_PREVIOUS_MASK = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_INVDESTALPHA dest_blend = D3DBLEND_DESTALPHA }
							
	//ONE_INV_SRC_ALPHA = { blend_op = D3DBLENDOP_ADD src_blend = D3DBLEND_ONE dest_blend = D3DBLEND_INVSRCALPHA }
	
	
	//TODO Add your own blendmodes here
	//Engine will automatically assign a value based on the index in the list
	//Starting from value 16 since the first 15 values are reserved for th4+ blendmodes
	//To set a custom vertex- or pixelshader use vertex_shader = "filename" and/or pixel_shader = "filename"
	
	//For example WATEREFFECT = { blendmode = BLEND vertex_shader = "WaterEffect.vsh" pixel_shader = "WaterEffect.psh" }
	//Now WATEREFFECT will have blendmode 16 and I was thinking to use that "000000000" part of the texture like I had idea before
	//But since the game don't read this data, I will just read it directly from the file and assign accordingly
	//So 4 bytes will be blendmode and 4 bytes will be flags, like mipmaps animation, env map, etc
	//I also had the idea to use material extension, but not sure how game will like this, guess we can try and see
	
	
]




//I was thinking if we have a list of options
//Then we can have things like UpdateOn = ChangeLevel/StartGame etc
//Then in ChangeLevel function we can have ForEachIn and update grass etc
LevelModOptions = [
	{ name = "LM_Control_bRevert" Value = 1 }
	 //0 = Revert
	 //1 = Nollie
	 //2 = SpinLeft
	 //3 = SpinRight
	 //4 = Revert+Nollie
	 //5 = SpinLeft+SpinRight
	{ name = "LM_Control_SpineButton" Value = 0 max = 6 }
	{ name = "LM_Control_bSpine" Value = 1 }
	{ name = "LM_Control_bAcid" Value = 1 }
	{ name = "LM_Control_bBank" Value = 1 }
	{ name = "LM_Control_bExtraTricks" Value = 1 }
	//0 = Normal speed
	//1 = Th4 Speed
	//2 = Fast Air Speed
	//3 = 10% faster
	//4 = 20% faster
	{ name = "LM_Control_AirTrickSpeed" Value = 0 max = 5}
	{ name = "LM_Control_bXinput" value = 0 }
	{ name = "LM_Control_bWalliePlant" Value = 1 }
	{ name = "LM_Control_bButtSlap" value = 1 }
	{ name = "LM_Control_bBoostPlant" value = 0 }
	{ name = "LM_Control_bWallplant" value = 1 }
	{ name = "LM_Control_bInvertedX" value = 0 }
	{ name = "LM_Control_bInvertedY" value = 0 }

	{ name = "LM_GUI_bShowHud" Value = 1 StartGame Do = UpdateShowHUD }
	{ name = "LM_GUI_bNewMenu" Value = 1 }
	{ name = "LM_GUI_bShowGrafCounter" Value = 1 }
	{ name = "LM_GUI_bTrickNotifications" Value = 1 }
	{ name = "LM_GUI_bNetName" Value = 0 StartGame Do = UpdateNetName }
	
	{ name = "LM_BugFix_bTeleFix" Value = 1 }
	{ name = "LM_BugFix_bSoundFix" Value = 1 }
	
	{ name = "LM_GameOption_bUnLimitTags" Value = 1 }
	{ name = "LM_GameOption_bGrass" Value = 1 StartGame Do = UpdateGrass }
	{ name = "LM_GameOption_bExtraLayer" Value = 1 StartGame Do = UpdateExtraLayer }
	{ name = "LM_GameOption_bNetSky" Value = 0 StartGame Do = UpdateNetSky }
	{ name = "LM_GameOption_bFog" Value = 0 }
	{ name = "LM_Gameplay_bPedProps" value = 1 } //change to gameoption
	{ name = "LM_GameOption_b251Patch" value = 0 } //bugfix?
	{ name = "LM_GameOption_bFixBWManual" Value = 1 } //bugfix?
	{ name = "LM_DebugOption_bDebugMode" Value = 0 }
	
	{ name = "LM_LevelOption_TH4ProObjects" value = 0 StartGame Do = UpdateTH4ProObjects }
	{ name = "LM_LevelOption_TH4CompObjects" value = 0 StartGame Do = UpdateTH4CompObjects }

	{ name = "LM_GFX_eBuffering" value = 2 max = 3 } //"Off" "Double" "Triple"
	{ name = "LM_GFX_eAntiAlias" value = 4 max = 5 } //"Off" "auto" "2x" "4x" "8x"
	{ name = "LM_GFX_bFiltering" value = 1 }
	{ name = "LM_GFX_eFixStutter" value = 1 max = 4 } //"Off" "Exact" "Hybrid" "Sleep"
	{ name = "LM_GFX_bVSync" value = 0 }
	{ name = "LM_GFX_bWindowed" value = 0 }
	{ name = "LM_GFX_TargetFPS" value = 60 max = 300 }
]


//node compress items

//shared options for a vertical menu
LM_Menu_Shared_Vertical = {
	Type = verticalmenu
	parent = contain1
	x = 170.0 y = 90.0 w = 300.0 h = 400.0 
	just_center_x just_center_y blue_top 
}

//shared options for a bool menu item
LM_Menu_Shared_Bool = {	
	Type = textmenuelement 
	text = "Foo"
	target = "LM_ToggleOption"
}

script new_go_back
    if GotParam from
       SwitchOffMenu id = <From>
	   SwitchToMenu DoNotMakeRoot menu = <To>
	else
	    go_back id = <id>
    endif
endscript

//shared option for a back menu item
LM_Menu_Shared_Back = {
	Type = textmenuelement 
	auto_id 
	text = "Back"
	target = "new_go_back"	
}


//=======================levelmod settings menus============================

SCRIPT MaybeAddHostOption
	RestoreGoBack
	IF OnServer
		AddLine parent = Levelmod_menu_root Type = textmenuelement id = lm_hostoption_id text = "Host Options" link = levelmod_HostOptions_root
	ENDIF

	AddLine parent = Levelmod_menu_root Type = textmenuelement id = back_id text = "Back" target = "go_back" Params = { id = Levelmod_menu_root } 
ENDSCRIPT

//levelmod settings root menu struct
Levelmod_menu_root = { 
	LM_Menu_Shared_Vertical
	id = Levelmod_menu_root
	children = levelmod_menu_root_children
	eventhandler = { type = showeventhandler target = "MaybeAddHostOption" }
}

SCRIPT sFixGoBack
    MoveMenu id = game_options_names_menu y = 0
    MoveMenu id = game_options_on_off_menu y = 0
    EnableBackEvent
	FixGoBack
	if GotParam SelectCallback
	   SetMenuSelectCallback <SelectCallback>
	else
	    SetMenuSelectCallback NULL
	endif
ENDSCRIPT

SCRIPT EditControls_OnShow
    sFixGoBack
	MoveMenu id = game_options_names_menu y = 64
    MoveMenu id = game_options_on_off_menu y = 64
ENDSCRIPT

levelmod_menu_root_children = [
	//title
	{ Type = textmenuelement auto_id text = "LevelMod settings" static dont_gray drawer = title }

	//game options that affect gameplay
	{ Type = textmenuelement auto_id text = "Game Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_game items = game_menu_items title = "Game Options" OnShow = sFixGoBack params = { SelectCallback = LevelModSettingsMenu_OnSelect } }  }
	
	//GUI options, like Show HUD, show GrafCounter, etc
	{ Type = textmenuelement auto_id text = "GUI Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_gui items = game_menu_items title = "GUI Options" OnShow = sFixGoBack } }
	
	//hardware graphics options (AA, filtering, 
	{ Type = textmenuelement auto_id text = "Graphics Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_gfx items = game_menu_items title = "Graphics Options" OnShow = sFixGoBack  params = { SelectCallback = LevelModSettingsMenu_OnSelect } } }
	
	
	//Options that affect certain parts of th level
	{ Type = textmenuelement auto_id text = "Level Options" link = levelmod_menu_LevelOptions }	
		
	//Control options, like spine transfer, acid drop, etc
	{ Type = textmenuelement auto_id text = "Control Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_control items = game_menu_items title = "Control Options" OnShow = sFixGoBack  params = { SelectCallback = LevelModSettingsMenu_OnSelect } } }
	
	//Edit Controls
	{ Type = textmenuelement auto_id text = "Edit Controls" link = newSettingsMenu     
	target = "populate_game_options" params = { mask = cat_edit items = game_menu_items title = "Edit Controls" OnShow = EditControls_OnShow } }
]


//Level menu struct
levelmod_menu_LevelOptions = { 
	LM_Menu_Shared_Vertical
	id = levelmod_menu_LevelOptions
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = levelmod_menu_LevelOptions }
	children = levelmod_menu_LevelOptions_items
}

levelmod_menu_LevelOptions_items = [ 
	{ Type = textmenuelement auto_id text = "Level Options" static dont_gray drawer = title }

	//enables pseudo 3d layered grass in t2x and th4 levels
	{ LM_Menu_Shared_Bool id = LM_GameOption_bGrass_id params = { name = LM_GameOption_bGrass id = LM_GameOption_bGrass_id on = "3D Grass: on" off = "3D Grass: off" action = UpdateGrass } }

	//enables extra layers in th4+ levels
	{ LM_Menu_Shared_Bool id = LM_GameOption_bExtraLayer_id params = { name = LM_GameOption_bExtraLayer id = LM_GameOption_bExtraLayer_id on = "Extra Layers: on" off = "Extra Layers: off" action = UpdateExtraLayer } }

	//enables sky in network modes (an option cause of netname rendering bug) 
	{ LM_Menu_Shared_Bool id = LM_GameOption_bNetSky_id params = { name = LM_GameOption_bNetSky id = LM_GameOption_bNetSky_id on = "Net sky: on" off = "Net sky: off" action = UpdateNetSky } }
	
	//enables fog
	{ LM_Menu_Shared_Bool id = LM_GameOption_bFog_id params = { name = LM_GameOption_bFog id = LM_GameOption_bFog_id on = "Fog: on" off = "Fog: off" action = UpdateFog } }

	//this one needs a better drawer here?
	{ Type = textmenuelement auto_id text = "THPS4 levels" static dont_gray drawer = keyboard_property }

	//enables proset objects in thps4 levels 
	{ LM_Menu_Shared_Bool id = LM_LevelOption_TH4ProObjects_id params = { name = LM_LevelOption_TH4ProObjects id = LM_LevelOption_TH4ProObjects_id on = "Proset Objects: on" off = "Proset Objects: off" action = UpdateTH4ProObjects } }

	//enables competition objects in thps4 levels 
	{ LM_Menu_Shared_Bool id = LM_LevelOption_TH4CompObjects_id params = { name = LM_LevelOption_TH4CompObjects id = LM_LevelOption_TH4CompObjects_id on = "Comp Objects: on" off = "Comp Objects: off" action = UpdateTH4CompObjects } }

	//goes back to previous menu
	{ LM_Menu_Shared_Back Params = { id = levelmod_menu_LevelOptions } } 
] 


//Control->Air menu struct
LevelMod_menu_air = { 
	LM_Menu_Shared_Vertical
	id = LevelMod_menu_air
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = LevelMod_menu_air }
	children = levelmod_menu_air_items
	RestoreBack
}

levelmod_menu_air_items = [
	{ Type = textmenuelement auto_id text = "Air" static dont_gray drawer = title }
	
	//option to disable buttslap
	{ LM_Menu_Shared_Bool id = LM_Control_bButtSlap_id params = { name = LM_Control_bButtSlap id = LM_Control_bButtSlap_id on = "Buttslap: on" off = "Buttslap: off" } }
	
	//Sets spine button
	//0 = Revert
	//1 = Nollie
	//2 = SpinLeft
	//3 = SpinRight
	//4 = Revert+Nollie
	//5 = SpinLeft+SpinRight
	{ Type = textmenuelement id = LM_Control_SpineButton_id text = "Spine Button" link = spine_button_menu params = { name = LM_Control_SpineButton id = LM_Control_SpineButton_id } }	
	{ Type = textmenuelement id = SpineButtonText_id text = "Foo" static dont_gray drawer = keyboard_property params = { id = SpineButtonText_id TextFromValue = LM_Control_SpineButton_Text name = LM_Control_SpineButton UpdateText } }
	{ LM_Menu_Shared_Bool id = LM_Control_bSpine_id params = { name = LM_Control_bSpine id = LM_Control_bSpine_id on = "Spine Transfer: on" off = "Spine Transfer: off" } }
	{ LM_Menu_Shared_Bool id = LM_Control_bAcid_id params = { name = LM_Control_bAcid id = LM_Control_bAcid_id on = "Acid Drop: on" off = "Acid Drop: off" } }
	{ LM_Menu_Shared_Bool id = LM_Control_bBank_id params = { name = LM_Control_bBank id = LM_Control_bBank_id on = "Bank Drop: on" off = "Bank Drop: off" } }
	
	//Set the airtrick speed
	//0 = normal
	//1 = THPS4
	//2 = Fast 1
	{ Type = slidermenuelement id = LM_Control_AirTrickSpeed_id text = "Foo" lower = 0 upper = 4 delta = 1 start = 0 wrap = 0 right_side_w = 80 eventhandlers = [ {Type = showeventhandler target = "LM_SetOption" params = { id = LM_Control_AirTrickSpeed_id TextFromValue = LM_Control_AirTrickSpeed_Text TextOnly UpdateText } }{ Type = ContentsChangedEventHandler target = "LM_SetOption" params = { name = LM_Control_AirTrickSpeed id = LM_Control_AirTrickSpeed_id TextFromValue = LM_Control_AirTrickSpeed_Text UpdateText } } ] }
	
	//goes back to previous menu
	{ LM_Menu_Shared_Back Params = { id = LevelMod_menu_air } } 
]

//Control->Air menu struct
LevelMod_menu_wall = { 
	LM_Menu_Shared_Vertical
	id = LevelMod_menu_wall
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = LevelMod_menu_wall }
	children = levelmod_menu_wall_items
	RestoreBack
}

levelmod_menu_wall_items = [
	{ Type = textmenuelement auto_id text = "Wall" static dont_gray drawer = title }
	
	//option to disable wallieplant chain
	{ LM_Menu_Shared_Bool id = LM_Control_bWalliePlant_id params = { name = LM_Control_bWalliePlant id = LM_Control_bWalliePlant_id on = "Wallieplant: on" off = "Wallieplant: off" LinkedTo_id = LM_Control_bBoostPlant_id } }
	
	//option to disable wallieplant boostplant
	{ LM_Menu_Shared_Bool id = LM_Control_bBoostPlant_id LinkedTo = LM_Control_bWalliePlant params = { name = LM_Control_bBoostPlant id = LM_Control_bBoostPlant_id on = "Boostplant: on" off = "Boostplant: off" } }

	//option to disable wallplant
	{ LM_Menu_Shared_Bool id = LM_Control_bWallPlant_id params = { name = LM_Control_bWallPlant id = LM_Control_bWallPlant_id on = "Wallplant: on" off = "Wallplant: off" } }
	
	{ LM_Menu_Shared_Back Params = { id = LevelMod_menu_wall } } 
]

//Control->Edit->Load menu struct
load_keymap_menu = { 
	LM_Menu_Shared_Vertical
	id = load_keymap_menu
	eventhandler = { Type = showeventhandler target = "KeyMapMenu_OnShow" params = { SelectCallback = Load_KeyMapMenu_OnSelect } }
	children = load_keymap_menu_items
}

load_keymap_menu_items = [
	{ Type = textmenuelement auto_id               text = "Load Controls"    static dont_gray drawer = title }
	//This is used to display info text
	{ Type = textmenuelement id = load_info_text   text = ""                 static }
	//Load from input.map
	{ Type = textmenuelement id = load_game_id     text = "Load From Game"     target = "sKeyMapScript" params = { LoadGame Load } load_info_text = "Load File: Data\input.map" }
	//Load from settings.ini
	{ Type = textmenuelement id = load_settings_id text = "Load From Settings" target = "sKeyMapScript" params = { LoadSettings Load } load_info_text = "Load File: Controls.ini" }
	//Load defaults(currently not implemented)
	{ Type = textmenuelement id = load_default_id  text = "Load Defaults"      target = "sKeyMapScript" params = { LoadDefault Load } load_info_text = "Load Default Controls" }
	{ LM_Menu_Shared_Back Params = { from = load_keymap_menu to = newSettingsMenu } } 
	] 


//Control->Edit->Save menu struct
save_keymap_menu = { 
	LM_Menu_Shared_Vertical
	id = save_keymap_menu
	eventhandler = { Type = showeventhandler target = "KeyMapMenu_OnShow" params = { SelectCallback = Save_KeyMapMenu_OnSelect } }
	children = save_keymap_menu_items
}

save_keymap_menu_items = [
	{ Type = textmenuelement auto_id               text = "Save Controls"    static dont_gray drawer = title }
	//This is used to display info text
	{ Type = textmenuelement id = save_info_text   text = ""                 static }
	{ Type = textmenuelement id = save_info_text2  text = ""                 static }
	//Save to input.map
	{ Type = textmenuelement id = save_game_id     text = "Save To Game"     target = "sKeyMapScript" params = { SaveGame Save } save_info_text = "Save File: Data\input.map" save_info_text2 = "This will get loaded if you remove controls.ini" }
    //Save to controls.ini
	{ Type = textmenuelement id = save_settings_id text = "Save To Settings" target = "sKeyMapScript" params = { SaveSettings Save } save_info_text = "Save File: Controls.ini" save_info_text2 = "This will get loaded when game loads"  }
	//We need to figure out why back button keeps getting bugged...
	{ LM_Menu_Shared_Back Params = { from = save_keymap_menu to = newSettingsMenu } } 
	] 

//Maybe we should add something like this to all menus?
script Save_KeyMapMenu_OnSelect
    //This function can get param from array
	//1st param need to be the array
	//mask can be any number of params that you want to match
	//params can be any number of params you want to get
	//if function finds a match it will add the params to stack with the same name
    if GetParamFromArray save_keymap_menu_items params = { save_info_text save_info_text2 } mask = { id = <id> }
	    SetMenuElementText <save_info_text> id = save_info_text
		SetMenuElementText <save_info_text2> id = save_info_text2
	else
	    SetMenuElementText "" id = save_info_text
		SetMenuElementText "" id = save_info_text2
	endif
endscript

script Load_KeyMapMenu_OnSelect
    if GetParamFromArray load_keymap_menu_items params = { load_info_text } mask = { id = <id> }
	    SetMenuElementText <load_info_text> id = load_info_text
	else
	    SetMenuElementText "" id = load_info_text
	endif
endscript

script KeyMapMenuSetInfoText
    SetMenuElementText <...> id = edit_info_id
endscript

script KeyMapMenu_OnShow
    if GotParam SelectCallback
        SetMenuSelectCallback <SelectCallback>
	endif
    EnableBackEvent
	FixGoBack
endscript

levelmod_HostOptions_root = {
	LM_Menu_Shared_Vertical 
	id = levelmod_HostOptions_root
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = levelmod_HostOptions_root }
	children = LM_HostOptions
}


SCRIPT CreateLevelModMenus
Settings_CreateOptionsMenu //items = game_menu_items

	
	//adds levelmod menus
	CreateAndAttachMenu { Levelmod_menu_Root }
	//CreateAndAttachMenu { LevelMod_menu_Control }
	//CreateAndAttachMenu { LevelMod_menu_GUI }
	//CreateAndAttachMenu { LevelMod_menu_GameOptions }
	CreateAndAttachMenu { LevelMod_menu_Air }
	CreateAndAttachMenu { LevelMod_menu_Wall }
	CreateAndAttachMenu { load_keymap_menu }
	CreateAndAttachMenu { save_keymap_menu }
	CreateAndAttachMenu { levelmod_menu_LevelOptions }
	
	CreateAndAttachMenu { levelmod_HostOptions_root }

	CreateMenu { 
		Type = verticalmenu 
		id = spine_button_menu 
		x = 170.0 y = 90.0 w = 300.0 h = 400.0 
		just_center_x just_center_y blue_top 
		children = [ 
			{ Type = textmenuelement auto_id text = "Spine Button" static dont_gray drawer = title }
			{ Type = textmenuelement auto_id text = "Revert" target = "LM_SetOption" link = LevelMod_menu_air Params = { id = SpineButtonText_id name = LM_Control_SpineButton Value = 0 TextFromValue = LM_Control_SpineButton_Text } }
			{ Type = textmenuelement auto_id text = "Nollie" target = "LM_SetOption" link = LevelMod_menu_air Params = { id = SpineButtonText_id name = LM_Control_SpineButton Value = 1 TextFromValue = LM_Control_SpineButton_Text } }
			{ Type = textmenuelement auto_id text = "Left Spin Button" target = "LM_SetOption" link = LevelMod_menu_air Params = { id = SpineButtonText_id name = LM_Control_SpineButton Value = 2 TextFromValue = LM_Control_SpineButton_Text } }
			{ Type = textmenuelement auto_id text = "Right Spin Button" target = "LM_SetOption" link = LevelMod_menu_air Params = { id = SpineButtonText_id name = LM_Control_SpineButton Value = 3 TextFromValue = LM_Control_SpineButton_Text } } 
			{ Type = textmenuelement auto_id text = "Revert + Nollie" target = "LM_SetOption" link = LevelMod_menu_air Params = { id = SpineButtonText_id name = LM_Control_SpineButton Value = 4 TextFromValue = LM_Control_SpineButton_Text } } 
			{ Type = textmenuelement auto_id text = "Both Spin Buttons" target = "LM_SetOption" link = LevelMod_menu_air Params = { id = SpineButtonText_id name = LM_Control_SpineButton Value = 5 TextFromValue = LM_Control_SpineButton_Text } } ] }
	AttachChild parent = contain1 child = spine_button_menu
ENDSCRIPT


//toggle functions

SCRIPT UpdateGrass
	printf "Spawned Script UpdateGras..."
	IF IsOptionOn LM_GameOption_bGrass
		Create prefix = "3DGrassMesh"
		printf "Grass On"
	ELSE
		Kill prefix = "3DGrassMesh"
		printf "Grass Off"
	ENDIF
ENDSCRIPT

SCRIPT UpdateExtraLayer
	printf "Spawned Script UpdateExtraLayer..."
	IF IsOptionOn LM_GameOption_bExtraLayer
		Create prefix = "ExtraLayer"
	ELSE
		
		Kill prefix = "ExtraLayer"
	ENDIF
ENDSCRIPT

SCRIPT UpdateNetName
	IF IsOptionOn LM_GUI_bNetName
		TogglePlayerNames 1
	ELSE
		TogglePlayerNames 0
	ENDIF
ENDSCRIPT

SCRIPT UpdateShowHUD
	printf "Spawned Script UpdateShowHUD..."
	IF IsOptionOn LM_GUI_bShowHud
		TogglePanel 1
	ELSE
		TogglePanel 0
	ENDIF
ENDSCRIPT


SCRIPT UpdateNetSky
	IF IsOptionOn LM_GameOption_bNetSky
		ForEachIn master_level_list do = Settings_UpdateNetSky params = { <...> }
	ELSE
		LoadLevelGeometry sky = ""
	ENDIF
ENDSCRIPT

SCRIPT Settings_UpdateNetSky
	IF GotParam load_script
		IF LevelIs <load_script>
			IF GotParam lev_sky
				LoadLevelGeometry sky = <lev_sky>
			ENDIF
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT UpdateFog
	ForEachIn master_level_list do = Settings_UpdateLevelFog params = { <...> }
ENDSCRIPT

SCRIPT Settings_UpdateLevelFog
	IF GotParam load_script
		IF LevelIs <load_script>
			PrepareLevelFog <...>
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT DestroyMessageAndGoBack
    Wait 1.0 seconds
    SwitchOffMenu id = simple_menu
    SwitchToMenu DoNotMakeRoot <...>
ENDSCRIPT

SCRIPT sKeyMapScript
    if GotParam Load
	    DisplayMessage MessageScript = Message_LoadingData PauseLength = MemCardMessageShortPause
		if KeyMapScript <...>
		    DisplayMessage MessageScript = Message_LoadSuccessful
		else
		    DisplayMessage MessageScript = Message_LoadFailed
		endif
		SpawnScript DestroyMessageAndGoBack params = { menu = load_keymap_menu }
	    
	endif
    if GotParam Save
        DisplayMessage MessageScript = Message_SavingData PauseLength = MemCardMessageShortPause
		if KeyMapScript <...>
		    DisplayMessage MessageScript = Message_SaveSuccessful
		else
		    DisplayMessage MessageScript = Message_SaveFailed
		endif
		SpawnScript DestroyMessageAndGoBack params = { menu = save_keymap_menu }
    endif
	

ENDSCRIPT


SCRIPT UpdateTH4CompObjects
	IF IsOptionOn LM_LevelOption_TH4CompObjects
		Create prefix = "G_Comp_"
		Create prefix = "TRG_G_COMP_"
		Kill prefix = "G_COMPNOT_"
		Kill prefix = "TRG_G_COMPNOT_"
	ELSE
		Kill prefix = "G_Comp_"
		Kill prefix = "TRG_G_COMP_"
		Create prefix = "G_COMPNOT_"
		Create prefix = "TRG_G_COMPNOT_"
	ENDIF
ENDSCRIPT

SCRIPT UpdateTH4ProObjects
	IF IsOptionOn LM_LevelOption_TH4ProObjects
		Create prefix = "Proset"
		Create prefix = "TRG_Proset"
		Kill prefix = "PROSET1NOT_"
		Kill prefix = "PROSET2NOT_"
		Kill prefix = "PROSET3NOT_"
		Kill prefix = "PROSET4NOT_"
		Kill prefix = "PROSET5NOT_"
		Kill prefix = "PROSET6NOT_"
		Kill prefix = "PROSET7NOT_"
		Kill prefix = "TRG_PROSET1NOT_"
		Kill prefix = "TRG_PROSET2NOT_"
		Kill prefix = "TRG_PROSET3NOT_"
		Kill prefix = "TRG_PROSET4NOT_"
		Kill prefix = "TRG_PROSET5NOT_"
		Kill prefix = "TRG_PROSET6NOT_"
		Kill prefix = "TRG_PROSET7NOT_"
	ELSE
		Kill prefix = "Proset"
		Kill prefix = "TRG_Proset"
		Create prefix = "PROSET1NOT_"
		Create prefix = "PROSET2NOT_"
		Create prefix = "PROSET3NOT_"
		Create prefix = "PROSET4NOT_"
		Create prefix = "PROSET5NOT_"
		Create prefix = "PROSET6NOT_"
		Create prefix = "PROSET7NOT_"
		Create prefix = "TRG_PROSET1NOT_"
		Create prefix = "TRG_PROSET2NOT_"
		Create prefix = "TRG_PROSET3NOT_"
		Create prefix = "TRG_PROSET4NOT_"
		Create prefix = "TRG_PROSET5NOT_"
		Create prefix = "TRG_PROSET6NOT_"
		Create prefix = "TRG_PROSET7NOT_"
	ENDIF
ENDSCRIPT


SCRIPT OptionsOnChangeLevel
   IF GotParam ChangeLevel
	   printf "Going to spawn Script"
	   IF GetParam do
		   Goto <do>
	   ENDIF
   ENDIF
ENDSCRIPT

SCRIPT OptionsOnStartGame
   IF GotParam StartGame
	   printf "Going to spawn Script"
	   IF GetParam do
		   Goto <do>
	   ENDIF
   ENDIF
ENDSCRIPT

SCRIPT sLaunchGFXCommand command = Reset
	LaunchGFXCommand <command> params = <params>
ENDSCRIPT

SCRIPT sAddOption max = 2
	AddOption Name = <Name> Value = <Value> max = <max>
ENDSCRIPT

SCRIPT AddOptions
	ForEachIn LevelModOptions do = sAddOption params =  <...>
ENDSCRIPT

SCRIPT LM_SetOption_Do
	LM_SetOption <...>
	GoTo <Do> params = <params>
ENDSCRIPT

SCRIPT LM_SetOption_Slider
	if GotParam TextFromValue
		SetSliderText id = <id> enum = <TextFromValue>
	endif
	LM_SetOption <...>
ENDSCRIPT
 
SCRIPT LM_SetOption
	printf "Setting option"
	IF GetParam name
		GetParam id ForceUpdate
		printf "Going to set option"
		IF LM_GotParam TextOnly
			SetSliderValue option = <name>
			IF LM_GotParam UpdateText
				GetParam TextFromValue
				GetOptionText option = <name> text = <TextFromValue>
				printf "setting menu element text"
				SetMenuElementText id = <id> <text>
			ENDIF
			LM_MaybeMakeStatic option = <name>
		ELSE
			IF GetParam Value
				//Sets option from value
				IF SetOption <name> value = <Value>
					IF LM_GotParam UpdateText
						GetParam TextFromValue
						GetOptionText option = <name> text = <TextFromValue>
						SetMenuElementText id = <id> <text>
					ENDIF
				ENDIF
			ELSE
				//Sets option from slider
				IF SetOption <name>
					IF LM_GotParam UpdateText
						printf "Getting option text"
						GetParam TextFromValue
						GetOptionText option = <name> text = <TextFromValue>
						printf "setting menu element text"
						SetMenuElementText id = <id> <text>
					ENDIF
				ENDIF
			ENDIF
		ENDIF
		printf "DONE"
	ENDIF

ENDSCRIPT

//text too long. maybe should make menu wider
LM_Control_AirTrickSpeed_Text = [
	"Normal Trick Speed"
	"THPS4 Trick Speed"
	"Fast Air Trick Speed"
	"10 percent faster"
	"20 percent faster"
]

LM_Control_SpineButton_Text = [
	"Revert"
	"Nollie"
	"SpinLeft"
	"SpinRight"
	"Revert+Nollie"
	"SpinLeft+SpinRight"
]

SCRIPT LM_MaybeMakeStatic
//IsOptionOverriden will auto-generate an option_id if none is found
//It will generate it using syntax "[OptionName]_id"
	IF IsOptionOverriden <option>
		MakeTextMenuElementStatic <option_id>
	ELSE
		IF GotParam option_id
			MakeTextMenuElementStatic <option_id> nonstatic = 1
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT LM_ToggleHostOption 
	printf "called LM_ToggleHostOption"
    LM_ToggleOption  HostOption <...>
ENDSCRIPT

//a generic toggle func to take option, item id and on off text
SCRIPT LM_ToggleOption 
	//Check if this is the top item
	IF LM_GotParam auto_id
	    printf "LevelModSettings"
	ELSE
		    IF GetParam id ForceUpdate
			    GetParam name
				//Is this a special menu?
				IF LM_GotParam link
					//Todo handle linked menus
			
			
				ELSE
					printf "called LM_ToggleOption"
		
					//give textonly param if we only want to update text
					IF NotTrue GotParam TextOnly
						printf "Toggle"
						IF GotParam HostOption
				   		    ToggleHostOption <name>
						ELSE
				    		ToggleOption <name>
						ENDIF
				
						IF GotParam TextFromValue
							printf "Updating TextFromValue"
							IF GotParam option
								GetOptionText option = <option> text = <TextFromValue>
							ELSE
								GetOptionText option = <name> text = <TextFromValue>
							ENDIF
							SetMenuElementText id = <id> <text>
						ELSE
							IF IsOptionOn <name>
								SetMenuElementText id = <id> <on>
								printf "on"
							ELSE
								SetMenuElementText id = <id> <off>
							ENDIF
						ENDIF
					ELSE

				        IF GetParam TextFromValue
					    	printf "Updating TextFromValue"

					    	IF GetParam option
						    	GetOptionText option = <option> text = <TextFromValue>
					    	ELSE
								GetOptionText option = <name> text = <TextFromValue>
							ENDIF
							SetMenuElementText id = <id> <text>
						ELSE
						    //This is a new script that I added to be able to get debug info easier
							//Each param will get printed on a new line in the MessageBox
							//If you get Unknown string it means the qbtable for the param is not found
							//In non debugmode only settings.qb is loaded to save loading time and memory
							//So if you use MessageBox outside settings.qb you need to launch game in DebugMode
						    //MessageBox id = <id> name = <name> option = <option>
						    IF GotParam name
							    IF IsOptionOn <name>
								    GetParam on
								    SetMenuElementText id = <id> <on>
								    printf "on"
						        ELSE
							        GetParam off
							        SetMenuElementText id = <id> <off>
							        printf "off"
						        ENDIF
							ELSE
							    GetParam option
								IF IsOptionOn <option>
								    GetParam on
								    SetMenuElementText id = <id> <on>
								    printf "on"
						        ELSE
							        GetParam off
							        SetMenuElementText id = <id> <off>
							        printf "off"
						        ENDIF
							ENDIF
					    ENDIF
				    ENDIF
					
					//Make menu static if option is overriden or otherwise disabled
				    LM_MaybeMakeStatic option = <name>
		
		            IF GotParam LinkedTo
			            IF IsOptionOn <LinkedTo>
				            MakeTextMenuElementStatic <id> nonstatic = 1
					    ELSE
						    MakeTextMenuElementStatic <id>
					    ENDIF
				    ELSE
					    IF LM_GotParam LinkedTo_id
						    IF IsOptionOn <name>
								MakeTextMenuElementStatic <LinkedTo_id> nonstatic = 1
						    ELSE
							    MakeTextMenuElementStatic <LinkedTo_id> 
							ENDIF
					    ENDIF
				    ENDIF
				ENDIF	
			ELSE
				printf "without menu id!"
			ENDIF
	ENDIF
	
	IF GotParam action
		<action>
	ENDIF
ENDSCRIPT

//intented to loop through list items and update item text based on selected option
SCRIPT UpdateMenuText
    EnableBackEvent
	ForEachIn <children> do = LM_ToggleOption params = { TextOnly <...> }
	printf "menu text init done"
	IF GotParam RestoreBack
	    RestoreGoBack
	ENDIF
	SetMenuSelectCallback NULL
ENDSCRIPT





//******************************
// NEW SETTINGS CODE STARTS HERE
//******************************

back_menu_item = {
	text = "Back"						
	option_id = back_item
	toggle_id = back_item_toggle 
	link = Levelmod_menu_root
}

game_menu_items = [
    { static text = ""                  option_id = info_text                                                               toggle_id = info         cat_game cat_gfx cat_control }
	{ static text = ""                  option_id = info_text2                                                              toggle_id = info2        cat_gfx }
	
	{ IsBool text = "251x Patch" 		option_id = LM_GameOption_b251Patch_id	    option = LM_GameOption_b251Patch 		toggle_id = item1_toggle cat_game info_text = "Allow more than 251 multiplier" }
	{ IsBool text = "No BW Manual"		option_id = LM_GameOption_bFixBWManual_id	option = LM_GameOption_bFixBWManual 	toggle_id = item2_toggle cat_game info_text = "Fix backward manuals" }
	{ IsBool text = "Unlimited Tags" 	option_id = LM_GameOption_bUnLimitTags_id	option = LM_GameOption_bUnLimitTags 	toggle_id = item3_toggle cat_game info_text = "Allow more than 32 tags in 1 combo" }
	{ IsBool text = "Tele Stance Fix" 	option_id = LM_BugFix_bTeleFix_id	        option = LM_BugFix_bTeleFix 			toggle_id = item4_toggle cat_game info_text = "Fix stance after teleport" }
	{ IsBool text = "Ped Props"			option_id = LM_Gameplay_bPedProps_id	    option = LM_Gameplay_bPedProps 			toggle_id = item5_toggle cat_game info_text = "Ped props messages" }
	
	//since debug is now a separate dll, don't need this right?
	//{ IsBool text = "Debug Console" 	option_id = item6	option = LM_DebugOption_bDebugMode 		toggle_id = item6_toggle cat_game }

	{ 		 text = "Air"			option_id = levelmod_menu_air_id        link = levelmod_menu_air            toggle_id = item1_toggle cat_control info_text = "Air Options Menu" }	
	{ 		 text = "Wall"			option_id = levelmod_menu_wall_id       link = levelmod_menu_wall           toggle_id = item2_toggle cat_control info_text = "Wall Options Menu" }	
	{ IsBool text = "Reverts" 		option_id = LM_Control_bRevert_id	    option = LM_Control_bRevert 		toggle_id = item3_toggle cat_control info_text = "Allow Reverts" }
	{ IsBool text = "Extra tricks"	option_id = LM_Control_bExtraTricks_id	option = LM_Control_bExtraTricks 	toggle_id = item4_toggle cat_control info_text = "Allow Extra Tricks" }
	{ IsBool text = "XInput" 		option_id = LM_Control_bXinput_id	    option = LM_Control_bXinput 		toggle_id = item5_toggle cat_control info_text = "XINPUT controller support" }
	{ IsBool text = "Invert Cam X"  option_id = LM_Control_bInvertedX_id	option = LM_Control_bInvertedX 		toggle_id = item6_toggle cat_control info_text = "Invert camera Left/Right" }
	{ IsBool text = "Invert Cam Y"  option_id = LM_Control_bInvertedY_id	option = LM_Control_bInvertedY 		toggle_id = item7_toggle cat_control info_text = "Invert camera Up/Down" }
	//This freezes for some reason...
	//{        text = "Edit Controls" option_id = levelmod_menu_edit_id       link = newSettingsMenu              toggle_id = item8_toggle cat_control   target = "populate_game_options" params = { mask = cat_edit items = game_menu_items title = "Edit Controls" } }
	
	{        text = "Load"                   option_id = load_id             link = load_keymap_menu   toggle_id = toggle_load_id     cat_edit }
	{        text = "Save"                   option_id = save_id             link = save_keymap_menu   toggle_id = toggle_save_id     cat_edit }
	//Editable keys
	{ IsMap  text = "Up (¡)"                 option_id = up_id               KeyMap = Up               toggle_id = up_text_id         cat_edit }
	{ IsMap  text = "Down (¥)"               option_id = down_id             KeyMap = Down             toggle_id = down_text_id       cat_edit }
	{ IsMap  text = "Left (§)"               option_id = left_id             KeyMap = Left             toggle_id = left_text_id       cat_edit }
	{ IsMap  text = "Right (£)"              option_id = right_id            KeyMap = Right            toggle_id = right_text_id      cat_edit }
	{ IsMap  text = "Ollie (¬)"              option_id = ollie_id            KeyMap = X                toggle_id = ollie_text_id      cat_edit }
	{ IsMap  text = "Grind (ª)"              option_id = grind_id            KeyMap = Triangle         toggle_id = grind_text_id      cat_edit }
	{ IsMap  text = "Flip (©)"               option_id = flip_id             KeyMap = Square           toggle_id = flip_text_id       cat_edit }
	{ IsMap  text = "Grab («)"               option_id = grab_id             KeyMap = Circle           toggle_id = grab_text_id       cat_edit }
	{ IsMap  text = "Spin Left (ª1)"         option_id = spinleft_id         KeyMap = L1               toggle_id = spinleft_text_id   cat_edit }
	{ IsMap  text = "Spin Right"             option_id = spinright_id        KeyMap = R1               toggle_id = spinright_text_id  cat_edit }
	{ IsMap  text = "Revert"                 option_id = revert_id           KeyMap = R2               toggle_id = revert_text_id     cat_edit }
	{ IsMap  text = "Nollie (ª2)"            option_id = nollie_id           KeyMap = L2               toggle_id = nollie_text_id     cat_edit }
	{ IsMap  text = "Spine Transfer"         option_id = SpineTransfer_id    KeyMap = SpineTransfer    toggle_id = spine_text_id      cat_edit }
	{        text = "Press any key to Map"   option_id = edit_tip1           x = 0  y = -64 w = 300   toggle_id = edit1_id           cat_edit }
    {        text = "Press Back to unassign" option_id = edit_tip2           x = 0  y = -48 w = 300   toggle_id = edit2_id           cat_edit }
    {        text = "Press Escape to abort"  option_id = edit_tip3           x = 0  y = -32 w = 300   toggle_id = edit3_id           cat_edit }
	{        text = ""                       option_id = edit_error          x = 0  y = -16 w = 300   toggle_id = error_id           cat_edit }
    

	{ IsBool text = "Extra Messages" 	option_id = LM_GUI_bTrickNotifications_id  option = LM_GUI_bTrickNotifications 	toggle_id = item1_toggle cat_gui } 
	{ IsBool text = "Show HUD" 			option_id = LM_GUI_bShowHud_id             option = LM_GUI_bShowHud 			toggle_id = item2_toggle cat_gui Do = UpdateShowHUD } 
	{ IsBool text = "Player names" 		option_id = LM_GUI_bNetName_id             option = LM_GUI_bNetName 			toggle_id = item3_toggle cat_gui Do = UpdateNetName } 
	{ IsBool text = "New Net Menu" 		option_id = LM_GUI_bNewMenu_id             option = LM_GUI_bNewMenu 			toggle_id = item4_toggle cat_gui } 
	{ IsBool text = "GrafCounter" 		option_id = LM_GUI_bShowGrafCounter_id     option = LM_GUI_bShowGrafCounter 	toggle_id = item5_toggle cat_gui } 
	
	{ IsEnum text = "Buffering" 		option_id = LM_GFX_eBuffering_id  option = LM_GFX_eBuffering 	toggle_id = item1_toggle cat_gfx TextValues = [ "Off" "Double" "Triple" ] Do = sLaunchGFXCommand info_text = "Double/Tripple buffering" info_text2 = "May increase input lag" } 
	{ IsEnum text = "MSAA Level" 		option_id = LM_GFX_eAntiAlias_id  option = LM_GFX_eAntiAlias 	toggle_id = item2_toggle cat_gfx TextValues = [ "Off" "auto" "2x" "4x" "8x" ] Do = sLaunchGFXCommand info_text = "Anti Aliasing" info_text2 = "Disable to improve performance" } 
	{ IsBool text = "Windowed"		    option_id = LM_GFX_bWindowed_id   option = LM_GFX_bWindowed     toggle_id = item3_toggle cat_gfx Do = sLaunchGFXCommand params = { command = ToggleWindowed } info_text = "Launch game in windowed mode" info_text2 = "Toggle by pressing ALT + ENTER" }
	{ IsBool text = "Texture Filtering" option_id = LM_GFX_bFiltering_id  option = LM_GFX_bFiltering 	toggle_id = item5_toggle cat_gfx info_text = "Generate Texture mipmaps" info_text2 = "May produce slight graphic bugs"} 
	{ IsBool text = "Enable VSync" 	    option_id = LM_GFX_bVSync_id      option = LM_GFX_bVSync        toggle_id = item6_toggle cat_gfx Do = sLaunchGFXCommand info_text = "Sync rendering to monitor refresh rate" info_text2 = "Enable if you see tearing" } 
	{ IsInt  text = "FPS Lock:"	        option_id = LM_GFX_TargetFPS_id   option = LM_GFX_TargetFPS     toggle_id = item7_toggle cat_gfx min = 60 max = 300  Do = sLaunchGFXCommand params = { command = TargetFPS } info_text = "Set the target fps" info_text2 = "" }
	{ IsEnum text = "Stutter Fix" 	    option_id = LM_GFX_eFixStutter_id option = LM_GFX_eFixStutter   toggle_id = item8_toggle cat_gfx TextValues = [ "Off" "Exact" "Hybrid" "Sleep" ] Do = sLaunchGFXCommand params = { command = FixStutter } info_text = "Improve framerate consistency" info_text2 = "Exact is usually best option" } 
]


script Settings_CreateOptionsMenu

	//game options shared container
	CreateAndAttachMenu { 
		type = verticalmenu 
		Id = newSettingsMenu
		parent = contain1 
		x = 170.0 y = 40.0 w = 300.0
		just_center_x just_center_y blue_top 
		//eventhandler = { type = showeventhandler target = "populate_game_options" params = { mask = <cat> items = <items> } }
		children = [ 
			{ type = textmenuelement id = game_options_title text = "test" static dont_gray drawer = title }
			{ type = menucontainer Id = options_multi_container show_all_children }
		]
	}
	
	//column for menu items
	CreateAndAttachMenu { 
		type = verticalmenu 
		Id = game_options_names_menu 
		parent = options_multi_container 
		x = 0.0 y = 0.0 w = 240.0
		//num_visible = 12 
		just_center_x just_center_y not_rounded 
	}
	
	//column for setting state or enum values
	CreateAndAttachMenu { 
		type = verticalmenu 
		Id = game_options_on_off_menu 
		//follow_menu = game_options_names_menu 
		parent = options_multi_container 
		x = 240.0 y = 0.0 w = 60.0
		//num_visible = 12 
		just_center_x just_center_y not_rounded static dont_gray 
	}
endscript

script populate_game_options params = { }
	DepopulateMenu id = game_options_names_menu
	DepopulateMenu id = game_options_on_off_menu

	SetMenuElementText <title> id = game_options_title
	
	ForeachIn <items> do = Settings_AddLine params = { <...> }
	Settings_AddLine back_menu_item target = "go_back" Params = { id = game_options_names_menu } 
	IF GotParam OnShow
		Goto <OnShow> params = <params>
	ENDIF
endscript

script LevelModSettingsMenu_OnSelect
    if GetParamFromArray game_menu_items params = { info_text info_text2 } mask = { option_id = <id> }
	    SetMenuElementText <info_text> id = info_text
		SetMenuElementText <info_text2> id = info_text2
	else
	    SetMenuElementText "" id = info_text
		SetMenuElementText "" id = info_text2
	endif
endscript

script AddEnum
	GetMaximumIndex array = <TextValues>
	GetOptionValue <option>
	AddLine { 
		parent = game_options_names_menu 
		Type = slidermenuelement 
		id = <option_id> 
		text = <text> 
		lower = 0 upper = <max> delta = 1 start = <OptionValue> wrap = 1 right_side_w = 100
		eventhandlers = [ {Type = showeventhandler target = "LM_SetOption_Slider" params = { id = <option_id>  TextFromValue = <TextValues> TextOnly } }{ Type = ContentsChangedEventHandler target = "LM_SetOption_Do" params = { Do = <Do> params = <params> name = <option> id = <option_id> TextFromValue = <TextValues> } } ]
	}
	//if GotParam toggle_id
				//GetOptionText option = <option> text = <TextValues>
				//SetMenuElementText id = <toggle_id> <text>
	//endif
endscript

script AddKeyInput
    AddLine {
	parent = game_options_names_menu 
		Type = textmenuelement 
		id = <option_id>
		text = <text>
    }
	
	printf "Going to Attach"
	AttachEventHandler { Type = ChooseEventHandler object = <option_id> target = "Settings_EditKeyMap" params = { KeyMap = <KeyMap> text_id = <toggle_id> <...> } }
	printf "Going to update"
	Settings_UpdateKeyMapText { KeyMap = <KeyMap> text_id = <toggle_id> }
endscript

script AddBool
	AddLine { 
		parent = game_options_names_menu 
		Type = textmenuelement 
		id = <option_id>
		text = <text>
	}
		
	AttachEventHandler { Type = ChooseEventHandler object = <option_id> target = "Settings_ToggleOption" params = { option = <option> toggle_id = <toggle_id> <...> } }
	Settings_UpdateBoolText { option = <option> toggle_id = <toggle_id> }
endscript

script AddInt
	GetOptionValue <option>
	AddLine { 
		parent = game_options_names_menu 
		Type = slidermenuelement 
		id = <option_id> 
		text = <text> 
		lower = <min> upper = <max> delta = 1 start = <OptionValue> wrap = 1 right_side_w = 100
		eventhandlers = [ {Type = showeventhandler target = "LM_SetOption_Slider" params = { id = <option_id>  TextOnly } }{ Type = ContentsChangedEventHandler target = "LM_SetOption_Do" params = { Do = <Do> params = <params> name = <option> id = <option_id>  } } ]
	}
endscript

//to avoid multiple nested IFs
//should rewrite this func in switch-case-ish logic
//like if isEnum call enum func, if IsBool call bool func etc.
script Settings_AddLine params = {}
	if GotParam <mask>
	    		
		if GotParam toggle_id
			AddLine {
				parent = game_options_on_off_menu 
				Type = textmenuelement 
				id = <toggle_id>
				text = " "
			}
		else
			AddLine {
				parent = game_options_on_off_menu 
				Type = textmenuelement 
				auto_id
				text = " "
			}
		endif

		if GotParam link
			    AddLine { 
				    parent = game_options_names_menu 
				    Type = textmenuelement 
				    id = <option_id>
				    text = <text>
				    target = <target>
				    params = <params>
				    link = <link>
				    w = 300.0
			    }
		else
		    //MessageBox <option_id>
		    if GotParam IsEnum
			    AddEnum <...>
		    else
			    if GotParam IsInt
				    AddInt <...>
			    else
			        if GotParam IsBool
				       AddEnum <...> TextValues = [ "Off" "On" ]
					else
					    if GotParam IsMap
						    printf "Add KeyInput"
					        AddKeyInput <...>
						else 
						    if GotParam static
							    AddLine { 
								parent = game_options_names_menu 
								Type = textmenuelement 
								id = <option_id> 
								text = <text> 
								static 
								}
							else
							    AddLine { 
				                    parent = game_options_names_menu 
				                    Type = textmenuelement 
				                    id = <option_id>
				                    text = <text>
				                    static dont_gray drawer = main_warning x = <x> y = <y> w = <w> lock_layout lock_width
			                    }
							endif
					    endif
				    endif
			    endif
		    endif
		endif	

		if GotParam option
			LM_MaybeMakeStatic option = <option> option_id = <option_id>
		endif
	endif
endscript

script Settings_ToggleOption params = { }
	ToggleOption <option>
	Settings_UpdateBoolText <...>
	if GotParam do
		GoTo <do> params = <params>
	endif
endscript

script Settings_UpdateKeyMapText
    printf "GetText"
    GetTextFromKeyMap <KeyMap>
	printf "SetElement"
	SetMenuElementText <text> id = <text_id>
	printf "Update done"
endscript

script Settings_EditKeyMap
    EditKeyMap <KeyMap>
    DisableBackEvent
endscript

//Updates menu item text based on boolean setting
script Settings_UpdateBoolText
	VerifyParam param = option func = Settings_UpdateBoolText <...>
	VerifyParam param = toggle_id func = Settings_UpdateBoolText <...>
	
	if IsOptionOn <option>
		SetMenuElementText "On" id = <toggle_id> 
	else
		SetMenuElementText "Off" id = <toggle_id> 
	endif
endscript
