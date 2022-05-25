#include "Game.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "PeterPepperComponent.h"
#include "CharacterMoveCommand.h"
#include "InputManager.h"
#include "LevelComponent.h"
#include "Renderer.h"

void Game::LoadGame() const
{
	auto pScene = dae::SceneManager::GetInstance().CreateScene("BurgerTime").lock();
	dae::SceneManager::GetInstance().SetActiveScene("BurgerTime");

	auto gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<LevelComponent>(std::make_shared<LevelComponent>(gameObject, "../Data/Level.txt", 3));
	auto size = gameObject->GetComponent<LevelComponent>().lock()->getLevelSize();
	auto window = dae::Renderer::GetInstance().GetWindow();
	int windowWidth;
	int windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);
	glm::vec2 pos;
	pos.x = static_cast<float>(windowWidth - size.first) / 2.f;
	pos.y = static_cast<float>(windowHeight - size.second) / 2.f;
	gameObject->SetPosition(pos);
	pScene->Add(gameObject);
	auto level = gameObject->GetComponent<LevelComponent>();

	gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<PeterPepperComponent>(std::make_shared<PeterPepperComponent>(gameObject, 3, level));
	std::weak_ptr<PeterPepperComponent> peterPepperComponent = gameObject->GetComponent<PeterPepperComponent>();
	dae::InputManager::GetInstance().AddKeyboardInput('w', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::ClimbingUp));
	dae::InputManager::GetInstance().AddKeyboardInput('s', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::ClimbingDown));
	dae::InputManager::GetInstance().AddKeyboardInput('a', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::WalkingLeft));
	dae::InputManager::GetInstance().AddKeyboardInput('d', dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::WalkingRight));
	dae::InputManager::GetInstance().AddControllerInput(0x5820, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::ClimbingUp));
	dae::InputManager::GetInstance().AddControllerInput(0x5821, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::ClimbingDown));
	dae::InputManager::GetInstance().AddControllerInput(0x5823, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::WalkingLeft));
	dae::InputManager::GetInstance().AddControllerInput(0x5822, dae::InputType::Hold, std::make_shared<CharacterMoveCommand>(peterPepperComponent, Action::WalkingRight));
	pScene->Add(gameObject);

}
