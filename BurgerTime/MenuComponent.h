#pragma once
#include "BaseComponent.h"
#include "Scene.h"
#include "GameManagerComponent.h"
class MenuComponent final : public dae::BaseComponent
{
public:
	MenuComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;

	void SelectGameMode(GameModes gameMode);
private:
	std::weak_ptr<dae::Scene> m_pScene;
	std::weak_ptr<dae::Scene> m_pGameScene;
	std::weak_ptr<GameManagerComponent> m_pGame;
};

