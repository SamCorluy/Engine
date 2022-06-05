#pragma once
#include<memory>
#include "BaseComponent.h"
#include "Transform.h"
#include "NodeComponent.h"
#include "LevelComponent.h"
#include "PeterPepperComponent.h"
#include <vector>
#include "Structs.h"

struct IngredientPartition
{
	bool hasDropped;
	glm::vec2 botLeft;
};
class IngredientComponent final : public Engine::BaseComponent
{
public:
	IngredientComponent(std::pair<int, int> idx, const std::shared_ptr<Engine::GameObject>& owner, int scale, const std::weak_ptr<LevelComponent>& level, IngredientType type);
	virtual void Update() override;
	virtual void StaticUpdate() override;
	virtual void Render(const Engine::Transform& pos) const override;
	const int GetTextureHeight() const;
	const bool hasDropped() const;
	void setDropped(bool dropped, std::weak_ptr<PeterPepperComponent>& player);
	void InitiateDrop(std::weak_ptr<PeterPepperComponent>& player, int extraDrops = 0);
	bool CheckOverlap(std::weak_ptr<PeterPepperComponent>& player, size_t idx);
	const std::weak_ptr<NodeComponent> GetNode() const;
	const std::weak_ptr<NodeComponent> GetStartNode() const;
	const int GetBurgerOffset() const;
	const std::weak_ptr<PeterPepperComponent> GetPlayer() const;
	//void SetExtraDrops(int extraDrops);
	const std::pair<int, int> GetRectSize() const;
	void SetReachedPlate();
	const bool HasReachedPlate() const;
private:
	//std::weak_ptr<dae::Subject> m_pSubject;
	std::vector<IngredientPartition> m_pPartitions;
	//std::weak_ptr<PeterPepperComponent> m_pPlayer;
	std::weak_ptr<NodeComponent> m_pNode;
	std::weak_ptr<NodeComponent> m_pStartNode;
	std::vector<glm::vec2> m_PrevPlayerPos{};
	//const int m_TextureSize;
	int m_TextureHeight;
	const int m_DropHeight;
	int m_pBurgerOffset;
	std::weak_ptr<PeterPepperComponent> m_pPlayer;
	int m_ExtraDrops;
	std::pair<int, int> m_RectSize;
	bool m_ReachedPlate = false;
};

