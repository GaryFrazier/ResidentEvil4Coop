#include "pch.h"

uintptr_t modBase;

void Initialize()
{
	modBase = (uintptr_t)GetModuleHandle(NULL);
}
