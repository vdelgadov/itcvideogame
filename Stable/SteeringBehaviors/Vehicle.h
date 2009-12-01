#ifndef _VEHICLE_H
#define _VEHICLE_H
#include "../Main/Engine.h"

//bool networkMove(int objectID, float x, float y, float z, LPVOID param); 
void broadcastPosition(int iID, float x, float y, float z);

template <class V>
class Vehicle{

public:

	Vehicle<V>(double max_sp, V pos){
		this->m_dMaxSpeed = max_sp;
		this->m_vVel = V();
		this->m_vPos = pos;
	
		
	
	}

	void setId(int id){
		this->m_iID = id;
	}

	Vehicle<V>() {
		this->m_dMaxSpeed = 0;
		this->m_vVel = V();
		this->m_vHeading = V();
		this->m_vPos = V();
	}

	const V &getCurrVel() const{
		return m_vVel;
	}

	void setCurrVel(const V &new_vel){
		this->m_vVel = new_vel;
	}

	 const V &getPos() const {
		return this->m_vPos;
	}

	void setPos(const V &new_pos){
		this->m_vPos = new_pos;
	}

	void setHeading(const V &new_head){
		this->m_vHeading = new_head;
	
	}
	double getMaxSpeed(){
		return this->m_dMaxSpeed;
	}

	void setMaxSpeed(double sp){
		this->m_dMaxSpeed = sp;
	
	}

	 V getHeading(){
		V heading = this->m_vVel;
		heading.normalize();
		return heading;
		// return this->m_vHeading;
	}

	 void update(double time) {
		if(SERVER)
		{
			cout << this->getCurrVel().x <<" "<<this->getCurrVel().y<<" " << this->getCurrVel().z << endl;
			m_vPos += this->getCurrVel();// * time;

			//networkMove(m_iID, this->getCurrVel().x, this->getCurrVel().y, this->getCurrVel().z, NULL); 
			broadcastPosition(m_iID, this->getCurrVel().x, this->getCurrVel().y, this->getCurrVel().z);
		}
		
		
	 }


private:
	double m_dMaxSpeed;
	V m_vVel;
	V m_vPos;
	V m_vHeading;
	int m_iID;
	

};
#endif