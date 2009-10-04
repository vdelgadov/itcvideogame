#include "Engine.h"
#include "CObject.h"
//#include "Physics.h"
using namespace std;




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
//		Physics* physics;
		

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


