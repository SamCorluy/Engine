#pragma once
#include "LevelComponent.h"
#include "PeterPepperComponent.h"
#include "BurgerComponent.h"
#include "EnemyComponent.h"
#include "Structs.h"
#include "PointsObserver.h"
class GameManagerComponent final : public dae::BaseComponent
{
public:
	GameManagerComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;
private:
	void DeterminePathEnemies(std::vector<std::weak_ptr<EnemyComponent>>& enemies);
	void InitSinglePlayer();
	void CheckBurgerOverlap();
	void CheckPlayerOverlap();
	const bool CheckRectOverlap(Rect rectOne, Rect rectTwo);
	void SpawnHotDog();
	void SpawnEgg();
	void SpawnPickle();
	void HandleFallingBurgers();
	void HandleSalt();
	void Reset();
	std::weak_ptr<dae::Scene> m_pScene;
	std::weak_ptr<LevelComponent> m_pLevel;
	std::weak_ptr<PeterPepperComponent> m_pPlayer;
	std::vector<std::weak_ptr<BurgerComponent>> m_pBurgers;
	std::vector<std::weak_ptr<NodeComponent>> FindPath(std::weak_ptr<NodeComponent> start, std::weak_ptr<NodeComponent> end, std::weak_ptr<NodeComponent> prevNode);
	std::vector<std::weak_ptr<NodeComponent>> m_TempPath;
	std::vector<std::weak_ptr<EnemyComponent>> m_pHotDogs;
	std::vector<std::weak_ptr<EnemyComponent>> m_pEggs;
	std::vector<std::weak_ptr<EnemyComponent>> m_pPickles;
	std::shared_ptr<PointsObserver> m_pPointsObserver;
	const float m_HotDogSpawnCoolDown;
	float m_CurrentHotDogSpawnCoolDown;
	const float m_PickleSpawnCoolDown;
	float m_CurrentPickleSpawnCoolDown;
	const float m_EggSpawnCoolDown;
	float m_currentEggSpawnCoolDown;
};

