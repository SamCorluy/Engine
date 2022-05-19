#include "MiniginPCH.h"
#include "PointsObserver.h"

dae::PointsObserver::PointsObserver(TextComponent* textComp)
	: m_Points(0)
	, m_TextComp(textComp)
{ }

void dae::PointsObserver::Notify(Event event)
{
	if (event == Event::BURGER_DROP)
	{
		m_Points += 25;
		m_TextComp->SetText("Points: " + std::to_string(m_Points));
	}
	if (event == Event::ENEMY_KILL)
	{
		m_Points += 50;
		m_TextComp->SetText("Points: " + std::to_string(m_Points));
	}
}
