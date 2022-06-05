#pragma once
#include <map>
#include <memory>
#include <string>
#include "Singleton.h"

namespace Engine
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		std::weak_ptr<Scene> CreateScene(const std::string& name);

		void Update();
		void StaticUpdate();
		void Render();
		void SetActiveScene(const std::string& name);
		const std::weak_ptr<Scene> GetActiveScene() const;

	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::map<std::string, std::shared_ptr<Scene>> m_Scenes;
		std::weak_ptr<Scene> m_pActiveScene = std::shared_ptr<Scene>(nullptr);
	};
}
