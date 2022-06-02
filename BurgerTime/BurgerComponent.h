#pragma once
#include "IngredientComponent.h"
#include "Scene.h"
#include "LevelComponent.h"
#include "NodeComponent.h"
class BurgerComponent final : public dae::BaseComponent
{
public:
	BurgerComponent(const std::weak_ptr<NodeComponent> node, const std::weak_ptr<dae::Scene>& scene, std::vector<BurgerInit> burgerInit, const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level, size_t priority);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;
	const std::vector< std::weak_ptr<IngredientComponent>> getIngredients() const;
	const bool IsComplete() const;
private:
	std::vector< std::weak_ptr<IngredientComponent>> m_pIngredients;
	std::pair<int, int> m_RectSize;
};
