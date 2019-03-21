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

class Event
{
protected:
	virtual ~Event() {};
};

struct InputEvent : public Event {
	InputEvent(std::string str): STR {str} {}
	std::string STR;
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
class MemberFunctionHandler : public HandlerFunctionBase
{
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
			handlers = new HandlerList();
			subscribers[typeid(EventType)] = handlers;
		}

		handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
	}
private:
	std::map<std::type_index, HandlerList*> subscribers;
};



