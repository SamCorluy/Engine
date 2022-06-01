#pragma once
#include "GameObject.h"
#include "BaseComponent.h"
#include "TextComponent.h"

class CounterComponent final : public dae::BaseComponent
{
public:
	CounterComponent(std::shared_ptr<dae::GameObject> owner, const std::shared_ptr<dae::Font> font, int value);

	void Update() override;
	void StaticUpdate() override;
	void Render(const dae::Transform& pos) const override;
	void SetCounter(int value);
private:
	std::weak_ptr<dae::TextComponent> m_pTextComp;
};

