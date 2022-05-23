#include "CharacterMoveCommand.h"

CharacterMoveCommand::CharacterMoveCommand(const std::weak_ptr<PeterPepperComponent>& target, const Action action)
	:BaseCommand()
	, m_pTarget{target}
	, m_Action{ action }
{
}

void CharacterMoveCommand::Execute()
{
	m_pTarget.lock()->Move(m_Action);
}
