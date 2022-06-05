#pragma once
#include "BaseComponent.h"
#include "Scene.h"
#include "TextComponent.h"
class HighScoreMenuComponent final : public Engine::BaseComponent
{
public:
	HighScoreMenuComponent(const std::shared_ptr<Engine::GameObject> owner, const std::weak_ptr<Engine::Scene>& scene, std::vector<int> scores);

	void Update() override;
	void StaticUpdate() override;
	void Render(const Engine::Transform& pos) const override;

	void BackToMenu();
	void InitMenu(std::vector<int> scores);
private:
	std::weak_ptr<Engine::Scene> m_pScene;
	std::vector<std::weak_ptr<Engine::TextComponent>> m_pPlayerTitles;
	std::vector<std::weak_ptr<Engine::TextComponent>> m_pPlayerScores;
	std::vector<std::weak_ptr<Engine::TextComponent>> m_pHighScores;
	bool m_controlsAdded = false;
};

