#pragma once



#include <list>
#include <math.h>



#include "../AIController/Actor.h"

#include "../AIController/AIController.cpp"

ASDFNJASDKJN
#pragma warning "FUCKING JAVA"

#include "../SteeringBehaviors/Vector2D.h"

#include "../SteeringBehaviors/Vector3D.h"
using namespace std;
class InfluenceMap {
private:
	double **m_dMap;
	double m_dWidth, m_dHEight;
	list<Actor*> m_lActors;
	int m_iMapWidth, m_iMapHeight;

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
	

	void setActorList(list<Actor*> new_list){
		this->m_lActors = new_list;
		this->update();
	}

	void addActor(Actor* a){
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

	void update(double time=0){
		list<Actor*>::iterator it = this->m_lActors.begin();
		int c, r;
		for(c=0; c<this->m_iMapHeight; c++)
			for(r=0;r<this->m_iMapWidth; r++)
				this->m_dMap[r][c] = 0;

		for(; it != this->m_lActors.end(); ++it){
			this->mapCoords((*it)->getVehicle()->getPos(), &c, &r);
			int inf_rad = (*it)->getController()->getInfluenceRadius();
			double influence = pow(-1.0, (*it)->getController()->getCategory());

			int up, down, left, right;
			up = r-inf_rad;
			down = r+inf_rad;
			left = c-inf_rad;
			right = c+inf_rad;
			while(down >= this->m_iMapHeight)
				down--;
			
			while(up< 0)
				up++;
			
			while(left < 0)
				left++;

			while(right >= this->m_iMapWidth)
				right--;
			

			for(int i=up; i<=down; i++)
				for(int j=left; j<=right; j++)
					this->m_dMap[i][j] += influence;
		}
	
	
	
	
	
	}


	double getSpot(int r, int c){
		return this->m_dMap[r][c];
	}

};
