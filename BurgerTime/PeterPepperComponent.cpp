#include "PeterPepperComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include<vector>
#include <iostream>

PeterPepperComponent::PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, glm::vec2 pos)
	:BaseComponent(owner)
	, currentPos{pos}
	, m_MovementProcessed{false}
{
	owner->AddComponent<dae::Subject>(std::make_shared<dae::Subject>(owner));
	m_pSubject = owner->GetComponent<dae::Subject>();
	std::vector<dae::AnimationInit> animInitList;
	std::string fileName{"PeterPepperWalkSideWays.png"};
	animInitList.push_back(dae::AnimationInit(3, 0.4f, fileName));
	fileName = "peterPepperLadderDown.png";
	animInitList.push_back(dae::AnimationInit(2, 0.3f, fileName));
	fileName = "peterPepperLadderUp.png";
	animInitList.push_back(dae::AnimationInit(2, 0.3f, fileName));
	fileName = "PeterPepperIdle.png";
	animInitList.push_back(dae::AnimationInit(1, 0.f, fileName));
	owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList));
	owner->SetPosition(pos);
}

void PeterPepperComponent::Update()
{
	if (!m_MovementProcessed)
	{
		auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
		comp.lock()->SetActiveAnimation(3);
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
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	switch (action)
	{
	case Action::Walking:
		comp.lock()->SetActiveAnimation(0);
		break;
	case Action::Idle:
		break;
	case Action::ClimbingUp:
		comp.lock()->SetActiveAnimation(2);
		break;
	case Action::ClimbingDown:
		comp.lock()->SetActiveAnimation(1);
		break;
	default:
		break;
	}
	m_MovementProcessed = true;
}
