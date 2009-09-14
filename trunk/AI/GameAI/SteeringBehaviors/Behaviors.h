#include "Vehicle.h"
#include <list>

using namespace std;
class SteeringBehaviors{
	
public:
	static Vector2D seek(Vector2D t_pos, Vehicle* veh);
	static Vector2D flee(Vector2D t_pos, Vehicle* veh);
	static Vector2D pursuit(Vehicle* veh, Vehicle* target);
	static Vector2D evade(Vehicle* veh, Vehicle* pursuer);
	static Vector2D wander(Vehicle* veh, double  wander_radius, double wander_distance);
	static Vector2D interpose(Vehicle* veh, Vehicle* to_int1, Vector2D point);
	static Vector2D interpose(Vehicle* veh, Vehicle* to_int1, Vehicle* to_int2);
	static Vector2D interpose(Vehicle* veh, Vector2D point1, Vector2D point2);

};

