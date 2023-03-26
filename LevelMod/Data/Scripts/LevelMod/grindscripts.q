
script SkateInOrBail
  OnGroundExceptions_NoEndRun
  ClearException GroundGone
  OnExceptionRun SkateInOrBail_Out
  SetQueueTricks NoTricks
  ClearManualTrick
  if GotParam FallingRight
    if SkateInAble Right
      SetLandedFromVert
      SetState ground
      Move x = -1
      Move y = -5
      OrientToNormal
      Rotate y = -30 Duration = 0.2 seconds
      OnGroundExceptions_NoEndRun
      ClearException GroundGone
      SetQueueTricks NoTricks
      SetManualTricks NoTricks
      SetExtraTricks tricks = Reverts Duration = 20
      if GotParam OutAnim
        PlayAnim Anim = <OutAnim> BlendPeriod = 0.3
      else
        PlayAnim Anim = <initanim> Backwards BlendPeriod = 0.3
      endif
      WaitAnimWhilstChecking
      goto SkateInLand
    else
      goto <GrindBail>
    endif
  endif
  if GotParam FallingLeft
    if SkateInAble Left
      SetLandedFromVert
      printf "ground......................."
      SetState ground
      Move x = 1
      Move y = -5
      OrientToNormal
      Rotate y = 30 Duration = 0.2 seconds
      SetState ground
      OnGroundExceptions_NoEndRun
      ClearException GroundGone
      SetQueueTricks NoTricks
      SetManualTricks NoTricks
      SetExtraTricks tricks = Reverts Duration = 20
      if GotParam OutAnim
        PlayAnim Anim = <OutAnim> BlendPeriod = 0.3
      else
        PlayAnim Anim = <initanim> Backwards BlendPeriod = 0.3
      endif
      WaitAnimWhilstChecking
      goto SkateInLand
    else
      goto <GrindBail>
    endif
  endif
  printf "Missing a FallingLeft or FallingRight ?"
  goto <GrindBail>
endscript

script SkateInLand
  OnExceptionRun SkateInLandOut
  NollieOff
  Vibrate Actuator = 1 Percent = 80 Duration = 0.1
  if Crouched
    PlayAnim Anim = CrouchedLand BlendPeriod = 0.1
  else
    PlayAnim Anim = Land BlendPeriod = 0.1
  endif
  ClearTrickQueue
  OnExceptionRun SkateInLandOut
  CheckGapTricks
  ClearGapTricks
  OnGroundExceptions_NoEndRun
  WaitAnim 10 Percent
  ClearPanel_Landed
  OnGroundExceptions
  CheckforNetBrake
  AllowRailtricks
  WaitAnimWhilstChecking AndManuals
  goto OnGroundAI
endscript

script SkateInLandOut
  AllowRailtricks
  ClearPanel_Landed
endscript

script SkateInOrBail_Out
  ClearPanel_Landed
endscript
Extra_5050_FS =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_FS Params = { IsExtra = yes } }
  { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_FS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Up Triangle 200 } Scr = Trick_NoseGrind_FS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Down Triangle 200 } Scr = Trick_5_0_FS Params = { IsExtra = yes } } ]
Extra_5050_BS =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_BS Params = { IsExtra = yes } }
  { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_BS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Up Triangle 200 } Scr = Trick_NoseGrind_BS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Down Triangle 200 } Scr = Trick_5_0_BS Params = { IsExtra = yes } } ]
Extra_Trick_Crooked_FS =
[ { Trigger = { Press Square 200 } Trick_Crooked_FS Params = { IsExtra = yes } } ]
Extra_NoseGrinds_FS =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_FS Params = { Name = 'FS Nosebluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Up Down Triangle 300 } Scr = Trick_Crooked_FS Params = { Name = 'FS Crooked' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_NGCrook_FS Params = { Name = 'FS OverCrook' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Noseslide_FS_ok Params = { Name = 'FS Noseslide' IsExtra = yes } } ]
Extra_NoseGrinds_BS =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_BS Params = { Name = 'BS Nosebluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Up Down Triangle 300 } Scr = Trick_Crooked_BS Params = { Name = 'BS Crooked' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_NGCrook_BS Params = { Name = 'BS OverCrook' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Noseslide_BS_ok Params = { Name = 'BS Noseslide' IsExtra = yes } } ]
Extra_TailGrinds_FS =
[ { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_FS Params = { Name = 'FS Bluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Down Up Triangle 300 } Scr = Trick_Smith_FS Params = { Name = 'FS Smith' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_Feeble_FS Params = { Name = 'FS Feeble' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Tailslide_FS_ok Params = { Name = 'FS Tailslide' IsExtra = yes } } ]
Extra_TailGrinds_BS =
[ { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_BS Params = { Name = 'BS Bluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Down Up Triangle 300 } Scr = Trick_Smith_BS Params = { Name = 'BS Smith' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_Feeble_BS Params = { Name = 'BS Feeble' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Tailslide_BS_ok Params = { Name = 'BS Tailslide' IsExtra = yes } } ]
Extra_5050_FS_DD =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_FS Params = { IsExtra = yes } }
  { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_FS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Up Triangle 200 } Scr = Trick_NoseGrind_FS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Down Triangle 200 } Scr = Trick_5_0_FS Params = { IsExtra = yes } }
  { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]
Extra_5050_BS_DD =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_BS Params = { IsExtra = yes } }
  { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_BS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Up Triangle 200 } Scr = Trick_NoseGrind_BS Params = { IsExtra = yes } }
  { Trigger = { AirTrickLogic Down Triangle 200 } Scr = Trick_5_0_BS Params = { IsExtra = yes } }
  { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]
Extra_Trick_Crooked_FS_DD =
[ { Trigger = { Press Square 200 } Trick_Crooked_FS Params = { IsExtra = yes } }
  { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]
Extra_NoseGrinds_FS_DD =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_FS Params = { Name = 'FS Nosebluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Up Down Triangle 300 } Scr = Trick_Crooked_FS Params = { Name = 'FS Crooked' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_NGCrook_FS Params = { Name = 'FS OverCrook' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Noseslide_FS_ok Params = { Name = 'FS Noseslide' IsExtra = yes } }
  { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]
Extra_NoseGrinds_BS_DD =
[ { Trigger = { TripleInOrder Up Up Triangle 300 } Scr = Trick_NoseBluntSlide_BS Params = { Name = 'BS Nosebluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Up Down Triangle 300 } Scr = Trick_Crooked_BS Params = { Name = 'BS Crooked' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_NGCrook_BS Params = { Name = 'BS OverCrook' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Noseslide_BS_ok Params = { Name = 'BS Noseslide' IsExtra = yes } }
  { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]
Extra_TailGrinds_FS_DD =
[ { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_FS Params = { Name = 'FS Bluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Down Up Triangle 300 } Scr = Trick_Smith_FS Params = { Name = 'FS Smith' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_Feeble_FS Params = { Name = 'FS Feeble' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Tailslide_FS_ok Params = { Name = 'FS Tailslide' IsExtra = yes } }
  { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]
Extra_TailGrinds_BS_DD =
[ { Trigger = { TripleInOrder Down Down Triangle 300 } Scr = Trick_Bluntslide_BS Params = { Name = 'BS Bluntslide' IsExtra = yes } }
  { Trigger = { TripleInOrder Down Up Triangle 300 } Scr = Trick_Smith_BS Params = { Name = 'BS Smith' IsExtra = yes } }
  { Trigger = { InOrder Circle Circle 250 } Scr = Trick_Feeble_BS Params = { Name = 'BS Feeble' IsExtra = yes } }
  { Trigger = { InOrder Square Square 250 } Scr = Trick_Tailslide_BS_ok Params = { Name = 'BS Tailslide' IsExtra = yes } }
  { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]
dropdown =
[ { Trigger = { Press R2 300 } Scr = DropDownRight Params = { IsExtra = yes } }
  { Trigger = { Press L2 300 } Scr = DropDownLeft Params = { IsExtra = yes } } ]

script Grind GrindTweak = 7
  KillExtraTricks
  if BailIsOn
    SetState Air
    goto DoingTrickBail
  endif
  BailOff
  Obj_ClearFlag FLAG_SKATER_MANUALCHEESE
  CleanUpSpecialItems
  if IsOptionOn LM_Control_bDropDown
    SetExtraTricks tricks = dropdown
  endif
  Vibrate Actuator = 1 Percent = 50 Duration = 0.25
  Vibrate Actuator = 0 Percent = 50
  if IsGrind <Type>
    SparksOn
  else
    SparksOff
  endif
  ClearExceptions
  if GotParam Nollie
    SetException Ex = Ollied Scr = NollieNoDisplay
    NollieOn
  else
    if GotParam OutAnimOnOllie
      SetException Ex = Ollied Scr = Ollie Params = { <...> }
    else
      SetException Ex = Ollied Scr = Ollie
    endif
    NollieOff
  endif
  SetException Ex = PointRail Scr = PointRail
  SetException Ex = OffRail Scr = OffRail Params = { initanim = <initanim> OutAnim = <OutAnim> }
  SetException Ex = Landed Scr = Land
  SetException Ex = OffMeterTop Scr = SkateInOrBail Params = { <...> FallingLeft }
  SetException Ex = OffMeterBottom Scr = SkateInOrBail Params = { <...> FallingRight }
  SetException Ex = SkaterCollideBail Scr = SkaterCollideBail
  SetException Ex = MadeOtherSkaterBail Scr = MadeOtherSkaterBail_Called CallInsteadOfGoto


  SetTrickScore 50
  SetTrickName "Kissed the Rail"
  OnExceptionRun Grind_Kissed
  ClearTrickQueue
  ClearManualTrick
  ClearExtraGrindTrick
  SetQueueTricks NoTricks
  SetManualTricks NoTricks
  SetRailSound <Type>
  if GotParam IsSpecial
    SetGrindTweak 36
  else
    SetGrindTweak <GrindTweak>
  endif
  if GotParam IsExtra
    LaunchExtraMessage
    AdjustBalance SpeedMult = 1.25
  endif
  if GotParam Profile
    if ProfileEquals is_named = <Profile>
      SwitchOnAtomic special_item
      SwitchOffAtomic special_item_2
    endif
  endif
  if GotParam NoBlend
    PlayAnim Anim = <initanim> BlendPeriod = 0.0
  else
    PlayAnim Anim = <initanim> BlendPeriod = 0.3
  endif
  DoBalanceTrick ButtonA = Right ButtonB = Left Type = <Type> DoFlipCheck
  WaitAnim 10 frames
  if GotParam IsSpecial
    LaunchSpecialMessage text = "Special Grind"
  endif
  OnExceptionRun Normal_Grind
  SetTrickName <Name>
  SetTrickScore <Score>
  Display BlockSpin
  WaitAnimFinished
  if GotParam FlipAfterInit
    Flip
    PlayBonkSound
    BoardRotate
    BlendperiodOut 0
  endif
  if GotParam Idle
    PlayAnim Anim = <Anim> Cycle NoRestart
  else
    PlayAnim Anim = <Anim> Wobble
  endif
  if GotParam Extratricks
    if IsOptionOn LM_Control_bDropDown
      AllowDropDown
    endif
    SetExtraTricks tricks = <Extratricks> ignore = <Name>
  endif
  if AnimEquals MaulRollGrind_Idle
    begin
      if FrameIs 10
        PlaySound SaberTwirl5 pitch = 65
      endif
      wait 1 frame
    repeat
  endif
endscript

script Grind_Kissed
  if GotParam MadeOtherSkaterBail
  else
    KillExtraTricks
  endif
  Display BlockSpin
  if ProfileEquals is_named = maul
  else
    SwitchOffAtomic special_item
    SwitchOnAtomic special_item_2
  endif
endscript

script Normal_Grind
  if GotParam MadeOtherSkaterBail
  else
    KillExtraTricks
  endif
  if ProfileEquals is_named = maul
  else
    SwitchOffAtomic special_item
    SwitchOnAtomic special_item_2
  endif
endscript

script OffRail
  KillExtraTricks
  Vibrate Actuator = 0 Percent = 0
  SetState Air
  SetException Ex = Landed Scr = Land
  //this is from th4, fixes rail to wallride if triangle is held
  SetException Ex = WallRideLeft Scr = WallRide Params = { Left }
  SetException Ex = WallRideRight Scr = WallRide Params = { Right }

  DoNextTrick
  if GotParam EarlyOut
    PlayAnim Anim = <EarlyOut> BlendPeriod = 0.1 Backwards
  else
    if GotParam OutAnim
      if GotParam OutAnimBackwards
        PlayAnim Anim = <OutAnim> Backwards BlendPeriod = 0.1
      else
        PlayAnim Anim = <OutAnim> BlendPeriod = 0.1
      endif
    else
      PlayAnim Anim = <initanim> BlendPeriod = 0.1 Backwards
    endif
  endif
  if GotParam BoardRotate
    BlendperiodOut 0
    BoardRotateAfter
  endif
  if GotParam FlipAfter
    printf "flipping.................."
    FlipAfter
  endif
  WaitAnimFinished
  goto Airborne
endscript

script Trick_5050_BS Name = 'BS 50-50'
  Grind { Name = <Name> Score = 100 initanim = Init_FiftyFifty Anim = FiftyFifty_range Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_5050_BS IsExtra = <IsExtra> }
endscript

script Trick_5050_FS Name = 'FS 50-50'
  Grind { Name = <Name> Score = 100 initanim = Init_FiftyFifty Anim = FiftyFifty_range Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_5050_FS IsExtra = <IsExtra> }
endscript

script Trick_5050_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_5050_BS Params = { NoBlend = yes }
endscript

script Trick_5050_FS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_5050_FS Params = { NoBlend = yes }
endscript

script Trick_Boardslide_FS Name = 'FS Boardslide'
  Rotate
  Grind { Name = <Name> Score = 200 GrindTweak = 14 initanim = Init_FSBoardslide Anim = FSBoardslide_range OutAnim = FSBoardslide_Out Type = Slide NoBlend = yes GrindBail = Nutter IsExtra = <IsExtra> }
endscript

script Trick_Boardslide_BS Name = 'BS Boardslide'
  Grind { Name = <Name> Score = 200 GrindTweak = 14 initanim = Init_BSBoardslide Anim = BSBoardslide_range OutAnim = BSBoardslide_Out Type = Slide NoBlend = yes GrindBail = Nutter IsExtra = <IsExtra> }
endscript

script Trick_Lipslide_FS Name = 'FS Lipslide'
  Grind { Name = <Name> Score = 200 GrindTweak = 14 initanim = Init_FSLipslide Anim = BSBoardslide_range OutAnim = BSBoardslide_Out Type = Slide NoBlend = yes GrindBail = Nutter IsExtra = <IsExtra> }
endscript

script Trick_Lipslide_BS Name = 'BS Lipslide'
  Rotate
  Grind { Name = <Name> Score = 200 GrindTweak = 14 initanim = Init_BSLipslide Anim = FSBoardslide_range OutAnim = FSBoardslide_Out Type = Slide NoBlend = yes GrindBail = Nutter IsExtra = <IsExtra> }
endscript

script Trick_Tailslide_FS
  if BadLedge
    goto Trick_Noseslide_BS_ok
  else
    goto Trick_Tailslide_FS_ok
  endif
endscript

script Trick_Tailslide_FS_ok Name = 'FS Tailslide'
  Grind { Name = <Name> Score = 150 GrindTweak = 11 initanim = Init_FSTailslide Anim = FSTailslide_range OutAnim = FSTailslide_Out Type = Slide NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_TailGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_Tailslide_BS
  if BadLedge
    goto Trick_Noseslide_FS_ok
  else
    goto Trick_Tailslide_BS_ok
  endif
endscript

script Trick_Tailslide_BS_ok Name = 'BS Tailslide'
  Grind { Name = <Name> Score = 150 GrindTweak = 11 initanim = Init_Tailslide Anim = Tailslide_range OutAnim = BSTailslide_Out Type = Slide NoBlend = <NoBlend> GrindBail = BackwardsGrindBails Extratricks = Extra_TailGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_Tailslide_FS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_Tailslide_BS Params = { NoBlend = yes }
endscript

script Trick_Tailslide_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_Tailslide_FS Params = { NoBlend = yes }
endscript

script Trick_Noseslide_FS
  if BadLedge
    goto Trick_Tailslide_BS_ok
  else
    goto Trick_Noseslide_FS_ok
  endif
endscript

script Trick_Noseslide_FS_ok Name = 'FS Noseslide'
  Grind { Name = <Name> Score = 150 GrindTweak = 11 initanim = Init_FSNoseslide Anim = FSNoseslide_range Type = Slide Nollie = yes OutAnim = FSNoseSlide_Out NoBlend = <NoBlend> GrindBail = BackwardsGrindBails Extratricks = Extra_NoseGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_Noseslide_BS
  if BadLedge
    goto Trick_Tailslide_FS_ok
  else
    goto Trick_Noseslide_BS_ok
  endif
endscript

script Trick_Noseslide_BS_ok Name = 'BS Noseslide'
  Grind { Name = <Name> Score = 150 GrindTweak = 11 initanim = Init_Noseslide Anim = Noseslide_range OutAnim = BSNoseslide_Out Type = Slide Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_Noseslide_FS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_Noseslide_BS Params = { NoBlend = yes }
endscript

script Trick_Noseslide_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_Noseslide_FS Params = { NoBlend = yes }
endscript

script Trick_NoseGrind_FS Name = 'FS Nosegrind'
  Grind { Name = <Name> Score = 100 initanim = Init_Nosegrind Anim = Nosegrind_range Type = Grind Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_NoseGrind_BS Name = 'BS Nosegrind'
  Grind { Name = <Name> Score = 100 initanim = Init_Nosegrind Anim = Nosegrind_range Type = Grind Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_NoseGrind_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_5_0_FS Params = { NoBlend = yes }
endscript

script Trick_NoseGrind_FS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_5_0_BS Params = { NoBlend = yes }
endscript

script Trick_5_0_FS Name = 'FS 5-0'
  Grind { Name = <Name> Score = 100 initanim = Init_Tailgrind Anim = Tailgrind_range Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_TailGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_5_0_BS Name = 'BS 5-0'
  Grind { Name = <Name> Score = 100 initanim = Init_Tailgrind Anim = Tailgrind_range Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_TailGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_5_0_FS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_NoseGrind_BS Params = { NoBlend = yes }
endscript

script Trick_5_0_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_NoseGrind_BS Params = { NoBlend = yes }
endscript

script Trick_Crooked_FS Name = 'FS Crooked'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_FSCrooked Anim = FSCrooked_range Type = Grind Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_Crooked_FS_rot
  Rotate
  goto Trick_Crooked_FS
endscript

script Trick_Crooked_BS Name = 'BS Crooked'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_BSCrooked Anim = BSCrooked_range Type = Grind Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_Crooked_FS_180
  if Backwards
    printf "I'm backwards............"
  endif
  FlipAndRotate
  BoardRotateAfter
  goto Trick_Crooked_BS Params = { NoBlend = yes }
endscript

script Trick_Crooked_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_Crooked_FS Params = { NoBlend = yes }
endscript

script Trick_NGCRook_FS_rot
  Rotate
  printf "fixed it bitches................................"
  goto Trick_NGCrook_FS
endscript

script Trick_NGCrook_FS Name = 'FS Overcrook'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_FSOvercrook Anim = FSOvercrook_range Type = Grind Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_NGCrook_BS Name = 'BS Overcrook'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_BSOvercrook Anim = BSOvercrook_range Type = Grind Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_NGCRook_FS_180
  FlipAndRotate
  goto Trick_NGCrook_BS Params = { NoBlend = yes }
endscript

script Trick_NGCrook_BS_180
  FlipAndRotate
  goto Trick_NGCrook_FS Params = { NoBlend = yes }
endscript

script Trick_Smith_FS Name = 'FS Smith'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_FSSmith Anim = FSSmith_range Type = Grind NoBlend = <NoBlend> GrindBail = Nutter Extratricks = Extra_TailGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_Smith_FS_rot
  Rotate
  goto Trick_Smith_FS
endscript

script Trick_Smith_BS Name = 'BS Smith'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_BSSmith Anim = BSSmith_range Type = Grind NoBlend = <NoBlend> GrindBail = Nutter Extratricks = Extra_TailGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_Smith_FS_180
  FlipAndRotate
  goto Trick_Smith_BS Params = { NoBlend = yes }
endscript

script Trick_Smith_BS_180
  FlipAndRotate
  goto Trick_Smith_FS Params = { NoBlend = yes }
endscript

script Trick_Feeble_FS Name = 'FS Feeble'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_FSFeeble Anim = FSFeeble_range Type = Grind NoBlend = <NoBlend> GrindBail = Nutter Extratricks = Extra_TailGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_Feeble_FS_rot
  Rotate
  goto Trick_Feeble_FS
endscript

script Trick_Feeble_BS Name = 'BS Feeble'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = Init_BSFeeble Anim = BSFeeble_range Type = Grind NoBlend = <NoBlend> GrindBail = Nutter Extratricks = Extra_TailGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_Feeble_FS_180
  FlipAndRotate
  goto Trick_Feeble_BS Params = { NoBlend = yes }
endscript

script Trick_Feeble_BS_180
  FlipAndRotate
  goto Trick_Feeble_FS Params = { NoBlend = yes }
endscript

script Trick_Bluntslide_BS Name = 'BS Bluntslide'
  Grind { Name = <Name> Score = 250 GrindTweak = 18 initanim = Init_BSBluntSlide Anim = BSBluntSlide_range Type = Slide NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_TailGrinds_BS IsExtra = <IsExtra> }
endscript

script Trick_Bluntslide_FS Name = 'FS Bluntslide'
  Grind { Name = <Name> Score = 250 GrindTweak = 18 initanim = Init_FSBluntSlide Anim = FSBluntSlide_range Type = Slide NoBlend = <NoBlend> GrindBail = BackwardsGrindBails Extratricks = Extra_TailGrinds_FS IsExtra = <IsExtra> }
endscript

script Trick_NoseBluntSlide_BS Name = 'BS Nosebluntslide'
  Grind { Name = <Name> Score = 250 GrindTweak = 18 initanim = Init_BSNoseblunt Anim = BSNoseblunt_range Type = Slide NoBlend = <NoBlend> GrindBail = BackwardsGrindBails Extratricks = Extra_NoseGrinds_BS IsExtra = <IsExtra> Nollie = yes }
endscript

script Trick_NoseBluntSlide_FS Name = 'FS Nosebluntslide'
  Grind { Name = <Name> Score = 250 GrindTweak = 18 initanim = Init_FSNoseblunt Anim = FSNoseblunt_range Type = Slide NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Nollie = yes IsExtra = <IsExtra> Extratricks = Extra_NoseGrinds_FS }
endscript

script Trick_Bluntslide_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_Bluntslide_BS Params = { NoBlend = yes }
endscript

script Trick_Bluntslide_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_Bluntslide_FS Params = { NoBlend = yes }
endscript

script Trick_Nosebluntslide_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_NoseBluntSlide_BS Params = { NoBlend = yes }
endscript

script Trick_Nosebluntslide_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_NoseBluntSlide_FS Params = { NoBlend = yes }
endscript

script Trick_Hurricane_BS
  Grind { Name = 'BS Hurricane' Score = 400 initanim = BSHurricaneGrind_Init Anim = BSHurricaneGrind_Range OutAnim = BSHurricaneGrind_Out Type = Grind NoBlend = <NoBlend> GrindBail = BackwardsGrindBails Nollie = yes IsSpecial OutAnimOnOllie }
endscript

script Trick_Hurricane_FS
  Grind { Name = 'FS Hurricane' Score = 400 initanim = FSHurricaneGrind_Init Anim = FSHurricaneGrind_Range OutAnim = FSHurricaneGrind_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial OutAnimOnOllie }
endscript

script Trick_Hurricane_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_Hurricane_BS Params = { NoBlend = yes }
endscript

script Trick_Hurricane_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_Hurricane_FS Params = { NoBlend = yes }
endscript

script Trick_Darkslide_BS
  Grind { Name = 'BS Darkslide' Score = 400 initanim = Darkslide_Init Anim = Darkslide_Range OutAnim = Darkslide_Out Type = Slide NoBlend = <NoBlend> GrindBail = BackwardsGrindBails Nollie = yes IsSpecial OutAnimOnOllie BoardRotate }
endscript

script Trick_Darkslide_FS
  Grind { Name = 'FS Darkslide' Score = 400 initanim = Darkslide_Init Anim = Darkslide_Range OutAnim = Darkslide_Out Type = Slide NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial OutAnimOnOllie BoardRotate }
endscript

script Trick_Darkslide_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_Darkslide_BS Params = { NoBlend = yes }
endscript

script Trick_Darkslide_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_Darkslide_FS Params = { NoBlend = yes }
endscript

script Trick_Coffin_BS
  Grind { Name = 'BS Coffin' Score = 500 initanim = CoffinGrind_Init Anim = CoffinGrind_Range OutAnim = CoffinGrind_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial OutAnimOnOllie }
endscript

script Trick_Coffin_FS
  Grind { Name = 'FS Coffin' Score = 500 initanim = CoffinGrind_Init Anim = CoffinGrind_Range OutAnim = CoffinGrind_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial OutAnimOnOllie }
endscript

script Trick_Coffin_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_Coffin_BS Params = { NoBlend = yes }
endscript

script Trick_Coffin_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_Coffin_FS Params = { NoBlend = yes }
endscript

script Trick_fandangle_BS
  Grind { Name = 'BS Fandangle' Score = 500 initanim = fandangle_Init Anim = fandangle_Range OutAnim = Fandangle_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial Extratricks = Extra_NoseGrinds_BS OutAnimOnOllie BoardRotate }
endscript

script Trick_fandangle_FS
  Grind { Name = 'FS Fandangle' Score = 500 initanim = fandangle_Init Anim = fandangle_Range OutAnim = Fandangle_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial Extratricks = Extra_NoseGrinds_FS OutAnimOnOllie BoardRotate }
endscript

script Trick_fandangle_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_fandangle_BS Params = { NoBlend = yes }
endscript

script Trick_fandangle_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_fandangle_FS Params = { NoBlend = yes }
endscript

script Trick_CrailSlide_BS
  Grind { Name = 'BS Crail Slide' Score = 500 initanim = CrailSlide_Init Anim = CrailSlide_Range OutAnim = CrailSlide_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial Extratricks = Extra_TailGrinds_BS OutAnimOnOllie }
endscript

script Trick_CrailSlide_FS
  Grind { Name = 'FS Crail Slide' Score = 500 initanim = CrailSlide_Init Anim = CrailSlide_Range OutAnim = CrailSlide_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial Extratricks = Extra_TailGrinds_FS OutAnimOnOllie }
endscript

script Trick_CrailSlide_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_CrailSlide_BS Params = { NoBlend = yes }
endscript

script Trick_CrailSlide_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_CrailSlide_FS Params = { NoBlend = yes }
endscript

script Trick_GrindOverturn_BS
  Grind { Name = 'BS 5-0 Overturn' Score = 500 initanim = GrindOverturn_Init Anim = GrindOverturn_Range OutAnim = GrindOverturn_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial Extratricks = Extra_TailGrinds_BS OutAnimOnOllie }
endscript

script Trick_GrindOverturn_FS
  Grind { Name = 'FS 5-0 Overturn' Score = 500 initanim = GrindOverturn_Init Anim = GrindOverturn_Range OutAnim = GrindOverturn_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial Extratricks = Extra_TailGrinds_FS OutAnimOnOllie }
endscript

script Trick_GrindOverturn_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_GrindOverturn_BS Params = { NoBlend = yes }
endscript

script Trick_GrindOverturn_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_GrindOverturn_FS Params = { NoBlend = yes }
endscript

script Trick_HangTenNoseGrind_BS
  Grind { Name = 'Hang Ten Nosegrind' Score = 500 initanim = HangTenNoseGrind_Init Anim = HangTenNoseGrind_Range OutAnim = HangTenNoseGrind_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_BS IsSpecial Nollie }
endscript

script Trick_HangTenNoseGrind_FS
  Grind { Name = 'Hang Ten Nosegrind' Score = 500 initanim = HangTenNoseGrind_Init Anim = HangTenNoseGrind_Range OutAnim = HangTenNoseGrind_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_FS IsSpecial Nollie }
endscript

script Trick_HangTenNoseGrind_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_HangTenNoseGrind_BS Params = { NoBlend = yes }
endscript

script Trick_HangTenNoseGrind_FS_180
  Rotate
  BoardRotateAfter
  goto Trick_HangTenNoseGrind_FS Params = { NoBlend = yes }
endscript

script Trick_Sobriety_BS
  Grind { Name = 'Sobriety Grind' Score = 500 initanim = Sobriety_Init Anim = Sobriety_Range Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial }
endscript

script Trick_Sobriety_FS
  goto Trick_Sobriety_BS
endscript

script Trick_Sobriety_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_Sobriety_BS Params = { NoBlend = yes }
endscript

script Trick_Sobriety_FS_180
  goto Trick_Sobriety_BS_180
endscript

script Trick_Handstand5050_BS
  Grind { Name = 'Handstand 50-50' Score = 500 initanim = Handstand5050Grind_Init Anim = Handstand5050Grind_Range OutAnim = Handstand5050Grind_out Type = Grind NoBlend = <NoBlend> GrindBail = NoseManualBail2 IsSpecial OutAnimOnOllie }
endscript

script Trick_Handstand5050_FS
  goto Trick_Handstand5050_BS
endscript

script Trick_Handstand5050_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_Handstand5050_BS Params = { NoBlend = yes }
endscript

script Trick_Handstand5050_FS_180
  goto Trick_Handstand5050_BS_180
endscript

script Trick_RowleyDarkSlide_BS
  Grind { Name = 'Rowley Darkslide' Score = 500 initanim = RowleyDarkSlide_Init Anim = RowleyDarkSlide_Range OutAnim = RowleyDarkSlide_out Type = Slide NoBlend = <NoBlend> GrindBail = Nutter IsSpecial OutAnimOnOllie BoardRotate }
endscript

script Trick_RowleyDarkSlide_FS
  goto Trick_RowleyDarkSlide_BS
endscript

script Trick_RowleyDarkSlide_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_RowleyDarkSlide_BS Params = { NoBlend = yes }
endscript

script Trick_RowleyDarkSlide_FS_180
  goto Trick_RowleyDarkSlide_BS_180
endscript

script Trick_BigHitter_BS Extratricks = Extra_TailGrinds_BS
  Grind { Name = 'Big Hitter II' Score = 500 initanim = BigHitter_Init Anim = BigHitter_Range OutAnim = BigHitter_out Type = Slide NoBlend = <NoBlend> GrindBail = Nutter IsSpecial Extratricks = <Extratricks> OutAnimOnOllie }
endscript

script Trick_BigHitter_FS
  goto Trick_BigHitter_BS Params = { Extratricks = Extra_TailGrinds_FS }
endscript

script Trick_BigHitter_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_BigHitter_BS Params = { NoBlend = yes }
endscript

script Trick_BigHitter_FS_180
  goto Trick_BigHitter_BS_180
endscript

script Trick_NosegrindPivot_BS Extratricks = Extra_TailGrinds_FS
  Grind { Name = 'Nosegrind to Pivot' Score = 500 initanim = NosegrindPivot_Init Anim = NosegrindPivot_Range OutAnim = NosegrindPivot_Out Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial BoardRotate FlipAfter = 50 Extratricks = <Extratricks> EarlyOut = Init_Tailgrind OutAnimOnOllie }
endscript

script Trick_NosegrindPivot_FS
  goto Trick_NosegrindPivot_BS
endscript

script Trick_NosegrindPivot_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_NosegrindPivot_BS Params = { NoBlend = yes }
endscript

script Trick_NosegrindPivot_FS_180
  goto Trick_NosegrindPivot_BS_180
endscript

script Trick_LaidBack_BS Extratricks = Extra_TailGrinds_BS
  Grind { Name = 'Layback BS Smith' Score = 500 initanim = FSFeebleLayback_Init Anim = FSFeebleLayback_Range OutAnim = FSFeebleLayback_Out Type = Grind NoBlend = <NoBlend> GrindBail = Nutter IsSpecial Extratricks = <Extratricks> OutAnimOnOllie Profile = Thomas }
endscript

script Trick_LaidBack_FS Extratricks = Extra_TailGrinds_FS
  Grind { Name = 'Layback FS Feeble' Score = 500 initanim = FSFeebleLayback_Init Anim = FSFeebleLayback_Range OutAnim = FSFeebleLayback_Out Type = Grind NoBlend = <NoBlend> GrindBail = Nutter IsSpecial Extratricks = <Extratricks> OutAnimOnOllie Profile = Thomas }
endscript

script Trick_LaidBack_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_LaidBack_FS Params = { NoBlend = yes }
endscript

script Trick_LaidBack_FS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_LaidBack_BS Params = { NoBlend = yes }
endscript

script Trick_TailblockSlide_BS Extratricks = Extra_TailGrinds_BS
  Grind { Name = 'Tailblock Slide' Score = 500 initanim = TailblockSlide_Init Anim = TailblockSlide_Range OutAnim = TailblockSlide_Init Type = Slide NoBlend = <NoBlend> GrindBail = Nutter Extratricks = <Extratricks> IsSpecial }
endscript

script Trick_TailblockSlide_FS
  goto Trick_TailblockSlide_BS Params = { Extratricks = Extra_TailGrinds_FS }
endscript

script Trick_TailblockSlide_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_TailblockSlide_BS Params = { NoBlend = yes }
endscript

script Trick_TailblockSlide_FS_180
  goto Trick_TailblockSlide_BS_180
endscript

script Trick_HumanDart_BS
  Grind { Name = 'The Human Dart' Score = 500 initanim = HumanDart_Init Anim = HumanDart_Range OutAnim = HumanDart_Out Type = Grind NoBlend = <NoBlend> GrindBail = Nutter IsSpecial OutAnimOnOllie }
endscript

script Trick_HumanDart_FS
  goto Trick_HumanDart_BS
endscript

script Trick_HumanDart_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_HumanDart_BS Params = { NoBlend = yes }
endscript

script Trick_HumanDart_FS_180
  goto Trick_HumanDart_BS_180
endscript

script Trick_DrunkGrind_BS
  Grind { Name = 'S.U.I Grind' Score = 500 initanim = DrunkGrind_Init Anim = DrunkGrind_Idle Type = Grind NoBlend = <NoBlend> GrindBail = Nutter IsSpecial OutAnimOnOllie }
endscript

script Trick_DrunkGrind_FS
  goto Trick_DrunkGrind_BS
endscript

script Trick_DrunkGrind_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_DrunkGrind_BS Params = { NoBlend = yes }
endscript

script Trick_DrunkGrind_FS_180
  goto Trick_DrunkGrind_BS_180
endscript

script Trick_MaulRollGrind_BS
  Grind { Name = 'Dark Jedi Grind' Score = 500 initanim = MaulRollGrind_Init Anim = MaulRollGrind_Idle Idle Type = Grind NoBlend = <NoBlend> GrindBail = Nutter IsSpecial }
endscript

script Trick_MaulRollGrind_FS
  goto Trick_MaulRollGrind_BS
endscript

script Trick_MaulRollGrind_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_MaulRollGrind_BS Params = { NoBlend = yes }
endscript

script Trick_MaulRollGrind_FS_180
  goto Trick_MaulRollGrind_BS_180
endscript

script Trick_PaddleGrind_BS
  Grind { Name = 'Paddle Out Grind' Score = 500 initanim = PaddleGrind_Init Anim = PaddleGrind_Idle Idle Type = Grind NoBlend = <NoBlend> GrindBail = Nutter IsSpecial }
endscript

script Trick_PaddleGrind_FS
  goto Trick_PaddleGrind_BS
endscript

script Trick_PaddleGrind_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_PaddleGrind_BS Params = { NoBlend = yes }
endscript

script Trick_PaddleGrind_FS_180
  goto Trick_PaddleGrind_BS_180
endscript

script Trick_Salad_FS Name = 'FS Salad'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = FSSaladGrind_Init Anim = FSSaladGrind_range Type = Grind NoBlend = <NoBlend> GrindBail = Nutter Extratricks = Extra_TailGrinds_FS IsSpecial IsExtra = <IsExtra> }
endscript

script Trick_Salad_BS Name = 'BS Salad'
  Grind { Name = <Name> Score = 125 GrindTweak = 9 initanim = BSSaladGrind_Init Anim = BSSaladGrind_range Type = Grind NoBlend = <NoBlend> GrindBail = Nutter Extratricks = Extra_TailGrinds_BS IsSpecial IsExtra = <IsExtra> }
endscript

script Trick_Salad_FS_180
  FlipAndRotate
  goto Trick_Salad_BS Params = { NoBlend = yes }
endscript

script Trick_Salad_BS_180
  FlipAndRotate
  goto Trick_Salad_FS Params = { NoBlend = yes }
endscript

script Trick_NoseSlideLipSlide_FS
  if BadLedge
    goto Trick_NoseSlideLipSlide_BS_ok
  else
    goto Trick_NoseSlideLipSlide_FS_ok
  endif
endscript

script Trick_NoseSlideLipSlide_FS_ok Name = 'FS Noseslide LipSlide'
  Grind { Name = <Name> Score = 500 initanim = FSNoseSlideLipSlide Anim = BSBoardslide_range Type = Slide Nollie = yes NoBlend = <NoBlend> GrindBail = Nutter Extratricks = Extra_NoseGrinds_FS IsSpecial IsExtra = <IsExtra> }
endscript

script Trick_NoseSlideLipSlide_BS
  if BadLedge
    goto Trick_NoseSlideLipSlide_FS_ok
  else
    goto Trick_NoseSlideLipSlide_BS_ok
  endif
endscript

script Trick_NoseSlideLipSlide_BS_ok Name = 'BS NoseSlide LipSlide'
  Grind { Name = <Name> Score = 500 initanim = BSNoseSlideLipSlide Anim = FSBoardslide_range Type = Slide Nollie = yes NoBlend = <NoBlend> GrindBail = FiftyFiftyFall Extratricks = Extra_NoseGrinds_BS IsSpecial IsExtra = <IsExtra> }
endscript

script Trick_NoseSlideLipSlide_FS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_NoseSlideLipSlide_BS Params = { NoBlend = yes }
endscript

script Trick_NoseSlideLipSlide_BS_180
  FlipAndRotate
  BoardRotateAfter
  goto Trick_NoseSlideLipSlide_FS Params = { NoBlend = yes }
endscript

script Trick_BoomBox_BS
  Grind { Name = 'Ghetto Blastin' Score = 500 special_item initanim = BoomBox_Init Anim = BoomBox_Range2 Type = Grind NoBlend = <NoBlend> GrindBail = FiftyFiftyFall IsSpecial Profile = Muska }
endscript

script Trick_BoomBox_FS
  goto Trick_BoomBox_BS Params = { Anim = BoomBox_Range }
endscript

script Trick_BoomBox_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_BoomBox_BS Params = { NoBlend = yes Anim = BoomBox_Range }
endscript

script Trick_BoomBox_FS_180
  goto Trick_BoomBox_BS_180
endscript

script Trick_CrookedBigSpin_BS
  Grind { Name = 'Crook BigSpin Switch FS Crook' Score = 500 special_item initanim = CrookedBigSpin Anim = FSCrooked_range OutAnim = Init_FSCrooked OutAnimBackwards Type = Grind NoBlend = <NoBlend> GrindBail = BackwardsGrindBails IsSpecial FlipAfterInit }
endscript

script Trick_CrookedBigSpin_FS
  goto Trick_CrookedBigSpin_BS Params = { NoBlend = yes Name = 'Overcrook BigSpin Overcrook' }
endscript

script Trick_CrookedBigSpin_BS_180
  Rotate
  BoardRotateAfter
  goto Trick_CrookedBigSpin_BS Params = { NoBlend = yes }
endscript

script Trick_CrookedBigSpin_FS_180
  goto Trick_CrookedBigSpin_BS_180
endscript
