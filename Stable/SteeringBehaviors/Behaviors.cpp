#include "stdafx.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
V SteeringBehaviors::seek(V t_pos, Vehicle* veh){
	
	V steering_force = (t_pos - veh->getPos());
	if(steering_force.magnitude() < veh->getMaxSpeed())
		return V();
	
	steering_force.normalize();

	
	steering_force *= veh->getMaxSpeed() ;
	return steering_force;
}

V SteeringBehaviors::flee(V t_pos, Vehicle* veh){

	V steering_force = (veh->getPos() - t_pos);
	
	if(steering_force.magnitude() > 10){
		steering_force = V();
		return steering_force;
	}
	
	steering_force.normalize();

	steering_force *= veh->getMaxSpeed();


	
	return steering_force;
}

V SteeringBehaviors::pursuit(Vehicle *veh, Vehicle *target){
	V calcSpot = target->getHeading()*target->getMaxSpeed() + target->getPos();
	return SteeringBehaviors::seek(calcSpot, veh);
}

V SteeringBehaviors::evade(Vehicle* veh, Vehicle* pursuer){
	V calcSpot = pursuer->getHeading()*pursuer->getMaxSpeed() + pursuer->getPos();
	return SteeringBehaviors::flee(calcSpot, veh);
}

V SteeringBehaviors::wander(Vehicle *veh, double wander_radius, double wander_distance){
		double theta = 	(double(rand())/RAND_MAX)*2*M_PI;
		double y = wander_radius * sin(theta);
		double x = wander_radius * cos(theta);
		V wander_vec = V(x, y);
		V wander_offset = veh->getHeading();
		wander_vec += wander_offset;
		wander_vec.normalize();
		return wander_vec * veh->getMaxSpeed();// - veh->getPos(); 
} 

V SteeringBehaviors::interpose(Vehicle *veh, Vehicle *to_int1, V point){
		V calc_spot = to_int1->getCurrVel() + to_int1->getPos();
		V new_spot = calc_spot - point;
		new_spot *= 0.5;
		return SteeringBehaviors::seek(new_spot, veh);
}

V SteeringBehaviors::interpose(Vehicle *veh, Vehicle *to_int1, Vehicle *to_int2){
		V calc_spot1 = to_int1->getCurrVel() + to_int1->getPos();
		V calc_spot2 = to_int2->getCurrVel() + to_int2->getPos();
		V new_spot = calc_spot2 - calc_spot1;
		new_spot *= 0.5;
		new_spot += calc_spot1;
		return SteeringBehaviors::seek(new_spot, veh);
}

V SteeringBehaviors::interpose(Vehicle *veh, V point1, V point2){
		V new_spot = point2 - point1;
		new_spot *= 0.5;
		new_spot += point1;
		return SteeringBehaviors::seek(new_spot, veh);
}

V SteeringBehaviors::flock(Vehicle *veh, Vehicle flock[], int flockSize, double theta, double viewRadius, double evadeRadius){
	V resultant = V();
	V centerOfMass = V();
	int flockers = 0;
	for (int i =0; i<flockSize; i++){
		double distance = (flock[i].getPos() - veh->getPos()).magnitude();
		if (distance<=evadeRadius && distance>0) return SteeringBehaviors::evade(veh, &flock[i]);
		if(distance>viewRadius) continue;
		resultant+=(flock[i].getCurrVel() * (1/(distance+1)));
		centerOfMass+= flock[i].getPos();
		flockers++;
	}
	centerOfMass*= ((1/flockers));
	centerOfMass= veh->getPos() - centerOfMass;
	centerOfMass.normalize();
	resultant-= centerOfMass * 0.01;
	resultant.normalize();
	return resultant * veh->getMaxSpeed();
}

V SteeringBehaviors::flockLead(Vehicle *veh, Vehicle flock[], Vehicle *leader, int flockSize, 
									  double theta, double viewRadius, double evadeRadius, double leaderRadius){
	V resultant = V();
	V centerOfMass = V();
	int flockers = 0;
	for (int i =0; i<flockSize; i++){
		double distance = (flock[i].getPos() - veh->getPos()).magnitude();
		if (distance<=evadeRadius && distance>0) return SteeringBehaviors::evade(veh, &flock[i]);
		if(distance>viewRadius) continue;
		resultant+=(flock[i].getCurrVel() * (1/(distance+1)));
		centerOfMass+= flock[i].getPos();
		flockers++;
	}
	centerOfMass*= ((1/flockers));
	centerOfMass= veh->getPos() - centerOfMass;
	centerOfMass.normalize();
	resultant-= centerOfMass * 0.01;
	double disLead = (leader->getPos() - veh->getPos()).magnitude();
	if(disLead<=leaderRadius)
		resultant+=leader->getCurrVel();
	resultant.normalize();
	return resultant * veh->getMaxSpeed();
}

V SteeringBehaviors::flockEvade(Vehicle *veh, Vehicle flock[], Vehicle *predator, 
									  int flockSize,double theta, double viewRadius, 
									  double evadeRadius, double predatorRadius, int importance){
	V resultant = V();
	V centerOfMass = V();
	int flockers = 0;
	for (int i =0; i<flockSize; i++){
		double distance = (flock[i].getPos() - veh->getPos()).magnitude();
		if (distance<=evadeRadius && distance>0) return SteeringBehaviors::evade(veh, &flock[i]);
		if(distance>viewRadius) continue;
		resultant+=(flock[i].getCurrVel() * (1/(distance+1)));
		centerOfMass+= flock[i].getPos();
		flockers++;
	}
	centerOfMass*= ((1/flockers));
	centerOfMass= veh->getPos() - centerOfMass;
	centerOfMass.normalize();
	resultant-= centerOfMass * 0.01;
	double disPred = (predator->getPos() - veh->getPos()).magnitude();
	//panic, flee
	if(disPred<=predatorRadius/2)
		return SteeringBehaviors::evade(veh, predator);
	if(disPred<=predatorRadius)
		resultant+= SteeringBehaviors::evade(veh, predator) * importance;
	resultant.normalize();
	return resultant * veh->getMaxSpeed();
}