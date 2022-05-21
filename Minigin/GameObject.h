#pragma once
#include "Transform.h"
#include "list"
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class GameObject final
	{
	public:
		GameObject() = default;
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Update(float deltaTime);
		void StaticUpdate(float deltaTime);
		void Render() const;

		template<typename T> void AddComponent(const std::weak_ptr<BaseComponent>& comp);
		template<typename T> std::weak_ptr<T> GetComponent() const;
		template<typename T> void RemoveComponent();

		void SetParent(GameObject* parent);
		GameObject* GetParent() const;

		size_t GetChildCount() const;
		GameObject* GetChildAt(int index) const;

		void RemoveChild(int index);
		void AddChild(GameObject* go);

		void SetPosition(float x, float y)
		{
			m_Transform.SetPosition(x, y, 0.0f);
		}
		void SetSubject(Subject* subject)
		{
			m_Subject = subject;
		}
		Subject* GetSubject() { return m_Subject; };

	private:
		Transform m_Transform{};
		GameObject* m_Parent = nullptr;
		std::list<GameObject*> m_Children;
		std::vector<std::shared_ptr<BaseComponent>> m_pComponentObjects;
		Subject* m_Subject;
	};

	template<typename T> void dae::GameObject::AddComponent(const std::weak_ptr<BaseComponent>& comp)
	{
		if (!GetComponent<T>().lock())
		{
			m_pComponentObjects.push_back(comp.lock());
		}
	}

	template<typename T> std::weak_ptr<T> dae::GameObject::GetComponent() const
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

	template<typename T> void dae::GameObject::RemoveComponent()
	{
		auto comp = GetComponent<T>();
		if (comp)
		{
			m_pComponentObjects.erase(comp);
			delete comp;
		}
	}
	//class Texture2D;

	//// todo: this should become final.
	//class GameObject final : public SceneObject
	//{
	//public:
	//	void Update(float deltaTime) override;

	//	void SetTexture(const std::string& filename);
	//	void SetPosition(float x, float y);

	//	GameObject() = default;
	//	virtual ~GameObject();
	//	GameObject(const GameObject& other) = delete;
	//	GameObject(GameObject&& other) = delete;
	//	GameObject& operator=(const GameObject& other) = delete;
	//	GameObject& operator=(GameObject&& other) = delete;

	//private:
	//	Transform m_Transform;
	//	// todo: mmm, every gameobject has a texture? Is that correct?
	//	//std::shared_ptr<Texture2D> m_Texture{};
	//};
}
