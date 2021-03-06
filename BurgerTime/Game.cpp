#include "Game.h"
#include "MenuComponent.h"
#include "MenuCommand.h"
//#include "InputManager.h"

void Game::LoadGame() const
{
	auto pScene = Engine::SceneManager::GetInstance().CreateScene("Menu").lock();
	Engine::SceneManager::GetInstance().SetActiveScene("Menu");

	//auto gameObject = std::make_shared<dae::GameObject>();
	//gameObject->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(gameObject, pScene));
	//pScene->Add(gameObject);

	auto menuObject = std::make_shared<Engine::GameObject>();
	menuObject->AddComponent<MenuComponent>(std::make_shared<MenuComponent>(menuObject, pScene));
	pScene->Add(menuObject);
}
