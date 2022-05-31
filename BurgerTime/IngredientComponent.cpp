#include "IngredientComponent.h"
#include "TextureManagerComponent.h"
#include "GameObject.h"
#include <iostream>


IngredientComponent::IngredientComponent(std::pair<int, int> idx, const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level, IngredientType type)
	: BaseComponent(owner)
	//, m_TextureSize{8 * scale}
	, m_DropHeight{ 8 * scale / 5 }
	, m_PlayerIdx{0}
	, m_ExtraDrops{0}
	, m_RectSize{ 32 * scale, 8 * scale }
{
	std::string filePath{"Textures/Burger/"};
	switch (type)
	{
	case IngredientType::BunTop:
		m_TextureHeight = 7 * scale;
		filePath += "BunTop/";
		break;
	case IngredientType::BunBot:
		m_TextureHeight = 7 * scale;
		filePath += "BunBot/";
		break;
	case IngredientType::Patty:
		m_TextureHeight = 7 * scale;
		filePath += "Patty/";
		break;
	case IngredientType::Lettuce:
		m_TextureHeight = 7 * scale;
		filePath += "Lettuce/";
		break;
	case IngredientType::Cheese:
		m_TextureHeight = 6 * scale;
		filePath += "Cheese/";
		break;
	case IngredientType::Tomato:
		m_TextureHeight = 7 * scale;
		filePath += "Tomato/";
		break;
	default:
		break;
	}
	glm::vec2 pos;
	auto map = level.lock()->GetGrid();
	pos.x = map[idx].lock()->GetNodePos().first + map[idx].lock()->GetNodeSize().first / 2 + level.lock()->GetOwner().lock()->GetTransform().GetPosition().x;
	pos.y = map[idx].lock()->GetNodePos().second + level.lock()->GetOwner().lock()->GetTransform().GetPosition().y;
	m_pNode = map[idx];
	m_pStartNode = m_pNode;
	GetOwner().lock()->SetPosition(pos);
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	info.second.y = 0;
	m_pBurgerOffset = (int)info.second.y;
	for (int i = 1; i < 5; ++i)
	{
		info.first = filePath + std::to_string(i) + ".png";
		int a = -m_RectSize.second * 3;
		int b = m_RectSize.second * i;
		int c = a + b;
		//info.second.x = static_cast<float>(-(16 * scale) + (8 * scale * i));
		info.second.x = static_cast<float>(c);
		textureInfo.push_back(info);
		IngredientPartition part;
		part.hasDropped = false;
		part.botLeft.x = info.second.x + GetOwner().lock()->GetTransform().GetPosition().x;
		part.botLeft.y = info.second.y + GetOwner().lock()->GetTransform().GetPosition().y;
		m_pPartitions.push_back(part);
	}
	GetOwner().lock()->AddComponent<dae::TextureManagerComponent>(std::make_shared<dae::TextureManagerComponent>(GetOwner().lock(), textureInfo, scale));
}

void IngredientComponent::Update()
{
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	if (pos.y < m_pNode.lock()->GetOwner().lock()->GetTransform().GetPosition().y + m_pNode.lock()->GetNodePos().second && !m_pNode.lock()->GetConnectionIgnoringWalkable(Direction::DOWN).expired())
		m_pNode = m_pNode.lock()->GetConnectionIgnoringWalkable(Direction::DOWN);
}

void IngredientComponent::StaticUpdate()
{
}

void IngredientComponent::Render(const dae::Transform&) const
{
}

const int IngredientComponent::GetTextureHeight() const
{
	return m_TextureHeight;
}

void IngredientComponent::InitiateDrop(int playerIdx)
{
	m_PlayerIdx = playerIdx;
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	bool walkedOn = false;
	for (int i = 0; i < m_pPartitions.size(); ++i)
	{
		auto text = GetOwner().lock()->GetComponent<dae::TextureManagerComponent>();
		auto offset = text.lock()->getOffset(i);
		if (offset.y == m_pBurgerOffset - m_DropHeight)
		{
			walkedOn = true;
			offset.y += m_DropHeight;
		}
		text.lock()->setOffset(i, offset);
	}
	if(walkedOn)
		pos.y -= m_DropHeight;
	GetOwner().lock()->SetPosition(pos);
}

const bool IngredientComponent::hasDropped() const
{
	for (auto par : m_pPartitions)
		if (!par.hasDropped)
			return false;
	return true;
}

void IngredientComponent::setDropped(bool dropped, int playerIdx)
{
	if (!dropped)
		m_pStartNode = m_pNode;
	if (m_ExtraDrops > 0 && !dropped)
	{
		dropped = true;
		--m_ExtraDrops;
		//std::cout << dropped << "\n";
	}
	for (size_t i = 0; i < m_pPartitions.size(); ++i)
		m_pPartitions[i].hasDropped = dropped;
	if(dropped)
		InitiateDrop(playerIdx);
}

bool IngredientComponent::CheckOverlap(std::weak_ptr<PeterPepperComponent>& player, int playerIdx)
{
	auto playerPos = player.lock()->GetOwner().lock()->GetTransform().GetPosition();
	//std::cout << player.lock()->getNode().lock()->GetNodePos().first << " " << player.lock()->getNode().lock()->GetNodePos().first << "\n";
	//std::cout <<m_pNode.lock()->GetNodePos().first << " " << m_pNode.lock()->GetNodePos().first << "\n\n";
	if (player.lock()->getNode().lock() == m_pNode.lock())
	{
		for (int i = 0; i < m_pPartitions.size(); ++i)
		{
			if (!m_pPartitions[i].hasDropped && ((playerPos.x >= m_pPartitions[i].botLeft.x + m_RectSize.second / 2 && m_PrevPlayerPos.x < m_pPartitions[i].botLeft.x + m_RectSize.second / 2) ||
				(playerPos.x < m_pPartitions[i].botLeft.x + m_RectSize.second / 2 && m_PrevPlayerPos.x >= m_pPartitions[i].botLeft.x + m_RectSize.second / 2)))
			{
				auto text = GetOwner().lock()->GetComponent<dae::TextureManagerComponent>();
				auto offset = text.lock()->getOffset(i);
				offset.y -= m_DropHeight;
				text.lock()->setOffset(i, offset);
				m_pPartitions[i].hasDropped = true;
				if (hasDropped())
				{
					InitiateDrop(playerIdx);
				}
			}
		}
	}
	m_PrevPlayerPos = playerPos;
	return hasDropped();
}

const std::weak_ptr<NodeComponent> IngredientComponent::GetNode() const
{
	return m_pNode;
}

const std::weak_ptr<NodeComponent> IngredientComponent::GetStartNode() const
{
	return m_pStartNode;
}

const int IngredientComponent::GetBurgerOffset() const
{
	return m_pBurgerOffset;
}

const int IngredientComponent::GetPlayerIdx() const
{
	return m_PlayerIdx;
}

void IngredientComponent::SetExtraDrops(int extraDrops)
{
	m_ExtraDrops = extraDrops;
}

const std::pair<int, int> IngredientComponent::GetRectSize() const
{
	return m_RectSize;
}
