#include "../InfluenceMaps/InfluenceMap.h"
#include "../AIController/Actor.h"

void InfluenceMap::update(double time){
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