#ifndef SIMPLE_ENGINE_H_
#define SIMPLE_ENGINE_H_
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <windows.h>
#include <windows.h>
#include <dinput.h>
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

//===============================================================
// Clean up
#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }


//Dinput


class CEngine {
public:
	D3DXMATRIX matProjection;
	D3DXMATRIX matView;
	HINSTANCE hInstance;
	HWND hWnd;
	LPDIRECT3D9 d3d;
	LPDIRECT3DDEVICE9 d3ddev;
	LPDIRECT3DVERTEXBUFFER9 v_buffer;
	int iVertexSize;
	int iVertexDataSize;
	int iWidth;
	int iHeight;
	int iFVFFlags;
	
	CEngine(HINSTANCE Instance, int width, int height) : iWidth(width), iHeight(height),
															v_buffer(NULL), d3d(NULL), d3ddev(NULL),
															iVertexSize(0), iFVFFlags(0), iVertexDataSize(0) {
		hInstance = Instance;
		initWindow();
		init3D();
	}

	~CEngine() {
		close3D();
	}

	void close3D() {
		if(v_buffer){v_buffer->Release();}
		d3ddev->Release();
		d3d->Release();
	}

	void initWindow() {
		WNDCLASSEX wc;
	
	    ZeroMemory(&wc, sizeof(WNDCLASSEX));

		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = hInstance;
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.lpszClassName = L"WindowClass";

		RegisterClassEx(&wc);

		hWnd = CreateWindowEx(NULL, L"WindowClass", L"D3D nine",
							  WS_OVERLAPPEDWINDOW, 0, 0, iWidth, iHeight,
							  0, 0, hInstance, 0);
		//hWnd = CreateWindow(L"D3DWndClassNAme",L"jojo",WS_OVERLAPPEDWINDOW,100,100,800,600,0,0, hInstance,0);

		ShowWindow(hWnd, SW_SHOWDEFAULT);

	}

	void init3D() {
		d3d = Direct3DCreate9(D3D_SDK_VERSION);
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory(&d3dpp,sizeof(d3dpp));
		// Step 2: Verify hardware support for specified formats in windowed and full screen modes.
	
		D3DDISPLAYMODE mode;
		D3DDEVTYPE devType=(D3DDEVTYPE)D3DDEVTYPE_HAL;
		d3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode);
		d3d->CheckDeviceType(D3DADAPTER_DEFAULT, devType, mode.Format, mode.Format, true);
		d3d->CheckDeviceType(D3DADAPTER_DEFAULT, devType, D3DFMT_X8R8G8B8, D3DFMT_X8R8G8B8, false);

		// Step 3: Check for requested vertex processing and pure device.

			D3DCAPS9 caps;
			d3d->GetDeviceCaps(D3DADAPTER_DEFAULT, devType, &caps);

			DWORD devBehaviorFlags = 0;
			if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
				devBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
			else
				devBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

			// If pure device and HW T&L supported
			if( caps.DevCaps & D3DDEVCAPS_PUREDEVICE &&
				devBehaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING)
					devBehaviorFlags |= D3DCREATE_PUREDEVICE;

			// Step 4: Fill out the D3DPRESENT_PARAMETERS structure.

			d3dpp.BackBufferWidth            = 0; 
			d3dpp.BackBufferHeight           = 0;
			d3dpp.BackBufferFormat           = D3DFMT_UNKNOWN;
			d3dpp.BackBufferCount            = 1;
			d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
			d3dpp.MultiSampleQuality         = 0;
			d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
			d3dpp.hDeviceWindow              = hWnd;
			d3dpp.Windowed                   = true;
			d3dpp.EnableAutoDepthStencil     = true; 
			d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
			d3dpp.Flags                      = 0;
			d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;


			// Step 5: Create the device.

			d3d->CreateDevice(
				D3DADAPTER_DEFAULT, // primary adapter
				D3DDEVTYPE_HAL,           // device type
				hWnd,          // window associated with device
				devBehaviorFlags,   // vertex processing
				&d3dpp,            // present parameters
				&d3ddev);      // return created device
	}

	void SetTrianglesVBuffer(const void *data, DWORD datasize, DWORD vertexsize, DWORD fvfflags) {
		if (v_buffer)
			v_buffer->Release();
		d3ddev->CreateVertexBuffer(datasize,
								   0,
								   fvfflags,
								   D3DPOOL_MANAGED,
								   &v_buffer,
								   NULL);
		void* pVoid;
		iVertexSize = vertexsize;
		iFVFFlags = fvfflags;
		iVertexDataSize = datasize;
		// load the vertices
		v_buffer->Lock(0, 0, &pVoid, 0);
		memcpy(pVoid, data, datasize);
		v_buffer->Unlock();
	}

	bool ProcessMessages() {
		MSG msg;
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if(msg.message == WM_QUIT)

            return false;
		return true;
	}

	void SetPerspective(float fovy, float n, float f) {
        D3DXMatrixPerspectiveFovLH(&matProjection, fovy,
                1.0f,
				n,f);

        d3ddev->SetTransform(D3DTS_PROJECTION, &matProjection);
	}

	void Clear() {
		d3ddev->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	}

	void Begin() {
		d3ddev->BeginScene();
	}

	void End() {
	    d3ddev->EndScene();
	    d3ddev->Present(NULL, NULL, NULL, NULL);
	}

	void Render() {
	    d3ddev->SetFVF(iFVFFlags);
		d3ddev->SetStreamSource(0, v_buffer, 0, iVertexSize);
		d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, iVertexDataSize/iVertexSize);
	}


protected:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
	    switch(message)
	    {
			case WM_DESTROY:
	            {
					PostQuitMessage(0);
					return 0;
				} break;
		}
	
	    return DefWindowProc (hWnd, message, wParam, lParam);
	}

};
#endif
