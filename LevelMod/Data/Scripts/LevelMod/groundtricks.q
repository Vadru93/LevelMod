TRICK_PRELOAD_TIME = 160
ROTATEY_TRIGGER_TIME = 300

NoTricks = [ ]
Dpad = [ Up Down Left Right Upright UpLeft DownRight DownLeft ]

TRIGGER_MANUAL_BRANCHFLIP = { InOrder Square Square 200 }
EXTRA_BRANCHFLIP = { Trigger = TRIGGER_MANUAL_BRANCHFLIP Scr = ManualLink }

//changed taunts to dpad, cause original layout doesn't make any sense anyway.
GroundTricks = [
  { Trigger = { InOrder Down R2 , 300 } scr = ToggleSwitchRegular params = { PowerSlide } }
  { Trigger = { Press R2 20 } Scr = ToggleSwitchRegular }
  { Trigger = { Press L2 20 } Scr = ToggleNollieRegular }
  { Trigger = { TapOnceRelease Up X 300 } Scr = NoComply Params = { Name = 'No Comply' Score = 100 } }
  { Trigger = { HoldTwoAndPress R1 L1 Up 500 } Scr = Props }
  { Trigger = { HoldTwoAndPress R1 L1 Left 500 } Scr = Taunt Params = { Anim = Taunt1 PanelMessage = "Who's your daddy?" } }
  { Trigger = { HoldTwoAndPress R1 L1 Down 500 } Scr = Taunt Params = { Anim = Taunt2 PanelMessage = "No way" } }
  //{ Trigger = { HoldTwoAndPress R1 L1 Triangle 500 } Scr = Props }
  { Trigger = { HoldTwoAndPress R1 L1 Right 500 } Scr = Taunt Params = { Anim = Taunt3 PanelMessage = "Come get some" } }
  //{ Trigger = { HoldTwoAndPress R1 L1 Circle 500 } Scr = Taunt Params = { Anim = Taunt2 PanelMessage = "No way" } }
]

SpecialManualTricks = [
  { Trigger = { TripleInOrder Up Right Triangle 400 } Duration = 700 TrickSlot = SpMan_U_R_Triangle }
  { Trigger = { TripleInOrder Up Down Triangle 400 } Duration = 700 TrickSlot = SpMan_U_D_Triangle }
  { Trigger = { TripleInOrder Up Left Triangle 400 } Duration = 700 TrickSlot = SpMan_U_L_Triangle }
  { Trigger = { TripleInOrder Right Up Triangle 400 } Duration = 700 TrickSlot = SpMan_R_U_Triangle }
  { Trigger = { TripleInOrder Right Down Triangle 400 } Duration = 700 TrickSlot = SpMan_R_D_Triangle }
  { Trigger = { TripleInOrder Right Left Triangle 400 } Duration = 700 TrickSlot = SpMan_R_L_Triangle }
  { Trigger = { TripleInOrder Down Up Triangle 400 } Duration = 700 TrickSlot = SpMan_D_U_Triangle }
  { Trigger = { TripleInOrder Down Right Triangle 400 } Duration = 700 TrickSlot = SpMan_D_R_Triangle }
  { Trigger = { TripleInOrder Down Left Triangle 400 } Duration = 700 TrickSlot = SpMan_D_L_Triangle }
  { Trigger = { TripleInOrder Left Up Triangle 400 } Duration = 700 TrickSlot = SpMan_L_U_Triangle }
  { Trigger = { TripleInOrder Left Right Triangle 400 } Duration = 700 TrickSlot = SpMan_L_R_Triangle }
  { Trigger = { TripleInOrder Left Down Triangle 400 } Duration = 700 TrickSlot = SpMan_L_D_Triangle }
]

ManualTricks = [
  { Trigger = { InOrder Up Down 400 } Duration = 700 Trick_Manual }
  { Trigger = { InOrder Down Up 400 } Duration = 700 Trick_NoseManual }
]

GroundManualTricks = [
  { Trigger = { InOrder Up Down 220 } Trick_Manual }
  { Trigger = { InOrder Down Up 220 } Trick_NoseManual }
]

Reverts = [
  { Trigger = { Press R2 200 } TrickSlot = ExtraSlot1 }
  { Trigger = { Press L2 200 } TrickSlot = ExtraSlot2 }
]

Jumptricks0 = [ { Trigger = { TapOnceRelease Up X 300 } Scr = NoComply Params = { Name = 'No Comply' Score = 100 } } ]
Jumptricks = [ { Trigger = { TapTwiceRelease Up X 500 } TrickSlot = JumpSlot } ]

//changed to proper anims, originally all 3 are boneless
Trick_Boneless = { Scr = Boneless Params = { Name = 'Boneless' Score = 250 } }
Trick_Fastplant = { Scr = Boneless Params = { Name = 'Fastplant' Score = 250 Anim = Fastplant } }
Trick_Beanplant = { Scr = Boneless Params = { Name = 'Beanplant' Score = 250 Anim = Beanplant } }

SCR_MANUAL = { Scr = Manual }


//special manual tricks
TrickDef_Sproing = {
  Name = 'Sproing'
  Score = 1500
  InitAnim = Sproing
  FromAirAnim = Sproing
  BalanceAnim = Manual_Range
  OffMeterTop = ManualBail
  OffMeterBottom = ManualLand
  ExtraTricks = ManualBranches
  IsSpecial
  SproingFlip
}

Trick_Sproing = { SCR_MANUAL Params = TrickDef_Sproing }

Trick_ClawDrag = { SCR_MANUAL Params = { Name = 'The Claw Drag' UnlockFlag = SKATER_UNLOCKED_WOLVERINE OnlyWith = Wolverine Score = 1000 Sparks InitAnim = Clawdrag_Init FromAirAnim = Clawdrag_Init BalanceIdle = Clawdrag_Idle OffMeterTop = ManualBail OffMeterBottom = NoseManualBail IsSpecial ExtraTricks = FlatlandBranches } }

Trick_LazyAss = { SCR_MANUAL Params = { Name = 'Lazy Ass Manual' UnlockFlag = SKATER_UNLOCKED_DICK Score = 1500 InitAnim = LazyAss_Init FromAirAnim = LazyAss_Init BalanceIdle = LazyAss_Idle OutAnim = LazyAss_Out OffMeterTop = ManualBail OffMeterBottom = NoseManualBail Friction = 0.5 IsSpecial WaitOnOlliePercent = 10 ExtraTricks = FlatlandBranches } }

Trick_HandStand50Flip = { SCR_MANUAL Params = { Name = 'HandStand 360 Hand Flip' Score = 1400 InitAnim = HandStand50Flip_Init FromAirAnim = HandStand50Flip_Init BalanceAnim = HandStand50Flip_Range OutAnim = HandStand50Flip_Out OffMeterTop = ManualBail OffMeterBottom = NoseManualBail Friction = 0.5 IsSpecial WaitOnOlliePercent = 20 ExtraTricks = FlatlandBranches } }

Trick_HandstandDoubleFlip = { SCR_MANUAL Params = { Name = 'HandStand Double Flip' Score = 1400 InitAnim = HandstandHandFlip_Init FromAirAnim = HandstandHandFlip_Init BalanceAnim = HandstandHandFlip_Range OutAnim = HandStandDoubleFlip OffMeterTop = NoseManualBail OffMeterBottom = ManualBail Friction = 0.5 IsSpecial WaitOnOlliePercent = 40 ExtraTricks = FlatlandBranches ExtraTricks2 = HandstandBranches } }

Trick_ReemoSlide = { SCR_MANUAL Params = { Name = 'Reemo Slide' Score = 1300 InitAnim = ReemoSlide_Init FromAirAnim = ReemoSlide_Init BalanceAnim = ReemoSlide_Range OutAnim = ReemoSlide_out OffMeterTop = NoseManualBail OffMeterBottom = ManualBail Friction = 0.5 IsSpecial BoardRotate ExtraTricks = FlatlandBranches } }

Trick_PrimoSlide = { SCR_MANUAL Params = { Name = 'Primo' Score = 1200 InitAnim = Primo_Init FromAirAnim = Primo_Init BalanceAnim = Primo_Range OutAnim = Primo_out OffMeterTop = NoseManualBail OffMeterBottom = ManualBail Friction = 0.5 IsSpecial ExtraTricks = FlatlandBranches ExtraTricks2 = ToRail_Branches } }

Trick_OneFootOneWheel = { SCR_MANUAL Params = { Name = 'One Wheel Nosemanual' Score = 1400 InitAnim = OneFootOneWheel_Init FromAirAnim = OneFootOneWheel_Init BalanceAnim = OneFootOneWheel_Range OutAnim = OneFootOneWheel_Init OffMeterTop = ManualLand OffMeterBottom = NoseManualBail Friction = 0.5 IsSpecial Nollie ExtraTricks = NoseManualBranches PutDownAnim = PutDownOneWheel } }

Trick_ZZZZManual = { SCR_MANUAL Params = { Name = 'ZZZZ Manual' UnlockFlag = SKATER_UNLOCKED_BUM Score = 1400 InitAnim = ZZZZManual_init FromAirAnim = ZZZZManual_init BalanceIdle = ZZZZManual_idle OffMeterTop = ManualBail OffMeterBottom = NoseManualBail Friction = 0.5 IsSpecial ExtraTricks = FlatlandBranches } }

Trick_DanceParty = { SCR_MANUAL Params = { Name = 'Ahhh yeahhh!' Score = 1400 UnlockFlag = SKATER_UNLOCKED_CARRERA InitAnim = DanceParty_Init FromAirAnim = DanceParty_Init BalanceIdle = DanceParty_Idle OffMeterTop = ManualBail OffMeterBottom = NoseManualBail Friction = 20 IsSpecial ExtraTricks = FlatlandBranches } }

Trick_NinjaManual = { SCR_MANUAL Params = { Name = 'Sith Saber Spin' UnlockFlag = SKATER_UNLOCKED_MAUL OnlyWith = Maul Score = 1400 FlipFrame = 24 InitAnim = MaulNinjaManual_Init FromAirAnim = MaulNinjaManual_Init BalanceIdle = MaulNinjaManual_Idle OffMeterTop = ManualBail OffMeterBottom = NoseManualBail Friction = 0.5 IsSpecial BlendPeriod = 0.0 ExtraTricks = FlatlandBranches } }

Trick_DuckDive = { SCR_MANUAL Params = { Name = 'Duck Dive Shark Attack' UnlockFlag = SKATER_UNLOCKED_SLATER Score = 1400 InitAnim = DuckDive_Init FromAirAnim = DuckDive_Init BalanceIdle = DuckDive_Idle OutAnim = DuckDive_Out OffMeterTop = ManualBail OffMeterBottom = NoseManualBail IsSpecial WaitOnOlliePercent = 0 } }

Trick_VisitingHome = { SCR_MANUAL Params = { Name = 'Visiting Home' Score = 2000 UnlockFlag = SKATER_UNLOCKED_DEMONESS OnlyWith = Demoness InitAnim = VisitingHome_Init FromAirAnim = VisitingHome_Init BalanceIdle = VisitingHome_Idle OutAnim = VisitingHome_Out OffMeterTop = NoseManualBail OffMeterBottom = NoseManualBail IsSpecial WaitOnOlliePercent = 1 } }

Trick_WorkForFoodManual = { SCR_MANUAL Params = { Name = 'Can Ya Spare a Dime?' UnlockFlag = SKATER_UNLOCKED_BUM OnlyWith = Ollie Score = 1500 InitAnim = WorkForFood_Init FromAirAnim = WorkForFood_Init BalanceIdle = WorkForFood_Idle OffMeterTop = ManualBail OffMeterBottom = NoseManualBail Friction = 0.5 IsSpecial ExtraTricks = FlatlandBranches } }




TrickDef_Manual = {
  Name = 'Manual'
  Score = 100
  InitAnim = Manual
  FromAirAnim = ManualFromAir
  BalanceAnim = Manual_Range
  OffMeterTop = ManualBail
  OffMeterBottom = ManualLand
  Manual_Branchlinks
}

Trick_Manual = { SCR_MANUAL Params = TrickDef_Manual }

TrickDef_NoseManual = {
  Name = 'Nose Manual'
  Score = 100
  InitAnim = Nosemanual
  FromAirAnim = NoseManualFromAir
  BalanceAnim = NoseManual_Range
  Nollie
  OffMeterTop = ManualLand
  OffMeterBottom = NoseManualBail
  NoseManual_Branchlinks
}

Trick_NoseManual = { SCR_MANUAL Params = TrickDef_NoseManual }

NoseManualBranches = [ { Trigger = { Press R2 200 } Trick_Gturn } ]
ManualBranches = [ { Trigger = { Press R2 200 } Trick_Gturn2 } ]

//original unused?
//ManualBranches2 = [ { Trigger = { Press R2 200 } Trick_Gturn3 } ]
//NoseManualBranches2 = [ { Trigger = { Press R2 200 } Trick_Gturn4 } ]

TrickDef_NosePivot = {
  Name = 'Nose Pivot'
  Score = 250
  Anim = GTurn
  BalanceAnim = Manual
  FlipAfter
  BalanceAnim = Manual_Range
  Trickslack = 0
  PlayCessSound
  Extrapercent = 100
  TimeAdd = 2
  SpeedMult = 1.2
  OffMeterTop = ManualBail
  OffMeterBottom = ManualLand
  Manual_Branchlinks
}

Trick_Gturn = { Trigger = { Press R2 200 } Scr = ManualLink Params = TrickDef_NosePivot }

TrickDef_Pivot = {
  Name = 'Pivot'
  Score = 250
  Anim = GTurn2
  BalanceAnim = Manual
  FlipAfter
  BalanceAnim = NoseManual_Range
  Trickslack = 0
  PlayCessSound
  Extrapercent = 100
  Nollie
  TimeAdd = 2
  SpeedMult = 1.2
  OffMeterTop = ManualLand
  OffMeterBottom = NoseManualBail
  NoseManual_Branchlinks
}

Trick_Gturn2 = { Trigger = { Press R2 200 } Scr = ManualLink Params = TrickDef_Pivot }

Manual_Branchlinks = {
  ExtraTricks = ManualBranches
  NewExtraTricks = ManualBranches
  NewExtraTricks2 = NewFlatlandBranches
}

NoseManual_Branchlinks = {
  ExtraTricks = NoseManualBranches
  NewExtraTricks = NoseManualBranches
  NewExtraTricks2 = NewFlatlandBranches
}

TrickDef_AntiCasper = {
  Name = 'Anti Casper'
  Score = 1350
  InitAnim = AntiCasper_Init
  FromAirAnim = AntiCasper_Init
  BalanceAnim = AntiCasper_Range
  OutAnim = AntiCasper_out
  OffMeterTop = ManualLand
  OffMeterBottom = NoseManualBail
  Friction = 0.5
  BoardRotate
  Nollie
  ExtraTricks = FlatlandBranches
  NewExtraTricks = NewFlatlandBranches
}

Trick_AntiCasper = { SCR_MANUAL Params = { TrickDef_AntiCasper IsSpecial } }



TrickDef_Casper = {
  Name = 'Casper'
  Score = 1250
  InitAnim = Casper_Init
  FromAirAnim = Casper_Init
  BalanceAnim = Casper_Range
  OutAnim = Casper_out
  OffMeterTop = ManualBail
  OffMeterBottom = ManualLand
  Friction = 0.5
  BoardRotate
  Casper_Branchlinks
}

Trick_Casper = { SCR_MANUAL Params = { TrickDef_Casper IsSpecial } }

CasperBranches = [ { Trigger = { AirTrickLogic Left Square 500 } Scr = ManualLink Params = { TrickDef_CasperFlip IsExtra } } ]
NewCasperBranches = [ { EXTRA_BRANCHFLIP Params = { TrickDef_CasperFlip IsExtra } } ]

TrickDef_CasperFlip = {
  Name = 'Casper Flip'
  Score = 500
  Extrapercent = 100
  Anim = CasperFlip
  BalanceAnim = Casper_Range
  OutAnim = Casper_out
  Trickslack = 0
  BoardRotate
  Parent = 'Casper'
  Casper_Branchlinks
}

Casper_Branchlinks = {
  ExtraTricks = FlatlandBranches
  ExtraTricks2 = CasperBranches
  NewExtraTricks = NewFlatlandBranches
  NewExtraTricks2 = NewCasperBranches
}



TrickDef_Handstand = {
  Name = 'HandStand'
  Score = 1250
  InitAnim = HandstandHandFlip_Init
  FromAirAnim = HandstandHandFlip_Init
  BalanceAnim = HandstandHandFlip_Range
  OutAnim = HandstandHandFlip_out
  OffMeterTop = NoseManualBail
  OffMeterBottom = ManualBail
  Friction = 0.5
  WaitOnOlliePercent = 10
  Handstand_Branchlinks
}

Trick_Handstand = { SCR_MANUAL Params = { TrickDef_Handstand IsSpecial } }

HandstandBranches = [ { Trigger = { AirTrickLogic Left Square 500 } Scr = ManualLink Params = { TrickDef_Handflip IsExtra } } ]
NewHandstandBranches = [ { EXTRA_BRANCHFLIP Params = { TrickDef_Handflip IsExtra } } ]

TrickDef_Handflip = {
  Name = 'Handflip'
  Score = 500
  Anim = HStandFliptoHStand
  BalanceAnim = HandstandHandFlip_Range
  OutAnim = HandstandHandFlip_out
  Trickslack = 0
  Parent = 'HandStand'
  Handstand_Branchlinks
}

Handstand_Branchlinks = {
  ExtraTricks = FlatlandBranches
  ExtraTricks2 = HandstandBranches
  NewExtraTricks = NewFlatlandBranches
  NewExtraTricks2 = NewHandstandBranches
}




TrickDef_ToRail = {
  Name = 'To Rail'
  Score = 500
  InitAnim = Primo_Init
  FromAirAnim = Primo_Init
  BalanceAnim = Primo_Range
  OutAnim = Primo_out
  OffMeterTop = NoseManualBail
  OffMeterBottom = ManualBail
  Friction = 1.0
  ToRail_Branchlinks
}

Trick_ToRail = { SCR_MANUAL Params = { TrickDef_ToRail IsSpecial } }

ToRail_Branches = [ { Trigger = { AirTrickLogic Left Square 500 } Scr = ManualLink Params = { TrickDef_RailFlip IsExtra } } ]
NewToRail_Branches = [ { EXTRA_BRANCHFLIP Params = { TrickDef_RailFlip IsExtra } } ]

TrickDef_RailFlip = {
  Name = 'Rail Flip'
  Score = 500
  Anim = RailFlip
  BalanceAnim = Primo_Range
  Trickslack = 0
  Parent = 'To Rail'
  ToRail_Branchlinks
}

ToRail_Branchlinks = {
  ExtraTricks = FlatlandBranches
  ExtraTricks2 = ToRail_Branches
  NewExtraTricks = NewFlatlandBranches
  NewExtraTricks2 = NewToRail_Branches
}


TrickDef_Truckstand = {
  Name = 'Truckstand'
  Score = 1300
  InitAnim = Truckstand_Init
  FromAirAnim = Truckstand_Init
  BalanceAnim = Truckstand_Range
  OutAnim = Truckstand_Out
  OffMeterTop = NoseManualBail
  OffMeterBottom = ManualBail
  Friction = 1
  Truckstand_BranchLinks
}

Trick_Truckstand = { SCR_MANUAL Params = { TrickDef_Truckstand IsSpecial } }

TruckstandBranches = [
  { Trigger = { AirTrickLogic Left Square 500 } Scr = ManualLink Params = TrickDef_TruckstandFlip }
  { Trigger = { TripleInOrder Right Right Triangle 500 } Scr = ManualLink Params = { Name = 'Truck Spin' Score = 1000 Anim = Truckstand_Init BalanceAnim = Truckstand_Range OutAnim = Truckstand_Out ExtraTricks = FlatlandBranches NewExtraTricks = NewFlatlandBranches ExtraTricks2 = TruckstandBranches Trickslack = 0 IsExtra } }
  { Trigger = { HoldTwoAndPress R2 L2 Triangle 500 } Scr = ManualLink Params = { Name = 'Pogo to Hair Flip' RodneyOnly Score = 1500 Anim = Sweet BalanceAnim = Truckstand_Range OutAnim = Truckstand_Out ExtraTricks = FlatlandBranches NewExtraTricks = NewFlatlandBranches ExtraTricks2 = TruckstandBranches Trickslack = 0 IsExtra } }
]

NewTruckstandBranches = [
  { EXTRA_BRANCHFLIP Params = { TrickDef_TruckstandFlip IsExtra } }
]

TrickDef_TruckstandFlip = {
  Name = 'Truckstand Flip'
  Score = 500
  Anim = Truckstand_Flip
  BalanceAnim = Truckstand_Range
  OutAnim = Truckstand_Out
  ExtraTricks = FlatlandBranches
  NewExtraTricks = NewFlatlandBranches
  ExtraTricks2 = TruckstandBranches
  Trickslack = 0
  Parent = 'Truckstand'
  Truckstand_BranchLinks
}

Truckstand_BranchLinks = {
  ExtraTricks = FlatlandBranches
  ExtraTricks2 = TruckstandBranches
  NewExtraTricks = NewFlatlandBranches
  NewExtraTricks2 = NewTruckstandBranches
}

//original struct
FlatlandBranches = [
  { Trigger = { TripleInOrder Left Right Triangle 500 } Trick_AntiCasper }
  { Trigger = { TripleInOrder Right Left Triangle 500 } Trick_Casper }
  { Trigger = { TripleInOrder Left Left Triangle 500 } Trick_ToRail }
  { Trigger = { TripleInOrder Right Right Triangle 500 } Trick_Truckstand }
  { Trigger = { TripleInOrder Right Right Circle 500 } Trick_Handstand }
]

//new struct
NewFlatlandBranches = [
  { Trigger = { InOrder Square Circle 300 } Trick_AntiCasper }
  { Trigger = { InOrder Square Triangle 300 } Trick_Casper }
  { Trigger = { InOrder Circle Circle 300 } Trick_Handstand }
  { Trigger = { InOrder Circle Square 300 } Trick_ToRail }
  //{ Trigger = { InOrder Circle Triangle 300 } Trick_OneFootManual }
  //{ Trigger = { InOrder Circle Triangle 300 } Trick_OneFootNosemanual }
  //{ Trigger = { InOrder Triangle Triangle 300 } Trick_Pogo }
  //{ Trigger = { InOrder Triangle Circle 300 } Trick_SwitchFootPogo }
  { Trigger = { InOrder Triangle Square 300 } Trick_Truckstand }
]









SCRIPT Manual BlendPeriod = 0.3
  ClearLipCombos
  ClearExceptions
  KillExtraTricks

  SetException Ex = GroundGone Scr = GroundGone Params = { NoBoneless }
  SetException Ex = MadeOtherSkaterBail Scr = MadeOtherSkaterBail_Called CallInsteadOfGoto

  IF GotParam Nollie
    SetException Ex = Ollied Scr = NollieNoDisplay Params = { <...> }
    NollieOn
  ELSE
    SetException Ex = Ollied Scr = Ollie Params = { <...> }
  ENDIF

  SetException Ex = FlailHitWall Scr = FlailHitWall
  SetException Ex = FlailLeft Scr = FlailLeft
  SetException Ex = FlailRight Scr = FlailRight
  SetException Ex = OffMeterTop Scr = <OffMeterTop> Params = { <...> }
  SetException Ex = OffMeterBottom Scr = <OffMeterBottom> Params = { <...> }
  SetException Ex = Carplant Scr = Carplant
  SetException Ex = CarBail Scr = CarBail
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  OnExceptionRun ManualOut

  SetQueueTricks NoTricks

  IF GotParam IsSpecial
    SetManualTricks NoTricks
  ELSE
    IF GotParam IsExtra
      SetManualTricks NoTricks
    ELSE
      SetManualTricks NoTricks Special = SpecialManualTricks
    ENDIF
  ENDIF

  StartBalanceTrick
  Vibrate Actuator = 1 Percent = 25

  IF GotParam Friction
    SetRollingFriction <Friction>
  ENDIF

  IF GotParam IsSpecial
    IF GotParam Nollie
      DoBalanceTrick ButtonA = Up ButtonB = Down Type = Nosemanual Tweak = 5
    ELSE
      DoBalanceTrick ButtonA = Up ButtonB = Down Type = Manual Tweak = 5
    ENDIF
  ELSE
    IF GotParam Nollie
      DoBalanceTrick ButtonA = Up ButtonB = Down Type = Nosemanual Tweak = 1
    ELSE
      DoBalanceTrick ButtonA = Up ButtonB = Down Type = Manual Tweak = 1
    ENDIF
  ENDIF

  IF GotParam FromAir
    PlayAnim Anim = <FromAirAnim> BlendPeriod = 0.0
  ELSE
    PlayAnim Anim = <InitAnim> BlendPeriod = 0.3
  ENDIF

  IF Obj_FlagSet FLAG_SKATER_MANUALCHEESE
    IF AirTimeLessThan 1 second
      AdjustBalance TimeAdd = 2 SpeedMult = 1.2
      //some debug message?
      //IF CD
      //ELSE
      //  LaunchSpecialMessage text = "Cheeseball"
      //ENDIF
    ENDIF
  ELSE
    Obj_SetFlag FLAG_SKATER_MANUALCHEESE
  ENDIF

  IF SpeedGreaterThan 300
    Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
  ENDIF

  WaitAnim 50 Percent
  IF GotParam IsSpecial
  ELSE
    IF GotParam IsExtra
    ELSE
      DoNextManualTrick
    ENDIF
  ENDIF

  IF GotParam IsSpecial
    LaunchSpecialMessage
  ELSE
    IF GotParam IsExtra
      LaunchExtraMessage
    ENDIF
  ENDIF

  GroundTricks_AddExtraManualTricks ignore = <Name> <...>

  IF AnimEquals WorkForFood_Init
    SpawnSkaterScript Ollie_Signage
  ENDIF

  WaitAnimFinished

  SetTrickName <Name>
  SetTrickScore <Score>
  Display Blockspin

  IF GotParam SproingFlip
    BlendPeriodOut 0
    FlipAfter
  ENDIF

  IF GotParam BalanceIdle
    PlayAnim Anim = <BalanceIdle> cycle BlendPeriod = <BlendPeriod>
  ELSE
    PlayAnim Anim = <BalanceAnim> wobble
  ENDIF

  IF AnimEquals VisitingHome_Idle
    SwitchOnAtomic special_item
    SwitchOffAtomic skin
    SwitchOffBoard
    Obj_Attachframe bone = "right_fingers"
    Obj_Attachframe bone = "left_fingers"
    BEGIN
      BloodParticlesOn Name = "breath.png" start_col = 1426128895 end_col = 153 blend_mode = 72 num = 25 emit_w = 1.0 emit_h = 1.0 angle = 10 size = 40.0 bone = "right_fingers" growth = 0 time = 5 speed = 40 grav = 0 life = 3
      BloodParticlesOn Name = "breath.png" start_col = 1429221424 end_col = 4671303 num = 25 emit_w = 2.0 emit_h = 2.0 angle = 20 size = 20.0 bone = "left_fingers" growth = 1.5 time = 5 speed = 50 grav = 0 life = 3
      Wait 0.5 second
    REPEAT
  ENDIF

  IF GotParam Sparks
    BEGIN
      ClawSparks
      Wait 0.5 frames
    REPEAT
  ENDIF

  IF AnimEquals LazyAss_Idle
    BEGIN
      IF frameis 20
        PlaySound bitchslap2
      ENDIF
      Wait 1 Frame
    REPEAT
  ENDIF

  IF AnimEquals MaulNinjaManual_Idle
    BEGIN
      IF Flipped
        PlayAnim Anim = CessSlide180_FS
        PlayCessSound
        WaitAnimFinished
        Flip
        PlayAnim Anim = <BalanceIdle> cycle BlendPeriod = <BlendPeriod>
      ENDIF
      IF frameis 1
        PlaySound sabertwirl5
      ENDIF
      Wait 1 Frame
    REPEAT
  ENDIF
ENDSCRIPT

SCRIPT SpecialManualSFX
ENDSCRIPT

claw_sparks_partice_params = {
  Name = "Grass_1.png"
  start_col = -16711681
  end_col = -2013265665
  blend_mode = 68
  num = 10
  emit_w = 2.0
  emit_h = 2.1
  angle = -50
  size = 1.0
  growth = 3
  time = 0.25
  speed = 150
  grav = 0
  life = 0.25
}

SCRIPT ClawSparks
  IF ProfileEquals is_named = Wolverine
    Obj_Attachframe bone = "left_fingers"
    Obj_Attachframe bone = "right_fingers"
    BloodParticlesOn { claw_sparks_partice_params bone = "left_fingers" }
    BloodParticlesOn { claw_sparks_partice_params bone = "right_fingers" }
  ENDIF
ENDSCRIPT

SCRIPT ManualOut
  IF GotParam MadeOtherSkaterBail
  ELSE
    KillExtraTricks
  ENDIF
  IF ProfileEquals is_named = Demoness
    SwitchOffAtomic special_item
    SwitchOnAtomic skin
    SwitchOnBoard
  ENDIF
ENDSCRIPT

SCRIPT ManualLand
  SetException Ex = RunHasEnded Scr = EndOfRun
  ClearPanel_Landed
  ClearManualTrick
  ClearEventBuffer
  ClearTrickQueue
  KillExtraTricks
  IF GotParam PutDownAnim
    PlayAnim Anim = <PutDownAnim> BlendPeriod = 0.0
  ELSE
    IF GotParam Nollie
      PlayAnim Anim = PutDownNosemanual BlendPeriod = 0.0
    ELSE
      PlayAnim Anim = PutDownManual BlendPeriod = 0.0
    ENDIF
  ENDIF
  WaitAnimWhilstChecking
  Goto OnGroundAI
ENDSCRIPT


SCRIPT ManualLink grindslack = 25 Trickslack = 10 displaypercent = 50 Extrapercent = 50 TimeAdd = 0 SpeedMult = 1
  IF GotParam RodneyOnly
    IF ProfileEquals is_named = mullen
    ELSE
      Goto ManualLink Params = { Name = 'Truck Spin' Score = 1000 Anim = Truckstand_Init BalanceAnim = Truckstand_Range OutAnim = Truckstand_Out ExtraTricks = FlatlandBranches ExtraTricks2 = TruckstandBranches Trickslack = 0 IsExtra }
    ENDIF
  ENDIF
  IF GotParam Nollie
    NollieOn
    SetException Ex = Ollied Scr = NollieNoDisplay
  ELSE
    NollieOff
    SetException Ex = Ollied Scr = Ollie
  ENDIF
  IF GotParam OffMeterTop
    SetException Ex = OffMeterTop Scr = <OffMeterTop> Params = { <...> }
    SetException Ex = OffMeterBottom Scr = <OffMeterBottom> Params = { <...> }
  ENDIF
  KillExtraTricks
  OnExceptionRun ManualOut
  BailOn
  SetTrickName <Name>
  SetTrickScore <Score>
  AdjustBalance TimeAdd = <TimeAdd> SpeedMult = <SpeedMult>
  PlayAnim Anim = <Anim> BlendPeriod = 0.3
  IF GotParam FlipAfter
    FlipAfter
    BlendPeriodOut 0
    BoardRotateAfter
  ENDIF
  Wait 5 frames
  IF GotParam PlayCessSound
    PlayCessSound
  ENDIF

  IF GotParam IsSpecial
    LaunchSpecialMessage
  ELSE
    IF GotParam IsExtra
      LaunchExtraMessage
    ENDIF
  ENDIF

  WaitAnim <displaypercent>

  IF GotParam NoDisplay
  ELSE
    Display
  ENDIF

  WaitAnim <Extrapercent> Percent

  GroundTricks_AddExtraManualTricks ignore = <Parent> <...>

  WaitAnim <grindslack> frames fromend
  Bailoff
  WaitAnim <Trickslack> frames fromend
  WaitAnimFinished
  IF GotParam BalanceIdle
    PlayAnim Anim = <BalanceIdle> cycle
  ELSE
    PlayAnim Anim = <BalanceAnim> wobble
  ENDIF
  IF AnimEquals LazyAss_Idle
    BEGIN
      IF frameis 20
        PlaySound bitchslap2
      ENDIF
      WaitoneGameFrame
    REPEAT
  ENDIF
ENDSCRIPT

//the logic is that we store 2 sets of extra tricks: original and new
//first we assign old tricks
SCRIPT GroundTricks_AddExtraManualTricks
  IF GotParam ExtraTricks
    IF GotParam ExtraTricks2
      SetExtraTricks <ExtraTricks2> <ExtraTricks> ignore = <ignore>
    ELSE
      SetExtraTricks <ExtraTricks> ignore = <ignore>
    ENDIF
  ENDIF

  //if new tricks are allowed, we assign new extensions
  //if new ones are missing, then old tricks are in effect
  //should we have a separate option here? now uses extra tricks toggle

  IF IsOptionOn LM_Control_bExtraTricks
    IF GotParam NewExtraTricks
      IF GotParam NewExtraTricks2
        SetExtraTricks <NewExtraTricks2> <NewExtraTricks> ignore = <ignore>
      ELSE
        SetExtraTricks <NewExtraTricks> ignore = <ignore>
      ENDIF
    ENDIF
  ENDIF
ENDSCRIPT


SCRIPT Revert FSName = 'FS Revert' BSName = 'BS Revert' FSAnim = RevertFS BSAnim = RevertBS
  ClearLipCombos
  KillExtraTricks
  SetTrickScore 100
  IF InSplitScreenGame
  ELSE
    IF IsOptionOn LM_GUI_bTrickNotifications
      LaunchPanelMessage "Landing Trick" properties = panelcombo
    ENDIF
  ENDIF
  OnGroundExceptions_NoEndRun
  SetQueueTricks NoTricks
  SetManualTricks Special = SpecialManualTricks ManualTricks
  Vibrate Actuator = 0 Percent = 80 Duration = 0.5
  Vibrate Actuator = 1 Percent = 80 Duration = 0.1
  NollieOff
  PlayCessSound
  IF Obj_FlagSet FLAG_SKATER_REVERTFS
    Obj_ClearFlag FLAG_SKATER_REVERTFS
    PlayAnim Anim = <FSAnim>
    SetTrickName <FSName>
  ELSE
    IF Obj_FlagSet FLAG_SKATER_REVERTBS
      Obj_ClearFlag FLAG_SKATER_REVERTBS
      PlayAnim Anim = <BSAnim>
      SetTrickName <BSName>
    ELSE
      IF LastSpinWas Frontside
        PlayAnim Anim = <FSAnim>
        SetTrickName <FSName>
      ELSE
        PlayAnim Anim = <BSAnim>
        SetTrickName <BSName>
      ENDIF
    ENDIF
  ENDIF
  Display Blockspin
  FlipAfter
  BoardRotateAfter
  BlendPeriodOut 0.0
  SetSpecialFriction [ 0 10 20 30 50 ]
  BEGIN
    IF AnimFinished
      BREAK
    ENDIF
    WaitoneGameFrame
  REPEAT
  SetRollingFriction #"default"
  DoNextManualTrick FromAir
  OnGroundExceptions
  CheckGapTricks
  ClearPanel_Landed
  ClearEventBuffer
  ClearManualTrick
  IF Crouched
    PlayAnim Anim = CrouchIdle BlendPeriod = 0.3
  ELSE
    PlayAnim Anim = CrouchIdle BlendPeriod = 0.3
  ENDIF
  WaitAnimWhilstChecking AndManuals
  Goto OnGroundAI
ENDSCRIPT

SCRIPT ToggleSwitchRegular
  OnGroundExceptions
  SetException Ex = Ollied Scr = Ollie Params = { NoDoNextTrick }
  Vibrate Actuator = 0 Percent = 80 Duration = 0.5
  Vibrate Actuator = 1 Percent = 80 Duration = 0.1
  NollieOff
  PlayCessSound
  DoNextTrick
  //add toggle later
  //if LM_Control_bPowerslide
    if GotParam PowerSlide
      Rotate y = 180 duration = 0.3 seconds
    endif
  //endif
  IF LeftPressed
    IF Flipped
      PlayAnim Anim = CessSlide180_FS
    ELSE
      PlayAnim Anim = CessSlide180_BS
    ENDIF
  ELSE
    IF RightPressed
      IF Flipped
        PlayAnim Anim = CessSlide180_BS
      ELSE
        PlayAnim Anim = CessSlide180_FS
      ENDIF
    ELSE
      PlayAnim Anim = CessSlide180_FS
    ENDIF
  ENDIF
  WaitAnim 50 Percent
  FlipAfter
  BoardRotateAfter
  BlendPeriodOut 0.0
  WaitAnimWhilstChecking AndManuals
  IF CanKick
    PlayAnim Anim = PushCycle cycle BlendPeriod = 0.0
  ELSE
    IF AnimFinished
      PlayAnim Anim = Idle cycle BlendPeriod = 0.0
    ENDIF
  ENDIF
  ClearTrickQueue
  Goto OnGroundAI
ENDSCRIPT

SCRIPT ToggleNollieRegular
  ClearExceptions
  SetException Ex = GroundGone Scr = GroundGone
  SetException Ex = Ollied Scr = Nollie Params = { NoDoNextTrick }
  SetException Ex = OntoRail Scr = OntoRail
  SetException Ex = FlailHitWall Scr = FlailHitWall
  SetException Ex = FlailLeft Scr = FlailLeft
  SetException Ex = FlailRight Scr = FlailRight
  SetException Ex = Carplant Scr = Carplant
  SetException Ex = CarBail Scr = CarBail
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  SetQueueTricks GroundTricks
  DoNextTrick

  IF InNollie
    NollieOff
    IF Crouched
      PlayAnim Anim = CrouchToNollie BlendPeriod = 0.1 Backwards
    ELSE
      PlayAnim Anim = SkatingToNollie BlendPeriod = 0.1 Backwards
    ENDIF
  ELSE
    NollieOn
    IF Crouched
      PlayAnim Anim = CrouchToNollie BlendPeriod = 0.1
    ELSE
      PlayAnim Anim = SkatingToNollie BlendPeriod = 0.1
    ENDIF
  ENDIF

  WaitAnimWhilstChecking AndManuals

  //fixed original bug where it wouldn't go back to regular
  IF InNollie
    Goto OnGroundNollieAI
  ELSE
    Goto OnGroundAI
  ENDIF
ENDSCRIPT

SCRIPT Nollie
  IF GotParam NoDoNextTrick
  ELSE
    DoNextTrick
  ENDIF
  ClearTrickQueue
  ClearEventBuffer Buttons = Dpad OlderThan = TRICK_PRELOAD_TIME
  #"Jump"
  InAirExceptions
  Vibrate Actuator = 1 Percent = 50 Duration = 0.05
  NollieOn
  PlayAnim Anim = Nollie BlendPeriod = 0.1
  IF ProfileEquals stance = regular
    IF Flipped
      SetTrickName 'Nollie'
    ELSE
      SetTrickName 'Fakie Ollie'
    ENDIF
  ELSE
    IF Flipped
      SetTrickName 'Fakie Ollie'
    ELSE
      SetTrickName 'Nollie'
    ENDIF
  ENDIF
  SetTrickScore 200
  Display
  Bailoff
  WaitAnimWhilstChecking
  Goto Airborne
ENDSCRIPT

SCRIPT NollieNoDisplay
  ClearTrickQueue
  ClearEventBuffer Buttons = Dpad OlderThan = TRICK_PRELOAD_TIME
  SetTrickName 'Nollie'
  SetTrickScore 100
  Display Deferred
  #"Jump"
  InAirExceptions
  Vibrate Actuator = 1 Percent = 80 Duration = 0.05
  NollieOn
  IF GotParam OutAnim
    PlayAnim Anim = <OutAnim> BlendPeriod = 0.3
  ELSE
    PlayAnim Anim = Nollie BlendPeriod = 0.1
  ENDIF
  IF GotParam BoardRotate
    BlendPeriodOut 0
    BoardRotateAfter
  ENDIF
  Bailoff
  WaitAnimWhilstChecking
  Goto Airborne
ENDSCRIPT


SCRIPT Ollie
  IF GotParam NoDoNextTrick
  ELSE
    DoNextTrick
  ENDIF
  SetTrickName 'Ollie'
  SetTrickScore 100
  Display Deferred
  ClearTrickQueue
  ClearEventBuffer Buttons = Dpad OlderThan = TRICK_PRELOAD_TIME
  #"Jump"
  InAirExceptions
  Vibrate Actuator = 1 Percent = 50 Duration = 0.05
  IF GotParam FromLip
    printf "came from lip=================="
    PlayAnim Anim = <OutAnim> BlendPeriod = 0.0
    BlendPeriodOut 0
  ELSE
    IF GotParam OutAnim
      PlayAnim Anim = <OutAnim> BlendPeriod = 0.3
    ELSE
      PlayAnim Anim = Ollie BlendPeriod = 0.0 NoRestart
    ENDIF
  ENDIF
  IF GotParam BoardRotate
    BlendPeriodOut 0
    BoardRotateAfter
  ENDIF
  IF GotParam RotateAfter
    RotateAfter
  ENDIF
  IF GotParam FlipAfter
    printf "=========flipping the skater in Ollie"
    FlipAfter
  ENDIF
  IF GotParam WaitOnOlliePercent
    BailOn
    WaitAnim <WaitOnOlliePercent> Percent
  ENDIF
  Bailoff
  WaitAnimWhilstChecking
  Goto Airborne
ENDSCRIPT

SCRIPT NoComply
  ClearTrickQueue
  ClearEventBuffer Buttons = Dpad OlderThan = TRICK_PRELOAD_TIME
  #"Jump"
  InAirExceptions
  Vibrate Actuator = 1 Percent = 80 Duration = 0.05
  IF GotParam Name
    SetTrickName <Name>
    SetTrickScore <Score>
    Display
  ENDIF
  Bailoff
  PlayAnim Anim = NoComply BlendPeriod = 0.2
  WaitAnimWhilstChecking
  Goto Airborne
ENDSCRIPT

SCRIPT Boneless Anim = Boneless Height = BonelessHeight
  ClearTrickQueue
  ClearEventBuffer Buttons = Dpad OlderThan = TRICK_PRELOAD_TIME
  Jump <Height>
  InAirExceptions
  Vibrate Actuator = 1 Percent = 80 Duration = 0.1
  PlaySound boneless09 pitch = 85
  IF GotParam Name
    SetTrickName <Name>
    SetTrickScore <Score>
    Display
  ENDIF
  PlayAnim Anim = <Anim> BlendPeriod = 0.2
  Bailoff
  WaitAnimWhilstChecking
  Goto Airborne
ENDSCRIPT

SCRIPT BonelessWaitAnimWhilstChecking
  BEGIN
    DoNextTrick
    IF GotParam AndManuals
      DoNextManualTrick
    ENDIF
    IF AnimFinished
      BREAK
    ENDIF
    WaitoneGameFrame
    IF OnGround
      Goto Land
    ENDIF
  REPEAT
ENDSCRIPT

SCRIPT Taunt
  OnGroundExceptions
  PlayAnim Anim = <Anim> BlendPeriod = 0.3
  LaunchPanelMessage <PanelMessage>
  IF SpeedLessThan 1
    SetRollingFriction 100
    WaitAnimWhilstChecking AndManuals
    Goto Handbrake
  ELSE
    WaitAnimWhilstChecking AndManuals
    Goto OnGroundAI
  ENDIF
ENDSCRIPT

SCRIPT Props message1 = "Props"
  OnGroundExceptions
  PlayAnim Anim = random( @Prop @Cheer1 ) BlendPeriod = 0.3
  LaunchPanelMessage <message1>
  IF SpeedLessThan 1
    SetRollingFriction 100
    WaitAnimWhilstChecking AndManuals
    Goto Handbrake
  ELSE
    WaitAnimWhilstChecking AndManuals
    Goto OnGroundAI
  ENDIF
ENDSCRIPT

//SCRIPT Taunt1
//  OnGroundExceptions
//  PlayAnim Anim = Taunt1 NoRestart BlendPeriod = 0.3
//  LaunchPanelMessage "Who's your Daddy?"
//  WaitAnimWhilstChecking AndManuals
//  Goto OnGroundAI
//ENDSCRIPT

//SCRIPT Taunt2
//  OnGroundExceptions
//  PlayAnim Anim = Taunt2 NoRestart BlendPeriod = 0.3
//  LaunchPanelMessage "No F-ckin' way"
//  WaitAnimWhilstChecking AndManuals
//  Goto OnGroundAI
//ENDSCRIPT

//SCRIPT Taunt3
//  OnGroundExceptions
//  PlayAnim Anim = Taunt3 NoRestart BlendPeriod = 0.3
//  LaunchPanelMessage "Come get some"
//  WaitAnimWhilstChecking AndManuals
//  Goto OnGroundAI
//ENDSCRIPT

SCRIPT BitchSlap
  OnGroundExceptions
  PlayAnim Anim = Slapright NoRestart BlendPeriod = 0.3
  WaitAnimWhilstChecking AndManuals
  Goto OnGroundAI
ENDSCRIPT
