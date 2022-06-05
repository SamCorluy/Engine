#pragma once
#include "BaseComponent.h"
#include "Scene.h"
#include "GameManagerComponent.h"
#include "TextureManagerComponent.h"
class MenuComponent final : public Engine::BaseComponent
{
public:
	MenuComponent(const std::shared_ptr<Engine::GameObject> owner, const std::weak_ptr<Engine::Scene>& scene);

	void Update() override;
	void StaticUpdate() override;
	void Render(const Engine::Transform& pos) const override;

	void SelectGameMode();
	void MoveUp();
	void MoveDown();
	void InitMenu();
private:
	std::weak_ptr<Engine::Scene> m_pScene;
	std::weak_ptr<Engine::Scene> m_pGameScene;
	std::weak_ptr<Engine::Scene> m_pHighScoreScene;
	std::weak_ptr<Engine::Scene> m_pActiveScene;
	std::weak_ptr<GameManagerComponent> m_pGame;
	bool m_IsInit = false;

	std::weak_ptr<Engine::TextComponent> m_pSinglePlayer;
	std::weak_ptr<Engine::TextComponent> m_pCoop;
	std::weak_ptr<Engine::TextComponent> m_pPVP;

	std::weak_ptr<Engine::TextureManagerComponent> m_pSelector;
	GameModes m_CurrentGameMode;
};

