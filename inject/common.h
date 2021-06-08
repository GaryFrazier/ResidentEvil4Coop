#pragma once

struct Vector3
{
	float x;
	float y;
	float z;
} unaligned;

extern uintptr_t modBase;

void InitializeInjection();
void HookCollisionDisableClient();
void HookCollisionDisableServer();
