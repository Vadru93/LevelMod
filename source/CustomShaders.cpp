#include "pch.h"
#include "CustomShaders.h"
#include "SuperSectors.h"
#include "d3d9.h"

#define ReadDWORD() *(DWORD*)pFile; pFile+=4
#define p_loaded_shaders *(bool*)0x00040D22

extern __restrict LPDIRECT3DDEVICE9 pDevice;


/*ShaderObject* shaders = NULL;
DWORD numShaders = 0;*/

enum MaterialEffects
{
    NONE = 0,
    ENV_MAP = 1,
    UV_ANIM = 2,
};

static DWORD PixelShaderTable[8];
//bool loadedShaders = false;
using namespace Gfx;

void Gfx::UnloadShaders()
{
    if (shaders)
    {
        loadingShaders = false;
        loadedShaders = false;
        for (DWORD i = 0; i < numShaders; i++)
        {
            if (shaders[i].numTextures && shaders[i].textures)
            {
                for (DWORD j = 0; j < shaders[i].numTextures; j++)
                {
                    if (shaders[i].textures[j].texture)
                    {
                        shaders[i].textures[j].texture->Release();
                        shaders[i].textures[j].texture = NULL;
                    }
                }
                delete[] shaders[i].textures;
                shaders[i].textures = NULL;
            }
        }
        delete[] shaders;
        shaders = NULL;
        numShaders = 0;
    }
    if (materials)
    {
        delete[] materials;
        materials = NULL;
        numMaterials = 0;
        oldMaterial = NULL;
    }
    if (animations)
    {
        delete[]animations;
        animations = NULL;
        numAnimations = 0;
    }
}

D3DMATERIAL9* NULL_MAT;
D3DMATERIAL9 NULLMAT;

void Mesh::AddShader(ShaderObject* shader, DWORD matIndex)
{
    //matIndex++;
    DWORD pShader = (DWORD)this;
    _printf("pShader %X inedx %d\n", pShader, matIndex);
    pShader += 0x2C;//
    if (matIndex)
    {
        pShader += matIndex * 0x30;
    }
    DWORD pVertexShader = pShader + 4;
    pShader = *(DWORD*)pShader;

    if (!pShader)
    {
        MessageBox(0, "Wrong pointer in AddShader..", "", 0);
        return;
    }

    DWORD retry = pShader;
    pShader = *(DWORD*)pShader;

    if (!pShader)
    {
        Sleep(1000);
        pShader = *(DWORD*)retry;
        if (!pShader)
        {
            _printf("Unloaded texture?\n");
            return;
        }
    }
    char* pTexName = (char*)(pShader + 4 * 4);
    pShader += 0xF0;
    /*if (*(DWORD*)pShader != 0 && *(DWORD*)((*(DWORD*)pShader)) != *(DWORD*)(((DWORD)shader)))
    {
        MessageBox(0, "shader missmatch", "", 0);
        return;
    }*/
    *(DWORD*)pShader = (DWORD)shader;
    if (shader->env_tiling[0])
    {
        char msg[256] = "";
        sprintf(msg, "MatSplit %p Tedxture %s\n%s %X\n", this, pTexName, this->sector ? FindChecksumName(this->sector->name) : "No SuperSector", *(DWORD*)pVertexShader);
        //MessageBox(0, msg, "EnvMap", 0);
        *(DWORD*)pVertexShader = 0x252;
    }


}

void Gfx::LoadCustomShaders(char* path)
{
    /*NULL_MAT = &NULLMAT;
    ZeroMemory(NULL_MAT, sizeof(D3DMATERIAL9));
    loadingShaders = true;
    _printf("OpenShader: %s\n", path);
    FILE* f = fopen(path, "rb");
    bool first = true;
    if (f)
    {
        /*fseek(f, 0, SEEK_END);
        DWORD size = ftell(f);
        BYTE* pFile = new BYTE[size];
        BYTE* oldFile = pFile;
        fseek(f, 0, SEEK_SET);
        fread(pFile, 1, size, f);
        fclose(f);

        DWORD numMats = ReadDWORD();
        numMaterials = 0;
        if (materials)
            delete [] materials;
        _printf("Number of Materials %d\n", numMats);
        if (numMats)
        {
            materials = new D3DMATERIAL9[numMats];

            for (DWORD i = 0; i < numMats; i++)
            {
                materials[numMaterials] = *(D3DMATERIAL9*)pFile;
                pFile += sizeof(D3DMATERIAL9);
                numMaterials++;
            }
        }

        DWORD numAnims = ReadDWORD();
        numAnimations = 0;
        if (animations)
            delete [] animations;
        _printf("Number of Animations %d\n", numAnims);
        if (numMaterials)
        {
            animations = new Animation[numAnims];

            for (DWORD i = 0; i < numAnims; i++)
            {
                animations[numAnimations] = *(Animation*)pFile;
                pFile += sizeof(Animation);
                numAnimations++;
            }
        }


        DWORD numShaderObjects = ReadDWORD();
        _printf("numShaders %d\n", numShaderObjects);
        if (numShaderObjects)
        {
            shaders = new ShaderObject[numShaderObjects * 4];

            for (DWORD i = 0; i < numShaderObjects; i++)
            {
                DWORD checksum = ReadDWORD();
                _printf("Trying to find SuperSector %s...\n", FindChecksumName(checksum));
                DWORD numSplits = ReadDWORD();

                SuperSector* sector = SuperSector::GetSuperSector(checksum);

                if (sector)
                {
                    if (first)
                    {
                        DWORD retries = 0;
                        while (!sector->mesh && retries < 50)
                        {
                            _printf("Waiting for matsplits to load %d(50)...\n", retries + 1);
                            retries++;
                            Sleep(100);
                            if (GameState::GotSuperSectors == false)
                            {
                                _printf("Unloading shaders...\n");
                                UnloadShaders();
                                return;
                            }
                        }
                        Sleep(500);
                        first = false;
                    }
                    _printf("Found sector numSplits %d\n", numSplits);

                    __assume(numSplits < 5);//To prevent warnings
                    for (DWORD j = 0; j < numSplits; j++)
                    {
                        DWORD matIndex = ReadDWORD();
                        *(DWORD*)&shaders[numShaders].shaderId = ReadDWORD();
                        *(DWORD*)&shaders[numShaders].flags = ReadDWORD();

                        DWORD temp = ReadDWORD();
                        if (*(DWORD*)&shaders[numShaders].flags == ENV_MAP)
                            *(DWORD*)&shaders[numShaders].env_tiling[0] = temp;
                        else 
                            shaders[numShaders].anim = animations ? &animations[temp] : NULL;
                        *(DWORD*)&shaders[numShaders].env_tiling[1] = ReadDWORD();

                        DWORD idx = *(DWORD*)pFile;
                        pFile += 4;
                        if (idx > numMaterials)
                            MessageBox(0, "Not good...", "TOO HIGH MATINDEX", 0);
                        shaders[numShaders].material = materials ? &materials[idx] : NULL;

                        shaders[numShaders].blend_op = ReadDWORD();
                        shaders[numShaders].src_blend = ReadDWORD();
                        shaders[numShaders].dest_blend = ReadDWORD();
                        shaders[numShaders].numTextures = ReadDWORD();

                        if (sector->mesh)
                            sector->mesh->AddShader(&shaders[numShaders], matIndex);
                        else
                        {
                            _printf("No Mesh? %p\n", &sector->mesh);
                            for (DWORD k = 0; k < 3; k++)
                            {
                                Sleep(2);
                                if (sector->mesh)
                                {
                                    sector->mesh->AddShader(&shaders[numShaders], matIndex);
                                    break;
                                }
                            }
                        }

                        if (shaders[numShaders].numTextures)
                        {
                            shaders[numShaders].textures = new ShaderTexture[shaders[numShaders].numTextures];

                            for (DWORD k = 0; k < shaders[numShaders].numTextures; k++)
                            {
                                shaders[numShaders].textures[k].fixedAlpha = ReadDWORD();
                                shaders[numShaders].textures[k].blendMode = ReadDWORD();
                                DWORD len = ReadDWORD();
                                memcpy(shaders[numShaders].textures[k].texName, (char*)pFile, len);
                                HRESULT hres = D3DXCreateTextureFromFileExA(pDevice, (char*)pFile, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_FROM_FILE, D3DUSAGE_DYNAMIC, D3DFMT_FROM_FILE, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &shaders[numShaders].textures[k].texture);
                                if (FAILED(hres))
                                {
                                    shaders[numShaders].textures[k].texture = NULL;
                                    _printf("FAILED TO CREATE TEXTURE %s\n", (char*)pFile);
                                }
                                pFile += len;
                                _printf("LOCATION %X\n", pFile);
                            }
                        }
                        else
                        {
                            _printf("fixed alpha? %X\n", pFile);
                            shaders[numShaders].alphaRef = ReadDWORD();
                        }
                        numShaders++;
                    }

                }
                else
                {
                    _printf("Couldn't find sector %s %X\n", FindChecksumName(checksum), pFile);
                    for (DWORD j = 0; j < numSplits; j++)
                    {
                        pFile += 9 * 4;
                        DWORD numTextures = ReadDWORD();

                        if (numTextures)
                        {
                            for (DWORD k = 0; k < numTextures; k++)
                            {
                                pFile += 4 * 2;
                                DWORD len = ReadDWORD();
                                pFile += len;
                            }
                        }
                        else
                        {
                            pFile += 4;
                        }
                    }
                }
            }

            delete[] oldFile;
            //Sleep(1000);
            _printf("Finished loading shaders\n");
     fclose(f);
            loadedShaders = true;
        //}
    }
    //loadingShaders = false;
//loadedShaders = true;*/
}

bool reset = false;
bool restore = false;
bool restoreMaterial = false;
bool restore_matrix = false;

void __stdcall SetVertexShader_hook()
{
    static float t;
    static DWORD oldShader = 0x005CEDB4;
    static DWORD pReturn = 0x00550DC0;
    static DWORD ShaderTextureSkip = (sizeof(ShaderTexture) - 4);
    static D3DMATRIX env_mat = { 0.5f,  0.0f, 0.0f, 0.0f,
                                 0.0f, -0.5f, 0.0f, 0.0f,
                                 0.0f,  0.0f, 0.0f,	0.0f,
                                 0.5f,  0.5f, 0.0f, 0.0f };
    static D3DMATRIX uv_mat = { 1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f };

    static ShaderObject2* pShader = NULL;

    //p_loaded_shaders is false until shaders are fully loaded.
    if (p_loaded_shaders)
    {
        //skip 10 bytes to get pointer to our matsplit
        _asm add esi, 0x10
        _asm mov esi, [esi]

        //Check for NULL
        _asm test esi, esi
        _asm je SKIP;

        //Get pointer to material
        _asm mov esi, [esi];

        //Check for NULL
        _asm test esi, esi;
        _asm je SKIP;

        //Custom shader pointer is is now 240 bytes from here
        _asm add esi, 288;
        //_asm mov edi, [esi];

        //Check if pointer is NULL
        _asm cmp [esi], 0x30303030
        _asm je SKIP;

        //Tell the engine we wanna reset textures next call
        reset = true;


        //Get old pixelshader
        DWORD oldPixelShader = *(DWORD*)0x005CEDB4;

        _asm mov pShader, esi;

        if (pShader->flags == ENV_MAP)
        {
            if (!restore_matrix)
            {
                restore_matrix = true;
                pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            }

            env_mat.m[0][0] = pShader->env_tiling[0] * uv_tiling_threshold;
            env_mat.m[1][1] = pShader->env_tiling[1] * uv_tiling_threshold;

            pDevice->SetTransform(D3DTS_TEXTURE0, &env_mat);
            pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
        }
        else if (pShader->flags == UV_ANIM)
        {
            if (!restore_matrix)
            {
                restore_matrix = true;
                pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            }
            else
                pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);

            t += Game::skater->GetFrameLength() * uv_anim_threshold;
            float uoff = (t * pShader->anim->UVel) + (pShader->anim->UAmplitude * sinf(pShader->anim->UFrequency * t + pShader->anim->UPhase));
            float voff = (t * pShader->anim->VVel) + (pShader->anim->VAmplitude * sinf(pShader->anim->VFrequency * t + pShader->anim->VPhase));

            // Reduce offset mod 16 and put it in the range -8 to +8.
            /*uoff	+= 8.0f;
            uoff	-= (float)(( (int)uoff >> 4 ) << 4 );
            voff	+= 8.0f;
            voff	-= (float)(( (int)voff >> 4 ) << 4 );*/
            uoff -= (float)(int)uoff;
            voff -= (float)(int)voff;


            uv_mat._31 = uoff;//( uoff < 0.0f ) ? ( uoff + 8.0f ) : ( uoff - 8.0f );
            uv_mat._32 = voff;//( voff < 0.0f ) ? ( voff + 8.0f ) : ( voff - 8.0f );

            pDevice->SetTransform(D3DTS_TEXTURE0, &uv_mat);

        }
        else if (restore_matrix)
        {
            restore_matrix = false;
            pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
            pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
        }

        //pDevice->SetMaterial(pShader->material);


        //Get old blend_op
        DWORD old_blend = *(DWORD*)(D3DRS_BLENDOP * 4 + 0x00971948);
        //Set wanted blend_op
        *(DWORD*)(D3DRS_BLENDOP * 4 + 0x00971C18) = pShader->blend_op;
        if (old_blend != pShader->blend_op)
        {
            //Set old blend_op
            *(DWORD*)(D3DRS_BLENDOP * 4 + 0x00971948) = pShader->blend_op;
            pDevice->SetRenderState(D3DRS_BLENDOP, pShader->blend_op);
        }

        old_blend = *(DWORD*)(D3DRS_SRCBLEND * 4 + 0x00971948);
        //Set wanted src_blend
        *(DWORD*)(D3DRS_SRCBLEND * 4 + 0x00971C18) = pShader->src_blend;
        if (old_blend != pShader->src_blend)
        {
            //Set old src_blend
            *(DWORD*)(D3DRS_SRCBLEND * 4 + 0x00971948) = pShader->src_blend;
            pDevice->SetRenderState(D3DRS_SRCBLEND, pShader->src_blend);
        }

        old_blend = *(DWORD*)(D3DRS_DESTBLEND * 4 + 0x00971948);
        //Set wanted dest_blend
        *(DWORD*)(D3DRS_DESTBLEND * 4 + 0x00971C18) = pShader->dest_blend;
        if (old_blend != pShader->dest_blend)
        {
            //Set old blend_op
            *(DWORD*)(D3DRS_DESTBLEND * 4 + 0x00971948) = pShader->dest_blend;
            pDevice->SetRenderState(D3DRS_DESTBLEND, pShader->dest_blend);
        }

         old_blend = *(DWORD*)(D3DRS_ALPHAREF*4 + 0x00971948);
         *(DWORD*)(D3DRS_ALPHAREF * 4 + 0x00971C18) = pShader->alphaRef;
         if (old_blend != pShader->alphaRef)
         {
             *(DWORD*)(D3DRS_ALPHAREF * 4 + 0x00971948) = pShader->alphaRef;
             pDevice->SetRenderState(D3DRS_ALPHAREF, pShader->alphaRef);
         }
        _asm pop edi
        _asm pop esi
        _asm mov esp, ebp
        _asm pop ebp
        _asm jmp[pReturn];

    SKIP:


        //Reset extralayer textures to NULL
        //Maybe also need to reset blendmodes???
        if (reset)
        {

            if (restore_matrix)
            {
                restore_matrix = false;
                pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
                pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
            }

        }

    }
    _asm pop edi
    _asm pop esi
    _asm mov esp, ebp
    _asm pop ebp
    _asm jmp[pReturn];
}


void __stdcall SetVertexShader_hook_old()
{
    static float t;
    static DWORD oldShader = 0x005CEDB4;
    static DWORD pReturn = 0x00550DC0;
    static DWORD ShaderTextureSkip = (sizeof(ShaderTexture) - 4);
    static D3DMATRIX env_mat = { 0.5f,  0.0f, 0.0f, 0.0f,
                                 0.0f, -0.5f, 0.0f, 0.0f,
                                 0.0f,  0.0f, 0.0f,	0.0f,
                                 0.5f,  0.5f, 0.0f, 0.0f };
    static D3DMATRIX uv_mat = { 1.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 1.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f,
                              0.0f, 0.0f, 0.0f, 0.0f };

    static ShaderObject* pShader = NULL;

    //loadedShaders is false until shaders are fully loaded.
    if (loadedShaders)
    {
        //skip 10 bytes to get pointer to our matsplit
        _asm add esi, 0x10
        _asm mov esi, [esi]

            //Check for NULL
            _asm test esi, esi
        _asm je SKIP;

        //Get pointer to material
        _asm mov esi, [esi];

        //Check for NULL
        _asm test esi, esi;
        _asm je SKIP;

        //Custom shader pointer is is now 240 bytes from here
        _asm add esi, 0xF0;
        _asm mov edi, [esi];

        //Check if pointer is NULL
        _asm test edi, edi
        _asm je SKIP;

        //Tell the engine we wanna reset textures next call
        reset = true;


        //Get old pixelshader
        DWORD oldPixelShader = *(DWORD*)0x005CEDB4;

        _asm mov pShader, edi;

        if (false && pShader->shaderId != oldPixelShader)
        {
            //Move the shader into old location
            *(DWORD*)0x005CEDB4 = pShader->shaderId;

            //pDevice->SetPixelShader(pShader->shaderId);
        }
    
        if (pShader->flags == ENV_MAP)
        {
            if (!restore_matrix)
            {
                restore_matrix = true;
                pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            }

            /*env_mat.m[0][0] = pShader->env_tiling[0]*0.5f;
            env_mat.m[1][1] = pShader->env_tiling[1]*0.5f;*/

            pDevice->SetTransform(D3DTS_TEXTURE0, &env_mat);
            pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
        }
        else if (pShader->flags == UV_ANIM)
        {
            if (!restore_matrix)
            {
                restore_matrix = true;
                pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
            }
            else
                pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);

            t += Game::skater->GetFrameLength();
            float uoff = (t * pShader->anim->UVel) + (pShader->anim->UAmplitude * sinf(pShader->anim->UFrequency * t + pShader->anim->UPhase));
            float voff = (t * pShader->anim->VVel) + (pShader->anim->VAmplitude * sinf(pShader->anim->VFrequency * t + pShader->anim->VPhase));

            // Reduce offset mod 16 and put it in the range -8 to +8.
            /*uoff	+= 8.0f;
            uoff	-= (float)(( (int)uoff >> 4 ) << 4 );
            voff	+= 8.0f;
            voff	-= (float)(( (int)voff >> 4 ) << 4 );*/
            uoff -= (float)(int)uoff;
            voff -= (float)(int)voff;


            uv_mat._31 = uoff;//( uoff < 0.0f ) ? ( uoff + 8.0f ) : ( uoff - 8.0f );
            uv_mat._32 = voff;//( voff < 0.0f ) ? ( voff + 8.0f ) : ( voff - 8.0f );

            pDevice->SetTransform(D3DTS_TEXTURE0, &uv_mat);

        }
        else if(restore_matrix)
        {
            restore_matrix = false;
            pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
            pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
        }

        pDevice->SetMaterial(pShader->material);


        //Get old blend_op
        DWORD old_blend = *(DWORD*)(D3DRS_BLENDOP * 4 + 0x00971948);
        //Set wanted blend_op
        *(DWORD*)(D3DRS_BLENDOP * 4 + 0x00971C18) = pShader->blend_op;
        if (old_blend != pShader->blend_op)
        {
            //Set old blend_op
            *(DWORD*)(D3DRS_BLENDOP * 4 + 0x00971948) = pShader->blend_op;
            pDevice->SetRenderState(D3DRS_BLENDOP, pShader->blend_op);
        }

        old_blend = *(DWORD*)(D3DRS_SRCBLEND * 4 + 0x00971948);
        //Set wanted src_blend
        *(DWORD*)(D3DRS_SRCBLEND * 4 + 0x00971C18) = pShader->src_blend;
        if (old_blend != pShader->src_blend)
        {
            //Set old src_blend
            *(DWORD*)(D3DRS_SRCBLEND * 4 + 0x00971948) = pShader->src_blend;
            pDevice->SetRenderState(D3DRS_SRCBLEND, pShader->src_blend);
        }

        old_blend = *(DWORD*)(D3DRS_DESTBLEND * 4 + 0x00971948);
        //Set wanted dest_blend
        *(DWORD*)(D3DRS_DESTBLEND * 4 + 0x00971C18) = pShader->dest_blend;
        if (old_blend != pShader->dest_blend)
        {
            //Set old blend_op
            *(DWORD*)(D3DRS_DESTBLEND * 4 + 0x00971948) = pShader->dest_blend;
            pDevice->SetRenderState(D3DRS_DESTBLEND, pShader->dest_blend);
        }

       /* old_blend = *(DWORD*)(D3DRS_ALPHAREF*4 + 0x00971948);
        *(DWORD*)(D3DRS_ALPHAREF * 4 + 0x00971C18) = pShader->alphaRef;
        if (old_blend != pShader->alphaRef)
        {
            *(DWORD*)(D3DRS_ALPHAREF * 4 + 0x00971948) = pShader->alphaRef;
            pDevice->SetRenderState(D3DRS_ALPHAREF, pShader->alphaRef);
        }*/
        _asm pop edi
        _asm pop esi
        _asm mov esp, ebp
        _asm pop ebp
        _asm jmp[pReturn];

    SKIP:

        //Reset extralayer textures to NULL
        //Maybe also need to reset blendmodes???
        if (reset)
        {

            if (restore_matrix)
            {
                restore_matrix = false;
                pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
                pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_PASSTHRU);
            }
      
        }

    }
    _asm pop edi
    _asm pop esi
    _asm mov esp, ebp
    _asm pop ebp
    _asm jmp[pReturn];
}


__declspec(naked) void __cdecl SetVertexShader_naked()
{
    static DWORD oldShader = 0x005CEDB4;
    static DWORD pReturn = 0x00550DC0;
    static DWORD ShaderTextureSkip = (sizeof(ShaderTexture) - 4);
    static D3DMATRIX env_mat = { 0.5f,  0.0f, 0.0f, 0.0f,
                                 0.0f, -0.5f, 0.0f, 0.0f,
                                 0.0f,  0.0f, 0.0f,	0.0f,
                                 0.5f,  0.5f, 0.0f, 0.0f };

    static D3DMATRIX* p_env_mat = &env_mat;

    //Store esi and edi'
    _asm push edi;
    _asm push esi;
    //loadedShaders is false until shaders are fully loaded.
    _asm mov al, loadedShaders;
    _asm test al, al;
    _asm je SKIP;

    //skip 10 bytes to get pointer to our matsplit
    _asm add esi, 0x10
    _asm mov esi, [esi]

        //Check for NULL
        _asm test esi, esi
    _asm je SKIP;

    //Get pointer to material
    _asm mov esi, [esi];

    //Check for NULL
    _asm test esi, esi;
    _asm je SKIP;

    //Custom shader pointer is is now 240 bytes from here
    _asm add esi, 0xF0;
    _asm mov edi, [esi];

    //Check if pointer is NULL
    _asm test edi, edi
    _asm je SKIP;

    //Tell the engine we wanna reset textures next call
    reset = true;

    //Store ecx, probably not needed?
    _asm push ecx;

    //Get old pixelshader
    _asm mov esi, [0x005CEDB4];

    //Get shaderId
    _asm mov ecx, [edi];

    //Compare old shader to new shader
    //_asm cmp esi, ecx;
    //If same, then skip pixelshader
    //je SKIP_SHADER
    //For now always skip pixelshader
    _asm jmp SKIP_SHADER;

    //Move the shader into old location
    _asm mov esi, 0x005CEDB4;
    _asm mov[esi], ecx

    //Get pointer to d3ddevice
    _asm mov eax, [pDevice];
    //push shader
    _asm push ecx;
    //Get pointer to memberfunctions
    _asm push eax;
    _asm mov ecx, [eax];
    //device->SetPixelShader
    _asm call dword ptr[ecx + 0x000001AC];
SKIP_SHADER:

    //Now we need to set BlendModes
    //Store ebx
    _asm push ebx;


    _asm add edi, 4
    _asm mov eax, [edi];
    _asm add edi, 4
    _asm test eax, eax;
    _asm je DISABLE_ENVMAP;
    _asm mov[env_mat], eax
    _asm mov eax, [edi];
    _asm mov[env_mat + 20], eax;


    _asm mov restore_matrix, 1
    _asm mov edx, [pDevice];
    _asm push p_env_mat;
    _asm push D3DTS_TEXTURE0;
    _asm push edx;
    _asm mov eax, [edx];
    _asm call dword ptr[eax + 0x000000B0];

    _asm mov edx, [pDevice];
    _asm push D3DTTFF_COUNT2;
    _asm push D3DTSS_TEXTURETRANSFORMFLAGS;
    _asm push 0;
    _asm push edx;
    _asm mov eax, [edx];
    _asm call dword ptr[eax + 0x0000010C]

        _asm mov edx, [pDevice];
    _asm push D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR;
    _asm push D3DTSS_TEXCOORDINDEX
    _asm push 0;
    _asm push edx;
    _asm mov eax, [edx];
    _asm call dword ptr[eax + 0x0000010C]
        _asm jmp SKIP_DISABLE_ENVMAP;
DISABLE_ENVMAP:
    _asm mov restore_matrix, 0
    _asm mov edx, [pDevice];
    _asm push D3DTTFF_DISABLE;
    _asm push D3DTSS_TEXTURETRANSFORMFLAGS;
    _asm push 0;
    _asm push edx;
    _asm mov eax, [edx];
    _asm call dword ptr[eax + 0x0000010C];

    _asm mov edx, [pDevice];
    _asm push D3DTSS_TCI_PASSTHRU;
    _asm push D3DTSS_TEXCOORDINDEX
    _asm push 0;
    _asm push edx;
    _asm mov eax, [edx];
    _asm call dword ptr[eax + 0x0000010C];
SKIP_DISABLE_ENVMAP:

    _asm add edi, 4
    _asm mov eax, [Gfx::oldMaterial];
    _asm mov ebx, [edi];
    /*_asm cmp ebx, eax;
    _asm je SKIP_MATERIAL_MODULATION;*/
    //_asm mov restoreMaterial, 1
    _asm mov eax, [pDevice];
    _asm push ebx;
    _asm push eax;
    _asm mov ecx, [eax];
    //Device->SetMaterial
    _asm mov[Gfx::oldMaterial], ebx;
    _asm call dword ptr[ecx + 0xC4];

SKIP_MATERIAL_MODULATION:

    //First BLENDOP
    _asm add edi, 4
    _asm mov eax, [edi];
    _asm add edi, 4;
    _asm mov ebx, D3DRS_BLENDOP;
    //Get old BLENDOP
    _asm mov ecx, [ebx * 4 + 0x00971948];
    _asm lea esi, [ebx * 4 + 0x00971C18];
    _asm mov[esi], eax;
    _asm cmp eax, ecx;
    //if old == new skip
    _asm je SKIP_OP;
    _asm push eax;
    _asm push ebx;
    _asm mov[esi], eax;
    _asm lea esi, [ebx * 4 + 0x00971948];
    _asm mov[esi], eax;
    _asm mov ecx, [0x00970E48];
    _asm mov ecx, [ecx];
    _asm push ecx;
    _asm mov ecx, [ecx];
    //Device->SetRenderState
    _asm call dword ptr[ecx + 0xC8];
SKIP_OP:

    //Then SRCBLEND
    _asm mov eax, [edi];
    _asm add edi, 4;
    _asm mov ebx, D3DRS_SRCBLEND;
    //Get old SRCBLEND
    _asm mov ecx, [ebx * 4 + 0x00971948];
    _asm lea esi, [ebx * 4 + 0x00971C18];
    _asm mov[esi], eax;
    _asm cmp eax, ecx;
    //if old == new skip
    _asm je SKIP_SRC;
    _asm push eax;
    _asm push ebx;
    _asm mov[esi], eax;
    _asm lea esi, [ebx * 4 + 0x00971948];
    _asm mov[esi], eax;
    /*_asm lea esi, [ebx * 4 + 0x00971C18];
    _asm mov[esi], eax;*/
    _asm mov ecx, [0x00970E48];
    _asm mov ecx, [ecx];
    _asm push ecx;
    _asm mov ecx, [ecx];
    //Device->SetRenderState
    _asm call dword ptr[ecx + 0xC8];
SKIP_SRC:

    //last DESTBLEND
    _asm mov eax, [edi];
    _asm add edi, 4;
    _asm mov ebx, D3DRS_DESTBLEND;
    //Get old DESTBLEND
    _asm mov ecx, [ebx * 4 + 0x00971948];
    _asm lea esi, [ebx * 4 + 0x00971C18];
    _asm mov[esi], eax;
    _asm cmp eax, ecx;
    //if old == new skip
    _asm je SKIP_DEST;
    _asm push eax;
    _asm push ebx;
    _asm mov[esi], eax;
    _asm lea esi, [ebx * 4 + 0x00971948];
    _asm mov[esi], eax;
    /*_asm lea esi, [ebx * 4 + 0x00971C18];
    _asm mov[esi], eax;*/
    _asm mov ecx, [0x00970E48];
    _asm mov ecx, [ecx];
    _asm push ecx;
    _asm mov ecx, [ecx];
    //Device->SetRenderState
    _asm call dword ptr[ecx + 0xC8];
SKIP_DEST:


    //Now get numTextures
    _asm xor esi, esi;
    _asm mov ecx, [edi];
    _asm test ecx, ecx
    _asm je ADD_FIXEDALPHA;
    _asm mov edi, [edi + 4];
ADD_TEXTURE:
    //For each texture call Device->SetTexture

    //GetTexture
    _asm mov eax, [edi];
    //Increase texture pass
    _asm inc esi;
    //Push Texture
    _asm push eax
    //Push Pass
    _asm push esi
    //Get Device
    _asm mov ecx, [0x00970E48];
    _asm mov ecx, [ecx];
    _asm push ecx;
    _asm mov ecx, [ecx];
    //Device->SetTexture
    _asm call dword ptr[ecx + 0x000000F4];
    //if we have more textures add them
ADD_FIXEDALPHA:
    _asm add edi, 4;
    _asm mov eax, [edi];
    //_asm test eax, eax
    _asm jmp NO_FIXEDALPHA;
    _asm mov restore, 1
    _asm mov eax, [pDevice];
    _asm push 0x0E;
    _asm push 0x01;
    _asm push esi;
    _asm push eax;
    _asm mov eax, [eax];
    _asm call dword ptr[eax + 0x0000010C]
        _asm mov ebx, D3DRS_TEXTUREFACTOR
    _asm push ecx;
    _asm push esi;
    //Get old TEXTUREFFACTOR
    _asm mov ecx, [ebx * 4 + 0x00971948];
    _asm lea esi, [ebx * 4 + 0x00971C18];
    _asm mov[esi], eax;
    _asm cmp eax, ecx;
    //if old == new skip
    _asm je FIXEDBLEND_SKIP;
    _asm push eax;
    _asm push ebx;
    _asm mov[esi], eax;
    _asm lea esi, [ebx * 4 + 0x00971948];
    _asm mov[esi], eax;
    _asm mov ecx, [0x00970E48];
    _asm mov ecx, [ecx];
    _asm push ecx;
    _asm mov ecx, [ecx];
    //Device->SetRenderStageState
    _asm call dword ptr[ecx + 0xC8];
FIXEDBLEND_SKIP:
    _asm pop esi;
    _asm pop ecx;
NO_FIXEDALPHA:
    _asm cmp esi, ecx;
    _asm add edi, ShaderTextureSkip;
    _asm jb ADD_TEXTURE;
    /*ADD_NULL_TEXTURE:
        //if numTextures was less than 4 we need to NULL the remaining textures
        _asm cmp esi, 4;
        _asm jae SKIP_NULL;
        _asm inc esi;
        _asm push 0;
        _asm push esi;
        _asm mov ecx, [0x00970E48];
        _asm mov ecx, [ecx];
        _asm push ecx;
        _asm mov ecx, [ecx];
        //Device->SetTexture
        _asm call dword ptr[ecx + 0x000000F4];
        _asm jmp ADD_NULL_TEXTURE;
    SKIP_NULL:*/

    //Restore all values, maybe not needed?
    //Atleast esi is needed for sure...
    _asm pop ebx;
    _asm pop ecx;
    _asm pop esi;
    _asm pop edi;
    _asm jmp[pReturn];

SKIP:

    //Reset extralayer textures to NULL
    //Maybe also need to reset blendmodes???
    _asm mov al, reset;
    _asm test al, al
    _asm je NO_RESET;

    _asm mov al, restore_matrix;
    _asm test al, al;
    _asm je NO_RESTORE_ENV
    _asm mov edx, [pDevice];
    _asm push D3DTTFF_DISABLE;
    _asm push D3DTSS_TEXTURETRANSFORMFLAGS;
    _asm push 0;
    _asm push edx;
    _asm mov eax, [edx];
    _asm call dword ptr[eax + 0x0000010C];

    _asm mov edx, [pDevice];
    _asm push D3DTSS_TCI_PASSTHRU;
    _asm push D3DTSS_TEXCOORDINDEX
    _asm push 0;
    _asm push edx;
    _asm mov eax, [edx];
    _asm call dword ptr[eax + 0x0000010C];
NO_RESTORE_ENV:

    _asm mov al, restoreMaterial;
    _asm test al, al
    _asm je NO_RESTORE_MAT;

    _asm mov eax, [pDevice];
    _asm mov edi, NULL_MAT;
    _asm push edi
    _asm push eax;
    _asm mov ecx, [eax];
    //Device->SetMaterial
    _asm mov[Gfx::oldMaterial], edi;
    _asm call dword ptr[ecx + 0xC4];
NO_RESTORE_MAT:

    /*//Restore BLENDOP
    _asm mov edi, D3DRS_BLENDOP;
    _asm lea esi, [edi * 4 + 0x00971C18];
    _asm mov edi, D3DBLENDOP_ADD;
    _asm mov [esi], edi;

    //Restore SRCBLEND
    _asm mov edi, D3DRS_SRCBLEND;
    _asm lea esi, [edi * 4 + 0x00971C18];
    _asm mov edi, D3DBLEND_SRCALPHA;
    _asm mov[esi], edi;

    //Restore DESTBLEND
    _asm mov edi, D3DRS_DESTBLEND;
    _asm lea esi, [edi * 4 + 0x00971C18];
    _asm mov edi, D3DBLEND_INVSRCALPHA;
    _asm mov[esi], edi;*/

    /*_asm mov edi, [pDevice];
    _asm push 0;
    _asm push 1;
    _asm push edi;
    _asm mov esi, [edi];
    //Device->SetTexture
    _asm call dword ptr[esi + 0x00000104];
    _asm push 0;
    _asm push 2;
    _asm push edi;
    _asm mov esi, [edi];
    //Device->SetTexture
    _asm call dword ptr[esi + 0x00000104];
    _asm push 0;
    _asm push 3;
    _asm push edi;
    _asm mov esi, [edi];
    //Device->SetTexture
    _asm call dword ptr[esi + 0x00000104];*/
    _asm xor eax, eax;
    _asm mov restoreMaterial, al;
    _asm mov reset, al;
    _asm mov restore_matrix, al;
    _asm mov bl, restore;
    _asm test bl, bl
    _asm je NO_RESET;
    _asm mov restore, al;
    _asm mov eax, [pDevice];
    _asm push 0x0D;
    _asm push 0x01;
    _asm push 0x00;
    _asm push eax;
    _asm mov ecx, [eax];
    _asm call dword ptr[ecx + 0x0000010C];
    /*_asm mov eax, [pDevice];
    _asm push 0x0D;
    _asm push 0x01;
    _asm push 0x02;
    _asm push eax;
    _asm mov ecx, [eax];
    _asm call dword ptr[ecx + 0x0000010C];
    _asm mov eax, [pDevice];
    _asm push 0x0D;
    _asm push 0x01;
    _asm push 0x03;
    _asm push eax;
    _asm mov ecx, [eax];
    _asm call dword ptr[ecx + 0x0000010C];*/
NO_RESET:
    //Restore esi value from the stack
    _asm pop esi
    _asm pop edi
    //Jump to original function
    _asm jmp[pReturn];
}