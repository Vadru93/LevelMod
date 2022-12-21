#include "pch.h"
#include "Timer.h"
#include <Mmsystem.h>

extern bool init;
extern bool init2;

#define FPS(x) (1000000.0 / x)
#define FPS_LOWRES(x) (1000.0 / x)
#define FPS_SCALE(x) (60.0 / x)

namespace NewTimer
{
    DWORD reset_time = 0;
    LARGE_INTEGER timer_time;
    double fFreq = 0;
    LARGE_INTEGER freq;
    LARGE_INTEGER endTime;
    LARGE_INTEGER elapsedTime;
    LARGE_INTEGER old_start;
    DWORD timer_lock = 0x10;
    double target = 1000000.0 / Gfx::target_fps;
    double framelength = target;
    DWORD frameticks = 0;
    DWORD max_frame_ticks = 0;
    DWORD min_frame_ticks = 0;
    double hybrid_limit = 16.0;
    DWORD time = 0;
    unsigned long long time_now;
    DWORD hybrid_loop_time = 0;
    DWORD ticks_hybrid;
    DWORD lastTimeToggleWindowed = 0;


    void CalculateFPSTimers()
    {
        DWORD frameticks2 = (DWORD)(16666.666666666 / (1000.0 / (double)freq.QuadPart));
        frameticks = (DWORD)((1000000.0 / Gfx::target_fps) / fFreq);
        max_frame_ticks = frameticks + 2000;
        min_frame_ticks = frameticks - 2000;
        //Add some headroom for hardware delay and rounding errors, probably should add this to ini for platform specific stored value
        frameticks -= 750;
        DWORD frameticks3 = (DWORD)(0.0166666666 / (1.0 / (double)freq.QuadPart));
        debug_print("f1 %u f2 %u f3 %u\n", frameticks, frameticks2, frameticks3);

        //Special anim speed and maybe more gui speed stuff? 
        //is 0.058 on original 60 fps lock
        //*(float*)0x00458B68 = (float)((FPS_SCALE(Gfx::target_fps)) * 0.058);//Stack value
        //*(float*)0x0058E100 = (float)((FPS_SCALE(Gfx::target_fps)) * 0.058);//Memory value

        Gfx::exact_high  = FPS(Gfx::target_fps) + Gfx::exact_high_diff;
        Gfx::exact_low   = FPS(Gfx::target_fps) - Gfx::exact_low_diff;
        Gfx::hybrid_high = FPS(Gfx::target_fps) + Gfx::hybrid_high_diff;
        Gfx::sleep_high  = FPS(Gfx::target_fps) - Gfx::sleep_high_diff;
        Gfx::sleep_low   = FPS(Gfx::target_fps) - Gfx::sleep_low_diff;

        *(float*)0x00850FD0 = (float)FPS_LOWRES(Gfx::target_fps);
        double target = FPS(Gfx::target_fps);
        double framelength = target;

        if (Gfx::target_fps == 60.0)
            Gfx::frame_modifier = 0.97248;
        else if (Gfx::target_fps == 120.0)
            Gfx::frame_modifier = 0.973;
        else if (Gfx::target_fps == 144.0)
            Gfx::frame_modifier = 0.97612;
        else if (Gfx::target_fps == 300.0)
            Gfx::frame_modifier = 0.9774592502;
        else
        {
            if (Gfx::target_fps < 120.0)
                Gfx::frame_modifier = Map(Gfx::target_fps, 60.0, 120.0, 0.97248, 0.971593312);
            else if(Gfx::target_fps < 144.0)
                Gfx::frame_modifier = Map(Gfx::target_fps, 120.0, 144.0, 0.971593312, 0.97612);
            else
                Gfx::frame_modifier = Map(Gfx::target_fps, 144.0, 300.0, 0.97612, 0.9774592502);
        }

        Gfx::frame_modifier *= 0.000001;
    }

    LARGE_INTEGER profile_start;
    void ProfileStart()
    {
//#ifdef _DEBUG
        QueryPerformanceCounter(&profile_start);
//#endif
    }

    void ProfileEnd()
    {
//#ifdef _DEBUG
        LARGE_INTEGER end;
        QueryPerformanceCounter(&end);
        end.QuadPart -= profile_start.QuadPart;
        printf("Ms %f\n", (double)end.LowPart * NewTimer::fFreq);
//#endif
    }

    void Initialize()
    {
        QueryPerformanceCounter(&startTime);
        QueryPerformanceFrequency(&freq);
        fFreq = FPS((double)freq.QuadPart);
        ticks_hybrid = freq.QuadPart / 75;

        if (!p_bWindowed)
            timer_lock = 0x0D;

        ResetTime();
    }

    bool ResetTimeScript(CStruct* pStruct, CScript* pScript)
    {
        ResetTime();
        return QScript::ResetClock(pStruct, pScript);
    }
    void ResetTime()
    {
        debug_print("ResetTime\n");

        //First reset framelength
        *(float*)0x00850FD0 = (float)FPS_LOWRES(Gfx::target_fps);
        target = FPS(Gfx::target_fps);
        framelength = target;
        hybrid_loop_time = 0;

        //old time is used to reset script timers
        DWORD old_time = GetTime();

        //Reset timer
        timeBeginPeriod(1);
        reset_time = timeGetTime();
        timeEndPeriod(1);
        time_now = reset_time;

        lastTimeToggleWindowed = 0;

        //Now we need to reset physics, script timers and KeyState timers

        //Sanity check, get latest skater pointer and check if we are initialized(to prevent crashing)
        Game::skater = Skater::UpdateSkater();
        if (Game::skater && !init && init2)
        {
            KeyMap::UpdateKeyMap();
            //Reset specifically used KeyState timers
            KeyState::ResetTimers();

            //Now reseting script timers
            //This is probably not needed(so far didn't find any script running while time is reset)
            //However if it will happen it's good to have this here, else a script might get stuck in endless loop

            //First get the longest wait period of currently running scripts
            DWORD longest_wait = CScript::GetLongestWaitPeriod();

            //if our timer is above longest wait period we can safely increase our timer by this time
            if (reset_time > longest_wait)
            {
                reset_time -= longest_wait;
                CScript::ResetScriptTimers(longest_wait, old_time);
            }
            else//This will be worst case scenario that will probably never happen
                CScript::ResetScriptTimers();

            //Lastly reset Physics(also probably not needed, but do it anyway just in case)
            Game::skater->ResetPhysics();
        }
    }

    void UpdateTimer()
    {
        /*timeBeginPeriod(1);
        time = timeGetTime() - reset_time;
        timeEndPeriod(1);*/
    }

    unsigned long long GetFrameTime()
    {
        return time_now;
    }

    unsigned long long __stdcall GetTime()
    {
        //timer_old_start = timer_time.LowPart;
        timeBeginPeriod(1);
        time_now = timeGetTime() - reset_time;
        timeEndPeriod(1);
        return time_now;

        /*
        QueryPerformanceCounter(&timer_time);
        timer_time.QuadPart -= *(LONGLONG*)0x00850fb0;

        double ms = double(timer_time.LowPart) * fFreq;
        ms += 0.5;
        DWORD truncated = (DWORD)ms;

        if (timer_time.HighPart)
        {
            ms = double(timer_time.HighPart) * fFreq;
            ms += 0.5;
            DWORD truncated2 = (DWORD)ms;
            _asm mov edx, truncated2;
        }
        else
            _asm xor edx, edx

            return truncated;*/
    }

    LARGE_INTEGER TimerStart_Hybrid()
    {
        old_start = startTime;


        QueryPerformanceCounter(&startTime);
        old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
        double ms = (double((old_start.LowPart)) * fFreq);

        //Only update framelength if above 25 fps
        if (ms < FPS(20))
        {
            framelength = ms;
            //Only update FPS lock if window is active
            if (isActive)
            {
                if (ms >= Gfx::hybrid_high) // ~59.88
                {
                    if (hybrid_limit)
                        hybrid_limit--;
                }
                else
                {
                    if (ms > Gfx::hybrid_high && frameticks > min_frame_ticks)//59,97 fps
                    {
                        /*DWORD diff = (DWORD)((ms - (1000000 / Gfx::target_fps))* 0.005);
                        diff += 4;
                        diff & 20;
                        debug_print("Dec %u\n", diff);*/
                        if (frameticks > 2)
                            frameticks -= 1;
                    }
                    else if (ms < Gfx::hybrid_low && frameticks < max_frame_ticks)//60,24 fps
                    {
                        debug_print("Inc\n");
                        frameticks += 2;
                    }
                    if (hybrid_loop_time > ticks_hybrid)
                        hybrid_limit++;
                }
            }
        }
        else
            framelength = target;
        return startTime;
    }

    LARGE_INTEGER TimerStart()
    {
        UpdateTimer();
        old_start = startTime;


        QueryPerformanceCounter(&startTime);
        old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
        double ms = (double((old_start.LowPart)) * fFreq);
        
        //Only update framelength if above 25 fps
        if (ms < FPS(20))
        {
            framelength = ms;
            //Only update FPS lock if window is active
            if (isActive)
            {
                if (ms > Gfx::exact_high && frameticks > min_frame_ticks)//59,97 fps
                {
                    /*DWORD diff = (DWORD)((ms - (1000000 / Gfx::target_fps))* 0.005);
                    diff += 4;
                    diff & 20;
                    debug_print("Dec %u\n", diff);*/
                    if (frameticks > 2)
                        frameticks -= 1;
                }
                else if (ms < Gfx::exact_low && frameticks < max_frame_ticks)//60,24 fps
                {
                    debug_print("Inc\n");
                    frameticks += 2;
                }
            }
        }
        else
            framelength = target;
        return startTime;
    }

    LARGE_INTEGER TimerStart_Sleep()
    {
        old_start = startTime;


        QueryPerformanceCounter(&startTime);
        old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
        double ms = (double((old_start.LowPart)) * fFreq);
        
        //Only update framelength if above 25 fps
        if (ms < FPS(20))
        {
            framelength = ms;
            //Only update FPS lock if window is active
            if (isActive)
            {
                //debug_print("2nd %f ", ms);

                //We need to cap FPS around 60 because else some physics and scripts will not work correctly
                //Also this is the most fair in a game heavily dependant on speed etc
                //Maybe in the future can change this so can have more FPS
                //Vsync is always on when we are using the new timer
                //So on a screen with 60hz this will not matter too much because we will get perfect 60 FPS
                //However on my screen with 144hz it's pretty hard to get consistent 60 FPS
                //Currently worst case scenario is 59.9-65 and it's usually around 63-64
                //For some reason it's more consistent in window mode than in fullscreen mode
                if (ms >= Gfx::sleep_high) // ~61.01 FPS
                {
                    BYTE target_ms = p_target_ms;
                    if (target_ms > 1)
                    {
                        target_ms--;
                        p_target_ms = target_ms;
                    }
                }
                else if (ms < Gfx::sleep_low) // ~65.02 FPS
                {
                    BYTE target_ms = p_target_ms;
                    if (target_ms < timer_lock)
                    {
                        target_ms++;
                        p_target_ms = target_ms;
                    }
                }
            }
        }
        else
            framelength = target;
        return startTime;
    }

    void TimerElapsed()
    {
        LARGE_INTEGER targetTime;
        targetTime.QuadPart = startTime.QuadPart + frameticks;
        while (endTime.QuadPart < targetTime.QuadPart)
        {
            QueryPerformanceCounter(&endTime);
        }
        /*if (endTime.HighPart == startTime.HighPart)
        {
            //elapsedTime.LowPart = (endTime.LowPart - startTime.LowPart);
            _asm xor edx, edx
            double ms = (double((endTime.LowPart - startTime.LowPart)) * fFreq);
            ms += 0.55f;

            DWORD truncated = ms;
            //004F9462
            return truncated;// (elapsedTime.LowPart * 1000) / freq.LowPart;
        }
        else
        {
            elapsedTime.LowPart = 0xFFFFFFFF - startTime.LowPart + endTime.LowPart;
            _asm xor edx, edx
            //return (elapsedTime.LowPart * 1000) / freq.LowPart;
            double ms = (double(elapsedTime.LowPart) * fFreq);
            ms += 0.55f;

            DWORD truncated = ms;
            return truncated;
        }*/
    }

    //50% sleep 50% loop
    void TimerElapsed_Hybrid()
    {
        QueryPerformanceCounter(&endTime);
        LARGE_INTEGER elapsedTime;
        elapsedTime.QuadPart = (endTime.QuadPart - startTime.QuadPart);
        double ms = ((double)(elapsedTime.LowPart) * fFreq);

        if (hybrid_limit > 2.0)
        {
            ms *= 0.001;
            DWORD truncated = (DWORD)(((hybrid_limit - ms) * 0.5));
            if (truncated && truncated < 14)
                Sleep(truncated);
        }
        else
            debug_print("Use exact\n");

        LARGE_INTEGER targetTime;
        targetTime.QuadPart = startTime.QuadPart + frameticks;
        if (endTime.QuadPart < targetTime.QuadPart)
        {
            QueryPerformanceCounter(&endTime);
            hybrid_loop_time = targetTime.QuadPart - endTime.QuadPart;
        }
        while (endTime.QuadPart < targetTime.QuadPart)
        {
            QueryPerformanceCounter(&endTime);
        }
    }

    DWORD TimerElapsed_Sleep()
    {
        QueryPerformanceCounter(&endTime);
        /*if (endTime.HighPart == startTime.HighPart)
        {
            //elapsedTime.LowPart = (endTime.LowPart - startTime.LowPart);
            _asm xor edx, edx
            double ms = (double((endTime.LowPart - startTime.LowPart)) * fFreq);
            ms += 0.55f;

            DWORD truncated = ms;
            //004F9462
            return truncated;// (elapsedTime.LowPart * 1000) / freq.LowPart;
        }
        else
        {
            elapsedTime.LowPart = 0xFFFFFFFF - startTime.LowPart + endTime.LowPart;
            _asm xor edx, edx
            //return (elapsedTime.LowPart * 1000) / freq.LowPart;
            double ms = (double(elapsedTime.LowPart) * fFreq);
            ms += 0.55f;

            DWORD truncated = ms;
            return truncated;
        }*/

        LARGE_INTEGER elapsedTime;
        elapsedTime.QuadPart = (endTime.QuadPart - startTime.QuadPart);
        double ms = ((double)(elapsedTime.LowPart) * fFreq);
        ms *= 0.0055;

        DWORD truncated = (DWORD)ms;
        return truncated;
    }

    /*DWORD TimerElapsed()
    {
        QueryPerformanceCounter(&endTime);

        if (endTime.HighPart == startTime.HighPart)
        {
            elapsedTime.LowPart = (endTime.LowPart - startTime.LowPart);
            _asm xor edx, edx;
            return (DWORD)((float)elapsedTime.LowPart * fFreq);
        }
        else
        {
            elapsedTime.LowPart = 0xFFFFFFFF - startTime.LowPart + endTime.LowPart;
            _asm xor edx, edx
            return (DWORD)((float)elapsedTime.LowPart * fFreq);
        }
    }*/

};
