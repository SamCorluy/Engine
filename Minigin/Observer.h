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
		virtual void Notify(Event event) = 0;
	};
}
