#pragma once
#include "AIController.h"
#include "../SteeringBehaviors/Vehicle.h"
#include "../Main/Vector3D.h"
#include "../Main/CObjectMesh.cpp"



class Actor : public CObjectMesh{

public:
        Actor(int ID,float fPosX, float fPosY, float fPosZ, float fRotX, float fRotY, float fRotZ,float fScale, LPCTSTR fileName,CEngine* engine):CObjectMesh(ID, fPosX,  fPosY,  fPosZ,  fRotX,  fRotY,  fRotZ, fScale,  fileName, engine){
        
                /*this->m_pVeh = new Vehicle<Vector3D>();
                this->m_pVeh->setPos(Vector3D(fPosX, fPosY, fPosZ));
                this->m_pVeh->setMaxSpeed(1);*/
                
        }

   /*     Vehicle<Vector3D>* getVehicle(){
                return m_pVeh;
        }*/

        Controller* getController(){
                return this->m_pController;
        }

        void setController(AIController* cont){
                this->m_pController = cont;
        }

        void update(double time){
                //cout << "updating" << endl;
                this->m_pController->update(time);
				this->getVehicle()->update(time);
                this->render();
        }

        FSM<Actor>* getFSM(){
                return this->m_pController->getFSM();
        }
		int getViewRadius(){
			return this->m_iViewRadius;
		}
		void setViewRadius(int vr){
			this->m_iViewRadius = vr;
		}

	

private:
//	Vehicle<Vector3D>* m_pVeh;
	AIController* m_pController;
	unsigned int m_iViewRadius;
};