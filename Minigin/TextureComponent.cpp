#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "TextureComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "ElapsedTime.h"

dae::TextureComponent::TextureComponent(const std::shared_ptr<GameObject>& owner, std::vector<const AnimationInit> animInfo)
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
	if (!(m_ActiveTexture == 0 && m_Textures.size() > 0) && m_Textures[m_ActiveTexture].texture != nullptr)
	{
		float accumTime = m_Textures[m_ActiveTexture].accumTime;
		float duration = m_Textures[m_ActiveTexture].duration;
		int frames = m_Textures[m_ActiveTexture].frames;
		float timePerFrame = duration / frames;
		int currentFrame = int(accumTime / timePerFrame);
		auto texture = m_Textures[m_ActiveTexture].texture;
		int width, height;
		SDL_QueryTexture(texture->GetSDLTexture(), NULL, NULL, &width, &height);

		Rect srcRect;
		srcRect.x = currentFrame * width;
		srcRect.y = height;
		srcRect.width = int(width / frames);
		srcRect.height = height;
		Rect dstRect;
		dstRect.x = position.x;
		dstRect.y = position.y;
		dstRect.width = srcRect.width;
		dstRect.height = srcRect.width;
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
	if (!(m_ActiveTexture == 0 && m_Textures.size() > 0) && m_Textures[m_ActiveTexture].texture != nullptr)
	{
		float accumTime = m_Textures[m_ActiveTexture].accumTime;
		float duration = m_Textures[m_ActiveTexture].duration;
		accumTime += ElapsedTime::GetInstance().GetElapsedTime();
		accumTime = float(int( 100.f * accumTime) % int(100.f * duration)) / 100.f;
		m_Textures[m_ActiveTexture].accumTime = accumTime;
	}
}
