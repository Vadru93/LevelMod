#include "pch.h"
#include "Bugfixes.h"

void FixRenderBugs()
{
    //const DWORD renderjmpAddr1 = 0x004f4ba9;
    const BYTE renderjmp1[] = { 0xE9, 0x10, 0xB6, 0xF0, 0xFF };
    DWORD old;
    VirtualProtect((void*)0x004f4ba9, sizeof(renderjmp1), PAGE_EXECUTE_READWRITE, &old);
    //memcpy((void*)0x004f4ba9, renderjmp1, sizeof(renderjmp1));
    //VirtualProtect((void*)0x004f4ba9, sizeof(renderjmp1), old, &old);
    const BYTE renderFix1[] = { 0x85, 0xD2, 0x0F, 0x84, 0x10, 0x4A, 0x0F, 0x00, 0x89, 0x0A, 0x8B, 0x48, 0x10, 0xE9, 0xDE, 0x49, 0x0F, 0x00 };
    VirtualProtect((void*)0x004001be, sizeof(renderFix1), PAGE_EXECUTE_READWRITE, &old);
    //memcpy((void*)0x004001be, renderFix1, sizeof(renderFix1));
    //VirtualProtect((void*)0x004001be, sizeof(renderFix1), old, &old);

    VirtualProtect((void*)0x004FEE77, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEE77 = 0x008920CC;// (DWORD)&RenderFix;
    //VirtualProtect((void*)0x004FEE77, 4, old, &old);

    VirtualProtect((void*)0x004FEEA4, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEEA4 = 0x008920CC;//(DWORD)&RenderFix;
    //VirtualProtect((void*)0x004FEEA4, 4, old, &old);


    VirtualProtect((void*)0x004FEEAF, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEEAF = 0x008920CC;//(DWORD)&RenderFix;
    //VirtualProtect((void*)0x004FEEAF, 4, old, &old);

    VirtualProtect((void*)0x004FEEC6, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEEC6 = 0x008920CC;//(DWORD)&RenderFix;
    //VirtualProtect((void*)0x004FEEC6, 4, old, &old);

    VirtualProtect((void*)0x004FF050, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FF050 = 0x008920CC;//(DWORD)&RenderFix;


    static BYTE pFix[0xA401] = { 0 };
    VirtualProtect((void*)0x004FEE51, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF064, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEE51 = (DWORD)&pFix + 1;
    *(DWORD*)0x004FF064 = (DWORD)&pFix;

    /*VirtualProtect((void*)0x004FEBEE, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEBEE = 0x0008AC1C;
    VirtualProtect((void*)0x004FEC88, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEC88 = 0x0008AC1C;
    VirtualProtect((void*)0x004FECDC, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FECDC = 0x0008AC1C;


    VirtualProtect((void*)0x004FF2C1, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FF2C1 = 0x0008AC20;
    VirtualProtect((void*)0x004FF2E1, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FF2E1 = 0x0008AC20;

    VirtualProtect((void*)0x004FEB9A, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEC36, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEB9A = 0x0008AC34;
    *(DWORD*)0x004FEC36 = 0x0008AC34;

    VirtualProtect((void*)0x004FEBC7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEC43, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FEBC7 = 0x0008AC38;
    *(DWORD*)0x004FEC43 = 0x0008AC38;


    //NS Extensions PLG...
    VirtualProtect((void*)0x004FC01A, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC01A = 0x0008AC3C;

    VirtualProtect((void*)0x004FCAE0, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCAE0 = 0x0008AC40;

    VirtualProtect((void*)0x004FC8F7, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC8F7 = 0x0008AC44;

    VirtualProtect((void*)0x004FC0B1, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC0B1 = 0x0008AC48;

    VirtualProtect((void*)0x004FCA3F, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCA3F = 0x0008AC4C;

    VirtualProtect((void*)0x004FC9ED, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC9ED = 0x0008AC50;

    VirtualProtect((void*)0x004FCA91, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCA91 = 0x0008AC54;

    VirtualProtect((void*)0x004FCA91, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00411EF1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00411F21, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00411F56, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0041214B, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00412288, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00412371, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004123E6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00412480, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00412788, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00413661, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004676B9, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00467D9C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00467DE3, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004692F8, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0046A7A1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0046A7E2, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0046AB7E, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00490D69, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F96AE, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FA0FB, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC921, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD0F1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD197, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD341, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD3D1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD491, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD737, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEA36, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEA46, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEDEF, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00501091, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00501BE4, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00501CEA, 4, PAGE_EXECUTE_READWRITE, &old);*/
    /**(DWORD*)0x004FCA91 = 0x0008AC58;
    *(DWORD*)0x00411EF1 = 0x0008AC58;
    *(DWORD*)0x00411F21 = 0x0008AC58;
    *(DWORD*)0x00411F56 = 0x0008AC58;
    *(DWORD*)0x0041214B = 0x0008AC58;
    *(DWORD*)0x00412288 = 0x0008AC58;
    *(DWORD*)0x00412371 = 0x0008AC58;
    *(DWORD*)0x004123E6 = 0x0008AC58;
    *(DWORD*)0x00412480 = 0x0008AC58;
    *(DWORD*)0x00412788 = 0x0008AC58;
    *(DWORD*)0x00413661 = 0x0008AC58;
    *(DWORD*)0x004676B9 = 0x0008AC58;
    *(DWORD*)0x00467D9C = 0x0008AC58;
    *(DWORD*)0x00467DE3 = 0x0008AC58;
    *(DWORD*)0x004692F8 = 0x0008AC58;
    *(DWORD*)0x0046A7A1 = 0x0008AC58;
    *(DWORD*)0x0046A7E2 = 0x0008AC58;
    *(DWORD*)0x0046AB7E = 0x0008AC58;
    *(DWORD*)0x00490D69 = 0x0008AC58;
    *(DWORD*)0x004F96AE = 0x0008AC58;
    *(DWORD*)0x004FA0FB = 0x0008AC58;
    *(DWORD*)0x004FC921 = 0x0008AC58;
    //*(DWORD*)0x004FD0F1 = 0x0008AC58;
    *(DWORD*)0x004FD197 = 0x0008AC58;
    *(DWORD*)0x004FD341 = 0x0008AC58;
    /**(DWORD*)0x004FD3D1 = 0x0008AC58;
    *(DWORD*)0x004FD491 = 0x0008AC58;
    *(DWORD*)0x004FD737 = 0x0008AC58;
    *(DWORD*)0x004FEA36 = 0x0008AC58;
    *(DWORD*)0x004FEA46 = 0x0008AC58;
    *(DWORD*)0x004FEDEF = 0x0008AC58;
    *(DWORD*)0x00501091 = 0x0008AC58;
    *(DWORD*)0x00501BE4 = 0x0008AC58;
    *(DWORD*)0x00501CEA = 0x0008AC58;*/

    /*VirtualProtect((void*)0x004FC949, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC949 = 0x00926828;


    VirtualProtect((void*)0x004FC99B, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC99B = 0x0092682C;
    VirtualProtect((void*)0x004FC06C, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC06C = 0x00926830;


    /*VirtualProtect((void*)0x004FC044, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC7C1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC7F1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC856, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC87D, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC044 = 0x00926834;
    *(DWORD*)0x004FC7C1 = 0x00926834;
    *(DWORD*)0x004FC7F1 = 0x00926834;
    *(DWORD*)0x004FC856 = 0x00926834;
    *(DWORD*)0x004FC87D = 0x00926834;

    /*VirtualProtect((void*)0x004FCABB, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCAE5, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCB56, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCB72, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCB8D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCBA6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCBC6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCC26, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE9C6, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCABB = 0x00926838;
    *(DWORD*)0x004FCAE5 = 0x00926838;
    *(DWORD*)0x004FCB56 = 0x00926838;
    *(DWORD*)0x004FCB72 = 0x00926838;
    *(DWORD*)0x004FCB8D = 0x00926838;
    *(DWORD*)0x004FCBA6 = 0x00926838;
    *(DWORD*)0x004FCBC6 = 0x00926838;
    *(DWORD*)0x004FCC26 = 0x00926838;
    *(DWORD*)0x004FE9C6 = 0x00926838;

    VirtualProtect((void*)0x004FC8CF, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCC46, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCC87, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCDF1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCE46, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FCEEE, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD0D1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE9DA, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF3AB, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC8CF = 0x0092683C;
    *(DWORD*)0x004FCC46 = 0x0092683C;
    *(DWORD*)0x004FCC87 = 0x0092683C;
    *(DWORD*)0x004FCDF1 = 0x0092683C;
    *(DWORD*)0x004FCE46 = 0x0092683C;
    *(DWORD*)0x004FCEEE = 0x0092683C;
    *(DWORD*)0x004FD0D1 = 0x0092683C;
    *(DWORD*)0x004FE9DA = 0x0092683C;
    *(DWORD*)0x004FF3AB = 0x0092683C;

    VirtualProtect((void*)0x004FC0A7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC0D6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC103, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC1F1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC247, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC48C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC746, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE9F6, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FC0A7 = 0x00926840;
    *(DWORD*)0x004FC0D6 = 0x00926840;
    *(DWORD*)0x004FC103 = 0x00926840;
    *(DWORD*)0x004FC1F1 = 0x00926840;
    *(DWORD*)0x004FC247 = 0x00926840;
    *(DWORD*)0x004FC48C = 0x00926840;
    *(DWORD*)0x004FC746 = 0x00926840;
    *(DWORD*)0x004FE9F6 = 0x00926840;

    VirtualProtect((void*)0x0040103F, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00411FC0, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0041258D, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0041264F, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0042E812, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00490011, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004902F9, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0049091C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F3771, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F3D7C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F442F, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F4B01, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FB007, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FB0A6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC973, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD7A6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD7F1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FD961, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FDA17, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FDD1C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE0A6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FED6B, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEDD9, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FFAF8, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FFE35, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FFE91, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x005025A7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00502609, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00504CD2, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x0040103F = 0x00926844;
    *(DWORD*)0x00411FC0 = 0x00926844;
    *(DWORD*)0x0041258D = 0x00926844;
    *(DWORD*)0x0041264F = 0x00926844;
    *(DWORD*)0x0042E812 = 0x00926844;
    *(DWORD*)0x00490011 = 0x00926844;
    *(DWORD*)0x004902F9 = 0x00926844;
    *(DWORD*)0x0049091C = 0x00926844;
    *(DWORD*)0x004F3771 = 0x00926844;
    *(DWORD*)0x004F3D7C = 0x00926844;
    *(DWORD*)0x004F442F = 0x00926844;
    *(DWORD*)0x004F4B01 = 0x00926844;
    *(DWORD*)0x004FB007 = 0x00926844;
    *(DWORD*)0x004FB0A6 = 0x00926844;
    *(DWORD*)0x004FC973 = 0x00926844;
    *(DWORD*)0x004FD7A6 = 0x00926844;
    *(DWORD*)0x004FD7F1 = 0x00926844;
    *(DWORD*)0x004FD961 = 0x00926844;
    *(DWORD*)0x004FDA17 = 0x00926844;
    *(DWORD*)0x004FDD1C = 0x00926844;
    *(DWORD*)0x004FE0A6 = 0x00926844;
    *(DWORD*)0x004FED6B = 0x00926844;
    *(DWORD*)0x004FEDD9 = 0x00926844;
    *(DWORD*)0x004FFAF8 = 0x00926844;
    *(DWORD*)0x004FFE35 = 0x00926844;
    *(DWORD*)0x004FFE91 = 0x00926844;
    *(DWORD*)0x005025A7 = 0x00926844;
    *(DWORD*)0x00502609 = 0x00926844;
    *(DWORD*)0x00504CD2 = 0x00926844;

    VirtualProtect((void*)0x004FCA17, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE1C6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE1E0, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCA17 = 0x00926848;
    *(DWORD*)0x004FE1C6 = 0x00926848;
    *(DWORD*)0x004FE1E0 = 0x00926848;

    VirtualProtect((void*)0x004691DC, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00487776, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0049088B, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x00491910, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F99A1, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F9A4A, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F9E28, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F9F84, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004F9FE4, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FC9C5, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE201, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE232, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE276, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE2C6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE346, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x0050086D, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004691DC = 0x0092684C;
    *(DWORD*)0x00487776 = 0x0092684C;
    *(DWORD*)0x0049088B = 0x0092684C;
    *(DWORD*)0x00491910 = 0x0092684C;
    *(DWORD*)0x004F99A1 = 0x0092684C;
    *(DWORD*)0x004F9A4A = 0x0092684C;
    *(DWORD*)0x004F9E28 = 0x0092684C;
    *(DWORD*)0x004F9F84 = 0x0092684C;
    *(DWORD*)0x004F9FE4 = 0x0092684C;
    *(DWORD*)0x004FC9C5 = 0x0092684C;
    *(DWORD*)0x004FE201 = 0x0092684C;
    *(DWORD*)0x004FE232 = 0x0092684C;
    *(DWORD*)0x004FE276 = 0x0092684C;
    *(DWORD*)0x004FE2C6 = 0x0092684C;
    *(DWORD*)0x004FE346 = 0x0092684C;
    *(DWORD*)0x0050086D = 0x0092684C;

    VirtualProtect((void*)0x004FCA69, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE366, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE3A7, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE601, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE696, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE7A6, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE952, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEA06, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCA69 = 0x00926850;
    *(DWORD*)0x004FE366 = 0x00926850;
    *(DWORD*)0x004FE3A7 = 0x00926850;
    *(DWORD*)0x004FE601 = 0x00926850;
    *(DWORD*)0x004FE696 = 0x00926850;
    *(DWORD*)0x004FE7A6 = 0x00926850;
    *(DWORD*)0x004FE952 = 0x00926850;
    *(DWORD*)0x004FEA06 = 0x00926850;

    VirtualProtect((void*)0x004FCB41, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCB41 = 0x00926854;

    VirtualProtect((void*)0x004FCB12, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE126, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE142, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE166, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FE18E, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FEA16, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FCB12 = 0x00926858;
    *(DWORD*)0x004FE126 = 0x00926858;
    *(DWORD*)0x004FE142 = 0x00926858;
    *(DWORD*)0x004FE166 = 0x00926858;
    *(DWORD*)0x004FE18E = 0x00926858;
    *(DWORD*)0x004FEA16 = 0x00926858;

    VirtualProtect((void*)0x004FF541, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF570, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF57F, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF5A4, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF5C1, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FF541 = 0x0092685C;
    *(DWORD*)0x004FF570 = 0x0092685C;
    *(DWORD*)0x004FF57F = 0x0092685C;
    *(DWORD*)0x004FF5A4 = 0x0092685C;
    *(DWORD*)0x004FF5C1 = 0x0092685C;

    VirtualProtect((void*)0x004FF56B, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF577, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF585, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF591, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF59C, 4, PAGE_EXECUTE_READWRITE, &old);
    VirtualProtect((void*)0x004FF5CB, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004FF56B = 0x00926860;
    *(DWORD*)0x004FF577 = 0x00926860;
    *(DWORD*)0x004FF585 = 0x00926860;
    *(DWORD*)0x004FF591 = 0x00926860;
    *(DWORD*)0x004FF59C = 0x00926860;
    *(DWORD*)0x004FF5CB = 0x00926860;*/




    //004FC7C0 Function that set texture to 00000000

    //VirtualProtect((void*)0x004FF050, 4, old, &old);

    /*DWORD Old = 0x0090b048;
    for (DWORD i = 0; i < 1024; i++)
    {
        *(DWORD*)(RenderFix + i * 4) = *(DWORD*)(old + i * 4);
    }*/

    /*for (DWORD i = 0; i < 4096; i++)
    {
        RenderFix[i] = 0;
    }*/

}

void FixVertBugs()
{
    const BYTE vertjmp1[] = { 0xE9, 0x8A, 0xD6, 0xF5, 0xFF, 0x90, 0x90, 0x90 };
    DWORD old;
    VirtualProtect((void*)0x004A2C39, sizeof(vertjmp1), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x004A2C39, vertjmp1, sizeof(vertjmp1));
    //VirtualProtect((void*)0x004A2C39, sizeof(vertjmp1), old, &old);
    const BYTE vertfix1[116] = {
      0xD9, 0x85, 0x60, 0x87, 0x00, 0x00, 0xD8, 0x5C, 0x24, 0x14, 0xDF, 0xE0, 0x25, 0x00, 0x41, 0x00, 0x00, 0x75, 0x0A, 0x8B, 0x85, 0x60, 0x87, 0x00, 0x00, 0x89, 0x44, 0x24, 0x14, 0x8B, 0x44, 0x24, 0x10, 0x8B, 0x4C, 0x24, 0x14, 0xE9, 0x4F, 0x29, 0x0a, 0x00,
      0x31, 0xFF, 0xD9, 0x85, 0x60, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0x6A, 0x00, 0x6A, 0x08, 0x6A, 0x00, 0x8B, 0xCD, 0xD9, 0x9D, 0x60, 0x86, 0x00, 0x00, 0xD9, 0x85, 0x70, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0xD9, 0x9D, 0x70, 0x86, 0x00, 0x00, 0xE8, 0x5B, 0xF9, 0x09, 0x00, 0x84, 0xC0, 0x0F, 0x85, 0xFC, 0x28, 0x0A, 0x00, 0x47, 0x83, 0xFF, 0x0C, 0x7C, 0xC1, 0xE9, 0x5B, 0x2A, 0x0A, 0x00, 0x00, 0x00, 0x40, 0x40
    };
    VirtualProtect((void*)0x004002c8, sizeof(vertfix1), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x004002c8, vertfix1, sizeof(vertfix1));
    //VirtualProtect((void*)0x004002c8, sizeof(vertfix1), old, &old);
    const BYTE vertjmp2[] = { 0x0F, 0x84, 0xC9, 0xD6, 0xF5, 0xFF };
    VirtualProtect((void*)0x004A2C23, sizeof(vertjmp2), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x004A2C23, vertjmp2, sizeof(vertjmp2));
    //VirtualProtect((void*)0x004A2C23, sizeof(vertjmp2), old, &old);
    /*const BYTE vertfix2 [] = {0x31, 0xFF, 0xD9, 0x85, 0x60, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0x6A, 0x00, 0x6A, 0x08, 0x6A, 0x00, 0x8B, 0xCD, 0xD9, 0x9D, 0x60, 0x86, 0x00, 0x00, 0xD9, 0x85, 0x70, 0x86, 0x00, 0x00, 0xD8, 0x25, 0x38, 0x03, 0x40, 0x00, 0xD9, 0x9D, 0x70, 0x86, 0x00, 0x00, 0xE8, 0x5B, 0xF9, 0x09, 0x00, 0x84, 0xC0, 0x0F, 0x85, 0xFC, 0x28, 0x0A, 0x00, 0x47, 0x83, 0xFF, 0x0C, 0x7C, 0xC1, 0xE9, 0x5B, 0x2A, 0x0A, 0x00, 0x00, 0x00, 0x40, 0x40};
    VirtualProtect((void*)0x004002F2, sizeof(vertfix2), PAGE_EXECUTE_READWRITE, &old);
    memcpy((void*)0x004002F2, vertfix2, sizeof(vertfix2));*/
    const DWORD b = 0x0058EA10;
    VirtualProtect((void*)b, 4, PAGE_EXECUTE_READWRITE, &old);
    *(float*)b = 36.0f;
    //VirtualProtect((void*)b, 4, old, &old);
    /*const DWORD c = 0x004A2BDD;
    VirtualProtect((void*)c, 1, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)c = 0x4B;*/
}

//Limit chat message to 149 characters, to prevent crashing
void FixChatHook(char* chatText)
{
    /*chatText[0] = 0x00;
    chatText[1] = 0x00;*/
    if (chatText && strlen(chatText) > 149)
    {
        chatText[0] = 0x00;
        chatText[1] = 0x00;
        chatText[148] = 0x00;
        chatText[149] = 0x00;
    }
}

void FixChat()
{
    DWORD old;



    VirtualProtect((VOID*)0x004558FB, 5, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x004558FB = 0x5011468D;
    *(BYTE*)0x004558FF = 0x90;
    //VirtualProtect((VOID*)0x004558FB, 5, old, &old);

    VirtualProtect((VOID*)0x00455912, 4, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x00455912 = (DWORD)FixChatHook - 0x00455916;
    //VirtualProtect((VOID*)0x00455912, 4, old, &old);
}

void FixQBParsing()
{
    DWORD old;
    VirtualProtect((void*)0x0042B2CC, 1, PAGE_EXECUTE_READWRITE, &old);
    *(BYTE*)0x0042B2CC = 8;
    //VirtualProtect((void*)0x0042B2CC, 1, old, &old);
    VirtualProtect((void*)0x0042B2D4, 5, PAGE_EXECUTE_READWRITE, &old);
    *(DWORD*)0x0042B2D4 = 0x04408BC3;
    *(BYTE*)0x0042B2D8 = 0xC3;
    //VirtualProtect((void*)0x0042B2D4, 5, old, &old);
}