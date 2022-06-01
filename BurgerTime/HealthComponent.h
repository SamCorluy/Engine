#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
class HealthComponent final : public dae::BaseComponent
{
public:
	HealthComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, int health);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	//const std::weak_ptr<NodeComponent> getNode() const;
	void RemoveLife() const;
private:
	std::pair<int, int> m_RectSize;
};
