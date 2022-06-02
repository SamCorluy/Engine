#pragma once
#include "BaseComponent.h"

namespace dae
{
	enum class Event
	{
		CHARACTER_DEAD,
		SCORE_CHANGE,
		SALT_THROW
	};

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
		virtual void Notify(Event event, int data) = 0;
	};
}
