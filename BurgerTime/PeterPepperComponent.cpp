#include "PeterPepperComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include<vector>
#include <iostream>
#include "ElapsedTime.h"
PeterPepperComponent::PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, const std::weak_ptr<dae::Scene>& scene)
	:BaseComponent(owner)
	, m_MovementProcessed{false}
	, m_pCurrentNode{ node }
	, m_FloorOffset{floorOffset}
	, m_SaltCooldown{5.f}
	, m_ThrowDuration{0.4f}
	, m_ElapsedTime{0.f}
	, m_ThrowingSalt{false}
	, m_CanThrow{true}
	, m_pScene{scene}
	, m_Direction{Direction::RIGHT}
{
	// Initialize subject
	owner->AddComponent<dae::Subject>(std::make_shared<dae::Subject>(owner));
	m_pSubject = owner->GetComponent<dae::Subject>();

	// Handling animation info
	std::vector<dae::AnimationInit> animInitList;
	std::string fileName{ "Textures/PeterPepper/PeterPepperWalkSideWays.png" };
	animInitList.push_back(dae::AnimationInit(3, 0.32f, fileName));
	fileName = "Textures/PeterPepper/peterPepperLadderDown.png";
	animInitList.push_back(dae::AnimationInit(2, 0.2f, fileName));
	fileName = "Textures/PeterPepper/peterPepperLadderUp.png";
	animInitList.push_back(dae::AnimationInit(2, 0.2f, fileName));
	fileName = "Textures/PeterPepper/PeterPepperIdle.png";
	animInitList.push_back(dae::AnimationInit(1, 0.f, fileName));
	fileName = "Textures/PeterPepper/PeterPepperThrowSaltWalk.png";
	animInitList.push_back(dae::AnimationInit(1, 0.f, fileName));
	fileName = "Textures/PeterPepper/PeterPepperThrowSaltDown.png";
	animInitList.push_back(dae::AnimationInit(1, 0.f, fileName));
	fileName = "Textures/PeterPepper/PeterPepperThrowSaltUp.png";
	animInitList.push_back(dae::AnimationInit(1, 0.f, fileName));
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

void PeterPepperComponent::Update()
{
	if (!m_MovementProcessed)
	{
		auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
		comp.lock()->SetActiveAnimation(3);
		comp.lock()->SetFlip(false);
	}
	if(!m_ThrowingSalt)
		m_MovementProcessed = false;
	if(!m_CanThrow)
	{
		m_ElapsedTime += ElapsedTime::GetInstance().GetElapsedTime();
		if (m_ElapsedTime >= m_SaltCooldown)
		{
			m_ElapsedTime = 0.f;
			m_CanThrow = true;
		}
		if (m_ElapsedTime >= m_ThrowDuration)
		{
			m_ThrowingSalt = false;
		}
	}
}

void PeterPepperComponent::StaticUpdate()
{
}

void PeterPepperComponent::Render(const dae::Transform&) const
{
}

const std::weak_ptr<NodeComponent> PeterPepperComponent::getNode() const
{
	return m_pCurrentNode;
}

void PeterPepperComponent::Move(Action action)
{
	if (m_ThrowingSalt)
		return;
	auto rectSize = GetOwner().lock()->GetComponent<dae::AnimationComponent>().lock()->getActiveAnimRec();
	//auto grid = m_pLevel.lock()->GetGrid();
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	auto levelPos = m_pCurrentNode.lock()->GetOwner().lock()->GetTransform().GetPosition();
	//auto idx = m_pLevel.lock()->CoordinateToIndex(pos);
	auto ladderRect = m_pCurrentNode.lock()->GetLadderAccessSize();
	//auto offset = m_pCurrentNode.lock()->GetFloorOffset();
	switch (action)
	{
	case Action::WalkingLeft:
		if ((m_pCurrentNode.lock()->GetConnection(Direction::LEFT).expired() || !m_pCurrentNode.lock()->GetConnection(Direction::LEFT).lock()->IsFloor())
			&& (levelPos.x + m_pCurrentNode.lock()->GetNodePos().first) >= (pos.x - rectSize.x / 2.f))
		{
			pos.x = levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (!m_pCurrentNode.lock()->IsFloor() || pos.y > (levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset + ladderRect.second))
			return;
		if (pos.y > levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset || pos.y < levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset)
			pos.y = levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset;
		comp.lock()->SetActiveAnimation(0);
		m_Direction = Direction::LEFT;
		comp.lock()->SetFlip(false);
		pos.x -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x < m_pCurrentNode.lock()->GetNodePos().first + levelPos.x)
			m_pCurrentNode = m_pCurrentNode.lock()->GetConnection(Direction::LEFT);
		GetOwner().lock()->SetPosition(pos);
		break;
	case Action::WalkingRight:
		if ((m_pCurrentNode.lock()->GetConnection(Direction::RIGHT).expired() || !m_pCurrentNode.lock()->GetConnection(Direction::RIGHT).lock()->IsFloor())
			&& (levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + m_pCurrentNode.lock()->GetNodeSize().first) <= (pos.x + rectSize.x / 2.f))
		{
			pos.x = levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + m_pCurrentNode.lock()->GetNodeSize().first - rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (!m_pCurrentNode.lock()->IsFloor() || pos.y > (levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset + ladderRect.second))
			return;
		if (pos.y > levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset || pos.y < levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset)
			pos.y = levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset;
		comp.lock()->SetActiveAnimation(0);
		m_Direction = Direction::RIGHT;
		comp.lock()->SetFlip(true);
		pos.x += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x > m_pCurrentNode.lock()->GetNodePos().first + levelPos.x + m_pCurrentNode.lock()->GetNodeSize().first)
			m_pCurrentNode = m_pCurrentNode.lock()->GetConnection(Direction::RIGHT);
		GetOwner().lock()->SetPosition(pos);
		break;
	case Action::ClimbingUp:
		if ((m_pCurrentNode.lock()->GetConnection(Direction::UP).expired() || !m_pCurrentNode.lock()->GetConnection(Direction::UP).lock()->HasLadderAccess())
			&& (levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset) >= pos.y)
		{
			pos.y = levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + (m_pCurrentNode.lock()->GetNodeSize().first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + (m_pCurrentNode.lock()->GetNodeSize().first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(2);
			m_Direction = Direction::UP;
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + m_pCurrentNode.lock()->GetNodeSize().first / 2;
			pos.y += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			if (pos.y > m_pCurrentNode.lock()->GetNodePos().second + levelPos.y + m_pCurrentNode.lock()->GetNodeSize().second)
				m_pCurrentNode = m_pCurrentNode.lock()->GetConnection(Direction::UP);
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	case Action::ClimbingDown:
		if ((m_pCurrentNode.lock()->GetConnection(Direction::DOWN).expired() || !m_pCurrentNode.lock()->GetConnection(Direction::DOWN).lock()->HasLadderAccess())
			&& (levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset) >= pos.y)
		{
			pos.y = levelPos.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + (m_pCurrentNode.lock()->GetNodeSize().first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + (m_pCurrentNode.lock()->GetNodeSize().first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(1);
			m_Direction = Direction::DOWN;
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + m_pCurrentNode.lock()->GetNodePos().first + m_pCurrentNode.lock()->GetNodeSize().first / 2;
			pos.y -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			if (pos.y < m_pCurrentNode.lock()->GetNodePos().second + levelPos.y)
				m_pCurrentNode = m_pCurrentNode.lock()->GetConnection(Direction::DOWN);
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	default:
		break;
	}
	m_MovementProcessed = true;
}

void PeterPepperComponent::ThrowSalt()
{
	m_MovementProcessed = true;
	if (m_CanThrow)
	{
		m_CanThrow = false;
		m_ThrowingSalt = true;
		std::weak_ptr<NodeComponent> spawnNode;
		if (m_pCurrentNode.lock()->GetConnection(m_Direction).expired())
			spawnNode = m_pCurrentNode;
		else
			spawnNode = m_pCurrentNode.lock()->GetConnection(m_Direction);
		size_t texture{};
		bool flip{false};
		switch (m_Direction)
		{
		case Direction::LEFT:
			texture = 4;
			break;
		case Direction::RIGHT:
			flip = true;
			texture = 4;
			break;
		case Direction::UP:
			texture = 6;
			break;
		case Direction::DOWN:
			texture = 5;
			break;
		default:
			break;
		}
		auto obj = std::make_shared<dae::GameObject>();
		obj->AddComponent<SaltComponent>(std::make_shared<SaltComponent>(obj, 3, spawnNode, m_FloorOffset, m_Direction));
		m_pScene.lock()->Add(obj);
		auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
		comp.lock()->SetActiveAnimation(texture);
		comp.lock()->SetFlip(flip);
	}
}
