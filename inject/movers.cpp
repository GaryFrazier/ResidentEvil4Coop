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

    std::cout << "enemy " << baseEnemyPointer << " " << *baseEnemyPointer << " " << baseEnemy << "\n";

    if (*baseEnemyPointer == (int)0x0 || *baseEnemyPointer == (int)0x1 || *baseEnemyPointer == (int)0xCCCCCCCC || (int)baseEnemy == (int)0x1 || (int)baseEnemy == (int)0x0 || (int)baseEnemy == (int)0xCCCCCCCC)
    {
        std::cout << "fail\n";
        return;
    }
    else
    {
        Enemy* currentEnemy = baseEnemy;
        if (!(*baseEnemyPointer == 0x0 || baseEnemyPointer == 0x0 || *(baseEnemyPointer) == (int)*(GetPlayerPointer()) || *(baseEnemyPointer) == (int)*(GetPartnerObjectPointer())))
        {
            std::cout << "mov base enemy\n";

            *(Vector3*)(*(baseEnemyPointer)+0x94) = currentEnemy->pos;
            *(float*)(*(baseEnemyPointer)+0xA4) = currentEnemy->rot;
            *(short*)(*(baseEnemyPointer)+0x324) = currentEnemy->health;
        }

        std::cout << "next enemy\n";
        int* nextEnemyPtr = (int*)(*(baseEnemyPointer)+0x8);
        currentEnemy = currentEnemy->nextEnemy;

        while (nextEnemyPtr != 0x0 && (int)currentEnemy < 0xCCCCCCCC && currentEnemy != 0x0 && (int)currentEnemy < 0xCCCCCCCC)
        {
            if (!(*nextEnemyPtr == 0x0 || nextEnemyPtr == 0x0 || *(nextEnemyPtr) == (int)*(GetPlayerPointer()) || *(nextEnemyPtr) == (int)*(GetPartnerObjectPointer())))
            {
                std::cout << "mov enemy\n";
                *(Vector3*)(*(nextEnemyPtr)+0x94) = currentEnemy->pos;
                *(float*)(*(nextEnemyPtr)+0xA4) = currentEnemy->rot;
                *(short*)(*(nextEnemyPtr)+0x324) = currentEnemy->health;
            }

            std::cout << "next enemy 2\n";

            nextEnemyPtr = (int*)(*(nextEnemyPtr)+0x8);
            currentEnemy = currentEnemy->nextEnemy;
            
        }
    }

    return;
}