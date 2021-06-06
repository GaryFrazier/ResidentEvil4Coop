#pragma once
#include <windows.h>
#include "Memory.h"
#include "pch.h"

// return, calling convention, params
typedef void (__cdecl* _MoveCharacter)(Vector3* startPos, Vector3* offset, Vector3* characterLocation);
_MoveCharacter MoveCharacter;

DWORD WINAPI MainThread(LPVOID param)
{
    Initialize();

    while (!GetAsyncKeyState(VK_END))
    {
        if (GetAsyncKeyState(VK_NUMPAD2) & 1)
        {
            Vector3* currentPos = GetCurrentLocation();
            MoveCharacter = (_MoveCharacter)(modBase + 0x575E50);

            struct Vector3 offset;
            offset.x = 200;
            Vector3* offsetPointer = &offset;
            MoveCharacter(currentPos, offsetPointer, currentPos);
        }
    }
    FreeLibraryAndExitThread((HMODULE)param, 0);
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, MainThread, hModule, 0, 0);
        break;
    default:
        break;
    }
    return TRUE;
}