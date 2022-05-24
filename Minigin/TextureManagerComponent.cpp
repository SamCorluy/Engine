#include "MiniginPCH.h"
#include "TextureManagerComponent.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "ElapsedTime.h"

dae::TextureManagerComponent::TextureManagerComponent(const std::shared_ptr<GameObject>& owner, const std::vector<std::pair<const std::string, glm::vec2>>& textureInfo)
	: BaseComponent(owner)
{
	for (auto instance : textureInfo)
	{
		AddTexture(instance.first, instance.second);
	}
}

void dae::TextureManagerComponent::Render(const Transform& pos) const
{
	int scale{ 3 };
	auto window = Renderer::GetInstance().GetWindow();
	int windowHeight;
	SDL_GetWindowSize(window, nullptr, &windowHeight);
	auto position = pos.GetPosition();
	for (auto text : m_Textures)
	{
		int width, height;
		SDL_QueryTexture(text->texture->GetSDLTexture(), NULL, NULL, &width, &height);
		Rect dstRect;
		dstRect.x = static_cast<int>(position.x + text->offset.x) * scale;
		dstRect.y = windowHeight - static_cast<int>(position.y + text->offset.y) * scale;
		dstRect.width = width * scale;
		dstRect.height = height * scale;
		Renderer::GetInstance().RenderTexture(*text->texture, static_cast<float>(dstRect.x), static_cast<float>(dstRect.y), static_cast<float>(dstRect.width), static_cast<float>(dstRect.height));
	}
}

void dae::TextureManagerComponent::AddTexture(const std::string& fileName, glm::vec2 offset)
{
	TextureStruct text{};
	text.texture = ResourceManager::GetInstance().LoadTexture(fileName);
	text.offset = offset;
	m_Textures.push_back(std::make_shared<TextureStruct>(text));
}

void dae::TextureManagerComponent::StaticUpdate()
{

}

void dae::TextureManagerComponent::Update()
{
}