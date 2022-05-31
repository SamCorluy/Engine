#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "Subject.h"
#include "NodeComponent.h"
#include "SaltComponent.h"
#include "Scene.h"
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
	PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, const std::weak_ptr<dae::Scene>& scene);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	const std::weak_ptr<NodeComponent> getNode() const;
	void Move(Action action);
	void ThrowSalt();
private:
	std::weak_ptr<dae::Subject> m_pSubject;
	bool m_MovementProcessed;
	std::weak_ptr<NodeComponent> m_pCurrentNode;
	const int m_FloorOffset;
	const float m_SaltCooldown;
	const float m_ThrowDuration;
	float m_ElapsedTime;
	bool m_ThrowingSalt;
	bool m_CanThrow;
	Direction m_Direction;
	std::weak_ptr<SaltComponent> m_pSaltComponent;
	std::weak_ptr<dae::Scene> m_pScene;
};

