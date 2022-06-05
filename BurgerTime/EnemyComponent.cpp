#include "EnemyComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include<vector>
#include <iostream>
#include "ElapsedTime.h"
#include "MovementComponent.h"
#include "SoundLocator.h"
EnemyComponent::EnemyComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, std::string textFolder, AnimDurationInit animDurationInit, int points, float ladderChance)
	:BaseComponent(owner)
	, m_pCurrentNode{ node }
	, m_pPrevNode{ node }
	, m_FloorOffset{ floorOffset }
	, m_ReachedChoicePoint{ true }
	, m_RectSize{ 8 * scale, 16 * scale }
	, m_DeathDuration{ animDurationInit.kill}
	, m_StunDuration{animDurationInit.stun * 5.f}
	, m_Stunned{false}
	, m_Dead{false}
	, m_Points{points}
	, m_ElapsedTime{0}
	, m_ChanceToTakeLadder{ladderChance}
	, m_Velocity{75.f}
{
	// Initialize subject
	//owner->AddComponent<dae::Subject>(std::make_shared<dae::Subject>(owner));
	//m_pSubject = owner->GetComponent<dae::Subject>();

	// Handling animation info
	std::vector<dae::AnimationInit> animInitList;
	std::string fileName = textFolder + "/Walk.png";
	animInitList.push_back(dae::AnimationInit(2, animDurationInit.walk, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = textFolder + "/Down.png";
	animInitList.push_back(dae::AnimationInit(2, animDurationInit.down, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = textFolder + "/Up.png";
	animInitList.push_back(dae::AnimationInit(2, animDurationInit.up, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = textFolder + "/Stunned.png";
	animInitList.push_back(dae::AnimationInit(2, animDurationInit.stun, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = textFolder + "/Death.png";
	animInitList.push_back(dae::AnimationInit(4, animDurationInit.kill, fileName, { -m_RectSize.first / 2, 0 }));
	owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList, scale));

	// Handle spawnpoint
	//std::pair<int, int> idx = { 0,0 };
	//auto map = level.lock()->GetGrid();
	//auto offset = level.lock()->GetFloorOffset();
	//auto node = map[idx];
	glm::vec2 pos;
	auto nodeTransform = m_pCurrentNode.lock()->GetOwner().lock()->GetTransform().GetPosition();
	pos.x = nodeTransform.x + node.lock()->GetNodePos().first + node.lock()->GetNodeSize().first / 2.f;
	pos.y = nodeTransform.y + node.lock()->GetNodePos().second + m_FloorOffset;
	owner->SetPosition(pos);
}

void EnemyComponent::Update()
{
	if (!m_HasSpawned)
	{
		m_ElapsedTime += ElapsedTime::GetInstance().GetElapsedTime();
		if (m_ElapsedTime >= m_SpawnTime)
			m_HasSpawned = true;
	}
	if (m_Dead)
	{
		m_ElapsedTime += ElapsedTime::GetInstance().GetElapsedTime();
		if (m_ElapsedTime >= m_DeathDuration)
			GetOwner().lock()->Remove();
		return;
	}
	if (m_Stunned)
	{
		m_ElapsedTime += ElapsedTime::GetInstance().GetElapsedTime();
		if (m_ElapsedTime >= m_StunDuration)
		{
			m_Stunned = false;
			auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
			switch (m_Direction)
			{
			case Direction::LEFT:
				comp.lock()->SetActiveAnimation(0);
				comp.lock()->SetFlip(false);
				break;
			case Direction::RIGHT:
				comp.lock()->SetActiveAnimation(0);
				comp.lock()->SetFlip(true);
				break;
			case Direction::UP:
				comp.lock()->SetActiveAnimation(2);
				comp.lock()->SetFlip(false);
				break;
			case Direction::DOWN:
				comp.lock()->SetActiveAnimation(1);
				comp.lock()->SetFlip(false);
				break;
			default:
				break;
			}
		}
	}
	if (m_Stunned || m_pTargetNode.expired() || !GetOwner().lock()->GetComponent<MovementComponent>().expired())
		return;

	auto levelPos = m_pCurrentNode.lock()->GetOwner().lock()->GetTransform().GetPosition();
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	if (m_Direction == Direction::LEFT)
	{
		if (pos.x == levelPos.x + m_pTargetNode.lock()->GetNodePos().first + m_pTargetNode.lock()->GetNodeSize().first / 2)
			return;
		pos.x -= m_Velocity * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x < levelPos.x + m_pCurrentNode.lock()->GetNodePos().first)
		{
			m_pCurrentNode = m_pTargetNode;
			m_pPrevNode = m_pTargetNode.lock()->GetConnection(Direction::RIGHT);
		}
		if (pos.x < levelPos.x + m_pTargetNode.lock()->GetNodePos().first + m_pTargetNode.lock()->GetNodeSize().first / 2)
		{
			pos.x = levelPos.x + m_pTargetNode.lock()->GetNodePos().first + m_pTargetNode.lock()->GetNodeSize().first / 2;
			m_ReachedChoicePoint = true;
		}
	}
	if (m_Direction == Direction::RIGHT)
	{
		if (pos.x == levelPos.x + m_pTargetNode.lock()->GetNodePos().first + m_pTargetNode.lock()->GetNodeSize().first / 2)
			return;
		pos.x += m_Velocity * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x > levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + m_pCurrentNode.lock()->GetNodeSize().first)
		{
			m_pCurrentNode = m_pTargetNode;
			m_pPrevNode = m_pTargetNode.lock()->GetConnection(Direction::LEFT);
		}
		if (pos.x > levelPos.x + m_pTargetNode.lock()->GetNodePos().first + m_pTargetNode.lock()->GetNodeSize().first / 2)
		{
			pos.x = levelPos.x + m_pTargetNode.lock()->GetNodePos().first + m_pTargetNode.lock()->GetNodeSize().first / 2;
			m_ReachedChoicePoint = true;
		}
	}
	if (m_Direction == Direction::UP)
	{
		if (pos.y == levelPos.y + m_pTargetNode.lock()->GetNodePos().second + m_FloorOffset)
			return;
		pos.y += m_Velocity * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.y > levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_pCurrentNode.lock()->GetNodeSize().second)
		{
			m_pCurrentNode = m_pTargetNode;
			m_pPrevNode = m_pTargetNode.lock()->GetConnection(Direction::DOWN);
		}
		if (pos.y > levelPos.y + m_pTargetNode.lock()->GetNodePos().second + m_FloorOffset)
		{
			pos.y = levelPos.y + m_pTargetNode.lock()->GetNodePos().second + m_FloorOffset;
			m_ReachedChoicePoint = true;
		}
	}
	if (m_Direction == Direction::DOWN)
	{
		if (pos.y == levelPos.y + m_pTargetNode.lock()->GetNodePos().second + m_FloorOffset)
			return;
		pos.y -= m_Velocity * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.y < levelPos.y + m_pCurrentNode.lock()->GetNodePos().second)
		{
			m_pPrevNode = m_pTargetNode.lock()->GetConnection(Direction::UP);
			m_pCurrentNode = m_pTargetNode;
		}
		if (pos.y < levelPos.y + m_pTargetNode.lock()->GetNodePos().second + m_FloorOffset)
		{
			pos.y = levelPos.y + m_pTargetNode.lock()->GetNodePos().second + m_FloorOffset;
			m_ReachedChoicePoint = true;
		}
	}
	GetOwner().lock()->SetPosition(pos);
}

void EnemyComponent::StaticUpdate()
{
}

void EnemyComponent::Render(const dae::Transform&) const
{
}

const std::weak_ptr<NodeComponent> EnemyComponent::getNode() const
{
	return m_pCurrentNode;
}

const std::weak_ptr<NodeComponent> EnemyComponent::getPrevNode() const
{
	return m_pPrevNode;
}

void EnemyComponent::Move(std::weak_ptr<NodeComponent> targetNode)
{
	if (m_Stunned || m_Dead)
		return;
	if (m_pCurrentNode.lock()->HasLadderAccess() && m_pCurrentNode.lock()->IsFloor() &&
		((!m_pCurrentNode.lock()->GetConnection(Direction::UP).expired() && m_pCurrentNode.lock()->GetConnection(Direction::UP).lock() != m_pPrevNode.lock())
			||(!m_pCurrentNode.lock()->GetConnection(Direction::DOWN).expired() && m_pCurrentNode.lock()->GetConnection(Direction::DOWN).lock() != m_pPrevNode.lock())))
	{
		int percentage = rand()%101;
		//std::cout << percentage << "\n";
		if (m_ChanceToTakeLadder >= percentage)
		{
			if (m_pCurrentNode.lock()->GetConnection(Direction::UP).expired() || m_pCurrentNode.lock()->GetConnection(Direction::UP).lock() == m_pPrevNode.lock())
				m_pTargetNode = m_pCurrentNode.lock()->GetConnection(Direction::DOWN).lock();
			else if (m_pCurrentNode.lock()->GetConnection(Direction::DOWN).expired() || m_pCurrentNode.lock()->GetConnection(Direction::DOWN).lock() == m_pPrevNode.lock())
				m_pTargetNode = m_pCurrentNode.lock()->GetConnection(Direction::UP).lock();
			else
			{
				int direction = rand() % 2;
				if (direction == 0)
					m_pTargetNode = m_pCurrentNode.lock()->GetConnection(Direction::DOWN).lock();
				else
					m_pTargetNode = m_pCurrentNode.lock()->GetConnection(Direction::UP).lock();
			}
		}
		else
			m_pTargetNode = targetNode;
	}
	else
		m_pTargetNode = targetNode;

	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	if (m_pCurrentNode.lock()->GetConnection(Direction::LEFT).lock() == m_pTargetNode.lock())
	{
		m_Direction = Direction::LEFT;
		comp.lock()->SetActiveAnimation(0);
		comp.lock()->SetFlip(false);
	}
	else if (m_pCurrentNode.lock()->GetConnection(Direction::RIGHT).lock() == m_pTargetNode.lock())
	{
		comp.lock()->SetActiveAnimation(0);
		comp.lock()->SetFlip(true);
		m_Direction = Direction::RIGHT;
	}
	else if (m_pCurrentNode.lock()->GetConnection(Direction::UP).lock() == m_pTargetNode.lock())
	{
		comp.lock()->SetActiveAnimation(2);
		comp.lock()->SetFlip(false);
		m_Direction = Direction::UP;
	}
	else if (m_pCurrentNode.lock()->GetConnection(Direction::DOWN).lock() == m_pTargetNode.lock())
	{
		comp.lock()->SetActiveAnimation(1);
		comp.lock()->SetFlip(false);
		m_Direction = Direction::DOWN;
	}

	m_ReachedChoicePoint = false;
}

const bool EnemyComponent::ReachedChoicePoint() const
{
	return m_ReachedChoicePoint;
}

const std::pair<int, int> EnemyComponent::GetRectSize() const
{
	return m_RectSize;
}

void EnemyComponent::Stun()
{
	if (m_Dead || m_Stunned)
		return;
	m_HasSpawned = true;
	m_Stunned = true;
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	comp.lock()->SetActiveAnimation(3);
	m_ElapsedTime = 0.f;
	SoundLocator::get_sound_system().play(1, 0.3f, SoundType::EFFECT);
}

void EnemyComponent::Kill(std::weak_ptr<PeterPepperComponent>& player)
{
	if (m_Dead)
		return;
	if(!player.expired())
		player.lock()->AddPoints(m_Points);
	m_Dead = true;
	m_HasSpawned = true;
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	comp.lock()->SetActiveAnimation(4);
	m_ElapsedTime = 0.f;
	SoundLocator::get_sound_system().play(0, 0.3f, SoundType::EFFECT);
}

const bool EnemyComponent::IsStunned() const
{
	return m_Stunned;
}

const int EnemyComponent::GetFloorOffset() const
{
	return m_FloorOffset;
}

void EnemyComponent::SetCurrentNode(const std::weak_ptr<NodeComponent>& node)
{
	m_pCurrentNode = node;
}

void EnemyComponent::SetDirection(Direction dir)
{
	m_Direction = dir;
}

const bool EnemyComponent::IsDead() const
{
	return m_Dead;
}

const bool EnemyComponent::HasSpawned() const
{
	return m_HasSpawned;
}

const float EnemyComponent::GetVelocity() const
{
	return m_Velocity;
}
