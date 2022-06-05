#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"

Engine::BaseComponent::BaseComponent(const std::shared_ptr<GameObject>& owner)
	:m_pOwner(owner)
{
}
