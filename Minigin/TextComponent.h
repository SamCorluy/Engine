#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Font;
	class Texture2D;
	class TextComponent final : public BaseComponent
	{
	public:
		void StaticUpdate() override;
		void Update() override;
		void Render(const Transform& pos) const override;

		void SetText(const std::string& text);
		void SetFont(const std::shared_ptr<Font>& font);

		explicit TextComponent(const std::shared_ptr<GameObject>& owner, const std::string& text, const std::shared_ptr<Font>& font);
		explicit TextComponent(const std::shared_ptr<GameObject>& owner);
		virtual ~TextComponent() = default;

		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;
	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		std::shared_ptr<Font> m_Font;
		std::shared_ptr<Texture2D> m_TextTexture;
	};
}
