#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "AnimationComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include "ElapsedTime.h"

dae::AnimationComponent::AnimationComponent(const std::shared_ptr<GameObject>& owner, const std::vector<AnimationInit>& animInfo)
	: BaseComponent(owner)
	, m_ActiveTexture{0}
{
	for (auto instance : animInfo)
	{
		AddTexture(instance);
	}
}

void dae::AnimationComponent::Render(const Transform& pos) const
{
	auto position = pos.GetPosition();
	if (!(m_ActiveTexture == 0 && m_Textures.size() == 0) && m_Textures[m_ActiveTexture].texture != nullptr)
	{
		int scale{ 3 };
		auto window = Renderer::GetInstance().GetWindow();
		int windowHeight;
		SDL_GetWindowSize(window, nullptr, &windowHeight);

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
		dstRect.y = windowHeight - static_cast<int>(position.y);
		dstRect.width = srcRect.width * scale;
		dstRect.height = srcRect.width * scale;
		Renderer::GetInstance().RenderTexture(*m_Textures[m_ActiveTexture].texture, srcRect, dstRect);
	}
}

void dae::AnimationComponent::AddTexture(const AnimationInit animInfo)
{
	Animation animation{};
	animation.accumTime = 0.f;
	animation.duration = animInfo.duration;
	animation.texture = ResourceManager::GetInstance().LoadTexture(animInfo.fileName);
	animation.frames = animInfo.frames;
	m_Textures.push_back(animation);
}

bool dae::AnimationComponent::SetActiveAnimation(size_t index)
{
	if (index < m_Textures.size())
	{
		m_ActiveTexture = index;
		return true;
	}
	return false;
}

void dae::AnimationComponent::StaticUpdate()
{

}

void dae::AnimationComponent::Update()
{
	if (m_Textures[m_ActiveTexture].frames > 1 && !(m_ActiveTexture == 0 && m_Textures.size() == 0) && m_Textures[m_ActiveTexture].texture != nullptr)
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
