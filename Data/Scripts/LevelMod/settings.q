
//Options in this list will get added autmatically
//These options will override the original option
//If no connection is found the client will use the Value parameter

LM_HostOption_MenuItem = { Type = textmenuelement text = "Foo" target = "LM_ToggleHostOption" }

LM_HostOptions = [
    { Type = textmenuelement auto_id text = "LevelMod HostOptions" static dont_gray drawer = title }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bSpine Value = 1 id = LM_HostOption_bSpine_id override_true = LM_Control_bSpine params = { name = LM_HostOption_bSpine id = LM_HostOption_bSpine_id on = "Spine: Allowed" off = "Spine: Disallowed" } }
	{ LM_HostOption_MenuItem  name = LM_HostOption_bLimitTags Value = 0 id = LM_HostOption_bLimitTags_id override_false = LM_GameOption_bLimitTags  params = { name = LM_HostOption_bLimitTags id = LM_HostOption_bLimitTags_id off = "32 TagFix: Allowed" on = "32 TagFix: Disallowed" } }

]

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
	{ name = "LM_GameOption_b251Patch" value = 0 }
	{ name = "LM_DebugOption_bDebugMode" Value = 0 }
	{ name = "LM_LevelOption_TH4ProObjects" value = 0 StartGame Do = UpdateTH4ProObjects }
	{ name = "LM_LevelOption_TH4CompObjects" value = 0 StartGame Do = UpdateTH4CompObjects }	
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

//levelmod settings root menu struct
Levelmod_menu_root = { 
	LM_Menu_Shared_Vertical
	id = Levelmod_menu_root
	children = levelmod_menu_root_children
}

levelmod_menu_root_children = [
	//title
	{ Type = textmenuelement auto_id text = "LevelMod settings" static dont_gray drawer = title }

	//game options that affect gameplay
	{ Type = textmenuelement auto_id text = "Game Options" link = LevelMod_menu_GameOptions }
	
	//GUI options, like Show HUD, show GrafCounter, etc
	{ Type = textmenuelement auto_id text = "GUI Options" link = levelmod_menu_GUI	}
	
	//Options that affect certain parts of th level
	{ Type = textmenuelement auto_id text = "Level Options" link = levelmod_menu_LevelOptions }	
		
	//Control options, like spine transfer, acid drop, etc
	{ Type = textmenuelement auto_id text = "Control Options" link = levelmod_menu_control }	
			
	//goes back to previous menu
	{ LM_Menu_Shared_Back Params = { id = Levelmod_menu_root } } 
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



//Control menu struct
LevelMod_menu_control = { 
	LM_Menu_Shared_Vertical
	id = LevelMod_menu_control
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = LevelMod_menu_control }
	children = levelmod_menu_control_items
}

levelmod_menu_control_items = [ 
	{ Type = textmenuelement auto_id text = "Control Options" static dont_gray drawer = title }
	
	//Link to air options
	{ Type = textmenuelement auto_id text = "Air" link = levelmod_menu_air }	
	
	//Link to wall options
	{ Type = textmenuelement auto_id text = "Wall" link = levelmod_menu_wall }	


	//toggles between classic tricks and double tap extensions + some new additions
	{ LM_Menu_Shared_Bool id = LM_Control_bExtraTricks_id params = { name = LM_Control_bExtraTricks id = LM_Control_bExtraTricks_id on = "Extra tricks: on" off = "Extra tricks: off" } }
	
	//option to disable revert chain
	{ LM_Menu_Shared_Bool id = LM_Control_bRevert_id params = { name = LM_Control_bRevert id = LM_Control_bRevert_id on = "Reverts: on" off = "Reverts: off" } }

	//enables XInput support, restart required for this option to apply
	{ LM_Menu_Shared_Bool id = LM_Control_bXinput_id params = { name = LM_Control_bXinput id = LM_Control_bXinput_id on = "Xinput: on" off = "Xinput: off" } }

	//goes back to previous menu
	{ LM_Menu_Shared_Back Params = { id = LevelMod_menu_control } } 
	
	//old options
	//{ Type = textmenuelement id = tele_fix text = "Foo" target = "sToggleTeleFix" }
	//{ Type = textmenuelement id = sound_fix text = "Foo" target = "sToggleSoundFix" }
	//{ Type = textmenuelement id = tag_fix text = "Foo" target = "sToggleTagLimit" }
	//{ Type = textmenuelement id = grass_3d text = "Foo" target = "sToggle3DGrass" }
	//{ Type = textmenuelement id = print_debug text = "Foo" target = sTogglePrintDebug }
	//{ Type = textmenuelement id = show_console text = "Foo" target = sToggleShowConsole }
	//{ Type = textmenuelement id = spine_button text = "Spine Button" link = spine_button_menu } 
] 


//GameOptions menu struct
LevelMod_menu_GUI = { 
	LM_Menu_Shared_Vertical
	id = LevelMod_menu_GUI
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = LevelMod_menu_GUI }
	children = levelmod_menu_GUI_items
}

levelmod_menu_GUI_items = [
	{ Type = textmenuelement auto_id text = "GUI Options" static dont_gray drawer = title }
 	//option to remove annoying messages
	{ LM_Menu_Shared_Bool id = LM_GUI_bTrickNotifications_id params = { name = LM_GUI_bTrickNotifications id = LM_GUI_bTrickNotifications_id on = "Extra Messages: on" off = "Extra Messages: off" } } 
	
	//disables HUD completely, "pro" mode, "screenshot" mode
	{ LM_Menu_Shared_Bool id = LM_GUI_bShowHud_id params = { name = LM_GUI_bShowHud id = LM_GUI_bShowHud_id on = "Show HUD: on" off = "Show HUD: off" action = UpdateShowHUD } }
 
 	//disables HUD completely, "pro" mode, "screenshot" mode
	{ LM_Menu_Shared_Bool id = LM_GUI_bNetName_id params = { name = LM_GUI_bNetName id = LM_GUI_bNetName_id on = "Player names: on" off = "Player names: off" action = UpdateNetName } }
 
	 //The new LevelMod menu when you press esc
	{ LM_Menu_Shared_Bool id = LM_GUI_bNewMenu_id params = { name = LM_GUI_bNewMenu id = LM_GUI_bNewMenu_id on = "New Net Menu: on" off = "New Net Menu: off" } }
	
	//This counts your tags
	//currently only works correctly when you play alone
	{ LM_Menu_Shared_Bool id = LM_GUI_bShowGrafCounter_id params = { name = LM_GUI_bShowGrafCounter id = LM_GUI_bShowGrafCounter_id on = "GrafCounter: on" off = "GrafCounter: off" } }
	
	//goes back to previous menu
	{ LM_Menu_Shared_Back Params = { id = LevelMod_menu_GUI } } 
]


//GameOptions menu struct
LevelMod_menu_GameOptions = { 
	LM_Menu_Shared_Vertical
	id = LevelMod_menu_GameOptions
	eventhandler = { Type = showeventhandler target = "UpdateMenuText" params = LevelMod_menu_GameOptions }
	children = levelmod_menu_GameOptions_items
}

levelmod_menu_GameOptions_items = [
	{ Type = textmenuelement auto_id text = "Game Options" static dont_gray drawer = title }

	//251 patch
	{ LM_Menu_Shared_Bool id = LM_GameOption_b251Patch_id params = { name = LM_GameOption_b251Patch id = LM_GameOption_b251Patch_id on = "251x Patch: on" off = "251x Patch: off" } }
	
	//removes 32 tags limit
	{ LM_Menu_Shared_Bool id = LM_GameOption_bLimitTags_id params = { name = LM_GameOption_bLimitTags id = LM_GameOption_bLimitTags_id off = "Unlimited Tags: on" on = "Unlimited Tags: off" } }

	//This fixes Sk3_TeleportToNode
	//So that skater will have same orientation when it leave the teleport
	//As when it entered the teleport
	{ LM_Menu_Shared_Bool id = LM_BugFix_bTeleFix_id params = { name = LM_BugFix_bTeleFix id = LM_BugFix_bTeleFix_id on = "Tele Stance Fix: on" off = "Tele Stance Fix: off" } }
	
	//removes ped props (message and score bonus)
	{ LM_Menu_Shared_Bool id = LM_Gameplay_bPedProps_id params = { name = LM_Gameplay_bPedProps id = LM_Gameplay_bPedProps_id on = "Ped Props: on" off = "Ped Props: off" } }
		
	//enables debug console, restart required for this option to apply
	{ LM_Menu_Shared_Bool id = LM_DebugOption_bDebugMode_id params = { name = LM_DebugOption_bDebugMode id = LM_DebugOption_bDebugMode_id on = "Debug Console: on" off = "Debug Console: off" } }
		
	//goes back to previous menu
	{ LM_Menu_Shared_Back Params = { id = LevelMod_menu_GameOptions } } 
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
	//adds levelmod menus
	CreateAndAttachMenu { Levelmod_menu_Root }
	CreateAndAttachMenu { LevelMod_menu_Control }
	CreateAndAttachMenu { LevelMod_menu_GUI }
	CreateAndAttachMenu { LevelMod_menu_GameOptions }
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
		ForEachIn master_level_list do = sUpdateNetSky params = { <...> }
	ELSE
		LoadLevelGeometry sky = ""
	ENDIF
ENDSCRIPT

SCRIPT sUpdateNetSky
	IF GotParam load_script
		IF LevelIs <load_script>
			IF GotParam lev_sky
				LoadLevelGeometry sky = <lev_sky>
			ENDIF
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT UpdateFog
	ForEachIn master_level_list do = sUpdateFog params = { <...> }
ENDSCRIPT

SCRIPT sUpdateFog
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
ENDSCRIPT

//intented to loop through list items and update item text based on selected option
SCRIPT UpdateMenuText
	ForEachIn <children> do = LM_ToggleOption params = { <...> TextOnly }
	printf "menu text init done"
ENDSCRIPT
