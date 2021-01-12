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
struct RailNode;
static D3DXVECTOR3 oldCollNormal;
static D3DXVECTOR3 oldHitPoint;
static DWORD oldCollFlag;
static float oldHeight;
static DWORD previus_state;
//#pragma pack(1)
//Skater: contains information about camera, position, collision etc
struct Timer
{
    __inline static DWORD Begin()
    {
        return GetTickCount();
    }
};

EXTERN struct Skater//GetSkaterInfo 004769D0
{


    enum EStateType : DWORD
    {
        GROUND,
        AIR,
        WALL,
        LIP,
        RAIL,
        WALLPLANT
    };


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
    BYTE unk53[0x37];
    //40C
    float mTallyAngles;
    //410
    BYTE unknown1[0x165];
    //575
    bool bRail_Backwards;
    //576
    BYTE unknown9[0x0A];
    //580
    DWORD m_went_airborne_time;
    DWORD m_went_airborne_time2;
    //588
    DWORD m_land_time;
    DWORD m_land_time2;
    //590
    BYTE unknown0[0xC0];
    //650
    bool blockspin;
    //651
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
    BYTE unknown8[0x2F];//1 more
    //8380
    bool bRailSliding;
    BYTE padding;
    BYTE padding1;
    BYTE pdading2;
    DWORD padding3;
    //8388
    DWORD m_stop_sliding_time;
    DWORD m_stop_sliding_time2;
    BYTE unknown10[0x10];
    //83A0
    bool autoturn;
    //83A1
    BYTE unknown[0x57];//1 more
    //83F8
    D3DXMATRIX  lerp;//1 more
    //8438
    D3DXMATRIX  old;
    //8478
    BYTE unkn[0x20];
    //8498
    EStateType m_state;
    //849C
    DWORD unp;
    //84A0
    DWORD m_state_change_timestamp;
    DWORD m_state_change_timestamp2;
    BYTE unknown2[0x28];
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
    //bool trigger;//Maybe?
    //86C8
public:
    DWORD checksumName;
private:
    //86CC
    DWORD nodeIndex;
    //86D0
    DWORD triggerScript;
    //86D4
    BYTE unk7[0x78];
    //874C
    Vertex m_pre_lip_pos;
    //8758
    BYTE unk6[0x78];
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
    public:
    float normallerp;//m_tap_turns
    private:
    //8818
    BYTE u[0x18];
    //8830
        public:
    DWORD m_rail_time;
    DWORD m_rail_time2;
    //8838
    RailNode* mp_rail_node;
    private:
    //883c
    BYTE un[0xA2c];
    //9268
    void* trickFont;//not sure?
    //926c
    BYTE un3[0x8C];
    public:
    //92F8
    DWORD m_last_rail_node_name;


    __inline void SetTrigger(DWORD node_name, DWORD trigger_script)
    {
        unk5[0x0C] = true;
        checksumName = node_name;
        triggerScript = trigger_script;
        //m_last_rail_node_name = node_name;
    }


    __inline void SetGraffitiTrickStarted(bool bStart)
    {
        typedef void(__thiscall* const pSetGraffitiTrickStarted)(Skater* pThis, bool bStart);
        pSetGraffitiTrickStarted(0x004b5220)(this, bStart);
    }

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

    /*void SetState(EStateType state)
        
    {
        DWORD time;
        if (state != m_state)
        {
            time = Timer::Begin();
            m_state_change_timestamp = time;
            previus_state = (DWORD)m_state;
        }

        if (state != LIP)
        {
            m_pre_lip_pos = Vertex(0, 0, 0);
            if (m_state == LIP
                && (m_pre_lip_pos[X] != 0.0f || m_pre_lip_pos[Y] != 0.0f || m_pre_lip_pos[Z] != 0.0f))
            {
                position = m_pre_lip_pos;
                oldpos = position;
            }
        }

        if (state != GROUND) {
            SetGraffitiTrickStarted(true);
        }
        if (m_state == RAIL && state != RAIL && bRailSliding) {
            bRailSliding = false;
            m_stop_sliding_time = time;
        }
        if (m_state == AIR) {
            if (state != AIR) {
                m_land_time = time;
            }
        }
        else if (state == AIR) {
                if (m_state != WALL) {
                    m_went_airborne_time = time;
                }
                mTallyAngles = 0.0f;
                GetKeyState(KeyState::SPINLEFT)->Unpress();
                GetKeyState(KeyState::SPINRIGHT)->Unpress();
                //m_tap_turns = 0.0f;
                normallerp = 0.0f;
        }
    }*/

    static DWORD GetMaxArenaSize()
    {
        typedef DWORD(__cdecl* pGetMaxArenaSize)();
        return pGetMaxArenaSize(0x005586B0)();
    }

    static DWORD GetArenaSize()
    {
        typedef DWORD(__cdecl* pGetArenaSize)();
        return pGetArenaSize(0x005586C0)();
    }

    void SetState(EStateType state)
    {
        /*DWORD start_time_game = GetTime();
        for (DWORD i = 0; i < 100000; i++)
        {
            GetCurrentTime();
            GetCurrentTime();
            GetCurrentTime();
            GetCurrentTime();
        }
        DWORD stop_time_game = GetTime();

        DWORD start_time = GetCurrentTime();
        for (DWORD i = 0; i < 100000; i++)
        {
            GetTime();
            GetTime();
            GetTime();
            GetTime();
        }
        DWORD stop_time = GetCurrentTime();
        sprintf(testing, "%d %d", stop_time_game-start_time_game, stop_time-start_time);
        MessageBox(0, testing, testing, 0);*/
        typedef void(__thiscall* const pSetState)(void* pThis, EStateType state);
        pSetState(0x0049F6D0)(this, state);
    }

    DWORD GetCollisionName()
    {
        return checksumName;
    }

    void maybe_trip_rail_trigger(DWORD type);

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

        D3DXMATRIX* pMatrix = &GetMatrix();
        displaynormal = *(D3DXVECTOR4*)pMatrix->m[Y];
        currentnormal = normal;
        lastdisplaynormal = displaynormal;
        normallerp = 0.1f;
        *(D3DXVECTOR4*)pMatrix->m[Y] = normal;
        OrthoNormalizeAbout(&GetMatrix(), Y);
    }
    void SetNormalFast(D3DXVECTOR4& normal)
    {

        D3DXMATRIX* pMatrix = &GetMatrix();
        displaynormal = *(D3DXVECTOR4*)pMatrix->m[Y];
        currentnormal = normal;
        lastdisplaynormal = displaynormal;
        normallerp = 1.0f;
        *(D3DXVECTOR4*)pMatrix->m[Y] = normal;
        OrthoNormalizeAbout(&GetMatrix(), Y);
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


    KeyState* const __restrict GetKeyState(BYTE idx)
    {
        if (idx > 11)
            idx = 11;
        return &keystates[idx];
    }

    float GetFrameLength()
    {
        return framelength;
    }

    static Skater* const __restrict Instance()
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

    static Skater* const __restrict UpdateSkater()
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
        /*char test_msg[59];
        sprintf(test_msg, "%X\n", &((Skater*)pSkater)->mp_rail_node);
        MessageBox(0, test_msg, test_msg, 0);*/
        return (Skater*)pSkater;
    }

    Matrix& __restrict const GetLerpMatrix()
    {
        return *(Matrix*)&lerp;
    }

    D3DXMATRIX & const __restrict GetMatrix()
    {
        return matrix;
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

    void set_terrain(DWORD terrain)
    {
        typedef void(__thiscall* const p_set_terrain)(Skater* pThis, DWORD terrain);
        p_set_terrain(0x0049BA80)(this, terrain);
    }

    typedef void(__thiscall* const pTriggerScript)(Skater* pThis, DWORD triggerType, void**);
    void TriggerScript(DWORD triggerType)
    {
        pTriggerScript(0x0049D070)(this, triggerType, &scripts);
    }


    //call this before CheckCollision to set the ray for raytracing.
    EXTERN void SetRay(const D3DXVECTOR3 & start, const D3DXVECTOR3 & end);
    void FlipDirection()
    {
        D3DXVECTOR3 temp = this->startcol;
        this->startcol = this->endcol;
        this->endcol = temp;
    }

    void SetNormals()
    {
        *(D3DXVECTOR3*)&currentnormal = normal;
        currentnormal.w = 1.0f;
    }

    __inline void SkateOffRail();
    __inline void MaybeSkateOffRail(bool last_segment, Vertex & extra_dist, RailNode* pFrom, RailNode* pOnto);

    static void skate_off_rail()
    {
        Game::skater->SkateOffRail();
        _asm mov ecx, Game::skater;
    }
  
    //clear 
    typedef void(__thiscall* const pFlagException)(Skater* pThis, const char* name, DWORD unk2);
    void FlagException(const char* name, DWORD unk2 = 0)
    {
        pFlagException(0x0048F990)(this, name, unk2);
    }

    //Default is to ignore hollow collision flag
    __inline bool CollisionCheck()
    {
        return CollisionCheck(Collision::Flags::Hollow, Collision::IGNORE0);
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

    D3DXVECTOR3* const __restrict GetPosition()
    {
        return &position;
    }


    void SetPosition(D3DXVECTOR3 pos)
    {
        oldpos = position;
        position = pos;

    }


    D3DXVECTOR3* const __restrict GetVelocity()
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