#pragma once
#ifndef SKATER_H
#define SKATER_H
#include "_math.h"
#include "KeyState.h"
#include "Script.h"
#include "Collision.h"

bool GetZAngle(CStruct* pParams, CScript* pScript);
bool GetSkaterLook(CStruct* pParams, CScript* pScript);
bool StoreSkaterPos(CStruct* pParams, CScript* pScript);
bool SetSkaterPos(CStruct* pParams, CScript* pScript);
bool GetSkaterPos(CStruct* pParams, CScript* pScript);
bool GetSpeedScript(CStruct* pParams, CScript* pScript);

//Skater is not yet looked into that much.... 
struct Settings
{
    BYTE unk[256];
    typedef bool(__thiscall* const pIsTrue)(Settings* pThis, const char* name);
    bool IsTrue(const char* name)
    {
        return pIsTrue(0x004342E0)(this, name);
    }
};

struct SkaterProfile
{
    BYTE unk[256];

    typedef Settings* (__thiscall* const pGetSettings)(SkaterProfile* pThis);

    Settings* GetSettings()
    {
        return pGetSettings(0x00438B00)(this);
    }
};

struct SkaterProfileContainer
{
    SkaterProfile* profile;

    typedef SkaterProfile* (__cdecl* const GetSkaterProfile)(bool allocate);

    SkaterProfileContainer()
    {
        profile = GetSkaterProfile(0x004367E0)(false);//increas vp count
    }

    typedef void(__cdecl* const ReleaseSkaterProfile)();

    ~SkaterProfileContainer()
    {
        ReleaseSkaterProfile(0x00436830)();//decrease vp count
    }
};
static D3DXVECTOR3 oldCollNormal;
static D3DXVECTOR3 oldHitPoint;
static DWORD oldCollFlag;
static float oldHeight;
//#pragma pack(1)
//Skater: contains information about camera, position, collision etc
EXTERN struct Skater//GetSkaterInfo 004769D0
{
private://0575a190
    //0
    DWORD** memberFunctions;
    //4
    BYTE unk1[0x14];
    //18
    D3DXVECTOR3 position;
    //24
    float positionW;//1.0f
    //28
    D3DXVECTOR3 oldpos;
    //34
    float oldposW;//1.0f
    //38
    BYTE unk2[0x2FC];
    //334
    D3DXVECTOR3 velocity;
    //340
    float random;//1.0 when standingstill, else a high number
    //344
    BYTE unk3[0x10];//Some vector?
    //354
    D3DXMATRIX matrix;
    //394
    BYTE unknown5[0x3E];
    //3d0 isinputdisabled
    //3d1 unk
    //3D2
    bool bailOn;
    //3D3
    bool landedfromvert;
    //3D4
    bool truelandedfromvert;
    //3D5
    BYTE unknown0[0x27B];
    bool blockspin;
    BYTE unknown4[0x7CCF];
    //8320
    bool inVert;
    //8321
    BYTE unknown6[0xF];
    //8330
    bool tracking;
    //8331
    BYTE unknown7[0x1F];
    //8350
    bool canbreakvert;
    //8351
    BYTE unknown8[0x4F];//1 more
    //8380
    bool autoturn;
    //8381
    BYTE unknown[0x57];//1 more
    //83D8
    D3DXMATRIX  old;
    //8418
    D3DXMATRIX  lerp;//1 more
    //8458
    BYTE unknown2[0x58];
    //84D0
    KeyState keystates[12];
    //8620
    BYTE unknown3[0x3C];
    //865C
    D3DXVECTOR3 startcol;
    //8668
    float startcolW;//1.0f
    //866C
    D3DXVECTOR3 endcol;
    //8678
    float endcolW;//1.0f
    //867C
    void* scripts;
    //8680
    DWORD flag;//0 1 idk?
    //8684
    float unk;//some normal??
    //8688
    D3DXVECTOR3 normal;
    //8694
    D3DXVECTOR3 hitpoint;
    //86A0
    BYTE unk4[0x14];
    //86B4
    DWORD collFlags;
    //86B8
    BYTE unk5[0x10];
    //86C8
    DWORD checksumName;
    //86CC
    DWORD nodeIndex;
    //86D0
    BYTE unk6[0x100];
    //87D0
    float height;//relative to ground
    //87D4
    D3DXVECTOR4 displaynormal;
    //87E4
    D3DXVECTOR4 currentnormal;
    //87F4
    D3DXVECTOR4 lastdisplaynormal;
    //8804
    float wtf;
    //8808
    float framelength;
    //880C
    float un1;
    //8810
    float un2;
    //8814
    float normallerp;
    BYTE un[0xA50];
    void* trickFont;//not sure?

//#pragma pop(pack)
public:

    enum class TrickType : BYTE
    {
        Air = 0,
        Lip = 2,
        Ollie = 0x4,
        Grind = 6,
        Manual = 6,
        Gap = 0x10,
        Special = 0x20
    };

    DWORD GetCollisionName()
    {
        return checksumName;
    }

    typedef void(__thiscall* const pAddTrick)(void* pThis, const char* trickName, DWORD trickScore, TrickType type);
    void AddTrick(const char* trickName, DWORD trickScore, TrickType type)
    {
        pAddTrick(0x00435550)(this->trickFont, trickName, trickScore, type);
    }

    typedef bool(__thiscall* const pQueryEvents)(Skater* pThis, CStruct* query, BYTE mask, BYTE ignore);
    bool QueryEvents(CStruct* query, BYTE mask = 1, BYTE ignore = 0xFB)
    {
        return pQueryEvents(0x004B3490)(this, query, mask, ignore);
    }

    void CanSpin(bool spin = true)
    {
        blockspin = !spin;
    }

    bool IsHost()
    {
        return *(bool*)((DWORD)this + 0x84CC);
    }
    DWORD GetCurrentTime()
    {
        static const DWORD timer = 0x00409AE0;
        _asm call timer
        static DWORD temp = 0;
        _asm mov temp, eax
        return temp;
    }

    //the press is between 0x0-0xFF, press below or equal to 0x40 is deadzone
    void UpdateKeyState(BYTE key, DWORD press)
    {

        GetKeyState(key)->Update(this->GetCurrentTime(), press);


        //_printf("Updating press %d\nKeyState %p chc %X\n", press, this, this->checksum);
        /*typedef void(__thiscall* const pUpdate)(KeyState* pThis, DWORD press);
        pUpdate(0x0049BAA0)(this, press);*/
    }

    static DWORD GetCamModeAddress()
    {
        DWORD ptr = *(DWORD*)0x00930BB0;
        ptr = *(DWORD*)ptr;
        return ptr + 0x882C;
    }

    D3DXVECTOR4* GetCurrentNormal()
    {
        return &currentnormal;
    }

    DWORD GetCollFlags() const
    {
        return collFlags;
    }

    float GetHeight() const
    {
        return height;
    }

    typedef void(__thiscall* const pSetNormal)(Skater* pThis, D3DXVECTOR4& normal);
    void SetNormal(D3DXVECTOR4& normal)
    {

        D3DXMATRIX* pMatrix = GetMatrix();
        displaynormal = *(D3DXVECTOR4*)pMatrix->m[Y];
        currentnormal = normal;
        lastdisplaynormal = displaynormal;
        normallerp = 0.1f;
        *(D3DXVECTOR4*)pMatrix->m[Y] = normal;
        OrthoNormalizeAbout(GetMatrix(), Y);
    }
    void SetNormalFast(D3DXVECTOR4& normal)
    {

        D3DXMATRIX* pMatrix = GetMatrix();
        displaynormal = *(D3DXVECTOR4*)pMatrix->m[Y];
        currentnormal = normal;
        lastdisplaynormal = displaynormal;
        normallerp = 1.0f;
        *(D3DXVECTOR4*)pMatrix->m[Y] = normal;
        OrthoNormalizeAbout(GetMatrix(), Y);
    }


    typedef float(__thiscall* const pGetScriptedStat)(Skater* pThis, const char* name);
    float GetScriptedStat(const char* name)
    {
        return pGetScriptedStat(0x0049F670)(this, name);
    }


    typedef bool(__thiscall* const pCallMemberFunction)(Skater* pThis, DWORD checksum, CStruct* pStruct, CScript* pScript);
    bool CallMemberFunction(DWORD checksum, CStruct* pStruct = NULL, CScript* pScript = NULL)
    {
        DWORD* pMemberFunction = memberFunctions[1];
        _printf("Skater -> MemberFunction %p\nName %s pStruct %p pScript %p\n", pMemberFunction, FindChecksumName(checksum), pStruct, pScript);
        return pCallMemberFunction((DWORD)pMemberFunction)(this, checksum, pStruct, pScript);
    }


    bool CheckForWallpant();

    bool IsTracking()
    {
        return tracking;
    }

    bool InVert()
    {
        return inVert;
    }
    void SetLanded(bool value)
    {
        landedfromvert = value;
        truelandedfromvert = value;
    }


    KeyState* GetKeyState(BYTE idx)
    {
        if (idx > 11)
            idx = 11;
        return &keystates[idx];
    }

    float GetFrameLength()
    {
        return framelength;
    }

    static Skater* Instance()
    {
        /*static const DWORD ptr = 0x005D06C0;
        //VALIDATE_PTR((void*)ptr);
        DWORD pSkater = *(DWORD*)ptr + 0x580;
        //VALIDATE_PTR((void*)pSkater);
        pSkater = *(DWORD*)pSkater + 0x4;
        //VALIDATE_PTR((void*)pSkater);
        pSkater = *(DWORD*)pSkater + 0x2C;
        //VALIDATE_PTR((void*)pSkater);
        pSkater = *(DWORD*)pSkater + 0x48;
        if((void*)pSkater == NULL);
        return NULL;
        pSkater = *(DWORD*)pSkater;
        //VALIDATE_DATA((Skater*)pSkater, sizeof(Skater));
        return (Skater*)pSkater;*/
        return Game::skater;
    }

    static Skater* UpdateSkater()
    {
        static const DWORD ptr = 0x005D06C0;
        VALIDATE_PTR((void*)ptr);
        DWORD pSkater = *(DWORD*)ptr + 0x580;
        VALIDATE_PTR((void*)pSkater);
        pSkater = *(DWORD*)pSkater + 0x4;
        VALIDATE_PTR((void*)pSkater);
        pSkater = *(DWORD*)pSkater + 0x2C;
        VALIDATE_PTR((void*)pSkater);
        pSkater = *(DWORD*)pSkater + 0x48;
        pSkater = *(DWORD*)pSkater;
        VALIDATE_DATA((Skater*)pSkater, sizeof(Skater));
        return (Skater*)pSkater;
    }

    D3DXMATRIX* GetMatrix()
    {
        return &matrix;
    }


    //used for spine transfer, don't call this
    EXTERN void Store();

    //used for spine transfer, don't call this
    EXTERN void Restore();

    //used for spine transfer, don't call this
    EXTERN void Slerping();

    //used for spine transfer, don't call this
    EXTERN void MultiplyMatrix(const D3DXMATRIX& delta);

    //used for spine transfer, don't call this
    EXTERN bool CheckForSpine();

    //used for spine transfer, don't call this
    EXTERN void ResetLerpingMatrix();

    float GetAirGravity();


    D3DXVECTOR3* GetCollisionNormal()
    {
        return &normal;
    }



    //Tell Physcs we are inside vert air
    EXTERN void SetVertAir(bool value);

    EXTERN void SetTracking(bool value);

    EXTERN void SetCanBreakVert(bool value);

    typedef void(__thiscall* const pTriggerScript)(Skater* pThis, DWORD triggerType, void*);
    void TriggerScript(DWORD triggerType)
    {
        pTriggerScript(0x0049D070)(this, triggerType, scripts);
    }


    //call this before CheckCollision to set the ray for raytracing.
    EXTERN void SetRay(D3DXVECTOR3 start, D3DXVECTOR3 end);

    void SetNormals()
    {
        *(D3DXVECTOR3*)&currentnormal = normal;
        currentnormal.w = 1.0f;
    }

    //clear 
    typedef void(__thiscall* const pFlagException)(Skater* pThis, const char* name, DWORD unk2);
    void FlagException(const char* name, DWORD unk2 = 0)
    {
        pFlagException(0x0048F990)(this, name, unk2);
    }

    EXTERN bool CollisionCheck(Collision::Flags flag, bool ignore = false);

    D3DXVECTOR3* GetNormal()
    {
        return &*(D3DXVECTOR3*)&currentnormal;
    }


    D3DXVECTOR3* GetHitPoint()
    {
        return &hitpoint;
    }


    D3DXVECTOR3* GetOldPosition()
    {
        return &oldpos;
    }

    D3DXVECTOR3* GetPosition()
    {
        return &position;
    }


    void SetPosition(D3DXVECTOR3 pos)
    {
        oldpos = position;
        position = pos;

    }


    D3DXVECTOR3* GetVelocity()
    {
        /*DWORD pSpeed = (DWORD)this + 0x334;
        VALIDATE_DATA((D3DXVECTOR3*)pSpeed, sizeof(D3DXVECTOR3));

        return (D3DXVECTOR3*)pSpeed;*/
        return &velocity;
    }
};

//is in graf game?
bool Modulating();
#endif