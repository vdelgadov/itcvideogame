#include "InfluenceMap.h"
#include <iostream>
#include <windows.h>

int main(){
	InfluenceMap imap(10, 10, 50, 50);
	list<Actor*> actors;

	Actor* act = new Actor();
	act->getController()->setCategory(act->getController()->BAD_GUY);
	act->getController()->setInfluenceRadius(2);
	actors.push_back(act);
	
	Actor* act2 = new Actor();
	act2->getVehicle()->setPos(Vector3D(49.0, 49.0));
	act2->getController()->setCategory(act->getController()->GOOD_GUY);
	act2->getController()->setInfluenceRadius(2);
	actors.push_back(act2);
	imap.setActorList(actors);

	string h;
	while(true){
		Sleep(750);
		act->getVehicle()->setPos(act->getVehicle()->getPos() + Vector3D(5.0, 5.0));	
		act2->getVehicle()->setPos(act2->getVehicle()->getPos() + Vector3D(-5.0, -5.0));
		imap.update();
		for(int i = 0; i<10; i++){
			for(int j = 0; j<10; j++)
				cout << imap.getSpot(i, j) << " ";
			cout << endl;
		}
		cout << endl;
		cin >> h;		
	}
	

}