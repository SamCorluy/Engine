#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name) : m_Name(name) {}

Scene::~Scene() = default;

void Scene::Add(const std::shared_ptr<GameObject>& object)
{
	m_ObjectQueue.push_back(object);
}

void Scene::Update()
{
	for (auto& object : m_ObjectQueue)
		m_Objects.push_back(object);
	if(m_ObjectQueue.size() > 0)
		m_ObjectQueue.clear();
	for (auto& object : m_Objects)
		object->Update();
	m_Objects.erase(std::remove_if(m_Objects.begin(), m_Objects.end(), [](std::shared_ptr<GameObject>& object) { return object->NeedsRemoval(); }), m_Objects.end());
}

void dae::Scene::StaticUpdate()
{
	for (auto& object : m_Objects)
		object->StaticUpdate();
}

void Scene::Render() const
{
	for (auto& object : m_Objects)
		object->Render();
}

