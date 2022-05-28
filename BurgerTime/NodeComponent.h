#pragma once
#include "BaseComponent.h"
#include "GameObject.h"
struct GridNodeInit
{
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

	std::vector<std::weak_ptr<NodeComponent>> GetConnections() const;
	void AddConnection(const std::weak_ptr<NodeComponent> pConnection);

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
};

