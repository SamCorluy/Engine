#include "MiniginPCH.h"
#include "HealthObserver.h"

dae::HealthObserver::HealthObserver(int lives, TextComponent* textComp)
	:m_Lives(lives)
	,m_TextComp(textComp)
{ }

void dae::HealthObserver::Notify(Event event)
{
	if (event == Event::CHARACTER_DEAD)
	{
		--m_Lives;
		m_TextComp->SetText("Levens: " + std::to_string(m_Lives));
	}
}
