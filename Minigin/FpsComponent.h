#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class TextComponent;
	class FpsComponent final : public BaseComponent
	{
	public:
		void Update(float deltaTime) override;
		void StaticUpdate(float deltaTime) override;
		void Render(const Transform& pos) const override;

		explicit FpsComponent(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Font>& font);
		virtual ~FpsComponent() = default;
		void SetFont(const std::shared_ptr<Font>& font);
		FpsComponent(const FpsComponent& other) = delete;
		FpsComponent(FpsComponent&& other) = delete;
		FpsComponent& operator=(const FpsComponent& other) = delete;
		FpsComponent& operator=(FpsComponent&& other) = delete;
	private:
		std::weak_ptr<TextComponent> m_pTextComp;
	};
}
