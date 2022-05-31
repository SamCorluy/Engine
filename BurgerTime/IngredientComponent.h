#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "NodeComponent.h"
#include "LevelComponent.h"
#include "PeterPepperComponent.h"
#include <vector>

struct IngredientPartition
{
	bool hasDropped;
	glm::vec2 botLeft;
};
enum class IngredientType
{
	BunTop,
	BunBot,
	Patty,
	Lettuce,
	Cheese,
	Tomato
};
class IngredientComponent final : public dae::BaseComponent
{
public:
	IngredientComponent(std::pair<int, int> idx, const std::shared_ptr<dae::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level, IngredientType type);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const dae::Transform& pos) const override;
	const int GetTextureHeight() const;
	const bool hasDropped() const;
	void setDropped(bool dropped, int playerIdx);
	void InitiateDrop(int playerIdx);
	bool CheckOverlap(std::weak_ptr<PeterPepperComponent>& player, int playerIdx);
	const std::weak_ptr<NodeComponent> GetNode() const;
	const std::weak_ptr<NodeComponent> GetStartNode() const;
	const int GetBurgerOffset() const;
	const int GetPlayerIdx() const;
	void SetExtraDrops(int extraDrops);
	const std::pair<int, int> GetRectSize() const;
private:
	std::vector<IngredientPartition> m_pPartitions;
	//std::weak_ptr<PeterPepperComponent> m_pPlayer;
	std::weak_ptr<NodeComponent> m_pNode;
	std::weak_ptr<NodeComponent> m_pStartNode;
	glm::vec2 m_PrevPlayerPos{};
	//const int m_TextureSize;
	int m_TextureHeight;
	const int m_DropHeight;
	int m_pBurgerOffset;
	int m_PlayerIdx;
	int m_ExtraDrops;
	std::pair<int, int> m_RectSize;
};

