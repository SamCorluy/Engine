#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	m_pActiveScene.lock()->Update(deltaTime);
}

void dae::SceneManager::Render()
{
	m_pActiveScene.lock()->Render();
}

void dae::SceneManager::SetActiveScene(const std::string& name)
{
	m_pActiveScene = m_Scenes[name];
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes[name] = scene;
	if (m_pActiveScene.expired())
		m_pActiveScene = scene;
	return *scene;
}
