#include "SaltDisplayComponent.h"
#include "GameObject.h"
#include "TextureManagerComponent.h"

SaltDisplayComponent::SaltDisplayComponent(const std::shared_ptr<Engine::GameObject>& owner, int scale, int charges)
	:BaseComponent(owner)
	, m_RectSize{ 7 * scale, 8 * scale }
{
	std::vector<std::pair<const std::string, glm::vec2>> textureInfo;
	std::pair<std::string, glm::vec2> info;
	info.second.y = 0;
	for (int i = 0; i < charges; ++i)
	{
		info.first = "Textures/Salt.png";
		info.second.x = static_cast<float>((m_RectSize.first +1)* i);
		textureInfo.push_back(info);
	}
	GetOwner().lock()->AddComponent<Engine::TextureManagerComponent>(std::make_shared<Engine::TextureManagerComponent>(GetOwner().lock(), textureInfo, scale));
}

void SaltDisplayComponent::Update()
{
}

void SaltDisplayComponent::StaticUpdate()
{
}

void SaltDisplayComponent::Render(const Engine::Transform&) const
{
}

void SaltDisplayComponent::RemoveCharge() const
{
	GetOwner().lock()->GetComponent<Engine::TextureManagerComponent>().lock()->RemoveLastTexture();
}
