
//Options in this list will get added autmatically
//These options will override the original option
//If no connection is found the client will use the Value parameter

LM_HostOption_MenuItem = { Type = textmenuelement text = "Foo" target = "LM_ToggleHostOption" }

LM_HostOptions = [
    { Type = textmenuelement auto_id text = "LevelMod HostOptions" static dont_gray drawer = title }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bSpine Value = 1 id = LM_HostOption_bSpine_id override_true = LM_Control_bSpine params = { name = LM_HostOption_bSpine id = LM_HostOption_bSpine_id on = "Spine: Allowed" off = "Spine: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bBank Value = 1 id = LM_HostOption_bBank_id override_true = LM_Control_bBank params = { name = LM_HostOption_bBank id = LM_HostOption_bBank_id on = "Bank Drop: Allowed" off = "Bank Drop: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bAcid Value = 1 id = LM_HostOption_bAcid_id override_true = LM_Control_bAcid params = { name = LM_HostOption_bAcid id = LM_HostOption_bAcid_id on = "Acid Drop: Allowed" off = "Acid Drop: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bExtraTricks Value = 1 id = LM_HostOption_bExtraTricks_id override_true = LM_Control_bExtraTricks params = { name = LM_HostOption_bExtraTricks id = LM_HostOption_bExtraTricks_id on = "ExtraTricks: Allowed" off = "ExtraTricks: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bLimitTags Value = 0 id = LM_HostOption_bLimitTags_id override_false = LM_GameOption_bLimitTags  params = { name = LM_HostOption_bLimitTags id = LM_HostOption_bLimitTags_id off = "32 TagFix: Allowed" on = "32 TagFix: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bWallplant Value = 1 id = LM_HostOption_bWallplant_id override_true = LM_Control_bWallplant  params = { name = LM_HostOption_bWallplant id = LM_HostOption_bWallplant_id on = "Wallplant: Allowed" off = "Wallplant: Disallowed" } }
    { LM_HostOption_MenuItem  name = LM_HostOption_bWalliePlant Value = 1 id = LM_HostOption_bWalliePlant_id override_true = LM_Control_bWalliePlant  params = { name = LM_HostOption_bWalliePlant id = LM_HostOption_bWalliePlant_id on = "Wallieplant: Allowed" off = "Wallieplant: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bBoostPlant Value = 1 id = LM_HostOption_bBoostPlant_id override_true = LM_Control_bBoostPlant  params = { name = LM_HostOption_bBoostPlant id = LM_HostOption_bBoostPlant_id on = "Boostplant: Allowed" off = "Boostplant: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bButtSlap Value = 1 id = LM_HostOption_bButtSlap_id override_true = LM_Control_bButtSlap  params = { name = LM_HostOption_bButtSlap id = LM_HostOption_bButtSlap_id on = "Buttslap: Allowed" off = "Buttslap: Disallowed" } }
	
]


//maybe move to shader.q, when enough code for shaders??

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
	{ name = "LM_Control_SpineButton" Value = 0 }
	{ name = "LM_Control_bSpine" Value = 1 }
	{ name = "LM_Control_bAcid" Value = 1 }
	{ name = "LM_Control_bBank" Value = 1 }
	{ name = "LM_Control_bExtraTricks" Value = 1 }
	//0 = Normal speed
	//1 = Th4 Speed
	//2 = Fast Air Speed
	//3 = 10% faster
	//4 = 20% faster
	{ name = "LM_Control_AirTrickSpeed" Value = 0 }
	{ name = "LM_Control_bXinput" value = 0 }
	{ name = "LM_Control_bWalliePlant" Value = 1 }
	{ name = "LM_Control_bButtSlap" value = 1 }
	{ name = "LM_Control_bBoostPlant" value = 0 }
	{ name = "LM_Control_bWallplant" value = 1 }

	{ name = "LM_GUI_bShowHud" Value = 1 StartGame Do = UpdateShowHUD }
	{ name = "LM_GUI_bNewMenu" Value = 1 }
	{ name = "LM_GUI_bShowGrafCounter" Value = 1 }
	{ name = "LM_GUI_bTrickNotifications" Value = 1 }
	{ name = "LM_GUI_bNetName" Value = 0 StartGame Do = UpdateNetName }
	
	{ name = "LM_BugFix_bTeleFix" Value = 1 }
	{ name = "LM_BugFix_bSoundFix" Value = 1 }
	
	{ name = "LM_GameOption_bLimitTags" Value = 0 }
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

	{ name = "LM_GFX_eBuffering" value = 2 } //"Off" "Double" "Triple"
	{ name = "LM_GFX_eAntiAlias" value = 4 } //"Off" "auto" "2x" "4x" "8x"
	{ name = "LM_GFX_bFiltering" value = 1 }
	{ name = "LM_GFX_bFixStutter" value = 1 }
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

//shared option for a back menu item
LM_Menu_Shared_Back = {
	Type = textmenuelement 
	auto_id 
	text = "Back"
	target = "go_back"	
}


//=======================levelmod settings menus============================

SCRIPT MaybeAddHostOption
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

levelmod_menu_root_children = [
	//title
	{ Type = textmenuelement auto_id text = "LevelMod settings" static dont_gray drawer = title }

	//game options that affect gameplay
	{ Type = textmenuelement auto_id text = "Game Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_game items = game_menu_items title = "Game Options" } }
	
	//GUI options, like Show HUD, show GrafCounter, etc
	{ Type = textmenuelement auto_id text = "GUI Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_gui items = game_menu_items title = "GUI Options" } }
	
	//hardware graphics options (AA, filtering, 
	{ Type = textmenuelement auto_id text = "Graphics Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_gfx items = game_menu_items title = "Graphics Options" } }
	
	
	//Options that affect certain parts of th level
	{ Type = textmenuelement auto_id text = "Level Options" link = levelmod_menu_LevelOptions }	
		
	//Control options, like spine transfer, acid drop, etc
	{ Type = textmenuelement auto_id text = "Control Options" link = newSettingsMenu  
	target = "populate_game_options" params = { mask = cat_control items = game_menu_items title = "Control Options" } }
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
	{ Type = textmenuelement id = SpineButtonText_id text = "Foo" static dont_gray drawer = keyboard_property params = { id = SpineButtonText_id TextFromValue = LM_Control_SpineButton_Text name = LM_Control_SpineButton } }
	{ LM_Menu_Shared_Bool id = LM_Control_bSpine_id params = { name = LM_Control_bSpine id = LM_Control_bSpine_id on = "Spine Transfer: on" off = "Spine Transfer: off" } }
	{ LM_Menu_Shared_Bool id = LM_Control_bAcid_id params = { name = LM_Control_bAcid id = LM_Control_bAcid_id on = "Acid Drop: on" off = "Acid Drop: off" } }
	{ LM_Menu_Shared_Bool id = LM_Control_bBank_id params = { name = LM_Control_bBank id = LM_Control_bBank_id on = "Bank Drop: on" off = "Bank Drop: off" } }
	
	//Set the airtrick speed
	//0 = normal
	//1 = THPS4
	//2 = Fast 1
	{ Type = slidermenuelement id = LM_Control_AirTrickSpeed_id text = "Foo" lower = 0 upper = 4 delta = 1 start = 0 wrap = 0 right_side_w = 80 eventhandlers = [ {Type = showeventhandler target = "LM_SetOption" params = { id = LM_Control_AirTrickSpeed_id TextFromValue = LM_Control_AirTrickSpeed_Text TextOnly } }{ Type = ContentsChangedEventHandler target = "LM_SetOption" params = { name = LM_Control_AirTrickSpeed id = LM_Control_AirTrickSpeed_id TextFromValue = LM_Control_AirTrickSpeed_Text} } ] }
	
	//goes back to previous menu
	{ LM_Menu_Shared_Back Params = { id = LevelMod_menu_air } } 
]

//Control->Air menu struct
LevelMod_menu_wall = { 
	LM_Menu_Shared_Vertical
	id = LevelMod_menu_wall
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = LevelMod_menu_wall }
	children = levelmod_menu_wall_items
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



levelmod_HostOptions_root = {
	LM_Menu_Shared_Vertical 
	id = levelmod_HostOptions_root
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = levelmod_HostOptions_root }
	children = LM_HostOptions
}

SCRIPT AddHostOption
    printf "Adding HostOption"
    IF GotParam On
        AddLine parent = levelmod_HostOptions_root Type = textmenuelement id = <id> text = <On>
	ELSE
	    GetParam On
		GetParam id
	    AddLine parent = levelmod_HostOptions_root Type = textmenuelement id = <id> text = <On>
	ENDIF
ENDSCRIPT

SCRIPT UpdateHostOptions
    printf "OnShow"
    ForEachIn LM_HostOptions do = AddHostOption params = { <...> }
ENDSCRIPT


SCRIPT CreateLevelModMenus
	Settings_CreateOptionsMenu //items = game_menu_items

	
	//adds levelmod menus
	CreateAndAttachMenu { Levelmod_menu_Root }
	//CreateAndAttachMenu { LevelMod_menu_Control }
	//CreateAndAttachMenu { LevelMod_menu_GUI }
	//CreateAndAttachMenu { LevelMod_menu_GameOptions }
	CreateAndAttachMenu { LevelMod_menu_Air }
	CreateAndAttachMenu { LevelMod_menu_Wall }
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
	   IF GotParam do
           Goto <do>
	   ELSE
	       GetParam do
		   Goto <do>
	   ENDIF
   ENDIF
ENDSCRIPT

SCRIPT OptionsOnStartGame
   IF GotParam StartGame
       printf "Going to spawn Script"
	   IF GotParam do
           Goto <do>
	   ELSE
	       GetParam do
		   Goto <do>
	   ENDIF
   ENDIF
ENDSCRIPT



SCRIPT sAddOption
    AddOption Name = <Name> Value = <Value>
ENDSCRIPT

SCRIPT AddOptions
	ForEachIn LevelModOptions do = sAddOption params =  <...>
ENDSCRIPT
 
SCRIPT LM_SetOption
	printf "Setting option"
	IF LM_GotParam name
		GetParam name
		GetParam id
		printf "Going to set option"
		IF LM_GotParam TextOnly
			GetParam TextFromValue
			GetOptionText option = <name> text = <TextFromValue>
			printf "setting menu element text"
			SetMenuElementText id = <id> <text>
			
			LM_MaybeMakeStatic option = <name>
		ELSE
		    IF LM_GotParam Value
			    GetParam Value
				//Sets option from value
				IF SetOption <name> value = <Value>
				     GetParam TextFromValue
					 GetOptionText option = <name> text = <TextFromValue>
					 SetMenuElementText id = <id> <text>
				ENDIF
			ELSE
			    //Sets option from slider
				IF SetOption <name>
					printf "Getting option text"
					GetParam TextFromValue
					GetOptionText option = <name> text = <TextFromValue>
					printf "setting menu element text"
					SetMenuElementText id = <id> <text>
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
	//Check if this is the top item
	IF LM_GotParam auto_id
	 printf "LM_ToggleHost"
	ELSE
	
		//Is this a special menu?
		IF LM_GotParam link
			//Todo handle linked menus
			printf "Link"
			
		ELSE
			printf "called LM_ToggleHostOption"
		
			//give textonly param if we only want to update text
			IF #"Not" GotParam TextOnly
				printf "Toggle"
				ToggleHostOption <name>
			ENDIF
			
			//update text if we have item id
			//We need to use LM_GotParam because our param is inside a struct
			//And the normal GotParam function can't find it
			IF LM_GotParam id
			
				//GetParam will move the param from the struct to the script stack
				GetParam id
				IF LM_GotParam name
				    GetParam name
				ENDIF
				IF LM_GotParam TextFromValue
					printf "Updating TextFromValue"
					GetParam TextFromValue
					IF LM_GotParam option
					    GetParam option
					    GetOptionText option = <option> text = <TextFromValue>
					ELSE
					    GetOptionText option = <name> text = <TextFromValue>
					ENDIF
					SetMenuElementText id = <id> <text>
				ELSE
					IF IsOptionOn <name>
						GetParam on
						SetMenuElementText id = <id> <on>
						printf "on"
					ELSE
						GetParam off
						SetMenuElementText id = <id> <off>
						printf "off"
					ENDIF
				ENDIF
			ELSE
				printf "without menu id!"
			ENDIF
		ENDIF
		IF GotParam LinkedTo
		    IF #"Not" IsOptionOn <LinkedTo>
		        DestroyElement id = <id>
			ENDIF
		ELSE
		    IF LM_GotParam LinkedTo_id
		        GetParam LinkedTo
				IF GotParam Name
		            IF #"Not" IsOptionOn <name>
			            DestroyElement id = <LinkedTo_id>
			        ENDIF
				ENDIF
		    ENDIF
		ENDIF
	ENDIF
	
	IF GotParam action
		<action>
	ENDIF
	printf "Done"
ENDSCRIPT

//a generic toggle func to take option, item id and on off text
SCRIPT LM_ToggleOption 
	//Check if this is the top item
	IF LM_GotParam auto_id
	 printf "LevelModSettings"
	ELSE
	
		//Is this a special menu?
		IF LM_GotParam link
			//Todo handle linked menus
			
			
		ELSE
			printf "called LM_ToggleOption"
		
			//give textonly param if we only want to update text
			IF #"Not" GotParam TextOnly
				printf "Toggle"
				ToggleOption <name>
				
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
			
			//update text if we have item id
			//We need to use LM_GotParam because our param is inside a struct
			//And the normal GotParam function can't find it
			IF LM_GotParam id
			
				//GetParam will move the param from the struct to the script stack
				GetParam id
				IF LM_GotParam name
				    GetParam name
				ENDIF
				IF LM_GotParam TextFromValue
					printf "Updating TextFromValue"
					GetParam TextFromValue
					IF LM_GotParam option
					    GetParam option
					    GetOptionText option = <option> text = <TextFromValue>
					ELSE
					    GetOptionText option = <name> text = <TextFromValue>
					ENDIF
					SetMenuElementText id = <id> <text>
				ELSE
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
					ENDIF
				ENDIF
				LM_MaybeMakeStatic option = <name>
			ELSE
				printf "without menu id!"
			ENDIF
			ENDIF
		ENDIF
		IF GotParam LinkedTo
		    IF #"Not" IsOptionOn <LinkedTo>
		        DestroyElement id = <id>
			ENDIF
		ELSE
		    IF LM_GotParam LinkedTo_id
		        GetParam LinkedTo
				IF GotParam Name
		            IF #"Not" IsOptionOn <name>
			            DestroyElement id = <LinkedTo_id>
			        ENDIF
				ENDIF
		    ENDIF
		ENDIF
	ENDIF
	
	IF GotParam action
		<action>
	ENDIF
ENDSCRIPT

//intented to loop through list items and update item text based on selected option
SCRIPT UpdateMenuText
	ForEachIn <children> do = LM_ToggleOption params = { <...> TextOnly }
	printf "menu text init done"
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
	{ IsBool text = "251x Patch" 		option_id = item1	option = LM_GameOption_b251Patch 		toggle_id = item1_toggle cat_game }
	{ IsBool text = "No BW Manual"		option_id = item2	option = LM_GameOption_bFixBWManual 	toggle_id = item2_toggle cat_game }
	{ IsBool text = "Unlimited Tags" 	option_id = item3	option = LM_GameOption_bLimitTags 		toggle_id = item3_toggle cat_game }
	{ IsBool text = "Tele Stance Fix" 	option_id = item4	option = LM_BugFix_bTeleFix 			toggle_id = item4_toggle cat_game }
	{ IsBool text = "Ped Props"			option_id = item5	option = LM_Gameplay_bPedProps 			toggle_id = item5_toggle cat_game }
	{ IsBool text = "Debug Console" 	option_id = item6	option = LM_DebugOption_bDebugMode 		toggle_id = item6_toggle cat_game }

	{ 		 text = "Air"			option_id = item11 link = levelmod_menu_air toggle_id = item1_toggle  cat_control }	
	{ 		 text = "Wall"			option_id = item12 link = levelmod_menu_wall toggle_id = item2_toggle cat_control }	
	{ IsBool text = "Reverts" 		option_id = item13	option = LM_Control_bRevert 		toggle_id = item3_toggle cat_control }
	{ IsBool text = "Extra tricks"	option_id = item14	option = LM_Control_bExtraTricks 	toggle_id = item4_toggle cat_control }
	{ IsBool text = "XInput" 		option_id = item15	option = LM_Control_bXinput 		toggle_id = item5_toggle cat_control }

	{ IsBool text = "Extra Messages" 	option_id = item21 option = LM_GUI_bTrickNotifications 	toggle_id = item1_toggle cat_gui } 
	{ IsBool text = "Show HUD" 			option_id = item22 option = LM_GUI_bShowHud 			toggle_id = item2_toggle cat_gui Do = UpdateShowHUD } 
	{ IsBool text = "Player names" 		option_id = item23 option = LM_GUI_bNetName 			toggle_id = item3_toggle cat_gui Do = UpdateNetName } 
	{ IsBool text = "New Net Menu" 		option_id = item24 option = LM_GUI_bNewMenu 			toggle_id = item4_toggle cat_gui } 
	{ IsBool text = "GrafCounter" 		option_id = item25 option = LM_GUI_bShowGrafCounter 	toggle_id = item5_toggle cat_gui } 
	
	{ IsEnum text = "Buffering" 		option_id = item211 option = LM_GFX_eBuffering 	toggle_id = item1_toggle cat_gfx TextValues = [ "Off" "Double" "Triple" ] } 
	{ IsEnum text = "MSAA Level" 		option_id = item214 option = LM_GFX_eAntiAlias 	toggle_id = item4_toggle cat_gfx TextValues = [ "Off" "auto" "2x" "4x" "8x" ] } 
	{ IsBool text = "Texture Filtering" option_id = item215 option = LM_GFX_bFiltering 			toggle_id = item5_toggle cat_gfx } 
	{ IsBool text = "Fix Stuttering" 	option_id = item216 option = LM_GFX_bFixStutter 			toggle_id = item6_toggle cat_gfx } 
]


script Settings_CreateOptionsMenu

	//game options shared container
	CreateAndAttachMenu { 
		type = verticalmenu 
		Id = newSettingsMenu
		parent = contain1 
		x = 170.0 y = 90.0 w = 300.0
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

script populate_game_options
	DepopulateMenu id = game_options_names_menu
	DepopulateMenu id = game_options_on_off_menu

	SetMenuElementText <title> id = game_options_title
	
	ForeachIn <items> do = Settings_AddLine params = { <...> }
	Settings_AddLine back_menu_item target = "go_back" Params = { id = game_options_names_menu } 
endscript


//to avoid multiple nested IFs
//should rewrite this func in switch-case-ish logic
//like if isEnum call enum func, if IsBool call bool func etc.
script Settings_AddLine
	if GotParam <mask>
		if GotParam link
			AddLine { 
				parent = game_options_names_menu 
				Type = textmenuelement 
				id = <option_id>
				text = <text>
				target = <target>
				params = <params>
				w = 300.0
			}
		else
			AddLine { 
				parent = game_options_names_menu 
				Type = textmenuelement 
				id = <option_id>
				text = <text>
			}
		endif
		
		if GotParam toggle_id
			if GotParam IsEnum
				AddLine {
					parent = game_options_on_off_menu 
					Type = textmenuelement 
					id = <toggle_id>
				}
				GetOptionText option = <option> text = <TextValues>
				SetMenuElementText id = <toggle_id> <text>
			else
				AddLine {
					parent = game_options_on_off_menu 
					Type = textmenuelement 
					id = <toggle_id>
					text = " "
				}
			endif
		else
				AddLine {
					parent = game_options_on_off_menu 
					Type = textmenuelement 
					auto_id
					text = " "
				}
		endif
		
		if GotParam option
			LM_MaybeMakeStatic option = <option> option_id = <option_id>
		endif

		if GotParam IsBool
			AttachEventHandler { Type = ChooseEventHandler object = <option_id> target = "Settings_ToggleOption" params = { option = <option> toggle_id = <toggle_id> <...> } }
			Settings_UpdateBoolText { option = <option> toggle_id = <toggle_id> }
		endif
	endif
endscript

script Settings_ToggleOption
	ToggleOption <option>
	Settings_UpdateBoolText <...>
	if GotParam do
		GoTo <do>
	endif
endscript

script Settings_UpdateBoolText
	if IsOptionOn <option>
		SetMenuElementText "On" id = <toggle_id> 
	else
		SetMenuElementText "Off" id = <toggle_id> 
	endif
endscript