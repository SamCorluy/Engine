#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include "TextComponent.h"

namespace Engine
{
	class CounterComponent final : public BaseComponent
	{
	public:
		CounterComponent(std::shared_ptr<GameObject> owner, const std::shared_ptr<Font> font, int value);

		void Update() override;
		void StaticUpdate() override;
		void Render(const Transform& pos) const override;
		void SetCounter(int value);
		const int GetValue() const;
	private:
		std::weak_ptr<TextComponent> m_pTextComp;
		int m_Value;
	};
}

