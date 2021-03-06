#pragma once
#include "BaseComponent.h"

namespace Engine
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

		virtual void Notify(Event event, int data) = 0;
	};
}
