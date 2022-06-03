#include "MenuCommand.h"

MenuCommand::MenuCommand(const std::weak_ptr<MenuComponent>& target, GameModes gamemode)
	:BaseCommand()
	, m_pTarget{ target }
	, m_GameMode{ gamemode }
{
}

void MenuCommand::Execute()
{
	m_pTarget.lock()->SelectGameMode(m_GameMode);
}
