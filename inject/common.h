#pragma once

struct Vector3
{
	float x;
	float y;
	float z;
} unaligned;

extern uintptr_t modBase;

void Initialize();
void HookCollisionDisableClient();
void HookCollisionDisableServer();
