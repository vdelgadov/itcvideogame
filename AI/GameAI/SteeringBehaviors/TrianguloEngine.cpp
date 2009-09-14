// TrianguloEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TrianguloEngine.h"
#include "SimpleEngine.h"

// define the screen resolution and keyboard macros
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// I always find this easier than finding the right option in VS' mess.
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

struct CUSTOMVERTEX {
	float X, Y, Z;
	DWORD COLOR;
};

#define CUSTOMFVF (D3DFVF_XYZ | D3DFVF_DIFFUSE)


int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	srand((unsigned) time(0));
	// set up and initialize Direct3D
	CSimpleEngine engine(hInstance, SCREEN_WIDTH, SCREEN_HEIGHT);

    // create the vertices using the CUSTOMVERTEX struct
    static const CUSTOMVERTEX vertices[] = 
    {
        { 2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(0, 0, 255), },
        { 0.0f, 3.0f, 0.0f, D3DCOLOR_XRGB(0, 255, 0), },
        { -2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0), },
    };

	static const CUSTOMVERTEX vertices_part0[] = 
    {
        { 2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(224, 0, 0), },
        { 0.0f, 3.0f, 0.0f, D3DCOLOR_XRGB(255, 255, 255), },
        { -2.5f, -3.0f, 0.0f, D3DCOLOR_XRGB(224, 0, 0), },
    };

	

	engine.SetPerspective(45, 1, 100);

	// Setup the matrices
	// ------------------
	// This should probably be handled by a 'camera' object.
	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView,
                       &D3DXVECTOR3 (0.0f, 5.0f, 20.0f),    // eye
                       &D3DXVECTOR3 (0.0f, 0.0f, 0.0f),    // at
                       &D3DXVECTOR3 (0.0f, 1.0f, 0.0f));    // up
	engine.d3ddev->SetTransform(D3DTS_VIEW, &matView);


	const unsigned int n_particles = 3;
	Vehicle particles[n_particles];

	for (int i = 0; i < n_particles; i++) {
		particles[i].setCurrVel(Vector2D(float(rand())/RAND_MAX * 50 - 25, float(rand())/RAND_MAX * 50 - 25));
		float rad = float(i)/n_particles * M_PI * 2 - M_PI;
		float speed = float(rand())/RAND_MAX * 0.5+0.1;
		if (i == 0){
			speed = 0.5;
			particles[i].setPos(Vector2D(2.0, 0.0));
			
		}
		particles[i].setMaxSpeed(speed);
		particles[i].setCurrVel(Vector2D(sin(rad)*speed, cos(rad)*speed));
	}

	int step = 0;
	while (engine.ProcessMessages()) {
		engine.Clear();
		engine.Begin();

		D3DXMATRIX matRotate, matTranslate, matScale;
		engine.d3ddev->SetTransform(D3DTS_WORLD, &matRotate);
		for (int i = 0; i < n_particles; i++) {
			D3DXMatrixTranslation(&matTranslate, particles[i].getPos().x, particles[i].getPos().y, 0);
			Vector2D heading = particles[i].getHeading();
			float angle = -atan2(heading.x, heading.y);
			D3DXMatrixRotationYawPitchRoll(&matRotate, 0, 0, angle);
			D3DXMatrixScaling(&matScale, 0.125, 0.125, 0.125);
			engine.d3ddev->SetTransform(D3DTS_WORLD, &(matRotate*matScale*matTranslate));
			
			if (i == 0) {
				particles[i].setCurrVel(SteeringBehaviors::interpose(&particles[i], &particles[1], &particles[2]));
				
				
			}else{
				if(i == 1)
					particles[i].setCurrVel(SteeringBehaviors::wander(&particles[i], 0.5, 1.2*sin(double(step)/30.0)));
				else
					particles[i].setCurrVel(SteeringBehaviors::wander(&particles[i], 0.25, 1.2*cos(double(step)/50.0)));

			}
				
			/*else if (i < n_particles/3) {
				particles[i].setCurrVel(SteeringBehaviors::seek(particles[0].getPos(), &particles[i]));
				
			}
			else if (i < n_particles/3*2) {
				if (step/100 % 2)
					particles[i].setCurrVel(SteeringBehaviors::evade(&particles[i], &particles[0]));
				else
					particles[i].setCurrVel(SteeringBehaviors::wander(&particles[i], 0.5, 1.2*cos(double(step)/50.0)));
				
			}
			else {
					particles[i].setCurrVel(SteeringBehaviors::pursuit(&particles[i], &particles[0]));
				
			}*/
			particles[i].update(1/5.0);

			// Render one triangle.
			if (i == 0)
				engine.SetTrianglesVBuffer (vertices_part0, sizeof(vertices_part0), sizeof(CUSTOMVERTEX), CUSTOMFVF);
			else if (i == 1)
				engine.SetTrianglesVBuffer (vertices, sizeof(vertices), sizeof(CUSTOMVERTEX), CUSTOMFVF);
			engine.Render();
		}
		engine.End();
		Sleep(1000/24);
		++step;
	}
	return 0;
}
