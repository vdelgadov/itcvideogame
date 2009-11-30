#ifndef AI_CONT_H
#define AI_CONT_H
#include <list>
#include <queue>
#include "../Waypoints/Waypoint.h"
#include "../StateMachine/FSM.h"
#include "../InfluenceMaps/InfluenceMap.h"

class Actor;
class VM{

public:
	void execute(int x){}

};

class AIController {//: public Controller {

	public:
		enum category_t {BAD_GUY, GOOD_GUY};
		enum role_t {BRUTE, ARTILLERY, SKIRKMISHER};
		AIController(Actor* owner, category_t cat=BAD_GUY, role_t rol=BRUTE, int irad=1);
		AIController(Actor* owner, FSM<Actor>* f);
		FSM<Actor>* getFSM();
		Actor* getActor();
		Actor* getEnemy(){
			return m_pEnemy;
		}
	
		
		
		void update(double time);
		void planPath(Waypoint<Vector3D>* w);
		Waypoint<Vector3D>* findClosestWaypoint();
		void traversePath();
		void buildPath(Waypoint<Vector3D>* w, WaypointEdge<Vector3D> we);
		void setFSM(FSM<Actor>* fsm);
		VM* getVM(){
			return this->m_pVM;
		}
		unsigned int getInfluenceRadius(){
			return this->m_iInfluenceRadius;}
		category_t getCategory(){
			return this->m_Category;
		}

		role_t getRole(){
			return m_Role;
		}

		void setRole(role_t r){
			this->m_Role = r;
		}
		void setCategory(category_t c){
			this->m_Category = c;
		}

		void setInfluenceRadius(unsigned int ir){
			this->m_iInfluenceRadius = ir;
		}
		

		static list<Waypoint<Vector3D>*> s_lMap;
		list<Waypoint<Vector3D>*> m_lPath;



		static list<Waypoint<Vector3D>*> s_lMap;
		static InfluenceMap* s_InfluenceMap;
		
		
		list<Waypoint<Vector3D>*> m_lPath;

private:
		Actor* m_pOwner;
		Actor* m_pEnemy;
		FSM<Actor>* m_pFsm;
		VM* m_pVM;
		category_t m_Category;
		role_t m_Role;
		unsigned int m_iInfluenceRadius; //Chevychev Distance



		
};
#endif 