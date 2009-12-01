#include "../InfluenceMaps/InfluenceMap.h"
#include "Controller.h"
InfluenceMap* Controller::s_InfluenceMap;
Controller::Controller(CObjectMesh* owner)
{
	this->m_Category = GOOD_GUY;
	this->m_iInfluenceRadius = 1;
	if(!Controller::s_InfluenceMap)
		Controller::s_InfluenceMap = new InfluenceMap(50, 50, 100, 100); //NECESITO PREGUNTARLE AL ENGINE LAS DIMENSIONES DEL MAPA
	Controller::s_InfluenceMap->addActor(owner);
}