#pragma once
#pragma once
#include "Observer.h"
#include "TextComponent.h"

namespace dae
{
	class PointsObserver : public Observer
	{
	public:
		PointsObserver(TextComponent* textComp);
		virtual ~PointsObserver() = default;
		void Notify(Event event) override;
	private:
		int m_Points;
		TextComponent* m_TextComp;
	};
}

