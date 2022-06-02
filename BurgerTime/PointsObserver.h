#pragma once
#include "Observer.h"
#include "CounterComponent.h"
class PointsObserver final : public dae::Observer
{
public:
	PointsObserver(const std::weak_ptr<CounterComponent>& counterComp);

	void Notify(dae::Event event, const std::weak_ptr<dae::GameObject>& gameObject) override;
	void Notify(dae::Event event, int data) override;
private:
	std::weak_ptr<CounterComponent> m_pCounterComponent;
};

