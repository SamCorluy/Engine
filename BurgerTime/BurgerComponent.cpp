#include "BurgerComponent.h"
#include "GameObject.h"
#include "ElapsedTime.h"
#include <iostream>
#include "TextureManagerComponent.h"

const float dropVelocity{ 100.f };

BurgerComponent::BurgerComponent(const std::weak_ptr<NodeComponent> node, const std::weak_ptr<Engine::Scene>& scene, std::vector<BurgerInit> burgerInit, const std::shared_ptr<Engine::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level, size_t priority)
	: BaseComponent(owner)
	, m_RectSize{ 40 * scale, 6 * scale }
{
	// Initialize subject

	for (auto comp : burgerInit)
	{
		auto gameObject = std::make_shared<Engine::GameObject>();
		gameObject->AddComponent<IngredientComponent>(std::make_shared<IngredientComponent>(comp.idx, gameObject, scale, level, comp.type));
		scene.lock()->Add(gameObject, priority);
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
	GetOwner().lock()->AddComponent<Engine::TextureManagerComponent>(std::make_shared<Engine::TextureManagerComponent>(GetOwner().lock(), textureInfo, scale));
}

void BurgerComponent::Update()
{
	for (size_t i = 0; i < m_pIngredients.size(); ++i)
	{
		//Get current ingredient and its position
		auto ingredient = m_pIngredients[i];
		auto pos = ingredient.lock()->GetOwner().lock()->GetTransform().GetPosition();

		//Ignore if not dropping or reached plate
		if (!ingredient.lock()->hasDropped() || ingredient.lock()->HasReachedPlate())
			continue;

		//If first ingredient, don't need to check for ingredients below
		if (i == 0)
		{
			pos.y -= Engine::ElapsedTime::GetInstance().GetElapsedTime() * dropVelocity;
			//If ingredient reached plate
			if (pos.y <= GetOwner().lock()->GetTransform().GetPosition().y + 1.f)
			{
				pos.y = GetOwner().lock()->GetTransform().GetPosition().y + 1.f;
				ingredient.lock()->SetReachedPlate();
				//ingredient.lock()->GetOwner().lock()->SetPosition(pos);
			}
		}
		else
		{
			//Getting ingredient below and its position
			auto ingredientBelow = m_pIngredients[i - 1];
			auto posBelow = ingredientBelow.lock()->GetOwner().lock()->GetTransform().GetPosition();
			//Check if current ingredient positioned below the top of the ingredient below
			if (pos.y < posBelow.y + ingredientBelow.lock()->GetTextureHeight())
			{
				//Drop ingredient below and then continue to next ingredient
				pos.y = posBelow.y + ingredientBelow.lock()->GetTextureHeight();
				if (ingredientBelow.lock()->HasReachedPlate())
					ingredient.lock()->SetReachedPlate();
				else
				{
					auto player = ingredient.lock()->GetPlayer();
					ingredientBelow.lock()->setDropped(true, player);
				}
				//continue;
			}
			else
			{
				//Else continue dropping burger
				pos.y -= Engine::ElapsedTime::GetInstance().GetElapsedTime() * dropVelocity;
			}
		}
		//Check if burger is not on the start floor but current loccation is floor
		auto currentNode = ingredient.lock()->GetNode();
		auto nodePos = currentNode.lock()->GetOwner().lock()->GetTransform().GetPosition();
		if (!ingredient.lock()->HasReachedPlate() && currentNode.lock() != ingredient.lock()->GetStartNode().lock() && currentNode.lock()->IsFloor())
			//Check if burger passed or is on the current floor
			if (nodePos.y + currentNode.lock()->GetNodePos().second - ingredient.lock()->GetBurgerOffset() >= pos.y)
			{
				//Stop at floor and if still dropping continue to next ingredient
				auto player = ingredient.lock()->GetPlayer();
				ingredient.lock()->setDropped(false, player);
				if (ingredient.lock()->hasDropped())
					continue;
				pos.y = nodePos.y + currentNode.lock()->GetNodePos().second - ingredient.lock()->GetBurgerOffset();
			}
		ingredient.lock()->GetOwner().lock()->SetPosition(pos);
	}
}

void BurgerComponent::StaticUpdate()
{
}

void BurgerComponent::Render(const Engine::Transform&) const
{
}

const std::vector<std::weak_ptr<IngredientComponent>> BurgerComponent::getIngredients() const
{
	return m_pIngredients;
}

const bool BurgerComponent::IsComplete() const
{
	return m_pIngredients[m_pIngredients.size() - 1].lock()->HasReachedPlate();
}
