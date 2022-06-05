#include "MiniginPCH.h"
#include "CounterComponent.h"

CounterComponent::CounterComponent(std::shared_ptr<dae::GameObject> owner, const  std::shared_ptr<dae::Font> font, int value)
	: BaseComponent(owner)
	, m_Value{value}
{
	owner->AddComponent<dae::TextComponent>(std::make_shared<dae::TextComponent>(owner, std::to_string(value), font));
	m_pTextComp = owner->GetComponent<dae::TextComponent>();
}

void CounterComponent::Update()
{
}

void CounterComponent::StaticUpdate()
{
}

void CounterComponent::Render(const dae::Transform&) const
{
}

void CounterComponent::SetCounter(int value)
{
	m_Value = value;
	m_pTextComp.lock()->SetText(std::to_string(value));
}

const int CounterComponent::GetValue() const
{
	return m_Value;
}
