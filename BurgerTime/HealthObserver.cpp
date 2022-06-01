#include "HealthObserver.h"
#include "GameObject.h"
#include "PeterPepperComponent.h"

HealthObserver::HealthObserver(const std::weak_ptr<HealthComponent>& healthComp)
	: Observer()
	, m_pHealthComponent(healthComp)
{
}

void HealthObserver::Notify(dae::Event, const std::weak_ptr<dae::GameObject>&)
{
}

void HealthObserver::Notify(dae::Event event, int lives)
{
	if (event == dae::Event::CHARACTER_DEAD && lives > 0)
	{
		m_pHealthComponent.lock()->RemoveLife();
	}
}
