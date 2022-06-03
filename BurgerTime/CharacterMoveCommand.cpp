#include "CharacterMoveCommand.h"

CharacterMoveCommand::CharacterMoveCommand(const std::weak_ptr<MovementComponent>& target, const Direction dir)
	:BaseCommand()
	, m_pTarget{target}
	, m_Direction{ dir }
{
}

void CharacterMoveCommand::Execute()
{
	if(!m_pTarget.expired())
		m_pTarget.lock()->Move(m_Direction);
}
