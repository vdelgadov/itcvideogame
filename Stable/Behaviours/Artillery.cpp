#ifndef _ART
#define _ART
#include "Actor_States.cpp"


//InfluenceMap* AIController::s_InfluenceMap = NULL;



class ArtEngaging : public AState<Actor>{
private:

    static const double attacking_range;	
 
public:
	void enter(Actor* a){ 
		cout << "Entering Artillery Engaging" <<endl;
		//Enemy = null;//Engine.getClosestOrWhatever()...
	
	
	}
	void execute(Actor* a){
		AIController* aic = (AIController*)(a->getController());
		CObject* Enemy = aic->getEnemy();
		
		if(!Enemy){
			a->getFSM()->changeState("Idle");
			return;
		}
			
		Vector3D stf;
		int x, y;
		getBestPosition(a, &x, &y);
		double real_x = (x+0.5)*(/*Engine.getWidth()*/ 100.0 / AIController::s_InfluenceMap->getMapWidth());
		double real_y = (y+0.5)*(/*Engine.getWidth()*/ 100.0 / AIController::s_InfluenceMap->getMapHeight());
		stf = SteeringBehaviors<Vector3D>::seek(Vector3D(real_x, real_y, a->getVehicle()->getPos().z), a->getVehicle());
		stf += a->getVehicle()->getCurrVel();

		if((Enemy->getVehicle()->getPos() - a->getVehicle()->getPos()).magnitude() < 1)
			stf += SteeringBehaviors<Vector3D>::evade(a->getVehicle(), Enemy->getVehicle());
		stf.normalize();
		a->getVehicle()->setCurrVel(stf);
		
		if((a->getVehicle()->getPos() - Enemy->getVehicle()->getPos()).magnitude() < attacking_range){
			a->getFSM()->changeState("Attack");
			return;
		}
		
	}
	void exit(Actor* a){
		cout << "Exiting Brute Engaging" << endl;

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
const double ArtEngaging::attacking_range = 3.0;


class ArtAttack : public AState<Actor> {

	void enter(Actor* a){
		cout << "Entering Artillery Attack" << endl;
	}
	void execute(Actor* a){
	    AIController* aic = (AIController*)(a->getController());
		CObject* Enemy = aic->getEnemy();
		cout << "Fireball   <<O  " << endl;
		a->getFSM()->changeState("Engaging");

	}
	void exit(Actor* a){
		cout << "Exiting Artillery Attack" << endl;
	
	}

};
#endif