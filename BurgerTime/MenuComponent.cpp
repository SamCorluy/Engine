#include "MenuComponent.h"
#include "GameObject.h"
#include <iostream>

MenuComponent::MenuComponent(const std::shared_ptr<dae::GameObject> owner, const std::weak_ptr<dae::Scene>& scene)
	: BaseComponent(owner)
	, m_pScene{scene}
	, m_pGame{std::shared_ptr<GameManagerComponent>(nullptr)}
{
	m_pGameScene = dae::SceneManager::GetInstance().CreateScene("BurgerTime").lock();
	//MenuCommand command{ std::make_shared<MenuComponent>(*this), GameModes::SINGLEPLAYER};
}

void MenuComponent::Update()
{
}

void MenuComponent::StaticUpdate()
{
}

void MenuComponent::Render(const dae::Transform&) const
{
}

void MenuComponent::SelectGameMode(GameModes)
{
	if (dae::SceneManager::GetInstance().GetActiveScene().lock()->GetName() != m_pScene.lock()->GetName())
		return;
	std::cout << "chosen";
	dae::SceneManager::GetInstance().SetActiveScene("BurgerTime");
	auto gameObject = std::make_shared<dae::GameObject>();
	gameObject->AddComponent<GameManagerComponent>(std::make_shared<GameManagerComponent>(gameObject, m_pGameScene));
	m_pGame = gameObject->GetComponent<GameManagerComponent>();
	m_pGameScene.lock()->Add(gameObject);
}
