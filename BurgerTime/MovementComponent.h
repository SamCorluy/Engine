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
class MovementComponent final : public Engine::BaseComponent
{
public:
	MovementComponent(const std::shared_ptr<Engine::GameObject>& owner);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const Engine::Transform& pos) const override;

	void Move(Direction dir);
private:
	void MovePlayer(Direction dir);
	void MoveEnemy(Direction action);
};

