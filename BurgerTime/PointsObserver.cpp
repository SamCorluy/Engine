#include "PointsObserver.h"

PointsObserver::PointsObserver(const std::weak_ptr<CounterComponent>& counterComp)
	: Observer()
	, m_pCounterComponent{ counterComp }
{
}

void PointsObserver::Notify(dae::Event, const std::weak_ptr<dae::GameObject>&)
{
}

void PointsObserver::Notify(dae::Event event, int data)
{
	if (event == dae::Event::SCORE_CHANGE)
	{
		m_Points += data;
		m_pCounterComponent.lock()->SetCounter(m_Points);
	}
}
