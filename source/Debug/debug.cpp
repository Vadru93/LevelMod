#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "debug.h"

void Tracer(LPCSTR format, ...)
{
    if (format)
    {
        va_list vl;
        char str[4096];

        va_start(vl, format);
        _vsnprintf(str, (sizeof(str) - 1), format, vl);
        str[(sizeof(str) - 1)] = 0;
        va_end(vl);

        // Output to debugger channel
        OutputDebugString(str);
        printf(str);
    }
}

#ifdef _DEBUG
namespace LevelModSettings
{
    extern bool bLogging;
}
#endif

extern FILE* logFile;
extern CRITICAL_SECTION critical;
void DebugPrint(const char* file, DWORD line, const char* date, const char* string, ...)
{
    EnterCriticalSection(&critical);
    static char debug_msg[MAX_PATH];
    va_list myargs;
    va_start(myargs, string);
    vsprintf(debug_msg, string, myargs);
    va_end(myargs);
    printf(debug_msg);

    if (logFile)
    {
        if (*debug_msg != '\n')
            fprintf(logFile, "File %s line %u compiled %s\n", file, line, date);
        fprintf(logFile, debug_msg);
        fflush(logFile);
    }
    LeaveCriticalSection(&critical);
}

char* AddressToMappedName(HANDLE hOwner, PVOID pAddress, char* szBuffer, int iSize)
{
    if (szBuffer && (iSize > 0))
    {
        ZeroMemory(szBuffer, iSize);

        char szFullPath[MAX_PATH];
        if (GetMappedFileName(hOwner, pAddress, szFullPath, (sizeof(szFullPath) - 1)))
        {
            // Remove the path, keeping just the base name
            // TODO: You might optionally want the full path
            szFullPath[sizeof(szFullPath) - 1] = 0;
            char szFileName[_MAX_FNAME + _MAX_EXT], szExtension[_MAX_EXT];
            _splitpath(szFullPath, NULL, NULL, szFileName, szExtension);
            _snprintf(szBuffer, (iSize - 1), "%s%s", szFileName, szExtension);
            szBuffer[(iSize - 1)] = 0;
            return(szBuffer);
        }

        // Try alternate way, since first failed    
        {
            HMODULE hModule = NULL;
            if (pAddress)
            {
                // Try it as a module handle first        
                GetModuleHandleEx((GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT | GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS), (LPCTSTR)pAddress, &hModule);
                if (!hModule) hModule = (HMODULE)pAddress;
            }

            if (GetModuleBaseName(hOwner, hModule, szBuffer, (iSize - 1)) > 0)
                return(szBuffer);
            else
                // Fix weird bug where GetModuleBaseName() puts a random char in [0] on failure
                szBuffer[0] = 0;
        }
    }

    return(NULL);
}

bool ReportException(LPCTSTR pszFunction, LPEXCEPTION_POINTERS pExceptionInfo)
{
    char szModule[MAX_PATH] = { "Unknown" };
    AddressToMappedName(GetCurrentProcess(), pExceptionInfo->ExceptionRecord->ExceptionAddress, szModule, sizeof(szModule));
    Tracer("DLL: ** Exception: %08X, @  %08X, in \"%s\", from Module: \"%s\", Base: %08X **\n", pExceptionInfo->ExceptionRecord->ExceptionCode, pExceptionInfo->ExceptionRecord->ExceptionAddress, pszFunction, szModule, GetModuleHandle(szModule));
    return(false);
}

struct Line
{
    D3DXVECTOR3 v1;
    D3DXVECTOR3 v2;
    int color;

    short numFrames;
};

#define MAX_NUM_LINES 500000

Line DebugLines[500000];

void AddDebugLine(Vertex v0,Vertex v1, int rgb0, int numDrawFrames)
{
#ifdef	_DEBUG
    static int idx = 0;

    DebugLines[idx].v1 = v0;
    DebugLines[idx].v2 = v1;

    DebugLines[idx].color = rgb0;

    DebugLines[idx].numFrames = numDrawFrames;

    if (++idx >= 500000)
    {
        idx = 0;
    }
#endif
}

void DrawDebugLines()
{
#ifdef _DEBUG
    struct LineVert
    {
        D3DVECTOR	pos;
        D3DCOLOR	col;
    }; 

    static LineVert	line_verts[500000 * 2];
    DWORD index = 0;

    for (int i = 0; i < 500000; ++i)
    {
        if (DebugLines[i].numFrames>0)
        {
            DebugLines[i].numFrames--;
        }

        line_verts[index].pos = DebugLines[i].v1;
        line_verts[index].col = DebugLines[i].color;
        index++;
        line_verts[index].pos = DebugLines[i].v2;
        line_verts[index].col = DebugLines[i].color;
        index++;
    }

    if (index > 0)
    {
        DWORD old_op, old_src, old_dest, old_fvf;
        Gfx::pDevice->GetRenderState(D3DRS_BLENDOP, &old_op);
        Gfx::pDevice->GetRenderState(D3DRS_SRCBLEND, &old_src);
        Gfx::pDevice->GetRenderState(D3DRS_DESTBLEND, &old_dest);
        Gfx::pDevice->GetFVF(&old_fvf);

        Gfx::pDevice->SetTexture(0, NULL);
        Gfx::pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
        Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
        Gfx::pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
        Gfx::pDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
        Gfx::pDevice->DrawPrimitiveUP(D3DPT_LINELIST, index / 2, line_verts, sizeof(LineVert));

        Gfx::pDevice->SetRenderState(D3DRS_BLENDOP, old_op);
        Gfx::pDevice->SetRenderState(D3DRS_SRCBLEND, old_src);
        Gfx::pDevice->SetRenderState(D3DRS_DESTBLEND, old_dest);
        Gfx::pDevice->SetFVF(old_fvf);
    }
#endif
}