#pragma once
#include "BaseComponent.h"
#include "Transform.h"
class SaltDisplayComponent final : public Engine::BaseComponent
{
public:
	SaltDisplayComponent(const std::shared_ptr<Engine::GameObject>& owner, int scale, int charges);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const Engine::Transform& pos) const override;

	void RemoveCharge() const;
private:
	std::pair<int, int> m_RectSize;
};

