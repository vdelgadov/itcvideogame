#pragma once

#include "../StateMachine/states.h"
#include "../AIController/Actor.h"
#include "../SteeringBehaviors/Behaviors.h"
#include "../InfluenceMaps/InfluenceMap.h"

class Idle : public AState<Actor> {
	void enter(Actor* a){ cout << "Entering Idle" <<endl;}
	void execute(Actor* a){
		cout << "Idling" << endl;
		AIController* aic = (AIController*)(a->getController());
		Actor* Enemy = aic->getEnemy();

		int e_x, e_y, m_x, m_y;
		if(!Enemy){
			Vector3D w_v = SteeringBehaviors<Vector3D>::wander(a->getVehicle(), 10 , 10);
			w_v += a->getVehicle()->getCurrVel();
			w_v.normalize();
			cout << w_v.x << " " << w_v.y << " " << w_v.z << endl;
			a->getVehicle()->setCurrVel(w_v*a->getVehicle()->getMaxSpeed());
			return;
		}
		AIController::s_InfluenceMap->mapCoords(Enemy->getVehicle()->getPos(), &e_x, &e_y);
		AIController::s_InfluenceMap->mapCoords(a->getVehicle()->getPos(), &m_x, &m_y);
		if(abs(m_x - e_x) < a->getViewRadius() || abs(m_y - e_y) < a->getViewRadius()){
			a->getFSM()->changeState("Engaging");
			return;
		}
		
		


	}
	void exit(Actor* a){
		cout << "Exiting Idle" << endl;
	}
};
