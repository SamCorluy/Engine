#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "Subject.h"
enum class Action
{
	Walking,
	Idle,
	ClimbingUp,
	ClimbingDown
};
class PeterPepperComponent final : public dae::BaseComponent
{
public:
	PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, glm::vec2 pos);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	void Move(Action action);
private:
	std::weak_ptr<dae::Subject> m_pSubject;
	glm::vec2 currentPos;
	bool m_MovementProcessed;
};

