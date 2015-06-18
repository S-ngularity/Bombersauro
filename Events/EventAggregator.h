#ifndef EVENTAGGREGATOR
#define EVENTAGGREGATOR

#include <list>

#include "Events/Event.h"

class EventAggregator
{
	private:
		static EventAggregator *evtAggrInstance;

		EventAggregator(){evtAggrInstance = nullptr;} // private so that it can't be called
		EventAggregator(EventAggregator const&) = delete;
		EventAggregator& operator=(EventAggregator const&) = delete;

	public:
		static EventAggregator* Instance() { if (evtAggrInstance == nullptr) evtAggrInstance = new EventAggregator; return evtAggrInstance;}

		template<typename EventType>
		Event<EventType>& getEvent()
		{
			static Event<EventType> evt;

			return evt;
		}
};

#endif