#pragma once
#include "Transform.h"
#include "SceneObject.h"
#include "list"
#include "BaseComponent.h"
#include "Subject.h"

namespace dae
{
	class GameObject final
	{
	public:
		void Update(float deltaTime);
		void StaticUpdate(float deltaTime);
		void Render() const;

		template<typename T> void AddComponent(T* comp);
		template<typename T> T* GetComponent() const;
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

		~GameObject();
	private:
		Transform m_Transform{};
		GameObject* m_Parent = nullptr;
		std::list<GameObject*> m_Children;
		std::vector<BaseComponent*> m_pComponentObjects;
		Subject* m_Subject;
	};

	template<typename T> void dae::GameObject::AddComponent(T* comp)
	{
		if (!GetComponent<T>())
		{
			m_pComponentObjects.push_back(comp);
		}
	}

	template<typename T> T* dae::GameObject::GetComponent() const
	{
		auto it = std::find_if(m_pComponentObjects.begin(),
			m_pComponentObjects.end(),
			[](BaseComponent* sceneObject)
			{
				return typeid(T) == typeid(*sceneObject);
			});

		if (it == m_pComponentObjects.end())
			return nullptr;

		return dynamic_cast<T*>(*it);
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
