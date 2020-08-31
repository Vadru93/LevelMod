SCRIPT animload_skater_m
	animload_skater_m_fliptricks
	animload_skater_m_basics
	animload_skater_m_bails
	animload_skater_m_grabtricks
	animload_skater_m_flipgrab
	animload_skater_m_grind
	animload_skater_m_lip
	animload_skater_m_special
	animload_skater_m_flatland
	animload_skater_m_misc
	animload_skater_m_unused
ENDSCRIPT

SCRIPT animload_skater_m_unused
	//this script loads additional animations, found in THPS3 1.3 mod
	//this is here to prevent crashing online, in case someone loads with 1.3 tricks
	//same anims may be loaded later with proper chacksums 
	LoadAnim name = "Anims\skater_m\skater_m_IndyBackFlip.ska" descChecksum = 0x43424140
	LoadAnim name = "Anims\skater_m\skater_m_SomiSpin.ska" descChecksum = 0x44434241
	LoadAnim name = "Anims\skater_m\skater_m_ShoveItRewind.ska" descChecksum = 0x45444342
	LoadAnim name = "Anims\skater_m\skater_m_TripleHeelFlipARstyle.ska" descChecksum = 0x46454443
	LoadAnim name = "Anims\skater_m\skater_m_PogoAir.ska" descChecksum = 0x47464544
	LoadAnim name = "Anims\skater_m\skater_m_Salute_Idle.ska" descChecksum = 0x48474645
	LoadAnim name = "Anims\skater_m\skater_m_Salute_Init.ska" descChecksum = 0x49484746
	
	//flips
	LoadAnim name = "Anims\skater_m\skater_m_HeelFlipToJudo.ska" descChecksum = HeelFlipToJudo
	LoadAnim name = "Anims\skater_m\skater_m_DoubleHardFlip.ska" descChecksum = DoubleHardFlip
	LoadAnim name = "Anims\skater_m\skater_m_WellHardFlip.ska" descChecksum = WellHardFlip
	LoadAnim name = "Anims\skater_m\skater_m_ShoveItRewind.ska" descChecksum = ShoveItRewind
	LoadAnim name = "Anims\skater_m\skater_m_SomiSpin.ska" descChecksum = SomiSpin
	LoadAnim name = "Anims\skater_m\skater_m__360FliptoMute.ska" descChecksum = _360FliptoMute
	LoadAnim name = "Anims\skater_m\skater_m_BackFootKickFliptoMute.ska" descChecksum = BackFootKickFliptoMute
	
	//grabs
	LoadAnim name = "Anims\skater_m\skater_m_DarkJedi_Idle.ska" descChecksum = DarkJedi_Idle
	LoadAnim name = "Anims\skater_m\skater_m_DarkJedi_Init.ska" descChecksum = DarkJedi_Init
	LoadAnim name = "Anims\skater_m\skater_m_DarkJedi_Out.ska" descChecksum = DarkJedi_Out
	LoadAnim name = "Anims\skater_m\skater_m_Salute_Idle.ska" descChecksum = Salute_Idle
	LoadAnim name = "Anims\skater_m\skater_m_Salute_Init.ska" descChecksum = Salute_Init


	//LoadAnim name = "Anims\skater_m\skater_m_360Flip_Street.ska" descChecksum = unused_anim1
	//LoadAnim name = "Anims\skater_m\skater_m_GymnastTest.ska" descChecksum = unused_anim18
	//LoadAnim name = "Anims\skater_m\skater_m_HospitalFlip.ska" descChecksum = unused_anim29
	//LoadAnim name = "Anims\skater_m\skater_m_MuteBackFlip.ska" descChecksum = unused_anim37

	//LoadAnim name = "Anims\skater_m\skater_m_PogoAir.ska" descChecksum = unused_anim63
	//LoadAnim name = "Anims\skater_m\skater_m_RacketAir.ska" descChecksum = unused_anim68
	//LoadAnim name = "Anims\skater_m\skater_m_SpideyFlip.ska" descChecksum = unused_anim79
	//LoadAnim name = "Anims\skater_m\skater_m_SpideyVarial.ska" descChecksum = unused_anim83
	//LoadAnim name = "Anims\skater_m\skater_m_StretchFast.ska" descChecksum = unused_anim84
	
	//LoadAnim name = "Anims\skater_m\skater_m_CrouchIdle_Range.ska" descChecksum = unused_anim4
	//LoadAnim name = "Anims\skater_m\skater_m_TripleHeelFlipARstyle.ska" descChecksum = unused_anim85


	//bad/duplicate anims
	//LoadAnim name = "Anims\skater_m\skater_m_NewNollie.ska" descChecksum = unused_anim40
	//LoadAnim name = "Anims\skater_m\skater_m_YeeHawFrontFlip.ska" descChecksum = YeeHawFrontFlip
	//LoadAnim name = "Anims\skater_m\skater_m_IndyBackFlip.ska" descChecksum = unused_anim30
	//LoadAnim name = "Anims\skater_m\skater_m_PopShoveIt360.ska" descChecksum = unused_anim64
	//LoadAnim name = "Anims\skater_m\skater_m_360ShoveIt.ska" descChecksum = unused_anim2
	//LoadAnim name = "Anims\skater_m\skater_m_DuckDive_Idle2.ska" descChecksum = unused_anim9


	//grinds

	//LoadAnim name = "Anims\skater_m\skater_m_NoseGrabTailSlide_Init.ska" descChecksum = unused_anim41
	//LoadAnim name = "Anims\skater_m\skater_m_NoseGrabTailSlide_Out.ska" descChecksum = unused_anim42
	//LoadAnim name = "Anims\skater_m\skater_m_NoseGrabTailSlide_Range.ska" descChecksum = unused_anim43

	//LoadAnim name = "Anims\skater_m\skater_m_OneFooted5_0_Init.ska" descChecksum = unused_anim44
	//LoadAnim name = "Anims\skater_m\skater_m_OneFooted5_0_Range.ska" descChecksum = unused_anim45

	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedBluntSlideBS_Init.ska" descChecksum = unused_anim46
	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedBluntSlideBS_Out.ska" descChecksum = unused_anim47
	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedBluntSlideBS_Range.ska" descChecksum = unused_anim48

	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedNoseGrind_Init.ska" descChecksum = unused_anim49
	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedNoseGrind_Range.ska" descChecksum = unused_anim50
	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedSmithBS_Init.ska" descChecksum = unused_anim51

	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedSmithBS_Range.ska" descChecksum = unused_anim52
	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedSmithFS_Init.ska" descChecksum = unused_anim53
	//LoadAnim name = "Anims\skater_m\skater_m_OneFootedSmithFS_Range.ska" descChecksum = unused_anim54

	//LoadAnim name = "Anims\skater_m\skater_m_OverTurnSmithBS_Init.ska" descChecksum = unused_anim55
	//LoadAnim name = "Anims\skater_m\skater_m_OverTurnSmithBS_Out.ska" descChecksum = unused_anim56
	//LoadAnim name = "Anims\skater_m\skater_m_OverTurnSmithBS_Range.ska" descChecksum = unused_anim57

	//LoadAnim name = "Anims\skater_m\skater_m_OverTurnSmithFS_Init.ska" descChecksum = unused_anim58
	//LoadAnim name = "Anims\skater_m\skater_m_OverTurnSmithFS_Out.ska" descChecksum = unused_anim59
	//LoadAnim name = "Anims\skater_m\skater_m_OverTurnSmithFS_Range.ska" descChecksum = unused_anim60

	//LoadAnim name = "Anims\skater_m\skater_m_PrimoGrind_Init.ska" descChecksum = unused_anim65
	//LoadAnim name = "Anims\skater_m\skater_m_PrimoGrind_Out.ska" descChecksum = unused_anim66
	//LoadAnim name = "Anims\skater_m\skater_m_PrimoGrind_Range.ska" descChecksum = unused_anim67

	//LoadAnim name = "Anims\skater_m\skater_m_SpideyGrind_Init.ska" descChecksum = unused_anim80
	//LoadAnim name = "Anims\skater_m\skater_m_SpideyGrind_Out.ska" descChecksum = unused_anim81
	//LoadAnim name = "Anims\skater_m\skater_m_SpideyGrind_Range.ska" descChecksum = unused_anim82

	//LoadAnim name = "Anims\skater_m\skater_m_FSBeniFingerFlipCrook_Init.ska" descChecksum = unused_anim10
	//LoadAnim name = "Anims\skater_m\skater_m_FSBeniFingerFlipCrook_Out.ska" descChecksum = unused_anim11
	//LoadAnim name = "Anims\skater_m\skater_m_FSBeniFingerFlipCrook_Range.ska" descChecksum = unused_anim12

	//LoadAnim name = "Anims\skater_m\skater_m_FSBenihanaFingerFlipCrooked_Init.ska" descChecksum = unused_anim13
	//LoadAnim name = "Anims\skater_m\skater_m_FSBenihanaFingerFlipCrooked_Out.ska" descChecksum = unused_anim14
	//LoadAnim name = "Anims\skater_m\skater_m_FSBenihanaFingerFlipCrooked_Range.ska" descChecksum = unused_anim15

	//LoadAnim name = "Anims\skater_m\skater_m_FSMadonnaTailSlide_Init.ska" descChecksum = unused_anim16
	//LoadAnim name = "Anims\skater_m\skater_m_FSMadonnaTailSlide_Range.ska" descChecksum = unused_anim17

	//LoadAnim name = "Anims\skater_m\skater_m_HeelFlipBluntSlide_Init.ska" descChecksum = unused_anim19
	//LoadAnim name = "Anims\skater_m\skater_m_HeelFlipBluntSlide_Out.ska" descChecksum = unused_anim20
	//LoadAnim name = "Anims\skater_m\skater_m_HeelFlipBluntSlide_Range.ska" descChecksum = unused_anim21

	//LoadAnim name = "Anims\skater_m\skater_m_HeelFlipBSDarkLipSlide_Init.ska" descChecksum = unused_anim22
	//LoadAnim name = "Anims\skater_m\skater_m_HeelFlipBSDarkLipSlide_Out.ska" descChecksum = unused_anim23
	//LoadAnim name = "Anims\skater_m\skater_m_HeelFlipBSDarkLipSlide_Range.ska" descChecksum = unused_anim24

	//LoadAnim name = "Anims\skater_m\skater_m_HeelFliptoBSDarkSlideLipSlide_Init.ska" descChecksum = unused_anim25
	//LoadAnim name = "Anims\skater_m\skater_m_HeelFliptoBSDarkSlideLipSlide_Out.ska" descChecksum = unused_anim26
	//LoadAnim name = "Anims\skater_m\skater_m_HeelFliptoBSDarkSlideLipSlide_Range.ska" descChecksum = unused_anim27

	//LoadAnim name = "Anims\skater_m\skater_m_RocketTailSlideBS_Init.ska" descChecksum = unused_anim69
	//LoadAnim name = "Anims\skater_m\skater_m_RocketTailSlideBS_Out.ska" descChecksum = unused_anim70
	//LoadAnim name = "Anims\skater_m\skater_m_RocketTailSlideBS_Range.ska" descChecksum = unused_anim71

	//LoadAnim name = "Anims\skater_m\skater_m_RocketTailSlideFS_Init.ska" descChecksum = unused_anim72
	//LoadAnim name = "Anims\skater_m\skater_m_RocketTailSlideFS_Out.ska" descChecksum = unused_anim73
	//LoadAnim name = "Anims\skater_m\skater_m_RocketTailSlideFS_Range.ska" descChecksum = unused_anim74

	//LoadAnim name = "Anims\skater_m\skater_m_LaidBackGrind_Init.ska" descChecksum = unused_anim31
	//LoadAnim name = "Anims\skater_m\skater_m_LaidBackGrind_Out.ska" descChecksum = unused_anim32
	//LoadAnim name = "Anims\skater_m\skater_m_LaidBackGrind_Range.ska" descChecksum = unused_anim33

	//LoadAnim name = "Anims\skater_m\skater_m_MaulSpinGrab_Idle.ska" descChecksum = unused_anim34
	//LoadAnim name = "Anims\skater_m\skater_m_MaulSpinGrab_Init.ska" descChecksum = unused_anim35
	//LoadAnim name = "Anims\skater_m\skater_m_MaulSpinGrab_Out.ska" descChecksum = unused_anim36

	//LoadAnim name = "Anims\skater_m\skater_m_NeckBreakGrind_Init.ska" descChecksum = unused_anim38
	//LoadAnim name = "Anims\skater_m\skater_m_NeckBreakGrind_Range.ska" descChecksum = unused_anim39

	//LoadAnim name = "Anims\skater_m\skater_m_PivotGrind_Init.ska" descChecksum = unused_anim61
	//LoadAnim name = "Anims\skater_m\skater_m_PivotGrind_Out.ska" descChecksum = unused_anim62

ENDSCRIPT