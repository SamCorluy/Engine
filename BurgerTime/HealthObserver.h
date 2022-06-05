#pragma once
#include "Observer.h"
#include "HealthComponent.h"
class HealthObserver final: public Engine::Observer
{
public:
	HealthObserver(const std::weak_ptr<HealthComponent>& healthComp);

	void Notify(Engine::Event event, int data) override;
private:
	std::weak_ptr<HealthComponent> m_pHealthComponent;
};

