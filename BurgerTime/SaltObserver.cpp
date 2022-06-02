#include "SaltObserver.h"
#include "GameObject.h"

SaltObserver::SaltObserver(const std::weak_ptr<SaltDisplayComponent>& saltComp)
	: Observer()
	, m_pSaltComponent(saltComp)
{
}

void SaltObserver::Notify(dae::Event, const std::weak_ptr<dae::GameObject>&)
{
}

void SaltObserver::Notify(dae::Event event, int data)
{
	if (event == dae::Event::SALT_THROW && data >= 0)
	{
		m_pSaltComponent.lock()->RemoveCharge();
	}
}
