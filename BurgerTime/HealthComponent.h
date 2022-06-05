#pragma once
#include<memory>
#include "Transform.h"
#include "BaseComponent.h"

class HealthComponent final : public Engine::BaseComponent
{
public:
	HealthComponent(const std::shared_ptr<Engine::GameObject>& owner, int scale, int health);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const Engine::Transform& pos) const override;

	void RemoveLife() const;
private:
	std::pair<int, int> m_RectSize;
};
