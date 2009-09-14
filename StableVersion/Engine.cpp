// Engine.cpp : Defines the entry point for the application.#include "Engine.h"
#include "Engine.h"
#include "Debugging.h"
#include "Graphics.h"
#include "Params.h"


// I always find this easier than finding the right option in VS' mess.
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
using namespace std;



// define the screen resolution and keyboard macros
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

//variables que hay que pasar a archivo de configuracion
bool useDebugging = true;
//mover!!!
LPDIRECTINPUT8 din;    // El apuntador a nuestra interface de DirectInput
LPDIRECTINPUTDEVICE8 dinkeyboard;    // El apuntador a keyboard device
BYTE keystate[256];    // El arreglo de 256 bytes donde almacenaremos la informacion sobre el cambio de estado de cada tecla
void initDInput(HINSTANCE hInstance, HWND hWnd);    // Inicializamos DirectInput
void detect_input(void);    // Esto captura el estatus correcto del input de teclado
void cleanDInput(void);    // Cierra directInput y libera la memoria utilizada para administrar los cambios al teclado.
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	if(useDebugging)
	{
		useConsole();
	}
	DWORD ThreadIdGraphics;
	HANDLE ThreadHandleGraphics;
	//DWORD ThreadIdNetwork;
	//HANDLE ThreadHandleNetwork;

	Params params;
	
	// set up and initialize Direct3D
	CEngine engine(hInstance, SCREEN_WIDTH, SCREEN_HEIGHT);
	params.engine = &engine;
	
	//mover dinput
	initDInput(engine.hInstance, engine.hWnd);    // Inicializado de DirectInput

//*---------------
	params.scene->AddChild(new CObject(++params.ID,2.0,0.0,0.0,0.0,0.0,0.0,1.0,1.0,1.0,L"tiger.x", params.scene,params.engine));
	CObject* o = params.scene->find(params.ID);
	o->AddChild(new CObject(++params.ID,0.0,3.0,0.0,0.0,0.0,0.0,1.0,1.0,1.0,L"tiger.x", o,params.engine));
	 o = params.scene->find(params.ID);
	o->AddChild(new CObject(++params.ID,0.0,3.0,0.0,0.0,0.0,0.0,0.1,0.1,0.1,L"airplane 2.x", o,params.engine));
	

	//Start Graphics and Networking threads
	ThreadHandleGraphics = CreateThread(NULL,0,graphics,&params,0,&ThreadIdGraphics);
	//ThreadHandleNetwork = CreateThread(NULL,0,network,NULL,0,&ThreadIdNetwork);
	








int index = 0; // index+=0.03f;
	while(engine.ProcessMessages())
	{
		detect_input();    // actualizamos los datos de entrada del teclado antes de volver a hacer el rendereo del cuadro
		//mover todo
		
		if(keystate[DIK_D] & 0x80)
		{	params.scene->move(1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
			index += 1;
		}
		if(keystate[DIK_A] & 0x80)
		{	params.scene->move(-1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
			index += 1;
		}
		if(keystate[DIK_W] & 0x80)
		{	params.scene->move(0.0,0.0,-1.0,0.0,0.0,0.0,0.0,0.0,0.0);
			index += 1;
		}
		if(keystate[DIK_S] & 0x80)
		{	params.scene->move(0.0,0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0);
			index += 1;
		}
		if(keystate[DIK_SPACE] & 0x80)
		{	params.scene->move(0.0,-1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
			index += 1;
		}
		if(keystate[DIK_LCONTROL] & 0x80)
		{	params.scene->move(0.0,1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
			index += 1;
		}
		



		if(useDebugging)
		{
			printf("Esto lo escribio el thread principal (winMain)%i\n", index);
			//printf("Esto lo escribio el thread principal (winMain)\n");
			Sleep(100);
		}
	}
	params.notQuit = 0;
	cleanDInput();    // Liberamos DirectInput
	return 0;
}



//mover!!!!!
// Funcion de Inicializacion de DirectInput
void initDInput(HINSTANCE hInstance, HWND hWnd)
{
    // Creamos la interface de DirectInput
    DirectInput8Create(hInstance,    // El handle de la aplicacion
                       DIRECTINPUT_VERSION,    // La version compatible
                       IID_IDirectInput8,    // La version de interface de directinput
                       (void**)&din,    // El apuntador a la interface
                       NULL);    // COM, que en este caso pasamos NULL

    // Creacion del device para el teclado
    din->CreateDevice(GUID_SysKeyboard,    // el ID default de teclado que utilizamos
                      &dinkeyboard,    // el apuntador a la interface del device
                      NULL);    // COM, que tampoco usamos aqui, asi que pasamos NULL

    // pasamos el formato de los datos a formato de teclado
    dinkeyboard->SetDataFormat(&c_dfDIKeyboard);

    // declaramos el control que utilizaremos como control de teclado
    dinkeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
}


// Esta es la funcion que nos trae los datos de entrada mas actuales
void detect_input(void)
{
    // solicitamos el acceso al teclado
    dinkeyboard->Acquire();

    // aqui sacamos los datos de entrada
    dinkeyboard->GetDeviceState(256, (LPVOID)keystate);
}


// Esta es la funcion que cierra DirectInput
void cleanDInput(void)
{
    dinkeyboard->Unacquire();    // rendimos el acquire del teclado
    din->Release();    // cerramos directInput antes de salir
}