#pragma once



#include <list>
#include <math.h>



//#include "../AIController/Actor.h"

#include "../AIController/AIController.h"
#include "../Main/Controller.h"

#include "../Main/Vector2D.h"

#include "../Main/Vector3D.h"
#include "../Main/CObjectMesh.cpp"
using namespace std;

class Actor;
class InfluenceMap {
private:
	double **m_dMap;
	double m_dWidth, m_dHEight;
	list<CObjectMesh*> m_lActors;
	int m_iMapWidth;
	int m_iMapHeight;

public:
	
	int getMapWidth(){
		return this->m_iMapWidth;
	}

	int getMapHeight(){
		return this->m_iMapHeight;
	}
	InfluenceMap(int map_width, int map_height, double width, double height){
		this->m_dMap = new double*[map_height];
		for(int i = 0; i<map_height; i++){
			this->m_dMap[i] = new double[map_width];
			for(int j=0; j<map_width; j++)
				this->m_dMap[i][j] = 0;
		}
		this->m_iMapHeight = map_height;
		this->m_iMapWidth = map_width;
		this->m_dWidth = width;
		this->m_dHEight = height;

		
	}
	

	void setActorList(list<CObjectMesh*> new_list){
		this->m_lActors = new_list;
		this->update();
	}

	void addActor(CObjectMesh* a){
		this->m_lActors.push_back(a);
		this->update();
	}

	void mapCoords(Vector2D pos, int* map_x, int* map_y){
		double x_interval = this->m_dWidth/this->m_iMapWidth;
		double y_interval = this->m_dHEight/this->m_iMapHeight;
		*map_x = int(pos.x / x_interval);
		*map_y = int(pos.y / y_interval);
		
	}

	void mapCoords(Vector3D pos, int* map_x, int* map_z){
		this->mapCoords(Vector2D(pos.x, pos.z), map_x, map_z);
	
	}

	void update(double time=0);

	double getSpot(int r, int c){
		return this->m_dMap[r][c];
	}

};
