
script WallRide
    ClearExceptions
    SetException Ex = Landed Scr = Land
    SetException Ex = Ollied Scr = Wallie
    SetException Ex = GroundGone Scr = WallrideEnd
    SetException Ex = SkaterCollideBail Scr = SkaterCollideBail

    Vibrate Actuator = 1 Percent = 40

    if GotParam Left
        Move x = 36
    else
        Move x = -36
    endif

    //since boostplant is too easy without wall bail, we enable it too
    if IsOptionOn LM_Control_bBoostPlant
        if BailIsOn
            SetState Air
            Goto DoingTrickBail
        endif
    endif

    //added extra tricks for wallieplant
    if IsOptionOn LM_Control_bWalliePlant
        SetQueueTricks WallrideTricks
    endif

    NollieOff
    SetTrickScore 200
    PlayCessSound

    if GotParam Left
        if Flipped
            Walltricks_PlayWallrideAnim Trick_WallrideBS
        else
            Walltricks_PlayWallrideAnim Trick_WallrideFS
        endif
    else
        if Flipped
            Walltricks_PlayWallrideAnim Trick_WallrideFS
        else
            Walltricks_PlayWallrideAnim Trick_WallrideBS
        endif
    endif

    Display

endscript


Trick_WallrideFS = {
    Name = 'FS Wallride'
    AnimTrans = WallRideFrontTrans
    AnimLoop = WallRideFrontLoop
}

Trick_WallrideBS = {
    Name = 'BS Wallride'
    AnimTrans = WallRideBackTrans
    AnimLoop = WallRideBackLoop
}


script Walltricks_PlayWallrideAnim
    SetTrickName <Name>
    PlayAnim Anim = <AnimTrans> BlendPeriod = 0.0
    WaitAnimFinished
    PlayAnim Anim = <AnimLoop> BlendPeriod = 0.1 Cycle
endscript

//Added WalliePlant
WallrideTricks = [ { Trigger = { TapOnceRelease Up X 500 } Scr = WalliePlant } ]

script WalliePlant
    InAirExceptions
    Vibrate Actuator = 1 Percent = 50 Duration = 0.1
    PlayAnim Anim = Boneless BlendPeriod = 0.0
    SetTrickName "Wallieplant"
    SetTrickScore 500
    Display
    #"Jump" BonelessHeight
    PlaySound boneless09 pitch = 85
    WaitAnimWhilstChecking
    goto Airborne StretchTime = 10 BlendPeriod = 0
endscript

script WallrideEnd
    BlendperiodOut 0
    //this is in thps4
    SetException ex = Landed scr = Land
    ClearExceptions

    //this is in thps3
    //InAirExceptions
    //SetState Air
    //goto Airborne StretchTime = 10 BlendPeriod = 0
    
    //this is in thps4
    if InAir
        Goto Airborne
    endif
endscript


script Wallie
    //Changed order of stuff now boostplant works just like in thps4
    //I tried to fix non boostplant and this code makes it way harder
    //But it's still possible to get atleast double wallieplant
    //Maybe we can remove this option now?
    if IsOptionOn LM_Control_bWalliePlant 
        DoNextTrick
    else
        ClearTrickQueue
        ClearEventBuffer
        ClearExceptions
    endif

    Vibrate Actuator = 1 Percent = 50 Duration = 0.1
    PlayAnim Anim = Ollie BlendPeriod = 0.0
    SetTrickName "Wallie"
    SetTrickScore 250
    InAirExceptions
    Display
    #"Jump" //cause jump is a reserved keyword in later games
    WaitAnimWhilstChecking
    goto Airborne StretchTime = 10 BlendPeriod = 0
endscript

script WallRideBail
endscript



script WallplantOllie
    IF WallplantTimeGreaterThan 350
        ClearException Ollied
    ELSE
        Ollie
        ClearException Ollied
    ENDIF
endscript

WallplantOllie2 = [
  { Trigger = { Tap x 200 } Scr = Ollie Params = { JumpSpeed = 200 } }
]

WALLPLANT_WINDOW = 450

//maybe rename? where is this used?
Wallplant_Trick = [
    { InOrder x Down WALLPLANT_WINDOW }
    { InOrder x DownLeft WALLPLANT_WINDOW }
    { InOrder x DownRight WALLPLANT_WINDOW }
    { InOrder Down x WALLPLANT_WINDOW }
    { InOrder DownLeft x WALLPLANT_WINDOW }
    { InOrder DownRight x WALLPLANT_WINDOW }
]

script Wallplant
    if BailIsOn
        SetState Air
    endif
    
    NollieOff
    InAirExceptions
    ClearException Ollied
    PlaySound footstep03
    Vibrate Actuator = 1 Percent = 100 Duration = 0.1
    PlayAnim Anim = Beanplant BlendPeriod = 0
    SetTrickName 'Wallplant'
    SetTrickScore 750
    Display

    begin
        if WallplantTimeGreaterThan 180
            ClearEventBuffer Buttons = [ x ]
            SetException ex = Ollied scr = WallplantOllie CallInsteadofGoto
            //??
            //SetQueueTricks Wallplantollie2
            printf "Allowing ollie"
            break
        endif
        DoNextTrick
        WaitOneGameFrame
    repeat

    begin
        if WallplantTimeGreaterThan 400
            printf "Disallowing ollie"
            break
        endif
        DoNextTrick
        WaitOneGameFrame
    repeat

    ClearTrickQueue
    WaitAnimWhilstChecking
    Goto Airborne
endscript
