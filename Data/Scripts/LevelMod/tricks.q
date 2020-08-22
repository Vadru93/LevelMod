
SCRIPT SkaterInit
	NotInBail
	NotifyBailDone
	SetSkaterCamLerpReductionTimer time = 0
	ClearLipCombos
	AllowRailTricks
	ResetLandedFromVert
	BlendperiodOut 0
	Obj_Attachframe bone = "head"
	SetException Ex = RunHasEnded Scr = EndOfRun
	StopBalanceTrick
	SwitchOffAtomic special_item
	IF InNetGame
	ELSE
		ClearSkaterCamOverride
	ENDIF
	ClearSkaterFlags
	printf "Clearing skaters flags =============="
	SetExceptionThatPreservesBalanceTrick MadeOtherSkaterBail
	IF ProfileEquals is_named = demoness
		Obj_Attachframe bone = "left_wrist"
		Obj_Attachframe bone = "right_wrist"
		//calling fire spawn defined later
		Demoness_FlameOn											  
	ENDIF
	PlayAnim Anim = Idle
	SetBalanceTrickParams ManualParams
	goto OnGroundAI
ENDSCRIPT

SCRIPT ClientSkaterInit
	SwitchOffAtomic special_item
ENDSCRIPT

SCRIPT SkaterRetryScript
	SkaterInit
ENDSCRIPT

SCRIPT ClearSkaterFlags
	Obj_ClearFlag FLAG_SKATER_KILLING
	Obj_ClearFlag FLAG_SKATER_REVERTFS
	Obj_ClearFlag FLAG_SKATER_REVERTBS
	Obj_ClearFlag FLAG_SKATER_INLOOP
	Obj_ClearFlag FLAG_SKATER_LIPCOMBO
	Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
ENDSCRIPT

demoness_shared_particle_params = {
	name = "breath.png" 
	//these are rgba color values
	start_col = 1426128895
	end_col = 153 
	blend_mode = 72 
	num = 40 
	emit_w = 1.0 
	emit_h = 1.0 
	angle = 10 
	size = 20.0 
	growth = 0 
	time = 10000 
	speed = 50 
	grav = 0 
	life = 0.5
}

SCRIPT Demoness_FlameOn
	BloodParticlesOn { demoness_shared_particle_params bone = "left_wrist" }
	BloodParticlesOn { demoness_shared_particle_params bone = "right_wrist" }
ENDSCRIPT

Exceptions = [
	RunHasEnded
	SkaterCollideBail
	MadeOtherSkaterBail
	Ollied
	GroundGone
	Landed
	OntoRail
	OffRail
	FlailHitWall
	FlailLeft
	FlailRight
	OffMeterTop
	OffMeterBottom
	WallRideLeft
	WallRideRight
	WallRideBail
	CarBail
	CarPlant
	WonGame
	LostGame
	//Levelmod added exceptions
	AcidDrop
	SpineTransfer
	BankDrop
	Wallplant
]

SCRIPT OnGroundExceptions
	IF GotParam NoClear
	ELSE
		ClearExceptions
	ENDIF
	IF GotParam NoEndRun
	ELSE
		SetException Ex = RunHasEnded Scr = EndOfRun
	ENDIF
	SetException Ex = GroundGone Scr = GroundGone
	SetException Ex = Ollied Scr = Ollie
	SetException Ex = FlailHitWall Scr = FlailHitWall
	SetException Ex = FlailLeft Scr = FlailLeft
	SetException Ex = FlailRight Scr = FlailRight
	SetException Ex = CarPlant Scr = CarPlant
	SetException Ex = CarBail Scr = CarBail
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	SetException Ex = MadeOtherSkaterBail Scr = MadeOtherSkaterBail
	SetQueueTricks Jumptricks GroundTricks
	SetExtraGrindTricks special = SpecialGrindTricks GrindTricks
	SetManualTricks special = SpecialManualTricks GroundManualTricks
	VibrateOff
	SwitchOnBoard
	EnablePlayerInput
	BailOff
	BashOff
	SetRollingFriction #"default"
	CanSpin
	AllowRailTricks
	SetSkaterCamLerpReductionTimer time = 0
	Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
	IF GotParam NoEndRun
	ELSE
		ResetLandedFromVert
	ENDIF
ENDSCRIPT

SCRIPT OnGroundExceptions_NoClear
	OnGroundExceptions NoClear
ENDSCRIPT

SCRIPT OnGroundExceptions_NoEndRun
	OnGroundExceptions NoEndRun
ENDSCRIPT

SCRIPT InAirExceptions
	ClearExceptions
	SetException Ex = Landed Scr = Land
	SetException Ex = WallRideLeft Scr = WallRide Params = { Left }
	SetException Ex = WallRideRight Scr = WallRide Params = { Right }
	SetException Ex = AcidDrop Scr = AcidDrop
	SetException Ex = SpineTransfer Scr = SpineTransfer
	SetException Ex = BankDrop Scr = BankDrop
	SetException Ex = WallRideBail Scr = WallRideBail
	SetException Ex = CarPlant Scr = CarPlant CallInsteadofGoto
	SetException Ex = Wallplant Scr = Wallplant
	SetException Ex = CarBail Scr = CarBail
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	SetException Ex = MadeOtherSkaterBail Scr = MadeOtherSkaterBailAir CallInsteadofGoto
	SetQueueTricks special = SpecialTricks AirTricks
	SetExtraGrindTricks special = SpecialGrindTricks GrindTricks
	SetManualTricks special = SpecialManualTricks ManualTricks
	VibrateOff
	EnablePlayerInput
	BailOff
	BashOff
	SetRollingFriction #"default"
	SetState Air
	CanSpin
ENDSCRIPT

SCRIPT ClearTrickQueues
	ClearTrickQueue
	ClearManualTrick
	ClearExtraGrindTrick
ENDSCRIPT

//new function
SCRIPT AcidDrop
	SetTrickName 'Acid Drop'
	SetTrickScore 250
	Display NoDegrade
	IF #"Not" DoingTrick
		SetState Air
		InAirExceptions
	ENDIF
	begin
		if Released Circle
			break
		endif
		WaitOneGameFrame
	repeat

    Bailoff
	DoNextTrick
	Wait 2 frames
	KillExtraTricks
	WaitAnimWhilstChecking
	goto Airborne
ENDSCRIPT

//new function
SCRIPT SpineTransfer
	SetTrickName 'Spine Transfer'
	SetTrickScore 250
	Display NoDegrade
	IF #"Not" DoingTrick
		SetState Air
		InAirExceptions
	ENDIF
	begin
		if Released Circle
			break
		endif
		WaitOneGameFrame
	repeat

    Bailoff
	DoNextTrick
	Wait 2 frames
	KillExtraTricks
	WaitAnimWhilstChecking
	goto Airborne
ENDSCRIPT

//new function
SCRIPT ExitQP
	TurnToFaceVelocity
	goto Airborne
ENDSCRIPT

//new function
SCRIPT BankDrop
	SetTrickName 'Bank Drop'
	SetTrickScore 250
	Display NoDegrade
	IF #"Not" DoingTrick
		SetState Air
		InAirExceptions
	ENDIF
	wait 2 frames
	KillExtraTricks
	WaitAnimWhilstChecking
	goto Airborne
ENDSCRIPT

SCRIPT WallplantOllie
    IF WallplantTimeGreaterThan 350
	    ClearException Ollied
    ELSE
	    Ollie
		ClearException Ollied
	ENDIF
ENDSCRIPT

WallplantOllie2 =
[
  { Trigger = { Tap , x , 200 } Scr = Ollie Params = { JumpSpeed = 200 } }
]

WALLPLANT_WINDOW = 450
Wallplant_Trick =
[
  { InOrder , x , Down , WALLPLANT_WINDOW }
  { InOrder , x , DownLeft , WALLPLANT_WINDOW }
  { InOrder , x , DownRight , WALLPLANT_WINDOW }
  { InOrder , Down , x , WALLPLANT_WINDOW }
  { InOrder , DownLeft , x , WALLPLANT_WINDOW }
  { InOrder , DownRight , x , WALLPLANT_WINDOW }
]

SCRIPT Wallplant

  IF BailIsOn
    SetState Air
  ENDIF
  NollieOff
  InAirExceptions
  ClearException Ollied
  NoSpin
  Vibrate Actuator = 1 Percent = 100 Duration = 0.1
  PlayAnim Anim = Beanplant BlendPeriod = 0
  SetTrickName 'Wallplant'
  SetTrickScore 750
  Display BlockSpin
  NoSpin
  begin
    IF WallplantTimeGreaterThan 180
	    ClearEventBuffer Buttons = [ x ]
	    SetException ex = Ollied scr = WallplantOllie CallInsteadofGoto
		//SetQueueTricks Wallplantollie2
		printf "Allowing ollie"
		CanSpin
		break
	ENDIF
	DoNextTrick
	WaitOneGameFrame
  repeat
  
  begin
    IF WallplantTimeGreaterThan 400
		printf "Disallowing ollie"
		break
	ENDIF
	DoNextTrick
	WaitOneGameFrame
  repeat
  ClearTrickQueue
  WaitAnimWhilstChecking
  goto Airborne
ENDSCRIPT

SCRIPT WallRide
	ClearExceptions
	SetException Ex = Landed Scr = Land
	SetException Ex = Ollied Scr = Wallie
	SetException Ex = GroundGone Scr = WallrideEnd
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	Vibrate Actuator = 1 Percent = 40
	IF GotParam Left
		move x = 36
	ELSE
		move x = -36
	ENDIF
	
	//added extra tricks for wallieplant
	IF IsOptionOn LM_Control_bWalliePlant
		SetQueueTricks WallrideTricks
	ENDIF
	
	NollieOff
	SetTrickScore 200
	PlayCessSound
	IF GotParam Left
		IF Flipped
			SetTrickName 'BS Wallride'
			PlayAnim Anim = WallRideBackTrans BlendPeriod = 0.0
			WaitAnimFinished
			PlayAnim Anim = WallRideBackLoop BlendPeriod = 0.1 Cycle
		ELSE
			SetTrickName 'FS Wallride'
			PlayAnim Anim = WallRideFrontTrans BlendPeriod = 0.0
			WaitAnimFinished
			PlayAnim Anim = WallRideFrontLoop BlendPeriod = 0.1 Cycle
		ENDIF
	ELSE
		IF Flipped
			SetTrickName 'FS Wallride'
			PlayAnim Anim = WallRideFrontTrans BlendPeriod = 0.0
			WaitAnimFinished
			PlayAnim Anim = WallRideFrontLoop BlendPeriod = 0.1 Cycle
		ELSE
			SetTrickName 'BS Wallride'
			PlayAnim Anim = WallRideBackTrans BlendPeriod = 0.0
			WaitAnimFinished
			PlayAnim Anim = WallRideBackLoop BlendPeriod = 0.1 Cycle
		ENDIF
	ENDIF
	Display
ENDSCRIPT

//Added WalliePlant
WallrideTricks = [ { Trigger = { TapTwiceRelease Up X 500 } Scr = Trick_WallPlant } ]

SCRIPT Trick_WallPlant
	InAirExceptions
	Vibrate Actuator = 1 Percent = 50 Duration = 0.1
	PlayAnim Anim = Boneless BlendPeriod = 0.0
	SetTrickName "Wallieplant"
	SetTrickScore 500
	Display
	#"Jump" BonelessHeight
	WaitAnimWhilstChecking
	goto Airborne StretchTime = 10 BlendPeriod = 0
ENDSCRIPT

SCRIPT WallrideEnd
	BlendperiodOut 0
	ClearExceptions
	InAirExceptions
	SetState Air
	goto Airborne StretchTime = 10 BlendPeriod = 0
ENDSCRIPT

SCRIPT Wallie
	InAirExceptions
	
	//Added option here since this fix removed the ability to get boost plant
	//Boost plant is a bug that exists in thps4+ that allows you to do multiply wallieplants
	//If you press the wallieplant multiply times in air before you hit the wallride
	IF #"Not" IsOptionOn LM_Control_bBoostPlant
	    //this is needed here to skip to wallieplant if it's in chain. else it awards 2 tricks.
	    //can we let it be here with wallieplant disabled?
	    IF IsOptionOn LM_Control_bWalliePlant	
		    DoNextTrick
	    ENDIF
	ENDIF
	Vibrate Actuator = 1 Percent = 50 Duration = 0.1
	PlayAnim Anim = Ollie BlendPeriod = 0.0
	SetTrickName "Wallie"
	SetTrickScore 250
	Display
	#"Jump"
	WaitAnimWhilstChecking
	goto Airborne StretchTime = 10 BlendPeriod = 0
ENDSCRIPT

SCRIPT WallRideBail
ENDSCRIPT

SCRIPT OnGroundAI
	SetState Ground
	OnGroundExceptions
	SetRollingFriction #"default"
	EnablePlayerInput
	NollieOff
	BEGIN
		IF LeftPressed
			IF Crouched
				IF Flipped
					IF AnimEquals CrouchTurnRightIdle
					ELSE
						PlayAnim Anim = CrouchTurnRight NoRestart
						IF AnimFinished
							PlayAnim Anim = CrouchTurnRightIdle Cycle NoRestart
						ENDIF
					ENDIF
				ELSE
					IF AnimEquals CrouchTurnLeftIdle
					ELSE
						PlayAnim Anim = CrouchTurnLeft NoRestart
						IF AnimFinished
							PlayAnim Anim = CrouchTurnLeftIdle Cycle NoRestart
						ENDIF
					ENDIF
				ENDIF
			ELSE
				IF Flipped
					IF AnimEquals TurnRightIdle
					ELSE
						PlayAnim Anim = TurnRight NoRestart
						IF AnimFinished
							PlayAnim Anim = TurnRightIdle Cycle NoRestart
						ENDIF
					ENDIF
				ELSE
					IF AnimEquals TurnLeftIdle
					ELSE
						PlayAnim Anim = TurnLeft NoRestart
						IF AnimFinished
							PlayAnim Anim = TurnLeftIdle Cycle NoRestart
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ELSE
			IF RightPressed
				IF Crouched
					IF Flipped
						IF AnimEquals CrouchTurnLeftIdle
						ELSE
							PlayAnim Anim = CrouchTurnLeft NoRestart
							IF AnimFinished
								PlayAnim Anim = CrouchTurnLeftIdle Cycle NoRestart
							ENDIF
						ENDIF
					ELSE
						IF AnimEquals CrouchTurnRightIdle
						ELSE
							PlayAnim Anim = CrouchTurnRight NoRestart
							IF AnimFinished
								PlayAnim Anim = CrouchTurnRightIdle Cycle NoRestart
							ENDIF
						ENDIF
					ENDIF
				ELSE
					IF Flipped
						IF AnimEquals TurnLeftIdle
						ELSE
							PlayAnim Anim = TurnLeft NoRestart
							IF AnimFinished
								PlayAnim Anim = TurnLeftIdle Cycle NoRestart
							ENDIF
						ENDIF
					ELSE
						IF AnimEquals TurnRightIdle
						ELSE
							PlayAnim Anim = TurnRight NoRestart
							IF AnimFinished
								PlayAnim Anim = TurnRightIdle Cycle NoRestart
							ENDIF
						ENDIF
					ENDIF
				ENDIF
			ELSE
				IF Crouched
					PlayAnim Anim = CrouchIdle Cycle NoRestart
				ELSE
					IF Braking
						IF SpeedLessThan 1
							PlayAnim Anim = Idle NoRestart
							IF HeldLongerThan Button = down 2 second
								goto HandBrake
							ENDIF
						ELSE
							IF HeldLongerThan Button = down 0.2 second
								CessBrake
							ELSE
								PlayAnim Anim = Idle NoRestart
							ENDIF
						ENDIF
					ELSE
						IF CanKick
							IF AutoKickIsOff
								IF ShouldMongo
									PlayAnim Anim = MongoPush NoRestart
									WaitAnimFinished
									PlayAnim Anim = MongoPushCycle NoRestart
									WaitAnimFinished
									PlayAnim Anim = Idle Cycle NoRestart
								ELSE
									PlayAnim Anim = PushCycle NoRestart
									WaitAnimFinished
									PlayAnim Anim = Idle Cycle NoRestart
								ENDIF
							ELSE
								IF ShouldMongo
									IF AnimEquals MongoPushCycle
										PlayAnim Anim = MongoPushCycle Cycle NoRestart
									ELSE
										PlayAnim Anim = MongoPush NoRestart
										WaitAnimFinished
										PlayAnim Anim = MongoPushCycle Cycle NoRestart
									ENDIF
								ELSE
									PlayAnim Anim = PushCycle Cycle NoRestart
								ENDIF
							ENDIF
						ELSE
							IF AutoKickIsOff
								IF held square
									IF ShouldMongo
										PlayAnim Anim = MongoPushCycle Cycle NoRestart
									ELSE
										PlayAnim Anim = PushCycle Cycle NoRestart
									ENDIF
								ELSE
									IF AnimEquals [ PushCycle MongoPushCycle MongoPush ]
									ELSE
										PlayAnim Anim = Idle Cycle NoRestart
									ENDIF
								ENDIF
							ELSE
								IF AnimFinished
									PlayAnim Anim = Idle Cycle NoRestart
								ENDIF
							ENDIF
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ENDIF
		DoNextTrick
		DoNextManualTrick
		WaitOneGameFrame
		IF InNetGame
			CheckforNetBrake
		ENDIF
	REPEAT
ENDSCRIPT

SCRIPT CheckforNetBrake
	IF InNetGame
		IF MenuOnScreen
			goto NetBrake
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT CessBrake
	ClearEventBuffer
	IF SpeedGreaterThan 300
		PlayAnim Anim = CessSlide180_FS From = Start To = 28 speed = 1.2
		WaitAnim Frame 5
		PlayCessSound
		WaitAnim Frame 25
		PlayAnim Anim = CessSlide180_FS From = 27 To = Start speed = 1.2
		WaitAnimFinished
	ENDIF
	PlayAnim Anim = Idle Cycle NoRestart
ENDSCRIPT

SCRIPT NetBrake
	SetRollingFriction 20
	OnExceptionRun NetBrake_out
	CessBrake
	BEGIN
		IF SpeedLessThan 5
			IF ShouldMongo
				PlayAnim Anim = MongoBrakeIdle Cycle NoRestart
			ELSE
				PlayAnim Anim = BrakeIdle Cycle NoRestart
			ENDIF
		ENDIF
		IF MenuOnScreen
			WaitOneGameFrame
		ELSE
			BREAK
		ENDIF
	REPEAT
	SetRollingFriction #"default"
	IF InNollie
		goto OnGroundNollieAI
	ELSE
		goto OnGroundAI
	ENDIF
ENDSCRIPT

SCRIPT NetBrake_out
	SetRollingFriction #"default"
ENDSCRIPT

SCRIPT HandBrake
	ClearEventBuffer
	OnExceptionRun BrakeDone
	SetRollingFriction 100
	BEGIN
		IF AutoKickIsOff
			IF held square
				WaitOneGameFrame
				BREAK
			ENDIF
		ELSE
			IF held Up
				BREAK
			ENDIF
			IF Crouched
				BREAK
			ENDIF
		ENDIF
		IF RightPressed
			IF Flipped
				PlayAnim Anim = TurnLeft NoRestart
				IF AnimFinished
					PlayAnim Anim = TurnLeftIdle Cycle
				ENDIF
			ELSE
				PlayAnim Anim = TurnRight NoRestart
				IF AnimFinished
					PlayAnim Anim = TurnRightIdle Cycle
				ENDIF
			ENDIF
		ELSE
			IF LeftPressed
				IF Flipped
					PlayAnim Anim = TurnRight NoRestart
					IF AnimFinished
						PlayAnim Anim = TurnRightIdle Cycle
					ENDIF
				ELSE
					PlayAnim Anim = TurnLeft NoRestart
					IF AnimFinished
						PlayAnim Anim = TurnLeftIdle Cycle
					ENDIF
				ENDIF
			ELSE
				IF ShouldMongo
					PlayAnim Anim = MongoBrakeIdle Cycle NoRestart
				ELSE
					PlayAnim Anim = BrakeIdle Cycle NoRestart
				ENDIF
			ENDIF
		ENDIF
		WaitOneGameFrame
		DoNextTrick
		DoNextManualTrick
	REPEAT
	SetRollingFriction #"default"
	IF InNollie
		goto OnGroundNollieAI
	ELSE
		goto OnGroundAI
	ENDIF
ENDSCRIPT

SCRIPT BrakeDone
	SetRollingFriction #"default"
ENDSCRIPT

SCRIPT OnGroundNollieAI
	SetState Ground
	EnablePlayerInput
	ClearExceptions
	OnGroundExceptions
	SetException Ex = Ollied Scr = Nollie
	SetQueueTricks Jumptricks GroundTricks
	SetManualTricks special = SpecialManualTricks GroundManualTricks
	NollieOn
	BEGIN
		IF LeftPressed
			IF Crouched
				IF Flipped
					PlayAnim Anim = NollieCrouchTurnRight NoRestart
				ELSE
					PlayAnim Anim = NollieCrouchTurnLeft NoRestart
				ENDIF
			ELSE
				IF Flipped
					PlayAnim Anim = NollieSkatingTurnRight NoRestart
				ELSE
					PlayAnim Anim = NollieSkatingTurnLeft NoRestart
				ENDIF
			ENDIF
		ELSE
			IF RightPressed
				IF Crouched
					IF Flipped
						PlayAnim Anim = NollieCrouchTurnLeft NoRestart
					ELSE
						PlayAnim Anim = NollieCrouchTurnRight NoRestart
					ENDIF
				ELSE
					IF Flipped
						PlayAnim Anim = NollieSkatingTurnLeft NoRestart
					ELSE
						PlayAnim Anim = NollieSkatingTurnRight NoRestart
					ENDIF
				ENDIF
			ELSE
				IF Crouched
					PlayAnim Anim = NollieCrouchIdle Cycle NoRestart
				ELSE
					IF Braking
						IF SpeedLessThan 1
							PlayAnim Anim = NollieSkatingIdle NoRestart
							IF HeldLongerThan Button = down 2 second
								goto HandBrake
							ENDIF
						ELSE
							IF HeldLongerThan Button = down 0.2 second
								CessBrake
							ELSE
								PlayAnim Anim = Idle NoRestart
							ENDIF
						ENDIF
					ELSE
						IF CanKick
							IF AutoKickIsOff
								IF ShouldMongo
									IF AnimEquals MongoPushCycle
										PlayAnim Anim = MongoPushCycle NoRestart
									ELSE
										PlayAnim Anim = MongoPush NoRestart
										WaitAnimFinished
										PlayAnim Anim = MongoPushCycle NoRestart
									ENDIF
								ELSE
									PlayAnim Anim = PushCycle NoRestart
								ENDIF
							ELSE
								IF ShouldMongo
									IF AnimEquals MongoPushCycle
										PlayAnim Anim = MongoPushCycle Cycle NoRestart
									ELSE
										PlayAnim Anim = MongoPush NoRestart
										WaitAnimFinished
										PlayAnim Anim = MongoPushCycle Cycle NoRestart
									ENDIF
								ELSE
									PlayAnim Anim = PushCycle Cycle NoRestart
								ENDIF
							ENDIF
						ELSE
							IF AnimFinished
								PlayAnim Anim = NollieSkatingIdle Cycle NoRestart
							ENDIF
						ENDIF
					ENDIF
				ENDIF
			ENDIF
		ENDIF
		DoNextTrick
		DoNextManualTrick
		CheckforNetBrake
		WaitOneGameFrame
	REPEAT
ENDSCRIPT

SCRIPT FlailExceptions
	NollieOff
	SetException Ex = CarBail Scr = CarBail
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	SetException Ex = RunHasEnded Scr = EndOfRun
	ClearTrickQueue
	ClearManualTrick
	ClearEventBuffer
	CheckGapTricks
	ClearGapTricks
	ClearPanel_Landed
ENDSCRIPT

SCRIPT FlailHitWall
	FlailExceptions
	FlailVibrate
	PlayAnim Anim = FlailLeft
	WaitAnimFinished
	goto OnGroundAI
ENDSCRIPT

SCRIPT FlailLeft
	FlailExceptions
	FlailVibrate
	PlayAnim Anim = FlailLeft
	WaitAnimFinished
	goto OnGroundAI
ENDSCRIPT

SCRIPT FlailRight
	FlailExceptions
	FlailVibrate
	PlayAnim Anim = FlailRight
	WaitAnimFinished
	goto OnGroundAI
ENDSCRIPT

SCRIPT FlailVibrate
	IF SpeedGreaterThan 600
		Vibrate Actuator = 1 Percent = 80 Duration = 0.25
	ELSE
		Vibrate Actuator = 1 Percent = 50 Duration = 0.15
	ENDIF
ENDSCRIPT

SCRIPT GroundGone
	InAirExceptions
	SetException Ex = Ollied Scr = Ollie
	ClearTricksFrom GroundTricks
	IF GotParam NoBoneless
		SetQueueTricks special = SpecialTricks AirTricks
	ELSE
		SetQueueTricks special = SpecialTricks AirTricks Jumptricks JumpTricks0
	ENDIF
	IF Crouched
		PlayAnim Anim = Crouch2InAir
	ELSE
		PlayAnim Anim = Idle2InAir
	ENDIF
	BEGIN
		IF AirTimeGreaterThan Skater_Late_Jump_Slop
			ClearException Ollied
			SetQueueTricks special = SpecialTricks AirTricks
		ENDIF
		DoNextTrick
		IF GotParam AndManuals
			DoNextManualTrick
		ENDIF
		IF AnimFinished
			BREAK
		ENDIF
		WaitOneGameFrame
	REPEAT
	goto Airborne
ENDSCRIPT

AirAnimParams = { Hold BlendPeriod = 0.3 NoRestart }

SCRIPT Airborne StretchTime = 1 BlendPeriod = 0.3
	InAirExceptions
	BEGIN
		DoNextTrick
		IF LeftPressed
			IF Flipped
				PlayAnim Anim = AirTurnRight AirAnimParams
			ELSE
				PlayAnim Anim = AirTurnLeft AirAnimParams
			ENDIF
		ELSE
			IF RightPressed
				IF Flipped
					PlayAnim Anim = AirTurnLeft AirAnimParams
				ELSE
					PlayAnim Anim = AirTurnRight AirAnimParams
				ENDIF
			ELSE
				IF AirTimeGreaterThan <StretchTime> seconds
					PlayAnim Anim = Stretch BlendPeriod = <BlendPeriod> NoRestart
					BREAK
				ELSE
					PlayAnim Anim = AirIdle Cycle BlendPeriod = <BlendPeriod> NoRestart
				ENDIF
			ENDIF
		ENDIF
		WaitOneGameFrame
	REPEAT
	WaitAnimWhilstChecking
	PlayAnim Anim = StretchIdle BlendPeriod = 0.1 Cycle
ENDSCRIPT

SCRIPT Land
	SetSkaterCamLerpReductionTimer time = 0
	IF ProfileEquals is_named = demoness
		Demoness_FlameOn
	ENDIF
	IF AbsolutePitchGreaterThan 60
		IF PitchGreaterThan 60
			goto PitchBail
		ENDIF
	ENDIF
	IF SpeedGreaterThan 500
		IF YawBetween (60, 120)
			IF InSplitScreenGame
			ELSE
				IF IsOptionOn LM_GUI_bTrickNotifications
					LaunchPanelMessage "&C1Landed sideways" properties = panelcombo
				ENDIF
			ENDIF
			goto YawBail
		ENDIF
	ENDIF
	IF RollGreaterThan 50
		goto DoingTrickBail
	ENDIF
	IF DoingTrick
		goto DoingTrickBail
	ENDIF
	IF GotParam ReturnBacktoLipLand
	ELSE
		IF GotParam IgnoreAirTime
			goto Land2
		ELSE
			IF AirTimeLessThan 0.2 seconds
				goto Land2 Params = { LittleAir }
			ELSE
				GotoPreserveParams Land2
			ENDIF
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT Land2 RevertTime = 5
	AllowRailTricks
	NollieOff
	ClearLipCombos
	IF LandedFromVert
		Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
		IF IsOptionOn LM_Control_bRevert
			SetExtraTricks tricks = Reverts Duration = <RevertTime>
		ENDIF
	ELSE
		DoNextManualTrick FromAir
	ENDIF
	Vibrate Actuator = 1 Percent = 80 Duration = 0.1
	IF Crouched
		IF GotParam LittleAir
			PlayAnim Anim = CrouchBump
		ELSE
			IF Backwards
				FlipAndRotate
				PlayAnim Anim = CrouchedLandTurn BlendPeriod = 0
				BoardRotate
			ELSE
				IF YawBetween (45, 60)
					IF AirTimeGreaterThan 0.75 second
						PlayAnim Anim = SketchyCrouchLand BlendPeriod = 0.1
						IF InSplitScreenGame
						ELSE
							IF IsOptionOn LM_GUI_bTrickNotifications
								LaunchPanelMessage "&C1Sketchy" properties = panelcombo
							ENDIF
						ENDIF
					ELSE
						PlayAnim Anim = CrouchedLand BlendPeriod = 0.1
					ENDIF
				ELSE
					PlayAnim Anim = CrouchedLand BlendPeriod = 0.1
				ENDIF
			ENDIF
		ENDIF
	ELSE
		IF GotParam LittleAir
			PlayAnim Anim = IdleBump
		ELSE
			IF Backwards
				FlipAndRotate
				PlayAnim Anim = LandTurn BlendPeriod = 0
				BoardRotate
			ELSE
				IF YawBetween (45, 60)
					IF AirTimeGreaterThan 0.75 second
						PlayAnim Anim = SketchyLand BlendPeriod = 0.1
						IF InSplitScreenGame
						ELSE
							IF IsOptionOn LM_GUI_bTrickNotifications
								LaunchPanelMessage "&C1Sketchy" properties = panelcombo
							ENDIF
						ENDIF
					ELSE
						PlayAnim Anim = Land BlendPeriod = 0.1
					ENDIF
				ELSE
					PlayAnim Anim = Land BlendPeriod = 0.1
				ENDIF
			ENDIF
		ENDIF
	ENDIF
	ClearTrickQueue
	ClearEventBuffer buttons = [ x ]
	OnGroundExceptions_NoEndRun
	OnExceptionRun Landout
	SetManualTricks special = SpecialManualTricks ManualTricks
	IF GotParam NoReverts
	ELSE
		IF LandedFromVert
			ResetLandedFromVert
			BEGIN
				wait 1
			REPEAT <RevertTime>
		ELSE
			BEGIN
				DoNextManualTrick FromAir
				wait 1
			REPEAT 10
		ENDIF
	ENDIF
	CheckGapTricks
	ClearGapTricks
	ClearPanel_Landed
	ResetSpin
	ClearManualTrick
	OnGroundExceptions
	CheckforNetBrake
	WaitAnimWhilstChecking AndManuals
	goto OnGroundAI
ENDSCRIPT

SCRIPT Landout
	IF ExceptionTriggered RunHasEnded
		SetSpeed 0
		SetState Ground
		ClearExceptions
		SetException Ex = RunHasEnded Scr = EndOfRun
	ELSE
		OnGroundExceptions_NoClear
	ENDIF
	CheckGapTricks
	ClearGapTricks
	ClearPanel_Landed
	ClearManualTrick
ENDSCRIPT

SCRIPT WaitOnGround
	BEGIN
		IF OnGround
			BREAK
		ENDIF
		WaitOneGameFrame
	REPEAT
ENDSCRIPT

SCRIPT VibrateOff
	Vibrate Actuator = 0 Percent = 0
	Vibrate Actuator = 1 Percent = 0
ENDSCRIPT

SCRIPT EndOfRun
	SetState Ground
	CleanUpSpecialItems
	StopBalanceTrick
	CheckGapTricks
	ClearGapTricks
	ClearPanel_Landed
	ClearExceptions
	ClearTrickQueues
	ClearEventBuffer
	SetException Ex = LostGame Scr = LostGame
	DisablePlayerInput AllowCameraControl
	SetException Ex = SkaterCollideBail Scr = EndBail
	WaitOnGround
	SetRollingFriction 19
	wait 10 frames
	WaitOnGround
	IF SpeedGreaterThan 400
		PlayCessSound
		PlayAnim Anim = CessSlide180_FS
		WaitAnim 50 Percent
		PlayAnim Anim = CessSlide180_FS From = Current To = 0
		WaitAnimFinished
	ELSE
		PlayAnim Anim = brake
		WaitAnimFinished
	ENDIF
	PlayAnim Anim = BrakeIdle BlendPeriod = 0.3 Cycle
	BEGIN
		IF SpeedLessThan 40
			IF OnGround
				BREAK
			ENDIF
		ENDIF
		WaitOneGameFrame
	REPEAT
	IF InNetGame
		SetException Ex = WonGame Scr = WonGame
		SetException Ex = LostGame Scr = LostGame
	ENDIF
	wait 1 seconds
	IF IsCareerMode
		//changed tilt to negative
		SetSkaterCamOverride heading = 3 tilt = 0.2 time = 2
	ENDIF
	EndOfRunDone
	SetException Ex = LostGame Scr = LostGame
	wait 1 seconds
	IF InNetGame
		IF GameIsOver
		ELSE
			EnterObserveMode2
			IF GameModeEquals is_king
			ELSE
				LaunchPanelMessage "Waiting for other players to finish their combos..." properties = netstatusprops
			ENDIF
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT ForceEndOfRun
	VibrateOff
	MakeSkaterGoto EndOfRun
ENDSCRIPT

SCRIPT SkaterCollide_AtEndRun
ENDSCRIPT

SCRIPT WonGame
	PlayAnim random( @Anim = Prop @Anim = Cheer1 ) BlendPeriod = 0.3
	EndOfRunDone
	WaitAnimFinished
	IF ShouldMongo
		PlayAnim Anim = MongoBrakeIdle Cycle
	ELSE
		PlayAnim Anim = BrakeIdle Cycle
	ENDIF
ENDSCRIPT

SCRIPT LostGame
	PlayAnim Anim = BrakeDefeat
	WaitAnimFinished
	EndOfRunDone
	IF ShouldMongo
		PlayAnim Anim = MongoBrakeIdle Cycle
	ELSE
		PlayAnim Anim = BrakeIdle Cycle
	ENDIF
	WaitAnimFinished
ENDSCRIPT

SCRIPT EndBail
	EndOfRunDone
	ClearExceptions
	InBail
	TurnToFaceVelocity
	VibrateOff
	PlaySound random( @BoardBail01 @BoardBail02 ) 
	PlayAnim Anim = SlipForwards NoRestart BlendPeriod = 0.3
	wait 10 frames
	SwitchOffBoard
	wait 10 Frame
	PlaySound random( @HitBody03 @HitBody04 ) 
	wait 10 frames
	SetRollingFriction 18
	Vibrate Actuator = 1 Percent = 100 Duration = 0.2
	Obj_Spawnscript BloodSmall
	WaitAnim 25 Percent fromend
	PlaySound FoleyMove01 vol = 50
	WaitAnimFinished
	Obj_Spawnscript BloodPool
	PlayAnim Anim = GetUpForwards BlendPeriod = 0.1
	SetRollingFriction 20
	wait 50 frames
	SwitchOnBoard
	BoardRotate normal
	WaitAnimFinished
	NotifyBailDone
	NotInBail
	goto EndOfRun
ENDSCRIPT

SCRIPT EndOfRunTimeOut
	EndOfRunDone
ENDSCRIPT

SCRIPT CarPlant
	ClearExceptions
	InAirExceptions
	ClearException CarPlant
	PlaySound GrindMetalOn pitch = random( @80 @90 @85 ) 
	Obj_Spawnscript CarSparks
	DoCarPlantBoost
	SetTrickName "Car Plant"
	SetTrickScore 400
	Display
	IF DoingTrick
	ELSE
		PlayAnim Anim = Beanplant BlendPeriod = 0.3
		WaitAnimWhilstChecking
		goto Airborne
	ENDIF
ENDSCRIPT

SCRIPT CarSparks
	SparksOn
	wait 20 gameframes
	SetException Ex = CarPlant Scr = CarPlant
	wait 180 gameframes
	SparksOff
ENDSCRIPT

SCRIPT FreezeSkater
	SwitchOnBoard
	ClearExceptions
	SetQueueTricks NoTricks
	DisablePlayerInput
	SetRollingFriction 10000
	PlayAnim Anim = StartIdle Cycle
ENDSCRIPT

SCRIPT CarBail
	InBail
	Obj_Spawnscript BloodCar
	PlaySound hitdumpsterX
	PlaySound hitvehicle3
	goto NoseManualBail
ENDSCRIPT

SCRIPT FlailingFall
	IF Obj_FlagNotSet FLAG_SKATER_KILLING
		Obj_SetFlag FLAG_SKATER_KILLING
		InBail
		Obj_ClearFlag FLAG_SKATER_KILLING
	ELSE
		InBail
	ENDIF
	ClearExceptions
	SetQueueTricks NoTricks
	DisablePlayerInput AllowCameraControl
	PlayAnim Anim = StretchtoFlailingFall BlendPeriod = 0.3
	ClearGapTricks
	ClearPanel_Bailed
	WaitAnim 90 Percent
	SwitchOffBoard
	WaitAnimFinished
	PlayAnim Anim = FlailingFall Cycle
ENDSCRIPT

SCRIPT RestartSkater
	RestartSkaterExceptions
	ClearGapTricks
	ClearPanel_Bailed
	EnablePlayerInput
	OnGroundExceptions
	WaitAnimFinished
	goto OnGroundAI
ENDSCRIPT

SCRIPT DropIn DropInAnim = DropIn
	SetState Lip
	//changed tilt to negative
	SetSkaterCamOverride heading = 0 tilt = 0.05 time = 0.0
	RestartSkaterExceptions
	SetRollingFriction 10000
	DisablePlayerInput
	PlayAnim Anim = <DropInAnim> BlendPeriod = 0.0
	WaitAnim 20 Percent
	SetSkaterCamOverride heading = 0 tilt = 0 time = 2
	WaitAnim 85 Percent
	SetState Ground
	SetRollingFriction 0
	WaitAnimFinished
	EnablePlayerInput
	ClearSkaterCamOverride
	OnGroundExceptions
	WaitOneGameFrame
	goto Land
ENDSCRIPT

SCRIPT StartLevelSkating
	RestartSkaterExceptions
	IF AutoKickIsOff
		IF ShouldMongo
			PlayAnim Anim = MongoBrakeIdle
		ELSE
			PlayAnim Anim = BrakeIdle
		ENDIF
	ELSE
		DisablePlayerInput
		SetRollingFriction 100000
		PlayAnim Anim = StartIdle
		wait 2 frames
		WaitAnimFinished
		SetRollingFriction #"default"
		PlayAnim Anim = StartSkating1 BlendPeriod = 0.0
		WaitAnim 75 Percent
	ENDIF
	SetRollingFriction #"default"
	EnablePlayerInput
	ClearSkaterCamOverride
	OnGroundExceptions
	WaitAnimFinished
	goto OnGroundAI
ENDSCRIPT

SCRIPT StartSkating1
	RestartSkaterExceptions
	SetRollingFriction #"default"
	DisablePlayerInput AllowCameraControl
	IF ProfileEquals is_named = mullen
		SetRollingFriction 10000
		PlayAnim Anim = MullenStart BlendPeriod = 0.0
		WaitAnim 40 Percent
		PlaySound boneless09 pitch = 110
		PlayBonkSound
		BlendperiodOut 0
		WaitAnim 60 Percent
		SetRollingFriction #"default"
	ELSE
		IF AutoKickIsOff
			IF ShouldMongo
				PlayAnim Anim = MongoBrakeIdle
			ELSE
				PlayAnim Anim = BrakeIdle
			ENDIF
		ELSE
			PlayAnim Anim = StartSkating1 BlendPeriod = 0.0
			WaitAnim 75 Percent
		ENDIF
	ENDIF
	ClearSkaterCamOverride
	EnablePlayerInput
	OnGroundExceptions
	WaitAnimFinished
	goto OnGroundAI
ENDSCRIPT

SCRIPT StartSkatingCanada
	Obj_Spawnscript CanadaBreath
	goto StartSkating11
ENDSCRIPT

SCRIPT CanadaBreath
	Obj_Attachframe bone = "head"
	BEGIN
		BloodParticlesOn name = "breath.png" start_col = -8355712 end_col = 1434484864 num = 1 emit_w = 1.0 emit_h = 1.0 angle = 90 size = 4 bone = "head" growth = 6 time = 2 speed = 20 grav = 0 life = 2
		wait 10 frames
	REPEAT
ENDSCRIPT

SCRIPT PedProps name = "Ped Props" score = 500
	IF InSplitScreenGame
	ELSE
		LaunchPanelMessage "Spectator Bonus" properties = Panelcombo2
	ENDIF
	PlaySound PedProps vol = 500
	SetTrickName <name>
	SetTrickScore <score>
	Display BlockSpin
	WaitOneGameFrame
	IF OnGround
		IF DoingTrick
		ELSE
			CheckGapTricks
			ClearGapTricks
			ClearPanel_Landed
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT RestartSkaterExceptions
	LeaveObserveMode2
	AllowRailTricks
	BoardRotate normal
	ClearExceptions
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	ClearTrickQueue
	SetQueueTricks NoTricks
	ClearManualTrick
	ClearEventBuffer
	SwitchOnBoard
	Obj_Attachframe bone = "head"
ENDSCRIPT

SCRIPT NetPause
ENDSCRIPT

SCRIPT Foobar
	IF OnGround
		SetRollingFriction 100
		CessBrake
		HandBrake
	ELSE
		WaitOnGround
		Land
		HandBrake
	ENDIF
ENDSCRIPT

SCRIPT LaunchSpecialMessage text = "Special Trick"
	PlaySound HUD_specialtrickAA vol = 100
	IF InSplitScreenGame
	ELSE
		IF IsOptionOn LM_GUI_bTrickNotifications
			LaunchPanelMessage <text> properties = panelcombo
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT LaunchExtraMessage text = "Hidden Combo!"
	PlaySound HUD_specialtrickAA vol = 100
	IF InSplitScreenGame
	ELSE
		IF IsOptionOn LM_GUI_bTrickNotifications
			LaunchPanelMessage <text> properties = panelcombo
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT MakeSkaterFly
	MakeSkaterGoto FlyAi
ENDSCRIPT

SCRIPT FlyAi move = 8
	ClearExceptions
	SetQueueTricks NoTricks
	SetSpeed 0
	SetState Air
	SetRollingFriction 1000
	NoRailTricks
	BEGIN
		IF held R2
			#"Jump"
			wait 8 frames
		ENDIF
		IF held L1
			BREAK
		ENDIF
		IF held L2
			IF held square
				move x = 18
			ENDIF
			IF held Circle
				move x = -18
			ENDIF
			IF held Triangle
				move z = 18
			ENDIF
			IF held x
				move z = -18
			ENDIF
		ELSE
			IF held square
				move x = 6
			ENDIF
			IF held Circle
				move x = -6
			ENDIF
			IF held Triangle
				move z = 8
			ENDIF
			IF held x
				move z = -8
			ENDIF
		ENDIF
		WaitOneGameFrame
	REPEAT
	AllowRailTricks
	MakeSkaterGoto OnGroundAI
ENDSCRIPT

SCRIPT PedKnockDown
	Obj_Spawnscript BloodCar
	SetRollingFriction 0
	goto NoseManualBail
ENDSCRIPT

SCRIPT SkaterPushPed
	SetException Ex = CarBail Scr = CarBail
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	FlailVibrate
	NollieOff
	StopBalanceTrick
	PlaySound bitchslap2
	IF AnimEquals [ CrouchIdle SkateIdle Land MongoPushCycle PushCycle ]
		goto FlailHitWall
	ENDIF
	IF AnimEquals [ runout runoutquick Smackwallupright ]
		goto Bailsmack Params = { SmackAnim = Smackwallupright }
	ENDIF
	IF InAir
		goto Airborne
	ELSE
		IF IsInBail
			IF AnimEquals [ Smackwallfeet NutterFallBackward FiftyFiftyFallBackward BackwardsTest BackwardFaceSlam Smackwallfeet SlipBackwards ]
				goto Bailsmack Params = { SmackAnim = Smackwallfeet }
			ELSE
				goto Bailsmack Params = { <...> }
			ENDIF
		ENDIF
		goto OnGroundAI
	ENDIF
ENDSCRIPT

SCRIPT SkaterBreakGlass
	SetException Ex = CarBail Scr = CarBail
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	FlailVibrate
	NollieOff
	StopBalanceTrick
	IF AnimEquals [ CrouchIdle SkateIdle Land MongoPushCycle PushCycle ]
		goto FlailHitWall
	ENDIF
	IF AnimEquals [ runout runoutquick Smackwallupright ]
		goto Bailsmack Params = { SmackAnim = Smackwallupright }
	ENDIF
	IF InAir
		goto Airborne
	ELSE
		IF IsInBail
			IF AnimEquals [ Smackwallfeet NutterFallBackward FiftyFiftyFallBackward BackwardsTest BackwardFaceSlam Smackwallfeet SlipBackwards ]
				goto Bailsmack Params = { SmackAnim = Smackwallfeet }
			ELSE
				goto Bailsmack Params = { <...> }
			ENDIF
		ENDIF
		goto OnGroundAI
	ENDIF
ENDSCRIPT

SCRIPT SkaterCollideBall
	IF InAir
		InAirExceptions
		Obj_Spawnscript CarSparks
		PlayAnim Anim = Boneless BlendPeriod = 0.3
		SetTrickName "Scratchin the Ball!"
		SetTrickScore 400
		Display
		WaitAnimWhilstChecking AndManuals
		goto Airborne
	ELSE
		InBail
		Obj_Spawnscript BloodCar
		PlaySound bodysmackA
		goto NoseManualBail
		LaunchPanelMessage "Ball Busted!"
	ENDIF
ENDSCRIPT