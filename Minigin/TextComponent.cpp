#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"

Engine::TextComponent::TextComponent(const std::shared_ptr<GameObject>& owner, const std::string& text, const std::shared_ptr<Font>& font)
	:BaseComponent(owner),
	m_NeedsUpdate(true),
	m_Text(text),
	m_Font(font),
	m_TextTexture(nullptr)
{
	Update();
}

Engine::TextComponent::TextComponent(const std::shared_ptr<GameObject>& owner)
	:BaseComponent(owner),
	m_NeedsUpdate(true),
	m_Text(),
	m_Font(),
	m_TextTexture(nullptr)
{
	Update();
}

void Engine::TextComponent::StaticUpdate()
{
}

void Engine::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_shared<Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void Engine::TextComponent::Render(const Transform& pos) const
{
	auto position = pos.GetPosition();
	if (m_TextTexture != nullptr)
	{
		auto window = Renderer::GetInstance().GetWindow();
		int windowHeight;
		SDL_GetWindowSize(window, nullptr, &windowHeight);
		int height;
		SDL_QueryTexture(m_TextTexture->GetSDLTexture(), NULL, NULL, nullptr, &height);
		Renderer::GetInstance().RenderTexture(*m_TextTexture, position.x, windowHeight - (position.y + height), false);
	}
}

// This implementation uses the "dirty flag" pattern
void Engine::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void Engine::TextComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_Font = font;
	m_NeedsUpdate = true;
}

std::pair<int, int> Engine::TextComponent::GetTextSize()
{
	int width, height;
	SDL_QueryTexture(m_TextTexture->GetSDLTexture(), NULL, NULL, &width, &height);
	return {width, height};
}


