#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "Subject.h"
#include "NodeComponent.h"
#include <string>
#include "PeterPepperComponent.h"
struct AnimDurationInit
{
	float up;
	float down;
	float walk;
	float kill;
	float stun;
	AnimDurationInit(float u, float d, float w, float k, float s)
		: up{u}
		, down{d}
		, walk{w}
		, kill{k}
		, stun{s}
	{}
};
class EnemyComponent final : public dae::BaseComponent
{
public:
	EnemyComponent(const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<NodeComponent>& node, const int floorOffset, std::string textFolder, AnimDurationInit animDurationInit, int points);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;

	const std::weak_ptr<NodeComponent> getNode() const;
	const std::weak_ptr<NodeComponent> getPrevNode() const;
	void Move(std::weak_ptr<NodeComponent> targetNode);
	const bool ReachedChoicePoint() const;
	const std::pair<int, int> GetRectSize() const;
	void Stun();
	void Kill(std::weak_ptr<PeterPepperComponent>& player);
	const bool IsStunned() const;
private:
	//std::weak_ptr<dae::Subject> m_pSubject;
	std::weak_ptr<NodeComponent> m_pCurrentNode;
	std::weak_ptr<NodeComponent> m_pPrevNode;
	std::weak_ptr<NodeComponent> m_pTargetNode;
	const int m_FloorOffset;
	Direction m_Direction;
	bool m_ReachedChoicePoint;
	std::pair<int, int> m_RectSize;
	bool m_Stunned;
	bool m_Dead;
	const float m_StunDuration;
	const float m_DeathDuration;
	float m_ElapsedTime;
	const int m_Points;
};

