//All new LevelMod Scripts
//Created: 2020-08-29
Script KillSafely
    IF IsAlive name = <name>
        Kill name = <name>
    ENDIF
EndScript

Script UnPauseSkaters_Script
UnPauseSkaters
printf "Finished reloading NodeArray"
EndScript

Script ReloadNodeArray_Script
  PauseSkaters
  ReloadNodeArray
EndScript

//Render customization values
uv_anim_threshold = 0.02
uv_tiling_threshold = 1.0

SCRIPT Float_Generic01 TurnDist = 40
	Obj_StickToGround off distAbove = 1 distBelow = 1
	Obj_SetGroundOffset 0.1
	Obj_SetPathAcceleration 0 mphps
	Obj_SetPathVelocity 4.6 mph
	Obj_SetPathTurnDist <TurnDist>
	Obj_SetPathAcceleration 30 mphps
	Obj_SetPathDeceleration 20 mphps
	Obj_SetPathMinStopVel 5
	Obj_SetFlag FLAG_CAR_TYPE_BUS
	IF GotParam nosound
		Obj_SetFlag FLAG_CAR_GENERIC_NOSOUND
	ELSE
		Obj_PlaySound carloop
	ENDIF
	Obj_FollowPathLinked
	Obj_SetException Ex = SCH_FLOAT_START_RUN scr = Float_Generic_Start Params = <...>
	Wait 1 frame
	Obj_StopAlongPath
	Sch_TrafficGone
	BEGIN
		Wait 1000 frame
	REPEAT
ENDSCRIPT

SCRIPT Float_Generic_Start
	Obj_StartAlongPath
	Wait 60 frames
	IF GotParam music
		Obj_PlayStream parademusic use_pos_info = 0 Vol = 300
		Wait 60 seconds
		Obj_PlayStream parademusic use_pos_info = 0
		Wait 60 seconds
	ENDIF
ENDSCRIPT

SCRIPT CPF_SetUpStandardPed
	Obj_StickToGround distAbove = 3 distBelow = 6
	Obj_SetPathAcceleration 0 mphps
	Obj_SetPathTurnDist 2.5 feet
	Obj_StickToGround off
	Obj_SetConstantHeight
	Obj_PathHeading On
	Obj_SetPathVelocity 20 fps
	Obj_SetPathAcceleration 35 fpsps
	Obj_SetPathDeceleration 35 fpsps
ENDSCRIPT

SCRIPT Car_Viper01 TurnDist = 20
	Obj_StickToGround distAbove = 3 distBelow = 6
	Obj_SetGroundOffset 0.1
	Obj_SetPathAcceleration 0 mphps
	Obj_SetPathTurnDist <TurnDist>
	Obj_FollowPathLinked
	Obj_SetPathAcceleration 60 mphps
	Obj_SetPathDeceleration 40 mphps
	Obj_SetPathMinStopVel 5
	Obj_Spawnscript Car_CheckForOtherCar01 Params = { PathNum = <PathNum> }
	Obj_SetPathVelocity 75 mph
	Obj_PlaySound carloop
	IF GotParam NoSkater
	ELSE
		Obj_Spawnscript Car_CheckForSkater01
	ENDIF
ENDSCRIPT

SCRIPT Sch_BB_Defense_Shooter
	CPF_SetUpStandardPed
	BEGIN
		//Obj_PlayAnim Anim = Ped_BP_GuardPlayer Cycle
		BEGIN
			IF Obj_FlagSet OBJFLAG_BBALL_NAV1
				Obj_ClearFlag OBJFLAG_BBALL_NAV1
				Wait random( @10 frames @20 frames @30 frames ) 
				Sch_BB_RunToNav Sch_BBall_Nav = TRG_Ped_BB_D2_Nav01
				Obj_LookAtNode Name = TRG_Ped_BB_O2_Nav01
				BREAK
			ENDIF
			IF Obj_FlagSet OBJFLAG_BBALL_NAV2
				Obj_ClearFlag OBJFLAG_BBALL_NAV2
				Wait random( @10 frames @20 frames @30 frames ) 
				Sch_BB_RunToNav Sch_BBall_Nav = TRG_Ped_BB_D2_Nav02
				Obj_LookAtNode Name = TRG_Ped_BB_O2_Nav02
				BREAK
			ENDIF
			IF Obj_FlagSet OBJFLAG_BBALL_NAV3
				Obj_ClearFlag OBJFLAG_BBALL_NAV3
				Wait random( @10 frames @20 frames @30 frames ) 
				Sch_BB_RunToNav Sch_BBall_Nav = TRG_Ped_BB_D2_Nav03
				Obj_LookAtNode Name = TRG_Ped_BB_O2_Nav03
				BREAK
			ENDIF
			IF Obj_FlagSet OBJFLAG_BBALL_NAV4
				Obj_ClearFlag OBJFLAG_BBALL_NAV4
				Wait random( @10 frames @20 frames @30 frames ) 
				Sch_BB_RunToNav Sch_BBall_Nav = TRG_Ped_BB_D2_Nav04
				Obj_LookAtNode Name = TRG_Ped_BB_O2_Nav04
				BREAK
			ENDIF
			Wait 10 frame
		REPEAT
	REPEAT
ENDSCRIPT

SCRIPT CPF_AutoAccel
	Obj_AdjustSound carloop volumePercent = 100 volumeStep = 0.7 pitchPercent = 100 pitchStep = 0.7
	Obj_SetPathVelocity 50 mph
ENDSCRIPT

SCRIPT CPF_AutoDecel
	Obj_AdjustSound carloop volumePercent = 80 volumeStep = 2 pitchPercent = 50 pitchStep = 1
	Obj_SetPathVelocity 25
ENDSCRIPT

SCRIPT CPF_ChaseCarCheckDistance
	Wait 10 gameframes
	IF Obj_ObjectInRect dist = 40 Name = TRG_Looter_Car
		Obj_SetPathVelocity 65 fps
		Wait 20 gameframes
		Obj_SetPathVelocity 75 fps
	ENDIF
	IF Obj_ObjectInRect dist = 20 Name = TRG_Police_Car02
		Obj_SetPathVelocity 65 fps
		Wait 20 gameframes
		Obj_SetPathVelocity 75 fps
	ENDIF
ENDSCRIPT

SCRIPT CPF_Chase180Left
	Obj_PlaySound random( @CarBrakeSqueal @LA_skid_11  ) random( @pitch = 50 @pitch = 80 @pitch = 100 @pitch = 130  ) Vol = 50
	Obj_PathHeading off
	Obj_Rotate relative = (0, 0, 90) time = 0.3 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, 120) time = 0.4 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, -40) time = 0.2 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_PlaySound random( @CarBrakeSqueal @LA_skid_11  ) random( @pitch = 150 @pitch = 80 @pitch = 100 @pitch = 130  ) Vol = 30
	Obj_Rotate relative = (0, 0, 15) time = 0.1 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, -5) time = 0.1 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_PathHeading On
	Obj_SetPathVelocity 75 mph
ENDSCRIPT

SCRIPT CPF_ChaseTurnRight
	Obj_SetPathVelocity 15 mph
	Obj_PathHeading off
	Obj_Rotate relative = (0, 0, -120) time = 0.5 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, 40) time = 0.2 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, -15) time = 0.1 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, 5) time = 0.1 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_PathHeading On
	Obj_SetPathVelocity 75 mph
ENDSCRIPT

SCRIPT CPF_ChaseTurnLeft
	Obj_PathHeading off
	Obj_Rotate relative = (0, 0, 120) time = 0.5 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, -40) time = 0.2 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, 15) time = 0.1 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_Rotate relative = (0, 0, -5) time = 0.1 FLAG_MAX_COORDS
	Obj_WaitRotate
	Obj_PathHeading On
ENDSCRIPT

SCRIPT Car_HeightCheck
	Obj_StickToGround distAbove = 3 distBelow = 3
ENDSCRIPT

SCRIPT Car_NoHeightCheck
	Obj_StickToGround off
	Obj_SetConstantHeight
ENDSCRIPT

SCRIPT GenericPro
ENDSCRIPT

SCRIPT GenericObject
ENDSCRIPT

SCRIPT CameraScript1
	PlaySound gapsound
ENDSCRIPT

SCRIPT CameraScript2
	PlaySound goaldone
ENDSCRIPT

SCRIPT BobbingObject
	Obj_RotY speed = 250
	BEGIN
		Obj_MoveToRelPos (0, 2, 0) time = 0.1
		Obj_WaitMove
		Obj_MoveToRelPos (0, 6, 0) time = 0.1
		Obj_WaitMove
		Obj_MoveToRelPos (0, 2, 0) time = 0.1
		Obj_WaitMove
		Obj_MoveToRelPos (0, 1, 0) time = 0.1
		Obj_WaitMove
		Obj_MoveToRelPos (0, -2, 0) time = 0.1
		Obj_WaitMove
		Obj_MoveToRelPos (0, -6, 0) time = 0.1
		Obj_WaitMove
		Obj_MoveToRelPos (0, -2, 0) time = 0.1
		Obj_WaitMove
		Obj_MoveToRelPos (0, -1, 0) time = 0.1
		Obj_WaitMove
	REPEAT
ENDSCRIPT




SCRIPT Pigeon_Generic
	Obj_ShadowOff
	Pigeon_Generic_Idle
ENDSCRIPT

SCRIPT Pigeon_Generic_Idle
	Obj_ClearExceptions
	Obj_SetInnerRadius 15
	Obj_SetException Ex = SkaterInRadius scr = Pigeon_Generic_SkaterNear
	BEGIN
		Obj_PlaySound Pigeon
		//Obj_CycleAnim anim = Idle numcycles = RANDOMRANGE(1, 3)
		//Obj_CycleAnim anim = peckfromidle
		BEGIN
			Obj_RotY speed = 25
			//Obj_CycleAnim anim = peck
			Obj_StopRotating
		REPEAT numcycles RANDOMRANGE(1, 5)
		//Obj_CycleAnim anim = pecktoidle
		//Obj_CycleAnim anim = Idle
		//Obj_CycleAnim anim = preen
	REPEAT
ENDSCRIPT

SCRIPT Pigeon_Generic_SkaterNear
	Obj_ClearExceptions
	//Obj_GetRandomLink
	//Obj_MoveToLink speed = 8 LinkNum = <link>
	//Obj_LookAtNodeLinked time = 0.2 LinkNum = <link>
	Obj_SetInnerRadius 5
	Obj_SetException Ex = SkaterInRadius scr = Pigeon_Generic_SkaterReallyNear
	//Obj_PlayAnim anim = run cycle
	Obj_WaitMove
	Obj_SetAnimCycleMode Off
	Obj_WaitAnimFinished
	Obj_CycleAnim anim = runtoidle
	Obj_SetInnerRadius 15
	Obj_SetException Ex = SkaterInRadius scr = Pigeon_Generic_SkaterNear
	goto Pigeon_Generic_Idle
ENDSCRIPT

SCRIPT Pigeon_Generic_SkaterReallyNear
	Obj_ClearExceptions
	Obj_PlaySound PigeonFly
	Obj_CycleAnim anim = FlyHop
	Obj_WaitMove
	Obj_SetInnerRadius 15
	Obj_SetException Ex = SkaterInRadius scr = Pigeon_Generic_SkaterNear
	goto Pigeon_Generic_Idle
ENDSCRIPT

SCRIPT SK4_TrafficLight01
	Obj_SetFlag FLAG_TRAFFICLIGHT_GREEN
	create name = <Green>
	wait RANDOMRANGE(5000, 12000)
	BEGIN
		Obj_SetFlag FLAG_TRAFFICLIGHT_YELLOW
		Obj_ClearFlag FLAG_TRAFFICLIGHT_GREEN
		kill name = <Green>
		create name = <Yellow>
		wait 3000
		Obj_SetFlag FLAG_TRAFFICLIGHT_RED
		Obj_ClearFlag FLAG_TRAFFICLIGHT_YELLOW
		kill name = <Yellow>
		create name = <Red>
		wait RANDOMRANGE(9000, 15000)
		Obj_SetFlag FLAG_TRAFFICLIGHT_GREEN
		Obj_ClearFlag FLAG_TRAFFICLIGHT_RED
		kill name = <Red>
		create name = <Green>
		wait RANDOMRANGE(7000, 12000)
	REPEAT
ENDSCRIPT

SCRIPT Ped_AI_Skater Path = TRG_Ped_AI_SkaterNav01
	Obj_SetPathVelocity 35 fps
	Obj_SetPathAcceleration 55 fpsps
	Obj_PathHeading On
	Obj_StickToGround distAbove = 3 distBelow = 16
	Obj_SetPathTurnDist 10 feet
	Obj_SetPathAcceleration 43 fpsps
	Obj_FollowPathLinked
	Obj_PlayAnim Anim = Crouchidle Cycle
ENDSCRIPT

SCRIPT Ped_InitAI
    CPF_PedGeneric <...>
ENDSCRIPT
