#include "HealthObserver.h"
#include "GameObject.h"

HealthObserver::HealthObserver(const std::weak_ptr<HealthComponent>& healthComp)
	: Observer()
	, m_pHealthComponent(healthComp)
{
}

void HealthObserver::Notify(Engine::Event event, int lives)
{
	if (event == Engine::Event::CHARACTER_DEAD && lives >= 0)
	{
		m_pHealthComponent.lock()->RemoveLife();
	}
}
