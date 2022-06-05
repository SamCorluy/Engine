#pragma once
#include "BaseComponent.h"
#include "Scene.h"
#include "GameManagerComponent.h"
#include "TextureManagerComponent.h"
class MenuComponent final : public dae::BaseComponent
{
public:
	MenuComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;

	void SelectGameMode();
	void MoveUp();
	void MoveDown();
	void InitMenu();
private:
	std::weak_ptr<dae::Scene> m_pScene;
	std::weak_ptr<dae::Scene> m_pGameScene;
	std::weak_ptr<dae::Scene> m_pHighScoreScene;
	std::weak_ptr<dae::Scene> m_pActiveScene;
	std::weak_ptr<GameManagerComponent> m_pGame;

	std::weak_ptr<dae::TextComponent> m_pSinglePlayer;
	std::weak_ptr<dae::TextComponent> m_pCoop;
	std::weak_ptr<dae::TextComponent> m_pPVP;

	std::weak_ptr<dae::TextureManagerComponent> m_pSelector;
	GameModes m_CurrentGameMode;
};

