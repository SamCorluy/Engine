#pragma once
#include "BaseCommand.h"
#include "MenuComponent.h"
enum class MenuInteraction
{
	SELECT,
	MOVEUP,
	MOVEDOWN
};
class MenuCommand final : public BaseCommand
{
public:
	MenuCommand(const std::weak_ptr<MenuComponent>& target, MenuInteraction interaction);
	~MenuCommand() override = default;

	void Execute() override;
private:
	std::weak_ptr<MenuComponent> m_pTarget;
	MenuInteraction m_Interaction;
};

