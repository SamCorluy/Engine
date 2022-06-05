#include "MenuComponent.h"
#include "GameObject.h"
#include <iostream>
#include "InputManager.h"
#include "MenuCommand.h"
#include "TextComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "SoundLocator.h"

MenuComponent::MenuComponent(const std::shared_ptr<Engine::GameObject> owner, const std::weak_ptr<Engine::Scene>& scene)
	: BaseComponent(owner)
	, m_pScene{scene}
	, m_pGame{std::shared_ptr<GameManagerComponent>(nullptr)}
	, m_pActiveScene{scene}
	, m_CurrentGameMode{GameModes::SINGLEPLAYER}
{
	m_pGameScene = Engine::SceneManager::GetInstance().CreateScene("BurgerTime").lock();
	m_pHighScoreScene = Engine::SceneManager::GetInstance().CreateScene("HighScore").lock();
	InitMenu();
	Engine::SoundLocator::get_sound_system().loadSound("../Data/Sounds/music.mp3", Engine::SoundType::MUSIC);
	Engine::SoundLocator::get_sound_system().loadSound("../Data/Sounds/EnemyDeath.mp3", Engine::SoundType::EFFECT);
	Engine::SoundLocator::get_sound_system().loadSound("../Data/Sounds/EnemyStunned.mp3", Engine::SoundType::EFFECT);
	Engine::SoundLocator::get_sound_system().loadSound("../Data/Sounds/WalkedOver.mp3", Engine::SoundType::EFFECT);
}

void MenuComponent::Update()
{
	if (m_pActiveScene.lock()->GetName() == m_pGameScene.lock()->GetName() || !m_IsInit)
	{
		m_IsInit = true;
		Engine::InputManager::GetInstance().AddKeyboardInput(0x40000058, Engine::InputType::Press, std::make_shared<MenuCommand>(GetOwner().lock()->GetComponent<MenuComponent>(), MenuInteraction::SELECT));
		Engine::InputManager::GetInstance().AddKeyboardInput('\r', Engine::InputType::Press, std::make_shared<MenuCommand>(GetOwner().lock()->GetComponent<MenuComponent>(), MenuInteraction::SELECT));
		Engine::InputManager::GetInstance().AddKeyboardInput('w', Engine::InputType::Press, std::make_shared<MenuCommand>(GetOwner().lock()->GetComponent<MenuComponent>(), MenuInteraction::MOVEUP));
		Engine::InputManager::GetInstance().AddKeyboardInput('s', Engine::InputType::Press, std::make_shared<MenuCommand>(GetOwner().lock()->GetComponent<MenuComponent>(), MenuInteraction::MOVEDOWN));
		Engine::InputManager::GetInstance().AddControllerInput(0x5820, Engine::InputType::Press, std::make_shared<MenuCommand>(GetOwner().lock()->GetComponent<MenuComponent>(), MenuInteraction::MOVEUP));
		Engine::InputManager::GetInstance().AddControllerInput(0x5821, Engine::InputType::Press, std::make_shared<MenuCommand>(GetOwner().lock()->GetComponent<MenuComponent>(), MenuInteraction::MOVEDOWN));
		Engine::InputManager::GetInstance().AddControllerInput(0x5800, Engine::InputType::Press, std::make_shared<MenuCommand>(GetOwner().lock()->GetComponent<MenuComponent>(), MenuInteraction::SELECT));
		m_pActiveScene = m_pScene;
	}
}

void MenuComponent::StaticUpdate()
{
}

void MenuComponent::Render(const Engine::Transform&) const
{
}

void MenuComponent::SelectGameMode()
{
	if (Engine::SceneManager::GetInstance().GetActiveScene().lock()->GetName() != m_pScene.lock()->GetName())
		return;
	Engine::InputManager::GetInstance().RemoveKeys();
	Engine::SceneManager::GetInstance().SetActiveScene("BurgerTime");
	m_pActiveScene = m_pGameScene;
	auto gameObject = std::make_shared<Engine::GameObject>();
	gameObject->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(gameObject, m_pGameScene, m_pHighScoreScene, m_CurrentGameMode));
	m_pGame = gameObject->GetComponent<GameManagerComponent>();
	m_pGameScene.lock()->Add(gameObject);
	Engine::SoundLocator::get_sound_system().play(0, 0.3f, Engine::SoundType::MUSIC);
}

void MenuComponent::MoveDown()
{
	if (Engine::SceneManager::GetInstance().GetActiveScene().lock()->GetName() != m_pScene.lock()->GetName())
		return;
	auto pos = m_pSelector.lock()->GetOwner().lock()->GetPosition();
	switch (m_CurrentGameMode)
	{
	case GameModes::SINGLEPLAYER:
		m_CurrentGameMode = GameModes::COOP;
		pos.y = m_pCoop.lock()->GetOwner().lock()->GetPosition().y - 3.f;
		m_pSelector.lock()->GetOwner().lock()->SetPosition(pos);
		break;
	case GameModes::COOP:
		m_CurrentGameMode = GameModes::PVP;
		pos.y = m_pPVP.lock()->GetOwner().lock()->GetPosition().y - 3.f;
		m_pSelector.lock()->GetOwner().lock()->SetPosition(pos);
		break;
	case GameModes::PVP:
		m_CurrentGameMode = GameModes::SINGLEPLAYER;
		pos.y = m_pSinglePlayer.lock()->GetOwner().lock()->GetPosition().y - 3.f;
		m_pSelector.lock()->GetOwner().lock()->SetPosition(pos);
		break;
	default:
		break;
	}
}

void MenuComponent::MoveUp()
{
	if (Engine::SceneManager::GetInstance().GetActiveScene().lock()->GetName() != m_pScene.lock()->GetName())
		return;
	auto pos = m_pSelector.lock()->GetOwner().lock()->GetPosition();
	switch (m_CurrentGameMode)
	{
	case GameModes::SINGLEPLAYER:
		m_CurrentGameMode = GameModes::PVP;
		pos.y = m_pPVP.lock()->GetOwner().lock()->GetPosition().y - 3.f;
		m_pSelector.lock()->GetOwner().lock()->SetPosition(pos);
		break;
	case GameModes::COOP:
		m_CurrentGameMode = GameModes::SINGLEPLAYER;
		pos.y = m_pSinglePlayer.lock()->GetOwner().lock()->GetPosition().y - 3.f;
		m_pSelector.lock()->GetOwner().lock()->SetPosition(pos);
		break;
	case GameModes::PVP:
		m_CurrentGameMode = GameModes::COOP;
		pos.y = m_pCoop.lock()->GetOwner().lock()->GetPosition().y - 3.f;
		m_pSelector.lock()->GetOwner().lock()->SetPosition(pos);
		break;
	default:
		break;
	}
}

void MenuComponent::InitMenu()
{
	// Get fonts ready
	auto font = Engine::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 20);
	auto font2 = Engine::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 16);
	auto font3 = Engine::ResourceManager::GetInstance().LoadFont("emulogic.ttf", 12);

	// Get window size
	auto window = Engine::Renderer::GetInstance().GetWindow();
	int windowWidth, windowHeight;
	SDL_GetWindowSize(window, &windowWidth, &windowHeight);

	glm::vec2 pos{};

	auto obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "CONTROLS:", font));
	auto textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y = static_cast<float>(windowHeight - textSize.second) - 20.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "<PLAYER ONE>", font2));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 15.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "UP: W", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "DOWN: S", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "LEFT: A", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "RIGHT: D", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "THROW SALT: E", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "<PLAYER TWO | PLAYER ONE SINGLEPLAYER>", font2));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 15.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "UP: LEFT JOYSTICK UP", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "DOWN: LEFT JOYSTICK DOWN", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "LEFT: LEFT JOYSTICK LEFT", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "RIGHT: LEFT JOYSTICK RIGHT", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "THROW SALT: A(CONTROLLER)", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "<MR HOTDOG>", font2));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 15.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "UP: LEFT JOYSTICK UP", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "DOWN: LEFT JOYSTICK DOWN", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "LEFT: LEFT JOYSTICK LEFT", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "RIGHT: LEFT JOYSTICK RIGHT", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "<MENU>", font2));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 15.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "PREV: W, LEFT JOYSTICK DOWN", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "NEXT: S, LEFT JOYSTICK UP", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "SELECT: ENTER, A(CONTROLLER)", font3));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 5.f;
	obj->SetPosition(pos);
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "SINGLEPLAYER", font));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y = 120.f;
	obj->SetPosition(pos);
	m_pSinglePlayer = obj->GetComponent<Engine::TextComponent>();
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	textureInfo.push_back({ "Textures/MenuSelection.png", glm::vec2{0.f, 0.f} });
	obj->AddComponent<Engine::TextureManagerComponent>(std::make_shared<Engine::TextureManagerComponent>(obj, textureInfo, 1));
	pos.x -= 3.f;
	pos.y -= 3.f;
	obj->SetPosition(pos);
	m_pSelector = obj->GetComponent<Engine::TextureManagerComponent>();
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "COOP", font));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 12.f;
	obj->SetPosition(pos);
	m_pCoop = obj->GetComponent<Engine::TextComponent>();
	m_pScene.lock()->Add(obj);

	obj = std::make_shared<Engine::GameObject>();
	obj->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(obj, "PVP", font));
	textSize = obj->GetComponent<Engine::TextComponent>().lock()->GetTextSize();
	pos.x = static_cast<float>(windowWidth - textSize.first) / 2.f;
	pos.y -= textSize.second + 15.f;
	obj->SetPosition(pos);
	m_pPVP = obj->GetComponent<Engine::TextComponent>();
	m_pScene.lock()->Add(obj);
}
