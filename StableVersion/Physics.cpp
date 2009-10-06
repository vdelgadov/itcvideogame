#include "Physics.h"
#include "CObject.h"

bool Physics::checkBoundingSphere(CObject* obj1, CObject* obj2)
{
	if(!obj2 || obj2->ID == 0)
	{
		return false;
	}

	//Initialize the return value
  //double t = 0.0f;

  // Relative velocity
  Vector3D    dv    = obj2->vehicle.getCurrVel() - obj1->vehicle.getCurrVel();
  // Relative position
  Vector3D    dp    = obj2->vehicle.getPos() - obj1->vehicle.getPos();
  //Minimal distance squared
  double r = obj1->Radius + obj2->Radius;
  //dP^2-r^2
  double pp = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z - r*r;
  //(1)Check if the spheres are already intersecting
  if ( pp < 0 ) return true;

  //dP*dV
  double pv    = dp.x * dv.x + dp.y * dv.y + dp.z * dv.z;
  //(2)Check if the spheres are moving away from each other
  if ( pv >= 0 ) return false;

  //dV^2
  double vv = dv.x * dv.x + dv.y * dv.y + dv.z * dv.z;
  //(3)Check if the spheres can intersect within 1 frame
  if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 ) return false;

  //tmin = -dP*dV/dV*2 
  //the time when the distance between the spheres is minimal
  double tmin = -pv/vv;

  //Discriminant/(4*dV^2) = -(dp^2-r^2+dP*dV*tmin)
  return ( pp + pv * tmin > 0 );


	
}
