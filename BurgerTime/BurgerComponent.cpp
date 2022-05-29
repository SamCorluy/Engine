#include "BurgerComponent.h"
#include "GameObject.h"
#include "ElapsedTime.h"

const float dropVelocity{ 100.f };

BurgerComponent::BurgerComponent(const std::weak_ptr<dae::Scene>& scene, std::vector<BurgerInit> burgerInit, const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level)
	: BaseComponent(owner)
{
	for (auto comp : burgerInit)
	{
		auto gameObject = std::make_shared<dae::GameObject>();
		gameObject->AddComponent<IngredientComponent>(std::make_shared<IngredientComponent>(comp.idx, gameObject, scale, level, comp.type));
		scene.lock()->Add(gameObject);
		m_pIngredients.push_back(gameObject->GetComponent<IngredientComponent>());
	}
}

void BurgerComponent::Update()
{
	for (size_t i = 0; i < m_pIngredients.size(); ++i)
	{
		auto ingredient = m_pIngredients[i];
		auto pos = ingredient.lock()->GetOwner().lock()->GetTransform().GetPosition();
		if (!ingredient.lock()->hasDropped() || pos.y < 0)
			continue;

		if (i == 0)
		{
			pos.y -= ElapsedTime::GetInstance().GetElapsedTime() * 50.f;
		}
		else
		{
			auto ingredientBelow = m_pIngredients[i - 1];
			auto posBelow = ingredientBelow.lock()->GetOwner().lock()->GetTransform().GetPosition();
			if (pos.y < posBelow.y + ingredientBelow.lock()->GetTextureHeight())
			{
				pos.y = posBelow.y + ingredientBelow.lock()->GetTextureHeight();
				continue;
			}
			else
			{
				pos.y -= ElapsedTime::GetInstance().GetElapsedTime() * dropVelocity;
			}
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
