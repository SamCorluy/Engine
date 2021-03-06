#include "PeterPepperComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include<vector>
#include <iostream>
#include "ElapsedTime.h"
#include "Observer.h"
PeterPepperComponent::PeterPepperComponent(const std::shared_ptr<Engine::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, const std::weak_ptr<Engine::Scene>& scene, int lives, std::string folder)
	:BaseComponent(owner)
	, m_MovementProcessed{false}
	, m_pCurrentNode{ node }
	, m_pStartNode{ node }
	, m_FloorOffset{floorOffset}
	, m_SaltCooldown{1.2f}
	, m_ThrowDuration{0.4f}
	, m_ElapsedTime{0.f}
	, m_ThrowingSalt{false}
	, m_CanThrow{true}
	, m_pScene{scene}
	, m_Direction{Direction::RIGHT}
	, m_RectSize{ 16 * scale, 16 * scale }
	, m_Lives{lives}
	, m_DieDuration{ 2.f }
	, m_Dead{false}
	, m_Velocity{100.f}
{
	// Initialize subject
	owner->AddComponent<Engine::Subject>(std::make_shared<Engine::Subject>(owner));
	m_pSubject = owner->GetComponent<Engine::Subject>();

	// Handling animation info
	std::vector<Engine::AnimationInit> animInitList;
	std::string fileName{ folder + "PeterPepperWalkSideWays.png" };
	animInitList.push_back(Engine::AnimationInit(3, 0.32f, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = folder + "peterPepperLadderDown.png";
	animInitList.push_back(Engine::AnimationInit(2, 0.2f, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = folder + "peterPepperLadderUp.png";
	animInitList.push_back(Engine::AnimationInit(2, 0.2f, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = folder + "PeterPepperIdle.png";
	animInitList.push_back(Engine::AnimationInit(1, 0.f, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = folder + "PeterPepperThrowSaltWalk.png";
	animInitList.push_back(Engine::AnimationInit(1, 0.f, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = folder + "PeterPepperThrowSaltDown.png";
	animInitList.push_back(Engine::AnimationInit(1, 0.f, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = folder + "PeterPepperThrowSaltUp.png";
	animInitList.push_back(Engine::AnimationInit(1, 0.f, fileName, { -m_RectSize.first / 2, 0 }));
	fileName = folder + "PeterPepperDeath.png";
	animInitList.push_back(Engine::AnimationInit(12, m_DieDuration, fileName, { -m_RectSize.first / 2, 0 }));
	owner->AddComponent<Engine::AnimationComponent>(std::make_shared<Engine::AnimationComponent>(owner, animInitList, scale));

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
	if (m_Dead)
	{
		m_ElapsedTime += Engine::ElapsedTime::GetInstance().GetElapsedTime();
		return;
	}
	if (!m_MovementProcessed)
	{
		auto comp = GetOwner().lock()->GetComponent<Engine::AnimationComponent>();
		comp.lock()->SetActiveAnimation(3);
		comp.lock()->SetFlip(false);
	}
	if(!m_ThrowingSalt)
		m_MovementProcessed = false;
	if(!m_CanThrow)
	{
		m_ElapsedTime += Engine::ElapsedTime::GetInstance().GetElapsedTime();
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

void PeterPepperComponent::Render(const Engine::Transform&) const
{
}

const std::weak_ptr<NodeComponent> PeterPepperComponent::getNode() const
{
	return m_pCurrentNode;
}

const std::weak_ptr<SaltComponent> PeterPepperComponent::GetSalt() const
{
	return m_pSaltComponent;
}

const bool PeterPepperComponent::IsThrowingSalt() const
{
	return m_ThrowingSalt;
}

void PeterPepperComponent::ThrowSalt()
{
	m_MovementProcessed = true;
	if (m_CanThrow && m_SaltCharges > 0)
	{
		--m_SaltCharges;
		if (!m_pSubject.expired())
			m_pSubject.lock()->Notify(Engine::Event::SALT_THROW, m_SaltCharges);
		m_CanThrow = false;
		m_ThrowingSalt = true;
		size_t texture{};
		bool flip{false};
		auto pos = GetOwner().lock()->GetTransform().GetPosition();
		//float offset = static_cast<float>(m_RectSize.first);
		switch (m_Direction)
		{
		case Direction::LEFT:
			pos.x -= m_RectSize.first;
			texture = 4;
			break;
		case Direction::RIGHT:
			pos.x += m_RectSize.first;
			flip = true;
			texture = 4;
			break;
		case Direction::UP:
			pos.y += m_RectSize.second;
			texture = 6;
			break;
		case Direction::DOWN:
			pos.y -= m_RectSize.second;
			texture = 5;
			break;
		default:
			break;
		}
		auto obj = std::make_shared<Engine::GameObject>();
		obj->AddComponent<SaltComponent>(std::make_shared<SaltComponent>(obj, 3, m_pCurrentNode, pos, m_FloorOffset, m_Direction));
		m_pScene.lock()->Add(obj);
		auto comp = GetOwner().lock()->GetComponent<Engine::AnimationComponent>();
		comp.lock()->SetActiveAnimation(texture);
		comp.lock()->SetFlip(flip);
		m_pSaltComponent = obj->GetComponent<SaltComponent>();
	}
}

void PeterPepperComponent::Die()
{
	if (m_Dead)
		return;
	auto comp = GetOwner().lock()->GetComponent<Engine::AnimationComponent>();
	comp.lock()->SetActiveAnimation(7);
	--m_Lives;
	if (!m_pSubject.expired())
		m_pSubject.lock()->Notify(Engine::Event::CHARACTER_DEAD, m_Lives);
	m_ElapsedTime = 0.f;
	m_CanThrow = false;
	m_ThrowingSalt = false;
	m_Dead = true;
	if (m_Lives <= 0)
	{
		if(!m_pSaltComponent.expired())
			m_pSaltComponent.lock()->GetOwner().lock()->Remove();
		GetOwner().lock()->Remove();
	}
}

void PeterPepperComponent::Reset(const std::weak_ptr<NodeComponent>& node)
{
	m_pStartNode = node;
	m_Dead = false;
	m_CanThrow = true;
	m_ThrowingSalt = false;
	m_MovementProcessed = false;
	m_pCurrentNode = m_pStartNode;
	m_ElapsedTime = 0.f;
	glm::vec2 pos;
	auto nodeTransform = m_pCurrentNode.lock()->GetOwner().lock()->GetTransform().GetPosition();
	pos.x = nodeTransform.x + m_pCurrentNode.lock()->GetNodePos().first + m_pCurrentNode.lock()->GetNodeSize().first / 2.f;
	pos.y = nodeTransform.y + m_pCurrentNode.lock()->GetNodePos().second + m_FloorOffset;
	GetOwner().lock()->SetPosition(pos);
}

void PeterPepperComponent::Respawn()
{
	Reset(m_pStartNode);
}

bool PeterPepperComponent::IsDead()
{
	return m_Dead;
}

bool PeterPepperComponent::DeathAnimationFinished()
{
	return m_Dead && m_ElapsedTime >= m_DieDuration;
}

const std::pair<int, int> PeterPepperComponent::GetRectSize() const
{
	return m_RectSize;
}

void PeterPepperComponent::AddPoints(int points)
{
	m_Points += points;
	m_pSubject.lock()->Notify(Engine::Event::SCORE_CHANGE, m_Points);
}

const int PeterPepperComponent::GetFloorOffset() const
{
	return m_FloorOffset;
}

void PeterPepperComponent::SetDirection(Direction dir)
{
	m_Direction = dir;
}

void PeterPepperComponent::SetMovementProcessed(bool processed)
{
	m_MovementProcessed = processed;
}

void PeterPepperComponent::SetCurrentNode(const std::weak_ptr<NodeComponent>& node)
{
	m_pCurrentNode = node;
}

const float PeterPepperComponent::GetVelocity() const
{
	return m_Velocity;
}
