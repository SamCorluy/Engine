#include "MiniginPCH.h"
#include "Transform.h"

void Engine::Transform::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
