#include "HealthComponent.h"
#include "GameObject.h"
#include "TextureManagerComponent.h"

HealthComponent::HealthComponent(const std::shared_ptr<Engine::GameObject>& owner, int scale, int health)
	: BaseComponent(owner)
	, m_RectSize{ 7 * scale, 8 * scale }
{
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	info.second.y = 0;
	for (int i = 0; i < health; ++i)
	{
		info.first = "Textures/Health.png";
		info.second.x = static_cast<float>((m_RectSize.first + 1) * i);
		textureInfo.push_back(info);
	}
	GetOwner().lock()->AddComponent<Engine::TextureManagerComponent>(std::make_shared<Engine::TextureManagerComponent>(GetOwner().lock(), textureInfo, scale));
}

void HealthComponent::Update()
{
}

void HealthComponent::StaticUpdate()
{
}

void HealthComponent::Render(const Engine::Transform&) const
{
}

void HealthComponent::RemoveLife() const
{
	GetOwner().lock()->GetComponent<Engine::TextureManagerComponent>().lock()->RemoveLastTexture();
}
