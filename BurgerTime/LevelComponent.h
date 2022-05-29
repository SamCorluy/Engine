#pragma once
#include "BaseComponent.h"
#include <memory>
#include <string>
#include <map>
#include "NodeComponent.h"
//struct GridNode
//{
//	std::pair<int, int> nodeSize;
//	std::pair<int, int> nodePos;
//	bool ladderAccess;
//	bool floor;
//};
class LevelComponent final : public dae::BaseComponent
{
public:
	LevelComponent(const std::shared_ptr<dae::GameObject>& owner, const std::string& filePath, int scale, const std::weak_ptr<dae::Scene>& scene);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;
	std::pair<int, int> getLevelSize();
	const std::map<std::pair<int, int>, std::weak_ptr<NodeComponent>> GetGrid() const;
	const std::pair<int, int> CoordinateToIndex(const glm::vec2& pos);
	const int GetFloorOffset() const;

	void SetNodeOffset(const glm::vec2& pos);
private:
	void ReadFile(const std::string& filePath, int scale, const std::weak_ptr<dae::Scene>& scene);
	std::map<std::pair<int, int>, std::weak_ptr<NodeComponent>> m_Grid;
	std::pair<int, int> m_GridSize;
	std::pair<int, int> m_EvenTileSize;
	std::pair<int, int> m_OddTileSize;
	//std::pair<int, int> m_LadderAccessSize;
	int m_FloorOffset;
};

