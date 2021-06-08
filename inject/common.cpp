#include "pch.h"

uintptr_t modBase;

void Initialize()
{
	modBase = (uintptr_t)GetModuleHandle(NULL);
	HookCollisionDisable();
}


bool Hook(void* toHook, void* ourFunct, int len)
{
	if (len < 5)
	{
		return false;
	}

	DWORD curProtection;
	VirtualProtect(toHook, len, PAGE_EXECUTE_READWRITE, &curProtection);

	memset(toHook, 0x90, len);

	DWORD relativeAddress = ((DWORD)ourFunct - (DWORD)toHook) - 5;

	*(BYTE*)toHook = 0xE9;
	*(DWORD*)((DWORD)toHook + 1) = relativeAddress;

	DWORD temp;
	VirtualProtect(toHook, len, curProtection, &temp);

	return true;
}

// collision hooking

DWORD collisionJmpBack;
DWORD collisionJmpBack2;
DWORD leonPointer;
DWORD collisionPointer;
void __declspec(naked) disableCollisionOnAllButLeon()
{
	__asm {
		push eax
		mov eax, [leonPointer]
		mov eax,[eax]
		cmp ebx,eax
		pop eax
		jne SKIP_COLLISION
		call collisionPointer

		SKIP_COLLISION:
		
		jmp[collisionJmpBack]
	}
}

void __declspec(naked) disableCollisionOnAllButLeon2()
{
	__asm {
		push eax
		mov eax, [leonPointer]
		mov eax, [eax]
		cmp ebx, eax
		pop eax
		jne SKIP_COLLISION2
		call collisionPointer

		SKIP_COLLISION2:
		
		jmp[collisionJmpBack2]
	}
}


// Skips collision detection on all but leon (should only be used on client so it can easily sync with server, server needs a specific one to only ignore ashley, if enemy's get too broken)
void HookCollisionDisable()
{
	int hookLength = 5;
	DWORD hookAddress = modBase + 0x195018;
	leonPointer = modBase + 0x857054;
	collisionPointer = modBase + 0x3959;
	collisionJmpBack = hookAddress + hookLength;

	Hook((void*)hookAddress, disableCollisionOnAllButLeon, hookLength);

	DWORD hookAddress2 = modBase + 0x195044;
	collisionJmpBack2 = hookAddress2 + hookLength;

	Hook((void*)hookAddress2, disableCollisionOnAllButLeon2, hookLength);
}