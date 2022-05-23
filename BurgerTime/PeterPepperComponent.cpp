#include "PeterPepperComponent.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include<vector>

PeterPepperComponent::PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, glm::vec2 pos)
	:BaseComponent(owner)
	, currentPos{pos}
{
	owner->AddComponent<dae::Subject>(std::make_shared<dae::Subject>(owner));
	m_pSubject = owner->GetComponent<dae::Subject>();
	std::vector<dae::AnimationInit> animInitList;
	std::string fileName{"PeterPepperWalkSideWays.png"};
	animInitList.push_back(dae::AnimationInit(3, 0.4f, fileName));
	owner->AddComponent<dae::TextureComponent>(std::make_shared<dae::TextureComponent>(owner, animInitList));
}

void PeterPepperComponent::Update()
{
}

void PeterPepperComponent::StaticUpdate()
{
}

void PeterPepperComponent::Render(const dae::Transform&) const
{
}

void PeterPepperComponent::Move(bool)
{
}
