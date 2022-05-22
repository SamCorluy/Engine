#include "MiniginPCH.h"
#include "FpsComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Texture2D.h"

dae::FpsComponent::FpsComponent(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Font>& font)
	: BaseComponent(owner)
{
	owner->AddComponent<TextComponent>(std::make_shared<TextComponent>(owner, "60 FPS", font));
	m_pTextComp = owner->GetComponent<TextComponent>();
}

void dae::FpsComponent::Update(float deltaTime)
{
	int fps = (int)std::floor(1.f / deltaTime);

	m_pTextComp.lock()->SetText(std::to_string(fps) + " FPS");
}

void dae::FpsComponent::StaticUpdate(float)
{
}

void dae::FpsComponent::Render(const Transform&) const
{
}

void dae::FpsComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_pTextComp.lock()->SetFont(font);
}
