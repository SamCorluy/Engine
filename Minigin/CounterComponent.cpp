#include "MiniginPCH.h"
#include "CounterComponent.h"

Engine::CounterComponent::CounterComponent(std::shared_ptr<GameObject> owner, const  std::shared_ptr<Font> font, int value)
	: BaseComponent(owner)
	, m_Value{value}
{
	owner->AddComponent<Engine::TextComponent>(std::make_shared<Engine::TextComponent>(owner, std::to_string(value), font));
	m_pTextComp = owner->GetComponent<Engine::TextComponent>();
}

void Engine::CounterComponent::Update()
{
}

void Engine::CounterComponent::StaticUpdate()
{
}

void Engine::CounterComponent::Render(const Transform&) const
{
}

void Engine::CounterComponent::SetCounter(int value)
{
	m_Value = value;
	m_pTextComp.lock()->SetText(std::to_string(value));
}

const int Engine::CounterComponent::GetValue() const
{
	return m_Value;
}
