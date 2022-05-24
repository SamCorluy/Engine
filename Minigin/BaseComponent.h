#pragma once
#include "Transform.h"
#include<memory>

namespace dae
{
	class GameObject;
	class BaseComponent
	{
	public:
		BaseComponent(const std::shared_ptr<GameObject>& owner);
		virtual ~BaseComponent() = default;

		BaseComponent(const BaseComponent& other) = delete;
		BaseComponent(BaseComponent&& other) = delete;
		BaseComponent& operator=(const BaseComponent& other) = delete;
		BaseComponent& operator=(BaseComponent&& other) = delete;

		virtual void Update() = 0;
		virtual void StaticUpdate() = 0;
		virtual void Render(const Transform& pos) const = 0;

		std::weak_ptr<GameObject> GetOwner() const { return m_pOwner; };
	private:
		std::weak_ptr<GameObject> m_pOwner;
	};
}

