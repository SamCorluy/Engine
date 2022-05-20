#pragma once
#include "BaseComponent.h"
#include "Event.h"

namespace dae
{
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;

		Observer(const Observer & other) = delete;
		Observer(Observer && other) = delete;
		Observer& operator=(const Observer & other) = delete;
		Observer& operator=(Observer && other) = delete;

		virtual void Notify(Event event, const std::weak_ptr<GameObject>& gameObject) = 0;
	};
}
