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
	for (auto&& child : m_Children)
	{
		child->Update(elapsedSec);
	}
	if(m_Subject)
		m_Subject->Update(elapsedSec);
}

void dae::GameObject::StaticUpdate(float elapsedSec)
{
	for (auto object : m_pComponentObjects)
	{
		object->StaticUpdate(elapsedSec);
	}
	for (auto&& child : m_Children)
	{
		child->StaticUpdate(elapsedSec);
	}
}

void dae::GameObject::SetParent(GameObject* parent)
{
	auto prevParent = m_Parent;
	auto amount = m_Parent->GetChildCount();

	for (size_t i = 0; i < amount; i++)
	{
		prevParent->GetChildAt((int)i)->m_Parent = parent;
	}

	parent->m_Children = prevParent->m_Children;
	prevParent->m_Children.clear();
}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_Parent;
}

size_t dae::GameObject::GetChildCount() const
{
	return m_Children.size();
}

dae::GameObject* dae::GameObject::GetChildAt(int index) const
{
	GameObject* ptr = nullptr;
	if ((int)m_Children.size() > index)
	{
		auto it = m_Children.begin();
		std::advance(it, index);
		ptr = *it;
	}
	return ptr;
}

void dae::GameObject::RemoveChild(int index)
{
	if (index < (int)m_Children.size())
		m_Children.remove(GetChildAt(index));
}

void dae::GameObject::AddChild(GameObject* go)
{
	m_Children.emplace_back(go);
	m_Children.back()->m_Parent = this;
}

void dae::GameObject::Render() const
{
	for (auto object : m_pComponentObjects)
	{
		object->Render(m_Transform.GetPosition());
	}
	for (auto&& child : m_Children)
	{
		child->Render();
	}
}

dae::GameObject::~GameObject()
{
	for (auto obj : m_pComponentObjects)
		delete obj;
	for (auto child : m_Children)
		delete child;
}
