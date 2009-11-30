#ifndef BEHAVIORS_H
#define BEHAVIORS_H
#include "Vehicle.h"
#include <list>

using namespace std;
template <class V>
class SteeringBehaviors{
	
public:
	static V seek(V t_pos, Vehicle<V>* veh);
	static V flee(V t_pos, Vehicle<V>* veh);
	static V pursuit(Vehicle<V>* veh, Vehicle<V>* target);
	static V evade(Vehicle<V>* veh, Vehicle<V>* pursuer);
	static V wander(Vehicle<V>* veh, double  wander_radius, double wander_distance);
	static V interpose(Vehicle<V>* veh, Vehicle<V>* to_int1, V point);
	static V interpose(Vehicle<V>* veh, Vehicle<V>* to_int1, Vehicle<V>* to_int2);
	static V interpose(Vehicle<V>* veh, V point1, V point2);
	static V flock(Vehicle<V> *veh, Vehicle<V> flock[], int flockSize, double theta, double viewRadius, double evadeRadius);
	static V flockLead(Vehicle<V> *veh, Vehicle<V> flock[], Vehicle<V> *leader, int flockSize, double theta, double viewRadius, double evadeRadius, double leaderRadius);
	static V flockEvade(Vehicle<V> *veh, Vehicle<V> flock[], Vehicle<V> *predator, int flockSize,double theta, double viewRadius, double evadeRadius, double predatorRadius, int importance);
	static void align(list<Vehicle<V>*> nbors);

};

template <class V>
V SteeringBehaviors<V>::seek(V t_pos, Vehicle<V>* veh){
	
	V steering_force = (t_pos - veh->getPos());
	if(steering_force.magnitude() < veh->getMaxSpeed()){
		veh->setCurrVel(V());
		return V();
	}
	
	steering_force.normalize();

	
	//steering_force *= veh->getMaxSpeed() ;
	return steering_force;
}

template <class V>
V SteeringBehaviors<V>::flee(V t_pos, Vehicle<V>* veh){

	V steering_force = (veh->getPos() - t_pos);
	
	if(steering_force.magnitude() > 10){
		veh->setCurrVel(V());
		return V();
	}
	
	steering_force.normalize();

//	steering_force *= veh->getMaxSpeed();


	
	return steering_force;
}

template <class V>
V SteeringBehaviors<V>::pursuit(Vehicle<V> *veh, Vehicle<V> *target){
	V calcSpot = target->getHeading()*target->getMaxSpeed() + target->getPos();
	return SteeringBehaviors<V>::seek(calcSpot, veh);
}

template <class V>
V SteeringBehaviors<V>::evade(Vehicle<V>* veh, Vehicle<V>* pursuer){
	V calcSpot = pursuer->getHeading()*pursuer->getMaxSpeed() + pursuer->getPos();
	return SteeringBehaviors<V>::flee(calcSpot, veh);
}

template <class V>
V SteeringBehaviors<V>::wander(Vehicle<V> *veh, double wander_radius, double wander_distance){
		double theta = 	(double(rand())/RAND_MAX)*2*M_PI;
		double y = wander_radius * sin(theta);
		double x = wander_radius * cos(theta);
		V wander_vec = V(x, y);
		wander_vec.normalize();
		wander_vec += (veh->getHeading()*wander_distance);
		return wander_vec;// * veh->getMaxSpeed();// - veh->getPos(); 
} 

template <class V>
V SteeringBehaviors<V>::interpose(Vehicle<V> *veh, Vehicle<V> *to_int1, V point){
		V calc_spot = to_int1->getCurrVel() + to_int1->getPos();
		V new_spot = calc_spot - point;
		new_spot *= 0.5;
		return SteeringBehaviors<V>::seek(new_spot, veh);
}

template <class V>
V SteeringBehaviors<V>::interpose(Vehicle<V> *veh, Vehicle<V> *to_int1, Vehicle<V> *to_int2){
		V calc_spot1 = to_int1->getCurrVel() + to_int1->getPos();
		V calc_spot2 = to_int2->getCurrVel() + to_int2->getPos();
		V new_spot = calc_spot2 - calc_spot1;
		new_spot *= 0.5;
		new_spot += calc_spot1;
		return SteeringBehaviors<V>::seek(new_spot, veh);
}

template <class V>
V SteeringBehaviors<V>::interpose(Vehicle<V> *veh, V point1, V point2){
		V new_spot = point2 - point1;
		new_spot *= 0.5;
		new_spot += point1;
		return SteeringBehaviors<V>::seek(new_spot, veh);
}

template <class V>
V SteeringBehaviors<V>::flock(Vehicle<V> *veh, Vehicle<V> flock[], int flockSize, double theta, double viewRadius, double evadeRadius){
	V resultant = V();
	V centerOfMass = V();
	int flockers = 0;
	for (int i =0; i<flockSize; i++){
		double distance = (flock[i].getPos() - veh->getPos()).magnitude();
		if (distance<=evadeRadius && distance>0) return SteeringBehaviors<V>::evade(veh, &flock[i]);
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

template <class V>
V SteeringBehaviors<V>::flockLead(Vehicle<V> *veh, Vehicle<V> flock[], Vehicle<V> *leader, int flockSize, 
									  double theta, double viewRadius, double evadeRadius, double leaderRadius){
	V resultant = V();
	V centerOfMass = V();
	int flockers = 0;
	for (int i =0; i<flockSize; i++){
		double distance = (flock[i].getPos() - veh->getPos()).magnitude();
		if (distance<=evadeRadius && distance>0) return SteeringBehaviors<V>::evade(veh, &flock[i]);
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

template <class V>
V SteeringBehaviors<V>::flockEvade(Vehicle<V> *veh, Vehicle<V> flock[], Vehicle<V> *predator, 
									  int flockSize,double theta, double viewRadius, 
									  double evadeRadius, double predatorRadius, int importance){
	V resultant = V();
	V centerOfMass = V();
	int flockers = 0;
	for (int i =0; i<flockSize; i++){
		double distance = (flock[i].getPos() - veh->getPos()).magnitude();
		if (distance<=evadeRadius && distance>0) return SteeringBehaviors<V>::evade(veh, &flock[i]);
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
		return SteeringBehaviors<V>::evade(veh, predator);
	if(disPred<=predatorRadius)
		resultant+= SteeringBehaviors<V>::evade(veh, predator) * importance;
	resultant.normalize();
	return resultant * veh->getMaxSpeed();
}

#endif