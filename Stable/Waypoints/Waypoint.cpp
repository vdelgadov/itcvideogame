#include "Waypoint.h"

Waypoint::Waypoint(){
	this->m_vPos = Vector2D();
	this->m_pId = "";
	this->m_lNbors = list<Waypoint*>();
}

Waypoint::Waypoint(string i){
	this->m_vPos = Vector2D();
	this->m_pId = i;
	this->m_lNbors = list<Waypoint*>();
}

Waypoint::Waypoint(string i, Vector2D p){
	this->m_vPos = p;
	this->m_pId = i;
	this->m_lNbors = list<Waypoint*>();
}

void Waypoint::addNbor(Waypoint *n){
	this->m_lNbors.push_back(n);
}

bool Waypoint::isNbor(Waypoint* to) const{
	list<Waypoint *>::const_iterator it;
	
	for(it = this->m_lNbors.begin();it != this->m_lNbors.end(); ++it){
		if((*it)->getId().compare(this->m_pId)){
			return true;
		}
	}
	

	return false;
}

double Waypoint::sqDistanceTo(Vector2D to){
	return (to.x - this->m_vPos.x)*(to.x - this->m_vPos.x) + (to.y - this->m_vPos.x)*(to.y - this->m_vPos.y);

}

Waypoint* Waypoint::closestNborTo(Vector2D to){
	list<Waypoint *>::const_iterator it;
	Waypoint* closest = this;
	double lowest_dist = closest->sqDistanceTo(to);
	for(it = this->m_lNbors.begin();it != this->m_lNbors.end(); ++it){
		double it_dist = (*it)->sqDistanceTo(to) + this->sqDistanceTo((*it)->getPos());
		if(it_dist <= lowest_dist){
			closest = (*it);
			lowest_dist = it_dist;
		}
	}

	return closest;
}
