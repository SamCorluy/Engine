#pragma once
#include <map>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void Render();
		void SetActiveScene(const std::string& name);

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::map<std::string, std::shared_ptr<Scene>> m_Scenes;
		std::weak_ptr<Scene> m_pActiveScene = std::shared_ptr<Scene>(nullptr);
	};
}
