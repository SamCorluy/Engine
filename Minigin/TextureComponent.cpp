#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"

dae::TextureComponent::TextureComponent(const std::shared_ptr<GameObject>& owner, const std::string& filename)
	: BaseComponent(owner)
{
	SetTexture(filename);
}

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

}

void dae::TextureComponent::Update(float)
{
}
