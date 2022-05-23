#pragma once
#include "BaseCommand.h"
#include "PeterPepperComponent.h"
#include <memory>
class CharacterMoveCommand final : public BaseCommand
{
public:
	CharacterMoveCommand(const std::weak_ptr<PeterPepperComponent>& target, const Action action);
	~CharacterMoveCommand() override = default;

	void Execute() override;
private:
	std::weak_ptr<PeterPepperComponent> m_pTarget;
	Action m_Action;
};

