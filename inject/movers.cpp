#include "pch.h"
#include "movers.h"

typedef void(__cdecl* _MovePartner)(Vector3* source, Vector3* destination, Vector3* actorLocation);
_MovePartner MovePartnerFunc;

void MovePartner(Vector3* destination)
{
	MovePartnerFunc = (_MovePartner)(modBase + 0x576510);

	struct Vector3 zero;
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	Vector3* zeroPointer = &zero;

	Vector3* actorLocation = GetPartnerLocation();
	
	//check ALL pointers to make sure no bad memory is accessed
	if ((int)actorLocation > (int)modBase && (int)destination > (int)modBase)
	{
		MovePartnerFunc(zeroPointer, destination, actorLocation);
	}
}

void RotatePartner(float newRot)
{
	float* actorRot = GetPartnerRotation();

	//check ALL pointers to make sure no bad memory is accessed
	if ((int)actorRot > (int)modBase)
	{
		*actorRot = newRot;
	}
}

void SetEnemyData(Enemy* baseEnemy)
{
    int* baseEnemyPointer = (int*)(modBase + 0x7FDB18);

    if (*baseEnemyPointer == 0x0 || baseEnemy == 0x0)
    {
        return;
    }
    else
    {
        Enemy* currentEnemy = baseEnemy;
        *(Vector3*)(*(baseEnemyPointer)+0x94) = currentEnemy->pos;
        *(float*)(*(baseEnemyPointer)+0xA4) = currentEnemy->rot;
        *(short*)(*(baseEnemyPointer)+0x324) = currentEnemy->health;

        int* nextEnemyPtr = (int*)(*(baseEnemyPointer)+0x8);
        currentEnemy = currentEnemy->nextEnemy;

        while (nextEnemyPtr != 0x0 && currentEnemy != 0x0)
        {
            *(Vector3*)(*(nextEnemyPtr)+0x94) = currentEnemy->pos;
            *(float*)(*(nextEnemyPtr)+0xA4) = currentEnemy->rot;
            *(short*)(*(nextEnemyPtr)+0x324) = currentEnemy->health;

            nextEnemyPtr = (int*)(*(nextEnemyPtr)+0x8);
            currentEnemy = currentEnemy->nextEnemy;
        }
    }

    return;
}