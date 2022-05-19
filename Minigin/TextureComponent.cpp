#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::TextureComponent::TextureComponent()
	:m_NeedsUpdate(true), m_Texture(nullptr)
{}

void dae::TextureComponent::Render(glm::vec3 pos) const
{
	if (m_Texture != nullptr)
	{
		Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void dae::TextureComponent::SetTexture(const std::string& filename)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::TextureComponent::StaticUpdate(float)
{
	//if (m_NeedsUpdate)
	//{
	//	//const SDL_Color color = { 255,255,255 }; // only white text is supported now
	//	//const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), color);
	//	//if (surf == nullptr)
	//	//{
	//	//	throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	//	//}
	//	//auto texture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surf);
	//	//if (texture == nullptr)
	//	//{
	//	//	throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	//	//}
	//	//SDL_FreeSurface(surf);
	//	//m_Texture = std::make_shared<Texture2D>(texture);
	//	m_NeedsUpdate = false;
	//}
}

void dae::TextureComponent::Update(float)
{
}
