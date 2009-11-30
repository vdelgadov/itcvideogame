#pragma once




#include "Actor_States.cpp"
InfluenceMap* AIController::s_InfluenceMap;
#include "Brute.cpp"
#include "Artillery.cpp"
#include "Skirk.cpp"
#include "../AIController/AIController.h"


class RoleFactory{
	
public:
	enum state_t {ATTACK, ENGAGE};
	static AState<Actor>* CreateRoleState(AIController::role_t r, state_t s ){
		switch(r){
			case AIController::BRUTE:
				return CreateBruteState(s);
			case AIController::ARTILLERY:
				return CreateArtState(s);
			case AIController::SKIRKMISHER:
				return CreateSkirkState(s);
		
		}
	
	}

	static AState<Actor>* CreateBruteState(state_t s){
		
		if(s == ATTACK)
			return new BruteAttack();
		return new BruteEngaging();
	
	}
	
	static AState<Actor>* CreateArtState(state_t s){
		
		if(s == ATTACK)
			return new ArtAttack();
		return new ArtEngaging();
	
	}

	static AState<Actor>* CreateSkirkState(state_t s){
		
		if(s == ATTACK)
			return new SkirkAttack();
		return new SkirkEngaging();
	
	}

};