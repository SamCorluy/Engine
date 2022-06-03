#pragma once
#include "BaseComponent.h"
#include "PeterPepperComponent.h"
//enum class Action
//{
//	WalkingLeft,
//	WalkingRight,
//	ClimbingUp,
//	ClimbingDown
//};
class MovementComponent final : public dae::BaseComponent
{
public:
	MovementComponent(const std::shared_ptr<dae::GameObject>& owner);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	void Move(Direction dir);
private:
	void MovePlayer(Direction dir);
	void MoveEnemy(Direction action);
};

