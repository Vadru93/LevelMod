#pragma once
#ifndef OBSERVE_H
#define OBSERVE_H
#include "Skater.h"


struct ObserveMode
{
    //PlayerInfo: 0x008E2498; + 0x13C;
    float cameraAngle;
    Skater* skater;
    //Vertex* pos;
    Vertex* camera;
    D3DXVECTOR3 currentDistance;
    //Vertex* velocity;

    void* current;
    void* first;
    void* last;
    char name[64];
    DWORD timeNext;
    bool observing;

    ObserveMode()
    {
        //get data
        UpdateInfo(true);
    }

    void Enter();

    void Leave()
    {
        observing = false;
    }

    static Vertex* GetCamera()
    {
        static const DWORD ptr = 0x008E1E78;
        if (bDebugMode && InvalidReadPtr(((void*)ptr)))
            return NULL;
        DWORD pCamera = *(DWORD*)ptr + 0x84;
        if (bDebugMode && InvalidReadPtr(((void*)pCamera)))
            return NULL;
        pCamera = *(DWORD*)pCamera + 0xB0;
        if (bDebugMode && InvalidReadPtr(((void*)pCamera)))
            return NULL;
        pCamera = *(DWORD*)pCamera + 0x320;
        if (bDebugMode && InvalidReadPtr(((void*)pCamera)))
            return NULL;
        pCamera = *(DWORD*)pCamera + 0x20C;
        if (bDebugMode && InvalidReadPtr(((void*)pCamera)))
            return NULL;
        return (Vertex*)(*(DWORD*)pCamera + 0x40);
    }

    DWORD GetCamModeAddress()
    {
        _printf("Getting camera addr ");
        DWORD addr = *(DWORD*)((DWORD)first + 0x14);
        _printf("%p\n", addr);
        addr += 0x882C;
        return addr;
    }

    //check if player exist and pointers work
    bool PlayerExist()
    {
        if (current && (!bDebugMode || !InvalidReadPtr(current)) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF && skater)
        {
            _printf("wtf no exist\n");
            return true;
        }

        first = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x13C);
        last = (void*)*(DWORD*)(*(DWORD*)0x008E2498 + 0x140);
        current = first;
        while (current != last && current && (!bDebugMode || !InvalidReadPtr(current)) && *(DWORD*)current == 0x0058E504 && *(DWORD*)((DWORD)current + 4) != 0xFFFFFFFF && skater)
        {
            if (!strcmp(name, (char*)((DWORD)current + 32)))
                return true;
            current = *(void**)((DWORD)current + 12);
            if (!current)
                current = first;
        }
        UpdateInfo();
        return false;
    }

    //update all pointers and information
    void UpdateInfo(bool firstTime = false);

    //follow next player
    void Next(DWORD time = 0);//curr +12

    //Old code
    void InterpolateCamera();

    //called each frame, update camera pos
    void Update();


};

#endif