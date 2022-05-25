#include "PeterPepperComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include<vector>
#include <iostream>
#include "ElapsedTime.h"

PeterPepperComponent::PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level)
	:BaseComponent(owner)
	, m_MovementProcessed{false}
	, m_pLevel{ level }
	, m_Offset{0*scale, 3*scale}
{
	owner->AddComponent<dae::Subject>(std::make_shared<dae::Subject>(owner));
	m_pSubject = owner->GetComponent<dae::Subject>();
	std::vector<dae::AnimationInit> animInitList;
	std::string fileName{ "PeterPepperWalkSideWays.png" };
	animInitList.push_back(dae::AnimationInit(3, 0.4f, fileName));
	fileName = "peterPepperLadderDown.png";
	animInitList.push_back(dae::AnimationInit(2, 0.3f, fileName));
	fileName = "peterPepperLadderUp.png";
	animInitList.push_back(dae::AnimationInit(2, 0.3f, fileName));
	fileName = "PeterPepperIdle.png";
	animInitList.push_back(dae::AnimationInit(1, 0.f, fileName));
	owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList, scale));
	std::pair<int, int> idx = { 0,0 };
	auto map = level.lock()->GetGrid();
	auto node = map[idx];
	glm::vec2 pos;
	auto levelTransform = level.lock()->GetOwner().lock()->GetTransform().GetPosition();
	pos.x = levelTransform.x + node.nodePos.first + m_Offset.first;
	pos.y = levelTransform.y + node.nodePos.second + m_Offset.second;
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
	m_MovementProcessed = false;
}

void PeterPepperComponent::StaticUpdate()
{
}

void PeterPepperComponent::Render(const dae::Transform&) const
{
}

void PeterPepperComponent::Move(Action action)
{
	auto grid = m_pLevel.lock()->GetGrid();
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	auto idx = m_pLevel.lock()->CoordinateToIndex(pos);
	switch (action)
	{
	case Action::WalkingLeft:
		if (grid.find({ idx.first -1, idx.second }) != grid.end() && grid[{ idx.first - 1, idx.second }].floor)
		{
			comp.lock()->SetActiveAnimation(0);
			comp.lock()->SetFlip(false);
			pos.x -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			GetOwner().lock()->SetPosition(pos);
		}
		else
			return;
		break;
	case Action::WalkingRight:
		if (grid.find({ idx.first + 1, idx.second }) != grid.end() && grid[{ idx.first + 1, idx.second }].floor)
		{
			comp.lock()->SetActiveAnimation(0);
			comp.lock()->SetFlip(true);
			pos.x += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			GetOwner().lock()->SetPosition(pos);
		}
		else
			return;
		break;
	case Action::ClimbingUp:
		comp.lock()->SetActiveAnimation(2);
		comp.lock()->SetFlip(false);
		break;
	case Action::ClimbingDown:
		comp.lock()->SetActiveAnimation(1);
		comp.lock()->SetFlip(false);
		break;
	default:
		break;
	}
	m_MovementProcessed = true;
}
