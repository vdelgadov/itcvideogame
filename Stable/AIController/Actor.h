#ifndef ACTOR_H
#define ACTOR_H
#include "AIController.h"
#include "../SteeringBehaviors/Vehicle.h"
#include "../SteeringBehaviors/Vector2D.h"
#include "../Main/CObjectMesh.cpp"




class Actor : public CObjectMesh{

public:
	Actor(int ID,float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRotZ,float fScale, LPCTSTR fileName,CEngine* engine):CObjectMesh(ID,fPosX, fPosY, fPosZ, fRotX, fRotY,  fRotZ, fScale, fileName, engine){
		
		this->m_pVeh = new Vehicle<Vector2D>();//(0.01, Vector2D(fPosX, fPosZ));
	}
/*	Actor(AIController* cont){
		this->m_pVeh = new Vehicle<Vector2D>();
		this->m_pController = cont;
	}*/
	Vehicle<Vector2D>* getVehicle(){
		return m_pVeh;
	}

	AIController* getController(){
		return this->m_pController;
	}

	void setController(AIController* cont){
		this->m_pController = cont;
	}

	void update(double time){
		//cout << "updating" << endl;
	
		this->m_pVeh->update(time);
		D3DXMATRIX translationTemp;
		D3DXMatrixTranslation(&translationTemp,this->m_pVeh->getPos().x, 0,this->m_pVeh->getPos().y);
		this->translation *= translationTemp;
	}


	void render(){
		//cout << "render II " << endl;
		this->getController()->getFSM()->update();
		CObjectMesh::render();
	}
private:
	Vehicle<Vector2D>* m_pVeh;
	AIController* m_pController;
};
#endif