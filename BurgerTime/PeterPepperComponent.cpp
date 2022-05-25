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
{
	// Initialize subject
	owner->AddComponent<dae::Subject>(std::make_shared<dae::Subject>(owner));
	m_pSubject = owner->GetComponent<dae::Subject>();

	// Handling animation info
	std::vector<dae::AnimationInit> animInitList;
	std::string fileName{ "PeterPepperWalkSideWays.png" };
	animInitList.push_back(dae::AnimationInit(3, 0.32f, fileName));
	fileName = "peterPepperLadderDown.png";
	animInitList.push_back(dae::AnimationInit(2, 0.2f, fileName));
	fileName = "peterPepperLadderUp.png";
	animInitList.push_back(dae::AnimationInit(2, 0.2f, fileName));
	fileName = "PeterPepperIdle.png";
	animInitList.push_back(dae::AnimationInit(1, 0.f, fileName));
	owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList, scale));

	// Handle spawnpoint
	std::pair<int, int> idx = { 0,0 };
	auto map = level.lock()->GetGrid();
	auto offset = level.lock()->GetFloorOffset();
	auto node = map[idx];
	glm::vec2 pos;
	auto levelTransform = level.lock()->GetOwner().lock()->GetTransform().GetPosition();
	pos.x = levelTransform.x + node.nodePos.first + node.nodeSize.first / 2.f;
	pos.y = levelTransform.y + node.nodePos.second + offset;
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
	auto rectSize = GetOwner().lock()->GetComponent<dae::AnimationComponent>().lock()->getActiveAnimRec();
	auto grid = m_pLevel.lock()->GetGrid();
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	auto levelPos = m_pLevel.lock()->GetOwner().lock()->GetTransform().GetPosition();
	auto idx = m_pLevel.lock()->CoordinateToIndex(pos);
	auto ladderRect = m_pLevel.lock()->GetLadderAccess();
	auto offset = m_pLevel.lock()->GetFloorOffset();
	switch (action)
	{
	case Action::WalkingLeft:
		if ((grid.find({ idx.first - 1, idx.second }) == grid.end() || !grid[{ idx.first - 1, idx.second }].floor)
			&& (levelPos.x + grid[idx].nodePos.first) >= (pos.x - rectSize.x / 2.f))
		{
			pos.x = levelPos.x + grid[idx].nodePos.first + rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (pos.y > (levelPos.y + grid[idx].nodePos.second + offset + ladderRect.second))
			return;
		if (pos.y > levelPos.y + grid[idx].nodePos.second + offset)
			pos.y = levelPos.y + grid[idx].nodePos.second + offset;
		comp.lock()->SetActiveAnimation(0);
		comp.lock()->SetFlip(false);
		pos.x -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		GetOwner().lock()->SetPosition(pos);
		break;
	case Action::WalkingRight:
		if ((grid.find({ idx.first + 1, idx.second }) == grid.end() || !grid[{ idx.first + 1, idx.second }].floor)
			&& (levelPos.x + grid[idx].nodePos.first + grid[idx].nodeSize.first) <= (pos.x + rectSize.x / 2.f))
		{
			pos.x = levelPos.x + grid[idx].nodePos.first + grid[idx].nodeSize.first - rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (pos.y > (levelPos.y + grid[idx].nodePos.second + offset + ladderRect.second))
			return;
		if (pos.y > levelPos.y + grid[idx].nodePos.second + offset)
			pos.y = levelPos.y + grid[idx].nodePos.second + offset;
		comp.lock()->SetActiveAnimation(0);
		comp.lock()->SetFlip(true);
		pos.x += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		GetOwner().lock()->SetPosition(pos);
		break;
	case Action::ClimbingUp:
		if ((grid.find({ idx.first, idx.second + 1 }) == grid.end() || !grid[{ idx.first, idx.second + 1 }].ladderAccess)
			&& (levelPos.y + grid[idx].nodePos.second + offset) >= pos.y)
		{
			pos.y = levelPos.y + grid[idx].nodePos.second + offset;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + grid[idx].nodePos.first + (grid[idx].nodeSize.first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + grid[idx].nodePos.first + (grid[idx].nodeSize.first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(2);
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + grid[idx].nodePos.first + grid[idx].nodeSize.first / 2;
			pos.y += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	case Action::ClimbingDown:
		if ((grid.find({ idx.first, idx.second - 1 }) == grid.end() || !grid[{ idx.first, idx.second - 1 }].ladderAccess)
			&& (levelPos.y + grid[idx].nodePos.second + offset) >= pos.y)
		{
			pos.y = levelPos.y + grid[idx].nodePos.second + offset;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + grid[idx].nodePos.first + (grid[idx].nodeSize.first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + grid[idx].nodePos.first + (grid[idx].nodeSize.first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(1);
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + grid[idx].nodePos.first + grid[idx].nodeSize.first / 2;
			pos.y -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	default:
		break;
	}
	m_MovementProcessed = true;
}
