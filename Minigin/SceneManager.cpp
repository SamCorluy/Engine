#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void Engine::SceneManager::Update()
{
	m_pActiveScene.lock()->Update();
}

void Engine::SceneManager::StaticUpdate()
{
	m_pActiveScene.lock()->StaticUpdate();
}

void Engine::SceneManager::Render()
{
	m_pActiveScene.lock()->Render();
}

void Engine::SceneManager::SetActiveScene(const std::string& name)
{
	m_pActiveScene = m_Scenes[name];
}

const std::weak_ptr<Engine::Scene> Engine::SceneManager::GetActiveScene() const
{
	return m_pActiveScene;
}

std::weak_ptr<Engine::Scene> Engine::SceneManager::CreateScene(const std::string& name)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes[name] = scene;
	if (m_pActiveScene.expired())
		m_pActiveScene = scene;
	return scene;
}

