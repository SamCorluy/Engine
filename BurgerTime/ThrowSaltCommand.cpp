#include "ThrowSaltCommand.h"

ThrowSaltCommand::ThrowSaltCommand(const std::weak_ptr<PeterPepperComponent>& target)
	:BaseCommand()
	, m_pTarget{ target }
{
}

void ThrowSaltCommand::Execute()
{
	if (!m_pTarget.expired())
		m_pTarget.lock()->ThrowSalt();
}
