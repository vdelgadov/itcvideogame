#include "states.h"

#pragma once
template <class T>
class FSM {

	private:
		AState<T>* m_pCurrState;
		AState<T>* m_pLastState;
		T* m_pOwner;

	public:
	FSM(AState<T>* start, T* owner){
		m_pCurrState = start;
		m_pLastState = NULL;
		m_pOwner = owner;
		m_pCurrState->enter(m_pOwner);
	}

	void FSM<T>::changeState(AState<T>* ns){
		m_pCurrState->exit(m_pOwner);
		
		ns->enter(m_pOwner);
		delete m_pLastState;


		m_pLastState = m_pCurrState;
		m_pCurrState = ns;
	}

	void FSM<T>::update(){
		m_pCurrState->execute(m_pOwner);
	}

	void FSM<T>::revertToLast(){
		this->FSM<T>::changeState(this->m_pLastState);
	}

	FSM<T>::~FSM(){
		delete m_pCurrState;
		if(m_pLastState)
			delete m_pLastState;
	
	}
};