#pragma once
#include "BaseComponent.h"
#include <memory>
#include <string>
#include <map>
#include "NodeComponent.h"
#include "structs.h"
class LevelComponent final : public Engine::BaseComponent
{
public:
	LevelComponent(const std::shared_ptr<Engine::GameObject>& owner, const std::string& filePath, int scale, const std::weak_ptr<Engine::Scene>& scene);

	void Update() override;
	void StaticUpdate() override;
	void Render(const Engine::Transform& pos) const override;
	std::pair<int, int> getLevelSize();
	const std::map<std::pair<int, int>, std::weak_ptr<NodeComponent>> GetGrid() const;
	const std::pair<int, int> CoordinateToIndex(const glm::vec2& pos);
	const int GetFloorOffset() const;
	std::vector<std::vector<BurgerInit>> GetBurgerInit() const;

	const std::pair<int, int> GetPlayerOneSpawn() const;
	const std::pair<int, int> GetPlayerTwoSpawn() const;
	const std::pair<int, int> GetHotDogSpawn() const;
	const std::pair<int, int> GetEggSpawn() const;
	const std::pair<int, int> GetPickleSpawn() const;

	void SetNodeOffset(const glm::vec2& pos);
private:
	void ReadFile(const std::string& filePath, int scale, const std::weak_ptr<Engine::Scene>& scene);
	std::map<std::pair<int, int>, std::weak_ptr<NodeComponent>> m_Grid;
	std::pair<int, int> m_GridSize;
	std::pair<int, int> m_EvenTileSize;
	std::pair<int, int> m_OddTileSize;
	std::pair<int, int> m_PlayerOneSpawn;
	std::pair<int, int> m_PlayerTwoSpawn;
	std::pair<int, int> m_HotDogSpawn;
	std::pair<int, int> m_EggSpawn;
	std::pair<int, int> m_PickleSpawn;
	std::vector<std::vector<BurgerInit>> m_BurgerData;
	int m_TileHeightVoid;
	//std::pair<int, int> m_LadderAccessSize;
	int m_FloorOffset;
};

