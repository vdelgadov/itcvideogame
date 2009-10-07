// Engine.cpp : Defines the entry point for the application.
#include "Networking.h"
#include "Engine.h"
#include "Debugging.h"
#include "Graphics.h"

#include "Params.h"
#include "DirectInput.h"
#include "CObjectMesh.cpp"
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
	//o->AddChild(new CObjectMesh(++params.ID,0.0,5.0,0.0,-45.0,0.0,0.0,0.5,L"tiger.x",params.engine));
	// o = params.scene->find(params.ID);
//	o->AddChild(new CObject(++params.ID,-5.0f,3.0f,0.0,0.0,0.0,0.0,0.1f,0.1f,0.1f,L"airplane 2.x", o,params.engine));
	
	
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



