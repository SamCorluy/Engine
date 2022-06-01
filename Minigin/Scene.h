#pragma once
#include "SceneManager.h"
#include <vector>

namespace dae
{
	class GameObject;
	class Scene
	{
		friend std::weak_ptr<Scene> SceneManager::CreateScene(const std::string& name);
	public:
		void Add(const std::shared_ptr<GameObject>& object);

		void Update();
		void StaticUpdate();
		void Render() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_Name;
		std::vector<std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::shared_ptr<GameObject>> m_ObjectQueue{};

		static unsigned int m_IdCounter; 
	};

}
