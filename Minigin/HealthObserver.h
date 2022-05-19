#pragma once
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
	class HealthObserver : public Observer
	{
	public:
		HealthObserver(int lives, TextComponent* textComp);
		virtual ~HealthObserver() = default;
		void Notify(Event event) override;
	private:
		int m_Lives;
		TextComponent* m_TextComp;
	};
}

