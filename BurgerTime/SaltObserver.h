#pragma once
#include "Observer.h"
#include "SaltDisplayComponent.h"
class SaltObserver final : public dae::Observer
{
public:
	SaltObserver(const std::weak_ptr<SaltDisplayComponent>& saltComp);

	void Notify(dae::Event event, int data) override;
private:
	std::weak_ptr<SaltDisplayComponent> m_pSaltComponent;
};

