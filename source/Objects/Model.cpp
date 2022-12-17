#include "pch.h"
#include "Model.h"
#include "Script\Node.h"
#include "SuperSectors.h"


__declspec(naked) void Obj_MoveToNode_Naked()
{
    static Model* pModel;
    static CStruct* pStruct;
    static DWORD pOldESP;
    static DWORD pOldEBP;
    static DWORD pCall = 0x0048B6E0;
    static DWORD pJmp = 0x0048E03A;
    _asm pushad;
    _asm pushfd;
    _asm mov pModel, ecx;
    _asm mov pStruct, eax;
    /*_asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;*/
    Obj_MoveToNode(pModel, pStruct);
    _asm mov ecx, pModel;
    /*_asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;*/
    _asm popfd;
    _asm popad;
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
    _asm pushad;
    _asm pushfd;
    _asm mov pModel, ecx;
    _asm mov pStruct, eax;
    /*_asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;*/
    Obj_FollowPathLinked(pModel, pStruct);
    _asm mov ecx, pModel;
    /*_asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;*/
    _asm popfd;
    _asm popad;
    _asm call[pCall];
    _asm jmp pJmp;
}

__declspec(naked) void BouncyObj_Go_Naked()
{
    static Model* pModel;
    static CStruct* pStruct;
    /*static DWORD pOldESP;
    static DWORD pOldEBP;*/
    _asm pushad;
    _asm pushfd;
    static DWORD pJmp = 0x004842A0;
    _asm mov pModel, ecx;
    /*_asm mov pOldESP, esp;
    _asm mov pOldEBP, ebp;*/
    BouncyObj_Go(pModel);
    _asm mov ecx, pModel;
    /*_asm mov esp, pOldESP;
    _asm mov ebp, pOldEBP;*/
    _asm popfd;
    _asm popad;
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
                debug_print("BouncyObj_OnBounce: %s\n", FindChecksumName(name->Data));
            CStructHeader* BounceScript;

            if (node->GetStruct(Checksums::BounceScript, &BounceScript))//BounceScript exists, let's spawn it!
            {
                debug_print("Going to spawn BounceScript %s\n", FindChecksumName(BounceScript->Data));
                QScript::SpawnScript(BounceScript->Data, 0, mdl->GetNodeIndex());
            }

            /*CStructHeader* BounceSound;

            if (node->GetStruct(Checksums::BounceSound, &BounceSound) && BounceSound->Data)
            {
                debug_print("BounceSound %s\n", FindChecksumName(BounceSound->Data));
                CScript script;
                CStruct params(QBKeyHeader::LOCAL, BounceSound->Data);
                Game::PlaySound(&params, &script);

            }*/
        }
    }

    DWORD trigger = p_trigger_node;
    if (trigger)//We have hit a trigger poly
    {
        debug_print("BouncyObj has hit a trigger poly\n");
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

                    while (opcode != QScript::ScriptToken::EndScript)
                    {
                        pScript++;
                        switch (opcode)
                        {
                        case QScript::ScriptToken::If2:
                        case QScript::ScriptToken::Else2:
                        case QScript::ScriptToken::EndSwitch2:
                            pScript += 2;
                            break;

                        case QScript::ScriptToken::String:
                        case QScript::ScriptToken::LocalString:
                            pScript += *(DWORD*)pScript + 4;
                            break;

                        case QScript::ScriptToken::Int:
                        case QScript::ScriptToken::Float:
                        case QScript::ScriptToken::Jump:
                        case QScript::ScriptToken::NewLineNumber:
                            pScript += 4;
                            break;

                            //Known key values
                        case QScript::ScriptToken::NewLine:
                        case QScript::ScriptToken::Struct:
                        case QScript::ScriptToken::EndStruct:
                        case QScript::ScriptToken::Array:
                        case QScript::ScriptToken::EndArray:
                        case QScript::ScriptToken::Equals:
                        case QScript::ScriptToken::Property:
                        case QScript::ScriptToken::Comma:
                        case QScript::ScriptToken::Parenthesis:
                        case QScript::ScriptToken::EndParenthesis:
                        case QScript::ScriptToken::Begin:
                        case QScript::ScriptToken::Repeat:
                        case QScript::ScriptToken::Break:
                        case QScript::ScriptToken::Script:
                        case QScript::ScriptToken::If:
                        case QScript::ScriptToken::Else:
                        case QScript::ScriptToken::ElseIf:
                        case QScript::ScriptToken::EndIf:
                        case QScript::ScriptToken::Global:
                        case QScript::ScriptToken::RandomRange:
                        case QScript::ScriptToken::Not:
                            break;

                        case QScript::ScriptToken::QBKey:
                            DWORD key;
                            key = *(DWORD*)pScript;
                            if (key == Checksums::Shatter || key == Checksums::ShatterAndDie || key == trigger)
                            {
                                //This is probably going to Shatter something, let's spawn the TriggerScript
                                debug_print("Going to spawn TriggerScript %s\n", FindChecksumName(TriggerScript->Data));
                                QScript::SpawnScript(TriggerScript->Data, 0, index);
                                return;
                            }
                            pScript += 4;
                            break;

                        case QScript::ScriptToken::Pair:
                            pScript += 8;
                            break;

                        case QScript::ScriptToken::Vector:
                            pScript += 12;
                            break;

                        case QScript::ScriptToken::Random:
                        case QScript::ScriptToken::RandomPermute:
                        case QScript::ScriptToken::RandomNoRepeat:
                            pScript += *(DWORD*)pScript * 4 + 4;
                            break;

                        default:
                            //Should add all the unhandled opcodes to prevent crashing and other issues
                            debug_print("%X @ %p\n", opcode, pScript);
                            MessageBox(0, "Unhandled opcode...", __FUNCTION__, 0);
                            break;
                        }
                        opcode = *pScript;
                    }

                }
            }


        }
        else
            debug_print("Couldn't find Node %s\n", trigger);

    }
}

void BouncyObj_Go(Model* mdl)
{
    debug_print("Model %p\n", mdl);
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
                debug_print("Killing Shadow %s\n", FindChecksumName(collide->Data));
                sector->SetState(MeshState::kill);
            }
            else
                debug_print(__FUNCTION__ " Couldn't find sector %s\n", FindChecksumName(collide->Data));
        }

        if (node->GetStruct(Checksums::CollideScript, &collide))//Found a CollideScript, let's spawn it!
        {
            debug_print("Going to spawn script %s\n", FindChecksumName(collide->Data));
            QScript::SpawnScript(collide->Data, NULL, mdl->GetNodeIndex());
        }
    }
    else
        debug_print(__FUNCTION__ " Couldn't find Node[%d]\n", mdl->GetNodeIndex());
}


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
                debug_print(__FUNCTION__ " -> Going to move collision...\n");

                CStructHeader* pNode;
                if (pStruct->GetStruct(Checksums::Name, &pNode))
                    sector->MoveToNode(pNode->Data);
                else
                    debug_print("Need param <Name> in " __FUNCTION__ "\n");

            }
            else
            {
                debug_print("Couldn't find SuperSector %s in " __FUNCTION__ "\n", FindChecksumName(collision->Data));
            }
        }
        /*else
            //debug_print("No Collision found %X?\n", node);*/
    }
    else
        debug_print("Couldn't find NodeIndex %d in " __FUNCTION__ "\n", mdl->GetNodeIndex());
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
                debug_print("model %X\n", mdl);
                debug_print(__FUNCTION__ " -> Going to move collision...\n");

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
                debug_print("Couldn't find SuperSector %s in " __FUNCTION__ "\n", FindChecksumName(collision->Data));
            }
        }
    }
    else
        debug_print("Couldn't find NodeIndex %d in " __FUNCTION__ "\n", mdl->GetNodeIndex());
}