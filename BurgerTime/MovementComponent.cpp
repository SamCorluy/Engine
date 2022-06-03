#include "MovementComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "EnemyComponent.h"
#include "ElapsedTime.h"

MovementComponent::MovementComponent(const std::shared_ptr<dae::GameObject>& owner)
	: BaseComponent(owner)
{
}

void MovementComponent::Update()
{
}

void MovementComponent::StaticUpdate()
{
}

void MovementComponent::Render(const dae::Transform&) const
{
}

void MovementComponent::Move(Direction dir)
{
	if (!GetOwner().lock()->GetComponent<PeterPepperComponent>().expired())
	{
		MovePlayer(dir);
	}
	else if (!GetOwner().lock()->GetComponent<EnemyComponent>().expired())
	{
		MoveEnemy(dir);
	}
}

void MovementComponent::MovePlayer(Direction dir)
{
	auto player = GetOwner().lock()->GetComponent<PeterPepperComponent>();
	if (player.lock()->IsThrowingSalt() || player.lock()->IsDead())
		return;
	auto rectSize = GetOwner().lock()->GetComponent<dae::AnimationComponent>().lock()->getActiveAnimRec();
	//auto grid = m_pLevel.lock()->GetGrid();
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	auto levelPos = player.lock()->getNode().lock()->GetOwner().lock()->GetTransform().GetPosition();
	//auto idx = m_pLevel.lock()->CoordinateToIndex(pos);
	auto ladderRect = player.lock()->getNode().lock()->GetLadderAccessSize();
	//auto offset = m_pCurrentNode.lock()->GetFloorOffset();
	switch (dir)
	{
	case Direction::LEFT:
		if ((player.lock()->getNode().lock()->GetConnection(dir).expired() || !player.lock()->getNode().lock()->GetConnection(dir).lock()->IsFloor())
			&& (levelPos.x + player.lock()->getNode().lock()->GetNodePos().first) >= (pos.x - rectSize.x / 2.f))
		{
			pos.x = levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (!player.lock()->getNode().lock()->IsFloor() || pos.y > (levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset() + ladderRect.second))
			return;
		if (pos.y > levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset() || pos.y < levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset())
			pos.y = levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset();
		comp.lock()->SetActiveAnimation(0);
		player.lock()->SetDirection(dir);
		comp.lock()->SetFlip(false);
		pos.x -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x < player.lock()->getNode().lock()->GetNodePos().first + levelPos.x)
			player.lock()->SetCurrentNode(player.lock()->getNode().lock()->GetConnection(Direction::LEFT));
		GetOwner().lock()->SetPosition(pos);
		break;
	case Direction::RIGHT:
		if ((player.lock()->getNode().lock()->GetConnection(dir).expired() || !player.lock()->getNode().lock()->GetConnection(dir).lock()->IsFloor())
			&& (levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + player.lock()->getNode().lock()->GetNodeSize().first) <= (pos.x + rectSize.x / 2.f))
		{
			pos.x = levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + player.lock()->getNode().lock()->GetNodeSize().first - rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (!player.lock()->getNode().lock()->IsFloor() || pos.y > (levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset() + ladderRect.second))
			return;
		if (pos.y > levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset() || pos.y < levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset())
			pos.y = levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset();
		comp.lock()->SetActiveAnimation(0);
		player.lock()->SetDirection(dir);
		comp.lock()->SetFlip(true);
		pos.x += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x > player.lock()->getNode().lock()->GetNodePos().first + levelPos.x + player.lock()->getNode().lock()->GetNodeSize().first)
			player.lock()->SetCurrentNode(player.lock()->getNode().lock()->GetConnection(dir));
		GetOwner().lock()->SetPosition(pos);
		break;
	case Direction::UP:
		if ((player.lock()->getNode().lock()->GetConnection(dir).expired() || !player.lock()->getNode().lock()->GetConnection(dir).lock()->HasLadderAccess())
			&& (levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset()) >= pos.y)
		{
			pos.y = levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset();
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + (player.lock()->getNode().lock()->GetNodeSize().first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + (player.lock()->getNode().lock()->GetNodeSize().first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(2);
			player.lock()->SetDirection(dir);
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + player.lock()->getNode().lock()->GetNodeSize().first / 2;
			pos.y += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			if (pos.y > player.lock()->getNode().lock()->GetNodePos().second + levelPos.y + player.lock()->getNode().lock()->GetNodeSize().second)
				player.lock()->SetCurrentNode(player.lock()->getNode().lock()->GetConnection(dir));
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	case Direction::DOWN:
		if ((player.lock()->getNode().lock()->GetConnection(dir).expired() || !player.lock()->getNode().lock()->GetConnection(dir).lock()->HasLadderAccess())
			&& (levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset()) >= pos.y)
		{
			pos.y = levelPos.y + player.lock()->getNode().lock()->GetNodePos().second + player.lock()->GetFloorOffset();
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + (player.lock()->getNode().lock()->GetNodeSize().first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + (player.lock()->getNode().lock()->GetNodeSize().first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(1);
			player.lock()->SetDirection(dir);
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + player.lock()->getNode().lock()->GetNodePos().first + player.lock()->getNode().lock()->GetNodeSize().first / 2;
			pos.y -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			if (pos.y < player.lock()->getNode().lock()->GetNodePos().second + levelPos.y)
				player.lock()->SetCurrentNode(player.lock()->getNode().lock()->GetConnection(dir));
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	default:
		break;
	}
	player.lock()->SetMovementProcessed(true);
}

void MovementComponent::MoveEnemy(Direction dir)
{
	auto enemy = GetOwner().lock()->GetComponent<EnemyComponent>();
	if (enemy.lock()->IsDead() || enemy.lock()->IsStunned())
		return;
	auto rectSize = GetOwner().lock()->GetComponent<dae::AnimationComponent>().lock()->getActiveAnimRec();
	//auto grid = m_pLevel.lock()->GetGrid();
	auto comp = GetOwner().lock()->GetComponent<dae::AnimationComponent>();
	auto pos = GetOwner().lock()->GetTransform().GetPosition();
	auto levelPos = enemy.lock()->getNode().lock()->GetOwner().lock()->GetTransform().GetPosition();
	//auto idx = m_pLevel.lock()->CoordinateToIndex(pos);
	auto ladderRect = enemy.lock()->getNode().lock()->GetLadderAccessSize();
	//auto offset = m_pCurrentNode.lock()->GetFloorOffset();
	switch (dir)
	{
	case Direction::LEFT:
		if ((enemy.lock()->getNode().lock()->GetConnection(dir).expired() || !enemy.lock()->getNode().lock()->GetConnection(dir).lock()->IsFloor())
			&& (levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first) >= (pos.x - rectSize.x / 2.f))
		{
			pos.x = levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (!enemy.lock()->getNode().lock()->IsFloor() || pos.y > (levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset() + ladderRect.second))
			return;
		if (pos.y > levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset() || pos.y < levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset())
			pos.y = levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset();
		comp.lock()->SetActiveAnimation(0);
		enemy.lock()->SetDirection(dir);
		comp.lock()->SetFlip(false);
		pos.x -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x < enemy.lock()->getNode().lock()->GetNodePos().first + levelPos.x)
			enemy.lock()->SetCurrentNode(enemy.lock()->getNode().lock()->GetConnection(Direction::LEFT));
		GetOwner().lock()->SetPosition(pos);
		break;
	case Direction::RIGHT:
		if ((enemy.lock()->getNode().lock()->GetConnection(dir).expired() || !enemy.lock()->getNode().lock()->GetConnection(dir).lock()->IsFloor())
			&& (levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + enemy.lock()->getNode().lock()->GetNodeSize().first) <= (pos.x + rectSize.x / 2.f))
		{
			pos.x = levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + enemy.lock()->getNode().lock()->GetNodeSize().first - rectSize.x / 2;
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		if (!enemy.lock()->getNode().lock()->IsFloor() || pos.y > (levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset() + ladderRect.second))
			return;
		if (pos.y > levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset() || pos.y < levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset())
			pos.y = levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset();
		comp.lock()->SetActiveAnimation(0);
		enemy.lock()->SetDirection(dir);
		comp.lock()->SetFlip(true);
		pos.x += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
		if (pos.x > enemy.lock()->getNode().lock()->GetNodePos().first + levelPos.x + enemy.lock()->getNode().lock()->GetNodeSize().first)
			enemy.lock()->SetCurrentNode(enemy.lock()->getNode().lock()->GetConnection(dir));
		GetOwner().lock()->SetPosition(pos);
		break;
	case Direction::UP:
		if ((enemy.lock()->getNode().lock()->GetConnection(dir).expired() || !enemy.lock()->getNode().lock()->GetConnection(dir).lock()->HasLadderAccess())
			&& (levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset()) >= pos.y)
		{
			pos.y = levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset();
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + (enemy.lock()->getNode().lock()->GetNodeSize().first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + (enemy.lock()->getNode().lock()->GetNodeSize().first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(2);
			enemy.lock()->SetDirection(dir);
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + enemy.lock()->getNode().lock()->GetNodeSize().first / 2;
			pos.y += 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			if (pos.y > enemy.lock()->getNode().lock()->GetNodePos().second + levelPos.y + enemy.lock()->getNode().lock()->GetNodeSize().second)
				enemy.lock()->SetCurrentNode(enemy.lock()->getNode().lock()->GetConnection(dir));
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	case Direction::DOWN:
		if ((enemy.lock()->getNode().lock()->GetConnection(dir).expired() || !enemy.lock()->getNode().lock()->GetConnection(dir).lock()->HasLadderAccess())
			&& (levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset()) >= pos.y)
		{
			pos.y = levelPos.y + enemy.lock()->getNode().lock()->GetNodePos().second + enemy.lock()->GetFloorOffset();
			GetOwner().lock()->SetPosition(pos);
			return;
		}
		else if ((levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + (enemy.lock()->getNode().lock()->GetNodeSize().first - ladderRect.first) / 2) <= (pos.x)
			&& (levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + (enemy.lock()->getNode().lock()->GetNodeSize().first + ladderRect.first) / 2) >= (pos.x))
		{
			comp.lock()->SetActiveAnimation(1);
			enemy.lock()->SetDirection(dir);
			comp.lock()->SetFlip(false);
			pos.x = levelPos.x + enemy.lock()->getNode().lock()->GetNodePos().first + enemy.lock()->getNode().lock()->GetNodeSize().first / 2;
			pos.y -= 100.f * ElapsedTime::GetInstance().GetElapsedTime();
			if (pos.y < enemy.lock()->getNode().lock()->GetNodePos().second + levelPos.y)
				enemy.lock()->SetCurrentNode(enemy.lock()->getNode().lock()->GetConnection(dir));
			GetOwner().lock()->SetPosition(pos);
		}
		break;
	default:
		break;
	}
}
