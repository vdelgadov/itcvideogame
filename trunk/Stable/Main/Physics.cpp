#include "Physics.h"
#include "CObjectMesh.cpp"
#include "Params.h"


bool Physics::checkBoundingSphereCollision(CObject* obj)
{
	int id = obj->ID;
	for(list<CObject*>::iterator it =  obj->pScene->lstChilds.begin(); it != obj->pScene->lstChilds.end(); ++it)
	{
		
		if((id != ((*it)->ID)) & ((*it)->boundingSphere))
		{
			/*
			cout << "Verificando Boundin Sphere "<< (*it)->ID <<endl;
			cout << "Pos Me: " << "posX: " << obj->getVehicle()->getPos().x << "posY: " << obj->getVehicle()->getPos().y << "posZ: " << obj->getVehicle()->getPos().z << endl;
			cout << "Pos Other: " << "posX: " << (*it)->getVehicle()->getPos().x << "posY: " << (*it)->getVehicle()->getPos().y << "posZ: " << (*it)->getVehicle()->getPos().z << endl;
			*/
			if(checkBoundingSphereCollision(obj,(*it)))
			{
				//cout << "true" << endl;
				return true;
				
			}
		}
		
	}
	//cout << "false" << endl;
	return false;

}

bool Physics::checkBoundingSphereCollision(CObject* obj1, CObject* obj2)
{

  // Relative velocity
	Vector3D dv1 = obj1->getVehicle()->getCurrVel();
	Vector3D dv2 = obj2->getVehicle()->getCurrVel();
	Vector3D    dv    = dv2 - dv1;
  // Relative position
	Vector3D    dp1 = (obj1->getVehicle()->getPos()+Vector3D(obj1->Center.x,obj1->Center.y,obj1->Center.z));
	Vector3D    dp2 = (obj2->getVehicle()->getPos()+Vector3D(obj2->Center.x,obj2->Center.y,obj2->Center.z));
	Vector3D    dp    = dp2 - dp1;
  //Minimal distance squared
  double r = obj1->Radius + obj2->Radius;
  //dP^2-r^2
  double pp = dp.x * dp.x + dp.y * dp.y + dp.z * dp.z - r*r;
  //(1)Check if the spheres are already intersecting
  if ( pp < 0 ) 
  {
	  return true;
  }

  //dP*dV
  double pv    = dp.x * dv.x + dp.y * dv.y + dp.z * dv.z;
  //(2)Check if the spheres are moving away from each other
  if ( pv >= 0 )
  {
	  return false;
  }

  //dV^2
  double vv = dv.x * dv.x + dv.y * dv.y + dv.z * dv.z;
  //(3)Check if the spheres can intersect within 1 frame
  if ( (pv + vv) <= 0 && (vv + 2 * pv + pp) >= 0 )
  {
	  return false;
  }

  //tmin = -dP*dV/dV*2 
  //the time when the distance between the spheres is minimal
  double tmin = -pv/vv;

  //Discriminant/(4*dV^2) = -(dp^2-r^2+dP*dV*tmin)
  return ( pp + pv * tmin < 0 );
	
}


