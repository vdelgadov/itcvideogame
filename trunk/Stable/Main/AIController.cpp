

#include "../AIController/AIController.h"



#include "../AIController/Actor.h"



#include "../SteeringBehaviors/Behaviors.h"


#include "../Behaviours/RoleFactory.cpp"





list<Waypoint<Vector3D>*> AIController::s_lMap;
InfluenceMap* AIController::s_InfluenceMap;

map<string, ScriptedState<Actor>, ltstr> fillFSM(role_t rol){
	map<string, ScriptedState<Actor>, ltstr> states;
	states["Attack"] = RoleFactory::CreateRoleState(rol, ATTACK);
	states["Engage"] =  RoleFactory::CreateRoleState(rol, ENGAGE);

	return states;
}
class Idle : public AState<Actor>{
	void enter(Actor* a)
	{ 
		//cout << "Entering Idle" <<endl;
	}
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

		Vector3D stf = SteeringBehaviors<Vector3D>::seek(m_pCurrent->getPos(), actor->getVehicle());
		Vector3D norm_vel = actor->getVehicle()->getCurrVel();
		norm_vel += stf;
		norm_vel.normalize();
		cout << "Going after " << m_pCurrent->getId()<<" " << actor->getVehicle()->getCurrVel().x<<" "<<actor->getVehicle()->getCurrVel().y<<endl;
		actor->getVehicle()->setCurrVel(norm_vel*actor->getVehicle()->getMaxSpeed());
//		actor->getVehicle()->update(actor->getController()->getFSM()->getTime());
	}

	
	void exit(Actor* actor){
		
	}

	~FollowingWaypoint(){
	}

private:
	Waypoint<Vector3D>* m_pCurrent;


};











AIController::AIController(Actor* owner, category_t cat=BAD_GUY, role_t rol=BRUTE, int irad=1){
//	m_pFsm = new FSM<Actor>(owner);
	m_pFsm = new FSM<Actor>(fillFSM(rol), "Idle", owner);	
	m_pFsm->setStart(new Idle());
	m_pOwner = owner;
	this->m_pOwner->setController(this);
	this->m_Category = cat;
	this->m_Role = rol;
	if(!AIController::s_InfluenceMap)
		AIController::s_InfluenceMap = new InfluenceMap(10, 10, 200, 200); //NECESITO PREGUNTARLE AL ENGINE LAS DIMENSIONES DEL MAPA
	AIController::s_InfluenceMap->addActor(this);
	this->m_iInfluenceRadius = irad;
}

AIController::AIController(Actor* owner, FSM<Actor>* f){
	m_pFsm = f;
	m_pOwner = owner;

}
Actor* AIController::getActor(){
	return m_pOwner;
}

FSM<Actor>* AIController::getFSM(){
	return m_pFsm;
}

void AIController::setFSM(FSM<Actor>* fsm){
	m_pFsm = fsm;
}

void AIController::update(double time){
	this->m_pEnemy = null; //Engine ...
	this->m_pFsm->update();

}

void AIController::planPath(Waypoint<Vector3D>* w){
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
		
		for(it = current.m_pTo->m_lNbors.begin(); it != current.m_pTo->m_lNbors.end(); ++it){
			pq.push(WaypointEdge(current.m_pTo, (*it), current.m_pTo->sqDistanceTo((*it)->getPos())+current.m_fCost));
		}
		visited.push_back(current);
	}


	return;*/

	Waypoint<Vector3D>* last = this->findClosestWaypoint();
	m_lPath.push_back(last);
	while(true){
		Waypoint<Vector3D>* next = last->closestNborTo(w->getPos());
		if(next == last){
			break;
		}
		this->m_lPath.push_back(next);
		last = next;
	}
}

void AIController::buildPath(Waypoint<Vector3D>* start, WaypointEdge<Vector3D> end){
	Waypoint<Vector3D>* from = end.m_pFrom;
	cout << "Screwed" <<endl;
	/*while(start != from){
		this->m_lPath.push_back(from);
	}*/
		
}

Waypoint<Vector3D>* AIController::findClosestWaypoint(){
	list<Waypoint<Vector3D> *>::iterator it;
	Waypoint<Vector3D>* closest;
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


