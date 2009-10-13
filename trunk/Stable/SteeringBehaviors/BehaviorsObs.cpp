#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
Vector2D SteeringBehaviors::seek(Vector2D t_pos, Vehicle* veh){
	
	Vector2D steering_force = (t_pos - veh->getPos());
	if(steering_force.magnitude() < veh->getMaxSpeed())
		return Vector2D();
	
	steering_force.normalize();

	
	steering_force *= veh->getMaxSpeed() ;
	return steering_force;
}

Vector2D SteeringBehaviors::flee(Vector2D t_pos, Vehicle* veh){

	Vector2D steering_force = (veh->getPos() - t_pos);
	
	if(steering_force.magnitude() > 10){
		steering_force = Vector2D();
		return steering_force;
	}
	
	steering_force.normalize();

	steering_force *= veh->getMaxSpeed();


	
	return steering_force;
}

Vector2D SteeringBehaviors::pursuit(Vehicle *veh, Vehicle *target){
	Vector2D calcSpot = target->getHeading()*target->getMaxSpeed() + target->getPos();
	return SteeringBehaviors::seek(calcSpot, veh);
}

Vector2D SteeringBehaviors::evade(Vehicle* veh, Vehicle* pursuer){
	Vector2D calcSpot = pursuer->getHeading()*pursuer->getMaxSpeed() + pursuer->getPos();
	return SteeringBehaviors::flee(calcSpot, veh);
}

Vector2D SteeringBehaviors::wander(Vehicle *veh, double wander_radius, double wander_distance){
		double theta = 	(double(rand())/RAND_MAX)*2*M_PI;
		double y = wander_radius * sin(theta);
		double x = wander_radius * cos(theta);
		Vector2D wander_vec = Vector2D(x, y);
		Vector2D wander_offset = Vector2D(wander_distance/2, wander_distance/2);
		wander_vec += wander_offset;
		wander_vec.normalize();
		return wander_vec * veh->getMaxSpeed();// - veh->getPos(); 
} 

Vector2D SteeringBehaviors::interpose(Vehicle *veh, Vehicle *to_int1, Vector2D point){
		Vector2D calc_spot = to_int1->getCurrVel() + to_int1->getPos();
		Vector2D new_spot = calc_spot - point;
		new_spot *= 0.5;
		return SteeringBehaviors::seek(new_spot, veh);
}

Vector2D SteeringBehaviors::interpose(Vehicle *veh, Vehicle *to_int1, Vehicle *to_int2){
		Vector2D calc_spot1 = to_int1->getCurrVel() + to_int1->getPos();
		Vector2D calc_spot2 = to_int2->getCurrVel() + to_int2->getPos();
		Vector2D new_spot = calc_spot2 - calc_spot1;
		new_spot *= 0.5;
		new_spot += calc_spot1;
		return SteeringBehaviors::seek(new_spot, veh);
}

Vector2D SteeringBehaviors::interpose(Vehicle *veh, Vector2D point1, Vector2D point2){
		Vector2D new_spot = point2 - point1;
		new_spot *= 0.5;
		new_spot += point1;
		return SteeringBehaviors::seek(new_spot, veh);
}

/*void SteeringBehaviors::align(std::list<Vehicle*> nbors){
	std::list<Vehicle>::iterator it;
	Vector2D avg;
	double sum_x, sum_y;
	int ents = 0;
	for(it = nbors.begin(); it != nbors.end(); it++){
		sum_x += (*it)->getPos().x;
		sum_y += (*it)->getPos().y;
		++ents;
	}

	if(ents != 0){
		avg = Vector2D(sum_x/ents, sum_y/ents);
	}

	for(it = nbors.begin(); it != nbors.end(); it++){
		(*it)->setCurrVel(avg);
	}
}*/