#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>

namespace Engine
{
	struct AnimationInit
	{
		size_t frames;
		float duration;
		const std::string fileName;
		std::pair<int, int> offset;
		AnimationInit(size_t fr, float d, const std::string& f, std::pair<int, int> o)
			: frames{fr}
			, duration{d}
			, fileName{f}
			, offset{o}
		{}
	};
	class Texture2D;
	struct Animation
	{
		size_t frames;
		float duration;
		std::shared_ptr<Texture2D> texture;
		float accumTime;
		glm::vec2 animRect;
		std::pair<int, int> offset;
	};
	class AnimationComponent final : public BaseComponent
	{
	public:
		void StaticUpdate() override;
		void Update() override;
		void Render(const Transform& pos) const override;

		void AddTexture(const AnimationInit animInfo);
		bool SetActiveAnimation(size_t index);
		void SetFlip(bool flip);

		explicit AnimationComponent(const std::shared_ptr<GameObject>& owner, const std::vector<AnimationInit>& animInfo, int scale);
		virtual ~AnimationComponent() = default;
		AnimationComponent(const AnimationComponent& other) = delete;
		AnimationComponent(AnimationComponent&& other) = delete;
		AnimationComponent& operator=(const AnimationComponent& other) = delete;
		AnimationComponent& operator=(AnimationComponent&& other) = delete;

		const glm::vec2 getActiveAnimRec();

	private:
		std::vector<Animation> m_Textures;
		size_t m_ActiveTexture;
		int m_Scale;
		bool m_Flip;
	};
}

