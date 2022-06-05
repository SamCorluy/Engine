#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

Engine::Scene::Scene(const std::string& name) : m_Name(name) {}

Engine::Scene::~Scene() = default;

void Engine::Scene::Add(const std::shared_ptr<GameObject>& object, size_t i)
{
	m_ChangeOrder = true;
	m_ObjectQueue.push_back({ object, i });
}

void Engine::Scene::Update()
{
	for (auto& object : m_ObjectQueue)
		m_Objects.push_back(object);
	if(m_ObjectQueue.size() > 0)
		m_ObjectQueue.clear();
	for (auto& object : m_Objects)
		object.first->Update();
	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(), [](std::pair<std::shared_ptr<GameObject>, size_t>& object) { return object.first->NeedsRemoval(); }), m_Objects.end());
	if (m_ChangeOrder)
		std::sort(m_Objects.begin(), m_Objects.end(), [](std::pair<std::shared_ptr<GameObject>, size_t>& object, std::pair<std::shared_ptr<GameObject>, size_t>& object2) { return object.second > object2.second; });
}

void Engine::Scene::StaticUpdate()
{
	for (auto& object : m_Objects)
		object.first->StaticUpdate();
}

void Engine::Scene::Render() const
{
	for (auto& object : m_Objects)
		object.first->Render();
}

const std::string Engine::Scene::GetName() const
{
	return m_Name;
}

