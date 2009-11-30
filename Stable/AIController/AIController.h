#ifndef AI_CONT_H
#define AI_CONT_H
#include <list>
#include <queue>
#include "../Waypoints/Waypoint.h"
#include "../StateMachine/FSM.h"

class Actor;
class VM{

public:
	void execute(int x){}

};

class AIController {//: public Controller {
	public:
		AIController(Actor* owner);
		FSM<Actor>* getFSM();
		Actor* getActor();
		void update(double time);
		void planPath(Waypoint<Vector3D>* w);
		Waypoint<Vector3D>* findClosestWaypoint();
		void traversePath();
		void buildPath(Waypoint<Vector3D>* w, WaypointEdge<Vector3D> we);
		void setFSM(FSM<Actor>* fsm);
		VM* getVM(){
			return this->m_pVM;
		}
		

		static list<Waypoint<Vector3D>*> s_lMap;
		list<Waypoint<Vector3D>*> m_lPath;



	private:
		Actor* m_pOwner;
		FSM<Actor>* m_pFsm;
		VM* m_pVM;
};
#endif 