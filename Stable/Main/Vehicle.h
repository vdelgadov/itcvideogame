#ifndef _VEHICLE_H
#define _VEHICLE_H
#include "Vector3D.h"
#include "Netwroking.h"
class Vehicle{

public:
	Vehicle(double max_sp, Vector3D pos){
		this->m_dMaxSpeed = max_sp;
		this->m_vVel = Vector3D();
		this->m_vPos = pos;
		
	}
	Vehicle() {
		this->m_dMaxSpeed = 0;
		this->m_vVel = Vector3D();
		this->m_vHeading = Vector3D();
		this->m_vPos = Vector3D();
	}

	const Vector3D &getCurrVel() const{
		return m_vVel;
	}

	void setCurrVel(const Vector3D &new_vel){
		this->m_vVel = new_vel;
	}

	const Vector3D &getPos() const {
		return this->m_vPos;
	}
	Vector3D getPos(){
		return this->m_vPos;
	}

	void setPos(const Vector3D &new_pos){
		this->m_vPos = new_pos;
	}


	void setHeading(const Vector3D &new_head){
		this->m_vHeading = new_head;
	
	}
	double getMaxSpeed(){
		return m_dMaxSpeed;
	}

	void setMaxSpeed(double sp){
		this->m_dMaxSpeed = sp;
	
	}

	 Vector3D getHeading(){
		/*Vector3D heading = this->m_vVel;
		heading.normalize();
		return heading;*/
		 return this->m_vHeading;
	}

	 void update(double time) {
		 m_vPos += this->getCurrVel();// * time;
	 }


private:
	double m_dMaxSpeed;
	Vector3D m_vVel;
	Vector3D m_vPos;
	Vector3D m_vHeading;

	

};
#endif