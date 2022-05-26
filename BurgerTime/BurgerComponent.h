#pragma once
#include "IngredientComponent.h"
#include "Scene.h"

struct BurgerInit
{
	std::pair<int, int> idx;
	IngredientType type;
};
class BurgerComponent final : public dae::BaseComponent
{
public:
	BurgerComponent(const std::weak_ptr<dae::Scene>& scene, std::vector<BurgerInit> burgerInit, const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level, std::weak_ptr<PeterPepperComponent>& peterPepper);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;
private:
	std::vector< std::weak_ptr<IngredientComponent>> m_pIngredients;
};
