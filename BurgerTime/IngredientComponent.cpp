#include "IngredientComponent.h"
#include "TextureManagerComponent.h"
#include "GameObject.h"


IngredientComponent::IngredientComponent(std::pair<int, int> idx, const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level, IngredientType type, std::weak_ptr<PeterPepperComponent>& peterPepper)
	: BaseComponent(owner)
	, m_pPlayer{peterPepper}
	, m_pLevel{level}
	, m_TextureSize{8 * scale}
	, m_DropHeight{ 8 * scale / 5 }
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
	pos.x = map[idx].nodePos.first + map[idx].nodeSize.first / 2 + level.lock()->GetOwner().lock()->GetTransform().GetPosition().x;
	pos.y = map[idx].nodePos.second + level.lock()->GetOwner().lock()->GetTransform().GetPosition().y;
	GetOwner().lock()->SetPosition(pos);
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	for (int i = 1; i < 5; ++i)
	{
		info.first = filePath + std::to_string(i) + ".png";
		int a = -m_TextureSize * 3;
		int b = m_TextureSize * i;
		int c = a + b;
		//info.second.x = static_cast<float>(-(16 * scale) + (8 * scale * i));
		info.second.x = static_cast<float>(c);
		info.second.y = -static_cast<float>(m_TextureSize / 3);
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
	auto playerPos = m_pPlayer.lock()->GetOwner().lock()->GetTransform().GetPosition();
	if (m_pLevel.lock()->CoordinateToIndex(playerPos) == m_pLevel.lock()->CoordinateToIndex(GetOwner().lock()->GetTransform().GetPosition()))
	{
		for (int i = 0; i < m_pPartitions.size(); ++i)
		{
			if (!m_pPartitions[i].hasDropped && ((playerPos.x >= m_pPartitions[i].botLeft.x + m_TextureSize / 2 && m_PrevPlayerPos.x < m_pPartitions[i].botLeft.x + m_TextureSize / 2) ||
				(playerPos.x < m_pPartitions[i].botLeft.x + m_TextureSize / 2 && m_PrevPlayerPos.x >= m_pPartitions[i].botLeft.x + m_TextureSize / 2)))
			{
				auto text = GetOwner().lock()->GetComponent<dae::TextureManagerComponent>();
				auto offset = text.lock()->getOffset(i);
				offset.y -= m_DropHeight;
				text.lock()->setOffset(i, offset);
				m_pPartitions[i].hasDropped = true;
				if (hasDropped())
				{
					InitiateDrop();
				}
			}
		}
	}
	m_PrevPlayerPos = playerPos;
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

void IngredientComponent::InitiateDrop()
{
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	pos.y -= m_DropHeight;
	GetOwner().lock()->SetPosition(pos);
	for (int i = 0; i < m_pPartitions.size(); ++i)
	{
		auto text = GetOwner().lock()->GetComponent<dae::TextureManagerComponent>();
		auto offset = text.lock()->getOffset(i);
		offset.y += m_DropHeight;
		text.lock()->setOffset(i, offset);
	}
}

const bool IngredientComponent::hasDropped() const
{
	for (auto par : m_pPartitions)
		if (!par.hasDropped)
			return false;
	return true;
}
