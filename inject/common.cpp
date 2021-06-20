#include "pch.h"

uintptr_t modBase;

void InitializeInjection()
{
	modBase = (uintptr_t)GetModuleHandle(NULL);

	SetAshleyCostume();
	HookPartnerSpawnLogic();

	if (isServer)
	{
		HookCollisionDisableServer();
	}
	else
	{
		HookCollisionDisableClient();
	}
	
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

DWORD partnerCollisionJumpBack;
DWORD partnerCollisionJumpBack2;
DWORD partnerPointer;
void __declspec(naked) disableCollisionOnPartner()
{
	__asm {
		push eax
		mov eax, [partnerPointer]
		mov eax, [eax]
		cmp ebx, eax
		pop eax
		je SKIP_COLLISION_PARTNER
		call collisionPointer

		SKIP_COLLISION_PARTNER:

		jmp[partnerCollisionJumpBack]
	}
}

void __declspec(naked) disableCollisionOnPartner2()
{
	__asm {
		push eax
		mov eax, [partnerPointer]
		mov eax, [eax]
		cmp ebx, eax
		pop eax
		je SKIP_COLLISION_PARTNER2
		call collisionPointer

		SKIP_COLLISION_PARTNER2:

		jmp[partnerCollisionJumpBack2]
	}
}


// Skips collision detection on all but leon (should only be used on client so it can easily sync with server)
void HookCollisionDisableClient()
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

// Skips collision detection on partner (for server only)
void HookCollisionDisableServer()
{
	int hookLength = 5;
	DWORD hookAddress = modBase + 0x195018;
	partnerPointer = modBase + 0x857060;
	collisionPointer = modBase + 0x3959;
	partnerCollisionJumpBack = hookAddress + hookLength;

	Hook((void*)hookAddress, disableCollisionOnPartner, hookLength);

	DWORD hookAddress2 = modBase + 0x195044;
	partnerCollisionJumpBack2 = hookAddress2 + hookLength;

	Hook((void*)hookAddress2, disableCollisionOnPartner2, hookLength);
}

DWORD partnerCheckJmpBack;
DWORD currentAreaPointer;
void __declspec(naked) hookPartnerSpawnLogicAsm()
{
	__asm {
		push eax
		push ebx
		mov eax, [currentAreaPointer]
		mov eax, [eax]
		mov ebx, 0x100
		cmp ebx, eax
		pop ebx
		pop eax
		je PARTNER_SPAWN_DEF
		cmp eax, 0

		jmp PARTNER_SPAWN_RET
		PARTNER_SPAWN_DEF :
		test dword ptr [eax + 0x00005028], 0x04000000 //BREAKING FOR SOME REASON

		PARTNER_SPAWN_RET:
		jmp[partnerCheckJmpBack]
	}
}


// Skips partner logic on all maps except ones specified)
void HookPartnerSpawnLogic()
{
	int hookLength = 10;
	DWORD hookAddress = modBase + 0x2c5201;

	currentAreaPointer = modBase + 0x85F70C;
	partnerCheckJmpBack = hookAddress + hookLength;

	Hook((void*)hookAddress, hookPartnerSpawnLogicAsm, hookLength);
}


void SetAshleyCostume() {
	byte* ashleyCostume = (byte*)(modBase + 0x85F72B);

	*ashleyCostume = 2;
}
