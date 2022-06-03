#include "Game.h"
#include "MenuComponent.h"
#include "MenuCommand.h"
#include "InputManager.h"

void Game::LoadGame() const
{
	auto pScene = dae::SceneManager::GetInstance().CreateScene("Menu").lock();
	dae::SceneManager::GetInstance().SetActiveScene("Menu");

	//auto gameObject = std::make_shared<dae::GameObject>();
	//gameObject->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(gameObject, pScene));
	//pScene->Add(gameObject);

	auto menuObject = std::make_shared<dae::GameObject>();
	menuObject->AddComponent<MenuComponent>(std::make_shared<MenuComponent>(menuObject, pScene));
	pScene->Add(menuObject);
	dae::InputManager::GetInstance().AddKeyboardInput('q', dae::InputType::Press, std::make_shared<MenuCommand>(menuObject->GetComponent<MenuComponent>(), GameModes::SINGLEPLAYER));
}
