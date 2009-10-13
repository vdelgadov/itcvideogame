#include <iostream>
#include <time.h>

using namespace std;
#pragma once
template <class T>

class AState{
public:
		virtual void enter(T*) = 0;
		virtual void execute(T*) = 0;
		virtual void exit(T*) = 0;

private:

};

template <class T>
class MockState1 : public AState<T>{
public:
	MockState1();
	void enter(T*);
	void execute(T*);
	void exit(T*);
	~MockState1();
private:
	 int m_counter;

};

template <class T>
class MockState2 : public AState<T>{
public:
	MockState2();
	void enter(T*);
	void execute(T*);
	void exit(T*);
	~MockState2();
private:
	int m_counter;

};


template <class T>
class MockState3 : public AState<T>{
public:
	MockState3();
	void enter(T*);
	void execute(T*);
	void exit(T*);
	~MockState3();


};


template <class T> MockState1<T>::MockState1(){
	
		m_counter = 0;
	}

template <class T>	void MockState1<T>::enter(T* actor){
		cout << actor->getName() << " is entering Mock State 1 \n" << endl;
		m_counter = 0;
	}


template <class T>	void MockState1<T>::execute(T* actor){
	
		cout << "Executing Mock State 1 counter now is " << m_counter << " \n" << endl;
		m_counter++;
		if(m_counter > 5){
			actor->getFSM()->changeState(new MockState2<T>());
		}
	}

template <class T>	void MockState1<T>::exit(T* actor){
		cout << actor->getName() << " is exiting Mock State 1, good bye Mock State 1 :) \n" << endl;
	
	}



    template <class T> MockState2<T>::MockState2(){
	
		m_counter = 0;
	}

	template <class T> void  MockState2<T>::enter(T* actor){
		cout << actor->getName() << " is entering Mock State 2 \n" << endl;
		m_counter = 0;
	}


	template <class T> void  MockState2<T>::execute(T* actor){
	
		cout << "Executing Mock State 2 counter now is " << m_counter << " \n" << endl;
		m_counter++;
		if(m_counter % 2 == 0){
			actor->getFSM()->changeState(new MockState3<T>());
		}
	}

	template <class T> void  MockState2<T>::exit(T* actor){
		cout << actor->getName() << " is exiting Mock State 2, good bye Mock State 2 :) \n" << endl;
	
	}


	
    template <class T> MockState3<T>::MockState3(){
		srand((unsigned)time(0));
		
	}

	template <class T> void  MockState3<T>::enter(T* actor){
		cout << actor->getName() << " is entering Mock State 3 \n" << endl;
		
	}


	template <class T> void  MockState3<T>::execute(T* actor){
		
		cout << "Executing Mock State 3 I have no counter :) \n"<< endl;
		if((int)rand() % 6 == 0){
			actor->getFSM()->changeState(new MockState1<T>());
		}
	}

	template <class T> void  MockState3<T>::exit(T* actor){
		cout << actor->getName() << " is exiting Mock State 3, good bye Mock State 3 :) \n" << endl;
	
	}