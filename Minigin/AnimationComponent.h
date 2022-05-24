#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include <string>
#include <memory>
#include <vector>

namespace dae
{
	struct AnimationInit
	{
		size_t frames;
		float duration;
		const std::string fileName;
		AnimationInit(size_t fr, float d, const std::string& f)
			: frames{fr}
			, duration{d}
			, fileName{f}
		{}
	};
	class Texture2D;
	struct Animation
	{
		size_t frames;
		float duration;
		std::shared_ptr<Texture2D> texture;
		float accumTime;
	};
	class AnimationComponent final : public BaseComponent
	{
	public:
		void StaticUpdate() override;
		void Update() override;
		void Render(const Transform& pos) const override;

		void AddTexture(const AnimationInit animInfo);
		bool SetActiveAnimation(size_t index);

		explicit AnimationComponent(const std::shared_ptr<GameObject>& owner, const std::vector<AnimationInit>& animInfo);
		virtual ~AnimationComponent() = default;
		AnimationComponent(const AnimationComponent& other) = delete;
		AnimationComponent(AnimationComponent&& other) = delete;
		AnimationComponent& operator=(const AnimationComponent& other) = delete;
		AnimationComponent& operator=(AnimationComponent&& other) = delete;

	private:
		std::vector<Animation> m_Textures;
		size_t m_ActiveTexture;
	};
}
