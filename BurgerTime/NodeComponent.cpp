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
	if(!m_pConnections[static_cast<int>(dir)].expired())
		if(((dir == Direction::LEFT || dir == Direction::RIGHT) && m_pConnections[static_cast<int>(dir)].lock()->m_Floor && m_Floor)
			|| ((dir == Direction::UP || dir == Direction::DOWN) && m_pConnections[static_cast<int>(dir)].lock()->m_LadderAccess && m_LadderAccess))
			return m_pConnections[static_cast<int>(dir)];
	return std::shared_ptr<NodeComponent>(nullptr);
}

std::weak_ptr<NodeComponent> NodeComponent::GetConnectionIgnoringWalkable(Direction dir) const
{
	return m_pConnections[static_cast<int>(dir)];
}

std::vector<std::weak_ptr<NodeComponent>> NodeComponent::GetConnections() const
{
	std::vector<std::weak_ptr<NodeComponent>> temp;
	for (size_t i = 0; i < m_pConnections.size(); ++i)
	{
		if (!GetConnection((Direction)i).expired())
			temp.push_back(m_pConnections[i]);
	}
	//for (auto con : m_pConnections)
	//{
	//	if (!con.expired() && (con.lock()->m_LadderAccess || con.lock()->m_Floor))
	//		temp.push_back(con);
	//}
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
