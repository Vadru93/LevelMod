SCRIPT Loop Name = 'Loop Balance' Score = 1000 InitAnim = CrouchIdle FromAirAnim = ManualFromAir BalanceAnim = CrouchIdle OffMeterTop = ManualBail OffMeterBottom = ManualBail
	ClearExceptions
	IF InAir
		Goto LoopOllie
	ENDIF
	Obj_SetFlag FLAG_SKATER_INLOOP
	OnExceptionRun LoopException
	SetException Ex = Ollied Scr = LoopOllie
	ClearLipCombos
	SetException Ex = GroundGone Scr = LoopOllie
	SetException Ex = MadeOtherSkaterBail Scr = MadeOtherSkaterBail_Called
	SetException Ex = OffMeterTop Scr = NoseManualBail
	SetException Ex = OffMeterBottom Scr = ManualBail
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	SetQueueTricks NoTricks
	SetManualTricks NoTricks
	StartBalanceTrick
	Vibrate Actuator = 1 Percent = 25
	SetRollingFriction 2
	PlayAnim Anim = Loop_Range wobble
	SetTrickName <Name>
	SetTrickScore 5000
	Display Blockspin
	BEGIN
		IF SpeedLessThan 400
			Obj_ClearFlag FLAG_SKATER_INLOOP
			IF InSplitScreenGame
			ELSE
				LaunchPanelMessage "&C1Not Enough Speed" properties = panelcombo
			ENDIF
			LoopOllie
		ENDIF
		WaitoneGameFrame
	REPEAT
ENDSCRIPT

SCRIPT LoopException
	Obj_ClearFlag FLAG_SKATER_INLOOP
ENDSCRIPT

SCRIPT LoopOllie
	ClearExceptions
	Obj_ClearFlag FLAG_SKATER_INLOOP
	IF AbsolutePitchGreaterThan 80
		Move y = 13
		SetSpeed 0
	ENDIF
	SetState Air
	InBail
	ClearExceptions
	SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
	SetException Ex = Landed Scr = BackwardFaceSlam
	SetQueueTricks NoTricks
	DisablePlayerInput AllowCameraControl
	PlayAnim Anim = StretchtoFlailingFall BlendPeriod = 0.3
	ClearGapTricks
	ClearPanel_Bailed
	IF AbsolutePitchGreaterThan 40
		SetSpeed 1
	ENDIF
	WaitAnim 90 Percent
	SwitchOffBoard
	WaitAnimFinished
	PlayAnim Anim = FlailingFall cycle
ENDSCRIPT

SCRIPT LoopLand
	IF Obj_FlagSet FLAG_SKATER_INLOOP
		StopBalanceTrick
		IF InSplitScreenGame
		ELSE
			LaunchPanelMessage "Loop Bonus!" properties = panelcombo
		ENDIF
		PlaySound HUD_specialtrickAA Vol = 500
		SetLandedFromVert
		Goto Land Params = { RevertTime = 30 }
	ELSE
		Goto LoopOllie
	ENDIF
ENDSCRIPT
