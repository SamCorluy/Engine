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
	dae::InputManager::GetInstance().AddKeyboardInput(0x40000058, dae::InputType::Press, std::make_shared<MenuCommand>(menuObject->GetComponent<MenuComponent>(), MenuInteraction::SELECT));
	dae::InputManager::GetInstance().AddKeyboardInput('\r', dae::InputType::Press, std::make_shared<MenuCommand>(menuObject->GetComponent<MenuComponent>(), MenuInteraction::SELECT));
	dae::InputManager::GetInstance().AddKeyboardInput('w', dae::InputType::Press, std::make_shared<MenuCommand>(menuObject->GetComponent<MenuComponent>(), MenuInteraction::MOVEUP));
	dae::InputManager::GetInstance().AddKeyboardInput('s', dae::InputType::Press, std::make_shared<MenuCommand>(menuObject->GetComponent<MenuComponent>(), MenuInteraction::MOVEDOWN));
}
