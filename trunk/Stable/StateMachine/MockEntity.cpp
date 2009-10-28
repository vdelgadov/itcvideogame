#include "FSM.h"
#include "states.h"
#include "ScriptedState.h"
#include <map>





class MockEntity {

public:
	MockEntity(char* n){
		m_name = n;
		std::map<string, ScriptedState<MockEntity>, ltstr> m;
		m["state 1"] = *(new ScriptedState<MockEntity>("state 1", 0, 5));
		m["state 2"] = *(new ScriptedState<MockEntity>("state 2", 6, 10));
		m_pFSM = new FSM<MockEntity>(m, "state 1", this);	
	}
	FSM<MockEntity>* getFSM(){
	
		return m_pFSM;
	
	}
	char* getName(){
		return m_name;
	}

	void update(){
		m_pFSM->update();
	}

	~MockEntity(){
		delete m_pFSM;
	}

private:
	char* m_name;
	FSM<MockEntity>* m_pFSM;

};