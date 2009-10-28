#ifndef FSM_H
#define FSM_H
#include "ScriptedState.h"
#include <map>

struct ltstr
{
  bool operator()(const string s1, const string s2) const
  {
	  return  s1.compare(s2) < 0;
	  
  }
};



template <class T>
class FSM {

private:
		AState<T>* m_pCurrState;
		AState<T>* m_pLastState;
		T* m_pOwner;
		double m_dTime;
		map<string, ScriptedState<T>, ltstr> m_mStates;


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
	FSM(map<string, ScriptedState<T>, ltstr> states, string start, T* owner){
		m_mStates = states;		
		m_pCurrState = &(this->m_mStates[start]);
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

	void changeState(string name){
		this->changeState(&(this->m_mStates[name]));
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