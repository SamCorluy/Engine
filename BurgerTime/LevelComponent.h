#pragma once
#include "BaseComponent.h"
#include <memory>
#include <string>
#include <map>
struct GridNode
{
	bool ladderAccess;
	bool floor;
};
class LevelComponent final : public dae::BaseComponent
{
public:
	LevelComponent(const std::shared_ptr<dae::GameObject>& owner, const std::string& filePath);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;
private:
	void ReadFile(const std::string& filePath);
	std::map<std::pair<int, int>, GridNode> m_Grid;
	std::pair<int, int> m_GridSize;
	std::pair<int, int> m_TileSize;
};

