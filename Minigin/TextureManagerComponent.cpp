#include "MiniginPCH.h"
#include "TextureManagerComponent.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "ElapsedTime.h"

dae::TextureManagerComponent::TextureManagerComponent(const std::shared_ptr<GameObject>& owner, const std::vector<std::pair<const std::string, glm::vec2>>& textureInfo, int scale)
	: BaseComponent(owner)
	, m_Scale{scale}
{
	for (auto instance : textureInfo)
	{
		AddTexture(instance.first, instance.second);
	}
}

void dae::TextureManagerComponent::Render(const Transform& pos) const
{
	auto window = Renderer::GetInstance().GetWindow();
	int windowHeight;
	SDL_GetWindowSize(window, nullptr, &windowHeight);
	auto position = pos.GetPosition();
	for (auto text : m_Textures)
	{
		int width, height;
		SDL_QueryTexture(text->texture->GetSDLTexture(), NULL, NULL, &width, &height);
		Rect dstRect;
		dstRect.x = static_cast<int>(position.x + text->offset.x);
		dstRect.y = windowHeight - static_cast<int>(position.y + text->offset.y) - height * m_Scale;
		dstRect.width = width * m_Scale;
		dstRect.height = height * m_Scale;
		Renderer::GetInstance().RenderTexture(*text->texture, static_cast<float>(dstRect.x), static_cast<float>(dstRect.y), static_cast<float>(dstRect.width), static_cast<float>(dstRect.height), false);
	}
}

void dae::TextureManagerComponent::AddTexture(const std::string& fileName, glm::vec2 offset)
{
	TextureStruct text{};
	text.texture = ResourceManager::GetInstance().LoadTexture(fileName);
	text.offset = offset;
	m_Textures.push_back(std::make_shared<TextureStruct>(text));
}

const glm::vec2 dae::TextureManagerComponent::getOffset(size_t idx) const
{
	glm::vec2 offset {};
	if(idx < m_Textures.size())
		offset = m_Textures[idx]->offset;
	return offset;
}

void dae::TextureManagerComponent::setOffset(size_t idx, glm::vec2 offset)
{
	if (idx < m_Textures.size())
		m_Textures[idx]->offset = offset;
}

void dae::TextureManagerComponent::StaticUpdate()
{

}

void dae::TextureManagerComponent::Update()
{
}