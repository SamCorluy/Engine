#include "MiniginPCH.h"
#include "Subject.h"

Engine::Subject::Subject(const std::shared_ptr<GameObject>& owner)
	: BaseComponent(owner)
{
}

void Engine::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	observers.push_back(observer);
}

void Engine::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Engine::Subject::StaticUpdate()
{
}

void Engine::Subject::Update()
{
}

void Engine::Subject::Render(const Transform&) const
{
}

void Engine::Subject::Notify(Event event, int data)
{
	for (int i = 0; i < static_cast<int>(observers.size()); ++i)
		observers[i]->Notify(event, data);
}
