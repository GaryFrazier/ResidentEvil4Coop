#pragma once
#include "pch.h"

// object pointers
int* GetPlayerPointer();
int* GetPartnerObjectPointer();

Vector3* GetCurrentLocation();
Vector3* GetPartnerLocation();
float* GetCurrentRotation();
float* GetPartnerRotation();

short GetCurrentAreaId();

Enemy* GetEnemies();