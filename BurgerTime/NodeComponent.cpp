#include "NodeComponent.h"

NodeComponent::NodeComponent(const std::shared_ptr<dae::GameObject> owner, GridNodeInit init)
	:BaseComponent(owner)
	, m_NodePos{init.nodePos}
	, m_NodeSize{init.nodeSize}
	, m_LadderAccess{init.ladderAccess}
	, m_Floor{init.floor}
{
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

std::vector<std::weak_ptr<NodeComponent>> NodeComponent::GetConnections() const
{
	return m_pConnections;
}

void NodeComponent::AddConnection(const std::weak_ptr<NodeComponent> pConnection)
{
	m_pConnections.push_back(pConnection);
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
