//Every windows application needs to include this
#include "windows.h"
//Every Direct3D application this
#include "d3d9.h"
#include "d3dx9.h"

bool g_bContinue = true;
LPD3DXEFFECT g_lpEffect = NULL;
LPDIRECT3DVERTEXBUFFER9 g_lpVertexBuffer = NULL;

//The combined transformation will end up in this matrix:
D3DXMATRIX g_ShaderMatrix;
//Camera Position
LPD3DXMESH meshTeapot;    // define the mesh pointer
D3DXVECTOR4 g_v3Position4;
D3DXVECTOR3 g_v3Position = D3DXVECTOR3(0.0f,0.0f,-5.0f);
D3DXVECTOR4 g_vLightDirection = D3DXVECTOR4(0.0f,0.0f,1.0f,1.0f);
//D3DXVECTOR4 g_vDIC = D3DXVECTOR4(0.4f,0.4f,0.4f,1.0f);
//D3DXVECTOR4 g_vSpecIC = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);

//Camera LookAt point
D3DXVECTOR3 g_v3LookAt = D3DXVECTOR3(0.0f,0.0f,0.0f);
//TEXTURE
LPDIRECT3DTEXTURE9 g_lpTexture = NULL;
LPDIRECT3DTEXTURE9 g_lpTexture2 = NULL;

//Definition of the first Vertex Format 
//including position and diffuse color
#define D3DFVF_COLOREDVERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(3))

struct COLORED_VERTEX
{
	float x, y, z;	//Position
	float i,j,k; //normal vector
	DWORD color;	//Color	
	float tu,tv;
	float ti,tj,tk; //tangent
};

//Besides the main function, there must be a message processing function
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	float fSpeed = 0.2f;
	switch( msg )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			g_bContinue = false;
			return 0;
		//handle keypress
		case WM_CHAR:
			switch(wParam)
			{
			case 'i':
				g_v3Position4.z += fSpeed;
				g_v3Position.z += fSpeed;
				break;
			case 'k':
				g_v3Position.z -= fSpeed;
				g_v3Position4.z-= fSpeed;
				break;
			}
			break;
		case WM_KEYDOWN:
			switch(wParam)
			{
			//change lookAt with arrow keys
			case VK_UP:
				g_v3LookAt.y += fSpeed;
				break;
			case VK_DOWN:
				g_v3LookAt.y -= fSpeed;
				break;
			case VK_LEFT:
				g_v3LookAt.x += fSpeed;
				break;
			case VK_RIGHT:
				g_v3LookAt.x -= fSpeed;
				break;
			case 0X069:
				g_v3Position4.z += fSpeed;
				g_v3Position.z += fSpeed;
				break;
			case 'j':
				g_v3Position.z -= fSpeed;
				g_v3Position4.z-= fSpeed;
				break;
			}
			break;
	}
	return DefWindowProc( hWnd, msg, wParam, lParam );
}

//The entry point of a windows application is the WinMain function
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT )
{
	
	//Create a window class.
	WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, 
					GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
					"Direct3D Window", NULL };
	//Register the window class.
	RegisterClassEx( &wc );
	//Create the application's window.
	HWND hWnd = CreateWindow( "Direct3D Window", "DirectX Wiki - D3D9 Tutorial 3", 
							WS_OVERLAPPEDWINDOW, 100, 100, 400, 400,
							GetDesktopWindow(), NULL, wc.hInstance, NULL );

	ShowWindow(hWnd,SW_SHOW);
	//Create the Direct3D Object
	LPDIRECT3D9 pD3D = NULL;
	if( NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION)))
	  return E_FAIL;
	//Setup the device presentation parameters
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	//The final step is to use the IDirect3D9::CreateDevice method to create the Direct3D device, as illustrated in the
	//following code example.
	LPDIRECT3DDEVICE9 pd3dDevice = NULL;
	if( FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp, &pd3dDevice ) ) )
	{
		MessageBox(hWnd, "No HAL HARDWARE_VERTEXPROCESSING! Sample will exit!", NULL, 0);
		pD3D->Release();
		pD3D = NULL;
		return E_FAIL;
	}
		
	//setting the  buffersize to 4 vertices * structure size
	UINT uiBufferSize = 4*sizeof(COLORED_VERTEX);
	//Creating Texture

	//creating the Vertexbuffer
	if( FAILED( pd3dDevice->CreateVertexBuffer( uiBufferSize,
			D3DUSAGE_WRITEONLY, D3DFVF_COLOREDVERTEX, D3DPOOL_DEFAULT, &g_lpVertexBuffer, NULL ) ) )
		return E_FAIL;
	
	COLORED_VERTEX* pVertices;
	//lock for writing.
	if( FAILED( g_lpVertexBuffer->Lock( 0, uiBufferSize, (void**)&pVertices, 0 ) ) )
		return E_FAIL;

	//Write now.
	//A simple Quadrangle
	pVertices[0].x =  -1.0f; //left
	pVertices[0].y =  -1.0f; //bottom
	pVertices[0].z =   0.0f;
	pVertices[0].color =  0xff0000ff; //red
	pVertices[0].i=0.0f;
	pVertices[0].j=0.0f;
	pVertices[0].k=1.0f;
	pVertices[0].tu=0.0f;
	pVertices[0].tv=1.0f;

	pVertices[0].ti=0.5f;
	pVertices[0].tj=0.5f;
	pVertices[0].tk=0.0f;


	pVertices[1].x =  -1.0f; //left
	pVertices[1].y =   1.0f; //top
	pVertices[1].z =   0.0f;
	pVertices[1].color =  0xff0000ff; //blue
	pVertices[1].i=0.0f;
	pVertices[1].j=0.0f;
	pVertices[1].k=1.0f;
	pVertices[1].tu=0.0f;
	pVertices[1].tv=0.0f;

	pVertices[1].ti=0.5f;
	pVertices[1].tj=-0.5f;
	pVertices[1].tk=0.0f;

	pVertices[2].x =   1.0f; //right
	pVertices[2].y =  -1.0f; //bottom
	pVertices[2].z =   0.0f;
	pVertices[2].color =  0xff0000ff; //green
	pVertices[2].i=0.0f;
	pVertices[2].j=0.0f;
	pVertices[2].k=1.0f;
	pVertices[2].tu=1.0f;
	pVertices[2].tv=1.0f;

	pVertices[2].ti=-0.5f;
	pVertices[2].tj=0.5f;
	pVertices[2].tk=0.0f;


	pVertices[3].x =  1.0f; //right
	pVertices[3].y =  1.0f; //top 
	pVertices[3].z =  0.0f;
	pVertices[3].color =  0xff0000ff; //white
	pVertices[3].i=0.0f;
	pVertices[3].j=0.0f;
	pVertices[3].k=1.0f;
	pVertices[3].tu=1.0f;
	pVertices[3].tv=0.0f;
	
	pVertices[3].ti=-0.5f;
	pVertices[3].tj=-0.5f;
	pVertices[3].tk=0;

	//unlock buffer.
	g_lpVertexBuffer->Unlock();

	//setting the vertex formats
	pd3dDevice->SetFVF( D3DFVF_COLOREDVERTEX );
	//handover the buffer to the device
	pd3dDevice->SetStreamSource( 0, g_lpVertexBuffer, 0, sizeof(COLORED_VERTEX) );
	if(FAILED(D3DXCreateEffectFromFile( pd3dDevice, "Effect.fx", NULL, 
		NULL, D3DXSHADER_ENABLE_BACKWARDS_COMPATIBILITY, NULL, &g_lpEffect, NULL )))
		return E_FAIL;
	
	D3DXCreateSphere(pd3dDevice,1.0f,40,40, &meshTeapot, NULL);
	LPD3DXMESH fvfMesh;
	
	//you need to clone the fvf before you do anything else

/*
	meshTeapot->CloneMeshFVF(D3DXMESH_VB_MANAGED , D3DFVF_COLOREDVERTEX, pd3dDevice, &fvfMesh);


	D3DXComputeTangentFrameEx( fvfMesh, D3DDECLUSAGE_TEXCOORD, 0, D3DDECLUSAGE_TANGENT, 0,
                               D3DX_DEFAULT, 0, D3DDECLUSAGE_NORMAL, 0,
                               D3DXTANGENT_CALCULATE_NORMALS,
			       NULL, -1, 0, -1, &newMesh, NULL );
	
	fvfMesh->Release();
*/	
	meshTeapot->CloneMeshFVF(D3DXMESH_VB_MANAGED , D3DFVF_COLOREDVERTEX, pd3dDevice, &fvfMesh);
	D3DXComputeTangentFrameEx( fvfMesh,
                           D3DX_DEFAULT,
                           0,
                           D3DX_DEFAULT,
                           0,
                           D3DX_DEFAULT,
                           0,
                           D3DDECLUSAGE_NORMAL,
                           0,
                           D3DXTANGENT_GENERATE_IN_PLACE | D3DXTANGENT_CALCULATE_NORMALS,
                           NULL,
                           -1.01f,
                           -0.01f,
                           -1.01f,
                           NULL,
                           NULL);
	




	if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, "brickwork-texture.jpg", &g_lpTexture ) ) )
		return E_FAIL;
	if( FAILED( D3DXCreateTextureFromFile( pd3dDevice, "bricks.jpg", &g_lpTexture2 ) ) )
		return E_FAIL;
	//the matrices:
	//the first transforms from world to view/camera space, 
	//the second from camera to screen space, 
	//the third transforms from object to world space
	D3DXMATRIX ViewMatrix, PerspectiveMatrix, WorldMatrix;
	//we intialize them with identity
	D3DXMatrixIdentity(&g_ShaderMatrix);
	D3DXMatrixIdentity(&WorldMatrix);
	D3DXMatrixIdentity(&ViewMatrix);
	D3DXMatrixIdentity(&PerspectiveMatrix);
	//calculating a perspective projection matrix
	//parameters besides the output matrix are:	
	//the fovY, the aspect ration, the near and far z values(for clipping)
	D3DXMatrixPerspectiveFovLH(&PerspectiveMatrix, D3DX_PI/4.0f, 1.0f, 0.01f, 100.0f);
	//Pass the Texture to the .fx file
	g_lpEffect->SetTexture("ColorMap", g_lpTexture);
	g_lpEffect->SetTexture("BumpMap", g_lpTexture2);
	

	
	MSG msg; 
	while( g_bContinue )
	{
		//Do a little position animation here for the objects world matrix
		D3DXMatrixTranslation(&WorldMatrix,sinf(GetTickCount()/1000.0f),0,0); 
		//Calculate a view matrix with position and look at vector
		D3DXMatrixLookAtLH(&ViewMatrix, &g_v3Position, &g_v3LookAt, &D3DXVECTOR3(0.0f,1.0f,0.0f));
		//Concatenating the matrices by multiplication.
		g_ShaderMatrix = ViewMatrix * PerspectiveMatrix * WorldMatrix;
		
		
		g_v3Position4.x=g_v3LookAt.x-g_v3Position.x;
		g_v3Position4.y=g_v3LookAt.y-g_v3Position.y;
		g_v3Position4.z=g_v3LookAt.z-g_v3Position.z;


		//handover the matrix to the effect.
		g_lpEffect->SetMatrix( "matWorldViewProj", &g_ShaderMatrix);
		g_lpEffect->SetMatrix( "matWorld", &WorldMatrix );
		g_lpEffect->SetVector("vecLightDir",&g_vLightDirection);
		g_lpEffect->SetVector("vecEye",&g_v3Position4);//posicion del ojo
		//g_lpEffect->SetVector("vDIC",&g_vDIC);//color del diffuse lighting
		//g_lpEffect->SetVector("VSpecIC",&g_vSpecIC);


		//Clear render region with blue
		pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );

		//before rendering something, you have to call this
		pd3dDevice->BeginScene();

		//rendering of scene objects happens here
		UINT uiPasses = 0;
		g_lpEffect->Begin(&uiPasses, 0);//0 = flags DWORD
		for (UINT uiPass = 0; uiPass < uiPasses; uiPass++)
		{
			g_lpEffect->BeginPass(uiPass);
			//meshTeapot->DrawSubset(0);
			//fvfMesh->DrawSubset(0);
			pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			g_lpEffect->EndPass();
		}
		g_lpEffect->End();

		//after the scene call
		pd3dDevice->EndScene();
		//update Screen
		pd3dDevice->Present(NULL, NULL, NULL, NULL);
		// A window has to handle its messages.
		TranslateMessage( &msg );
		DispatchMessage( &msg );
		PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
	}

	//Do not forget to clean up here
	meshTeapot->Release();
	g_lpTexture->Release();
	g_lpTexture2->Release();
	g_lpTexture=NULL;
	g_lpEffect->Release();
	g_lpEffect = NULL;
	g_lpVertexBuffer->Release();
	g_lpVertexBuffer = NULL;
	pd3dDevice->Release();
	pd3dDevice = NULL;
	pD3D->Release();
	pD3D = NULL; 

	
	return 0;
}