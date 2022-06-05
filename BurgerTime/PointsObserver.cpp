#include "PointsObserver.h"

PointsObserver::PointsObserver(const std::weak_ptr<Engine::CounterComponent>& counterComp)
	: Observer()
	, m_pCounterComponent{ counterComp }
{
}

void PointsObserver::Notify(Engine::Event event, int data)
{
	if (event == Engine::Event::SCORE_CHANGE)
	{
		m_pCounterComponent.lock()->SetCounter(data);
	}
}
