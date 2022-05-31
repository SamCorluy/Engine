#include "SaltComponent.h"
#include "AnimationComponent.h"
#include "ElapsedTime.h"

SaltComponent::SaltComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, Direction direction)
	:BaseComponent(owner)
	, m_pNode{node}
	, m_ElapsedTime{0}
	, m_FloorOffset{floorOffset}
	, m_Duration{0.5f}
{
	// Handling animation info
	std::vector<dae::AnimationInit> animInitList;
	std::string fileName{};
	bool flip = false;
	switch (direction)
	{
	case Direction::LEFT:
		fileName = "Textures/Salt/Walk.png";
		break;
	case Direction::RIGHT:
		fileName = "Textures/Salt/Walk.png";
		flip = true;
		break;
	case Direction::UP:
		fileName = "Textures/Salt/Up.png";
		break;
	case Direction::DOWN:
		fileName = "Textures/Salt/Down.png";
		break;
	default:
		break;
	}
	animInitList.push_back(dae::AnimationInit(4, m_Duration, fileName));
	owner->AddComponent<dae::AnimationComponent>(std::make_shared<dae::AnimationComponent>(owner, animInitList, scale));
	owner->GetComponent<dae::AnimationComponent>().lock()->SetFlip(flip);

	// Handle spawnpoint
	//std::pair<int, int> idx = { 0,0 };
	//auto map = level.lock()->GetGrid();
	//auto offset = level.lock()->GetFloorOffset();
	//auto node = map[idx];
	glm::vec2 pos;
	auto nodeTransform = m_pNode.lock()->GetOwner().lock()->GetTransform().GetPosition();
	pos.x = nodeTransform.x + node.lock()->GetNodePos().first + node.lock()->GetNodeSize().first / 2.f;
	pos.y = nodeTransform.y + node.lock()->GetNodePos().second + m_FloorOffset;
	owner->SetPosition(pos);
}

void SaltComponent::Update()
{
	m_ElapsedTime += ElapsedTime::GetInstance().GetElapsedTime();
}

void SaltComponent::StaticUpdate()
{
}

void SaltComponent::Render(const dae::Transform&) const
{
}

const bool SaltComponent::IsDone() const
{
	return m_Duration <= m_ElapsedTime;
}