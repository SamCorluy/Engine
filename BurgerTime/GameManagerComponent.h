#pragma once
#include "LevelComponent.h"
#include "PeterPepperComponent.h"
#include "BurgerComponent.h"
#include "EnemyComponent.h"
#include "Structs.h"
#include "PointsObserver.h"
#include "HealthComponent.h"
#include "SaltDisplayComponent.h"
enum class GameModes
{
	SINGLEPLAYER,
	COOP,
	PVP
};
class GameManagerComponent final : public dae::BaseComponent
{
public:
	GameManagerComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene, GameModes gameMode);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;
private:
	void DeterminePathEnemies(std::vector<std::weak_ptr<EnemyComponent>>& enemies);
	void InitSinglePlayer();
	void InitCoop();
	void InitPvp();
	void CheckBurgerOverlap();
	void CheckPlayerOverlap();
	const bool CheckRectOverlap(Rect rectOne, Rect rectTwo);

	//Enemy spawn functions
	void SpawnHotDog();
	void SpawnEgg();
	void SpawnPickle();

	//Initialize level
	void InitLevel(size_t index);

	//Handle enemies hit by falling burgers
	void HandleFallingBurgers();
	//Handle enemies hit by salt
	void HandleSalt();
	//ResetLevel
	void Reset();
	
	void InitPlayer(size_t idx, size_t totalPlayers);

	void NextLevel();

	void InitBurgers();

	void SpawnPlayerControlledHotDog();

	void EndGame();

	void ClearEnemies();

	void ClearLevel();

	void ClearBurgers();

	void ClearPlayers();
	std::weak_ptr<dae::Scene> m_pScene;
	std::weak_ptr<LevelComponent> m_pLevel;
	std::vector<std::weak_ptr<PeterPepperComponent>> m_pPlayers;
	std::vector<std::weak_ptr<BurgerComponent>> m_pBurgers;
	std::vector<std::weak_ptr<NodeComponent>> FindPath(std::weak_ptr<NodeComponent> start, std::weak_ptr<NodeComponent> end, std::weak_ptr<NodeComponent> prevNode);
	std::vector<std::weak_ptr<NodeComponent>> m_TempPath;
	std::vector<std::weak_ptr<EnemyComponent>> m_pHotDogs;
	std::vector<std::weak_ptr<EnemyComponent>> m_pEggs;
	std::vector<std::weak_ptr<EnemyComponent>> m_pPickles;
	std::weak_ptr<EnemyComponent> m_pPlayerControlledEnemy;
	std::vector<std::weak_ptr<CounterComponent>> m_pScoreComponents;
	std::vector<std::pair<std::weak_ptr<HealthComponent>, std::weak_ptr<SaltDisplayComponent>>> m_pHudElements;
	//std::shared_ptr<PointsObserver> m_pPointsObserver;
	const float m_HotDogSpawnCoolDown;
	float m_CurrentHotDogSpawnCoolDown;
	const float m_PickleSpawnCoolDown;
	float m_CurrentPickleSpawnCoolDown;
	const float m_EggSpawnCoolDown;
	float m_currentEggSpawnCoolDown;
	std::vector<std::string> m_LevelFolders;
	size_t m_CurrentLevel = 0;
};

