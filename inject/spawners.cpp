#include "pch.h"
#include "spawners.h"

typedef void(__cdecl* _InitializeLuis)(int* newLuisPointer);
_InitializeLuis InitializeLuis;

void InitLuis()
{
    int* luisPointer = GetPartnerObjectPointer();

    //InitializeLuis = (_InitializeLuis)(modBase + 0x4e8970);
    InitializeLuis = (_InitializeLuis)(modBase + 0x4ea820); //ashley for testing
    InitializeLuis((int*)*luisPointer);
    return;
}
