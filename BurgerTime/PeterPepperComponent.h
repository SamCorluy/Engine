#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "Subject.h"
#include "LevelComponent.h"
#include <memory>
enum class Action
{
	WalkingLeft,
	WalkingRight,
	ClimbingUp,
	ClimbingDown
};
class PeterPepperComponent final : public dae::BaseComponent
{
public:
	PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	void Move(Action action);
private:
	std::weak_ptr<dae::Subject> m_pSubject;
	bool m_MovementProcessed;
	std::weak_ptr<LevelComponent> m_pLevel;
};

