#pragma once
#include "SceneManager.h"
#include <vector>

namespace Engine
{
	class GameObject;
	class Scene
	{
		friend std::weak_ptr<Scene> SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<GameObject>& object, size_t i = 0);

		void Update();
		void StaticUpdate();
		void Render() const;
		const std::string GetName() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::pair<std::shared_ptr<GameObject>, size_t>> m_Objects{};
		std::vector<std::pair<std::shared_ptr<GameObject>, size_t>> m_ObjectQueue{};

		bool m_ChangeOrder = false;
	};

}
