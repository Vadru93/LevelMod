#include "pch.h"


__declspec(naked) void SetVertexShader_naked()
{
	static DWORD pReturn = 0x00550DC0;

	//Store old esi value onto the stack
	_asm push esi

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
	_asm sub esi, 17;
	_asm mov al, [esi];

	//Check if custom shader byte is on
	_asm cmp al, 0x31;	
	_asm jne SKIP;

	//Here is the part that sets the custom shader
	//First sub 4 bytes to get shaderid
	//Then move shader id to the stack
	_asm sub esi, 4;
	_asm mov esi, [esi];
	_asm mov [esp + 8], esi;

SKIP:
	//Restore esi value from the stack
	_asm pop esi
	//Jump to original function
	_asm jmp[pReturn];
}