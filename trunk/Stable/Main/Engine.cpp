// Engine.cpp : Defines the entry point for the application.
#include "Networking.h"
#include "Engine.h"
#include "Debugging.h"
#include "Graphics.h"

#include "Params.h"
#include "DirectInput.h"
//#include "CObjectMesh.cpp"
#include "../AIController/Actor.h"
#include "../AIController/AIController.cpp"
//#include "Physics.h"


// I always find this easier than finding the right option in VS' mess.
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
using namespace std;



// define the screen resolution and keyboard macros
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

//variables que hay que pasar a archivo de configuracion
bool useDebugging = true;



int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	
	if(useDebugging)
	{
		useConsole();
	}
	DWORD ThreadIdGraphics;
	HANDLE ThreadHandleGraphics;
	DWORD ThreadIdNetwork;
	HANDLE ThreadHandleNetwork;

	Params params;
	
	// set up and initialize Direct3D
	CEngine engine(hInstance, SCREEN_WIDTH, SCREEN_HEIGHT);
	params.engine = &engine;
	DirectInput* DI = new DirectInput(&params);
//	Physics* Physics = new Physics(&params);


//*---------------
	params.scene->AddChild(new CObjectMesh(++params.ID,0.0,0.0,0.0,0.0,0.0,0.0,1.0,L"BasicColumnScene.x", params.engine));
	//CObject* o = params.scene->find(params.ID);
	//printf("x:%f y:%f z:%f\n",o->vehicle.getPos().x,o->vehicle.getPos().y,o->vehicle.getPos().z);
	params.scene->AddChild(new CObjectMesh(++params.ID,0.0,1.0,0.0,0.0,0.0,0.0,0.5,L"tiger.x",params.engine));
	
	params.scene->AddChild(new CObjectMesh(++params.ID,3.0,1.0,0.0,0.0,0.0,0.0,0.5,L"tiger.x",params.engine));
	CObject* o = params.scene->find(params.ID);
	params.myObject = o;
	
	params.scene->AddChild(new Actor(++params.ID,-3.0,1.0,0.0,0.0,0.0,0.0,0.5,L"tiger.x",params.engine));
	AIController a_c(dynamic_cast<Actor*>(params.scene->find(params.ID)));
	Waypoint<Vector2D>* a = new Waypoint<Vector2D>("a", Vector2D());
	Waypoint<Vector2D>* b = new Waypoint<Vector2D>("b", Vector2D(0.0, 0.05));
	Waypoint<Vector2D>* c = new Waypoint<Vector2D>("c", Vector2D(0.05, 0.0));
	Waypoint<Vector2D>* d = new Waypoint<Vector2D>("d", Vector2D(0.10, 0.0));
	Waypoint<Vector2D>* e = new Waypoint<Vector2D>("e", Vector2D(0.075, -0.05));
	Waypoint<Vector2D>* f = new Waypoint<Vector2D>("f", Vector2D(0.075, 0.05));
	Waypoint<Vector2D>* g = new Waypoint<Vector2D>("g", Vector2D(0.10, -0.05));
	Waypoint<Vector2D>* h = new Waypoint<Vector2D>("h", Vector2D(0.10, -0.10));
	
	a_c.getActor()->getVehicle()->setMaxSpeed(0.0001);


	AIController::s_lMap.push_front(a);
	AIController::s_lMap.push_front(b);
	AIController::s_lMap.push_front(c);
	AIController::s_lMap.push_front(d);
	AIController::s_lMap.push_front(e);
	AIController::s_lMap.push_front(f);
	AIController::s_lMap.push_front(g);
	AIController::s_lMap.push_front(h);

	a->addNbor(b);
	a->addNbor(c);
	c->addNbor(d);
	c->addNbor(e);
	e->addNbor(g);
	g->addNbor(h);

	//b->addNbor(e);
	b->addNbor(b);

	a_c.planPath(b);

	//a_c.planPath(e);
	a_c.traversePath();

	
	//Start Graphics and Networking threads
	ThreadHandleGraphics = CreateThread(NULL,0,graphics,&params,0,&ThreadIdGraphics);
	ThreadHandleNetwork = CreateThread(NULL,0,networking,&params,0,&ThreadIdNetwork);
	








int index = 0; // index+=0.03f;
	while(engine.ProcessMessages())
	{
		if(!DI->Update())// actualizamos los datos de entrada del teclado antes de volver a hacer el rendereo del cuadro
		{
			break;
		}
		
		/*
					// set the world transform
		static float index = 0.0f;
		index += mousestate.lX * 0.003f;

		D3DXMATRIX matRotateY;
		D3DXMatrixRotationY(&matRotateY, index);
		d3ddev->SetTransform(D3DTS_WORLD, &(matRotateY));
		*/
		



		if(useDebugging)
		{
			//printf("Esto lo escribio el thread principal (winMain)%i\n", index);
			Sleep(100);
		}
	}
	params.notQuit = 0;
	Sleep(3000);
	engine.End();
	DI->cleanDInput();    // Liberamos DirectInput
	return 0;
}



