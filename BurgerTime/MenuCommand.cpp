#include "MenuCommand.h"

MenuCommand::MenuCommand(const std::weak_ptr<MenuComponent>& target, MenuInteraction interaction)
	: BaseCommand()
	, m_pTarget{ target }
	, m_Interaction{interaction}
{
}

void MenuCommand::Execute()
{
	switch (m_Interaction)
	{
	case MenuInteraction::SELECT:
		m_pTarget.lock()->SelectGameMode();
		break;
	case MenuInteraction::MOVEUP:
		m_pTarget.lock()->MoveUp();
		break;
	case MenuInteraction::MOVEDOWN:
		m_pTarget.lock()->MoveDown();
		break;
	default:
		break;
	}
}
