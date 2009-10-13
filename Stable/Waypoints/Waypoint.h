#ifndef WAYPOINT_H
#define WAYPOINT_H
#include <iostream>
#include <list>
#include <string>
#include "../SteeringBehaviors/Vector2D.h"



using namespace std;
template <class V>
class Waypoint{
public:
		Waypoint(){
			this->m_vPos = V();
			this->m_sId = "";
			this->m_lNbors = list<Waypoint<V>*>();
		}

		Waypoint(string i){
			this->m_vPos = V();
			this->m_sId = i;
			this->m_lNbors = list<Waypoint<V>*>();
		}

		Waypoint(string i, V p){
			this->m_vPos = p;
			this->m_sId = i;
			this->m_lNbors = list<Waypoint<V>*>();
		}



		double sqDistanceTo(Vector2D to){
			return ((to.x - this->m_vPos.x)*(to.x - this->m_vPos.x) + (to.y - this->m_vPos.y)*(to.y - this->m_vPos.y));
		}

	/*	double sqDistanceTo(Vector3D to){
			return 0;
		}*/



		string getId(){
			return this->m_sId;
		}

		V getPos(){
			return this->m_vPos;
		}


		void addNbor(Waypoint *n){
			this->m_lNbors.push_back(n);
		}

		bool isNbor(Waypoint* to){
			list<Waypoint<V> *>::iterator it;
			
			for(it = this->m_lNbors.begin();it != this->m_lNbors.end(); ++it){
				if((*it)->getId().compare(this->m_sId)){
					return true;
				}
			}
			

			return false;
		}

	

		Waypoint* closestNborTo(V to){
			list<Waypoint<V> *>::iterator it;
			Waypoint* closest = this;
			double lowest_dist = this->sqDistanceTo(to);
			for(it = this->m_lNbors.begin();it != this->m_lNbors.end(); ++it){
				double it_dist = (*it)->sqDistanceTo(to) + this->sqDistanceTo((*it)->getPos());
				if(it_dist <= lowest_dist){
					closest = (*it);
					lowest_dist = it_dist;
				}
			}
			
			return closest;
		}

	list<Waypoint*> m_lNbors;

	~Waypoint(){
	
	}

private:
	V m_vPos;
	string m_sId;

};




template <class V>
class WaypointEdge{
public:
	WaypointEdge(){
		this->m_fCost = 0;
		this->m_fHeur = 0;
	}

	WaypointEdge(Waypoint<V>* f, Waypoint<V>* t, float h_cost){
		this->m_pFrom = f;
		this->m_pTo = t;
		this->m_fCost = f->sqDistanceTo(t->getPos());
		this->m_fHeur = h_cost;
	}

	int operator<(const WaypointEdge &rhs) const{
		return (this->m_fCost+this->m_fHeur) < (rhs.m_fCost + rhs.m_fHeur);
	}

	float m_fCost;
	float m_fHeur;
	Waypoint<V>* m_pFrom;
	Waypoint<V>* m_pTo;
	int extra_info;
};
template <class V>
class compEdges{
public:
	int operator()(const WaypointEdge<V> &a, const WaypointEdge<V> &b){
		return a < b;
	}

};

#endif