#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object, size_t i)
{
	m_ChangeOrder = true;
	m_ObjectQueue.push_back({ object, i });
}

void Scene::Update()
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

void dae::Scene::StaticUpdate()
{
	for (auto& object : m_Objects)
		object.first->StaticUpdate();
}

void Scene::Render() const
{
	for (auto& object : m_Objects)
		object.first->Render();
}

