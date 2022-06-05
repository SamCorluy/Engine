#pragma once
#include "Observer.h"
#include "CounterComponent.h"
class PointsObserver final : public Engine::Observer
{
public:
	PointsObserver(const std::weak_ptr<Engine::CounterComponent>& counterComp);

	void Notify(Engine::Event event, int data) override;
private:
	std::weak_ptr<Engine::CounterComponent> m_pCounterComponent;
};

