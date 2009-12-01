#include "CObject.h"
#include "CObjectMesh.cpp"

CObject* CObject::findClosestTo(const Vector3D& point, int t){
		if (lstChilds.empty())
			return this;
		CObject *closest = NULL;
		
		double smallestdist = DBL_MAX;

		for (list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			double dist = ((*it)->vehicle->getPos() - point).magnitude() < smallestdist;
			CObjectMesh* com = (CObjectMesh*)*it;
			if (com->isRendereable && com->getController()->getCategory() == t && dist < smallestdist) {
				closest = *it;
				smallestdist = dist;
			}
		}
		return closest->findClosestTo(point, t);
	}