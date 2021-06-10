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
