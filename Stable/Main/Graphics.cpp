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
	CEngine* engine = (params->engine);
	
	float index = 0.0f;
	
	engine->SetPerspective(D3DX_PI/4,1.0f,100.0f);

	while (params->notQuit) {
		engine->Clear();
		engine->Begin();
		
	    D3DXMatrixLookAtLH(&(engine->matView),
                       &D3DXVECTOR3 (0.0f, 5.0f, -10.0f),    // eye
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // at
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // up
		engine->d3ddev->SetTransform(D3DTS_VIEW, &(engine->matView));

		for(list<CObject*>::iterator it = params->scene->lstChilds.begin(); it != params->scene->lstChilds.end(); ++it)
		{
			(*it)->update();
			//cout << "posX: " << (*it)->vehicle.getPos().x << "posY: " << (*it)->vehicle.getPos().y << "posZ: " << (*it)->vehicle.getPos().z << endl;
		}
		engine->End();
	}
	return 0;
}

