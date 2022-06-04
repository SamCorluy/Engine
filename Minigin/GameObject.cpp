#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

//dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (auto& object : m_pComponentObjects)
	{
		object->Update();
	}
}

void dae::GameObject::StaticUpdate()
{
	for (auto& object : m_pComponentObjects)
	{
		object->StaticUpdate();
	}
}

glm::vec2 dae::GameObject::GetPosition() const
{
	glm::vec2 pos{ m_Transform.GetPosition().x, m_Transform.GetPosition().y };
	return pos;
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
	for (auto& object : m_pComponentObjects)
	{
		object->Render(m_Transform);
	}
}

void dae::GameObject::Remove()
{
	m_Remove = true;
}

bool dae::GameObject::NeedsRemoval()
{
	return m_Remove;
}
