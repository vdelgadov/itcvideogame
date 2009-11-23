LPDIRECTINPUT8 din;    // El apuntador a nuestra interface de DirectInput
LPDIRECTINPUTDEVICE8 dinkeyboard;    // El apuntador a keyboard device
LPDIRECTINPUTDEVICE8 dinmouse;    // el apuntador al mouse device
BYTE keystate[256];    // El arreglo de 256 bytes donde almacenaremos la informacion sobre el cambio de estado de cada tecla
DIMOUSESTATE mousestate;    // el espacio de almacenamiento para la informacion del mouse


class DirectInput
{

	public:
	#define PI 3.14159265358979323846

	Params* params;
	LPDIRECTINPUT8 din;    // El apuntador a nuestra interface de DirectInput
	LPDIRECTINPUTDEVICE8 dinkeyboard;    // El apuntador a keyboard device
	LPDIRECTINPUTDEVICE8 dinmouse;    // el apuntador al mouse device
	BYTE keystate[256];    // El arreglo de 256 bytes donde almacenaremos la informacion sobre el cambio de estado de cada tecla
	DIMOUSESTATE mousestate;    // el espacio de almacenamiento para la informacion del mouse

	DirectInput(Params* Param)
	{
		params = Param;
		initDInput(params->engine->hInstance,params->engine->hWnd);
	}
	
	
	bool keyDown(char key)
	{
		return (keystate[key] & 0x80) != 0;
	}
	
	
	int Update()
	{
		detect_input();

		if(keyDown(DIK_ESCAPE))
		{	
			params->engine->End();
			return 0;
		}
		if(keyDown(DIK_A))
		{
			networkMove(params->myObject->ID,-1.0f,0.0f,0.0f,&params);
			//params->myObject->move(Vector3D(-1.0,0.0,0.0)* params->myObject->getVehicle()->getMaxSpeed());
		}
		if(keyDown(DIK_D))
		{	networkMove(params->myObject->ID,1.0f,0.0f,0.0f,&params);
			//params->myObject->move(Vector3D(1.0,0.0,0.0)* params->myObject->getVehicle()->getMaxSpeed());
		}
		if(keyDown(DIK_W))
		{	
			networkMove(params->myObject->ID,0.0f,0.0f,1.0f,&params);
			//params->myObject->move(Vector3D(0.0,0.0,1.0)* params->myObject->getVehicle()->getMaxSpeed());
		}
		if(keyDown(DIK_S))
		{	
			networkMove(params->myObject->ID,0.0f,0.0f,-1.0f,&params);
			//params->myObject->move(Vector3D(0.0,0.0,-1.0)* params->myObject->getVehicle()->getMaxSpeed());	
		}
		if(keyDown(DIK_SPACE))
		{	
			networkMove(params->myObject->ID,0.0f,-1.0f,0.0f,&params);
			//params->myObject->move(Vector3D(0.0,-1.0,0.0)* params->myObject->getVehicle()->getMaxSpeed());
			
		}
		if(keyDown(DIK_LCONTROL))
		{	
			networkMove(params->myObject->ID,0.0f,-1.0f,0.0f,&params);
			//params->myObject->move(Vector3D(0.0,-1.0,0.0)* params->myObject->getVehicle()->getMaxSpeed());			
		}
		//params->scene->move(mousestate.lX * 0.01f,0.0,0.0,0.0,0.0,0.0,0.0);
		//params->scene->move(0.0,mousestate.lY * -0.01f,0.0,0.0,0.0,0.0,0.0);
		//params->scene->move(0.0,0.0,mousestate.lZ * -0.01f,0.0,0.0,0.0,0.0);
		return 1;
	}


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

		// creamos el dispositivo mouse
		din->CreateDevice(GUID_SysMouse,
						  &dinmouse,
						  NULL);

		// especificamos el tipo de datos que nos va a pasar el device mouse
		dinmouse->SetDataFormat(&c_dfDIMouse);

		// declaramos el control que utilizaremos como control del mouse device
		dinmouse->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	}


	// Esta es la funcion que nos trae los datos de entrada mas actuales
	void detect_input(void)
	{
	   // solicitamos el acceso al teclado
		dinkeyboard->Acquire();

		// solicitamos el acceso al mouse
		dinmouse->Acquire();

		// aqui sacamos los datos de entrada
		dinkeyboard->GetDeviceState(256, (LPVOID)keystate);

		// aqui sacamos los datos de entrada del mouse
		dinmouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mousestate);
	}


	// Esta es la funcion que cierra DirectInput
	void cleanDInput(void)
	{
		dinkeyboard->Unacquire();    // rendimos el acquire del teclado
		dinmouse->Unacquire();    // rendimos el acquire / acceso del mouse
		din->Release();    // cerramos directInput antes de salir
	}
};