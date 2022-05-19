#pragma once
#include "Observer.h"
#define MAX_OBSERVERS 10

namespace dae
{
	class Subject
	{
	public:
		Subject();
		void AddObserver(Observer* observer)
		{
			if (numbObservers < MAX_OBSERVERS)
			{
				observers[numbObservers] = observer;
				++numbObservers;
			}
		}
		void RemoveObserver(Observer* observer)
		{
			for (int i = 0; i < numbObservers; i++)
			{
				if (observers[i] == observer)
				{
					delete observers[i];
					observers[i] = observers[MAX_OBSERVERS - 1];
					observers[MAX_OBSERVERS - 1] = nullptr;
					--numbObservers;
					break;
				}
			}
		}
		virtual void Update(float elapsedSec) = 0;
	protected:
		void Notify(Event event)
		{
			for (int i = 0; i < numbObservers; ++i)
				observers[i]->Notify(event);
		}
	private:
		Observer* observers[MAX_OBSERVERS];
		int numbObservers;
	};
}

