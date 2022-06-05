#pragma once
#include "BaseCommand.h"
#include "PeterPepperComponent.h"
#include <memory>
class ThrowSaltCommand final : public Engine::BaseCommand
{
public:
	ThrowSaltCommand(const std::weak_ptr<PeterPepperComponent>& target);
	~ThrowSaltCommand() override = default;

	void Execute() override;
private:
	std::weak_ptr<PeterPepperComponent> m_pTarget;
};