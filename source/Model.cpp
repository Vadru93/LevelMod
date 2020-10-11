#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "SuperSectors.h"


__declspec(naked) void Obj_MoveToNode_Naked()
{
    static Model* pModel;
    static CStruct* pStruct;
    static DWORD pOldESP;
    static DWORD pOldEBP;
    static DWORD pCall = 0x0048B6E0;
    static DWORD pJmp = 0x0048E03A;
    _asm mov pModel, ecx;
    _asm mov pStruct, eax;
    _asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;
    Obj_MoveToNode(pModel, pStruct);
    _asm mov ecx, pModel;
    _asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;
    _asm call[pCall];
    _asm jmp pJmp;
}

__declspec(naked) void Obj_FollowPathLinked_Naked()
{
    static Model* pModel;
    static CStruct* pStruct;
    static DWORD pOldESP;
    static DWORD pOldEBP;
    static DWORD pCall = 0x0048B500;
    static DWORD pJmp = 0x0048DA57;
    _asm mov pModel, ecx;
    _asm mov pStruct, eax;
    _asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;
    Obj_FollowPathLinked(pModel, pStruct);
    _asm mov ecx, pModel;
    _asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;
    _asm call[pCall];
    _asm jmp pJmp;
}

__declspec(naked) void BouncyObj_Go_Naked()
{
    static Model* pModel;
    static CStruct* pStruct;
    static DWORD pOldESP;
    static DWORD pOldEBP;
    static DWORD pJmp = 0x004842A0;
    _asm mov pModel, ecx;
    _asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;
    BouncyObj_Go(pModel);
    _asm mov ecx, pModel;
    _asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;
    _asm jmp pJmp;
}

__declspec(naked) void BouncyObj_OnBounce_Naked()
{
    static DWORD pJmp = 0x004003BE;
    static DWORD pOldESP;
    static DWORD pOldEBP;
    static BYTE flags;
    static Model* pModel;
    /*_asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;*/
    _asm pushad;
    _asm pushfd;
    _asm mov al, [esp + 0x100];
    _asm mov flags, al;

    //Check if our Collision was Hollow or not
    if (!(flags & (BYTE)Collision::Flags::Hollow))//Collidable. get the model pointer
    {
        _asm mov pModel, esi;
    }
    else//Hollow, set model to NULL
        pModel = NULL;
    BouncyObj_OnBounce(pModel);
    /*_asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;
    _asm xor eax, eax
    _asm mov al, [esp + 0x100];*/
    _asm popfd;
    _asm popad;
    _asm jmp pJmp;

}

void BouncyObj_OnBounce(Model* mdl)
{
    if (mdl)//We have collided, so let's check if BounceScript exists on this Node
    {
        CStructHeader* node = Node::GetNodeStructByIndex(mdl->GetNodeIndex());

        if (node)
        {
            CStructHeader* name;
            if (node->GetStruct(Checksums::Name, &name))
                _printf("BouncyObj_OnBounce: %s\n", FindChecksumName(name->Data));
            CStructHeader* BounceScript;

            if (node->GetStruct(Checksums::BounceScript, &BounceScript))//BounceScript exists, let's spawn it!
            {
                _printf("Going to spawn BounceScript %s\n", FindChecksumName(BounceScript->Data));
                QScript::SpawnScript(BounceScript->Data, 0, mdl->GetNodeIndex());
            }

            /*CStructHeader* BounceSound;

            if (node->GetStruct(Checksums::BounceSound, &BounceSound) && BounceSound->Data)
            {
                _printf("BounceSound %s\n", FindChecksumName(BounceSound->Data));
                CScript script;
                CStruct params(QBKeyHeader::LOCAL, BounceSound->Data);
                Game::PlaySound(&params, &script);

            }*/
        }
    }

    DWORD trigger = p_trigger_node;
    if (trigger)//We have hit a trigger poly
    {
        _printf("BouncyObj has hit a trigger poly\n");
        p_trigger_node = 0;//Reset the trigger checksum so we only trigger once
        DWORD index = 0;

        //Now we need to check if the Node has a TriggerScript
        CStructHeader* node = Node::GetNodeStructAndIndex(trigger, index);

        if (node)
        {
            CStructHeader* TriggerScript;
            if (node->GetStruct(Checksums::TriggerScript, &TriggerScript))//The Node has TriggerScript, now we need to check if we should spawn it...
            {
                QBKeyHeader* header = GetQBKeyHeader(TriggerScript->Data);
                if (header && header->type == QBKeyHeader::QBKeyType::SCRIPTED_FUNCTION)
                {
                    //This is a hacky test to see if the Script will Shatter something
                    char* pScript = header->pStr;

                    char opcode = *pScript;

                    while (opcode != 0x24)
                    {
                        pScript++;
                        switch (opcode)
                        {
                        case 0x49:
                        case 0x48:
                        case 0x47:
                            pScript += 2;
                            break;

                        case 0x1C:
                        case 0x1B:
                            pScript += *(DWORD*)pScript + 4;
                            break;

                        case 0x17:
                        case 0x1A:
                        case 0x2E:
                        case 2:
                            pScript += 4;
                            break;

                        case 1:
                        case 0x3:
                        case 0x4:
                        case 5:
                        case 6:
                        case 7:
                        case 9:
                        case 0x0E:
                        case 0x0F:
                        case 0x18:
                        case 0x23:
                        case 0x25:
                        case 0x26:
                        case 0x28:
                        case 0x2D:
                        case 0x30:
                        case 0x39:
                            break;

                        case 0x16:
                            DWORD key;
                            key = *(DWORD*)pScript;
                            if (key == Checksums::Shatter || key == Checksums::ShatterAndDie || key == trigger)
                            {
                                //This is probably going to Shatter something, let's spawn the TriggerScript
                                _printf("Going to spawn TriggerScript %s\n", FindChecksumName(TriggerScript->Data));
                                QScript::SpawnScript(TriggerScript->Data, 0, index);
                                return;
                            }
                            pScript += 4;
                            break;

                        case 0x1F:
                            pScript += 8;
                            break;

                        case 0x1E:
                            pScript += 12;
                            break;

                        case 0x40:
                        case 0x2F:
                        case 0x37:
                            pScript += *(DWORD*)pScript * 4 + 4;
                            break;

                        default:
                            //Should add all the unhandled opcodes to prevent crashing and other issues
                            _printf("%X @ %p\n", opcode, pScript);
                            MessageBox(0, "Unhandled opcode...", __FUNCTION__, 0);
                            break;
                        }
                        opcode = *pScript;
                    }

                }
            }


        }
        else
            _printf("Couldn't find Node %s\n", trigger);

    }
}

void BouncyObj_Go(Model* mdl)
{
    _printf("Model %p\n", mdl);
    //A BouncyObject has been hit by a skater
    //Now we need to check if there are any scripts to be called, or shadows to be removed
    CStructHeader* node = Node::GetNodeStructByIndex(mdl->GetNodeIndex());

    if (node)
    {
        CStructHeader* collide;
        if (node->GetStruct(Checksums::Shadow, &collide))//Found Shadow
        {
            //Now we need to try and get the SuperSector to be able to kill the shadow
            SuperSector* sector = SuperSector::GetSuperSector(collide->Data);
            if (sector)
            {
                _printf("Killing Shadow %s\n", FindChecksumName(collide->Data));
                sector->SetState(MeshState::kill);
            }
            else
                _printf(__FUNCTION__ " Couldn't find sector %s\n", FindChecksumName(collide->Data));
        }

        if (node->GetStruct(Checksums::CollideScript, &collide))//Found a CollideScript, let's spawn it!
        {
            _printf("Going to spawn script %s\n", FindChecksumName(collide->Data));
            QScript::SpawnScript(collide->Data, NULL, mdl->GetNodeIndex());
        }
    }
    else
        _printf(__FUNCTION__ " Couldn't find Node[%d]\n", mdl->GetNodeIndex());
}



std::map<DWORD, DWORD> movableObjects;

void Obj_MoveToNode(Model* mdl, CStruct* pStruct)
{
    CStructHeader* node = Node::GetNodeStructByIndex(mdl->GetNodeIndex());

    if (node)
    {
        CStructHeader* collision;
        if (node->GetStruct(Checksums::Collision, &collision))
        {
            SuperSector* sector = SuperSector::GetSuperSector(collision->Data);
            if (sector)
            {
                _printf(__FUNCTION__ " -> Going to move collision...\n");

                CStructHeader* pNode;
                if (pStruct->GetStruct(Checksums::Name, &pNode))
                    sector->MoveToNode(pNode->Data);
                else
                    _printf("Need param <Name> in " __FUNCTION__ "\n");

            }
            else
            {
                _printf("Couldn't find SuperSector %s in " __FUNCTION__ "\n", FindChecksumName(collision->Data));
            }
        }
        /*else
            //_printf("No Collision found %X?\n", node);*/
    }
    else
        _printf("Couldn't find NodeIndex %d in " __FUNCTION__ "\n", mdl->GetNodeIndex());
}

void Obj_FollowPathLinked(Model* mdl, CStruct* pStruct)
{
    CStructHeader* node = Node::GetNodeStructByIndex(mdl->GetNodeIndex());

    if (node)
    {
        CStructHeader* collision;
        if (node->GetStruct(Checksums::Collision, &collision))
        {
            SuperSector* sector = SuperSector::GetSuperSector(collision->Data);
            if (sector)
            {
                _printf("model %X\n", mdl);
                _printf(__FUNCTION__ " -> Going to move collision...\n");

                movingObjects.push_back(MovingObject(sector, mdl));

                MovingObject& obj = movingObjects.back();
                obj.vertices = new D3DXVECTOR3[sector->numVertices];
                //std::copy(obj.sector->vertices, obj.sector->vertices + obj.sector->numVertices, obj.vertices);
                D3DXVECTOR3 position = (sector->bboxMax + sector->bboxMin) / 2.0f;

                for (DWORD i = 0; i < sector->numVertices; i++)
                {
                    obj.vertices[i] = (sector->vertices[i] - position);
                }
            }
            else
            {
                _printf("Couldn't find SuperSector %s in " __FUNCTION__ "\n", FindChecksumName(collision->Data));
            }
        }
    }
    else
        _printf("Couldn't find NodeIndex %d in " __FUNCTION__ "\n", mdl->GetNodeIndex());
}