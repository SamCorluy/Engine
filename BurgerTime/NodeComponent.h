#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
enum class Direction
{
	LEFT = 0,
	RIGHT = 1,
	UP = 2,
	DOWN = 3
};
struct GridNodeInit
{
	std::pair<int, int> ladderAccessSize;
	std::pair<int, int> nodeSize;
	std::pair<int, int> nodePos;
	bool ladderAccess;
	bool floor;
};
class NodeComponent final : public dae::BaseComponent
{
public:
	NodeComponent(const std::shared_ptr<dae::GameObject> owner, GridNodeInit init);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;

	std::weak_ptr<NodeComponent> GetConnection(Direction dir) const;
	std::weak_ptr<NodeComponent> GetConnectionIgnoringWalkable(Direction dir) const;
	std::vector<std::weak_ptr<NodeComponent>> GetConnections() const;
	void SetConnection(const std::weak_ptr<NodeComponent> pConnection, Direction dir);

	const std::pair<int, int> GetLadderAccessSize() const;
	const std::pair<int, int> GetNodePos() const;
	const std::pair<int, int> GetNodeSize() const;
	const bool IsFloor() const;
	const bool HasLadderAccess() const;
private:
	std::vector<std::weak_ptr<NodeComponent>> m_pConnections;
	std::pair<int, int> m_NodeSize;
	std::pair<int, int> m_NodePos;
	bool m_LadderAccess;
	bool m_Floor;
	std::pair<int, int> m_LadderAccessSize;
};

