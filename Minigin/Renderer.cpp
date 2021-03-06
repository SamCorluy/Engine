#include "MiniginPCH.h"
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "GameObject.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Engine::Renderer::Init(SDL_Window * window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Engine::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();
	
	SDL_RenderPresent(m_Renderer);
}

void Engine::Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, bool flip) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	if(!flip)
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, 0.f, nullptr, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, bool flip) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);

	if (!flip)
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, 0.f, nullptr, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
}

void Engine::Renderer::RenderTexture(const Texture2D& texture, Rect src, Rect dst, bool flip) const
{
	SDL_Rect srcRect{};
	SDL_Rect dstRect{};
	srcRect.x = src.x;
	srcRect.y = src.y;
	srcRect.w = src.width;
	srcRect.h = src.height;
	dstRect.x = dst.x;
	dstRect.y = dst.y;
	dstRect.w = dst.width;
	dstRect.h = dst.height;
	if (!flip)
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect);
	else
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect, 0.f, nullptr, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
}

SDL_Window* Engine::Renderer::GetWindow() const
{
	return m_Window;
}
