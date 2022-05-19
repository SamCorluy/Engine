#pragma once
#include "Transform.h"

class BaseComponent
{
public:
	BaseComponent() = default;
	virtual ~BaseComponent() = default;
	virtual void Update(float deltaTime) = 0;
	virtual void StaticUpdate(float deltaTime) = 0;
	virtual void Render(glm::vec3 pos) const = 0;
};

