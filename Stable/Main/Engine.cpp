// Engine.cpp : Defines the entry point for the application.
#include "Physics.h"
#include "Networking.h"
#include "Engine.h"
#include "Debugging.h"
#include "Graphics.h"
//#include "Scripting.h"

#include "Params.h"
#include "DirectInput.h"
//#include "CObjectMesh.cpp"
#include "../AIController/Actor.h"
#include "../AIController/AIController.h"
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

//*---------------
	CObject* o;
	for(int i=0;i <MAXCLIENTS;i++)//max number of network clients defined in networking.h
	{
		params.scene->AddChild(new CObjectMesh(++params.ID,0.0,1.0,0.0,PI/2,0.0,0.0,0.5,L"tiger.x",params.engine));
		o = params.scene->find(params.ID);
		o->pScene = params.scene;
		o->getVehicle()->setMaxSpeed(0.05);
		o->isRendereable = false;
		o->boundingSphere = true;
	}
	//printf("x:%f y:%f z:%f\n",o->vehicle.getPos().x,o->vehicle.getPos().y,o->vehicle.getPos().z);
	//params.scene->AddChild(new CObjectMesh(++params.ID,-2.0,1.0,0.0,0.0,0.0,0.0,0.5,L"tiger.x",params.engine));

	/*params.scene->AddChild(new CObjectMesh(++params.ID,0.0,1.0,0.0,PI/2,0.0,0.0,0.5,L"tiger.x",params.engine));
	o = params.scene->find(params.ID);
	o->pScene = params.scene;
	o->boundingSphere = true;
	*/


	params.scene->AddChild(new CObjectMesh(++params.ID,-3.0,1.0,0.0,PI/2,0.0,0.0,0.5,L"tiger.x",params.engine));
	o = params.scene->find(params.ID);
	o->pScene = params.scene;
	o->boundingSphere = true;
	params.myObject = o;

	params.scene->AddChild(new CObjectMesh(++params.ID,4.0,1.0,0.0,-PI/2,0.0,0.0,0.5,L"tiger.x",params.engine));
	o = params.scene->find(params.ID);
	o->pScene = params.scene;
	o->boundingSphere = true;

	params.scene->AddChild(new CObjectMesh(++params.ID,0.0,0.0,0.0,0.0,0.0,0.0,1.0,L"BasicColumnScene.x", params.engine));
	
	o->pScene = params.scene;


	params.scene->AddChild(new Actor(++params.ID,-4.0,1.0,0.0,0.0,PI/2,0.0,0.2,L"bote2.x",params.engine));
	o = params.scene->find(params.ID);
	o->boundingSphere = true;
	o->pScene = params.scene;
	AIController a_c(dynamic_cast<Actor*>(params.scene->find(params.ID)));
	Waypoint<Vector3D>* a = new Waypoint<Vector3D>("a", Vector3D());
	Waypoint<Vector3D>* b = new Waypoint<Vector3D>("b", Vector3D(0.0f, 1.0f, 1.0f));
	Waypoint<Vector3D>* c = new Waypoint<Vector3D>("c", Vector3D(4.0f, 1.0f, 5.0f));
	Waypoint<Vector3D>* d = new Waypoint<Vector3D>("d", Vector3D(1.0f, 1.0f, 0.0f));
	Waypoint<Vector3D>* e = new Waypoint<Vector3D>("e", Vector3D(1.0f, 1.0f, 1.0f));
	Waypoint<Vector3D>* f = new Waypoint<Vector3D>("f", Vector3D(1.0f, 1.0f, 2.0f));
	Waypoint<Vector3D>* g = new Waypoint<Vector3D>("g", Vector3D(2.0f, 1.0f, 0.0f));
	Waypoint<Vector3D>* h = new Waypoint<Vector3D>("h", Vector3D(2.0f, 1.0f, 1.0f));
	Waypoint<Vector3D>* i = new Waypoint<Vector3D>("i", Vector3D(2.0f, 1.0f, 2.0f));

	a_c.getActor()->getVehicle()->setMaxSpeed(0.005);

	AIController::s_lMap.push_front(a);
	AIController::s_lMap.push_front(b);
	AIController::s_lMap.push_front(c);
	/*AIController::s_lMap.push_front(d);
	AIController::s_lMap.push_front(e);
	AIController::s_lMap.push_front(f);
	AIController::s_lMap.push_front(g);
	AIController::s_lMap.push_front(h);
	AIController::s_lMap.push_front(i);*/

	a->addNbor(b);
	b->addNbor(c);

	/*	c->addNbor(f);
	f->addNbor(e);
	e->addNbor(d);
	d->addNbor(g);
	g->addNbor(h);
	h->addNbor(i);*/

	a_c.planPath(c);
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
	networkQuit();
	params.notQuit = 0;
	Sleep(3000);
	engine.End();
	DI->cleanDInput();    // Liberamos DirectInput
	return 0;
}
