
SCRIPT Oil_Startup
	LoadStreamHeader "streams\oil\streams"
	LoadSound "Shared\GlassPane2X" vol = 100 dropoff = 100 pitch = 35 posupdateWithDoppler
	LoadSound "Shared\FallWater" vol = 100 pitch = 100
	LoadSound "Shared\FallPungee" vol = 100 pitch = 100
	LoadSound "Oil\Oil_Elevator" vol = 100 dropoff = 50 pitch = 100 posupdate
	LoadSound "Oil\Oil_elevator1" vol = 100 dropoff = 50 pitch = 100 posupdate
	LoadSound "Oil\Oil_Gate" vol = 100 dropoff = 50 pitch = 100 posupdate
	LoadSound "Oil\Oil_Door" vol = 100 dropoff = 50 pitch = 100 posupdate
	LoadSound "Oil\Oil_explode" vol = 1000 dropoff = 2000 pitch = 100 posupdate
	LoadSound "Oil\Oil_flame" vol = 200 dropoff = 200 pitch = 100 posupdate
	LoadSound "Oil\Oil_Flame2" vol = 200 dropoff = 200 pitch = 100 posupdate
	LoadSound "Oil\Oil_klaxon" vol = 200 pitch = 100
	LoadSound "Oil\Oil_steam" vol = 200 dropoff = 100 pitch = 125 posupdate
	LoadSound "Oil\Oil_steam2" vol = 200 dropoff = 100 pitch = 25 posupdate
	LoadSound "Oil\Oil_machinery" vol = 150 dropoff = 300 pitch = 100 posupdate
	LoadSound "Oil\Oil_machinery2" vol = 150 dropoff = 300 pitch = 100 posupdate
	LoadSound "Oil\Oil_pump2" vol = 100 dropoff = 250 pitch = 100 posupdate
	LoadSound "Oil\Skaterhelichop2b" vol = 1000 dropoff = 300 pitch = 110 posupdate
	LoadSound "Tok\TOK_secret2c" vol = 150 dropoff = 400 pitch = 100 posupdate
	LoadSound "Tok\TOK_secret4" vol = 100 pitch = 100
	LoadSound "Tok\TOK_secret5" vol = 100 pitch = 100
	LoadSound "Foun\foun_fan" vol = 100 dropoff = 50 pitch = 100 posupdate
	LoadSound "Oil\Can_Bird1" vol = 100 dropoff = 50 pitch = 100 posupdate
	LoadSound "Oil\Can_Bird2" vol = 100 dropoff = 50 pitch = 100 posupdate
	LoadSound "Ap\helicopter15" vol = 1000 dropoff = 300 pitch = 120 posupdate
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_00" visible = 0
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_01" visible = 0
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_02" visible = 0
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_03" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Fire" visible = 1
	SetParticleSysVisibility system = "Oil_Part_FireBlower" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_00" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_01" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_02" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_03" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_04" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_05" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Steam_00" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Steam_01" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Steam_02" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Steam_03" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Tank_00" visible = 1
	SpawnScript Oil_NetGame
ENDSCRIPT

SCRIPT JS_Oil_Retry
	LaunchConsoleMessage "firing reset"
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_00" visible = 0
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_01" visible = 0
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_02" visible = 0
	SetParticleSysVisibility system = "Oil_Part_BlowerUpper_03" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Fire" visible = 1
	SetParticleSysVisibility system = "Oil_Part_FireBlower" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Lo" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Hi" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_00" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_01" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_02" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_03" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_04" visible = 0
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_05" visible = 0
	SetParticleSysVisibility system = "Oil_Part_Steam_00" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Steam_01" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Steam_02" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Steam_03" visible = 1
	SetParticleSysVisibility system = "Oil_Part_Tank_00" visible = 1
ENDSCRIPT

SCRIPT Oil_NetGame
	IF InNetGame
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_00" visible = 0
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_01" visible = 0
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_02" visible = 0
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_03" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Fire" visible = 0
		SetParticleSysVisibility system = "Oil_Part_FireBlower" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Hi" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Hi" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Hi" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Hi" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Hi" visible = 0
		SetParticleSysVisibility system = "Oil_Part_HeliBlast_00" visible = 0
		SetParticleSysVisibility system = "Oil_Part_HeliBlast_01" visible = 0
		SetParticleSysVisibility system = "Oil_Part_HeliBlast_02" visible = 0
		SetParticleSysVisibility system = "Oil_Part_HeliBlast_03" visible = 0
		SetParticleSysVisibility system = "Oil_Part_HeliBlast_04" visible = 0
		SetParticleSysVisibility system = "Oil_Part_HeliBlast_05" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Steam_00" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Steam_01" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Steam_02" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Steam_03" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Tank_00" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_BC_Steam_00" visible = 0
	ENDIF
ENDSCRIPT

SCRIPT JS_BDJ_RampSettingsOn
	Obj_StickToGround On
ENDSCRIPT

SCRIPT JS_BDJ_RampSettingsOff
	Obj_StickToGround Off
	Obj_SetConstantHeight
ENDSCRIPT

SCRIPT JS_KillSkater_Lift_Up
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		Obj_SetFlag FLAG_SKATER_KILLING
		Obj_PlaySound Oil_Elevator
		LaunchPanelMessage properties = panel_message_death "&c1Goin Up!"
		SK3_Killskater_Finish poopoo r = 0 g = 0 b = 0
	ENDIF
ENDSCRIPT

SCRIPT JS_KillSkater_Lift_Down
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		Obj_SetFlag FLAG_SKATER_KILLING
		Obj_PlaySound Oil_Elevator
		LaunchPanelMessage properties = panel_message_death "&c1Goin Down!"
		SK3_Killskater_Finish poopoo r = 0 g = 0 b = 0
	ENDIF
ENDSCRIPT

SCRIPT JS_KillSkater_Helicopter
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		Obj_SetFlag FLAG_SKATER_KILLING
		Obj_PlaySound Skaterhelichop2b
		LaunchPanelMessage properties = panel_message_death "&c1Chop Chop!"
		SK3_Killskater_Finish poopoo r = 255 g = 0 b = 0
	ENDIF
ENDSCRIPT

SCRIPT JS_KillSkater_Steam
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		Obj_SetFlag FLAG_SKATER_KILLING
		LaunchPanelMessage properties = panel_message_death "&c1Steam Bath!"
		SK3_Killskater_Finish poopoo r = 255 g = 255 b = 255
	ENDIF
ENDSCRIPT

SCRIPT JS_Elevator_Controller
	BEGIN
		Create prefix = "Trp_Lift_Kill_1"
		Kill prefix = "Trp_Lift_Kill_2"
		Wait 10 seconds
		Kill prefix = "Trp_Lift_Kill_1"
		Create prefix = "Trp_Lift_Kill_2"
	REPEAT
ENDSCRIPT

SCRIPT JS_Prox_Elevator_Door_L1
	IF inside
		Kill prefix = "Oil_Lift_Door_L1_Static"
		Create prefix = "Oil_Lift_Door_L1_Open"
		Wait 0.5 seconds
		Kill prefix = "Oil_Lift_Door_L1_Open"
	ELSE
		Create prefix = "Oil_Lift_Door_L1_Close"
		Wait 0.5 seconds
		Kill prefix = "Oil_Lift_Door_L1_Close"
		Create prefix = "Oil_Lift_Door_L1_Static"
	ENDIF
ENDSCRIPT

SCRIPT JS_Prox_Elevator_Door_L2
	IF inside
		Kill prefix = "Oil_Lift_Door_L2_Static"
		Create prefix = "Oil_Lift_Door_L2_Open"
		Wait 0.5 seconds
		Kill prefix = "Oil_Lift_Door_L2_Open"
	ELSE
		Create prefix = "Oil_Lift_Door_L2_Close"
		Wait 0.5 seconds
		Kill prefix = "Oil_Lift_Door_L2_Close"
		Create prefix = "Oil_Lift_Door_L2_Static"
	ENDIF
ENDSCRIPT

SCRIPT JS_Prox_Elevator_Door_LW
	IF inside
		Kill prefix = "Oil_Lift_Door_LW_Static"
		Create prefix = "Oil_Lift_Door_LW_Open"
		Wait 0.5 seconds
		Kill prefix = "Oil_Lift_Door_LW_Open"
	ELSE
		Create prefix = "Oil_Lift_Door_LW_Close"
		Wait 0.5 seconds
		Kill prefix = "Oil_Lift_Door_LW_Close"
		Create prefix = "Oil_Lift_Door_LW_Static"
	ENDIF
ENDSCRIPT

SCRIPT JS_Gap_BlowerUpperTriggerA
	StartGap GapID = JS_Oil_BlowerUpperTrigger flags = [ CANCEL_MANUAL CANCEL_GROUND ]
ENDSCRIPT

SCRIPT JS_Gap_BlowerUpperTriggerB
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Oil_BlowerUpperTrigger Gapscript = JS_BlowerUpperTrigger
	ENDIF
ENDSCRIPT

SCRIPT JS_Gap_BlowerUpperTriggerC
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Oil_BlowerUpperTrigger text = "WHEEL LIP" score = 500
	ENDIF
ENDSCRIPT

SCRIPT JS_BlowerUpperTrigger
	IF InMultiplayerGame
		Create Name = TrP_BlowerUpper
		Create prefix = "TRG_BlowerWheel"
		Kill prefix = "TRG_BlowerTrigger"
		SpawnScript JS_Klaxon
	ELSE
		Create Name = TrP_BlowerUpper
		Create prefix = "TRG_BlowerWheel"
		Kill prefix = "TRG_BlowerTrigger"
		SetParticleSysVisibility system = "Oil_Part_Fire" visible = 0
		SetParticleSysVisibility system = "Oil_Part_FireBlower" visible = 1
		Kill Name = TRG_SFX_Pyre_Fire_On
		Create Name = TRG_SFX_Pyre_Fire_Off
		SpawnScript JS_Klaxon
	ENDIF
ENDSCRIPT

SCRIPT JS_Klaxon
	BEGIN
		Playsound Oil_klaxon
		Wait 1.5 seconds
	REPEAT 8
ENDSCRIPT

SCRIPT JS_BlowerUpper
	IF InMultiplayerGame
		Obj_PlaySound Oil_Flame2
		Wait 0.5 seconds
		Obj_PlaySound Oil_explode
		SpawnScript JS_PipeBurster
		Wait 0.5 seconds
		Kill prefix = "TrP_BlowerUpper"
		Kill prefix = "Oil_DOb_BlowerUpper_00_NW"
		Shatter Name = Oil_DOb_BlowerUpper_00_NW area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_00_SW"
		Shatter Name = Oil_DOb_BlowerUpper_00_SW area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_06"
		Create prefix = "TRG_OilPipe"
		Create prefix = "TRG_OilBowl"
		Kill prefix = "TRG_OilBloUp"
		Wait 0.5 seconds
		Kill prefix = "Oil_DOb_BlowerUpper_00_N"
		Shatter Name = Oil_DOb_BlowerUpper_00_N area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_00_W"
		Shatter Name = Oil_DOb_BlowerUpper_00_W area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_01"
		Kill prefix = "Oil_DOb_BlowerUpper_03"
		Shatter Name = Oil_DOb_BlowerUpper_03 area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_04"
		Wait 1.5 second
		Kill prefix = "Oil_DOb_BlowerUpper_02"
		Kill prefix = "Oil_DOb_BlowerUpper_05"
	ELSE
		Obj_PlaySound Oil_Flame2
		Wait 0.5 seconds
		Obj_PlaySound Oil_explode
		SpawnScript JS_PipeBurster
		Wait 0.5 seconds
		Kill prefix = "TrP_BlowerUpper"
		Kill prefix = "Oil_DOb_BlowerUpper_00_NW"
		Shatter Name = Oil_DOb_BlowerUpper_00_NW area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_00_SW"
		Shatter Name = Oil_DOb_BlowerUpper_00_SW area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_06"
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_00" visible = 1
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_01" visible = 1
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_02" visible = 1
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_03" visible = 1
		Create prefix = "TRG_OilPipe"
		Create prefix = "TRG_OilBowl"
		Kill prefix = "TRG_OilBloUp"
		Wait 0.5 seconds
		Kill prefix = "Oil_DOb_BlowerUpper_00_N"
		Shatter Name = Oil_DOb_BlowerUpper_00_N area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_00_W"
		Shatter Name = Oil_DOb_BlowerUpper_00_W area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_01"
		Kill prefix = "Oil_DOb_BlowerUpper_03"
		Shatter Name = Oil_DOb_BlowerUpper_03 area = 100000.0 variance = 0.6 vel_x = 0.0 vel_y = 200.0 vel_z = 0.0 spread = 2.0
		Kill prefix = "Oil_DOb_BlowerUpper_04"
		Wait 1.5 second
		Kill prefix = "Oil_DOb_BlowerUpper_02"
		Kill prefix = "Oil_DOb_BlowerUpper_05"
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_01" visible = 0
		Wait 0.5 seconds
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_02" visible = 0
		Wait 0.5 seconds
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_00" visible = 0
		SetParticleSysVisibility system = "Oil_Part_BlowerUpper_03" visible = 0
	ENDIF
ENDSCRIPT

SCRIPT JS_PipeBurster
	Create prefix = "Oil_Pipe_Burster_00"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_00"
	Create prefix = "Oil_Pipe_Burster_01"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_01"
	Create prefix = "Oil_Pipe_Burster_02"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_02"
	Create prefix = "Oil_Pipe_Burster_03"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_03"
	Create prefix = "Oil_Pipe_Burster_04"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_04"
	Create prefix = "Oil_Pipe_Burster_05"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_05"
	Create prefix = "Oil_Pipe_Burster_06"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_06"
	Create prefix = "Oil_Pipe_Burster_07"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_07"
	Create prefix = "Oil_Pipe_Burster_08"
	Wait 0.05 seconds
	Kill prefix = "Oil_Pipe_Burster_08"
ENDSCRIPT
EX_VALVE_BUSTED = 1

SCRIPT JS_HeliValve_A_00
	IF InMultiplayerGame
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_A_Valve_00
		Kill Name = TrP_Pipe_A_Valve_00
		Kill Name = Oil_Pipe_A_BreakA
		Kill Name = TRG_SFX_Pipe_A_Valve_On
		Create Name = Oil_Pipe_A_BreakB
		Create Name = TRG_SFX_Pipe_A_Valve_Off
		Shatter Name = Oil_Pipe_A_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_A_BreakC"
		Shatter prefix = "Oil_Pipe_A_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_A_BreakC"
		Obj_PlaySound GlassPane2X
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_A_Node_00
	ELSE
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_A_Valve_00
		Kill Name = TrP_Pipe_A_Valve_00
		Kill Name = Oil_Pipe_A_BreakA
		Kill Name = TRG_SFX_Pipe_A_Valve_On
		Create Name = Oil_Pipe_A_BreakB
		Create Name = TRG_SFX_Pipe_A_Valve_Off
		Shatter Name = Oil_Pipe_A_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_A_BreakC"
		Shatter prefix = "Oil_Pipe_A_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_A_BreakC"
		Obj_PlaySound GlassPane2X
		SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_A_Valve_00_Hi" visible = 1
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_A_Node_00
	ENDIF
ENDSCRIPT

SCRIPT JS_HeliValve_B_00
	IF InMultiplayerGame
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_B_Valve_00
		Kill Name = TrP_Pipe_B_Valve_00
		Kill Name = Oil_Pipe_B_BreakA
		Kill Name = TRG_SFX_Pipe_B_Valve_On
		Create Name = Oil_Pipe_B_BreakB
		Create Name = TRG_SFX_Pipe_B_Valve_Off
		Shatter Name = Oil_Pipe_B_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_B_BreakC"
		Shatter prefix = "Oil_Pipe_B_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_B_BreakC"
		Obj_PlaySound GlassPane2X
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_B_Node_00
	ELSE
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_B_Valve_00
		Kill Name = TrP_Pipe_B_Valve_00
		Kill Name = Oil_Pipe_B_BreakA
		Kill Name = TRG_SFX_Pipe_B_Valve_On
		Create Name = Oil_Pipe_B_BreakB
		Create Name = TRG_SFX_Pipe_B_Valve_Off
		Shatter Name = Oil_Pipe_B_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_B_BreakC"
		Shatter prefix = "Oil_Pipe_B_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_B_BreakC"
		Obj_PlaySound GlassPane2X
		SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_B_Valve_00_Hi" visible = 1
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_B_Node_00
	ENDIF
ENDSCRIPT

SCRIPT JS_HeliValve_C_00
	IF InMultiplayerGame
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_C_Valve_00
		Kill Name = TrP_Pipe_C_Valve_00
		Kill Name = Oil_Pipe_C_BreakA
		Kill Name = TRG_SFX_Pipe_C_Valve_On
		Create Name = Oil_Pipe_C_BreakB
		Create Name = TRG_SFX_Pipe_C_Valve_Off
		Shatter Name = Oil_Pipe_C_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_C_BreakC"
		Shatter prefix = "Oil_Pipe_C_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_C_BreakC"
		Obj_PlaySound GlassPane2X
		Wait 0.5 seconds
		Kill prefix = "TRG_Pipe_C_Node_00"
	ELSE
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_C_Valve_00
		Kill Name = TrP_Pipe_C_Valve_00
		Kill Name = Oil_Pipe_C_BreakA
		Kill Name = TRG_SFX_Pipe_C_Valve_On
		Create Name = Oil_Pipe_C_BreakB
		Create Name = TRG_SFX_Pipe_C_Valve_Off
		Shatter Name = Oil_Pipe_C_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_C_BreakC"
		Shatter prefix = "Oil_Pipe_C_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_C_BreakC"
		Obj_PlaySound GlassPane2X
		SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_C_Valve_00_Hi" visible = 1
		Wait 0.5 seconds
		Kill prefix = "TRG_Pipe_C_Node_00"
	ENDIF
ENDSCRIPT

SCRIPT JS_HeliValve_E_00
	IF InMultiplayerGame
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_E_Valve_00
		Kill Name = TrP_Pipe_E_Valve_00
		Kill Name = Oil_Pipe_E_BreakA
		Kill Name = TRG_SFX_Pipe_E_Valve_On
		Create Name = Oil_Pipe_E_BreakB
		Create Name = TRG_SFX_Pipe_E_Valve_Off
		Shatter Name = Oil_Pipe_E_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_E_BreakC"
		Shatter prefix = "Oil_Pipe_E_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_E_BreakC"
		Obj_PlaySound GlassPane2X
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_E_Node_00
	ELSE
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_E_Valve_00
		Kill Name = TrP_Pipe_E_Valve_00
		Kill Name = Oil_Pipe_E_BreakA
		Kill Name = TRG_SFX_Pipe_E_Valve_On
		Create Name = Oil_Pipe_E_BreakB
		Create Name = TRG_SFX_Pipe_E_Valve_Off
		Shatter Name = Oil_Pipe_E_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_E_BreakC"
		Shatter prefix = "Oil_Pipe_E_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_E_BreakC"
		Obj_PlaySound GlassPane2X
		SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_E_Valve_00_Hi" visible = 1
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_E_Node_00
	ENDIF
ENDSCRIPT

SCRIPT JS_HeliValve_F_00
	IF InMultiplayerGame
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_F_Valve_00
		Kill Name = TrP_Pipe_F_Valve_00
		Kill Name = Oil_Pipe_F_BreakA
		Kill Name = TRG_SFX_Pipe_F_Valve_On
		Create Name = Oil_Pipe_F_BreakB
		Create Name = TRG_SFX_Pipe_F_Valve_Off
		Shatter Name = Oil_Pipe_F_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_F_BreakC"
		Shatter prefix = "Oil_Pipe_F_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_F_BreakC"
		Obj_PlaySound GlassPane2X
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_F_Node_00
	ELSE
		SendException EX_VALVE_BUSTED Name = TRG_HeliValveCounter
		Kill Name = Oil_Pipe_F_Valve_00
		Kill Name = TrP_Pipe_F_Valve_00
		Kill Name = Oil_Pipe_F_BreakA
		Kill Name = TRG_SFX_Pipe_F_Valve_On
		Create Name = Oil_Pipe_F_BreakB
		Create Name = TRG_SFX_Pipe_F_Valve_Off
		Shatter Name = Oil_Pipe_F_Valve_00 area = 100 variance = 0.6 vel_x = 0.0 vel_y = 100 vel_z = 0.0 spread = 2
		Create prefix = "Oil_Pipe_F_BreakC"
		Shatter prefix = "Oil_Pipe_F_BreakC" area = 500 variance = 1 vel_x = 0 vel_y = 100 vel_z = 0 spread = 2
		Kill prefix = "Oil_Pipe_F_BreakC"
		Obj_PlaySound GlassPane2X
		SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Lo" visible = 0
		SetParticleSysVisibility system = "Oil_Part_Pipe_F_Valve_00_Hi" visible = 1
		Wait 0.5 seconds
		Kill Name = TRG_Pipe_F_Node_00
	ENDIF
ENDSCRIPT

SCRIPT JS_HeliValveCounter
	Obj_ClearExceptions
	Obj_SetException Ex = EX_VALVE_BUSTED Scr = JS_HeliValveCounter_Hit
ENDSCRIPT

SCRIPT JS_HeliValveCounter_Hit
	Obj_ClearExceptions
	Obj_VarInc Var = 0
	IF Obj_VarEQ Var = 0 Value = 1
		LaunchPanelMessage "1 of 5 Valves Busted"
	ENDIF
	IF Obj_VarEQ Var = 0 Value = 2
		LaunchPanelMessage "2 of 5 Valves Busted"
	ENDIF
	IF Obj_VarEQ Var = 0 Value = 3
		LaunchPanelMessage "3 of 5 Valves Busted"
	ENDIF
	IF Obj_VarEQ Var = 0 Value = 4
		LaunchPanelMessage "4 of 5 Valves Busted"
	ENDIF
	IF Obj_VarEQ Var = 0 Value = 5
		LaunchPanelMessage "5 of 5 Valves Busted"
		SpawnScript JS_Klaxon
		Wait 1 seconds
		SpawnScript JS_HeliBlaster
	ENDIF
	GoTo JS_HeliValveCounter
ENDSCRIPT

SCRIPT JS_HeliBlaster_BlastSFX
	Obj_PlaySound Oil_explode vol = 1000 dropoff = 2000 pitch = 100
ENDSCRIPT

SCRIPT JS_HeliBlaster_HeliCamSFX
	Obj_PlaySound helicopter15 vol = 1000 dropoff = 500 pitch = 120
ENDSCRIPT

SCRIPT JS_HeliBlaster
	IF InMultiplayerGame
		Create Name = Oil_L1_NE_HeliPad_Blaster
		Shatter Name = Oil_L1_NE_HeliPad_Blaster area = 10000.0 variance = 0.6 vel_x = 0.0 vel_y = -100.0 vel_z = 0.0 spread = -2.0 life = 7
		Shatter Name = Oil_L1_NE_HeliPad_Blaster area = 10000.0 variance = 0.6 vel_x = 0.0 vel_y = -300.0 vel_z = 0.0 spread = -1.5 life = 7
		Kill Name = Oil_L1_NE_HeliPad_Blaster
		Kill prefix = "Oil_L1_NE_HeliPad_Alpha"
		Kill prefix = "Oil_HJ_Stain06"
		Create prefix = "Oil_L1_NE_HeliPad_Beta"
	ELSE
		SpawnScript JS_HeliBlaster_CopterCam
		SpawnScript JS_HeliBlaster_Smoke
		Create Name = Oil_L1_NE_HeliPad_Blaster
		Shatter Name = Oil_L1_NE_HeliPad_Blaster area = 10000.0 variance = 0.6 vel_x = 0.0 vel_y = -100.0 vel_z = 0.0 spread = -2.0 life = 7
		Shatter Name = Oil_L1_NE_HeliPad_Blaster area = 10000.0 variance = 0.6 vel_x = 0.0 vel_y = -300.0 vel_z = 0.0 spread = -1.5 life = 7
		Kill Name = Oil_L1_NE_HeliPad_Blaster
		Kill prefix = "Oil_L1_NE_HeliPad_Alpha"
		Kill prefix = "Oil_HJ_Stain06"
		Create prefix = "Oil_L1_NE_HeliPad_Beta"
	ENDIF
ENDSCRIPT

SCRIPT JS_HeliBlaster_CopterCam
	IF InMultiplayerGame
	ELSE
		PauseSkaters
		Create prefix = "TRG_SFX_HeliBlaster"
		PlaySkaterCamAnim skater = 0 Name = "Cam_HeliBlaster_CopterCam"
		Wait 1 second
		SetSkaterCamAnimSkippable skater = 0 Skippable = 1
		BEGIN
			IF SkaterCamAnimFinished skater = 0 Name = "Cam_HeliBlaster_CopterCam"
				BREAK
			ENDIF
			Wait 1 gameframe
		REPEAT 840
		UnPauseSkaters
		Kill prefix = "TRG_SFX_HeliBlaster"
	ENDIF
ENDSCRIPT

SCRIPT JS_HeliBlaster_Smoke
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_00" visible = 1
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_01" visible = 1
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_02" visible = 1
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_03" visible = 1
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_04" visible = 1
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_05" visible = 1
	Wait 5 seconds
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_05" visible = 0
	Wait 4 seconds
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_04" visible = 0
	Wait 3 seconds
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_03" visible = 0
	Wait 2 seconds
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_02" visible = 0
	Wait 1 seconds
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_01" visible = 0
	Wait 0.5 seconds
	SetParticleSysVisibility system = "Oil_Part_HeliBlast_00" visible = 0
ENDSCRIPT

SCRIPT JS_DrillRoomPivot_UpDwn
	BEGIN
		Obj_RotY angle = 30 Speed = 60 flag_max_coords
		Obj_WaitRotate
		Obj_RotY angle = -60 Speed = 60 flag_max_coords
		Obj_WaitRotate
		Obj_RotY angle = 30 Speed = 60 flag_max_coords
		Obj_WaitRotate
	REPEAT
ENDSCRIPT

SCRIPT JS_DrillRoomPivot_DwnUp
	BEGIN
		Obj_RotY angle = -30 Speed = 60 flag_max_coords
		Obj_WaitRotate
		Obj_RotY angle = 60 Speed = 60 flag_max_coords
		Obj_WaitRotate
		Obj_RotY angle = -30 Speed = 60 flag_max_coords
		Obj_WaitRotate
	REPEAT
ENDSCRIPT

SCRIPT JS_DrillRoomPiston_UpDwn
	BEGIN
		Obj_MoveToRelPos (0, 0, -50) Speed = 100 ips flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, 100) Speed = 100 ips flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -50) Speed = 100 ips flag_max_coords
		Obj_WaitMove
		Wait 1 frame
	REPEAT
ENDSCRIPT

SCRIPT JS_DrillRoomPiston_DwnUp
	BEGIN
		Obj_MoveToRelPos (0, 0, 50) Speed = 100 ips flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -100) Speed = 100 ips flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, 50) Speed = 100 ips flag_max_coords
		Obj_WaitMove
		Wait 1 frame
	REPEAT
ENDSCRIPT

SCRIPT JS_Oil_Heli_MainRotor
	Obj_Rotz Speed = 3000 flag_max_coords
	Obj_PlaySound helicopter15
	BEGIN
		Obj_MoveToRelPos (0, 0, 20) Time = 2 deceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -20) Time = 2 acceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -20) Time = 2 deceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, 20) Time = 2 acceleration = 1200 flag_max_coords
		Obj_WaitMove
	REPEAT
ENDSCRIPT

SCRIPT JS_Oil_Heli_TailRotor
	Obj_Rotz Speed = 3000 flag_max_coords
	BEGIN
		Obj_MoveToRelPos (0, 0, 20) Time = 2 deceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -20) Time = 2 acceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -20) Time = 2 deceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, 20) Time = 2 acceleration = 1200 flag_max_coords
		Obj_WaitMove
	REPEAT
ENDSCRIPT

SCRIPT JS_Oil_Heli_Chassis
	BEGIN
		Obj_Rotz angle = 45 Speed = 20 flag_max_coords
		Obj_WaitRotate
		Obj_Rotz angle = -45 Speed = 20 flag_max_coords
		Obj_WaitRotate
	REPEAT
ENDSCRIPT

SCRIPT JS_Oil_Heli_Hover
	BEGIN
		Obj_MoveToRelPos (0, 0, 20) Time = 2 deceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -20) Time = 2 acceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, -20) Time = 2 deceleration = 1200 flag_max_coords
		Obj_WaitMove
		Obj_MoveToRelPos (0, 0, 20) Time = 2 acceleration = 1200 flag_max_coords
		Obj_WaitMove
	REPEAT
ENDSCRIPT

SCRIPT JS_OilG_MPipe2MPipe_W
	StartGap GapID = JS_MPipe2MPipeW flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_MPipe2MPipeE Gapscript = JS_OilS_MPipe2MPipe
ENDSCRIPT

SCRIPT JS_OilG_MPipe2MPipe_E
	StartGap GapID = JS_MPipe2MPipeE flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_MPipe2MPipeW Gapscript = JS_OilS_MPipe2MPipe
ENDSCRIPT

SCRIPT JS_OilS_MPipe2MPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_MPipe2MPipe
	Wait 1 frame
	EndGap GapID = Score_MPipe2MPipe text = "FAT PIPE HOP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_SPipe2Rail
	StartGap GapID = JS_SPipeRail flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2SPipe Gapscript = JS_OilS_SPipe2Rail
ENDSCRIPT

SCRIPT JS_OilG_Rail2SPipe
	StartGap GapID = JS_Rail2SPipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SPipeRail Gapscript = JS_OilS_SPipe2Rail
ENDSCRIPT

SCRIPT JS_OilS_SPipe2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SPipe2Rail
	Wait 1 frame
	EndGap GapID = Score_SPipe2Rail text = "RAIL 2 PIPE" score = 50
ENDSCRIPT

SCRIPT JS_OilG_SiloBaseGap_01
	StartGap GapID = JS_SiloBaseGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SiloBaseGap02 Gapscript = JS_OilS_SiloBaseGap
ENDSCRIPT

SCRIPT JS_OilG_SiloBaseGap_02
	StartGap GapID = JS_SiloBaseGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SiloBaseGap01 Gapscript = JS_OilS_SiloBaseGap
ENDSCRIPT

SCRIPT JS_OilS_SiloBaseGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloBaseGap
	Wait 1 frame
	EndGap GapID = Score_SiloBaseGap text = "SILO BASE GAP" score = 150
ENDSCRIPT

SCRIPT JS_OilG_SiloBase2Rail_01
	StartGap GapID = JS_SiloBase2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL ]
	EndGap GapID = JS_SiloBase2Rail02 Gapscript = JS_OilS_Base2Rail
ENDSCRIPT

SCRIPT JS_OilG_SiloBase2Rail_02
	StartGap GapID = JS_SiloBase2Rail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL ]
	EndGap GapID = JS_SiloBase2Rail01 Gapscript = JS_OilS_Base2Rail
ENDSCRIPT

SCRIPT JS_OilS_Base2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloBase2Rail
	Wait 1 frame
	EndGap GapID = Score_SiloBase2Rail text = "SILO BASE 2 RAIL" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Rail2SiloRailDrop_01
	StartGap GapID = JS_Rail2SiloRailDrop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Rail2SiloRailDrop02 Gapscript = JS_OilS_Rail2SiloRailDrop
ENDSCRIPT

SCRIPT JS_OilG_Rail2SiloRailDrop_02
	StartGap GapID = JS_Rail2SiloRailDrop02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Rail2SiloRailDrop01 Gapscript = JS_OilS_Rail2SiloRailDrop
ENDSCRIPT

SCRIPT JS_OilS_Rail2SiloRailDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Rail2SiloRailDrop
	Wait 1 frame
	EndGap GapID = Score_Rail2SiloRailDrop text = "RAIL 2 SILO RAIL DROP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2Rail_01
	StartGap GapID = JS_SiloRail2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SiloRail2Rail02 Gapscript = JS_OilS_SiloRail2Rail
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2Rail_02
	StartGap GapID = JS_SiloRail2Rail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SiloRail2Rail01 Gapscript = JS_OilS_SiloRail2Rail
ENDSCRIPT

SCRIPT JS_OilS_SiloRail2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloRail2Rail
	Wait 1 frame
	EndGap GapID = Score_SiloRail2Rail text = "SILO RAIL 2 RAIL" score = 50
ENDSCRIPT

SCRIPT JS_OilG_RampRail2SiloRail_01
	StartGap GapID = JS_RampRail2SiloRail flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_RampRail2SiloRail_02
	EndGap GapID = JS_RampRail2SiloRail Gapscript = JS_OilS_RampRail2SiloRail
ENDSCRIPT

SCRIPT JS_OilS_RampRail2SiloRail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RampRail2SiloRail
	Wait 1 frame
	EndGap GapID = Score_RampRail2SiloRail text = "RAMP RAIL 2 SILO RAIL" score = 1000
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2RailDrop_01
	StartGap GapID = JS_SiloRail2RailDrop flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2RailDrop_02
	EndGap GapID = JS_SiloRail2RailDrop Gapscript = JS_OilS_SiloRail2RailDrop
ENDSCRIPT

SCRIPT JS_OilS_SiloRail2RailDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloRail2RailDrop
	Wait 1 frame
	EndGap GapID = Score_SiloRail2RailDrop text = "SILO RAIL 2 RAIL DROP" score = 1000
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2PipeLowDrop_01
	StartGap GapID = JS_SiloRail2PipeLowDrop flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2PipeLowDrop_02
	EndGap GapID = JS_SiloRail2PipeLowDrop Gapscript = JS_OilS_SiloRail2PipeLowDrop
ENDSCRIPT

SCRIPT JS_OilS_SiloRail2PipeLowDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloRail2PipeLowDrop
	Wait 1 frame
	EndGap GapID = Score_SiloRail2PipeLowDrop text = "SILO RAIL 2 PIPE LOW DROP" score = 1000
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2PipeHighDrop_01
	StartGap GapID = JS_SiloRail2PipeHighDrop flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_SiloRail2PipeHighDrop_02
	EndGap GapID = JS_SiloRail2PipeHighDrop Gapscript = JS_OilS_SiloRail2PipeHighDrop
ENDSCRIPT

SCRIPT JS_OilS_SiloRail2PipeHighDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloRail2PipeHighDrop
	Wait 1 frame
	EndGap GapID = Score_SiloRail2PipeHighDrop text = "SILO RAIL 2 PIPE HIGH DROP" score = 1500
ENDSCRIPT

SCRIPT JS_OilG_SiloRailHop_01
	StartGap GapID = JS_SiloRailHop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SiloRailHop02 Gapscript = JS_OilS_SiloRailHop
ENDSCRIPT

SCRIPT JS_OilG_SiloRailHop_02
	StartGap GapID = JS_SiloRailHop02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SiloRailHop01 Gapscript = JS_OilS_SiloRailHop
ENDSCRIPT

SCRIPT JS_OilS_SiloRailHop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloRailHop
	Wait 1 frame
	EndGap GapID = Score_SiloRailHop text = "SILO RAIL HOP" score = 1000
ENDSCRIPT

SCRIPT JS_OilG_SiloRailCRailLinkup_01
	StartGap GapID = JS_SiloRailCRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL ]
	EndGap GapID = JS_SiloRailCRailLinkup02 Gapscript = JS_OilS_SiloRailCRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_SiloRailCRailLinkup_02
	StartGap GapID = JS_SiloRailCRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL ]
	EndGap GapID = JS_SiloRailCRailLinkup01 Gapscript = JS_OilS_SiloRailCRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_SiloRailCRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SiloRailCRailLinkup
	Wait 1 frame
	EndGap GapID = Score_SiloRailCRailLinkup text = "SILO RAIL C-RAIL LINKUP" score = 500
ENDSCRIPT

SCRIPT JS_OilG_RailCRailLinkup_01
	StartGap GapID = JS_RailCRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_RailCRailLinkup02 Gapscript = JS_OilS_RailCRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_RailCRailLinkup_02
	StartGap GapID = JS_RailCRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_RailCRailLinkup01 Gapscript = JS_OilS_RailCRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_RailCRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailCRailLinkup
	Wait 1 frame
	EndGap GapID = Score_RailCRailLinkup text = "RAIL C-RAIL LINKUP" score = 1000
ENDSCRIPT

SCRIPT JS_OilG_CRailPipeDrop_01
	StartGap GapID = JS_CRailPipeDrop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_CRailPipeDrop_02
	EndGap GapID = JS_CRailPipeDrop01 Gapscript = JS_OilS_CRailPipeDrop
ENDSCRIPT

SCRIPT JS_OilS_CRailPipeDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_CRailPipeDrop
	Wait 1 frame
	EndGap GapID = Score_CRailPipeDrop text = "C-RAIL 2 PIPE DROP" score = 750
ENDSCRIPT

SCRIPT JS_OilG_RampRail2Pipe_01
	StartGap GapID = JS_RampRail2Pipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_RampRail2Pipe_02
	EndGap GapID = JS_RampRail2Pipe Gapscript = JS_OilS_Rail2Pipe
ENDSCRIPT

SCRIPT JS_OilS_RampRail2Pipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RampRail2Pipe
	Wait 1 frame
	EndGap GapID = Score_RampRail2Pipe text = "RAMP RAIL 2 PIPE" score = 100
ENDSCRIPT

SCRIPT JS_OilG_SWPipe2Rail
	StartGap GapID = JS_SWPipe2Rail flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2SWPipe Gapscript = JS_OilS_NESWPipe2Rail
ENDSCRIPT

SCRIPT JS_OilG_Rail2SWPipe
	StartGap GapID = JS_Rail2SWPipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SWPipe2Rail Gapscript = JS_OilS_NESWPipe2Rail
ENDSCRIPT

SCRIPT JS_OilG_NEPipe2Rail
	StartGap GapID = JS_NEPipe2Rail flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2NEPipe Gapscript = JS_OilS_NESWPipe2Rail
ENDSCRIPT

SCRIPT JS_OilG_Rail2NEPipe
	StartGap GapID = JS_Rail2NEPipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_NEPipe2Rail Gapscript = JS_OilS_NESWPipe2Rail
ENDSCRIPT

SCRIPT JS_OilS_NESWPipe2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_NESWPipe2Rail
	Wait 1 frame
	EndGap GapID = Score_NESWPipe2Rail text = "UPPER PIPE RAIL LINKUP" score = 50
ENDSCRIPT

SCRIPT JS_OilG_SPipe2RailDrop_01
	StartGap GapID = JS_SPipe2RailDrop flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_SPipe2RailDrop_02
	EndGap GapID = JS_SPipe2RailDrop Gapscript = JS_OilG_SPipe2RailDrop
ENDSCRIPT

SCRIPT JS_OilG_SPipe2RailDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SPipe2RailDrop
	Wait 1 frame
	EndGap GapID = Score_SPipe2RailDrop text = "UPPER PIPE 2 RAIL DROP" score = 300
ENDSCRIPT

SCRIPT JS_OilG_LoPipe2Rail
	StartGap GapID = JS_LoPipe2Rail flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoRail2Pipe Gapscript = JS_OilS_LoPipe2Rail
ENDSCRIPT

SCRIPT JS_OilG_LoRail2Pipe
	StartGap GapID = JS_LoRail2Pipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoPipe2Rail Gapscript = JS_OilS_LoPipe2Rail
ENDSCRIPT

SCRIPT JS_OilS_LoPipe2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_LoPipe2Rail
	Wait 1 frame
	EndGap GapID = Score_LoPipe2Rail text = "LOWER PIPE RAIL LINKUP" score = 50
ENDSCRIPT

SCRIPT JS_OilG_LoPipe2LoPipe_01
	StartGap GapID = JS_LoPipe2LoPipe01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoPipe2LoPipe02 Gapscript = JS_OilS_LoPipe2LoPipe
ENDSCRIPT

SCRIPT JS_OilG_LoPipe2LoPipe_02
	StartGap GapID = JS_LoPipe2LoPipe02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoPipe2LoPipe01 Gapscript = JS_OilS_LoPipe2LoPipe
ENDSCRIPT

SCRIPT JS_OilS_LoPipe2LoPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_LoPipe2LoPipe
	Wait 1 frame
	EndGap GapID = Score_LoPipe2LoPipe text = "LOWER PIPE 2 PIPE" score = 100
ENDSCRIPT

SCRIPT JS_OilG_LoPipe2PipeXOver_01
	StartGap GapID = JS_LoPipe2PipeXOver01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoPipe2PipeXOver02 Gapscript = JS_OilS_LoPipe2PipeXOver
ENDSCRIPT

SCRIPT JS_OilG_LoPipe2PipeXOver_02
	StartGap GapID = JS_LoPipe2PipeXOver02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoPipe2PipeXOver01 Gapscript = JS_OilS_LoPipe2PipeXOver
ENDSCRIPT

SCRIPT JS_OilG_LoPipe2PipeXOver_03
	StartGap GapID = JS_LoPipe2PipeXOver03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoPipe2PipeXOver04 Gapscript = JS_OilS_LoPipe2PipeXOver
ENDSCRIPT

SCRIPT JS_OilG_LoPipe2PipeXOver_04
	StartGap GapID = JS_LoPipe2PipeXOver04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LoPipe2PipeXOver03 Gapscript = JS_OilS_LoPipe2PipeXOver
ENDSCRIPT

SCRIPT JS_OilS_LoPipe2PipeXOver
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_LoPipe2PipeXOver
	Wait 1 frame
	EndGap GapID = Score_LoPipe2PipeXOver text = "LOWER PIPE 2 PIPE CROSS OVER" score = 200
ENDSCRIPT

SCRIPT JS_OilG_ShortPipeRailLinkup_01
	StartGap GapID = JS_ShortPipeRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_ShortPipeRailLinkup02 Gapscript = JS_OilS_ShortPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_ShortPipeRailLinkup_02
	StartGap GapID = JS_ShortPipeRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_ShortPipeRailLinkup01 Gapscript = JS_OilS_ShortPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_ShortPipeRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_ShortPipeRailLinkup
	Wait 1 frame
	EndGap GapID = Score_ShortPipeRailLinkup text = "SHORT PIPE RAIL LINKUP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_ShortPipe2ShortPipe_01
	StartGap GapID = JS_ShortPipe2ShortPipe01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_ShortPipe2ShortPipe02 Gapscript = JS_OilS_ShortPipe2ShortPipe
ENDSCRIPT

SCRIPT JS_OilG_ShortPipe2ShortPipe_02
	StartGap GapID = JS_ShortPipe2ShortPipe02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_ShortPipe2ShortPipe01 Gapscript = JS_OilS_ShortPipe2ShortPipe
ENDSCRIPT

SCRIPT JS_OilS_ShortPipe2ShortPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_ShortPipe2ShortPipe
	Wait 1 frame
	EndGap GapID = Score_ShortPipe2ShortPipe text = "SHORT PIPE 2 SHORT PIPE" score = 50
ENDSCRIPT

SCRIPT JS_OilG_ShortPipeBowlLinkup_01
	StartGap GapID = JS_ShortPipeBowlLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_ShortPipeBowlLinkup02 Gapscript = JS_OilS_ShortPipeBowlLinkup
ENDSCRIPT

SCRIPT JS_OilG_ShortPipeBowlLinkup_02
	StartGap GapID = JS_ShortPipeBowlLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_ShortPipeBowlLinkup01 Gapscript = JS_OilS_ShortPipeBowlLinkup
ENDSCRIPT

SCRIPT JS_OilS_ShortPipeBowlLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_ShortPipeBowlLinkup
	Wait 1 frame
	EndGap GapID = Score_ShortPipeBowlLinkup text = "SHORT PIPE BOWL LINKUP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_CrankedPipeRailLinkup_01
	StartGap GapID = JS_CrankedPipeRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_CrankedPipeRailLinkup02 Gapscript = JS_OilS_CrankedPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_CrankedPipeRailLinkup_02
	StartGap GapID = JS_CrankedPipeRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_CrankedPipeRailLinkup01 Gapscript = JS_OilS_CrankedPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_CrankedPipeRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_CrankedPipeRailLinkup
	Wait 1 frame
	EndGap GapID = Score_CrankedPipeRailLinkup text = "CRANKED PIPE RAIL LINKUP" score = 50
ENDSCRIPT

SCRIPT JS_OilG_CrankedPipeRail2SiloBase_01
	StartGap GapID = JS_CrankedPipeRail2SiloBase01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_CrankedPipeRail2SiloBase_02
	EndGap GapID = JS_CrankedPipeRail2SiloBase01 Gapscript = JS_OilS_CrankedPipeRail2SiloBase
ENDSCRIPT

SCRIPT JS_OilS_CrankedPipeRail2SiloBase
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_CrankedPipeRail2SiloBase
	Wait 1 frame
	EndGap GapID = Score_CrankedPipeRail2SiloBase text = "CRANKED PIPE 2 SILO BASE" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Beam2Beam_01
	StartGap GapID = JS_Beam2Beam01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Beam2Beam02 Gapscript = JS_OilS_Beam2Beam
ENDSCRIPT

SCRIPT JS_OilG_Beam2Beam_02
	StartGap GapID = JS_Beam2Beam02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Beam2Beam01 Gapscript = JS_OilS_Beam2Beam
ENDSCRIPT

SCRIPT JS_OilS_Beam2Beam
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Beam2Beam
	Wait 1 frame
	EndGap GapID = Score_Beam2Beam text = "BEAM 2 BEAM" score = 700
ENDSCRIPT

SCRIPT JS_OilG_LongBeam2LongBeam_01
	StartGap GapID = JS_LongBeam2LongBeam01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LongBeam2LongBeam02 Gapscript = JS_OilS_LongBeam2LongBeam
ENDSCRIPT

SCRIPT JS_OilG_LongBeam2LongBeam_02
	StartGap GapID = JS_LongBeam2LongBeam02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LongBeam2LongBeam01 Gapscript = JS_OilS_LongBeam2LongBeam
ENDSCRIPT

SCRIPT JS_OilS_LongBeam2LongBeam
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_LongBeam2LongBeam
	Wait 1 frame
	EndGap GapID = Score_LongBeam2LongBeam text = "LONG BEAM 2 BEAM" score = 100
ENDSCRIPT

SCRIPT JS_OilG_LongBeam_01
	StartGap GapID = JS_LongBeam01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_LongBeam_02
	EndGap GapID = JS_LongBeam01 Gapscript = JS_OilS_LongBeam
ENDSCRIPT

SCRIPT JS_OilG_LongBeam_03
	StartGap GapID = JS_LongBeam02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_LongBeam_04
	EndGap GapID = JS_LongBeam02 Gapscript = JS_OilS_LongBeam
ENDSCRIPT

SCRIPT JS_OilS_LongBeam
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_LongBeam
	Wait 1 frame
	EndGap GapID = Score_LongBeam text = "LONG BEAM" score = 800
ENDSCRIPT

SCRIPT JS_OilG_RigLongBeamLinkup_01
	StartGap GapID = JS_RigLongBeamLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RigLongBeamLinkup02 Gapscript = JS_OilS_RigLongBeamLinkup
ENDSCRIPT

SCRIPT JS_OilG_RigLongBeamLinkup_02
	StartGap GapID = JS_RigLongBeamLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RigLongBeamLinkup01 Gapscript = JS_OilS_RigLongBeamLinkup
ENDSCRIPT

SCRIPT JS_OilS_RigLongBeamLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RigLongBeamLinkup
	Wait 1 frame
	EndGap GapID = Score_RigLongBeamLinkup text = "RIG LONG BEAM LINKUP" score = 300
ENDSCRIPT

SCRIPT JS_OilG_CrookedPipeRailLinkup_01
	StartGap GapID = JS_CrookedPipeRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedPipeRailLinkup02 Gapscript = JS_OilS_CrookedPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_CrookedPipeRailLinkup_02
	StartGap GapID = JS_CrookedPipeRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedPipeRailLinkup01 Gapscript = JS_OilS_CrookedPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_CrookedPipeRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_CrookedPipeRailLinkup
	Wait 1 frame
	EndGap GapID = Score_CrookedPipeRailLinkup text = "CROOKED PIPE RAIL LINKUP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_CrookedPipe2Pipe_01
	StartGap GapID = JS_CrookedPipe2Pipe01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedPipe2Pipe02 Gapscript = JS_OilS_CrookedPipe2Pipe
ENDSCRIPT

SCRIPT JS_OilG_CrookedPipe2Pipe_02
	StartGap GapID = JS_CrookedPipe2Pipe02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedPipe2Pipe01 Gapscript = JS_OilS_CrookedPipe2Pipe
ENDSCRIPT

SCRIPT JS_OilG_CrookedPipe2Pipe_03
	StartGap GapID = JS_CrookedPipe2Pipe03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedPipe2Pipe04 Gapscript = JS_OilS_CrookedPipe2Pipe
ENDSCRIPT

SCRIPT JS_OilG_CrookedPipe2Pipe_04
	StartGap GapID = JS_CrookedPipe2Pipe04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedPipe2Pipe03 Gapscript = JS_OilS_CrookedPipe2Pipe
ENDSCRIPT

SCRIPT JS_OilS_CrookedPipe2Pipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_CrookedPipe2Pipe
	Wait 1 frame
	EndGap GapID = Score_CrookedPipe2Pipe text = "CROOKED PIPE 2 PIPE" score = 100
ENDSCRIPT

SCRIPT JS_OilG_CrookedLedgeRailLinkup_01
	StartGap GapID = JS_CrookedLedgeRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedLedgeRailLinkup02 Gapscript = JS_OilS_CrookedLedgeRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_CrookedLedgeRailLinkup_02
	StartGap GapID = JS_CrookedLedgeRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_CrookedLedgeRailLinkup01 Gapscript = JS_OilS_CrookedLedgeRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_CrookedLedgeRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_CrookedLedgeRailLinkup
	Wait 1 frame
	EndGap GapID = Score_CrookedLedgeRailLinkup text = "CROOKED LEDGE RAIL LINKUP" score = 250
ENDSCRIPT

SCRIPT JS_OilG_Rail2Rail_01
	StartGap GapID = JS_Rail2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Rail2Rail02 Gapscript = JS_OilS_Rail2Rail
ENDSCRIPT

SCRIPT JS_OilG_Rail2Rail_02
	StartGap GapID = JS_Rail2Rail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Rail2Rail01 Gapscript = JS_OilS_Rail2Rail
ENDSCRIPT

SCRIPT JS_OilG_Rail2Rail_03
	StartGap GapID = JS_Rail2Rail03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Rail2Rail04 Gapscript = JS_OilS_Rail2Rail
ENDSCRIPT

SCRIPT JS_OilG_Rail2Rail_04
	StartGap GapID = JS_Rail2Rail04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Rail2Rail03 Gapscript = JS_OilS_Rail2Rail
ENDSCRIPT

SCRIPT JS_OilS_Rail2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Rail2Rail
	Wait 1 frame
	EndGap GapID = Score_Rail2Rail text = "RAIL 2 RAIL" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailXOver_01
	StartGap GapID = JS_Rail2RailXOver01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2RailXOver02 Gapscript = JS_OilS_Rail2RailXOver
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailXOver_02
	StartGap GapID = JS_Rail2RailXOver02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2RailXOver01 Gapscript = JS_OilS_Rail2RailXOver
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailXOver_03
	StartGap GapID = JS_Rail2RailXOver03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2RailXOver04 Gapscript = JS_OilS_Rail2RailXOver
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailXOver_04
	StartGap GapID = JS_Rail2RailXOver04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2RailXOver03 Gapscript = JS_OilS_Rail2RailXOver
ENDSCRIPT

SCRIPT JS_OilS_Rail2RailXOver
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Rail2RailXOver
	Wait 1 frame
	EndGap GapID = Score_Rail2RailXOver text = "RAIL 2 RAIL CROSS OVER" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailDrop_01
	StartGap GapID = JS_Rail2RailDrop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailDrop_02
	EndGap GapID = JS_Rail2RailDrop01 Gapscript = JS_OilS_Rail2RailDrop
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailDrop_03
	StartGap GapID = JS_Rail2RailDrop03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailDrop_04
	EndGap GapID = JS_Rail2RailDrop03 Gapscript = JS_OilS_Rail2RailDrop
ENDSCRIPT

SCRIPT JS_OilS_Rail2RailDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Rail2RailDrop
	Wait 1 frame
	EndGap GapID = Score_Rail2RailDrop text = "RAIL 2 RAIL DROP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailCrossDrop_01
	StartGap GapID = JS_Rail2RailCrossDrop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Rail2RailCrossDrop_02
	EndGap GapID = JS_Rail2RailCrossDrop01 Gapscript = JS_OilS_Rail2RailCrossDrop
ENDSCRIPT

SCRIPT JS_OilS_Rail2RailCrossDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Rail2RailCrossDrop
	Wait 1 frame
	EndGap GapID = Score_Rail2RailCrossDrop text = "RAIL 2 RAIL CROSS DROP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_RailOverRail_01
	StartGap GapID = JS_RailOverRail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailOverRail02 Gapscript = JS_OilS_RailOverRail
ENDSCRIPT

SCRIPT JS_OilG_RailOverRail_02
	StartGap GapID = JS_RailOverRail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailOverRail01 Gapscript = JS_OilS_RailOverRail
ENDSCRIPT

SCRIPT JS_OilS_RailOverRail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailOverRail
	Wait 1 frame
	EndGap GapID = Score_RailOverRail text = "RAIL OVER RAIL" score = 100
ENDSCRIPT

SCRIPT JS_OilG_RailLedgeHop_01
	StartGap GapID = JS_RailLedgeHop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailLedgeHop02 Gapscript = JS_OilS_RailLedgeHop
ENDSCRIPT

SCRIPT JS_OilG_RailLedgeHop_02
	StartGap GapID = JS_RailLedgeHop02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailLedgeHop01 Gapscript = JS_OilS_RailLedgeHop
ENDSCRIPT

SCRIPT JS_OilS_RailLedgeHop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailLedgeHop
	Wait 1 frame
	EndGap GapID = Score_RailLedgeHop text = "RAIL LEDGE HOP" score = 500
ENDSCRIPT

SCRIPT JS_OilG_RailLedgeXover_01
	StartGap GapID = JS_RailLedgeXover01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailLedgeXover02 Gapscript = JS_OilS_RailLedgeXover
ENDSCRIPT

SCRIPT JS_OilG_RailLedgeXover_02
	StartGap GapID = JS_RailLedgeXover02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailLedgeXover01 Gapscript = JS_OilS_RailLedgeXover
ENDSCRIPT

SCRIPT JS_OilS_RailLedgeXover
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailLedgeXover
	Wait 1 frame
	EndGap GapID = Score_RailLedgeXover text = "RAIL LEDGE CROSS OVER" score = 100
ENDSCRIPT

SCRIPT JS_OilG_RailLedge2Pipe_01
	StartGap GapID = JS_RailLedge2Pipe01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_RailLedge2Pipe_02
	EndGap GapID = JS_RailLedge2Pipe01 Gapscript = JS_OilS_RailLedge2Pipe
ENDSCRIPT

SCRIPT JS_OilS_RailLedge2Pipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailLedge2Pipe
	Wait 1 frame
	EndGap GapID = Score_RailLedge2Pipe text = "RAIL LEDGE 2 PIPE" score = 250
ENDSCRIPT

SCRIPT JS_OilG_RailLaunch2Rail_01
	StartGap GapID = JS_RailLaunch2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_RailLaunch2Rail_02
	EndGap GapID = JS_RailLaunch2Rail01 Gapscript = JS_OilS_RailLaunch2Rail
ENDSCRIPT

SCRIPT JS_OilS_RailLaunch2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailLaunch2Rail
	Wait 1 frame
	EndGap GapID = Score_RailLaunch2Rail text = "RAIL LAUNCH 2 RAIL" score = 350
ENDSCRIPT

SCRIPT JS_OilG_RailLaunch2Pipe_01
	StartGap GapID = JS_RailLaunch2Pipe01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_RailLaunch2Pipe_02
	EndGap GapID = JS_RailLaunch2Pipe01 Gapscript = JS_OilS_RailLaunch2Pipe
ENDSCRIPT

SCRIPT JS_OilS_RailLaunch2Pipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailLaunch2Pipe
	Wait 1 frame
	EndGap GapID = Score_RailLaunch2Pipe text = "RAIL LAUNCH 2 PIPE" score = 350
ENDSCRIPT

SCRIPT JS_OilG_BowlRailLedgeHop_01
	StartGap GapID = JS_BowlRailLedgeHop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlRailLedgeHop02 Gapscript = JS_OilS_BowlRailLedgeHop
ENDSCRIPT

SCRIPT JS_OilG_BowlRailLedgeHop_02
	StartGap GapID = JS_BowlRailLedgeHop02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlRailLedgeHop01 Gapscript = JS_OilS_BowlRailLedgeHop
ENDSCRIPT

SCRIPT JS_OilS_BowlRailLedgeHop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BowlRailLedgeHop
	Wait 1 frame
	EndGap GapID = Score_BowlRailLedgeHop text = "BOWL RAIL LEDGE HOP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Rail2BowlRailDrop_01
	StartGap GapID = JS_Rail2BowlRailDrop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Rail2BowlRailDrop_02
	EndGap GapID = JS_Rail2BowlRailDrop01 Gapscript = JS_OilS_Rail2BowlRailDrop
ENDSCRIPT

SCRIPT JS_OilS_Rail2BowlRailDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Rail2BowlRailDrop
	Wait 1 frame
	EndGap GapID = Score_Rail2BowlRailDrop text = "RAIL 2 BOWL RAIL DROP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_BowlRailLinkup_01
	StartGap GapID = JS_BowlRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlRailLinkup02 Gapscript = JS_OilS_BowlRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_BowlRailLinkup_02
	StartGap GapID = JS_BowlRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlRailLinkup01 Gapscript = JS_OilS_BowlRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_BowlRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BowlRailLinkup
	Wait 1 frame
	EndGap GapID = Score_BowlRailLinkup text = "BOWL RAIL LINKUP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Bowl2Bowl_01
	StartGap GapID = JS_Bowl2Bowl01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Bowl2Bowl02 Gapscript = JS_OilS_Bowl2Bowl
ENDSCRIPT

SCRIPT JS_OilG_Bowl2Bowl_02
	StartGap GapID = JS_Bowl2Bowl02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Bowl2Bowl01 Gapscript = JS_OilS_Bowl2Bowl
ENDSCRIPT

SCRIPT JS_OilS_Bowl2Bowl
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Bowl2Bowl
	Wait 1 frame
	EndGap GapID = Score_Bowl2Bowl text = "BOWL 2 BOWL" score = 100
ENDSCRIPT

SCRIPT JS_OilG_BowlRailXover_01
	StartGap GapID = JS_BowlRailXover01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlRailXover02 Gapscript = JS_OilS_BowlRailXover
ENDSCRIPT

SCRIPT JS_OilG_BowlRailXover_02
	StartGap GapID = JS_BowlRailXover02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlRailXover01 Gapscript = JS_OilS_BowlRailXover
ENDSCRIPT

SCRIPT JS_OilS_BowlRailXover
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BowlRailXover
	Wait 1 frame
	EndGap GapID = Score_BowlRailXover text = "BOWL RAIL CROSS OVER" score = 100
ENDSCRIPT

SCRIPT JS_OilG_BowlOverRailXover_01
	StartGap GapID = JS_BowlOverRailXover01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlOverRailXover02 Gapscript = JS_OilS_BowlOverRailXover
ENDSCRIPT

SCRIPT JS_OilG_BowlOverRailXover_02
	StartGap GapID = JS_BowlOverRailXover02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlOverRailXover01 Gapscript = JS_OilS_BowlOverRailXover
ENDSCRIPT

SCRIPT JS_OilS_BowlOverRailXover
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BowlOverRailXover
	Wait 1 frame
	EndGap GapID = Score_BowlOverRailXover text = "BOWL OVER RAIL CROSS OVER" score = 200
ENDSCRIPT

SCRIPT JS_OilG_RailTankRailLinkup_01
	StartGap GapID = JS_RailTankRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_RailTankRailLinkup02 Gapscript = JS_OilS_RailTankRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_RailTankRailLinkup_02
	StartGap GapID = JS_RailTankRailLinkup02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_RailTankRailLinkup01 Gapscript = JS_OilS_RailTankRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_RailTankRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailTankRailLinkup
	Wait 1 frame
	EndGap GapID = Score_RailTankRailLinkup text = "RAIL TANK RAIL LINKUP" score = 250
ENDSCRIPT

SCRIPT JS_OilG_TankRail2Rail_01
	StartGap GapID = JS_TankRail2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRail2Rail02 Gapscript = JS_OilS_TankRail2Rail
ENDSCRIPT

SCRIPT JS_OilG_TankRail2Rail_02
	StartGap GapID = JS_TankRail2Rail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRail2Rail01 Gapscript = JS_OilS_TankRail2Rail
ENDSCRIPT

SCRIPT JS_OilG_TankRail2Rail_03
	StartGap GapID = JS_TankRail2Rail03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRail2Rail04 Gapscript = JS_OilS_TankRail2Rail
ENDSCRIPT

SCRIPT JS_OilG_TankRail2Rail_04
	StartGap GapID = JS_TankRail2Rail04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRail2Rail03 Gapscript = JS_OilS_TankRail2Rail
ENDSCRIPT

SCRIPT JS_OilS_TankRail2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_TankRail2Rail
	Wait 1 frame
	EndGap GapID = Score_TankRail2Rail text = "TANK RAIL 2 RAIL" score = 150
ENDSCRIPT

SCRIPT JS_OilG_TankRailXOver_01
	StartGap GapID = JS_TankRailXOver01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRailXOver02 Gapscript = JS_OilS_TankRailXOver
ENDSCRIPT

SCRIPT JS_OilG_TankRailXOver_02
	StartGap GapID = JS_TankRailXOver02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRailXOver01 Gapscript = JS_OilS_TankRailXOver
ENDSCRIPT

SCRIPT JS_OilG_TankRailXOver_03
	StartGap GapID = JS_TankRailXOver03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRailXOver04 Gapscript = JS_OilS_TankRailXOver
ENDSCRIPT

SCRIPT JS_OilG_TankRailXOver_04
	StartGap GapID = JS_TankRailXOver04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TankRailXOver03 Gapscript = JS_OilS_TankRailXOver
ENDSCRIPT

SCRIPT JS_OilS_TankRailXOver
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_TankRailXOver
	Wait 1 frame
	EndGap GapID = Score_TankRailXOver text = "TANK RAIL CROSSOVER" score = 100
ENDSCRIPT

SCRIPT JS_OilG_LedgeHop_01
	StartGap GapID = JS_LedgeHop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LedgeHop02 Gapscript = JS_OilS_LedgeHop
ENDSCRIPT

SCRIPT JS_OilG_LedgeHop_02
	StartGap GapID = JS_LedgeHop02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_LedgeHop01 Gapscript = JS_OilS_LedgeHop
ENDSCRIPT

SCRIPT JS_OilS_LedgeHop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_LedgeHop
	Wait 1 frame
	EndGap GapID = Score_LedgeHop text = "LEDGE HOP" score = 50
ENDSCRIPT

SCRIPT JS_OilG_SlantPipeRailLinkup_01
	StartGap GapID = JS_SlantPipeRailLinkup01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipeRailLinkup2 Gapscript = JS_OilS_SlantPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilG_SlantPipeRailLinkup_02
	StartGap GapID = JS_SlantPipeRailLinkup2 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipeRailLinkup01 Gapscript = JS_OilS_SlantPipeRailLinkup
ENDSCRIPT

SCRIPT JS_OilS_SlantPipeRailLinkup
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SlantPipeRailLinkup
	Wait 1 frame
	EndGap GapID = Score_SlantPipeRailLinkup text = "SLANT PIPE RAIL LINKUP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_SlantPipe2SlantPipe_01
	StartGap GapID = JS_SlantPipe2SlantPipe01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipe2SlantPipe02 Gapscript = JS_OilS_SlantPipe2SlantPipe
ENDSCRIPT

SCRIPT JS_OilG_SlantPipe2SlantPipe_02
	StartGap GapID = JS_SlantPipe2SlantPipe02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipe2SlantPipe01 Gapscript = JS_OilS_SlantPipe2SlantPipe
ENDSCRIPT

SCRIPT JS_OilG_SlantPipe2SlantPipe_03
	StartGap GapID = JS_SlantPipe2SlantPipe03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipe2SlantPipe04 Gapscript = JS_OilS_SlantPipe2SlantPipe
ENDSCRIPT

SCRIPT JS_OilG_SlantPipe2SlantPipe_04
	StartGap GapID = JS_SlantPipe2SlantPipe04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipe2SlantPipe03 Gapscript = JS_OilS_SlantPipe2SlantPipe
ENDSCRIPT

SCRIPT JS_OilS_SlantPipe2SlantPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SlantPipe2SlantPipe
	Wait 1 frame
	EndGap GapID = Score_SlantPipe2SlantPipe text = "SLANT PIPE 2 SLANT PIPE" score = 100
ENDSCRIPT

SCRIPT JS_OilG_SlantPipeOverSlant_01
	StartGap GapID = JS_SlantPipeOverSlant01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipeOverSlant2 Gapscript = JS_OilS_SlantPipeOverSlant
ENDSCRIPT

SCRIPT JS_OilG_SlantPipeOverSlant_02
	StartGap GapID = JS_SlantPipeOverSlant2 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SlantPipeOverSlant01 Gapscript = JS_OilS_SlantPipeOverSlant
ENDSCRIPT

SCRIPT JS_OilS_SlantPipeOverSlant
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SlantPipeOverSlant
	Wait 1 frame
	EndGap GapID = Score_SlantPipeOverSlant text = "SLANT PIPE OVER SLANT" score = 250
ENDSCRIPT

SCRIPT JS_OilG_SlantPipe2OverPipe_01
	StartGap GapID = JS_SlantPipe2OverPipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_SlantPipe2OverPipe_02
	EndGap GapID = JS_SlantPipe2OverPipe Gapscript = JS_OilS_SlantPipe2OverPipe
ENDSCRIPT

SCRIPT JS_OilS_SlantPipe2OverPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SlantPipe2OverPipe
	Wait 1 frame
	EndGap GapID = Score_SlantPipe2OverPipe text = "SLANT PIPE 2 OVER PIPE" score = 250
ENDSCRIPT

SCRIPT JS_OilG_OverPipe2SlantPipe_01
	StartGap GapID = JS_OverPipe2SlantPipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_OverPipe2SlantPipe_02
	EndGap GapID = JS_OverPipe2SlantPipe Gapscript = JS_OilS_OverPipe2SlantPipe
ENDSCRIPT

SCRIPT JS_OilS_OverPipe2SlantPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_OverPipe2SlantPipe
	Wait 1 frame
	EndGap GapID = Score_OverPipe2SlantPipe text = "OVER PIPE 2 SLANT PIPE" score = 100
ENDSCRIPT

SCRIPT JS_OilG_FatPipe2Rail
	StartGap GapID = JS_FatPipe2Rail flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_Rail2FatPipe Gapscript = JS_OilS_FatPipe2Rail
ENDSCRIPT

SCRIPT JS_OilG_Rail2FatPipe
	StartGap GapID = JS_Rail2FatPipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_FatPipe2Rail Gapscript = JS_OilS_FatPipe2Rail
ENDSCRIPT

SCRIPT JS_OilS_FatPipe2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_FatPipe2Rail
	Wait 1 frame
	EndGap GapID = Score_FatPipe2Rail text = "FAT PIPE RAIL LINKUP" score = 350
ENDSCRIPT

SCRIPT JS_OilG_RailOverFatPipe_01
	StartGap GapID = JS_RailOverFatPipe01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailOverFatPipe02 Gapscript = JS_OilS_RailOverFatPipe
ENDSCRIPT

SCRIPT JS_OilG_RailOverFatPipe_02
	StartGap GapID = JS_RailOverFatPipe02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_RailOverFatPipe01 Gapscript = JS_OilS_RailOverFatPipe
ENDSCRIPT

SCRIPT JS_OilS_RailOverFatPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_RailOverFatPipe
	Wait 1 frame
	EndGap GapID = Score_RailOverFatPipe text = "RAIL OVER FAT PIPE" score = 100
ENDSCRIPT

SCRIPT JS_OilG_FatPipe2RailDrop_01
	StartGap GapID = JS_FatPipe2RailDrop flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_FatPipe2RailDrop_02
	EndGap GapID = JS_FatPipe2RailDrop Gapscript = JS_OilG_FatPipe2RailDrop
ENDSCRIPT

SCRIPT JS_OilG_FatPipe2RailDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_FatPipe2RailDrop
	Wait 1 frame
	EndGap GapID = Score_FatPipe2RailDrop text = "FAT PIPE 2 RAIL DROP" score = 350
ENDSCRIPT

SCRIPT JS_OilG_Rail2FatPipeDrop_01
	StartGap GapID = JS_Rail2FatPipeDrop flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Rail2FatPipeDrop_02
	EndGap GapID = JS_Rail2FatPipeDrop Gapscript = JS_OilG_Rail2FatPipeDrop
ENDSCRIPT

SCRIPT JS_OilG_Rail2FatPipeDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Rail2FatPipeDrop
	Wait 1 frame
	EndGap GapID = Score_Rail2FatPipeDrop text = "RAIL 2 FAT PIPE DROP" score = 2000
ENDSCRIPT

SCRIPT JS_OilG_FatPipeSlantPipeXOver_01
	StartGap GapID = JS_FatPipeSlantPipeXOver01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_FatPipeSlantPipeXOver02 Gapscript = JS_OilS_FatPipeSlantPipeXOver
ENDSCRIPT

SCRIPT JS_OilG_FatPipeSlantPipeXOver_02
	StartGap GapID = JS_FatPipeSlantPipeXOver02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_FatPipeSlantPipeXOver01 Gapscript = JS_OilS_FatPipeSlantPipeXOver
ENDSCRIPT

SCRIPT JS_OilS_FatPipeSlantPipeXOver
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_FatPipeSlantPipeXOver
	Wait 1 frame
	EndGap GapID = Score_FatPipeSlantPipeXOver text = "FAT PIPE SLANT PIPE X OVER" score = 300
ENDSCRIPT

SCRIPT JS_OilG_OverPipeFatPipeXfer_01
	StartGap GapID = JS_OverPipeFatPipeXfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_OverPipeFatPipeXfer02 Gapscript = JS_OilS_OverPipeFatPipeXfer
ENDSCRIPT

SCRIPT JS_OilG_OverPipeFatPipeXfer_02
	StartGap GapID = JS_OverPipeFatPipeXfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_OverPipeFatPipeXfer01 Gapscript = JS_OilS_OverPipeFatPipeXfer
ENDSCRIPT

SCRIPT JS_OilS_OverPipeFatPipeXfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_OverPipeFatPipeXfer
	Wait 1 frame
	EndGap GapID = Score_OverPipeFatPipeXfer text = "OVER PIPE FAT PIPE XFER" score = 250
ENDSCRIPT

SCRIPT JS_OilG_WharfRailGap_01
	StartGap GapID = JS_WharfRailGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_WharfRailGap02 Gapscript = JS_OilS_WharfRailGap
ENDSCRIPT

SCRIPT JS_OilG_WharfRailGap_02
	StartGap GapID = JS_WharfRailGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_WharfRailGap01 Gapscript = JS_OilS_WharfRailGap
ENDSCRIPT

SCRIPT JS_OilS_WharfRailGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_WharfRailGap
	Wait 1 frame
	EndGap GapID = Score_WharfRailGap text = "WHARF RAIL GAP" score = 150
ENDSCRIPT

SCRIPT JS_OilG_WharfRailXOver_01
	StartGap GapID = JS_WharfRailXOver01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_WharfRailXOver02 Gapscript = JS_OilS_WharfRailXOver
ENDSCRIPT

SCRIPT JS_OilG_WharfRailXOver_02
	StartGap GapID = JS_WharfRailXOver02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
	EndGap GapID = JS_WharfRailXOver01 Gapscript = JS_OilS_WharfRailXOver
ENDSCRIPT

SCRIPT JS_OilS_WharfRailXOver
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_WharfRailXOver
	Wait 1 frame
	EndGap GapID = Score_WharfRailXOver text = "WHARF RAIL X OVER" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Wharf2Wharf_01
	StartGap GapID = JS_Wharf2Wharf01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Wharf2Wharf02 Gapscript = JS_OilS_Wharf2Wharf
ENDSCRIPT

SCRIPT JS_OilG_Wharf2Wharf_02
	StartGap GapID = JS_Wharf2Wharf02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Wharf2Wharf01 Gapscript = JS_OilS_Wharf2Wharf
ENDSCRIPT

SCRIPT JS_OilS_Wharf2Wharf
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Wharf2Wharf
	Wait 1 frame
	EndGap GapID = Score_Wharf2Wharf text = "WHARF 2 WHARF" score = 200
ENDSCRIPT

SCRIPT JS_OilG_BargeRailGap_01
	StartGap GapID = JS_BargeRailGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeRailGap02 Gapscript = JS_OilS_BargeRailGap
ENDSCRIPT

SCRIPT JS_OilG_BargeRailGap_02
	StartGap GapID = JS_BargeRailGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeRailGap01 Gapscript = JS_OilS_BargeRailGap
ENDSCRIPT

SCRIPT JS_OilS_BargeRailGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BargeRailGap
	Wait 1 frame
	EndGap GapID = Score_BargeRailGap text = "BARGE RAIL GAP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_BargeBridgeGap_01
	StartGap GapID = JS_BargeBridgeGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeBridgeGap02 Gapscript = JS_OilS_BargeBridgeGap
ENDSCRIPT

SCRIPT JS_OilG_BargeBridgeGap_02
	StartGap GapID = JS_BargeBridgeGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeBridgeGap01 Gapscript = JS_OilS_BargeBridgeGap
ENDSCRIPT

SCRIPT JS_OilS_BargeBridgeGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BargeBridgeGap
	Wait 1 frame
	EndGap GapID = Score_BargeBridgeGap text = "BARGE BRIDGE GAP" score = 150
ENDSCRIPT

SCRIPT JS_OilG_Barge2WharfRail_01
	StartGap GapID = JS_Barge2WharfRail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Barge2WharfRail_02
	EndGap GapID = JS_Barge2WharfRail01 Gapscript = JS_OilS_Barge2WharfRail
ENDSCRIPT

SCRIPT JS_OilS_Barge2WharfRail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Barge2WharfRail
	Wait 1 frame
	EndGap GapID = Score_Barge2WharfRail text = "BARGE 2 WHARF RAIL" score = 300
ENDSCRIPT

SCRIPT JS_OilG_Barge2WharfEdge_01
	StartGap GapID = JS_Barge2WharfEdge01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Barge2WharfEdge02 Gapscript = JS_OilS_Barge2WharfEdge
ENDSCRIPT

SCRIPT JS_OilG_Barge2WharfEdge_02
	StartGap GapID = JS_Barge2WharfEdge02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Barge2WharfEdge01 Gapscript = JS_OilS_Barge2WharfEdge
ENDSCRIPT

SCRIPT JS_OilS_Barge2WharfEdge
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Barge2WharfEdge
	Wait 1 frame
	EndGap GapID = Score_Barge2WharfEdge text = "BARGE 2 WHARF EDGE" score = 300
ENDSCRIPT

SCRIPT JS_OilG_PipeAGap_01
	StartGap GapID = JS_PipeAGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeAGap02 Gapscript = JS_OilS_PipeAGap
ENDSCRIPT

SCRIPT JS_OilG_PipeAGap_02
	StartGap GapID = JS_PipeAGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeAGap01 Gapscript = JS_OilS_PipeAGap
ENDSCRIPT

SCRIPT JS_OilS_PipeAGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_PipeAGap
	Wait 1 frame
	EndGap GapID = Score_PipeAGap text = "PIPE A GAP" score = 75
ENDSCRIPT

SCRIPT JS_OilG_PipeBGap_01
	StartGap GapID = JS_PipeBGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeBGap02 Gapscript = JS_OilS_PipeBGap
ENDSCRIPT

SCRIPT JS_OilG_PipeBGap_02
	StartGap GapID = JS_PipeBGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeBGap01 Gapscript = JS_OilS_PipeBGap
ENDSCRIPT

SCRIPT JS_OilS_PipeBGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_PipeBGap
	Wait 1 frame
	EndGap GapID = Score_PipeBGap text = "PIPE B GAP" score = 75
ENDSCRIPT

SCRIPT JS_OilG_PipeCGap_01
	StartGap GapID = JS_PipeCGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeCGap02 Gapscript = JS_OilS_PipeCGap
ENDSCRIPT

SCRIPT JS_OilG_PipeCGap_02
	StartGap GapID = JS_PipeCGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeCGap01 Gapscript = JS_OilS_PipeCGap
ENDSCRIPT

SCRIPT JS_OilS_PipeCGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_PipeCGap
	Wait 1 frame
	EndGap GapID = Score_PipeCGap text = "PIPE C GAP" score = 75
ENDSCRIPT

SCRIPT JS_OilG_PipeEGap_01
	StartGap GapID = JS_PipeEGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeEGap02 Gapscript = JS_OilS_PipeEGap
ENDSCRIPT

SCRIPT JS_OilG_PipeEGap_02
	StartGap GapID = JS_PipeEGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeEGap01 Gapscript = JS_OilS_PipeEGap
ENDSCRIPT

SCRIPT JS_OilS_PipeEGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_PipeEGap
	Wait 1 frame
	EndGap GapID = Score_PipeEGap text = "PIPE E GAP" score = 75
ENDSCRIPT

SCRIPT JS_OilG_PipeFGap_01
	StartGap GapID = JS_PipeFGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeFGap02 Gapscript = JS_OilS_PipeFGap
ENDSCRIPT

SCRIPT JS_OilG_PipeFGap_02
	StartGap GapID = JS_PipeFGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_PipeFGap01 Gapscript = JS_OilS_PipeFGap
ENDSCRIPT

SCRIPT JS_OilS_PipeFGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_PipeFGap
	Wait 1 frame
	EndGap GapID = Score_PipeFGap text = "PIPE F GAP" score = 75
ENDSCRIPT

SCRIPT JS_OilG_Walkway2CRail_01
	StartGap GapID = JS_Walkway2CRail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL ]
	EndGap GapID = JS_Walkway2CRail02 Gapscript = JS_OilS_Walkway2CRail
ENDSCRIPT

SCRIPT JS_OilG_Walkway2CRail_02
	StartGap GapID = JS_Walkway2CRail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL ]
	EndGap GapID = JS_Walkway2CRail01 Gapscript = JS_OilS_Walkway2CRail
ENDSCRIPT

SCRIPT JS_OilS_Walkway2CRail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Walkway2CRail
	Wait 1 frame
	EndGap GapID = Score_Walkway2CRail text = "WALKWAY RAIL 2 C-RAIL" score = 550
ENDSCRIPT

SCRIPT JS_OilG_CraneRail2Rail_01
	StartGap GapID = JS_CraneRail2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_CraneRail2Rail02 Gapscript = JS_OilS_CraneRail2Rail
ENDSCRIPT

SCRIPT JS_OilG_CraneRail2Rail_02
	StartGap GapID = JS_CraneRail2Rail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_CraneRail2Rail01 Gapscript = JS_OilS_CraneRail2Rail
ENDSCRIPT

SCRIPT JS_OilS_CraneRail2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_CraneRail2Rail
	Wait 1 frame
	EndGap GapID = Score_CraneRail2Rail text = "CRANE RAIL 2 RAIL" score = 800
ENDSCRIPT

SCRIPT JS_OilG_FanRail2BargeRail_01
	StartGap GapID = JS_FanRail2BargeRail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_FanRail2BargeRail02 Gapscript = JS_OilS_FanRail2BargeRail
ENDSCRIPT

SCRIPT JS_OilG_FanRail2BargeRail_02
	StartGap GapID = JS_FanRail2BargeRail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_FanRail2BargeRail01 Gapscript = JS_OilS_FanRail2BargeRail
ENDSCRIPT

SCRIPT JS_OilS_FanRail2BargeRail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_FanRail2BargeRail
	Wait 1 frame
	EndGap GapID = Score_FanRail2BargeRail text = "FAN RAIL 2 BARGE RAIL" score = 1000
ENDSCRIPT

SCRIPT JS_OilG_FanRail2Rail_01
	StartGap GapID = JS_FanRail2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_FanRail2Rail02 Gapscript = JS_OilS_FanRail2Rail
ENDSCRIPT

SCRIPT JS_OilG_FanRail2Rail_02
	StartGap GapID = JS_FanRail2Rail02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_FanRail2Rail01 Gapscript = JS_OilS_FanRail2Rail
ENDSCRIPT

SCRIPT JS_OilS_FanRail2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_FanRail2Rail
	Wait 1 frame
	EndGap GapID = Score_FanRail2Rail text = "FAN RAIL 2 RAIL" score = 2000
ENDSCRIPT

SCRIPT JS_OilG_Crane2Pipes_01
	StartGap GapID = JS_Crane2Pipes flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Crane2Pipes_02
	EndGap GapID = JS_Crane2Pipes Gapscript = JS_OilS_Crane2Pipes
ENDSCRIPT

SCRIPT JS_OilS_Crane2Pipes
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Crane2Pipes
	Wait 1 frame
	EndGap GapID = Score_Crane2Pipes text = "CATCHING THE LINE" score = 150
ENDSCRIPT

SCRIPT JS_OilG_Up2Crane_01
	StartGap GapID = JS_Up2Crane flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Up2Crane_02
	EndGap GapID = JS_Up2Crane Gapscript = JS_OilS_Up2Crane
ENDSCRIPT

SCRIPT JS_OilS_Up2Crane
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Up2Crane
	Wait 1 frame
	EndGap GapID = Score_Up2Crane text = "UP 2 THE CRANE" score = 200
ENDSCRIPT

SCRIPT JS_OilG_Up2CraneRail_01
	StartGap GapID = JS_Up2CraneRail flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Up2CraneRail_02
	EndGap GapID = JS_Up2CraneRail Gapscript = JS_OilS_Up2CraneRail
ENDSCRIPT

SCRIPT JS_OilS_Up2CraneRail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Up2CraneRail
	Wait 1 frame
	EndGap GapID = Score_Up2CraneRail text = "UP 2 CRANE RAIL" score = 300
ENDSCRIPT

SCRIPT JS_OilG_Bowl2BowlTransfer_01
	StartGap GapID = JS_Bowl2BowlTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Bowl2BowlTransfer02 Gapscript = JS_OilS_Bowl2BowlTransfer
ENDSCRIPT

SCRIPT JS_OilG_Bowl2BowlTransfer_02
	StartGap GapID = JS_Bowl2BowlTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Bowl2BowlTransfer01 Gapscript = JS_OilS_Bowl2BowlTransfer
ENDSCRIPT

SCRIPT JS_OilS_Bowl2BowlTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Bowl2BowlTransfer
	Wait 1 frame
	EndGap GapID = Score_Bowl2BowlTransfer text = "BOWL 2 BOWL TRANSFER" score = 450
ENDSCRIPT

SCRIPT JS_OilG_BowlTransfer_01
	StartGap GapID = JS_BowlTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlTransfer02 Gapscript = JS_OilS_BowlTransfer
ENDSCRIPT

SCRIPT JS_OilG_BowlTransfer_02
	StartGap GapID = JS_BowlTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlTransfer01 Gapscript = JS_OilS_BowlTransfer
ENDSCRIPT

SCRIPT JS_OilS_BowlTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BowlTransfer
	Wait 1 frame
	EndGap GapID = Score_BowlTransfer text = "BOWL TRANSFER" score = 250
ENDSCRIPT

SCRIPT JS_OilG_Quarter2BowlTransfer_01
	StartGap GapID = JS_Quarter2BowlTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Quarter2BowlTransfer02 Gapscript = JS_OilS_Quarter2BowlTransfer
ENDSCRIPT

SCRIPT JS_OilG_Quarter2BowlTransfer_02
	StartGap GapID = JS_Quarter2BowlTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Quarter2BowlTransfer01 Gapscript = JS_OilS_Quarter2BowlTransfer
ENDSCRIPT

SCRIPT JS_OilS_Quarter2BowlTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Quarter2BowlTransfer
	Wait 1 frame
	EndGap GapID = Score_Quarter2BowlTransfer text = "QUARTER 2 BOWL TRANSFER" score = 450
ENDSCRIPT

SCRIPT JS_OilG_Bank2Rail_01
	StartGap GapID = JS_Bank2Rail01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_Bank2Rail_02
	EndGap GapID = JS_Bank2Rail01 Gapscript = JS_OilS_Bank2Rail
ENDSCRIPT

SCRIPT JS_OilS_Bank2Rail
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Bank2Rail
	Wait 1 frame
	EndGap GapID = Score_Bank2Rail text = "BANK 2 RAIL" score = 250
ENDSCRIPT

SCRIPT JS_OilG_OverPipesTransfer_01
	StartGap GapID = JS_OverPipesTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_OverPipesTransfer02 Gapscript = JS_OilS_OverPipesTransfer
ENDSCRIPT

SCRIPT JS_OilG_OverPipesTransfer_02
	StartGap GapID = JS_OverPipesTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_OverPipesTransfer01 Gapscript = JS_OilS_OverPipesTransfer
ENDSCRIPT

SCRIPT JS_OilS_OverPipesTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_OverPipesTransfer
	Wait 1 frame
	EndGap GapID = Score_OverPipesTransfer text = "OVER PIPES TRANSFER" score = 500
ENDSCRIPT

SCRIPT JS_OilG_SlantPipeOverPipe_01
	StartGap GapID = JS_SlantPipeOverPipe flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_SlantPipeOverPipe_02
	EndGap GapID = JS_SlantPipeOverPipe Gapscript = JS_OilS_SlantPipeOverPipe
ENDSCRIPT

SCRIPT JS_OilS_SlantPipeOverPipe
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SlantPipeOverPipe
	Wait 1 frame
	EndGap GapID = Score_SlantPipeOverPipe text = "SLANT PIPE OVER PIPE" score = 350
ENDSCRIPT

SCRIPT JS_OilG_SmallFanTransfer_01
	StartGap GapID = JS_SmallFanTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SmallFanTransfer02 Gapscript = JS_OilS_SmallFanTransfer
ENDSCRIPT

SCRIPT JS_OilG_SmallFanTransfer_02
	StartGap GapID = JS_SmallFanTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SmallFanTransfer01 Gapscript = JS_OilS_SmallFanTransfer
ENDSCRIPT

SCRIPT JS_OilG_SmallFanTransfer_03
	StartGap GapID = JS_SmallFanTransfer03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SmallFanTransfer04 Gapscript = JS_OilS_SmallFanTransfer
ENDSCRIPT

SCRIPT JS_OilG_SmallFanTransfer_04
	StartGap GapID = JS_SmallFanTransfer04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_SmallFanTransfer03 Gapscript = JS_OilS_SmallFanTransfer
ENDSCRIPT

SCRIPT JS_OilS_SmallFanTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_SmallFanTransfer
	Wait 1 frame
	EndGap GapID = Score_SmallFanTransfer text = "SMALL FAN TRANSFER" score = 250
ENDSCRIPT

SCRIPT JS_OilG_BigFanTransfer_01
	StartGap GapID = JS_BigFanTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BigFanTransfer02 Gapscript = JS_OilS_BigFanTransfer
ENDSCRIPT

SCRIPT JS_OilG_BigFanTransfer_02
	StartGap GapID = JS_BigFanTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BigFanTransfer01 Gapscript = JS_OilS_BigFanTransfer
ENDSCRIPT

SCRIPT JS_OilS_BigFanTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BigFanTransfer
	Wait 1 frame
	EndGap GapID = Score_BigFanTransfer text = "BIG FAN TRANSFER" score = 750
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingTransfer_01
	StartGap GapID = JS_TransferBuildingTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingTransfer02 Gapscript = JS_OilS_TransferBuildingTransfer
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingTransfer_02
	StartGap GapID = JS_TransferBuildingTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingTransfer01 Gapscript = JS_OilS_TransferBuildingTransfer
ENDSCRIPT

SCRIPT JS_OilS_TransferBuildingTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_TransferBuildingTransfer
	Wait 1 frame
	EndGap GapID = Score_TransferBuildingTransfer text = "TRANSFER BUILDING TRANSFER" score = 75
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingLevelTransfer_01
	StartGap GapID = JS_TransferBuildingLevelTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingLevelTransfer02 Gapscript = JS_OilS_TransferBuildingLevelTransfer
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingLevelTransfer_02
	StartGap GapID = JS_TransferBuildingLevelTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingLevelTransfer01 Gapscript = JS_OilS_TransferBuildingLevelTransfer
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingLevelTransfer_03
	StartGap GapID = JS_TransferBuildingLevelTransfer03 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingLevelTransfer04 Gapscript = JS_OilS_TransferBuildingLevelTransfer
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingLevelTransfer_04
	StartGap GapID = JS_TransferBuildingLevelTransfer04 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingLevelTransfer03 Gapscript = JS_OilS_TransferBuildingLevelTransfer
ENDSCRIPT

SCRIPT JS_OilS_TransferBuildingLevelTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_TransferBuildingLevelTransfer
	Wait 1 frame
	EndGap GapID = Score_TransferBuildingLevelTransfer text = "TRANSFER BUILDING LEVEL TRANSFER" score = 300
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingDrop_01
	StartGap GapID = JS_TransferBuildingDrop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingDrop_02
	EndGap GapID = JS_TransferBuildingDrop01 Gapscript = JS_OilS_TransferBuildingDrop
ENDSCRIPT

SCRIPT JS_OilS_TransferBuildingDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_TransferBuildingDrop
	Wait 1 frame
	EndGap GapID = Score_TransferBuildingDrop text = "TRANSFER BUILDING DROP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingBargeDrop_01
	StartGap GapID = JS_TransferBuildingBargeDrop01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingBargeDrop02 Gapscript = JS_OilS_TransferBuildingDrop
ENDSCRIPT

SCRIPT JS_OilG_TransferBuildingBargeDrop_02
	StartGap GapID = JS_TransferBuildingBargeDrop02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_TransferBuildingBargeDrop01 Gapscript = JS_OilS_TransferBuildingDrop
ENDSCRIPT

SCRIPT JS_OilS_TransferBuildingBargeDrop
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_TransferBuildingBargeDrop
	Wait 1 frame
	EndGap GapID = Score_TransferBuildingBargeDrop text = "TRANSFER BUILDING BARGE DROP" score = 3000
ENDSCRIPT

SCRIPT JS_OilG_BowlLevelTransfer_01
	StartGap GapID = JS_BowlLevelTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlLevelTransfer02 Gapscript = JS_OilS_BowlLevelTransfer
ENDSCRIPT

SCRIPT JS_OilG_BowlLevelTransfer_02
	StartGap GapID = JS_BowlLevelTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BowlLevelTransfer01 Gapscript = JS_OilS_BowlLevelTransfer
ENDSCRIPT

SCRIPT JS_OilS_BowlLevelTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BowlLevelTransfer
	Wait 1 frame
	EndGap GapID = Score_BowlLevelTransfer text = "BOWL LEVEL TRANSFER" score = 150
ENDSCRIPT

SCRIPT JS_OilG_BargeTransfer_01
	StartGap GapID = JS_BargeTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeTransfer02 Gapscript = JS_OilS_BargeTransfer
ENDSCRIPT

SCRIPT JS_OilG_BargeTransfer_02
	StartGap GapID = JS_BargeTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeTransfer01 Gapscript = JS_OilS_BargeTransfer
ENDSCRIPT

SCRIPT JS_OilS_BargeTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BargeTransfer
	Wait 1 frame
	EndGap GapID = Score_BargeTransfer text = "BARGE TRANSFER" score = 75
ENDSCRIPT

SCRIPT JS_OilG_BargeRampGap_01
	StartGap GapID = JS_BargeRampGap01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeRampGap02 Gapscript = JS_OilS_BargeRampGap
ENDSCRIPT

SCRIPT JS_OilG_BargeRampGap_02
	StartGap GapID = JS_BargeRampGap02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_BargeRampGap01 Gapscript = JS_OilS_BargeRampGap
ENDSCRIPT

SCRIPT JS_OilS_BargeRampGap
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_BargeRampGap
	Wait 1 frame
	EndGap GapID = Score_BargeRampGap text = "BARGE RAMP GAP" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Bumper2BumperTransfer_01
	StartGap GapID = JS_Bumper2BumperTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Bumper2BumperTransfer02 Gapscript = JS_OilS_Bumper2BumperTransfer
ENDSCRIPT

SCRIPT JS_OilG_Bumper2BumperTransfer_02
	StartGap GapID = JS_Bumper2BumperTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_Bumper2BumperTransfer01 Gapscript = JS_OilS_Bumper2BumperTransfer
ENDSCRIPT

SCRIPT JS_OilS_Bumper2BumperTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_Bumper2BumperTransfer
	Wait 1 frame
	EndGap GapID = Score_Bumper2BumperTransfer text = "BUMPER 2 BUMPER TRANSFER" score = 600
ENDSCRIPT

SCRIPT JS_OilG_HelipadLevelTransfer_01
	StartGap GapID = JS_HelipadLevelTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_HelipadLevelTransfer02 Gapscript = JS_OilS_HelipadLevelTransfer
ENDSCRIPT

SCRIPT JS_OilG_HelipadLevelTransfer_02
	StartGap GapID = JS_HelipadLevelTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_HelipadLevelTransfer01 Gapscript = JS_OilS_HelipadLevelTransfer
ENDSCRIPT

SCRIPT JS_OilS_HelipadLevelTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_HelipadLevelTransfer
	Wait 1 frame
	EndGap GapID = Score_HelipadLevelTransfer text = "HELIPAD LEVEL TRANSFER" score = 250
ENDSCRIPT

SCRIPT JS_OilG_HelipadBaseTransfer_01
	StartGap GapID = JS_HelipadBaseTransfer01 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_HelipadBaseTransfer02 Gapscript = JS_OilS_HelipadBaseTransfer
ENDSCRIPT

SCRIPT JS_OilG_HelipadBaseTransfer_02
	StartGap GapID = JS_HelipadBaseTransfer02 flags = [ CANCEL_GROUND CANCEL_LIP CANCEL_MANUAL CANCEL_RAIL CANCEL_WALL ]
	EndGap GapID = JS_HelipadBaseTransfer01 Gapscript = JS_OilS_HelipadBaseTransfer
ENDSCRIPT

SCRIPT JS_OilS_HelipadBaseTransfer
	Playsound HUD_JumpGap vol = 100
	StartGap GapID = Score_HelipadBaseTransfer
	Wait 1 frame
	EndGap GapID = Score_HelipadBaseTransfer text = "HELIPAD BASE TRANSFER" score = 100
ENDSCRIPT

SCRIPT JS_OilG_Lip_BargeCabin_01
	StartGap GapID = JS_Lip_BargeCabin flags = [ CANCEL_MANUAL CANCEL_GROUND ]
ENDSCRIPT

SCRIPT JS_OilG_Lip_BargeCabin_02
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Lip_BargeCabin text = "BARGE CABIN LIP" score = 100
	ENDIF
ENDSCRIPT

SCRIPT JS_OilG_Lip_TransferWindow_01
	StartGap GapID = JS_Lip_TransferWindow flags = [ CANCEL_MANUAL CANCEL_GROUND ]
ENDSCRIPT

SCRIPT JS_OilG_Lip_TransferWindow_02
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Lip_TransferWindow text = "TRANSFER BUILDING WINDOW LIP" score = 500
	ENDIF
ENDSCRIPT

SCRIPT JS_OilG_Lip_TransferFan_01
	StartGap GapID = JS_Lip_TransferFan flags = [ CANCEL_MANUAL CANCEL_GROUND ]
ENDSCRIPT

SCRIPT JS_OilG_Lip_TransferFan_02
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Lip_TransferFan text = "TRANSFER BUILDING FAN LIP" score = 250
	ENDIF
ENDSCRIPT

SCRIPT JS_OilG_Lip_HelipadLo_01
	StartGap GapID = JS_Lip_HelipadLo flags = [ CANCEL_MANUAL CANCEL_GROUND ]
ENDSCRIPT

SCRIPT JS_OilG_Lip_HelipadLo_02
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Lip_HelipadLo text = "HELIPAD LOW LIP" score = 100
	ENDIF
ENDSCRIPT

SCRIPT JS_OilG_Lip_HelipadHi_01
	StartGap GapID = JS_Lip_HelipadHi flags = [ CANCEL_MANUAL CANCEL_GROUND ]
ENDSCRIPT

SCRIPT JS_OilG_Lip_HelipadHi_02
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Lip_HelipadHi text = "HELIPAD HIGH LIP" score = 1000
	ENDIF
ENDSCRIPT

SCRIPT JS_OilG_Lip_Up2L1_01
	StartGap GapID = JS_Lip_Up2L1 flags = [ CANCEL_MANUAL CANCEL_GROUND ]
ENDSCRIPT

SCRIPT JS_OilG_Lip_Up2L1_02
	IF Triggertype [ TRIGGER_LIP_ON ]
		EndGap GapID = JS_Lip_Up2L1 text = "UP 2 LEVEL 1 LIP" score = 150
	ENDIF
ENDSCRIPT
