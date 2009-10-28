#include "AIController.h"
#include "Actor.h"

int main(){
	AIController a_c = AIController(new Actor());
	
	Waypoint<Vector2D>* a = new Waypoint<Vector2D>("a", Vector2D());
	Waypoint<Vector2D>* b = new Waypoint<Vector2D>("b", Vector2D(0.0, 5.0));
	Waypoint<Vector2D>* c = new Waypoint<Vector2D>("c", Vector2D(5.0, 0.0));
	Waypoint<Vector2D>* d = new Waypoint<Vector2D>("d", Vector2D(10.0, 0.0));
	Waypoint<Vector2D>* e = new Waypoint<Vector2D>("e", Vector2D(7.5, -5.0));
	Waypoint<Vector2D>* f = new Waypoint<Vector2D>("f", Vector2D(7.5, 5.0));
	Waypoint<Vector2D>* g = new Waypoint<Vector2D>("g", Vector2D(10.0, -5.0));
	Waypoint<Vector2D>* h = new Waypoint<Vector2D>("h", Vector2D(10.0, -10.0));
	
	a_c.getActor()->getVehicle()->setMaxSpeed(0.1);


	AIController::s_lMap.push_front(a);
	AIController::s_lMap.push_front(b);
	AIController::s_lMap.push_front(c);
	AIController::s_lMap.push_front(d);
	AIController::s_lMap.push_front(e);
	AIController::s_lMap.push_front(f);
	AIController::s_lMap.push_front(g);
	AIController::s_lMap.push_front(h);

	a->addNbor(b);
	a->addNbor(c);
	c->addNbor(d);
	c->addNbor(e);
	e->addNbor(g);
	g->addNbor(h);

	//b->addNbor(e);
	b->addNbor(f);

	a_c.planPath(h);

	//a_c.planPath(e);
	a_c.traversePath();
	

	while(true){
		a_c.update(1);
	};

}