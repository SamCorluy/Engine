#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

//dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float elapsedSec)
{
	for (auto object : m_pComponentObjects)
	{
		object->Update(elapsedSec);
	}
}

void dae::GameObject::StaticUpdate(float elapsedSec)
{
	for (auto object : m_pComponentObjects)
	{
		object->StaticUpdate(elapsedSec);
	}
}

glm::vec2 dae::GameObject::GetPosition() const
{
	glm::vec2 pos{ m_Transform.GetPosition().x, m_Transform.GetPosition().y };
	return glm::vec2();
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform.SetPosition(x, y, 0.0f);
}

void dae::GameObject::SetPosition(const glm::vec2& pos)
{
	m_Transform.SetPosition(pos.x, pos.y, 0.0f);
}

const dae::Transform& dae::GameObject::GetTransform() const
{
	return m_Transform;
}

void dae::GameObject::Render() const
{
	for (auto object : m_pComponentObjects)
	{
		object->Render(m_Transform);
	}
}
