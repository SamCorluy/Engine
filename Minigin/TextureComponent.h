#pragma once
#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{
	class Texture2D;
	class TextureComponent final : public BaseComponent
	{
	public:
		void StaticUpdate(float deltaTime) override;
		void Update(float deltaTime) override;
		void Render(glm::vec3 pos) const override;

		void SetTexture(const std::string& filename);

		explicit TextureComponent(const std::shared_ptr<GameObject>& owner, const std::string& filename);
		virtual ~TextureComponent() = default;
		TextureComponent(const TextureComponent& other) = delete;
		TextureComponent(TextureComponent&& other) = delete;
		TextureComponent& operator=(const TextureComponent& other) = delete;
		TextureComponent& operator=(TextureComponent&& other) = delete;

	private:
		std::shared_ptr<Texture2D> m_Texture;
	};
}

