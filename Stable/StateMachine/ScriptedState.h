#ifndef _SCRIPTED_STATE_H
#define _SCRIPTED_STATE_H
#include <string.h>
#include "states.h"

using namespace std;
template <class T>
class ScriptedState : public AState<T>{
private:
	string m_sName;
	int m_iScriptbegins, m_iScriptends, m_iCurrent;
public:
	ScriptedState(){
	
	}
	ScriptedState(string name, int beg, int end){
		m_iCurrent = m_iScriptbegins = beg;
		m_iScriptends = end;

		m_sName = name;
	}

	void execute(T* owner){
		
		if(m_iCurrent > m_iScriptends)
			return;
		/* Somtehing happens */
		/* VM Communication I guess*/
		owner->getController()->getVM()->execute(m_iCurrent);
		m_iCurrent++;
	
	}

	int getScriptBegin() const{
		return this->m_iScriptbegins;
	}
	void enter(T* owner){
		this->m_iCurrent = this->m_iScriptbegins+1;
	}
	void exit(T* owner){
	
	}



};
#endif