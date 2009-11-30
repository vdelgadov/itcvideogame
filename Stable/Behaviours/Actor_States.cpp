#pragma once

#include "../StateMachine/states.h"
#include "../AIController/Actor.h"
#include "../SteeringBehaviors/Behaviors.h"
#include "../InfluenceMaps/InfluenceMap.h"

class Idle : public AState<Actor> {
	void enter(Actor* a){ cout << "Entering Idle" <<endl;}
	void execute(Actor* a){
		AIController* aic = (AIController*)(a->getController());
		Actor* Enemy = aic->getEnemy();

		int e_x, e_y, m_x, m_y;
		if(!Enemy)
			return;
		AIController::s_InfluenceMap->mapCoords(Enemy->getVehicle()->getPos(), &e_x, &e_y);
		AIController::s_InfluenceMap->mapCoords(a->getVehicle()->getPos(), &m_x, &m_y);
		
		if(abs(m_x - e_x) < a->getViewRadius() || abs(m_y - e_y) < a->getViewRadius()){
			a->getFSM()->changeState("Engaging");
			return;
		}
		Vector3D w_v = SteeringBehaviors<Vector3D>::wander(a->getVehicle(), 15, 15);
		w_v += a->getVehicle()->getCurrVel();
		w_v.normalize();
		a->getVehicle()->setCurrVel(w_v*a->getVehicle()->getMaxSpeed());

	}
	void exit(Actor* a){
		cout << "Exiting Idle" << endl;
	}
};
