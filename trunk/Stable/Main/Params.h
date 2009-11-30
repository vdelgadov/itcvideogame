#ifndef PARAMS_H
#define PARAMS_H
#include "CObject.h"
#include "Engine.h"

#include "Physics.h"

class Params
{
	
	public:
		//current ID
		int ID;
		//All the objects (tree)
		CObject* scene;
		//keep running graphics
		int notQuit;
		//We need the engine to run graphics
		CEngine* engine;
		CObject* myObject;
		//Physics* physics;

	Params()
	{
		ID = 0;
		scene = new CObject();
//		physics = new Physics();
		notQuit = 1;
		engine = NULL;
		scene->pParent = NULL;
		
		
	}
	
};
#endif PARAMS_H_