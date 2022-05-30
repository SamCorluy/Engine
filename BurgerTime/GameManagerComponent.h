#pragma once
#include "LevelComponent.h"
#include "PeterPepperComponent.h"
#include "BurgerComponent.h"
#include "EnemyComponent.h"
class GameManagerComponent final : public dae::BaseComponent
{
public:
	GameManagerComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;
private:
	void InitSinglePlayer();
	void CheckBurgerOverlap();
	std::weak_ptr<dae::Scene> m_pScene;
	std::weak_ptr<LevelComponent> m_pLevel;
	std::weak_ptr<PeterPepperComponent> m_pPlayer;
	std::vector<std::weak_ptr<BurgerComponent>> m_pBurgers;
	std::vector<std::weak_ptr<NodeComponent>> FindPath(std::weak_ptr<NodeComponent> start, std::weak_ptr<NodeComponent> end, std::weak_ptr<NodeComponent> prevNode);
	std::vector<std::weak_ptr<NodeComponent>> m_TempPath;
	std::vector<std::weak_ptr<EnemyComponent>> m_pEnemies;
};

