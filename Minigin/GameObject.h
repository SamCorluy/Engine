#pragma once
#include "Transform.h"
#include "list"
#include "BaseComponent.h"
#include "Subject.h"
#include <vector>
#include <memory>

namespace Engine
{
	class Scene;
	class Texture2D;
	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject() = default;

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update();
		void StaticUpdate();
		void Render() const;

		template<typename T> void AddComponent(const std::weak_ptr<BaseComponent>& comp);
		template<typename T> std::weak_ptr<T> GetComponent() const;
		template<typename T> void RemoveComponent();
		void Remove();
		bool NeedsRemoval();

		glm::vec2 GetPosition() const;
		void SetPosition(float x, float y);
		void SetPosition(const glm::vec2& pos);

		const Transform& GetTransform() const;
	private:
		Transform m_Transform;
		std::vector<std::shared_ptr<BaseComponent>> m_pComponentObjects{};
		bool m_Remove{false};
	};

	template<typename T> void Engine::GameObject::AddComponent(const std::weak_ptr<BaseComponent>& comp)
	{
		if (!GetComponent<T>().lock() && comp.use_count() == 1)
		{
			m_pComponentObjects.push_back(comp.lock());
		}
	}

	template<typename T> std::weak_ptr<T> Engine::GameObject::GetComponent() const
	{
		std::weak_ptr<T> gameComp;
		for (std::shared_ptr<BaseComponent> comp: m_pComponentObjects)
		{
			gameComp = std::dynamic_pointer_cast<T>(comp);
			if (gameComp.lock() != nullptr)
				return gameComp;
		}

		return gameComp;
	}

	template<typename T> void Engine::GameObject::RemoveComponent()
	{
		auto comp = GetComponent<T>();
		if (comp)
		{
			m_pComponentObjects.erase(comp);
			delete comp;
		}
	}
}
