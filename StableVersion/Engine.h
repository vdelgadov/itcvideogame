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


//Dinput


class CEngine {
public:
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
							  NULL, NULL, hInstance, NULL);

		ShowWindow(hWnd, SW_SHOWDEFAULT);

	}

	void init3D() {
		d3d = Direct3DCreate9(D3D_SDK_VERSION);

		D3DPRESENT_PARAMETERS d3dpp;

		ZeroMemory(&d3dpp, sizeof(d3dpp));
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.hDeviceWindow = hWnd;
		d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
		d3dpp.BackBufferWidth = iWidth;
		d3dpp.BackBufferHeight = iHeight;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		d3d->CreateDevice(D3DADAPTER_DEFAULT,
                      D3DDEVTYPE_HAL,
                      hWnd,
                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                      &d3dpp,
                      &d3ddev);

	    d3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
		d3ddev->SetRenderState(D3DRS_ZENABLE, TRUE);
		d3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
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
        D3DXMATRIX matProjection;
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
