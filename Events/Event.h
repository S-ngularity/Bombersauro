#ifndef EVENT
#define EVENT

#include <list>
#include <functional>
#include <iostream>
template<typename EventType>
class Event
{
	private:
		// to do: substituir por map, gerar uma key e retornar a key ao fazer subscribe(), o subscriber guarda essa key e chama remove(key) quando precusar
		std::list< std::function<void(EventType &evt)> > subscriberList;

	public:
		void subscribe(std::function<void(EventType &evt)> action)
		{
			subscriberList.push_front(action);
		}

		void unsubscribe(std::function<void(EventType &evt)> action)
		{
			//subscriberList.remove(action);
		}

		void publishEvent(EventType evt)
		{
			for(auto action : subscriberList)
				action(evt);
		}
};

#endif