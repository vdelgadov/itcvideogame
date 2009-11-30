#ifndef PHYSICS_DEF
#define PHYSICS_DEF

class CObject;
class Params;

class Physics{
public:
/*	Params* params;
	Physics(Params* Param)
	{
		params = Param;
	}*/
	static bool checkBoundingSphereCollision(CObject* o);
	static bool checkBoundingSphereCollision(CObject* o, CObject* o2);
};

#endif