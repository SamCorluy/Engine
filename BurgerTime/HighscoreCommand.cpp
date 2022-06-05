#include "HighscoreCommand.h"

HighscoreCommand::HighscoreCommand(const std::weak_ptr<HighScoreMenuComponent>& target)
	: BaseCommand()
	, m_pTarget{target}
{
}

void HighscoreCommand::Execute()
{
	m_pTarget.lock()->BackToMenu();
}
