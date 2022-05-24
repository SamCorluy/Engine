#include "Game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PeterPepperComponent.h"
#include "CharacterMoveCommand.h"
#include "InputManager.h"
#include "LevelComponent.h"

void Game::LoadGame() const
{
	auto pScene = dae::SceneManager::GetInstance().CreateScene("BurgerTime").lock();
	dae::SceneManager::GetInstance().SetActiveScene("BurgerTime");

	auto gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<LevelComponent>(std::make_shared<LevelComponent>(gameObject, "../Data/Level.txt"));
	pScene->Add(gameObject);

	gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<PeterPepperComponent>(std::make_shared<PeterPepperComponent>(gameObject, glm::vec2(10.f, 50.f)));
	std::weak_ptr<PeterPepperComponent> peterPepperComponent = gameObject->GetComponent<PeterPepperComponent>();
	dae::InputManager::GetInstance().AddKeyboardInput('w', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::ClimbingUp));
	dae::InputManager::GetInstance().AddKeyboardInput('s', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::ClimbingDown));
	dae::InputManager::GetInstance().AddKeyboardInput('a', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::Walking));
	pScene->Add(gameObject);

}
