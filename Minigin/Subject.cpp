#include "MiniginPCH.h"
#include "Subject.h"

dae::Subject::Subject(const std::shared_ptr<GameObject>& owner)
	: BaseComponent(owner)
{
}

void dae::Subject::AddObserver(std::shared_ptr<Observer> observer)
{
	observers.push_back(observer);
}

void dae::Subject::RemoveObserver(std::shared_ptr<Observer> observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void dae::Subject::StaticUpdate()
{
}

void dae::Subject::Update()
{
}

void dae::Subject::Render(const Transform&) const
{
}

void dae::Subject::Notify(Event event, int data)
{
	for (int i = 0; i < static_cast<int>(observers.size()); ++i)
		observers[i]->Notify(event, data);
}
