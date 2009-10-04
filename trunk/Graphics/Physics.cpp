#include "Physics.h"
#include "CObject.h"

bool Physics::checkBoundingSphere(CObject* o, CObject* o2)
{
	if(!o2 || o2->ID == 0)
	{
		return true;
	}
	float relPosX = o->x - o2->x;
	float relPosY = o->y - o2->y;
	float relPosZ = o->z - o2->z;

	float dist = relPosX * relPosX + relPosY * relPosY + relPosZ * relPosZ;
	float minDist = o->Radius + o2->Radius;
	if(dist >= (minDist * minDist))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
