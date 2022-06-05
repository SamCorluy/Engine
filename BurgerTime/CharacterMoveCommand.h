#pragma once
#include "BaseCommand.h"
#include "MovementComponent.h"
#include <memory>
class CharacterMoveCommand final : public Engine::BaseCommand
{
public:
	CharacterMoveCommand(const std::weak_ptr<MovementComponent>& target, const Direction dir);
	~CharacterMoveCommand() override = default;

	void Execute() override;
private:
	std::weak_ptr<MovementComponent> m_pTarget;
	Direction m_Direction;
};

