#pragma once
#include "BaseCommand.h"
#include "MenuComponent.h"
class MenuCommand final : public BaseCommand
{
public:
	MenuCommand(const std::weak_ptr<MenuComponent>& target, GameModes gamemode);
	~MenuCommand() override = default;

	void Execute() override;
private:
	std::weak_ptr<MenuComponent> m_pTarget;
	GameModes m_GameMode;
};

