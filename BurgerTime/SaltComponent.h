#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "Subject.h"
#include "NodeComponent.h"
class SaltComponent final : public dae::BaseComponent
{
public:
	SaltComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, glm::vec2 pos, const int floorOffset, Direction direction);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	//const std::weak_ptr<NodeComponent> getNode() const;
	const bool IsDone() const;
	const std::pair<int, int> GetRectSize() const;
private:
	std::weak_ptr<NodeComponent> m_pNode;
	const int m_FloorOffset;
	const float m_Duration;
	float m_ElapsedTime;
	std::pair<int, int> m_RectSize;
};
