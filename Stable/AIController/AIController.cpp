#include "AIController.h"
#include "Actor.h"
#include "../SteeringBehaviors/Behaviors.h"

list<Waypoint<Vector2D>*> AIController::s_lMap;


class Idle : public AState<Actor>{
	void enter(Actor* a){ cout << "Entering Idle" <<endl;}
	void execute(Actor* a){

	}
	void exit(Actor* a){}
};

class FollowingWaypoint : public AState<Actor>{
public:
	FollowingWaypoint(){}
	void enter(Actor* actor){
	
		this->m_pCurrent = actor->getController()->m_lPath.front();
		actor->getController()->m_lPath.pop_front();
	}

	void execute(Actor* actor){
		if(this->m_pCurrent->sqDistanceTo(actor->getVehicle()->getPos()) < actor->getVehicle()->getMaxSpeed()){
			if(actor->getController()->m_lPath.size() < 1){
				actor->getController()->getFSM()->changeState(new Idle());
				return;
			}
			actor->getController()->getFSM()->changeState(new FollowingWaypoint());
			return;
		}

		Vector2D stf = SteeringBehaviors<Vector2D>::seek(m_pCurrent->getPos(), actor->getVehicle());
		Vector2D norm_vel = actor->getVehicle()->getCurrVel();
		norm_vel += stf;
		norm_vel.normalize();
		//cout << "Going after " << m_pCurrent->getId()<<" " << actor->getVehicle()->getCurrVel().x<<" "<<actor->getVehicle()->getCurrVel().y<<endl;
		actor->getVehicle()->setCurrVel(norm_vel*actor->getVehicle()->getMaxSpeed());
		actor->update(actor->getController()->getFSM()->getTime());
			
		//	getVehicle()->update(actor->getController()->getFSM()->getTime());
	}

	
	void exit(Actor* actor){
		
	}

	~FollowingWaypoint(){
	}

private:
	Waypoint<Vector2D>* m_pCurrent;
	

};


AIController::AIController(Actor* owner){
	m_pFsm = new FSM<Actor>(owner);
	m_pFsm->setStart(new Idle());
	m_pOwner = owner;
	this->m_pOwner->setController(this);
}

Actor* AIController::getActor(){
	return m_pOwner;
}

FSM<Actor>* AIController::getFSM(){
	return m_pFsm;
}

void AIController::update(double time){
	this->m_pFsm->update();
}

void AIController::planPath(Waypoint<Vector2D>* w){
	this->m_lPath.clear();
/*	list<WaypointEdge> visited;
	list<Waypoint*>::iterator it;
	priority_queue<WaypointEdge, vector<WaypointEdge>, compEdges> pq;
	Waypoint* start = this->findClosestWaypoint();
	//Waypoint* next = start->closestNborTo(start->getPos());
	
	if(start == w){
		this->m_lPath.push_back(start);
		return;
	}


	WaypointEdge current;
	for(it = start->m_lNbors.begin(); it != start->m_lNbors.end(); ++it){
		
		pq.push(WaypointEdge(start, (*it), start->sqDistanceTo(w->getPos())));
	}

	
	while(!pq.empty()){
		current = pq.top();
		if(current.m_pTo == w){
			this->buildPath(start, current);
			return;
		}
		//if(visited.
		cout << "here" <<endl;
		for(it = current.m_pTo->m_lNbors.begin(); it != current.m_pTo->m_lNbors.end(); ++it){
			pq.push(WaypointEdge(current.m_pTo, (*it), current.m_pTo->sqDistanceTo((*it)->getPos())+current.m_fCost));
		}
		visited.push_back(current);
	}


	return;*/

	Waypoint<Vector2D>* last = this->findClosestWaypoint();
	m_lPath.push_back(last);
	while(true){
		Waypoint<Vector2D>* next = last->closestNborTo(w->getPos());
		if(next == last){
			break;
		}
		this->m_lPath.push_back(next);
		last = next;
	}
}

void AIController::buildPath(Waypoint<Vector2D>* start, WaypointEdge<Vector2D> end){
	Waypoint<Vector2D>* from = end.m_pFrom;
	cout << "Screwed" <<endl;
	/*while(start != from){
		this->m_lPath.push_back(from);
	}*/
		
}

Waypoint<Vector2D>* AIController::findClosestWaypoint(){
	list<Waypoint<Vector2D> *>::iterator it;
	Waypoint<Vector2D>* closest;
	double lowest_dist = 99999;
	for(it = AIController::s_lMap.begin();it != AIController::s_lMap.end(); ++it){
		double it_dist = (*it)->sqDistanceTo(this->getActor()->getVehicle()->getPos());
		if(it_dist < lowest_dist){
			closest = (*it);
			lowest_dist = it_dist;
		}
	}
	
	return closest;
}

void AIController::traversePath(){
	this->m_pFsm->changeState(new FollowingWaypoint());
}


