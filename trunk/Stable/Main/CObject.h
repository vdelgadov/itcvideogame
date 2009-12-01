#ifndef OBJECT_DEF
#define OBJECT_DEF
#include <vector>
#include <list>
#include "Vector3D.h"
#include "Physics.h"
#include "Engine.h"
//#include "Params.h"
#include "Vertex.h"
#include <iostream>
#include "../SteeringBehaviors/Vehicle.h"

using namespace std;

class CObject
{
private:
	Vehicle<Vector3D>* vehicle;
public:
	//ID
	int ID;
	
	D3DXMATRIX translation, rotation, scale;
	CEngine* engine;
	CObject* pParent;
	CObject* pScene;
	

	//Relative position
	double fPosX;
	double fPosY;
	double fPosZ;

	//Child list and parent
	list<CObject*> lstChilds;

	//Bounding Sphere needed by others
	D3DXVECTOR3 Center;
	float Radius;
	bool boundingSphere;
	bool isRendereable;

public:	
	CObject()
	{
		//id
		this->ID = 0;
		this->vehicle = new Vehicle<Vector3D>(1,Vector3D(0.0,0.0,0.0));
		D3DXMatrixTranslation(&translation,0.0,0.0,0.0);
		D3DXMatrixRotationYawPitchRoll(&rotation, 0.0, 0.0, 0.0);
		D3DXMatrixScaling(&scale, 1.0, 1.0, 1.0);
		this->pParent = NULL;
		this->fPosX = 0.0;
		this->fPosY = 0.0;
		this->fPosZ = 0.0;
		this->boundingSphere = false;

		//parent
		this->pParent = NULL;
	}
	//quitar
	
	~CObject() {
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			delete *it;
		}
	}
	void AddChild(CObject* pNode)
	{
		lstChilds.push_back(pNode);
		pNode->pParent = this;
		pNode->translation = pNode->pParent->translation * pNode->translation;
		pNode->rotation = pNode->pParent->rotation * pNode->rotation;
		pNode->scale = pNode->pParent->scale * pNode->scale;
		
	}
	CObject* find(int id)
	{
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			CObject* o = *it;
			if( o->ID == id)
			{
				return o;
			}
			else
			{
				CObject* o = (*it)->find(id);
				if (o)
				{
					return o;
				}
			}
		}
		return NULL;
	}

	CObject* findClosestTo(const Vector3D& point, int t);
	void move(Vector3D& deltaMove)
	{
		this->getVehicle()->setCurrVel(deltaMove);
		if(!Physics::checkBoundingSphereCollision(this)) // if no intersections change pos
		{
			this->vehicle->setPos(this->vehicle->getPos() + deltaMove);
			for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it)
			{
				CObject* o = *it;
				o->move( deltaMove);
			}
		}
	}
	void setPosition(Vector3D deltaMove)
	{
		this->getVehicle()->setCurrVel(Vector3D(0,0,0));
		this->vehicle->setPos(deltaMove);
	}
	void moveOld(float tX, float tY, float tZ, float rX, float rY, float rZ, float scale)
	{
		
		//D3DXMATRIX translationTemp, rotationTemp, scaleTemp;
		//D3DXMatrixTranslation(&translationTemp,this->vehicle->getPos().x,this->vehicle->getPos().y,this->vehicle->getPos().z);
		//D3DXMatrixRotationYawPitchRoll(&rotationTemp, rX, rY, rZ);
		//D3DXMatrixScaling(&scaleTemp, scale, scale, scale);
		//if(!Physics::checkBoundingSphere(this)) // if no intersections change pos
		//{
			//this->translation = this->translation*translationTemp;
			//Update world and relative coordinates
			Vector3D pos = Vector3D(this->fPosX,this->fPosY,this->fPosZ);
			pos+=this->vehicle->getPos();
			this->vehicle->setPos( pos  );
			this->fPosX += tX;
			this->fPosY += tY;
			this->fPosZ += tZ;
			

			//this->rotation = this->rotation*rotationTemp;
			//this->scale = this->scale*rotationTemp;
			for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
				CObject* o = *it;
				//o->moveNoChecks( tX,  tY,  tZ,  rX,  rY,  rZ,  scale);
			}
		//}
	}
	void moveNoChecks(Vector3D deltaMove)
	{
		this->getVehicle()->setCurrVel(deltaMove);
		this->vehicle->setPos(this->vehicle->getPos() + deltaMove);
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it)
		{
			CObject* o = *it;
			o->moveNoChecks( deltaMove);
		}
		
	}
	virtual void render()
	{
		return;
	}
	virtual Vehicle<Vector3D>* getVehicle(){
		return this->vehicle;
	}
	virtual void update(double time=0){
		//cout << "updating cobject" << endl;
	}

};
#endif OBJECT_DEF