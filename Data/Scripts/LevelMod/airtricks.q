//there were weird 0xA7FFD73D params in levelout/th313
//it wasn't checked anywhere in the functions

AirTricks = [ 
	//flips
	{ Trigger = { TripleInOrder Left Left Square 300 } TrickSlot = Air_L_L_Square }
	{ Trigger = { TripleInOrder Right Right Square 300 } TrickSlot = Air_R_R_Square }
	{ Trigger = { TripleInOrder Up Up Square 300 } TrickSlot = Air_U_U_Square }
	{ Trigger = { TripleInOrder Down Down Square 300 } TrickSlot = Air_D_D_Square }
	{ Trigger = { AirTrickLogic Square UpLeft 500 } TrickSlot = Air_SquareUL }
	{ Trigger = { AirTrickLogic Square UpRight 500 } TrickSlot = Air_SquareUR }
	{ Trigger = { AirTrickLogic Square DownLeft 500 } TrickSlot = Air_SquareDL }
	{ Trigger = { AirTrickLogic Square DownRight 500 } TrickSlot = Air_SquareDR }
	{ Trigger = { AirTrickLogic Square Up 500 } TrickSlot = Air_SquareU }
	{ Trigger = { AirTrickLogic Square Down 500 } TrickSlot = Air_SquareD }
	{ Trigger = { AirTrickLogic Square Left 500 } TrickSlot = Air_SquareL }
	{ Trigger = { AirTrickLogic Square Right 500 } TrickSlot = Air_SquareR }
	
	//grabs
	{ Trigger = { TripleInOrder Left Left Circle 300 } TrickSlot = Air_L_L_Circle }
	{ Trigger = { TripleInOrder Right Right Circle 300 } TrickSlot = Air_R_R_Circle }
	{ Trigger = { TripleInOrder Up Up Circle 300 } TrickSlot = Air_U_U_Circle }
	{ Trigger = { TripleInOrder Down Down Circle 300 } TrickSlot = Air_D_D_Circle }
	{ Trigger = { AirTrickLogic Circle UpLeft 500 } TrickSlot = Air_CircleUL }
	{ Trigger = { AirTrickLogic Circle UpRight 500 } TrickSlot = Air_CircleUR }
	{ Trigger = { AirTrickLogic Circle DownLeft 500 } TrickSlot = Air_CircleDL }
	{ Trigger = { AirTrickLogic Circle DownRight 500 } TrickSlot = Air_CircleDR }
	{ Trigger = { AirTrickLogic Circle Up 500 } TrickSlot = Air_CircleU }
	{ Trigger = { AirTrickLogic Circle Down 500 } TrickSlot = Air_CircleD }
	{ Trigger = { AirTrickLogic Circle Right 500 } TrickSlot = Air_CircleR }
	{ Trigger = { AirTrickLogic Circle Left 500 } TrickSlot = Air_CircleL } 
]

SpecialTricks = [ 
	{ Trigger = { TripleInOrder Up Right Square 400 } TrickSlot = SpAir_U_R_Square }
	{ Trigger = { TripleInOrder Up Down Square 400 } TrickSlot = SpAir_U_D_Square }
	{ Trigger = { TripleInOrder Up Left Square 400 } TrickSlot = SpAir_U_L_Square }
	{ Trigger = { TripleInOrder Right Up Square 400 } TrickSlot = SpAir_R_U_Square }
	{ Trigger = { TripleInOrder Right Down Square 400 } TrickSlot = SpAir_R_D_Square }
	{ Trigger = { TripleInOrder Right Left Square 400 } TrickSlot = SpAir_R_L_Square }
	{ Trigger = { TripleInOrder Down Up Square 400 } TrickSlot = SpAir_D_U_Square }
	{ Trigger = { TripleInOrder Down Right Square 400 } TrickSlot = SpAir_D_R_Square }
	{ Trigger = { TripleInOrder Down Left Square 400 } TrickSlot = SpAir_D_L_Square }
	{ Trigger = { TripleInOrder Left Up Square 400 } TrickSlot = SpAir_L_U_Square }
	{ Trigger = { TripleInOrder Left Right Square 400 } TrickSlot = SpAir_L_R_Square }
	{ Trigger = { TripleInOrder Left Down Square 400 } TrickSlot = SpAir_L_D_Square }
	{ Trigger = { TripleInOrder Up Right Circle 400 } TrickSlot = SpAir_U_R_Circle }
	{ Trigger = { TripleInOrder Up Down Circle 400 } TrickSlot = SpAir_U_D_Circle }
	{ Trigger = { TripleInOrder Up Left Circle 400 } TrickSlot = SpAir_U_L_Circle }
	{ Trigger = { TripleInOrder Right Up Circle 400 } TrickSlot = SpAir_R_U_Circle }
	{ Trigger = { TripleInOrder Right Down Circle 400 } TrickSlot = SpAir_R_D_Circle }
	{ Trigger = { TripleInOrder Right Left Circle 400 } TrickSlot = SpAir_R_L_Circle }
	{ Trigger = { TripleInOrder Down Up Circle 400 } TrickSlot = SpAir_D_U_Circle }
	{ Trigger = { TripleInOrder Down Right Circle 400 } TrickSlot = SpAir_D_R_Circle }
	{ Trigger = { TripleInOrder Down Left Circle 400 } TrickSlot = SpAir_D_L_Circle }
	{ Trigger = { TripleInOrder Left Up Circle 400 } TrickSlot = SpAir_L_U_Circle }
	{ Trigger = { TripleInOrder Left Right Circle 400 } TrickSlot = SpAir_L_R_Circle }
	{ Trigger = { TripleInOrder Left Down Circle 400 } TrickSlot = SpAir_L_D_Circle } 
]

//why revert is here though
Trick_Revert = { Scr = Revert }

SCR_FLIP = { Scr = FlipTrick }
SCR_GRAB = { Scr = GrabTrick }

EXTRA_FLIP = { SCR_FLIP Trigger = { Press Square 300 } }
EXTRA_GRAB = { SCR_GRAB Trigger = { Press Circle 500 } }
EXTRA_TRICK = { IsExtra UseCurrent }

GRABTWEAK_SMALL = 15
GRABTWEAK_MEDIUM = 20
GRABTWEAK_LARGE = 30
GRABTWEAK_SPECIAL = 35


//special flip tricks
Trick_The900 = { SCR_FLIP Params = { Name = 'The 900' Score = 9000 Anim = The900 IsSpecial BoardRotate TrickSlack = 20 RotateAfter NoSpin Speed = 1.4 } }
Trick_KFBackflip = { SCR_FLIP Params = { Name = 'Kickflip Backflip' Score = 3000 Anim = KickflipBodyBackFlip IsSpecial Speed = 1.3 NoSpin TrickSlack = 15 SpinSlack = 25 } }
Trick_StaleFrontFlip = { SCR_FLIP Params = { Name = 'Stalefish FrontFlip' Score = 4500 Anim = StalefishFrontFlip IsSpecial Speed = 1.3 NoSpin TrickSlack = 15 SpinSlack = 30 } }
Trick_StaleBackFlip = { SCR_FLIP Params = { Name = 'Stalefish BackFlip' Score = 4500 Anim = StalefishBackFlip IsSpecial Speed = 1.3 NoSpin TrickSlack = 15 SpinSlack = 30 } }
Trick_MistyFlip = { SCR_FLIP Params = { Name = 'Misty Flip' Score = 5000 Anim = MistyFlip IsSpecial Speed = 1.45625 NoSpin TrickSlack = 15 SpinSlack = 30 } }
Trick_FS540HeelFlip = { SCR_FLIP Params = { Name = 'FS 540 HeelFlip' Score = 4500 Anim = FS540HeelFlip BoardRotate IsSpecial Speed = 1.3 RevertBS RotateAfter NoSpin TrickSlack = 15 SpinSlack = 20 } }
Trick_FS540 = { SCR_FLIP Params = { Name = 'FS 540' Score = 4500 Anim = FS540 BoardRotate IsSpecial TrickSlack = 10 RevertBS RotateAfter NoSpin Speed = 1.3 SpinSlack = 40 } }
Trick_McTwist = { SCR_FLIP Params = { Name = 'McTwist' Score = 5000 Anim = McTwist BoardRotate IsSpecial RevertBS NoSpin TrickSlack = 10 RotateAfter Speed = 1.1 SpinSlack = 20 } }
Trick_540TailWhip = { SCR_FLIP Params = { Name = '540 TailWhip' Score = 2000 Anim = _540TailWhip IsSpecial BoardRotate Speed = 1.2 TrickSlack = 15 } }
Trick_Gazelle = { SCR_FLIP Params = { Name = 'Gazelle Underflip' Score = 3500 Anim = GazelleUnderflip IsSpecial BoardRotate TrickSlack = 20 NoSpin } }
Trick_GhettoBird = { SCR_FLIP Params = { Name = 'Ghetto Bird' Score = 3500 Anim = GhettoBird IsSpecial BoardRotate TrickSlack = 20 FlipAfter NoSpin Speed = 1.3 } }
Trick_FingerFlipAirWalk = { SCR_FLIP Params = { Name = 'Fingerflip Airwalk' Score = 1500 Anim = FingerFlipAirWalk IsSpecial BoardRotate Speed = 1.3 TrickSlack = 30 } }
Trick_Jackass = { SCR_FLIP Params = { Name = 'The Jackass' Score = 1500 Anim = Jackass IsSpecial Speed = 1.2 TrickSlack = 20 BloodFrame = 1 } }
Trick_1234 = { SCR_FLIP Params = { Name = '1-2-3-4' Score = 1400 Anim = _1234 IsSpecial Speed = 1.1 TrickSlack = 20 } }
Trick_DoubleKFindy = { SCR_FLIP Params = { Name = 'Double Kickflip Indy' Score = 1100 Anim = DoubleKFVarialIndy TrickSlack = 20 IsSpecial BoardRotate } }
Trick_540Flip = { SCR_FLIP Params = { Name = '540 Flip' Score = 1450 Anim = _540flip BoardRotate IsSpecial Speed = 1.2 TrickSlack = 20 } }
Trick_BetweenLegsSlam = { SCR_FLIP Params = { Name = 'Slamma Jamma' Score = 1750 Anim = BetweenLegsSlam IsSpecial TrickSlack = 20 Speed = 1.3 } }
Trick_NollieFlipUnderflip = { SCR_FLIP Params = { Name = 'Nollie Flip Underflip' Score = 1050 Anim = NollieFlipUnderFlip IsSpecial TrickSlack = 10 Speed = 1.3 } }
Trick_HardFlipBackFootFlip = { SCR_FLIP Params = { Name = 'Hardflip Late Flip' Score = 1500 Anim = HardFlipBackFootFlip IsSpecial TrickSlack = 15 Speed = 1.3 BoardRotate } }
Trick_QuadrupleHeelFlip = { SCR_FLIP Params = { Name = 'Quad Heelflip' Score = 1200 Anim = QuadrupleHeelFlip IsSpecial Speed = 1.3 } }
Trick_Layout = { SCR_FLIP Params = { Name = 'Handspring Layout' UnlockFlag = SKATER_UNLOCKED_CARRERA Score = 3000 Anim = Maul IsSpecial Speed = 1.3 TrickSlack = 30 SpinSlack = 25 } }
Trick_Berzerker = { SCR_FLIP Params = { Name = 'The Berzerker' UnlockFlag = SKATER_UNLOCKED_WOLVERINE Score = 2500 Anim = Berzerker IsSpecial Speed = 1.3 TrickSlack = 50 GrindSlack = 50 } }
Trick_ClawPunch = { SCR_FLIP Params = { Name = 'Claw Punch' UnlockFlag = SKATER_UNLOCKED_WOLVERINE Score = 3000 Anim = ClawPunch IsSpecial Speed = 1.2 TrickSlack = 20 } }

Trick_KickflipUnderFlip = { SCR_FLIP Params = { TrickDef_KickflipUnderflip IsSpecial } }

TrickDef_KickflipUnderflip = { 
	Name = 'Kickflip Underflip' 
	Score = 1000
	Anim = KickflipUnderFlip 
	TrickSlack = 10 
}

//special grab tricks
Trick_JudoMadonna = { SCR_GRAB Params = { Name = 'Judo Madonna' Score = 1600 Anim = JudoMadonna_Init Idle = Madonna_Idle BackwardsAnim = Madonna IsSpecial ForceInit TrickSlack = 10 } }
Trick_KFSuperman = { SCR_GRAB Params = { Name = 'Kickflip Superman' Score = 1500 Anim = KFSupermanGrab_Init Idle = KFSupermanGrab_Range OutAnim = KFSupermanGrab_Out IsSpecial ForceInit } }
Trick_2KickMadonnaFlip = { SCR_GRAB Params = { Name = 'Double Kickflip Madonna' Score = 1750 Anim = _2KickMadonnaFlip_Init Idle = Madonna_Idle OutAnim = _2KickMadonnaFlip_Out IsSpecial ForceInit TrickSlack = 15 Speed = 1.3 } }
Trick_AirCasperFlip = { SCR_GRAB Params = { Name = 'Casper Flip 360 Flip' Score = 2500 Anim = AirCasperFlip_Init Idle = AirCasperFlip_Idle OutAnim = AirCasperFlip_Out IsSpecial Speed = 1.3 ForceInit TrickSlack = 20 } }
Trick_PizzaGuy = { SCR_GRAB Params = { Name = 'Pizza Guy' Score = 1500 Anim = PizzaGuy_Init Idle = PizzaGuy_Idle OutAnim = PizzaGuy_Out IsSpecial ForceInit BoardRotate Speed = 1.0 TrickSlack = 20 } }
Trick_KickflipOneFootTail = { SCR_GRAB Params = { Name = 'Kickflip One Foot Tail' Score = 1100 Anim = KickflipOneFootTail_Init Idle = KickflipOneFootTail_Range OutAnim = KickflipOneFootTail_Out IsSpecial ForceInit TrickSlack = 15 } }
Trick_MaulForceGrab = { SCR_GRAB Params = { Name = 'Force Grab' UnlockFlag = SKATER_UNLOCKED_MAUL Score = 1300 Anim = MaulForceGrab_Init Idle = MaulForceGrab_Idle OutAnim = MaulForceGrab_Out IsSpecial ForceInit } }
Trick_RodeoClown = { SCR_GRAB Params = { Name = 'Rodeo Clown' UnlockFlag = SKATER_UNLOCKED_SLATER Score = 1500 Anim = RodeoClown_Init Idle = RodeoClown_Idle OutAnim = RodeoClown_Out NoSpin IsSpecial ForceInit Rotate x = -360 Duration = 1 } }
Trick_SuperHandClap = { SCR_GRAB Params = { Name = 'Super Hand Clap' UnlockFlag = SKATER_UNLOCKED_EYEBALL Score = 1000 Anim = Eyeball_Init Idle = Eyeball_Idle IsSpecial ForceInit } }
Trick_WorkForFood = { SCR_GRAB Params = { Name = 'Panhandling' UnlockFlag = SKATER_UNLOCKED_BUM Score = 2000 Anim = WorkForFood_Init Idle = WorkForFood_Idle IsSpecial ForceInit } }
Trick_ACAir = { SCR_GRAB Params = { Name = 'AC Air' UnlockFlag = SKATER_UNLOCKED_DEMONESS Score = 666 Anim = ACAir_Init Idle = ACAir_Idle OutAnim = ACAir_Out IsSpecial ForceInit } }
Trick_AssumePosition = { SCR_GRAB Params = { Name = 'Assume The Position II' UnlockFlag = SKATER_UNLOCKED_DICK Score = 1000 Anim = AssumePosition_Init Idle = AssumePosition_Idle IsSpecial ForceInit TrickSlack = 10 Speed = 1.3 } }


//======flip tricks======

//===kickflip chain===

Trick_Kickflip = { SCR_FLIP Params = TrickDef_Kickflip }

TrickDef_Kickflip = { 
	Name = 'Kickflip / Dbl.Kickflip' 
	ShowName = 'Kickflip' 
	Score = 100 
	Anim = Kickflip 
	Nollie = NollieKickflip 
	ExtraTricks = KickflipExtras 
	NewExtraTricks = NewKickflipExtras 
}

KickflipExtras = [ 
	{ EXTRA_FLIP Params = { TrickDef_DoubleKickflip EXTRA_TRICK } }
	{ Trigger = { AirTrickLogic Circle Right 300 } Scr = FlipGrabBlend Params = { Name = 'Kickflip to Indy' Score = 400 Anim1 = KickFlipBlendFS Anim2 = Indy IsExtra } }
	{ Trigger = { AirTrickLogic Circle Up 300 } Scr = FlipGrabBlend Params = { Name = 'Kickflip to Crail' Score = 400 Anim1 = KickFlipBlendFS Anim2 = Crail IsExtra GrabStart = 5 Speed = 1 } } 
]

NewKickflipExtras = [ 
	{ EXTRA_FLIP Params = { TrickDef_DoubleKickflip EXTRA_TRICK Speed = 1.3 } } 
	{ SCR_GRAB Trigger = { AirTrickLogic Circle Right 300 } Params = { TrickDef_KickflipToIndy EXTRA_TRICK } } 
	{ SCR_GRAB Trigger = { AirTrickLogic Circle Up 300 } Params = { TrickDef_KickflipToCrail EXTRA_TRICK } } 
	{ SCR_GRAB Trigger = { AirTrickLogic Circle Left 300 } Params = { TrickDef_KickflipToMelon EXTRA_TRICK } } 
	{ SCR_FLIP Trigger = { Press Up 300 } Params = { TrickDef_KickflipUnderFlip EXTRA_TRICK Speed = 1.1 Score = 500 } }
]

//DoubleKickflip = [ { EXTRA_FLIP Params = { TrickDef_DoubleKickflip EXTRA_TRICK } } ]

TrickDef_DoubleKickflip = { 
	Name = 'Double Kickflip' 
	Score = 500 
	Anim = DoubleKickflip 
	ExtraTricks = TripleKickflip
	NewExtraTricks = NewTripleKickflip
} 

TripleKickflip = [ { EXTRA_FLIP Params = { TrickDef_TripleKickflip EXTRA_TRICK } } ] 
NewTripleKickflip = [ { EXTRA_FLIP Params = { TrickDef_TripleKickflip EXTRA_TRICK Speed = 1.3 } } ] 

TrickDef_TripleKickflip = { 
	Name = 'Triple Kickflip' 
	Score = 1000 
	Anim = TripleKickflip 
}

//===heelflip chain===

Trick_Heelflip = { SCR_FLIP Params = TrickDef_Heelflip }

TrickDef_Heelflip = { 
	Name = 'Heelflip / Dbl.Heelflip' 
	ShowName = 'Heelflip' 
	Score = 100 
	Anim = Heelflip 
	Nollie = NollieHeelflip 
	ExtraTricks = DoubleHeelflip
	NewExtraTricks = NewDoubleHeelflip
}

DoubleHeelflip = [ { EXTRA_FLIP Params = { TrickDef_DoubleHeelflip EXTRA_TRICK } } ] 
NewDoubleHeelflip = [ { EXTRA_FLIP Params = { TrickDef_DoubleHeelflip EXTRA_TRICK Speed = 1.3 } } ] 

TrickDef_DoubleHeelflip = { 
	Name = 'Double Heelflip' 
	Score = 500 
	Anim = DoubleHeelflip 
	TrickSlack = 15 
	ExtraTricks = TripleHeelflip
	NewExtraTricks = NewTripleHeelflip
}

TripleHeelflip = [ { EXTRA_FLIP Params = { TrickDef_TripleHeelflip EXTRA_TRICK } } ] 
NewTripleHeelflip = [ { EXTRA_FLIP Params = { TrickDef_TripleHeelflip EXTRA_TRICK Speed = 1.3 } } ] 

TrickDef_TripleHeelflip = { 
	Name = 'Triple Heelflip' 
	Score = 1000 
	Anim = TripleHeelflip 
}


//===impossible/2x/3x

Trick_Impossible = { SCR_FLIP Params = TrickDef_Impossible }

TrickDef_Impossible = { 
	Name = 'Impossible / Dbl.Impossible' 
	ShowName = 'Impossible' 
	Score = 100 
	Anim = Impossible 
	Nollie = NollieImpossible 
	ExtraTricks = DoubleImpossible 
	NewExtraTricks = NewDoubleImpossible 
}

DoubleImpossible = [ { EXTRA_FLIP Params = { TrickDef_DoubleImpossible EXTRA_TRICK } } ]
NewDoubleImpossible = [ { EXTRA_FLIP Params = { TrickDef_DoubleImpossible EXTRA_TRICK Speed = 1.3 } } ]

TrickDef_DoubleImpossible = { 
	Name = 'Double Impossible'
	Score = 500 
	Anim = DoubleImpossible 
	ExtraTricks = TripleImpossible 
	NewExtraTricks = NewTripleImpossible 
	TrickSlack = 15 
}

TripleImpossible = [ { EXTRA_FLIP Params = { TrickDef_TripleImpossible EXTRA_TRICK } } ]
NewTripleImpossible = [ { EXTRA_FLIP Params = { TrickDef_TripleImpossible EXTRA_TRICK Speed = 1.3 } } ]

TrickDef_TripleImpossible = { 
	Name = 'Triple Impossible' 
	Score = 1000 
	Anim = TripleImpossible 
	TrickSlack = 15 
}


//===varial kickflip/360 flip

Trick_VarialKickflip = { SCR_FLIP Params = TrickDef_VarialKickflip }

TrickDef_VarialKickflip = { 
	Name = 'Varial Kickflip / 360 Flip' 
	ShowName = 'Varial Kickflip' 
	Score = 300 
	Anim = VarialKickflip 
	BoardRotate 
	Nollie = NollieVarialKickflip 
	NewExtraTricks = ExtraVarialKickflip 
}

ExtraVarialKickflip = [ { EXTRA_FLIP Params = { TrickDef_360Flip EXTRA_TRICK } } ]

Trick_360Flip = { SCR_FLIP Params = TrickDef_360Flip }

TrickDef_360Flip = { 
	Name = '360 Flip' 
	Score = 500 
	Anim = _360Flip 
	Nollie = Nollie360Flip 
	Speed = 0.85
	//ExtraTricks = 0x16151413 
}


///===varial heelflip/360 heelflip===

Trick_VarialHeelflip = { SCR_FLIP Params = TrickDef_VarialHeelflip }

TrickDef_VarialHeelflip = { 
	Name = 'Varial Kickflip / 360 Heelflip' 
	ShowName = 'Varial Heelflip' 
	Score = 300 
	Anim = VarialHeelflip 
	BoardRotate 
	Nollie = NollieVarialHeelflip 
	NewExtraTricks = ExtraVarialHeelflip
} 

ExtraVarialHeelflip = [ { EXTRA_FLIP Params = { TrickDef_360Heelflip EXTRA_TRICK Name = '360 Heelflip' Score = 500 Speed = 1.2 } } ]

Trick_LaserFlip = { SCR_FLIP Params = { TrickDef_360Heelflip IsSpecial } }

TrickDef_360Heelflip = { 
	Name = 'Laser Flip' 
	Score = 1000 
	Anim = laserflip 
	TrickSlack = 20
}


//===hardflip/360 hardflip===

Trick_Hardflip = { SCR_FLIP Params = TrickDef_Hardflip }

TrickDef_Hardflip = { 
	Name = 'Hardflip / 360 Hardflip' 
	ShowName = 'Hardflip' 
	Score = 300 
	Anim = Hardflip 
	BoardRotate 
	Nollie = NollieHardflip 
	NewExtraTricks = Extra360Hardflip 
}

Extra360Hardflip = [ { EXTRA_FLIP Params = { TrickDef_360Hardflip EXTRA_TRICK Speed = 1.3 Score = 500 } } ]

Trick_360Hardflip = { SCR_FLIP Params = { TrickDef_360Hardflip IsSpecial } }

TrickDef_360Hardflip = { 
	Name = '360 Hardflip' 
	Score = 1350 
	Anim = _360Hardflip 
	Speed = 1.1 
}

//===inward heelflip===
//there is no 360 inward heelflip anim, so just link to self to avoid bails

Trick_InwardHeelflip = { SCR_FLIP Params = TrickDef_InwardHeelflip }

TrickDef_InwardHeelflip = {
	TrickDef_InwardHeelflip2x
	NewExtraTricks = ExtraInwardHeelflip 
}

ExtraInwardHeelflip = [ { EXTRA_FLIP Params = { TrickDef_InwardHeelflip2x EXTRA_TRICK } } ]

TrickDef_InwardHeelflip2x = {
	Name = 'Inward Heelflip' 
	Score = 350 
	Anim = InwardHeelflip 
	BoardRotate 
	Nollie = NollieInwardFlip 
	TrickSlack = 15 
}


//===pop shove-it chain===

Trick_PopShoveIt = { SCR_FLIP Params = TrickDef_PopShoveIt }

TrickDef_PopShoveIt = { 
	Name = 'Pop Shove-It / 360 Shove-It' 
	ShowName = 'Pop Shove-It' 
	Score = 100 
	Anim = PopShoveIt 
	BoardRotate 
	Nollie = NollieBSShoveIt 
	ExtraTricks = Extra360ShoveIt 
	NewExtraTricks = NewExtra360ShoveIt 
}

Extra360ShoveIt = [ { EXTRA_FLIP Params = { TrickDef_360ShoveIt EXTRA_TRICK } } ]
NewExtra360ShoveIt = [ { EXTRA_FLIP Params = { TrickDef_360ShoveIt EXTRA_TRICK Speed = 1.3 } } ]

TrickDef_360ShoveIt = { 
	Name = '360 Shove-It' 
	Score = 500 
	Anim = _360ShoveIt 
	Nollie = Nollie360ShoveIt
	ExtraTricks = Extra540ShoveIt 
}

Extra540ShoveIt = [ { EXTRA_FLIP Params = { TrickDef_540ShoveIt EXTRA_TRICK } } ]

TrickDef_540ShoveIt = {
	Name = '540 Shove-It' 
	Score = 1000 
	Anim = _540ShoveIt 
	BoardRotate
}


//===fs shove-it chain===

Trick_FSShoveIt = { SCR_FLIP Params = TrickDef_FSShoveIt }

TrickDef_FSShoveIt = { 
	Name = 'FS Shove-It / FS 360 Shove-It' 
	ShowName = 'FS Shove-It' 
	Score = 100 
	Anim = PopShoveItBS 
	Nollie = NollieFSShoveIt 
	BoardRotate 
	ExtraTricks = ExtraFS360ShoveIt 
	NewExtraTricks = NewExtraFS360ShoveIt 
}

ExtraFS360ShoveIt = [ { EXTRA_FLIP Params = { TrickDef_FS360ShoveIt EXTRA_TRICK } } ]
NewExtraFS360ShoveIt = [ { EXTRA_FLIP Params = { TrickDef_FS360ShoveIt EXTRA_TRICK Speed = 1.3 } } ]

TrickDef_FS360ShoveIt = {
	Name = '360 FS Shove-It' 
	Score = 500 
	Anim = FS360ShoveIt 
	ExtraTricks = ExtraFS540ShoveIt
}

ExtraFS540ShoveIt = [ { EXTRA_FLIP Params = { TrickDef_FS540ShoveIt EXTRA_TRICK } } ]

TrickDef_FS540ShoveIt = {
	Name = '540 FS Shove-It'
	Score = 1000 
	Anim = FS540ShoveIt 
	BoardRotate 
}


//===180 varial/360 varial

Trick_180Varial = { SCR_FLIP Params = TrickDef_180Varial }

TrickDef_180Varial = { 
	Name = '180 Varial / 360 Varial' 
	ShowName = '180 Varial' 
	Score = 700 
	Anim = _180Varial 
	BoardRotate 
	Speed = 1.3 
	TrickSlack = 25 
	NewExtraTricks = Extra360Varial 
}

Extra360Varial = [ { EXTRA_FLIP Params = { TrickDef_360Varial EXTRA_TRICK } } ]

Trick_360Varial = { SCR_FLIP Params = TrickDef_360Varial }

TrickDef_360Varial = { 
	Name = '360 Varial' 
	Score = 900 
	Anim = _360Varial 
	Speed = 1.3 
	TrickSlack = 25 
	BoardRotate 
	//ExtraTricks = 0x13121110
}


//===Front Foot Impossible/Dbl. FF Impossible===

TrickDef_FFImpossible = { 
	Name = 'Front Foot Impossible / Dbl. FF Imposs.' 
	ShowName = 'Front Foot Impossible' 
	Score = 400 
	Anim = FrontFootImposs 
	TrickSlack = 25 
	Speed = 1.1 
	NewExtraTricks = Extra2xFFImposs 
}

Extra2xFFImposs = [ { EXTRA_FLIP Params = { TrickDef_2xFFImpossible EXTRA_TRICK } } ]

TrickDef_2xFFImpossible = { 
	Name = 'Dbl. FF Impossible' 
	Score = 800 
	Anim = NollieImpossible
	Speed = 1.0
	Anim2 = FrontFootImposs 
	Anim2Wait = 25
	Anim2From = 3
	Anim2Speed = 1.1
	TrickSlack = 25 
	GrindSlack = 25 
} 

Trick_FFImpossible = { SCR_FLIP Params = TrickDef_FFImpossible }


//===ollie north/back foot flip===

Trick_OllieNorth = { SCR_FLIP Params = TrickDef_OllieNorth }

TrickDef_OllieNorth = { 
	Name = 'Ollie North / Back foot flip' 
	ShowName = 'Ollie North' 
	Score = 500 
	Anim = OllieNorth 
	Speed = 1.3 
	TrickSlack = 15 
	NewExtraTricks = ExtraOllieNorthBF 
}

ExtraOllieNorthBF = [ { EXTRA_FLIP Params = { TrickDef_OllieNorthBF EXTRA_TRICK } } ]

TrickDef_OllieNorthBF = { 
	Name = 'Ollie North Back Foot Flip' 
	Score = 1000 
	Anim = OllieNorth 
	Speed = 1.3 
	TrickSlack = 15 
	Anim2 = NollieKickflip
	Anim2Speed = 1.2
	Anim2Wait = 25
	Anim2From = 2
}


Trick_Fingerflip = { SCR_FLIP Params = { TrickDef_Fingerflip } }

TrickDef_Fingerflip = { 
	Name = 'Fingerflip' 
	Score = 700 
	Anim = FingerFlipVert 
	Speed = 1.3 
	TrickSlack = 25 
	NewExtraTricks = ExtraDoubleFingerFlip
}

ExtraDoubleFingerFlip = [ { EXTRA_FLIP Params = { TrickDef_DoubleFingerFlip EXTRA_TRICK } } ]

TrickDef_DoubleFingerFlip = {
	Name = 'Double Fingerflip' 
	Score = 1000 
	Anim = FingerFlipVert 
	Speed = 1.3 
	Anim2 = FingerFlipVert
	Anim2Wait = 23
	Anim2From = 7
	TrickSlack = 25 
	GrindSlack = 25 
}


Trick_Varial = { SCR_FLIP Params = TrickDef_Varial }

TrickDef_Varial = { 
	Name = 'Big Varial' 
	Score = 900 
	Anim = BigVarial 
	BoardRotate 
	TrickSlack = 25 
}


Trick_OllieAirwalk = { SCR_FLIP Params = { Name = 'Ollie Airwalk' Score = 500 Speed = 1.3 Anim = OllieAirWalk NewExtraTricks = CSOllieairwalkshoveit } }
CSOllieairwalkshoveit = [ { EXTRA_FLIP Params = { Name = 'Ollie Airwalk Late Shove-it' Score = 1000 Anim = OllieAirWalk Speed = 1.3 CSOllieairwalkshoveit IsExtra UseCurrent } } ]
Trick_HFVarialLien = { SCR_FLIP Params = { Name = 'Heelflip Varial Lien' Score = 800 Anim = HeelflipVarialLien BoardRotate TrickSlack = 15 } }
Trick_SalFlip = { SCR_FLIP Params = { Name = 'Sal Flip' Score = 900 Anim = SalFlip TrickSlack = 25 Speed = 1.3 } }

CSBenihanaFingerflip = { SCR_FLIP Params = { Name = 'Beni Fingerflip' Score = 1000 Anim = BenihanaFingerflip IsExtra } }




//flip trick links

0xA3A2A1A0 = { SCR_FLIP Params = { Name = 'Back Foot Kickflip' Score = 150 Anim = NollieKickflip Nollie = Kickflip ExtraTricks = KickflipExtras } }
0xA4A3A2A1 = { SCR_FLIP Params = { Name = 'Back Foot Heelflip' Score = 150 Anim = NollieHeelflip Nollie = Heelflip ExtraTricks = KickflipExtras } }

0xA5A4A3A2 = { SCR_FLIP Params = { Name = 'Indy BackFlip' Score = 4500 Anim = 0x43424140 IsSpecial Speed = 1.2 NoSpin TrickSlack = 15 SpinSlack = 30 } }
0xAAA9A8A7 = { SCR_GRAB Params = { Name = 'Salute!!!' Score = 1000 Anim = 0x49484746 Idle = 0x48474645 IsSpecial ForceInit } }
0xA6A5A4A3 = { SCR_FLIP Params = { Name = 'Somi Spin' Score = 1400 Anim = 0x44434241 IsSpecial TrickSlack = 10 Speed = 1.2 } }
0xA9A8A7A6 = { SCR_FLIP Params = { Name = 'Pogo Air' Score = 1400 Anim = 0x47464544 IsSpecial TrickSlack = 10 Speed = 1.2 } }
0xA7A6A5A4 = { SCR_FLIP Params = { Name = 'Shove it Rewind' Score = 1400 Anim = 0x45444342 IsSpecial TrickSlack = 10 Speed = 1.2 } }
0xA8A7A6A5 = { SCR_FLIP Params = { Name = 'Triple Heel Airwalk' Score = 1400 Anim = 0x46454443 IsSpecial TrickSlack = 10 Speed = 1.2 } }

//0x16151413 = [ { EXTRA_FLIP Params = { Name = '540 Flip' Score = 500 Anim = _540flip Nollie = _540flip BoardRotate IsExtra UseCurrent } } ]
0x13121110 = [ { EXTRA_FLIP Params = { Name = '540 Varial' Score = 1350 BoardRotate Anim = 0x3B9A0427 BoardRotate Nollie = 0x3B9A0427 IsExtra UseCurrent } } ]


//======grab tricks======

//===melon/method===

TrickDef_Melon = { 
	Name = 'Melon / Method' 
	ShowName = 'Melon'
	Score = 300 
	Anim = MelonGrab 
	Idle = MelonGrab_Idle 
	NewExtraTricks = ExtraMethod
}

ExtraMethod = [ { EXTRA_GRAB Params = { TrickDef_Method EXTRA_TRICK } } ]

TrickDef_Method = { 
	Name = 'Method' 
	Score = 400 
	Anim = Method 
	Idle = Method_Idle 
	Speed = 1.2 
} 

Trick_Melon = { SCR_GRAB Params = TrickDef_Melon }
Trick_Method = { SCR_GRAB Params = TrickDef_Method }

//===indy/stiffy===

TrickDef_Indy = { 
	Name = 'Indy / Stiffy' 
	ShowName = 'Indy' 
	Score = 300 
	Anim = Indy 
	Idle = Indy_Idle 
	NewExtraTricks = ExtraStiffy 
}

ExtraStiffy = [ { EXTRA_GRAB Params = { TrickDef_Stiffy EXTRA_TRICK } } ]

TrickDef_Stiffy = { 
	Name = 'Stiffy' 
	Score = 500 
	Anim = Stiffy 
	Idle = Stiffy_Idle 
	Speed = 1.25 
	ForceInit 
}

Trick_Indy = { SCR_GRAB Params = TrickDef_Indy }
Trick_Stiffy = { SCR_GRAB Params = TrickDef_Stiffy }

//===madonna/judo===

TrickDef_Madonna = { 
	Name = 'Madonna / Judo' 
	ShowName = 'Madonna' 
	Score = 600 
	TweakTrick = GRABTWEAK_LARGE 
	Anim = Madonna 
	Idle = Madonna_Idle 
	WaitPercent = 70 
	NewExtraTricks = ExtraJudo 
}

ExtraJudo = [ { EXTRA_GRAB Params = { TrickDef_Judo EXTRA_TRICK } } ]

TrickDef_Judo = {
	Name = 'Judo' 
	Score = 800 
	TweakTrick = 50 
	Anim = JudoGrab 
	Idle = JudoGrab_Idle 
	Speed = 1.3 
	
}

Trick_Madonna = { SCR_GRAB Params = TrickDef_Madonna }
Trick_Judo = { SCR_GRAB Params = TrickDef_Judo }

//===crail grab/tuckknee===

TrickDef_Crail = { 
	Name = 'Crail Grab / TuckKnee' 
	ShowName = 'Crail Grab' 
	Score = 350 
	Anim = Crail 
	Idle = Crail_Idle 
	NewExtraTricks = ExtraTuckKnee 
}

ExtraTuckKnee = [ { EXTRA_GRAB Params = { TrickDef_TuckKnee EXTRA_TRICK } } ]

TrickDef_TuckKnee = { 
	Name = 'TuckKnee' 
	Score = 400 
	Anim = TuckKnee 
	Idle = TuckKnee_Idle 
}

Trick_Crail = { SCR_GRAB Params = TrickDef_Crail }
Trick_TuckKnee = { SCR_GRAB Params = TrickDef_TuckKnee }

//===nosegrab/rocket air===

TrickDef_Nosegrab = { 
	Name = 'Nosegrab / Rocket Air' 
	ShowName = 'Nosegrab' 
	Score = 300 
	Anim = Nosegrab 
	Idle = Nosegrab_Idle 
	NewExtraTricks = ExtraRocket 
}

ExtraRocket = [ { EXTRA_GRAB Params = { TrickDef_Rocket EXTRA_TRICK } } ]

TrickDef_Rocket = { 
	Name = 'Rocket Air' 
	Score = 400 
	Anim = RocketAir 
	Idle = RocketAir_Idle 
}

Trick_Nosegrab = { SCR_GRAB Params = TrickDef_Nosegrab }
Trick_Rocket = { SCR_GRAB Params = TrickDef_Rocket }

//===Mute/Seatbelt Air===

TrickDef_Mute = { 
	Name = 'Mute / Seatbelt Air' 
	ShowName = 'Mute' 
	Score = 350 
	Anim = MuteGrab 
	Idle = MuteGrab_Idle 
	NewExtraTricks = ExtraSeatbelt 
}

ExtraSeatbelt = [ { EXTRA_GRAB Params = { TrickDef_Seatbelt EXTRA_TRICK } } ]

TrickDef_Seatbelt = { 
	Name = 'Seatbelt Air' 
	Score = 500
	Anim = Seatbelt 
	Idle = SeatBelt_Idle
}

Trick_Mute = { SCR_GRAB Params = TrickDef_Mute }
Trick_Seatbelt = { SCR_GRAB Params = TrickDef_Seatbelt }

//===indy nosebone/del mar indy===

TrickDef_IndyNosebone = { 
	Name = 'Indy Nosebone / Del Mar Indy' 
	ShowName = 'Indy Nosebone' 
	Score = 350 
	Anim = Nosebone 
	Idle = Nosebone_Idle 
	NewExtraTricks = ExtraDelMarIndy 
}

ExtraDelMarIndy = [ { EXTRA_GRAB Params = { TrickDef_DelMarIndy EXTRA_TRICK } } ]

TrickDef_DelMarIndy = {
	Name = 'Del Mar Indy' 
	Score = 400 
	Anim = IndyDelMar 
	Idle = IndyDelMar_Idle 
} 

Trick_IndyNosebone = { SCR_GRAB Params = TrickDef_IndyNosebone }
Trick_DelMarIndy = { SCR_GRAB Params = TrickDef_DelMarIndy }

//===japan/one foot japan===

Trick_Japan = { SCR_GRAB Params = TrickDef_Japan }

TrickDef_Japan = { 
	Name = 'Japan / One Foot Japan' 
	ShowName = 'Japan' 
	Score = 350 
	Anim = JapanAir 
	Idle = JapanAir_Idle 
	NewExtraTricks = ExtraOneFootJapan 
}

ExtraOneFootJapan = [ { EXTRA_GRAB Params = { TrickDef_OneFootJapan EXTRA_TRICK } } ]

Trick_OneFootJapan = { SCR_GRAB Params = TrickDef_OneFootJapan }

TrickDef_OneFootJapan = { 
	Name = 'One Foot Japan' 
	Score = 800 
	TweakTrick = GRABTWEAK_LARGE 
	Anim = OneFootJapan 
	Speed = 1.3 
	Idle = OneFootJapan_Idle 
	WaitPercent = 70 
}


//===crossbone/crookedcop===

Trick_Crossbone = { SCR_GRAB Params = TrickDef_Crossbone }

TrickDef_Crossbone = { 
	Name = 'Crossbone / CrookedCop' 
	ShowName = 'Crossbone' 
	Score = 350 
	Anim = Crossbone 
	Idle = Crossbone_Idle 
	NewExtraTricks = ExtraCrookedCop 
} 

ExtraCrookedCop = [ { EXTRA_GRAB Params = { TrickDef_CrookedCop EXTRA_TRICK } } ]

Trick_CrookedCop = { SCR_GRAB Params = TrickDef_CrookedCop }

TrickDef_CrookedCop = { 
	Name = 'CrookedCop' 
	Score = 400 
	Anim = CrookedCop 
	Idle = CrookedCop_Idle
}


//===tailgrab/one foot tailgrab

Trick_Tailgrab = { SCR_GRAB Params = TrickDef_Tailgrab }

TrickDef_Tailgrab = { 
	Name = 'Tailgrab / One Foot Tailgrab' 
	ShowName = 'Tailgrab' 
	Score = 300 
	Anim = Tailgrab 
	Idle = Tailgrab_Idle 
	NewExtraTricks = ExtraOneFootTail 
} 

ExtraOneFootTail = [ { EXTRA_GRAB Params = { TrickDef_OneFootTail EXTRA_TRICK } } ]

//this version allows excessive spam, tweak somehow
TrickDef_OneFootTail = { 
	Name = 'One Foot Tailgrab' 
	Score = 500 
	Anim = KickflipOneFootTail_Range 
	Idle = KickflipOneFootTail_Range 
	OutAnim = KickflipOneFootTail_Out 
}


//===airwalk/christ airwalk/christ

Trick_Airwalk = { SCR_GRAB Params = TrickDef_Airwalk }

TrickDef_Airwalk = {
	Name = 'Airwalk / Christ Air' 
	ShowName = 'Airwalk' 
	Score = 400 
	Anim = Airwalk 
	Idle = Airwalk_Idle2 
	NewExtraTricks = ExtraAirwalk
}

ExtraAirwalk = [ { EXTRA_GRAB Params = { TrickDef_ChristAir EXTRA_TRICK } } ]

Trick_ChristAir = { SCR_GRAB Params = TrickDef_ChristAir }

TrickDef_ChristAir = {
	Name = 'Christ Air'
	Score = 500 
	Anim = ChristAir_Init 
	Idle = ChristAir_Range 
	OutAnim = ChristAir_Out 
	ForceInit 
}


//===benihana/sacktap

Trick_Benihana = { SCR_GRAB Params = TrickDef_Benihana }

TrickDef_Benihana = { 
	Name = 'Benihana / Sacktap' 
	ShowName = 'Benihana' 
	Score = 300 
	Anim = Benihana
	Idle = Benihana_Idle 
	OutAnim = Benihana_Out 
	NewExtraTricks = BenihanaFingerflip
}

BenihanaFingerflip = [ 
	{ EXTRA_FLIP Params = { TrickDef_BeniFingerFlip IsExtra } }
	{ EXTRA_GRAB Params = { TrickDef_Sacktap IsExtra } } 
]

TrickDef_BeniFingerFlip = { 
	Name = 'Beni Fingerflip' 
	Score = 1000 
	Anim = BenihanaFingerflip 
}

Trick_SackTap = { SCR_GRAB Params = TrickDef_Sacktap }

TrickDef_Sacktap = { 
	Name = 'Sacktap' 
	Score = 1500 
	Anim = Sacktap_Init 
	Idle = Sacktap_Range 
	OutAnim = Sacktap_out 
	Speed = 1.5 
	TrickSlack = 20 
}




//to fix

Trick_FSShifty = { SCR_GRAB Params = { Name = 'FS Shifty' Score = 500 Anim = Shifty Idle = Shifty_Idle } }
Trick_Roastbeef = { SCR_GRAB Params = { Name = 'RoastBeef' Score = 350 Anim = Roastbeef Idle = Roastbeef_Idle ExtraTricks = 0xB6A69686 } }

Trick_Stalefish = { SCR_GRAB Params = { Name = 'Stalefish' Score = 400 Anim = Stalefish Idle = Stalefish_Idle } }
0xB6A69686 = [ { EXTRA_GRAB Params = { Name = 'Stalefish' Score = 400 Anim = Stalefish Idle = Stalefish_Idle IsExtra UseCurrent } } ]

Trick_SaranWrap = { SCR_GRAB Params = { Name = 'Wrap Around' Score = 450 TweakTrick = GRABTWEAK_LARGE Anim = SaranWrap Idle = SaranWrap_Idle OutAnim = AirIdle } }

Trick_Cannonball = { SCR_GRAB Params = { Name = 'Cannonball' Score = 250 TweakTrick = GRABTWEAK_SMALL Anim = CannonBall Idle = Cannonball_Idle Speed = 0.75 ExtraTricks = CSFingerFlipCannonball } }
CSFingerFlipCannonball =
[ { EXTRA_GRAB Params = { Name = 'Fingerflip Cannonball' Score = 500 Anim = CannonBall Speed = 1.4 Idle = Cannonball_Idle BackwardsAnim = CannonBall CSFingerFlipCannonball IsExtra UseCurrent } } ]




//these are original KF to tricks i guess?
Trick_KFIndy = { Scr = FlipGrabBlendFS Params = { Name = 'Kickflip to Indy' Score = 750 Anim = Kickflip IsExtra } }
Trick_KFMelon = { Scr = FlipGrabBlendBS Params = { Name = 'Kickflip to Melon' Score = 750 Anim = Heelflip IsExtra } }
KickflipToMelon = [ 
	{ 
		Trigger = { AirTrickLogic Circle Left 500 } 
		Scr = FlipGrabBlendBS 
		Params = { 
			Name = 'Kickflip to Melon' 
			Score = 400 
			Anim = Heelflip 
			Nollie = NollieHeelflip 
			EXTRA_TRICK 
		} 
	} 
]


//these are coolsnake's versions of KF to tricks

TrickDef_KickflipToIndy = { 
	Name = 'Kickflip to Indy' 
	Score = 500 
	Anim = Kickflip 
	Anim2 = Indy
	Idle = Indy_Idle 
	BackwardsAnim = Indy 
	Speed = 1.0 
	CSKickfliptoindy1 
	IsExtra 
} 

TrickDef_KickflipToCrail = { 
	Name = 'Kickflip to Crail' 
	Score = 500 
	Anim = Kickflip 
	Anim2 = Crail 
	Idle = Crail_Idle 
	BackwardsAnim = Crail 
	Speed = 1.0 
	CSKickfliptoCrail1 
	IsExtra 
	GrabStart = 5 
}

TrickDef_KickflipToMelon = { 
	Name = 'Kickflip to Melon' 
	Score = 500
	Anim = Kickflip
	Anim2 = MelonGrab 
	Idle = MelonGrab_Idle 
	BackwardsAnim = MelonGrab 
	Speed = 1.0 
	CSKickfliptoMelon1 
	IsExtra 
} 

CSKickfliptoMelon = { SCR_GRAB Params = TrickDef_KickflipToMelon }
CSKickfliptoCrail = { SCR_GRAB Params = TrickDef_KickflipToCrail }
CSKickfliptoIndy = { SCR_GRAB Params = TrickDef_KickflipToIndy }
		



//this is supposed to be the flips and rolls
0x1A191817 = [ 
	{ Trigger = { TripleInOrder Up Up R2 400 } Scr = 0x1B1A1918 }
	{ Trigger = { TripleInOrder Down Down R2 400 } Scr = 0x1C1B1A19 }
	{ Trigger = { TripleInOrder Left Left R2 400 } Scr = 0x1D1C1B1A }
	{ Trigger = { TripleInOrder Right Right R2 400 } Scr = 0x1E1D1C1B } 
]

//this is supposed to be the luge mode
LugeTricks = [ 
	{ Trigger = { TripleInOrder 0x20689278 0x20689278 0x20689278 400 } Scr = ExitLuge }
	{ Trigger = { TripleInOrder Circle Circle Circle 400 } Scr = 0x51B0E413 }
	{ Trigger = { TripleInOrder Square Square Square 400 } Scr = 0xB2A8A590 } 
]



script FlipTrick Speed = 1.0 TrickSlack = 10 GrindSlack = 25 Anim2From = 0 Anim2Wait = 0
	ClearTricksFrom Jumptricks Jumptricks0 Jumptricks
	CheckForOllie
	KillExtraTricks
	BailOn
	
	if GotParam NoSpin
		NoSpin
	endif
	
	if GotParam RevertFS
		Obj_SetFlag FLAG_SKATER_REVERTFS
	endif
	if GotParam RevertBS
		Obj_SetFlag FLAG_SKATER_REVERTBS
	endif

	//if we're in nollie state
	if InNollie
		//it's possible we don't have nollie anim, so check for it
		if GotParam Nollie
			if GotParam UseCurrent
				PlayAnim { Anim = <Nollie> From = Current to = End Speed = <Speed> BlendPeriod = 0.3 }
			else
				PlayAnim { Anim = <Nollie> Speed = <Speed> BlendPeriod = 0.3 }
			endif
		else
			if GotParam UseCurrent
				PlayAnim { Anim = <Anim> From = Current to = End Speed = <Speed> BlendPeriod = 0.3 }
			else
				PlayAnim { Anim = <Anim> Speed = <Speed> BlendPeriod = 0.3 }
			endif
		endif
	else
		if GotParam UseCurrent
			PlayAnim { Anim = <Anim> From = Current to = End Speed = <Speed> BlendPeriod = 0.3 }
		else
			PlayAnim { Anim = <Anim> Speed = <Speed> BlendPeriod = 0.3 }
		endif
	endif

	//this is a generic replacement for coolsnake's specific add flip tricks
	//if we have a second flip Anim, let's play it
	if GotParam Anim2
		WaitAnim <Anim2Wait> frames
		PlayAnim Anim = <Anim2> BlendPeriod = 0.3 Speed = <Anim2Speed> From = <Anim2From>
	//	if GotParam BoardRotate
	//		BlendperiodOut 0
	//		BoardRotateAfter
	//	endif
	endif

	if GotParam BoardRotate
		BlendperiodOut 0
		BoardRotateAfter
	endif
	
	if GotParam RotateAfter
		BlendperiodOut 0
		RotateAfter
	endif
	
	if GotParam FlipAfter
		BlendperiodOut 0
		FlipAfter
	endif

	Airtricks_SetExtraTricks <...>

	//rewrite using Anim2	
	if GotParam CSOllieairwalkshoveit
		WaitAnim 30 frames
		PlayAnim Anim = PopShoveItBS BlendPeriod = 0.3 Speed = <Speed> //1.3
		BlendperiodOut 0
		BoardRotateAfter
	endif
	
	Wait 15 frames

	//flips and rolls
	//SetExtraTricks tricks = 0x1A191817
	
	Airtricks_ShowTrickName <...>

	if GotParam BloodFrame
		Wait <BloodFrame> frames
		PlaySound bitchslap2 pitch = 90
		Bloodsplat
		SpawnSkaterScript BloodCar
		Wait 1 frame
		PlaySound headsmackB
	endif

	if GotParam SpinSlack
		WaitAnim <SpinSlack> frames fromend
		CanSpin
	endif
	
	if GotParam GrindSlack
		WaitAnim <GrindSlack> frames fromend
	endif
	
	BailOff
	WaitAnim <TrickSlack> frames fromend
	
	//what's this???
	//SetExtraTricks tricks = 0x72590D71
	
	DoNextTrick
	CanSpin
	WaitAnimFinished
	goto Airborne
endscript

script GrabTrick Speed = 1.0 x = -180 Duration = 1.0 GrabTweak = GRABTWEAK_MEDIUM WaitPercent = 60
	ClearTricksFrom Jumptricks Jumptricks0 Jumptricks
	CheckForOllie
	KillExtraTricks
	ClearExtraGrindTrick
	BailOn
	
	if GotParam IsExtra
		PlayAnim Anim = <Anim> BlendPeriod = 0.3 From = Current to = End Speed = <Speed>
	else
		PlayAnim Anim = <Anim> BlendPeriod = 0.3 Speed = <Speed>
	endif
	
	if AnimEquals DarkJedi_init
		SpawnSkaterScript MaulSFX
	endif
	
	if AnimEquals MaulForceGrab_Init
		PlaySound SaberTwirl5 pitch = 70
	endif
	
	if AnimEquals WorkForFood_Init
		SpawnSkaterScript Ollie_Signage
	endif
	
	Airtricks_SetExtraTricks <...>
	
	if GotParam CSFingerFlipCannonball
		PlayAnim Anim = FingerFlipVert BlendPeriod = 0.3 Speed = 1.3
		WaitAnim 23 frames
		PlayAnim Anim = Cannonball_Idle BlendPeriod = 0.3 Speed = 1.0 From = 20
	endif
	
	if GotParam CSKickfliptoindy1
		WaitAnim 10 percent
		PlayAnim Anim = KickflipBlendFS BlendPeriod = 0.3 From = Current
		Wait 15 frames
		Wait 5 frames
		PlayAnim Anim = Indy From = 10 to = End BlendPeriod = 0.3 Speed = 1.5
		WaitAnim 10 frames fromend
	endif
	
	if GotParam CSKickfliptoMelon1
		WaitAnim 10 percent
		PlayAnim Anim = KickflipBlendBS BlendPeriod = 0.3
		WaitAnim 15 frames
		Wait 10 frames
		PlayAnim Anim = MelonGrab From = 20 to = End BlendPeriod = 0.3 Speed = 1.4
		WaitAnimFinished
	endif
	
	if GotParam CSKickfliptoCrail1
		WaitAnim 10 percent
		PlayAnim Anim = KickflipBlendFS BlendPeriod = 0.3 From = Current
		Wait 15 frames
		PlayAnim Anim = Crail From = <GrabStart> to = End BlendPeriod = 0.3 Speed = <GrabSpeed>
		WaitAnim 10 frames fromend
	endif
	
	WaitAnim 50 percent
	
	Airtricks_ShowTrickName <...>
	
	//flips and rolls
	//SetExtraTricks tricks = 0x1A191817
	
	if GotParam ForceInit
		WaitAnimFinished
	else
		WaitAnim <WaitPercent> percent
	endif
	
	begin
		if Released Circle
			break
		endif
		if AnimFinished
			PlayAnim Anim = <Idle> cycle Speed = <Speed>
			if AnimEquals Eyeball_Idle
				SpawnSkaterScript Eyeball_SFX
			endif
			break
		endif
		WaitOneGameFrame
	repeat
	
	begin
		if Released Circle
			if GotParam OutAnim
				PlayAnim Anim = <OutAnim> BlendPeriod = 0.2 Speed = <Speed>
				break
			endif
			if GotParam BackwardsAnim
				PlayAnim Anim = <BackwardsAnim> Backwards BlendPeriod = 0.2 Speed = <Speed>
				break
			endif
			if AnimEquals Airwalk
				PlayAnim Anim = <Anim> From = Current to = 0 BlendPeriod = 0.2 Speed = <Speed>
			else
				PlayAnim Anim = <Anim> Backwards BlendPeriod = 0.2 Speed = <Speed>
			endif
			break
		endif
		WaitOneGameFrame
		
		if GotParam IsSpecial
			TweakTrick GRABTWEAK_SPECIAL
		else
			TweakTrick <GrabTweak>
		endif
		
	repeat
	
	if GotParam BoardRotate
		BlendperiodOut 0
		BoardRotateAfter
	endif
	
	if GotParam TrickSlack
		WaitAnim <TrickSlack> frames fromend
	endif
	
	BailOff
	DoNextTrick
	Wait 2 frames
	KillExtraTricks
	WaitAnimWhilstChecking
	goto Airborne
endscript

script Eyeball_SFX
	begin
		if AnimEquals Eyeball_Idle
			if frameis 2
				PlaySound bitchslap2 pitch = 120
			endif
		else
			break
		endif
		WaitOneGameFrame
	repeat
endscript

script MaulSFX
	PlaySound SaberTwirl5
	Wait 10
	PlaySound SaberTwirl5
	Wait 10
	PlaySound SaberTwirl5
	Wait 100
	PlaySound SaberTwirl5
	Wait 600
	PlaySound SaberHit1
endscript

script MaulSFX2
	PlaySound SaberHit1 pitch = 90
endscript

script Ollie_Signage
	SwitchOnAtomic special_item
	begin
		if AnimEquals WorkForFood_Init
		else
			if AnimEquals WorkForFood_Idle
			else
				break
			endif
		endif
		WaitOneGameFrame
	repeat
	SwitchOffAtomic special_item
endscript

script FlipGrabBlendFS Name = 'Kickflip to Indy'
	CheckForOllie
	ClearExtraGrindTrick
	KillExtraTricks
	if GotParam IsExtra
		LaunchExtraMessage
	endif
	PlayAnim Anim = KickflipBlendFS BlendPeriod = 0.3 From = Current
	SetTrickName <Name>
	Wait 15 frames
	Display
	Wait 5 frames
	PlayAnim Anim = Indy From = 10 to = End BlendPeriod = 0.3 Speed = 1.5
	WaitAnim 10 frames fromend
	DoNextTrick
	WaitAnimFinished
	Reverse
	WaitAnimWhilstChecking
endscript

script FlipGrabBlendBS Name = 'Kickflip to Melon'
	CheckForOllie
	ClearExtraGrindTrick
	KillExtraTricks
	if GotParam IsExtra
		LaunchExtraMessage
	endif
	PlayAnim Anim = KickflipBlendBS BlendPeriod = 0.3
	SetTrickName <Name>
	WaitAnim 15 frames
	Display
	Wait 10 frames
	PlayAnim Anim = MelonGrab From = 20 to = End BlendPeriod = 0.3 Speed = 1.4
	WaitAnimFinished
	Reverse
	DoNextTrick
	WaitAnimWhilstChecking
endscript

script FlipGrabBlend GrabStart = 10 GrabSpeed = 1.5
	CheckForOllie
	ClearManualTrick
	ClearExtraGrindTrick
	KillExtraTricks
	if GotParam IsExtra
		LaunchExtraMessage
	endif
	PlayAnim Anim = <Anim1> BlendPeriod = 0.3 From = Current
	SetTrickName <Name>
	SetTrickScore <Score>
	Display
	Wait 15 frames
	PlayAnim Anim = <Anim2> From = <GrabStart> to = End BlendPeriod = 0.3 Speed = <GrabSpeed>
	WaitAnim 10 frames fromend
	DoNextTrick
	WaitAnimFinished
	Reverse
	WaitAnimWhilstChecking
endscript

script CheckForOllie
	if GotException Ollied
		SetException Ex = Ollied Scr = CalledOllie CallInsteadOfGoto
	endif
endscript

script CalledOllie
	if AirTimeGreaterThan Skater_Late_Jump_Slop
		ClearException Ollied
	else
		#"Jump"
		IF #"Not" IsOptionOn LM_Control_bButtSlap
			ClearException Ollied
		ENDIF
	endif
endscript

SCRIPT Airtricks_ShowTrickName
	//the idea is to store trick menu name in Name param and show in game the ShowName param
	//if no showname param, means we can use just name
	IF GotParam ShowName
		SetTrickName <ShowName>
	ELSE
		SetTrickName <Name>
	ENDIF
	
	SetTrickScore <Score>
	Display
	
	//special sound has higher priority i suppose
	IF GotParam IsSpecial
		LaunchSpecialMessage
	ELSE
		IF GotParam IsExtra
			LaunchExtraMessage
		ENDIF
	ENDIF
ENDSCRIPT

SCRIPT Airtricks_SetExtraTricks
	if IsOptionOn LM_Control_bExtraTricks
		IF GotParam NewExtraTricks
			SetExtraTricks <NewExtraTricks> name = <name> Duration = 15
		ELSE
			IF GotParam ExtraTricks
				SetExtraTricks <ExtraTricks> name = <name> Duration = 15
			ENDIF
		ENDIF
	ELSE
		IF GotParam ExtraTricks
			SetExtraTricks <ExtraTricks> name = <name> Duration = 15
		ENDIF
	ENDIF
ENDSCRIPT
