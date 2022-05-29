#include "NodeComponent.h"

NodeComponent::NodeComponent(const std::shared_ptr<dae::GameObject> owner, GridNodeInit init)
	:BaseComponent(owner)
	, m_NodePos{init.nodePos}
	, m_NodeSize{init.nodeSize}
	, m_LadderAccess{init.ladderAccess}
	, m_Floor{init.floor}
	, m_LadderAccessSize{init.ladderAccessSize}
{
	for (size_t i = 0; i < 4; ++i)
	{
		m_pConnections.push_back(std::shared_ptr<NodeComponent>(nullptr));
	}
}

void NodeComponent::Update()
{
}

void NodeComponent::StaticUpdate()
{
}

void NodeComponent::Render(const dae::Transform&) const
{
}

std::weak_ptr<NodeComponent> NodeComponent::GetConnection(Direction dir) const
{
	return m_pConnections[static_cast<int>(dir)];
}

std::vector<std::weak_ptr<NodeComponent>> NodeComponent::GetConnections() const
{
	std::vector<std::weak_ptr<NodeComponent>> temp;
	for (auto con : m_pConnections)
	{
		if (!con.expired())
			temp.push_back(con);
	}
	return temp;
}

void NodeComponent::SetConnection(const std::weak_ptr<NodeComponent> pConnection, Direction dir)
{
	m_pConnections[static_cast<int>(dir)] = pConnection;
}

const std::pair<int, int> NodeComponent::GetNodePos() const
{
	return m_NodePos;
}

const std::pair<int, int> NodeComponent::GetNodeSize() const
{
	return m_NodeSize;
}

const bool NodeComponent::IsFloor() const
{
	return m_Floor;
}

const bool NodeComponent::HasLadderAccess() const
{
	return m_LadderAccess;
}

const std::pair<int, int> NodeComponent::GetLadderAccessSize() const
{
	return m_LadderAccessSize;
}
