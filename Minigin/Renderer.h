#pragma once
#include "Singleton.h"
#include "Structs.h"
#include "SDL.h"

namespace Engine
{
	class GameObject;
	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final : public Singleton<Renderer>
	{
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_clearColor{};	
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, bool flip) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, bool flip) const;
		void RenderTexture(const Texture2D& texture, Rect src, Rect dst, bool flip) const;
		SDL_Window* GetWindow() const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }

		const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
	};
}

