#include "MiniginPCH.h"
#include "FpsComponent.h"
#include "TextComponent.h"
#include "GameObject.h"
#include "Texture2D.h"
#include "ElapsedTime.h"

Engine::FpsComponent::FpsComponent(const std::shared_ptr<GameObject>& owner, const std::shared_ptr<Font>& font)
	: BaseComponent(owner)
{
	owner->AddComponent<TextComponent>(std::make_shared<TextComponent>(owner, "60 FPS", font));
	m_pTextComp = owner->GetComponent<TextComponent>();
}

void Engine::FpsComponent::Update()
{
	int fps = (int)std::floor(1.f / ElapsedTime::GetInstance().GetElapsedTime());

	m_pTextComp.lock()->SetText(std::to_string(fps) + " FPS");
}

void Engine::FpsComponent::StaticUpdate()
{
}

void Engine::FpsComponent::Render(const Transform&) const
{
}

void Engine::FpsComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_pTextComp.lock()->SetFont(font);
}
