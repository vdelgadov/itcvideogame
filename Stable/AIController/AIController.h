#ifndef AI_CONT_H
#define AI_CONT_H
//#include "Actor.h"
#include <list>
#include <queue>
#include "../Waypoints/Waypoint.h"
#include "../StateMachine/FSM.h"

class Actor;

class AIController {//: public Controller {
	public:
		AIController(Actor* owner);
		FSM<Actor>* getFSM();
		Actor* getActor();
		void update(double time);
		void planPath(Waypoint<Vector2D>* w);
		Waypoint<Vector2D>* findClosestWaypoint();
		void traversePath();
		void buildPath(Waypoint<Vector2D>* w, WaypointEdge<Vector2D> we);
		
		
		static list<Waypoint<Vector2D>*> s_lMap;
		list<Waypoint<Vector2D>*> m_lPath;


	private:
		Actor* m_pOwner;
		FSM<Actor>* m_pFsm;
};
#endif 