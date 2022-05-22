#include "MiniginPCH.h"
#include "Subject.h"

dae::Subject::Subject(const std::shared_ptr<GameObject>& owner)
	: numbObservers(0)
	, BaseComponent(owner)
{
}

void dae::Subject::AddObserver(Observer* observer)
{
	if (numbObservers < MAX_OBSERVERS)
	{
		observers[numbObservers] = observer;
		++numbObservers;
	}
}

void dae::Subject::RemoveObserver(Observer* observer)
{
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
}

void dae::Subject::StaticUpdate(float)
{
}

void dae::Subject::Update(float)
{
}

void dae::Subject::Render(const Transform&) const
{
}

void dae::Subject::Notify(Event event, const std::weak_ptr<GameObject>& gameObject)
{
	for (int i = 0; i < numbObservers; ++i)
		observers[i]->Notify(event, gameObject);
}

void dae::Subject::Notify(Event event, int data)
{
	for (int i = 0; i < numbObservers; ++i)
		observers[i]->Notify(event, data);
}
