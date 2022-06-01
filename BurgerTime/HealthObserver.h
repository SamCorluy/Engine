#pragma once
#include "Observer.h"
#include "HealthComponent.h"
class HealthObserver final: public dae::Observer
{
public:
	HealthObserver(const std::weak_ptr<HealthComponent>& healthComp);

	void Notify(dae::Event event, const std::weak_ptr<dae::GameObject>& gameObject) override;
	void Notify(dae::Event event, int data) override;
private:
	std::weak_ptr<HealthComponent> m_pHealthComponent;
};

