#ifndef _BRUTE
#define _BRUTE
#include "Actor_States.cpp"
InfluenceMap* AIController::s_InfluenceMap;
class BruteEngaging : public AState{
private:
	
	const double attacking_range = 0.5;
 
public:
	void enter(Actor* a){ 
		cout << "Entering Brute Engaging" <<endl;
		//Enemy = null;//Engine.getClosestOrWhatever()...
	
	
	}
	void execute(Actor* a){
		Actor* Enemy = a->getController()->getEnemy();
		if(!*Enemy){
			a->getFSM()->changeState("Idle");
			return;
		}
			
	
		int x, y;
		getBestPosition(a, &x, &y);
		double real_x = (x+0.5)*(Engine.getWidth() / AIController::s_InfluenceMap->getMapWidth());
		double real_y = (y+0.5)*(Engine.getWidth() / AIController::s_InfluenceMap->getMapHeight());
		str = SteeringBehaviors<Vector3D>::seek(Vector3D(real_x, real_y, a->getVehicle()->getPos().z), a->getVehicle);
		stf += a->getVehicle()->getCurrVel();
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
		vr = a->getViewRadius();

		int up, down, left, right;
			up = a_y-vr;
			down = a_y+vr;
			left = a_x-vr;
			right = a_x+vr;
			while(down >= AIController::s_InfluenceMap->getMapHeight)
				down--;
			
			while(up< 0)
				up++;
			
			while(left < 0)
				left++;

			while(right >= AIController::s_InfluenceMap->getMapWidth)
				right--;
			
			
			for(int i=up; i<=down; i++)
				for(int j=left; j<=right; j++){
					if(AIController::s_InfluenceMap->getSpot(i, j) < AIController::s_InfluenceMap->getSpot(*y, *x)){
						*x = j;
						*y = i;
					}
				}
	
	}

};

class BruteAttack : public AState(){
private:
	Vector3D m_vEnemyPos
public:
	void enter(Actor* a){
		m_vEnemyPos = a->getController()->getEnemy()->getVehicle()->getPos();
	}
	void execute(Actor* a){
		if(m_vEnemyPos != a->getController()->getEnemy()->getVehicle()->getPos()){
			a->getFSM()->changeState("Engaging");
			return;
		}
		cout << "BITE :[]" << endl;
	}
	void exit(Actor* a){}

};

#endif