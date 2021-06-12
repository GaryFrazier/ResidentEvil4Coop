#pragma once

struct Vector3
{
	float x;
	float y;
	float z;
} unaligned;

struct Enemy
{
	Vector3 pos;
	float rot;
	short health;
	Enemy* nextEnemy; // linked list
} enemy;

extern uintptr_t modBase;

void InitializeInjection();
void HookCollisionDisableClient();
void HookCollisionDisableServer();
