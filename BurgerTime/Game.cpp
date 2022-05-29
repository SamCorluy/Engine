#include "Game.h"
#include "GameManagerComponent.h"

void Game::LoadGame() const
{
	auto pScene = dae::SceneManager::GetInstance().CreateScene("BurgerTime").lock();
	dae::SceneManager::GetInstance().SetActiveScene("BurgerTime");

	auto gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(gameObject, pScene));
	pScene->Add(gameObject);
}
