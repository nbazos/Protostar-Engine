#pragma once
#include <functional>
#include <queue>
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <typeindex>

// EventBus Messaging System Based on Implementation from: https://medium.com/@savas/nomad-game-engine-part-7-the-event-system-45a809ccb68f

// ----------------------------------------------------------------------------------------------------------------------------------------

// Macro for DBG_NEW

// compile by using: cl /EHsc /W4 /D_DEBUG /MDd debug_new.cpp
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

// ----------------------------------------------------------------------------------------------------------------------------------------



class Event {
protected:
	virtual ~Event() {}
};

struct InputEvent : public Event 
{
	InputEvent(std::string str): STR {str} {}
	std::string STR;
	~InputEvent() {}
};

// This is the interface for MemberFunctionHandler that each specialization will use
class HandlerFunctionBase {
public:
	// Call the member function
	void exec(Event * evnt) {
		call(evnt);
	}
private:
	// Implemented by MemberFunctionHandler
	virtual void call(Event * evnt) = 0;
};

template<class T, class EventType>
class MemberFunctionHandler : public HandlerFunctionBase {
public:
	typedef void (T::*MemberFunction)(EventType*);

	MemberFunctionHandler(T * instance, MemberFunction memberFunction) : instance{ instance }, memberFunction{ memberFunction } {};

	void call(Event * evnt) {
		// Cast event to the correct type and call member function
		(instance->*memberFunction)(static_cast<EventType*>(evnt));
	}
private:
	// Pointer to class instance
	T * instance;

	// Pointer to member function
	MemberFunction memberFunction;
};

typedef std::list<HandlerFunctionBase*> HandlerList;
class EventBus {
public:
	EventBus() {}
	~EventBus() 
	{
		for (std::map<std::type_index, HandlerList*>::iterator itr = subscribers.begin(); itr != subscribers.end(); itr++)
		{
			// found it - delete it
			if (typeid(itr->second) == typeid(HandlerList*))
			{
				delete itr->second;

				// _CrtDumpMemoryLeaks();
			}
		}
		
	}

	template<typename EventType>
	void publish(EventType * evnt) {
		HandlerList * handlers = subscribers[typeid(EventType)];

		if (handlers == nullptr) {
			return;
		}

		for (auto & handler : *handlers) {
			if (handler != nullptr) {
				handler->exec(evnt);
			}
		}
	}

	template<class T, class EventType>
	void subscribe(T * instance, void (T::*memberFunction)(EventType *)) {
		HandlerList * handlers = subscribers[typeid(EventType)];

		//First time initialization
		if (handlers == nullptr) {
			handlers = DBG_NEW HandlerList();
			subscribers[typeid(EventType)] = handlers;
		}

		handlers->push_back(DBG_NEW MemberFunctionHandler<T, EventType>(instance, memberFunction));
	}
private:
	std::map<std::type_index, HandlerList*> subscribers;
};



