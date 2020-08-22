
WasInAir = 0

SCRIPT LaunchPanelMessage2
	IF gotparam id
		IF gotparam text
			LaunchPanelMessage properties = <id> <text>
		ELSE
			LaunchPanelMessage properties = <id> <...>
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT ScottKills
	sk3_killskater deathsound = boneless09 message1 = "poopoo" message2 = "waaaaa" message3 = "yowsers" message4 = "Uyeda likes ass" r = 0 g = 255 b = 0
ENDSCRIPT

SCRIPT GroundTele
	InAirExceptions
	SetException Ex = Ollied Scr = Ollie
	ClearException Ollied
	goto Airborne
	Wait 0.25 Seconds
ENDSCRIPT

SCRIPT Teleporting
	waitonegameframe
	waitonegameframe
	waitonegameframe
	waitonegameframe
	waitonegameframe
	IF InAir
	ELSE
		IF OnGround
			MakeSkaterGoto GroundTele
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT sk3_killskater
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		Obj_SetFlag FLAG_SKATER_KILLING
		InBail
		IF gotparam deathsound
			PlaySound <deathsound>
		ELSE
			PlaySound hitbody02 pitch = 90 vol = 150
			PlaySound bail_knee1 pitch = 90 vol = 50
		ENDIF
		IF gotparam message1
			LaunchPanelMessage properties = panel_message_death random( 
			@<message1>
			@<message2>
			@<message3>
			@<message4>) 
		ELSE
			LaunchPanelMessage properties = panel_message_death random( 
			@"&c1You suck!"
			@"&c1Dont do drugs!"
			@"&c1Nice one!"
			@"&c1Good one!"
			@"&c1Hmm..."
			@"&c1Loser!"
			@"&c1Out of bounds!") 
		ENDIF
		SK3_Killskater_Finish <...>
	ENDIF
ENDSCRIPT

SCRIPT SK3_TeleportToNode r = 255 g = 255 b = 255
	GetSpeed
	IF InAir
		change WasInAir = 1
		SpawnSkaterScript Teleporting
	ENDIF
	IF gotparam message1
		IF gotparam cut
			LaunchPanelMessage <message1> properties = thps4_message_props
		ELSE
			LaunchPanelMessage <message1> properties = panel_message_death
		ENDIF
	ENDIF
	IF OnRail
		StopBalanceTrick
		MakeSkaterGoto GroundGone
	ENDIF
	IF gotparam nodename
		Obj_MoveToNode Name = <nodename> Orient NoReset
		SetSpeed <Speed>
	ELSE
		LaunchPanelMessage properties = panel_message_death "ERROR! Please pass in nodename="
	ENDIF
	IF gotparam sound
		PlaySound <sound>
	ENDIF
	IF IsTrue WasInAir
		IF InAir
		ELSE
			ClearExceptions
		ENDIF
		change WasInAir = 0
	ENDIF
ENDSCRIPT

SCRIPT SK3_KillSkater_Water
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		FreezeCamera
		Obj_SetFlag FLAG_SKATER_KILLING
		PlaySound FallWater
		LaunchPanelMessage properties = panel_message_death random( 
		@"&c1All wet"
		@"&c1You big drip!"
		@"&c1WATER...BAAAD!"
		@"&c1No swimming"
		@"&c1I'm drowning!"
		@"&c1You're hosed!") 
		SK3_Killskater_Finish poopoo r = 100 g = 100 b = 200 camerafreezed <...>
		UnfreezeCamera
	ENDIF
ENDSCRIPT

SCRIPT SK3_KillSkater_Pungee
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		Obj_SetFlag FLAG_SKATER_KILLING
		PlaySound FallPungee
		LaunchPanelMessage properties = panel_message_death random( 
		@"&c1You got shafted"
		@"&c1Who built that?!"
		@"&c1Serve, set, spike!"
		@"&c1Shish kabob?"
		@"&c1Don't build this at home!") 
		SK3_Killskater_Finish r = 150 g = 50 b = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT SK3_Killskater_Finish r = 255 g = 0 b = 0
	printstruct
	ClearExceptions
	DisablePlayerInput
	IF gotparam Bail
		MakeSkaterGoto Killskater_Bail
	ENDIF
	BlendPeriodOut 0
	DoFlash { duration = 0.25 start_r = <r> start_g = <g> start_b = <b> start_a = 0 end_r = <r> end_g = <g> end_b = <b> end_a = 100 }
	Wait 0.25 Seconds
	ClearTrickQueues
	Wait 1 gameframe
	IF gotparam camerafreezed
		UnfreezeCamera
	ENDIF
	Killskater
	IF gotparam randomrespawn
		MakeSkaterGoto SkaterToRandom
	ELSE
		IF gotparam TeleTo
			Obj_MoveToNode Name = <TeleTo> Orient
		ENDIF
	ENDIF
	ClearGapTricks
	ClearEventBuffer
	ClearPanel_Bailed
	DoFlash { duration = 0.75 start_r = <r> start_g = <g> start_b = <b> start_a = 100 end_r = <r> end_g = <g> end_b = <b> end_a = 0 }
	Wait 0.75 Seconds
	NotInBail
	Obj_ClearFlag FLAG_SKATER_KILLING
ENDSCRIPT

SCRIPT Killskater_Bail
	PlayAnim Anim = SlipForwards BlendPeriod = 0.3
ENDSCRIPT

SCRIPT GrafCounterThread
	Wait 1 Seconds
	LaunchPanelMessage "Tags: 0" properties = tags_message
	KillSpawnedScript Name = GrafCounterThread
ENDSCRIPT

SCRIPT LaunchGrafCounter
	IF gotparam text
		KillSpawnedScript Name = GrafCounterThread
		LaunchPanelMessage <text> properties = tags_message
	ELSE
		SpawnScript GrafCounterThread
	ENDIF
ENDSCRIPT

tags_message = { font = "thps4.fnt" just = (0, 0) dims = (120, 70) colors = [ { (125, 14, 14) alpha = 230 }
		{ (125, 14, 14) alpha = 230 }
		{ (125, 14, 14) alpha = 230 }
		{ (125, 14, 14) alpha = 230 } ] key_points = [ { pos = (547, 286) alpha = 230 scale = 1.0 time = 0 }
		{ alpha = 230 scale = 0.8 time = 0.5 }
		{ alpha = 230 scale = 0.8 time = 9999999 } ] }
leaving_message = { font = "thps4.fnt" just = (0, 0) dims = (120, 70) colors = [ { (125, 14, 14) alpha = 230 }
		{ (125, 14, 14) alpha = 230 }
		{ (125, 14, 14) alpha = 230 }
		{ (125, 14, 14) alpha = 230 } ] key_points = [ { pos = (320, 70) alpha = 0 scale = 0.2 time = 0 }
		{ alpha = 230 scale = 0.95 time = 0.09 }
		{ alpha = 230 scale = 0.95 time = 1 }
		{ alpha = 0 scale = 0.2 time = 1.2 } ] }
thps4_message_props = { font = "thps4.fnt" just = (0, 0) dims = (80, 50) colors = [ { (118, 110, 14) alpha = 128 }
		{ (118, 110, 14) alpha = 128 }
		{ (118, 110, 14) alpha = 128 }
		{ (118, 110, 14) alpha = 128 } ] key_points = [ { pos = (320, 285) alpha = 0 scale = 0.2 time = 0 }
		{ alpha = 128 scale = 0.75 time = 0.09 }
		{ alpha = 128 scale = 0.75 time = 1 }
		{ alpha = 0 scale = 0.2 time = 1.2 } ] }
panel_message_death = { font = "small.fnt" just = (0, 0) dims = (320, 80) colors = [ { (110, 20, 20) alpha = 255 }
		{ (110, 20, 20) alpha = 255 } ] key_points = [ { pos = (330, 110) alpha = 0 scale = 4 time = 0 }
		{ scale = 1.2 alpha = 90 time = 0.4 }
		{ time = 2 }
		{ scale = 0 alpha = 0 time = 2.5 } ] }

SCRIPT TeleportSkaterUp dist = 2000
	SpawnSkaterScript MoveSkaterZ params = { dist = <dist> }
ENDSCRIPT

SCRIPT TeleportSkaterDown dist = -2000
	SpawnSkaterScript MoveSkaterZ params = { dist = <dist> }
ENDSCRIPT

SCRIPT MoveSkaterZ dist = 0
	IF ProfileEquals is_named = demoness
		BloodParticlesOff bone = "left_wrist"
		BloodParticlesOff bone = "right_wrist"
		printf "Particles turning off..........................................."
	ENDIF
	SparksOff
	Move y = <dist>
ENDSCRIPT

FLAG_TRAFFICLIGHT_GREEN = 0
FLAG_TRAFFICLIGHT_YELLOW = 1
FLAG_TRAFFICLIGHT_RED = 2
FLAG_CAR_GENERIC_STOPPED = 3
FLAG_CAR_GENERIC_NOSOUND = 4
FLAG_CAR_GENERIC_TURNING = 5
FLAG_CAR_SOUND_FRANTICSTOP = 6
FLAG_CAR_TYPE_BUS = 7

SCRIPT SK3_TrafficLight01
	Obj_SetFlag FLAG_TRAFFICLIGHT_GREEN
	create Name = <Green>
	Wait RANDOMRANGE(5000, 12000)
	BEGIN
		Obj_SetFlag FLAG_TRAFFICLIGHT_YELLOW
		Obj_ClearFlag FLAG_TRAFFICLIGHT_GREEN
		kill Name = <Green>
		create Name = <Yellow>
		Wait 3000
		Obj_SetFlag FLAG_TRAFFICLIGHT_RED
		Obj_ClearFlag FLAG_TRAFFICLIGHT_YELLOW
		kill Name = <Yellow>
		create Name = <Red>
		Wait RANDOMRANGE(9000, 15000)
		Obj_SetFlag FLAG_TRAFFICLIGHT_GREEN
		Obj_ClearFlag FLAG_TRAFFICLIGHT_RED
		kill Name = <Red>
		create Name = <Green>
		Wait RANDOMRANGE(7000, 12000)
	REPEAT
ENDSCRIPT

SCRIPT Car_Generic01 TurnDist = 20
	Obj_StickToGround off
	Obj_SetGroundOffset 0.1
	Obj_SetPathVelocity 45 mph
	Obj_SetPathTurnDist <TurnDist>
	Obj_FollowPathLinked
	Obj_SetPathAcceleration 30 mphps
	Obj_SetPathDeceleration 20 mphps
	Obj_SetPathMinStopVel 5
	Obj_SpawnScript Car_CheckForOtherCar01 params = { PathNum = <PathNum> }
	IF gotparam NoSound
		Obj_SetFlag FLAG_CAR_GENERIC_NOSOUND
	ELSE
		Obj_PlaySound carloop
	ENDIF
	IF gotparam NoSkater
	ELSE
		Obj_SpawnScript Car_CheckForSkater01
	ENDIF
	IF gotparam Bus
		Obj_SetFlag FLAG_CAR_TYPE_BUS
	ENDIF
ENDSCRIPT

SCRIPT Car_CheckForSkater01
	BEGIN
		Wait 10 gameframes
		IF Obj_FlagNotSet FLAG_CAR_GENERIC_STOPPED
			IF Obj_ObjectInRect dist = 70 width = 20 height = 40 type = [ skater ] offset = (0, 0, 0)
				Car_Sound_FranticStop01
				Obj_StopAlongPath 8
				BEGIN
					Wait 90 gameframes
					IF Obj_ObjectInRect dist = 70 width = 40 height = 20 type = [ skater ] offset = (0, 0, 0)
						Wait 60 gameframes
						Car_Sound_RandomHonk01
					ELSE
						Obj_StartAlongPath
						BREAK
					ENDIF
				REPEAT
			ENDIF
		ENDIF
	REPEAT
ENDSCRIPT

SCRIPT Car_CheckForOtherCar01
	BEGIN
		Wait 10 gameframes
		IF Obj_FlagNotSet FLAG_CAR_GENERIC_STOPPED
			IF Obj_FlagSet FLAG_CAR_TYPE_BUS
				IF Obj_ObjectInRect dist = 100 width = 55 height = 20 prefix = <PathNum> offset = (0, 0, 0)
					Obj_SpawnScript Car_DecelForOtherCar01
					IF Obj_ObjectInRect dist = 80 width = 55 height = 20 prefix = <PathNum> offset = (0, 0, 0)
						Car_StopForBus01 PathNum = <PathNum>
					ENDIF
				ENDIF
			ELSE
				IF Obj_ObjectInRect dist = 70 width = 50 height = 20 prefix = <PathNum> offset = (0, 0, 0)
					Obj_SpawnScript Car_DecelForOtherCar01
					IF Obj_ObjectInRect dist = 50 width = 50 height = 20 prefix = <PathNum> offset = (0, 0, 0)
						Car_StopForOtherCar01 PathNum = <PathNum>
					ENDIF
				ENDIF
			ENDIF
		ENDIF
	REPEAT
ENDSCRIPT

SCRIPT Car_CheckWhenTurning01
	BEGIN
		IF Obj_FlagNotSet FLAG_CAR_GENERIC_STOPPED
			IF Obj_ObjectInRect dist = 50 width = 50 height = 40 type = [ skater ] offset = (0, 0, 0)
				Car_Sound_FranticStop01
				Obj_StopAlongPath 4
				BEGIN
					Wait 90 gameframes
					IF Obj_ObjectInRect dist = 50 width = 50 height = 40 type = [ skater ] offset = (0, 0, 0)
						Wait 60 gameframes
						Car_Sound_RandomHonk01
					ELSE
						Obj_StartAlongPath
						BREAK
					ENDIF
				REPEAT
			ENDIF
			IF Obj_FlagNotSet FLAG_CAR_GENERIC_TURNING
				BREAK
			ENDIF
		ENDIF
		Wait 20 gameframes
	REPEAT
ENDSCRIPT

SCRIPT Car_Sound_Accel01
	IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
		Obj_AdjustSound carloop volumePercent = 100 volumeStep = 0.7 pitchPercent = 100 pitchStep = 0.7
	ENDIF
ENDSCRIPT

SCRIPT Car_Sound_Decel01
	IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
		Obj_AdjustSound carloop volumePercent = 80 volumeStep = 2 pitchPercent = 50 pitchStep = 1
		Obj_PlaySound Car_BrakeSqueal vol = RANDOMRANGE(5, 20) pitch = RANDOMRANGE(60, 80)
	ENDIF
ENDSCRIPT

SCRIPT Car_Sound_Stop01
	IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
		Obj_AdjustSound carloop volumePercent = 50 volumeStep = 1.5 pitchPercent = 30 pitchStep = 1.5
		Obj_PlaySound Car_BrakeSqueal vol = RANDOMRANGE(50, 70) pitch = 80
	ENDIF
ENDSCRIPT

SCRIPT Car_Sound_FranticStop01
	IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
		IF Obj_FlagNotSet FLAG_CAR_SOUND_FRANTICSTOP
			printf "frantic stop"
			Obj_PlaySound LA_Skid vol = RANDOMRANGE(30, 60)
			random( @@@
			Obj_PlaySound carhorn vol = RANDOMRANGE(30, 70) pitch = RANDOMRANGE(80, 100)) 
		ENDIF
	ENDIF
	Obj_SpawnScript Car_Counter_FranticStop01
ENDSCRIPT

SCRIPT Car_Counter_FranticStop01
	Obj_SetFlag FLAG_CAR_SOUND_FRANTICSTOP
	Wait 120 gameframes
	Obj_ClearFlag FLAG_CAR_SOUND_FRANTICSTOP
ENDSCRIPT

SCRIPT Car_Sound_RandomHonk01
	IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
		random( @@@@@@@@@@@@@@@@@@@@@@@
		@Obj_PlaySound carhorn vol = RANDOMRANGE(30, 70) pitch = RANDOMRANGE(80, 100)) 
	ENDIF
ENDSCRIPT

SCRIPT Car_Accel01
	Obj_SetPathVelocity RANDOMRANGE(40, 55) mph
	Car_Sound_Accel01
	Wait 60 gameframes
	Obj_ClearFlag FLAG_CAR_GENERIC_TURNING
ENDSCRIPT

SCRIPT Car_AccelAfterDecelForCar01
	Obj_SetPathVelocity RANDOMRANGE(40, 55) mph
	Car_Sound_Accel01
ENDSCRIPT

SCRIPT Car_Decel01
	Obj_SetPathVelocity RANDOMRANGE(25, 30) mph
	Car_Sound_Decel01
ENDSCRIPT

SCRIPT Car_DecelForTurn01
	Obj_SetPathVelocity RANDOMRANGE(25, 30) mph
	Car_Sound_Decel01
	Obj_SetFlag FLAG_CAR_GENERIC_TURNING
	Obj_SpawnScript Car_CheckWhenTurning01 params = { PathNum = <PathNum> }
ENDSCRIPT

SCRIPT Car_DecelTrafficLight01
	Obj_SetFlag FLAG_CAR_GENERIC_TURNING
	IF Obj_ObjectNotInRect dist = 50 type = [ skater car ]
		IF QueryFlag Name = <LightNode> FLAG_TRAFFICLIGHT_GREEN
			goto Car_DecelForTurn01 params = { PathNum = <PathNum> }
		ENDIF
		IF QueryFlag Name = <LightNode> FLAG_TRAFFICLIGHT_YELLOW
			random( 
			@goto Car_BlowYellow01 params = { PathNum = <PathNum> }
			@goto Car_BlowYellow01 params = { PathNum = <PathNum> }
			@goto Car_BlowYellow01 params = { PathNum = <PathNum> }
			@goto Car_Stop01 params = { LightNode = <LightNode> PathNum = <PathNum> }) 
		ENDIF
		IF QueryFlag Name = <LightNode> FLAG_TRAFFICLIGHT_RED
			goto Car_Stop01 params = { LightNode = <LightNode> PathNum = <PathNum> }
		ENDIF
	ELSE
	ENDIF
ENDSCRIPT

SCRIPT Car_DecelForOtherCar01
	Car_Decel01
	Wait 120 gameframes
	Car_AccelAfterDecelForCar01
ENDSCRIPT

SCRIPT Car_Start01
	Wait RANDOMRANGE(800, 1200)
	Obj_ClearFlag FLAG_CAR_GENERIC_STOPPED
	Obj_StartAlongPath
	Car_Sound_Accel01
ENDSCRIPT

SCRIPT Car_StartForOtherCar01
	Obj_ClearFlag FLAG_CAR_GENERIC_STOPPED
	Obj_SetPathVelocity 30 mph
	Obj_StartAlongPath
	Car_Sound_Accel01
	IF Obj_FlagSet FLAG_CAR_GENERIC_TURNING
		Obj_SpawnScript Car_CheckWhenTurning01 params = { PathNum = <PathNum> }
	ENDIF
ENDSCRIPT

SCRIPT Car_Stop01
	Obj_StopAlongPath RANDOMRANGE(25, 30) feet
	Obj_SetFlag FLAG_CAR_GENERIC_STOPPED
	Car_Sound_Stop01
	BEGIN
		Wait 20 gameframes
		IF QueryFlag Name = <LightNode> FLAG_TRAFFICLIGHT_GREEN
			Car_Start01
			BREAK
		ENDIF
	REPEAT
	Obj_SpawnScript Car_CheckWhenTurning01 params = { PathNum = <PathNum> }
ENDSCRIPT

SCRIPT Car_StopForOtherCar01
	Obj_SetFlag FLAG_CAR_GENERIC_STOPPED
	Obj_StopAlongPath 7
	Car_Sound_Stop01
	Car_InchUpAfterStop
	BEGIN
		IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
			Car_Sound_RandomHonk01
		ENDIF
		IF Obj_ObjectNotInRect dist = 50 width = 50 height = 20 prefix = <PathNum> offset = (0, 0, 0)
			Car_StartForOtherCar01 PathNum = <PathNum>
			BREAK
		ENDIF
		Wait 60 gameframes
	REPEAT
ENDSCRIPT

SCRIPT Car_StopForBus01
	Obj_SetFlag FLAG_CAR_GENERIC_STOPPED
	Obj_StopAlongPath 7
	Car_Sound_Stop01
	BEGIN
		IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
			Car_Sound_RandomHonk01
		ENDIF
		IF Obj_ObjectNotInRect dist = 80 width = 55 height = 20 prefix = <PathNum> offset = (0, 0, 0)
			Car_StartForOtherCar01 PathNum = <PathNum>
			BREAK
		ENDIF
		Wait 60 gameframes
	REPEAT
ENDSCRIPT

SCRIPT Car_BlowYellow01
	Obj_SetPathVelocity 50 mph
	IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
		Obj_AdjustSound carloop volumePercent = 130 volumeStep = 4 pitchPercent = 130 pitchStep = 4
		Obj_PlaySound LA_Skid vol = 100
	ENDIF
	Obj_SpawnScript Car_CheckWhenTurning01 params = { PathNum = <PathNum> }
ENDSCRIPT

SCRIPT Car_InchUpAfterStop
	IF Obj_ObjectNotInRect dist = 35 width = 50 height = 40 type = [ car skater ] offset = (0, 0, 0)
		Obj_StartAlongPath
		Wait 1 gameframes
		Obj_StopAlongPath 20
		IF Obj_FlagNotSet FLAG_CAR_GENERIC_NOSOUND
			Obj_PlaySound Car_BrakeSqueal vol = 10 pitch = 80
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT Ped_Generic01
	Obj_FollowPathLinked
	Obj_RandomPathMode On
	Obj_SetPathAcceleration 10
	Obj_SetPathDeceleration 10
ENDSCRIPT

SCRIPT Ped_StopAtNode01
	Obj_SetAnimCycleMode off
	Obj_WaitAnimFinished
	Obj_PlayAnim Anim = WalkToIdle
	Wait 2 gameframes
	Obj_StopAlongPath
	Obj_WaitAnimFinished
ENDSCRIPT

SCRIPT Ped_BackOnPath01
	printf "backonpath"
	Obj_StartAlongPath
	Obj_FollowPathStored
ENDSCRIPT

SCRIPT Ped_WalkToNextNode01
	random( 
	@Obj_SetPathVelocity 4
	Obj_StartAlongPath
	Obj_CycleAnim Anim = IdleToWalk
	Obj_PlayAnim Anim = Walk1 cycle
	@Obj_SetPathVelocity 4
	Obj_StartAlongPath
	Obj_CycleAnim Anim = IdleToWalk
	Obj_PlayAnim Anim = Walk2 cycle) 
ENDSCRIPT

SCRIPT Ped_Crosswalk01
	Ped_StopAtNode01
	BEGIN
		IF QueryFlag Name = <LightNode> FLAG_TRAFFICLIGHT_YELLOW
			Obj_PlayAnim Anim = Idle cycle
			BEGIN
				IF QueryFlag Name = <LightNode> FLAG_TRAFFICLIGHT_RED
					Obj_SetAnimCycleMode off
					Obj_WaitAnimFinished
					Wait RANDOMRANGE(0, 300)
					goto Ped_WalkToNextNode01
				ENDIF
				Wait 30 gameframes
			REPEAT
		ELSE
			Ped_Idle01
		ENDIF
	REPEAT
ENDSCRIPT

SCRIPT Ped_RandomWaitAtNode01
	Ped_StopAtNode01
	Obj_CycleAnim Anim = Idle
	BEGIN
		random( @
		Ped_Idle01
		@goto Ped_WalkToNextNode01) 
	REPEAT
ENDSCRIPT

SCRIPT Ped_Idle01
	random( 
	@Obj_CycleAnim Anim = ScratchChin
	@Obj_CycleAnim Anim = LookAtWatch
	@Obj_CycleAnim Anim = LookLeftRight
	@Obj_CycleAnim Anim = LookRight
	@Obj_CycleAnim Anim = LookLeftDown
	@Obj_CycleAnim Anim = Talk
	@Obj_CycleAnim Anim = Talk2
	@Obj_CycleAnim Anim = Idle
	@Obj_CycleAnim Anim = Idle
	@Obj_CycleAnim Anim = Idle
	@Obj_CycleAnim Anim = Idle
	@Obj_CycleAnim Anim = IdleToIdle2
	Obj_CycleAnim Anim = Idle2
	Obj_CycleAnim Anim = Idle2ToIdle
	@Obj_CycleAnim Anim = IdleToIdle3
	Obj_CycleAnim Anim = Idle3
	Obj_CycleAnim Anim = Idle3ToIdle) 
ENDSCRIPT

SCRIPT Gap_Gen_Rail
	IF gotparam start
		StartGap GapID = <GapID> flags = [ CANCEL_GROUND CANCEL_MANUAL CANCEL_WALL CANCEL_RAIL ]
	ELSE
		IF gotparam end
			EndGap GapID = <GapID> text = <text> Score = <Score>
		ELSE
			printf "### NO GAP TYPE SET FOR ID %g. MUST BE START OR END ###" g = <GapID>
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Manual
	IF gotparam start
		StartGap GapID = <GapID> flags = [ PURE_MANUAL ]
	ELSE
		IF gotparam end
			EndGap GapID = <GapID> text = <text> Score = <Score>
		ELSE
			printf "### NO GAP TYPE SET FOR ID %g. MUST BE START OR END ###" g = <GapID>
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_PureAir
	IF gotparam start
		StartGap GapID = <GapID> flags = [ PURE_AIR ]
	ELSE
		IF gotparam end
			EndGap GapID = <GapID> text = <text> Score = <Score>
		ELSE
			printf "### NO GAP TYPE SET FOR ID %g. MUST BE START OR END ###" g = <GapID>
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_AcrossTheStreet
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Across the street" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Ledge2Bench
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Ledge 2 Bench" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Ledge2Ledge
	Gap_Gen_LedgeHop <...>
ENDSCRIPT

SCRIPT Gap_Gen_Bench2Bench
	Gap_Gen_BenchHop <...>
ENDSCRIPT

SCRIPT Gap_Gen_BenchHop
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Bench hop" Score = 25 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Bench2Ledge
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Bench 2 Ledge" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Ledge2Bench
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Ledge 2 Bench" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Bench2Rail
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Bench 2 Rail" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Rail2Bench
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Rail 2 Bench" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_End
	IF gotparam end
		EndGap GapID = <GapID> text = <text> Score = <Score>
	ELSE
		printf "### NO GAP TYPE SET FOR ID %g. MUST BE START OR END ###" g = <GapID>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_HighLip
	IF gotparam start
		StartGap GapID = <GapID> flags = [ REQUIRE_LIP CANCEL_GROUND CANCEL_WALL CANCEL_MANUAL CANCEL_RAIL ]
	ELSE
		Wait 1 frame
		Gap_Gen_End GapID = <GapID> text = "High Lip" Score = 100
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Ledge2Rail
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Ledge 2 Rail" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Ledge2Wire
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Ledge 2 Wire" Score = 50 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_LedgeHop
	IF gotparam start
		Gap_Gen_Rail <...>
	ELSE
		EndGap GapID = <GapID> text = "Ledge Hop" Score = 25 <...>
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Lip
	IF gotparam start
		StartGap GapID = <GapID> flags = [ REQUIRE_LIP CANCEL_GROUND CANCEL_WALL CANCEL_MANUAL CANCEL_RAIL ]
	ELSE
		IF gotparam end
			Wait 1 frame
			Gap_Gen_End GapID = <GapID> text = <text> Score = <Score>
		ELSE
			printf "### NO GAP TYPE SET FOR ID %g. MUST BE START OR END ###" g = <GapID>
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT Gap_Gen_Print
	printf "fuck = %g" g = <GapID>
ENDSCRIPT
