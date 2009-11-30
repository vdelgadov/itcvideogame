#pragma once




#include "Actor_States.cpp"

#include "Brute.cpp"
#include "Artillery.cpp"
#include "Skirk.cpp"
#include "../AIController/AIController.h"


class RoleFactory{
	
public:
	enum state_t {ATTACK, ENGANGE};
	static AState* CreateRoleState(role_t r, state_t s ){
		switch(role_t){
			case BRUTE:
				return CreateBruteState(s);
			case ARTILLERY:
				return CreateArtState(s);
			case SKIRKMISHER:
				return CreateSkirkState(s);
		
		}
	
	}

	AState* CreateBruteState(state_t s){
		
		if(s == ATTACK)
			return new BruteAttack();
		return new BruteEngaging();
	
	}
	
	AState* CreateArtState(state_t s){
		
		if(s == ATTACK)
			return new ArtAttack();
		return new ArtEngaging();
	
	}

	AState* CreateSkirkState(state_t s){
		
		if(s == ATTACK)
			return new SkirkAttack();
		return new SkirkEngaging();
	
	}

};