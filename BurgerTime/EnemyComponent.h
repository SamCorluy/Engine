#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "Subject.h"
#include "NodeComponent.h"
#include <string>
class EnemyComponent final : public dae::BaseComponent
{
public:
	EnemyComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, std::string textFolder);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	const std::weak_ptr<NodeComponent> getNode() const;
	const std::weak_ptr<NodeComponent> getPrevNode() const;
	void Move(std::weak_ptr<NodeComponent> targetNode);
	const bool ReachedChoicePoint() const;
private:
	std::weak_ptr<dae::Subject> m_pSubject;
	std::weak_ptr<NodeComponent> m_pCurrentNode;
	std::weak_ptr<NodeComponent> m_pPrevNode;
	std::weak_ptr<NodeComponent> m_pTargetNode;
	const int m_FloorOffset;
	Direction m_Direction;
	bool m_ReachedChoicePoint;
};

