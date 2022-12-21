#undef NO_DEFINES
#include "pch.h"
#include "Skater.h"
#include "Physics\Physics.h"
#include "Objects\SuperSectors.h"

Vertex scol, ecol;
CStruct* last_trigger_node = NULL;
DWORD last_trigger_type = 0;
DWORD last_trigger_checksum = 0;
DWORD new_trigger_frame = 0;

bool Modulating()
{
    SkaterProfileContainer skater;
    if (skater.profile)
    {
        Settings* settings = skater.profile->GetSettings();
        if (settings)
            return settings->IsTrue("should_modulate_color");
    }
    return false;
}

typedef bool(__thiscall* const pCollisionCheck)(Skater* pThis, Collision::Flags ignore0, Collision::Flags flags, Collision::Flags ignore1);
EXTERN bool Skater::CollisionCheck(Collision::Flags flag, bool ignore)
{
    if (ignore)
        return pCollisionCheck(0x0049FC80)(this, flag, Collision::Flags::None, Collision::Flags::None);
    else
        return pCollisionCheck(0x0049FC80)(this, Collision::Flags::None, flag, Collision::Flags::None);
}

bool Skater::CollisionCheck_Hook(Collision::Flags ignore0, Collision::Flags flags, Collision::Flags ignore1)
{

    if (Game::skater)
    {
        RwLine line;
        line.start = startcol;

        line.end = endcol;

        WORD ignore = (WORD)ignore0;
        if (ignore)
        {
            *(WORD*)&ignore1 |= ignore;
        }
        ignore0 = flags;
        Collision::CollData cld(ignore0, ignore1);
        bool collided = Collision::FindNearestCollision(line, cld, true);
        if (collided)
        {
            normal = cld.normal;
            hitpoint = cld.point;
            this->collFlags = (DWORD)cld.collFlags;
            this->checksumName = cld.checksum;
            unk = cld.unk;
            flag = collided;
            SkaterCollided();
        }
        return collided;
    }
    return false;
}

bool GetZAngle(CStruct* pParams, CScript* pScript)
{
    static const DWORD ptr = 0x008E1E90;
    VALIDATE_PTR((void*)ptr);
    DWORD pAngle = *(DWORD*)ptr + 0xC0;
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x5C4;
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x0C;
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x10;
    VALIDATE_PTR((void*)pAngle);
    pAngle = *(DWORD*)pAngle + 0x40;
    VALIDATE_DATA((float*)pAngle, sizeof(float));
    CStructHeader* header = pScript->params->head;
    while (header)
    {
        if (header->QBkey == 0xFF7EBAF6)//angle
        {
            header->value.f = *(float*)pAngle * (D3DX_PI / 180.0f);
            break;
        }
        header = header->NextHeader;
    }
    return true;
}

bool GetSkaterLook(CStruct* pParams, CScript* pScript)
{
    static const DWORD ptr = 0x008E2498;
    VALIDATE_PTR((void*)ptr);
    DWORD pPos = *(DWORD*)ptr + 0x13C;
    VALIDATE_PTR((void*)pPos);
    pPos = *(DWORD*)pPos + 0x14;
    VALIDATE_PTR((void*)pPos);
    const DWORD old = pPos;
    pPos = *(DWORD*)pPos + 0x4BC;
    if (InvalidReadPtr((void*)pPos))
    {
        pPos = old;
        pPos = *(DWORD*)pPos + 0x3B4;
        VALIDATE_PTR((void*)pPos);
    }
    pPos = *(DWORD*)pPos + 0x18;
    VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
    Vertex look = *(Vertex*)pPos;
    static const DWORD ptr2 = 0x005D06C0;
    VALIDATE_PTR((void*)ptr2);
    DWORD pSpeed = *(DWORD*)ptr2 + 0x580;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x4;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x2C;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x48;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x334;
    VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));
    CStructHeader* header = pScript->params->head;
    look.x += ((Vertex*)pSpeed)->x;
    look.y += ((Vertex*)pSpeed)->y + 100.0f;
    look.z += ((Vertex*)pSpeed)->z;

    while (header)
    {
        if (header->QBkey == 0x7F261953)
        {
            *header->pVec = *(D3DXVECTOR3*)&look;
            break;
        }
        header = header->NextHeader;
    }
    return true;
}

bool stored = false;
Vertex pos;
float angleAxis;
float angle;

bool StoreSkaterPos(CStruct* pParams, CScript* pScript)
{
    static const DWORD ptr = 0x008E2498;
    VALIDATE_PTR((void*)ptr);
    DWORD pPos = *(DWORD*)ptr + 0x13C;
    VALIDATE_PTR((void*)pPos);
    pPos = *(DWORD*)pPos + 0x14;
    VALIDATE_PTR((void*)pPos);
    const DWORD old = pPos;
    pPos = *(DWORD*)pPos + 0x4BC;
    if (InvalidReadPtr((void*)pPos))
    {
        pPos = old;
        pPos = *(DWORD*)pPos + 0x3B4;
        VALIDATE_PTR((void*)pPos);
    }
    pPos = *(DWORD*)pPos + 0x18;
    VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
    stored = true;
    pos = *(Vertex*)pPos;


    angleAxis = *(float*)(pPos + 0x35C);
    angle = *(float*)(pPos + 0x364);
    return true;
}

bool SetSkaterPos(CStruct* pParams, CScript* pScript)
{
    static const DWORD ptr = 0x008E2498;
    VALIDATE_PTR((void*)ptr);
    DWORD pPos = *(DWORD*)ptr + 0x13C;
    VALIDATE_PTR((void*)pPos);
    pPos = *(DWORD*)pPos + 0x14;
    VALIDATE_PTR((void*)pPos);
    const DWORD old = pPos;
    pPos = *(DWORD*)pPos + 0x4BC;
    if (InvalidReadPtr((void*)pPos))
    {
        pPos = old;
        pPos = *(DWORD*)pPos + 0x3B4;
        VALIDATE_PTR((void*)pPos);
    }
    pPos = *(DWORD*)pPos + 0x18;
    VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
    if (stored)
    {
        static const DWORD ptr = 0x005D06C0;
        VALIDATE_PTR((void*)ptr);
        DWORD pSpeed = *(DWORD*)ptr + 0x580;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x4;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x2C;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x48;
        VALIDATE_PTR((void*)pSpeed);
        pSpeed = *(DWORD*)pSpeed + 0x334;
        VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));
        (*(Vertex*)pSpeed).x = 0.0f;
        (*(Vertex*)pSpeed).y = 0.0f;
        (*(Vertex*)pSpeed).z = 0.0f;
        *(float*)(pPos + 0x35C) = angleAxis;
        *(float*)(pPos + 0x364) = angle;
        *(Vertex*)(pPos + 16) = pos;
        *(Vertex*)pPos = pos;
        *(Vertex*)(pPos + 16) = pos;
        *(Vertex*)pPos = pos;

        *(float*)(pPos + 0x35C) = angleAxis;
        *(float*)(pPos + 0x364) = angle;

        (*(Vertex*)pSpeed).x = 0.0f;
        (*(Vertex*)pSpeed).y = 0.0f;
        (*(Vertex*)pSpeed).z = 0.0f;
    }
    return stored;
}

bool GetSkaterPos(CStruct* pParams, CScript* pScript)
{
    static const DWORD ptr = 0x008E2498;
    VALIDATE_PTR((void*)ptr);
    DWORD pPos = *(DWORD*)ptr + 0x13C;
    VALIDATE_PTR((void*)pPos);
    pPos = *(DWORD*)pPos + 0x14;
    VALIDATE_PTR((void*)pPos);
    const DWORD old = pPos;
    pPos = *(DWORD*)pPos + 0x4BC;
    if (InvalidReadPtr((void*)pPos))
    {
        pPos = old;
        pPos = *(DWORD*)pPos + 0x3B4;
        VALIDATE_PTR((void*)pPos);
    }
    pPos = *(DWORD*)pPos + 0x18;
    VALIDATE_DATA((Vertex*)pPos, sizeof(Vertex));
    CStructHeader* header = pScript->params->head;
    while (header)
    {
        if (header->QBkey == 0x7F261953)
        {
            *header->pVec = *(D3DXVECTOR3*)pPos;
            //header->pVec->z-=400.0f;
            break;
        }
        header = header->NextHeader;
    }
    return true;
}

bool GetSpeedScript(CStruct* pParams, CScript* pScript)
{
    //ExecuteQBScript("DefaultGapScript", pParams, pScript);
    //ExecuteQBScript("DefaultGapScript", pParams, pScript);
    static const DWORD ptr = 0x005D06C0;
    VALIDATE_PTR((void*)ptr);
    DWORD pSpeed = *(DWORD*)ptr + 0x580;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x4;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x2C;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x48;
    VALIDATE_PTR((void*)pSpeed);
    pSpeed = *(DWORD*)pSpeed + 0x334;
    VALIDATE_DATA((Vertex*)pSpeed, sizeof(Vertex));

    CStruct* const __restrict params = pScript->params;
    CStructHeader* const __restrict header = params->AllocateCStruct();
    params->tail->NextHeader = header;
    params->tail = header;
    header->Type = QBKeyHeader::FLOAT;
    header->QBkey = 0xF0D90109;
    const float speed = GetSpeed((const Vertex* const __restrict)pSpeed);
    if (speed > 0)
        header->value.f = speed;
    else
        header->value.f = 100.0f;
    header->NextHeader = NULL;
    //CStructHeader header = CStructHeader(QBKeyHeader::FLOAT, 0x3BD7B54E, speed != 0 ? speed : 100.0f, 0);

    /*typedef const int (__cdecl *const GetQBKeyHeaderFunc)(const unsigned long QBKey);
    const int addr = ((GetQBKeyHeaderFunc)(0x00426340))(0x3BD7B54E);

    if(addr!=0)
    {
    const float speed = GetSpeed((const Vertex* const __restrict)pSpeed);
    if(speed != 0)
    *(float*)(addr+12) = speed;
    else
    *(float*)(addr+12) = 100.0f;

    }
    else
    {
    QBKeyHeader* header = GetQBKeyHeader(0x3BD7B54E);
    if(header)
    {
    const float speed = GetSpeed((const Vertex* const __restrict)pSpeed);
    if(speed != 0)
    header->fVal = speed;
    else
    header->fVal = 100.0f;

    }
    MessageBox(0, "couldn't find script variable" ," LastSpeed", 0);
    return false;
    }*/
    CScript::DumpScripts();
    return true;
}


EXTERN void Skater::SetRay(const D3DXVECTOR3 & start, const D3DXVECTOR3 & end)
{
    this->startcol = start;
    this->endcol = end;


    /*lineVertices[0].x = start.x;
    lineVertices[0].y = start.y;
    lineVertices[0].z = start.z;
    lineVertices[0].rhw = 1.0f;
    lineVertices[0].colour = D3DCOLOR_XRGB(255, 0, 0);
    lineVertices[1].x = end.x;
    lineVertices[1].y = end.y;
    lineVertices[1].z = end.z;
    lineVertices[1].rhw = 1.0f;
    lineVertices[1].colour = D3DCOLOR_XRGB(255, 0, 0);
    numLineVertices = 2;*/

    /*f_AddRay AddRayd = (f_AddRay)GetProcAddress(GetModuleHandle("d3d8.dll"), "AddRay");
    if (AddRayd == NULL)
    MessageBox(0, "WTF?", "", 0);
    AddRayd(start, end, c);*/
}


EXTERN void Skater::SetCanBreakVert(bool value)
{
    canbreakvert = value;
}


EXTERN void Skater::SetVertAir(bool value)
{
    inVert = value;
    landedfromvert = false;
    truelandedfromvert = false;
}

EXTERN void Skater::SetTracking(bool value)
{
    tracking = value;
}


void Skater::MultiplyMatrix(const D3DXMATRIX& delta)
{
    matrix *= delta;
    old = matrix;
    lerp = matrix;

}

void Skater::Slerping()
{
    //debug_print("Slerping\n");
    //MessageBox(0, "trying to slerp", "", 0);

    //KeyState* state = this->GetKeyState(KeyState::UP);

    Slerp::timer += this->GetFrameLength();

    /*if (Slerp::timer > 0.05f && Slerp::done == false)
    {
    Matrix slerp;
    Interporlate(&slerp, 0.5f);
    Matrix inv = Slerp::old;
    inv.InvertUniform();
    Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&slerp);
    this->MultiplyMatrix(delta);
    if (!Slerp::trying)
    Slerp::done = true;
    Slerp::trying = false;
    }*/


    /**(Vertex*)delta.m[X] = Vertex(0.0f, 0.0f, 0.0f);
    delta.m[Y][W]  = 1.0f;
    *(Vertex*)delta.m[Z] = Vertex(0.0f, 0.0f, 0.0f);
    delta.m[W][X] = 0.0f;
    delta.m[W][Y] = 0.0f;
    delta.m[W][Z] = 0.0f;
    delta.m[W][W] = 1.0f;*/
    /*delta.m[W][W] = 1.0f;
    delta.m[X][W] = 0.0f;
    delta.m[Y][W] = 0.0f;
    delta.m[Z][W] = 1.0f;*/
    /*if (test == 0)
      test = 1;
      else if (test == 1)
      test = 2;
      else if (test == 2)
      {*/
    if (Slerp::slerping)
    {
        if (Slerp::timer <= Slerp::duration * 1.0f)
        {
            Matrix slerp;
            Interporlate(&slerp, SmoothStep(ClampMax(Slerp::timer / Slerp::duration, 1.0f)));
            Matrix inv = Slerp::old;
            inv.InvertUniform();
            Matrix delta = Matrix(*(D3DXMATRIX*)&inv * *(D3DXMATRIX*)&slerp);
            this->MultiplyMatrix(delta);
            Slerp::old = slerp;
        }
        else
        {
            debug_print("FINAL\n");
            Slerp::slerping = false;
            //tracking = true;
            /*QBKeyHeader* header = GetQBKeyHeader(crc32f((unsigned char*)"Normal_Lerp_Speed"));
            if (header)
            {

              Slerp::value = header->value.f;
              header->value.f = 0.01f;
            }*/
            if (Slerp::duration < 0.9f)
            {
                Vector norm = *(Vector*)&Slerp::end.m[Y];
                debug_print("norm %f %f %f, end %f %f %f", norm.x, norm.y, norm.z, Slerp::end.m[Y][X], Slerp::end.m[Y][Y], Slerp::end.m[Y][Z]);

                SetNormal(norm);
            }
            else
            {
                /*Vector norm = *(Vector*)&Slerp::end.m[Y];
                SetNormal(norm);*/
                /*CStruct pStruct;
                CScript pScript;
                Skater* skater = Skater::GetSkater();
                skater->CallMemberFunction(Checksums::OrientToNormal, &pStruct, &pScript);*/
            }
            //SetPosition(*GetHitPoint());
        }
    }




    /*if (Slerp::timer < 0.08f)
    {
    /*Matrix slerp;
    D3DXMatrixIdentity(&slerp);
    slerp.Rotate(Slerp::axis, 3.0f*Slerp::timer);
    Matrix inv = Slerp::old;
    inv.InvertUniform();
    Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&slerp);
    this->MultiplyMatrix(delta);
    Slerp::trying = false;
    Slerp::old = slerp;/
    }
    else if (Slerp::done==false)
    {
    Matrix inv = Slerp::old;
    inv.InvertUniform();
    Matrix delta = *(Matrix*)&(*(D3DXMATRIX*)&inv**(D3DXMATRIX*)&Slerp::end);
    this->MultiplyMatrix(delta);
    Slerp::done = true;
    Slerp::trying = false;
    Slerp::old = Slerp::end;
    }*/


    /*if (Slerp::timer >= Slerp::duration)
      matrix *= Slerp::end;
      else
      this->GetMatrix().Rotate(Slerp::axis, Slerp::timer/Slerp::duration);*/
      /*if (GetMatrix().m[Y][Y] > -0.05f)
        this->GetMatrix().Rotate(Slerp::axis, -3.0f*framelength);
      test = 0;
      //}
      /*_asm mov ecx, this
      static DWORD adjustnormal = 0x0049EAC0;
      _asm call adjustnormal*/
    Slerp::addedvel = true;
    Vertex* v = (Vertex*)this->GetVelocity();
    Vertex* p = (Vertex*)this->GetPosition();
    if (v->y < 0.0f && p->y < Slerp::height)	// just check if we have dropped below the target height		
    {
        Slerp::vel.Normalize(0.1f); // make spine velocity very small, but still there, so camera works
        Slerp::addedvel = false;
        /*if (Slerp::timer >= Slerp::duration)
        {
          ResetLerpingMatrix();
          Slerp::transfer = false;
          Slerp::slerping = false;
          SetLanded(Slerp::landed);
        }*/
    }
    else
        *v += Slerp::vel;
    Slerp::last = *v;
}
BYTE old_feeler[0x60*2];
//BYTE old_feeler2[0x60];


void Skater::Store()
{
    /*skpos = position;
    skopos = oldpos;
    skvel = velocity;*/
    memcpy(old_feeler, &m_feeler, 0x60*2);
    //memcpy(old_feeler2, &scripts+0x60, 0x60);
    /*oldCollNormal = normal;
    oldHitPoint = hitpoint;
    oldCollFlag = collFlags;
    oldHeight = height;
    scol = *(Vertex*)&startcol;
    ecol = *(Vertex*)&endcol;*/
}

void Skater::Restore()
{
    /*position = skpos;
    oldpos = skopos;
    skvel = velocity;^*/
    memcpy(&m_feeler, old_feeler, 0x60*2);
    //memcpy(&scripts + 0x60, old_feeler2, 0x60);
    /*normal = oldCollNormal;
    hitpoint = oldHitPoint;
    collFlags = oldCollFlag;
    height = oldHeight;
    scol = *(Vertex*)&startcol;
    ecol = *(Vertex*)&endcol;*/
}

void Skater::CheckEventTrigger(Node::TriggerType type, Collision::CollData& col)
{
    if (col.trigger)
    {
        DWORD checksum = col.checksum;
        if (m_current_trigger_type != type || checksum != last_trigger_checksum || Gfx::frameCounter > new_trigger_frame)
        {
            CArray* node_array = Node::GetNodeArray();
            DWORD node_index = Node::GetNodeIndex(checksum);
            CStruct* node = node_array->GetStructure(node_index);

            if (!SkateMod::ShouldBeAbsentNode(node))
            {
                DWORD trigger_script;
                if (node->GetChecksum(Checksums::TriggerScript, &trigger_script))
                {
                    last_trigger_node = NULL;
                    last_trigger_checksum = checksum;
                    new_trigger_frame = Gfx::frameCounter + 25;
                    m_current_trigger_type = type;
                    m_trigger_script = trigger_script;
                    SpawnAndRunScript(trigger_script, node_index, true, true);
                }
            }
        }
    }
}

void Skater::ResetLerpingMatrix()
{
    old = matrix;
    currentnormal = *(Vector*)&GetMatrix()[Y];
    displaynormal = *(Vector*)&GetMatrix()[Y];
    lastdisplaynormal = *(Vector*)&GetMatrix()[Y];
    normallerp = 0.0f;
    //old = matrix;
}

void HandleTriggers()
{
    Game::skater->HandleTriggers();
}

void Skater::HandleTriggers()
{
    if (GameState::loading_completed && position != old_trigger_pos)
    {
        RwLine line;
        line.start = old_trigger_pos;
        old_trigger_pos = position;
        line.end = position;
        Collision::CollData cld(Collision::trigger_cache->GetCache(line), Collision::Flags::Hollow + Collision::Flags::Trigger, Collision::Flags::None);

        if (Collision::FindNearestCollision(line, cld, true))
        {
            normal = cld.normal;
            hitpoint = cld.point;
            collFlags = (DWORD)cld.collFlags;
            skatable = false;
            trigger = true;
            //MessageBox(0, 0, 0, 0);
        }
    }
}