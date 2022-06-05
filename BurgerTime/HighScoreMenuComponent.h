#pragma once
#include "BaseComponent.h"
#include "Scene.h"
#include "TextComponent.h"
class HighScoreMenuComponent final : public dae::BaseComponent
{
public:
	HighScoreMenuComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene, std::vector<int> scores);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;

	void BackToMenu();
	void InitMenu(std::vector<int> scores);
private:
	std::weak_ptr<dae::Scene> m_pScene;
	std::vector<std::weak_ptr<dae::TextComponent>> m_pPlayerTitles;
	std::vector<std::weak_ptr<dae::TextComponent>> m_pPlayerScores;
	std::vector<std::weak_ptr<dae::TextComponent>> m_pHighScores;
	bool m_controlsAdded = false;
};

