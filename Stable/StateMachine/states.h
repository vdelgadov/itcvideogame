#ifndef _STATE_H
#define _STATE_H

template <class T>
class AState{
public:
		virtual void enter(T*) = 0;
		virtual void execute(T*) = 0;
		virtual void exit(T*) = 0;

};
#endif