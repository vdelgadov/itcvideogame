#ifndef OBJECT_DEF
#define OBJECT_DEF
#include <vector>
#include <list>
#include "Physics.h"
#include "Engine.h"
#include "Vehicle.h"
#include "Vertex.h"
#include <iostream>
using namespace std;

class CObject
{
public:
	//ID
	int ID;
	Vehicle vehicle;
	D3DXMATRIX translation, rotation, scale;
	CEngine* engine;
	CObject* pParent;
	

	//Relative position
	double fPosX;
	double fPosY;
	double fPosZ;

	//Child list and parent
	list<CObject*> lstChilds;
	
public:	
	CObject()
	{
		//id
		this->ID = 0;
		this->vehicle = Vehicle(1,Vector3D(0.0,0.0,0.0));
		D3DXMatrixTranslation(&translation,0.0,0.0,0.0);
		D3DXMatrixRotationYawPitchRoll(&rotation, 0.0, 0.0, 0.0);
		D3DXMatrixScaling(&scale, 1.0, 1.0, 1.0);
		this->pParent = NULL;
		this->engine = engine;
		this->fPosX = 0.0;
		this->fPosY = 0.0;
		this->fPosZ = 0.0;

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
	void move(float tX, float tY, float tZ, float rX, float rY, float rZ, float scale)
	{
		D3DXMATRIX translationTemp, rotationTemp, scaleTemp;
		D3DXMatrixTranslation(&translationTemp,tX,tY,tZ);
		D3DXMatrixRotationYawPitchRoll(&rotationTemp, rX, rY, rZ);
		D3DXMatrixScaling(&scaleTemp, scale, scale, scale);
		
		//if(!Physics::checkBoundingSphere(this)) // if no intersections change pos
		//{
			this->translation = this->translation*translationTemp;
			//Update world and relative coordinates
			Vector3D pos = Vector3D(this->fPosX,this->fPosY,this->fPosZ);
			pos+=this->vehicle.getPos();
			this->vehicle.setPos( pos  );
			this->fPosX += tX;
			this->fPosY += tY;
			this->fPosZ += tZ;
			

			this->rotation = this->rotation*rotationTemp;
			this->scale = this->scale*rotationTemp;
			for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
				CObject* o = *it;
				o->moveNoChecks( tX,  tY,  tZ,  rX,  rY,  rZ,  scale);
			}
		//}
	}
	void moveNoChecks(float tX, float tY, float tZ, float rX, float rY, float rZ, float scale)
	{
		D3DXMATRIX translationTemp, rotationTemp, scaleTemp;
		
		D3DXMatrixTranslation(&translationTemp,tX,tY,tZ);
		D3DXMatrixRotationYawPitchRoll(&rotationTemp, rX, rY, rZ);
		D3DXMatrixScaling(&scaleTemp, scale, scale, scale);
		
		
		this->translation = this->translation*translationTemp;
		//Update world and relative coordinates
		Vector3D pos = Vector3D(this->fPosX,this->fPosY,this->fPosZ);
		pos+=this->vehicle.getPos();
		this->vehicle.setPos( pos  );
		this->fPosX += tX;
		this->fPosY += tY;
		this->fPosZ += tZ;
		

		this->rotation = this->rotation*rotationTemp;
		this->scale = this->scale*rotationTemp;
		for(list<CObject*>::iterator it = lstChilds.begin(); it != lstChilds.end(); ++it) {
			CObject* o = *it;
			o->moveNoChecks( tX,  tY,  tZ,  rX,  rY,  rZ,  scale);
		}
		
	}
	virtual void render()
	{
		return;
	}

};
#endif OBJECT_DEF