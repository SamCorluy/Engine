#pragma once
#include "Minigin.h"

class Game final : public Engine::Minigin
{
public:
	Game() = default;
private:
	void LoadGame() const override;
};

