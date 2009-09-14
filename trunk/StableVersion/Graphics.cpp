#include "Engine.h"
#include "Graphics.h"
#include "Params.h"



struct CUSTOMVERTEX {
	float X, Y, Z;
	DWORD COLOR;
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)

DWORD WINAPI graphics(LPVOID Param)
{
	Params* params = (Params*)Param;
	CEngine engine = *(params->engine);
	
	float index = 0.0f;
	
	engine.SetPerspective(D3DX_PI/4,1.0f,100.0f);

	while (params->notQuit) {
		engine.Clear();
		engine.Begin();
		
		D3DXMATRIX matView;
	    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3 (0.0f, 0.0f, -10.0f),    // eye
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // at
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // up
		engine.d3ddev->SetTransform(D3DTS_VIEW, &matView);

		for(list<CObject*>::iterator it = params->scene->lstChilds.begin(); it != params->scene->lstChilds.end(); ++it)
		{
			(*it)->render();
			//params->scene->lstChilds.front()->render();
		}
		engine.End();
	}
	return 0;
}



/*DWORD WINAPI graphics(LPVOID Param)
{
	Params* params = (Params*)Param;
	CEngine engine = *(params->engine);


    // create the vertices using the CUSTOMVERTEX struct
    static const CUSTOMVERTEX vertices[] = 
    {
        { 2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
        { 0.0f, 3.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
        { -2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
    };

	engine.SetTrianglesVBuffer (vertices, sizeof(vertices), sizeof(CUSTOMVERTEX), CUSTOMFVF);

	engine.SetPerspective(45, 1, 100);

	float index = 0.0f;
	while (params->notQuit) {
		engine.Clear();
		engine.Begin();
		// Setup the matrices
		// ------------------
		// This should probably be handled by a 'camera' object.
		D3DXMATRIX matView;
	    D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3 (5.0f, 5.0f, 10.0f),    // eye
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // at
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // up
		engine.d3ddev->SetTransform(D3DTS_VIEW, &matView);

		index+=0.05f;
		D3DXMATRIX matRotate, matTranslate, matScale;
		D3DXMatrixRotationYawPitchRoll(&matRotate, index*2, index*0.1f, index*0.05f);
	    engine.d3ddev->SetTransform(D3DTS_WORLD, &matRotate);
		// Render one triangle.
		engine.Render();

		D3DXMatrixTranslation(&matTranslate, 0, 0, sin(index)+4);
		D3DXMatrixScaling(&matScale, 0.5, 0.5, 0.5);
		matRotate = matScale * matRotate * matTranslate * matRotate; // (RtoL) Rotate, Translate, rotate then scale the coord system.
		engine.d3ddev->SetTransform(D3DTS_WORLD, &matRotate);
		// Render another triangle.
		engine.Render();

		engine.End();
	}
	return 0;
}*/