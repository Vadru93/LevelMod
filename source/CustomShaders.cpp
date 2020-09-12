#include "pch.h"
#include "CustomShaders.h"
#include "SuperSectors.h"
#include "d3d9.h"

#define ReadDWORD() *(DWORD*)pFile; pFile+=4

extern LPDIRECT3DDEVICE9 pDevice;


/*ShaderObject* shaders = NULL;
DWORD numShaders = 0;*/

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
}

D3DMATERIAL9* NULL_MAT;
D3DMATERIAL9 NULLMAT;

void Gfx::LoadCustomShaders(char* path)
{
	NULL_MAT = &NULLMAT;
	ZeroMemory(NULL_MAT, sizeof(D3DMATERIAL9));
	loadingShaders = true;
	_printf("OpenShader: %s\n", path);
	FILE* f = fopen(path, "rb");
	bool first = true;
	if (f)
	{
		fseek(f, 0, SEEK_END);
		DWORD size = ftell(f);
		BYTE* pFile = new BYTE[size];
		BYTE* oldFile = pFile;
		fseek(f, 0, SEEK_SET);
		fread(pFile, 1, size, f);
		fclose(f);

		DWORD numMats = ReadDWORD();
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
							_printf("Waiting for matsplits to load %d(50)...\n", retries+1);
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
					__assume(numSplits < 5);
					for (DWORD j = 0; j < numSplits; j++)
					{
						DWORD matIndex = ReadDWORD();
						*(DWORD*)&shaders[numShaders].shaderId = ReadDWORD();
						DWORD idx = *(DWORD*)pFile;
						pFile += 4;
						if (idx > numMaterials)
							MessageBox(0, "Not good...", "TOO HIGH MATINDEX", 0);
						shaders[numShaders].material = &materials[idx];
						
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
							shaders[numShaders].fixedAlpha = ReadDWORD();
							shaders[numShaders].fixedAlpha = 0;
						}
						numShaders++;
					}

				}
				else
				{
					_printf("Couldn't find sector %s %X\n", FindChecksumName(checksum), pFile);
					for (DWORD j = 0; j < numSplits; j++)
					{
						pFile += 6 * 4;
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
			loadedShaders = true;
		}
	}
	loadingShaders = false;
}

bool reset = false;
bool restore = false;
bool restoreMaterial = false;

__declspec(naked) void __cdecl SetVertexShader_naked()
{
	static DWORD oldShader = 0x005CEDB4;
	static DWORD pReturn = 0x00550DC0;
	static DWORD ShaderTextureSkip = (sizeof(ShaderTexture) - 4);

	//Store old esi value onto the stack
	_asm push edi;
	_asm push esi;
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

	//Custom shader byte is is now 17 bytes from here
	_asm add esi, 0xF0;
	_asm mov edi, [esi];

	//Check if custom shader byte is on
	_asm test edi, edi
	_asm je SKIP;

	//Tell the engine we wanna reset textures next call
	reset = true;

	//Store ecx value, probably not needed?
	_asm push ecx;

	//Get old pixelshader
	_asm mov esi, [0x005CEDB4];

	//Get shaderId
	_asm mov ecx, [edi];

	//Compare old shader to new shader
	//_asm cmp esi, ecx;
	//If same, then skip shader
	_asm jmp SKIP_SHADER;

	//Move the shader into old location
	_asm mov esi, 0x005CEDB4;
	_asm mov [esi], ecx

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
	//Store values on stack, maybe not needed?
	_asm push ebx;
	
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
	_asm mov [Gfx::oldMaterial], ebx;
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
	_asm test eax, eax
	_asm je NO_FIXEDALPHA;
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
	//Get old DESTBLEND
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
	//Device->SetRenderState
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