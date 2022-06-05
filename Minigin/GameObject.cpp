#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

//dae::GameObject::~GameObject() = default;

void Engine::GameObject::Update()
{
	for (auto& object : m_pComponentObjects)
	{
		object->Update();
	}
}

void Engine::GameObject::StaticUpdate()
{
	for (auto& object : m_pComponentObjects)
	{
		object->StaticUpdate();
	}
}

glm::vec2 Engine::GameObject::GetPosition() const
{
	glm::vec2 pos{ m_Transform.GetPosition().x, m_Transform.GetPosition().y };
	return pos;
}

void Engine::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void Engine::GameObject::SetPosition(const glm::vec2& pos)
{
	m_Transform.SetPosition(pos.x, pos.y, 0.0f);
}

const Engine::Transform& Engine::GameObject::GetTransform() const
{
	return m_Transform;
}

void Engine::GameObject::Render() const
{
	for (auto& object : m_pComponentObjects)
	{
		object->Render(m_Transform);
	}
}

void Engine::GameObject::Remove()
{
	m_Remove = true;
}

bool Engine::GameObject::NeedsRemoval()
{
	return m_Remove;
}
