#ifndef _SKIRK
#define _SKIRK
#include "Actor_States.cpp"

//InfluenceMap* AIController::s_InfluenceMap;

class SkirkEngaging : public AState<Actor> {
private:

	static const double attacking_range;
public:
	void enter(Actor* a){ 
		cout << "Entering Skirk Engaging" <<endl;
		//Enemy = a->getController()->getEnemy();//Engine.getClosestOrWhatever()...
	
	
	}
	void execute(Actor* a){
		Actor* Enemy = a->getController()->getEnemy(); //...
		if(!Enemy){
			a->getFSM()->changeState("Idle");
			return;
		}
			
		int x, y;
		getBestPosition(a, &x, &y);
		int m_x, m_y;	
		AIController::s_InfluenceMap->mapCoords(a->getVehicle()->getPos(), &m_x, &m_y);
		double real_x = (x+0.5)*(/*Engine.getWidth()*/  100.0 / AIController::s_InfluenceMap->getMapWidth());
		double real_y = (y+0.5)*(/*Engine.getWidth()*/ 100.0 / AIController::s_InfluenceMap->getMapHeight());
		Vector3D stf;
		if(abs(m_x - x) < a->getController()->getInfluenceRadius() || abs(m_y - y) < a->getController()->getInfluenceRadius()){
			stf = SteeringBehaviors<Vector3D>::pursuit(a->getVehicle(), Enemy->getVehicle());
			stf += SteeringBehaviors<Vector3D>::seek(Vector3D(real_x, real_y, a->getVehicle()->getPos().z), a->getVehicle());
			if((a->getVehicle()->getPos() - Enemy->getVehicle()->getPos()).magnitude() < attacking_range){
				a->getFSM()->changeState("Attack");
				return;
			}
		}else{
			stf = SteeringBehaviors<Vector3D>::evade(a->getVehicle(), Enemy->getVehicle());
		}  
		stf += a->getVehicle()->getCurrVel();
		stf.normalize();
		a->getVehicle()->setCurrVel(stf);

		
	}
	void exit(Actor* a){
		cout << "Exiting Skirk Engaging" << endl;

	}

	void getBestPosition(Actor* a, int* x, int* y){
		int a_x, a_y;
		
		AIController::s_InfluenceMap->mapCoords(a->getVehicle()->getPos(), &a_x, &a_y);
		*x = a_x;
		*y = a_y;
		int vr = a->getViewRadius();

		int up, down, left, right;
			up = a_y-vr;
			down = a_y+vr;
			left = a_x-vr;
			right = a_x+vr;
			while(down >= AIController::s_InfluenceMap->getMapHeight())
				down--;
			
			while(up< 0)
				up++;
			
			while(left < 0)
				left++;

			while(right >= AIController::s_InfluenceMap->getMapWidth())
				right--;
			
			
			for(int i=up; i<=down; i++)
				for(int j=left; j<=right; j++){
					if(AIController::s_InfluenceMap->getSpot(i, j) > AIController::s_InfluenceMap->getSpot(*y, *x)){
						*x = j;
						*y = i;
					}
				}
	
	}

};

const double SkirkEngaging::attacking_range = 1.5;

class SkirkAttack : public AState<Actor> {

	void enter(Actor* a){
		cout << "Entering Skirk Attack" << endl;
	}
	void execute(Actor* a){
		Actor* Enemy = a->getController()->getEnemy();
		Vector3D stf = SteeringBehaviors<Vector3D>::pursuit(a->getVehicle(), Enemy->getVehicle());
		stf.normalize();
		a->getVehicle()->setCurrVel(stf*(a->getVehicle()->getMaxSpeed()+0.2));
		cout << "Chargin  >> " << endl;
		a->getFSM()->changeState("Engaging");

	}
	void exit(Actor* a){
		cout << "Exiting Skirk Attack" << endl;
	
	}

};
#endif