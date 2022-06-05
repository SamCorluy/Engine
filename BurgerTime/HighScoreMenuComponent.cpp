#include "HighScoreMenuComponent.h"
#include "BaseComponent.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include <regex>
#include <fstream>
#include "TextComponent.h"
#include "InputManager.h"
#include "HighscoreCommand.h"
HighScoreMenuComponent::HighScoreMenuComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene, std::vector<int> scores)
	: dae::BaseComponent(owner)
	, m_pScene{scene}
{
	InitMenu(scores);

}

void HighScoreMenuComponent::Update()
{
	if (!m_controlsAdded)
	{
		dae::InputManager::GetInstance().AddKeyboardInput('\r', dae::InputType::Press, std::make_shared<HighscoreCommand>(GetOwner().lock()->GetComponent<HighScoreMenuComponent>()));
		m_controlsAdded = true;
	}
}

void HighScoreMenuComponent::StaticUpdate()
{
}

void HighScoreMenuComponent::Render(const dae::Transform&) const
{
}

void HighScoreMenuComponent::BackToMenu()
{
	for (auto title : m_pPlayerTitles)
		if (!title.expired())
			title.lock()->GetOwner().lock()->Remove();
	for (auto score : m_pPlayerScores)
		if (!score.expired())
			score.lock()->GetOwner().lock()->Remove();
	for (auto score : m_pHighScores)
		if (!score.expired())
			score.lock()->GetOwner().lock()->Remove();

	GetOwner().lock()->Remove();

	dae::InputManager::GetInstance().RemoveKeys();
	dae::SceneManager::GetInstance().SetActiveScene("Menu");
}

void HighScoreMenuComponent::InitMenu(std::vector<int> scores)
{
	std::regex ScoreRegex{ "(\\d+)" };
	std::smatch matches{};
	std::ifstream in{ "../Data/HighScores.txt" };
	std::vector<int> highScores;
	if (!in)
		return;
	while (in)
	{
		std::string line;
		std::getline(in, line);
		if (std::regex_match(line, ScoreRegex))
		{
			std::regex_search(line, matches, ScoreRegex);
			highScores.push_back(std::stoi(matches[1]));
		}
	}
	auto size = highScores.size();
	if (size < 3)
		for (size_t i = 0; i < 3 - size; ++i)
			highScores.push_back(0);
	if (scores.size() >= 1)
		highScores.push_back(scores[0]);
	if (scores.size() > 1)
		highScores.push_back(scores[1]);

	std::sort(highScores.begin(), highScores.end(), [](int& value, int& value2) { return value > value2; });
	size = highScores.size();
	for (size_t i = 0; i < size - 3; ++i)
		highScores.pop_back();

	std::ofstream ofs;
	ofs.open("../Data/HighScores.txt", std::ofstream::out | std::ofstream::trunc);
	ofs << highScores[0] << std::endl << highScores[1] << std::endl << highScores[2];
	ofs.close();


	// Get fonts ready
	auto font = dae::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 20);

	// Get window size
	auto window = dae::Renderer::GetInstance().GetWindow();
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	glm::vec2 pos{};

	auto obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<dae::TextComponent>(std::make_shared<dae::TextComponent>(obj, "PLAYER 1:", font));
	auto textSize = obj->GetComponent<dae::TextComponent>().lock()->GetTextSize();
	if(scores.size() == 1)
		pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	else
		pos.x = static_cast<float>((windowWidth / 2.f) - textSize.first) / 2.f;
	pos.y = static_cast<float>(windowHeight - textSize.second) - 20.f;
	obj->SetPosition(pos);
	m_pPlayerTitles.push_back(obj->GetComponent<dae::TextComponent>());
	m_pScene.lock()->Add(obj);
	if (scores.size() > 1)
	{
		obj = std::make_shared<dae::GameObject>();
		obj->AddComponent<dae::TextComponent>(std::make_shared<dae::TextComponent>(obj, "PLAYER 2:", font));
		textSize = obj->GetComponent<dae::TextComponent>().lock()->GetTextSize();
		pos.x = static_cast<float>((windowWidth / 2.f) - textSize.first) / 2.f + static_cast<float>(windowWidth) / 2.f;
		obj->SetPosition(pos);
		m_pPlayerTitles.push_back(obj->GetComponent<dae::TextComponent>());
		m_pScene.lock()->Add(obj);
	}

	obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<dae::TextComponent>(std::make_shared<dae::TextComponent>(obj, std::to_string(scores[0]), font));
	textSize = obj->GetComponent<dae::TextComponent>().lock()->GetTextSize();
	if (scores.size() == 1)
		pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	else
		pos.x = static_cast<float>((windowWidth / 2.f) - textSize.first) / 2.f;
	pos.y -= static_cast<float>(textSize.second) + 10.f;
	obj->SetPosition(pos);
	m_pPlayerScores.push_back(obj->GetComponent<dae::TextComponent>());
	m_pScene.lock()->Add(obj);
	if (scores.size() > 1)
	{
		obj = std::make_shared<dae::GameObject>();
		obj->AddComponent<dae::TextComponent>(std::make_shared<dae::TextComponent>(obj, std::to_string(scores[1]), font));
		textSize = obj->GetComponent<dae::TextComponent>().lock()->GetTextSize();
		pos.x = static_cast<float>((windowWidth / 2.f) - textSize.first) / 2.f + static_cast<float>(windowWidth) / 2.f;
		obj->SetPosition(pos);
		m_pPlayerScores.push_back(obj->GetComponent<dae::TextComponent>());
		m_pScene.lock()->Add(obj);
	}

	obj = std::make_shared<dae::GameObject>();
	obj->AddComponent<dae::TextComponent>(std::make_shared<dae::TextComponent>(obj, "HIGHSCORES:", font));
	textSize = obj->GetComponent<dae::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= static_cast<float>(textSize.second) + 15.f;
	obj->SetPosition(pos);
	m_pHighScores.push_back(obj->GetComponent<dae::TextComponent>());
	m_pScene.lock()->Add(obj);

	for (auto score : highScores)
	{
		obj = std::make_shared<dae::GameObject>();
		obj->AddComponent<dae::TextComponent>(std::make_shared<dae::TextComponent>(obj, std::to_string(score), font));
		textSize = obj->GetComponent<dae::TextComponent>().lock()->GetTextSize();
		pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
		pos.y -= static_cast<float>(textSize.second) + 10.f;
		obj->SetPosition(pos);
		m_pHighScores.push_back(obj->GetComponent<dae::TextComponent>());
		m_pScene.lock()->Add(obj);
	}
}
