#include "pch.h"
#include "Timer.h"
#include <Mmsystem.h>

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
    double framelength = 1000.0/Gfx::target_fps;
    DWORD frameticks = 0;
    double hybrid_limit = 16.0;
    DWORD time = 0;

    void CalculateFPSTimers()
    {
        DWORD frameticks2 = (DWORD)(16666.666666666 / (1000000.0 / (double)freq.QuadPart));
        frameticks = (DWORD)((1000.0 / Gfx::target_fps) / fFreq);
        //Add some headroom for hardware delay and rounding errors, probably should add this to ini for platform specific stored value
        frameticks -= 750;
        DWORD frameticks3 = (DWORD)(0.0166666666 / (1.0 / (double)freq.QuadPart));
        debug_print("f1 %u f2 %u f3 %u\n", frameticks, frameticks2, frameticks3);

        //Special anim speed and maybe more gui speed stuff? 
        //is 0.058 on original 60 fps lock
        *(float*)0x00458B68 = (float)((60.0 / Gfx::target_fps) * 0.058);//Stack value
        *(float*)0x0058E100 = (float)((60.0 / Gfx::target_fps) * 0.058);//Memory value

        Gfx::exact_high = (1000.0 / Gfx::target_fps) + Gfx::exact_high_diff;
        Gfx::exact_low = (1000.0 / Gfx::target_fps) - Gfx::exact_low_diff;
        Gfx::hybrid_high = (1000.0 / Gfx::target_fps) + Gfx::hybrid_high_diff;
        Gfx::sleep_high = (1000.0 / Gfx::target_fps) - Gfx::sleep_high_diff;
        Gfx::sleep_low = (1000.0 / Gfx::target_fps) - Gfx::sleep_low_diff;

        framelength = 1000.0 / Gfx::target_fps;
    }

    void Initialize()
    {
        QueryPerformanceCounter(&startTime);
        QueryPerformanceFrequency(&freq);
        fFreq = 1000.0 / (double)freq.QuadPart;

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
        framelength = 1000.0 / Gfx::target_fps;
        timeBeginPeriod(1);
        reset_time = timeGetTime();
        timeEndPeriod(1);
    }

    void UpdateTimer()
    {
        /*timeBeginPeriod(1);
        time = timeGetTime() - reset_time;
        timeEndPeriod(1);*/
    }

    DWORD __cdecl GetTime()
    {
        //timer_old_start = timer_time.LowPart;
        timeBeginPeriod(1);
        DWORD _time = timeGetTime() - reset_time;
        timeEndPeriod(1);
        _asm xor edx, edx;
        return _time;


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

            return truncated;
    }

    LARGE_INTEGER TimerStart_Hybrid()
    {
        old_start = startTime;


        QueryPerformanceCounter(&startTime);
        old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
        double ms = (double((old_start.LowPart)) * fFreq);
        framelength = ms;
        if (ms < 32.0 && isActive)
        {
            if (ms >= Gfx::hybrid_high) // ~59.88
            {
                if (hybrid_limit)
                    hybrid_limit--;
            }
        }
        return startTime;
    }

    LARGE_INTEGER TimerStart()
    {
        UpdateTimer();
        old_start = startTime;


        QueryPerformanceCounter(&startTime);
        old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
        double ms = (double((old_start.LowPart)) * fFreq);
        framelength = ms;
        if (ms < 32.0 && isActive)
        {
            if (ms > Gfx::exact_high)//59,97 fps
            {
                debug_print("Dec\n");
                if(frameticks > 2)
                   frameticks -= 2;
            }
            else if (ms < Gfx::exact_low)//60,24 fps
            {
                debug_print("Inc\n");
                frameticks++;
            }
        }
        return startTime;
    }

    LARGE_INTEGER TimerStart_Sleep()
    {
        old_start = startTime;


        QueryPerformanceCounter(&startTime);
        old_start.QuadPart = startTime.QuadPart - old_start.QuadPart;
        double ms = (double((old_start.LowPart)) * fFreq);
        framelength = ms;
        if (ms < 32.0 && isActive)
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

        if (hybrid_limit > 3.0)
        {
            DWORD truncated = (DWORD)(((hybrid_limit - ms) * 0.5));
            if (truncated && truncated < 14)
                Sleep(truncated);
        }
        else
            debug_print("Use exact\n");

        LARGE_INTEGER targetTime;
        targetTime.QuadPart = startTime.QuadPart + frameticks;
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
        ms += 0.55;

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
