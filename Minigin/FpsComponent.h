#pragma once
#include "BaseComponent.h"
#include "Transform.h"

namespace dae
{
	class Font;
	class Texture2D;
	class FpsComponent final : public BaseComponent
	{
	public:
		void Update(float deltaTime) override;
		void StaticUpdate(float deltaTime) override;
		void Render(glm::vec3 pos) const override;

		explicit FpsComponent(const std::shared_ptr<Font>& font);
		explicit FpsComponent();
		virtual ~FpsComponent() = default;
		void SetFont(const std::shared_ptr<Font>& font);
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
	};
}
