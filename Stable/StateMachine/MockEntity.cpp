#include "FSM.h"
#include "states.h"

class MockEntity {

public:
	MockEntity(char* n){
		m_name = n;
		m_pFSM = new FSM<MockEntity>(new MockState1<MockEntity>, this);
	
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