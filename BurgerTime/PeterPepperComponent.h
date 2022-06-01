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
	PeterPepperComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, const std::weak_ptr<dae::Scene>& scene, int lives);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	const std::weak_ptr<NodeComponent> getNode() const;
	const std::weak_ptr<SaltComponent> GetSalt() const;
	void Move(Action action);
	void ThrowSalt();
	void Die();
	void Reset();
	bool IsDead();
	bool DeathAnimationFinished();
	const std::pair<int, int> GetRectSize() const;
private:
	std::weak_ptr<dae::Subject> m_pSubject;
	bool m_MovementProcessed;
	std::weak_ptr<NodeComponent> m_pCurrentNode;
	const std::weak_ptr<NodeComponent> m_pStartNode;
	const int m_FloorOffset;
	const float m_SaltCooldown;
	const float m_ThrowDuration;
	const float m_DieDuration;
	float m_ElapsedTime;
	bool m_ThrowingSalt;
	bool m_CanThrow;
	bool m_Dead;
	Direction m_Direction;
	std::weak_ptr<SaltComponent> m_pSaltComponent;
	std::weak_ptr<dae::Scene> m_pScene;
	std::pair<int, int> m_RectSize;
	int m_Lives;
};

