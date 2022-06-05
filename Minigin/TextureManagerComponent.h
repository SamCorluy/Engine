#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>
#include <map>

namespace Engine
{
	class Texture2D;
	struct TextureStruct
	{
		glm::vec2 offset;
		std::shared_ptr<Texture2D> texture;
	};
	class TextureManagerComponent final : public BaseComponent
	{
	public:
		void StaticUpdate() override;
		void Update() override;
		void Render(const Transform& pos) const override;

		void AddTexture(const std::string& fileName, glm::vec2 offset);
		void RemoveLastTexture();
		const glm::vec2 getOffset(size_t idx) const;
		void setOffset(size_t idx, glm::vec2 offset);

		explicit TextureManagerComponent(const std::shared_ptr<GameObject>& owner, const std::vector<std::pair<const std::string, glm::vec2>>& textureInfo, int scale);
		virtual ~TextureManagerComponent() = default;
		TextureManagerComponent(const TextureManagerComponent& other) = delete;
		TextureManagerComponent(TextureManagerComponent&& other) = delete;
		TextureManagerComponent& operator=(const TextureManagerComponent& other) = delete;
		TextureManagerComponent& operator=(TextureManagerComponent&& other) = delete;

	private:
		std::vector<std::shared_ptr<TextureStruct>> m_Textures;
		int m_Scale;
	};
}
