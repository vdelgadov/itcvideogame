// TrianguloEngine.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "TrianguloEngine.h"
#include "SimpleEngine.h"
#include "../AIController/AIController.h"
#include "../Waypoints/Waypoint.h"

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


	const unsigned int n_particles = 1;
	Vehicle<Vector2D> particles[n_particles];
	Actor actors[n_particles];

	for (int i = 0; i < n_particles; i++) {
		actors[i].getVehicle()->setCurrVel(Vector3D(float(rand())/RAND_MAX * 50 - 25, float(rand())/RAND_MAX * 50 - 25));
		actors[i].getVehicle()->setPos(Vector3D(float(rand())/RAND_MAX, float(rand()/RAND_MAX)));
		float rad = float(i)/n_particles * M_PI * 2 - M_PI;
		float speed = float(rand())/RAND_MAX * 0.5+0.1;
		if (i == 0){
			speed = 0.8;
			actors[i].getVehicle()->setPos(Vector3D(10.0, -12.5));
			
		}
		actors[i].getVehicle()->setMaxSpeed(speed);
		actors[i].getVehicle()->setCurrVel(Vector3D(0, 0));
	}


	Vector2D stf, norm_vel;

	AIController a_c = AIController(&(actors[0]));
	Waypoint<Vector3D>* a = new Waypoint<Vector3D>("a", Vector3D());
	Waypoint<Vector3D>* b = new Waypoint<Vector3D>("b", Vector3D(0.0, 5.0));
	Waypoint<Vector3D>* c = new Waypoint<Vector3D>("c", Vector3D(5.0, 0.0));
	Waypoint<Vector3D>* d = new Waypoint<Vector3D>("d", Vector3D(10.0, 0.0));
	Waypoint<Vector3D>* e = new Waypoint<Vector3D>("e", Vector3D(7.5, -5.0));
	Waypoint<Vector3D>* f = new Waypoint<Vector3D>("f", Vector3D(7.5, 5.0));
	Waypoint<Vector3D>* g = new Waypoint<Vector3D>("g", Vector3D(10.0, -5.0));
	Waypoint<Vector3D>* h = new Waypoint<Vector3D>("h", Vector3D(10.0, -10.0));
	AIController::s_lMap.push_front(a);
	AIController::s_lMap.push_front(b);
	AIController::s_lMap.push_front(c);
	AIController::s_lMap.push_front(d);
	AIController::s_lMap.push_front(e);
	AIController::s_lMap.push_front(f);
	AIController::s_lMap.push_front(g);
	AIController::s_lMap.push_front(h);

	a->addNbor(b);
	b->addNbor(a);
	a->addNbor(c);
	c->addNbor(a);
	c->addNbor(d);
	d->addNbor(c);
	c->addNbor(e);
	e->addNbor(c);
	e->addNbor(g);
	g->addNbor(e);
	g->addNbor(h);
	h->addNbor(g);
	h->addNbor(f);

	//b->addNbor(e);
	b->addNbor(f);

	a_c.planPath(a);

	a_c.getActor()->getVehicle()->setMaxSpeed(0.1);
	a_c.traversePath();


	int step = 0;
	while (engine.ProcessMessages()) {
		engine.Clear();
		engine.Begin();

		D3DXMATRIX matRotate, matTranslate, matScale;
		engine.d3ddev->SetTransform(D3DTS_WORLD, &matRotate);
		for (int i = 0; i < n_particles; i++) {
			D3DXMatrixTranslation(&matTranslate, actors[i].getVehicle()->getPos().x, actors[i].getVehicle()->getPos().y, 0);
			Vector3D heading = actors[i].getVehicle()->getHeading();
			float angle = -atan2(heading.x, heading.y);
			D3DXMatrixRotationYawPitchRoll(&matRotate, 0, 0, angle);
			D3DXMatrixScaling(&matScale, 0.125, 0.125, 0.125);
			engine.d3ddev->SetTransform(D3DTS_WORLD, &(matRotate*matScale*matTranslate));
		/*	
		if (i == 0) {
			
			//stf = //SteeringBehaviors<Vector2D>::seek(Vector2D(8.0, 8.0), actors[i].getVehicle());
			stf = SteeringBehaviors<Vector2D>::interpose(actors[i].getVehicle(), actors[1].getVehicle(), actors[2].getVehicle());
			
				
				
			}else{
				if(i == 1)
					stf = SteeringBehaviors<Vector2D>::wander(actors[i].getVehicle(), 0.5, 3*sin(double(step)/30.0));
				else if(i == 2)
					stf = SteeringBehaviors<Vector2D>::wander(actors[i].getVehicle(), 0.25, 3*cos(double(step)/50.0));

			}
				
			if (i < n_particles/3 && i > 2) {
				stf = SteeringBehaviors<Vector2D>::interpose(actors[i].getVehicle(), actors[0].getVehicle(), Vector2D(0,0));
				
			}
			else if (i > 2 && i < n_particles/3*2) {
				if (step/100 % 2)
					stf = SteeringBehaviors<Vector2D>::evade(actors[i].getVehicle(), actors[1].getVehicle());
				else
					stf = SteeringBehaviors<Vector2D>::wander(actors[i].getVehicle(), 0.5, 0.8*sin(double(step)/30.0));
				
			}
			else if(i > 2){
				stf = SteeringBehaviors<Vector2D>::pursuit(actors[i].getVehicle(), actors[(step/100)%2 + 1].getVehicle());
				
			}*/
		/*	norm_vel = actors[i].getVehicle()->getCurrVel();
			norm_vel += stf;
			norm_vel.normalize();
			norm_vel *= actors[i].getVehicle()->getMaxSpeed();
			actors[i].getVehicle()->setCurrVel(norm_vel);
			actors[i].getVehicle()->update(1/5.0);*/

			a_c.update(step/5.0);

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
