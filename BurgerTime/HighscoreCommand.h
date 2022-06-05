#pragma once
#include "BaseCommand.h"
#include "HighScoreMenuComponent.h"
class HighscoreCommand : public BaseCommand
{
public:
	HighscoreCommand(const std::weak_ptr<HighScoreMenuComponent>& target);
	~HighscoreCommand() override = default;

	void Execute() override;
private:
	std::weak_ptr<HighScoreMenuComponent> m_pTarget;
};

