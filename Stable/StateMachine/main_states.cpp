#include <iostream>
#include <windows.h>
#include "MockEntity.cpp"
#include "ScriptedState.h"

using namespace std;
int main(){
	MockEntity* actor = new MockEntity("Actor 1");
	
	while(1){
		Sleep(500);
		actor->update();

	   //actor->getFSM()->update();
	}
}