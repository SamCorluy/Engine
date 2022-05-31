#include "ThrowSaltCommand.h"

ThrowSaltCommand::ThrowSaltCommand(const std::weak_ptr<PeterPepperComponent>& target)
	:BaseCommand()
	, m_pTarget{ target }
{
}

void ThrowSaltCommand::Execute()
{
	m_pTarget.lock()->ThrowSalt();
}
