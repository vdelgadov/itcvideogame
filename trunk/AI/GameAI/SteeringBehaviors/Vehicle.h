#ifndef _VEHICLE_H
#define _VEHICLE_H
#include "Vector2D.h"
class Vehicle{

public:
	Vehicle(double max_sp, Vector2D pos){
		this->m_dMaxSpeed = max_sp;
		this->m_vVel = Vector2D();
		this->m_vPos = pos;
		
	}
	Vehicle() {
	}

	const Vector2D &getCurrVel() const{
		return m_vVel;
	}

	void setCurrVel(const Vector2D &new_vel){
		this->m_vVel = new_vel;
	}

	 const Vector2D &getPos() const {
		return this->m_vPos;
	}

	void setPos(const Vector2D &new_pos){
		this->m_vPos = new_pos;
	}


	double getMaxSpeed(){
		return m_dMaxSpeed;
	}

	void setMaxSpeed(double sp){
		this->m_dMaxSpeed = sp;
	
	}

	 Vector2D getHeading(){
		Vector2D heading = this->m_vVel;
		heading.normalize();
		return heading;
	}

	 void update(double time) {
		 m_vPos += this->getCurrVel() * time;
	 }


private:
	double m_dMaxSpeed;
	Vector2D m_vVel;
	Vector2D m_vPos;

	

};
#endif