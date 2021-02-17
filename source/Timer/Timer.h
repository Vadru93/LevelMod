#ifndef TIMER_H
#define TIMER_H
#define startTime (*(LARGE_INTEGER*)0x008E8E18)

namespace NewTimer
{
    extern DWORD reset_time;
    extern LARGE_INTEGER timer_time;
    extern double fFreq;
    extern LARGE_INTEGER freq;
    extern LARGE_INTEGER endTime;
    extern LARGE_INTEGER elapsedTime;
    extern LARGE_INTEGER old_start;
    extern DWORD timer_lock;
    extern double framelength;
    extern DWORD frameticks;
    extern double hybrid_limit;



    void CalculateFPSTimers();
    void Initialize();
    void ResetTime();
    bool ResetTimeScript(CStruct* pStruct, CScript* pScript);

    DWORD __cdecl GetTime();

    LARGE_INTEGER TimerStart_Hybrid();
    LARGE_INTEGER TimerStart();
    LARGE_INTEGER TimerStart_Sleep();

    void TimerElapsed();
    void TimerElapsed_Hybrid();
    DWORD TimerElapsed_Sleep();

};

#endif