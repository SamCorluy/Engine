#include "Game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PeterPepperComponent.h"

void Game::LoadGame() const
{
	auto pScene = dae::SceneManager::GetInstance().CreateScene("BurgerTime").lock();
	dae::SceneManager::GetInstance().SetActiveScene("BurgerTime");

	auto gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<PeterPepperComponent>(std::make_shared<PeterPepperComponent>(gameObject, glm::vec2(5.f, 5.f)));
	pScene->Add(gameObject);
}
