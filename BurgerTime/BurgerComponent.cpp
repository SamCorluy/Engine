#include "BurgerComponent.h"
#include "GameObject.h"
#include "ElapsedTime.h"
#include <iostream>
#include "TextureManagerComponent.h"

const float dropVelocity{ 100.f };

BurgerComponent::BurgerComponent(const std::weak_ptr<NodeComponent> node, const std::weak_ptr<dae::Scene>& scene, std::vector<BurgerInit> burgerInit, const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level)
	: BaseComponent(owner)
	, m_RectSize{ 40 * scale, 6 * scale }
{
	// Initialize subject

	for (auto comp : burgerInit)
	{
		auto gameObject = std::make_shared<dae::GameObject>();
		gameObject->AddComponent<IngredientComponent>(std::make_shared<IngredientComponent>(comp.idx, gameObject, scale, level, comp.type));
		scene.lock()->Add(gameObject);
		m_pIngredients.push_back(gameObject->GetComponent<IngredientComponent>());
	}
	glm::vec2 pos;
	pos.x = node.lock()->GetNodePos().first + node.lock()->GetNodeSize().first / 2 + node.lock()->GetOwner().lock()->GetTransform().GetPosition().x;
	pos.y = node.lock()->GetNodePos().second + node.lock()->GetOwner().lock()->GetTransform().GetPosition().y;
	GetOwner().lock()->SetPosition(pos);
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	info.second.y = static_cast<float>( - m_RectSize.second / 2);
	info.first = "Textures/Burger/Plate.png";
	info.second.x = static_cast<float>(-m_RectSize.first / 2);
	textureInfo.push_back(info);
	GetOwner().lock()->AddComponent<dae::TextureManagerComponent>(std::make_shared<dae::TextureManagerComponent>(GetOwner().lock(), textureInfo, scale));
}

void BurgerComponent::Update()
{
	for (size_t i = 0; i < m_pIngredients.size(); ++i)
	{
		auto ingredient = m_pIngredients[i];
		auto pos = ingredient.lock()->GetOwner().lock()->GetTransform().GetPosition();
		if (!ingredient.lock()->hasDropped())
			continue;


		if (i == 0)
		{
			pos.y -= ElapsedTime::GetInstance().GetElapsedTime() * dropVelocity;
		}
		else
		{
			auto ingredientBelow = m_pIngredients[i - 1];
			auto posBelow = ingredientBelow.lock()->GetOwner().lock()->GetTransform().GetPosition();
			if (pos.y < posBelow.y + ingredientBelow.lock()->GetTextureHeight())
			{
				pos.y = posBelow.y + ingredientBelow.lock()->GetTextureHeight();
				ingredientBelow.lock()->setDropped(true, ingredient.lock()->GetPlayerIdx());
				continue;
			}
			else
			{
				pos.y -= ElapsedTime::GetInstance().GetElapsedTime() * dropVelocity;
			}
		}
		if (ingredient.lock()->GetNode().lock() != ingredient.lock()->GetStartNode().lock() && ingredient.lock()->GetNode().lock()->IsFloor())
			if (ingredient.lock()->GetNode().lock()->GetOwner().lock()->GetTransform().GetPosition().y + ingredient.lock()->GetNode().lock()->GetNodePos().second - ingredient.lock()->GetBurgerOffset() >= pos.y)
			{
				pos.y = ingredient.lock()->GetNode().lock()->GetOwner().lock()->GetTransform().GetPosition().y + ingredient.lock()->GetNode().lock()->GetNodePos().second - ingredient.lock()->GetBurgerOffset();
				ingredient.lock()->setDropped(false, ingredient.lock()->GetPlayerIdx());
				if (ingredient.lock()->hasDropped())
					continue;
			}
		if (pos.y <= GetOwner().lock()->GetTransform().GetPosition().y + 1.f)
		{
			pos.y = GetOwner().lock()->GetTransform().GetPosition().y;
			ingredient.lock()->GetOwner().lock()->SetPosition(pos);
		}
		ingredient.lock()->GetOwner().lock()->SetPosition(pos);
	}
}

void BurgerComponent::StaticUpdate()
{
}

void BurgerComponent::Render(const dae::Transform&) const
{
}

const std::vector<std::weak_ptr<IngredientComponent>> BurgerComponent::getIngredients() const
{
	return m_pIngredients;
}
