#include "SaltObserver.h"
#include "GameObject.h"

SaltObserver::SaltObserver(const std::weak_ptr<SaltDisplayComponent>& saltComp)
	: Observer()
	, m_pSaltComponent(saltComp)
{
}

void SaltObserver::Notify(Engine::Event event, int data)
{
	if (event == Engine::Event::SALT_THROW && data >= 0)
	{
		m_pSaltComponent.lock()->RemoveCharge();
	}
}
