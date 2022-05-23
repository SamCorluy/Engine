#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "ElapsedTime.h"

dae::TextureComponent::TextureComponent(const std::shared_ptr<GameObject>& owner, const std::vector<AnimationInit>& animInfo)
	: BaseComponent(owner)
	, m_ActiveTexture{0}
{
	for (auto instance : animInfo)
	{
		AddTexture(instance);
	}
}

void dae::TextureComponent::Render(const Transform& pos) const
{
	auto position = pos.GetPosition();
	if (!(m_ActiveTexture == 0 && m_Textures.size() == 0) && m_Textures[m_ActiveTexture].texture != nullptr)
	{
		float accumTime = m_Textures[m_ActiveTexture].accumTime;
		float duration = m_Textures[m_ActiveTexture].duration;
		int frames = static_cast<int>(m_Textures[m_ActiveTexture].frames);
		float timePerFrame = duration / frames;
		int currentFrame = int(accumTime / timePerFrame);
		auto texture = m_Textures[m_ActiveTexture].texture;
		int width, height;
		SDL_QueryTexture(texture->GetSDLTexture(), NULL, NULL, &width, &height);

		Rect srcRect;
		srcRect.y = 0;
		srcRect.width = int(width / frames);
		srcRect.x = currentFrame * srcRect.width;
		srcRect.height = height;
		Rect dstRect;
		dstRect.x = static_cast<int>(position.x);
		dstRect.y = static_cast<int>(position.y);
		dstRect.width = srcRect.width * 4;
		dstRect.height = srcRect.width * 4;
		Renderer::GetInstance().RenderTexture(*m_Textures[m_ActiveTexture].texture, srcRect, dstRect);
		
	}
}

void dae::TextureComponent::AddTexture(const AnimationInit animInfo)
{
	Animation animation{};
	animation.accumTime = 0.f;
	animation.duration = animInfo.duration;
	animation.texture = ResourceManager::GetInstance().LoadTexture(animInfo.fileName);
	animation.frames = animInfo.frames;
	m_Textures.push_back(animation);
}

bool dae::TextureComponent::SetActiveAnimation(size_t index)
{
	if (index < m_Textures.size())
	{
		m_ActiveTexture = index;
		return true;
	}
	return false;
}

void dae::TextureComponent::StaticUpdate()
{

}

void dae::TextureComponent::Update()
{
	if (!(m_ActiveTexture == 0 && m_Textures.size() == 0) && m_Textures[m_ActiveTexture].texture != nullptr)
	{
		float accumTime = m_Textures[m_ActiveTexture].accumTime;
		float duration = m_Textures[m_ActiveTexture].duration;
		accumTime += ElapsedTime::GetInstance().GetElapsedTime();
		int a = static_cast<int>(10000.f * accumTime);
		int b = static_cast<int>(10000.f * duration);
		accumTime = static_cast<float>(a % b) / 10000.f;
		m_Textures[m_ActiveTexture].accumTime = accumTime;
	}
}
