#ifndef FSM_H
#define FSM_H
#include "states.h"

#pragma once
template <class T>
class FSM {

	private:
		AState<T>* m_pCurrState;
		AState<T>* m_pLastState;
		T* m_pOwner;
		double m_dTime;

	public:
		FSM(T *owner){
			m_pOwner = owner;
			m_pCurrState = NULL;
			m_pLastState = NULL;
			m_dTime = 0;
		}
		FSM(AState<T>* start, T* owner){
			m_pCurrState = start;
			m_pLastState = NULL;
			m_pOwner = owner;
			m_pCurrState->enter(m_pOwner);
		}



		double getTime(){
			return this->m_dTime;
		}

	void setStart(AState<T>* ss){
		m_pCurrState = ss;
		m_pCurrState->enter(m_pOwner);

	}	
	void changeState(AState<T>* ns){
		m_pCurrState->exit(m_pOwner);
		
		ns->enter(m_pOwner);
		delete m_pLastState;


		m_pLastState = m_pCurrState;
		m_pCurrState = ns;
	}

	void update(double time=1){
		this->m_dTime = time;
		m_pCurrState->execute(m_pOwner);
	}

	void revertToLast(){
		this->changeState(this->m_pLastState);
	}

	~FSM(){
		delete m_pCurrState;
		if(m_pLastState)
			delete m_pLastState;
	
	}
};
#endif