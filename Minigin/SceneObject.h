#pragma once
#include "Transform.h"
namespace dae
{
	class SceneObject
	{
	public:
		virtual void Update(float deltaTime) = 0;
		virtual void StaticUpdate(float deltaTime) = 0;
		virtual void Render() const = 0;
		/*void SetPosition(float x, float y)
		{
			m_Transform.SetPosition(x, y, 0.0f);
		}*/

		SceneObject() = default;
		virtual ~SceneObject() = default;
		SceneObject(const SceneObject& other) = delete;
		SceneObject(SceneObject&& other) = delete;
		SceneObject& operator=(const SceneObject& other) = delete;
		SceneObject& operator=(SceneObject&& other) = delete;
	protected:
		//Transform m_Transform;
	};
}
