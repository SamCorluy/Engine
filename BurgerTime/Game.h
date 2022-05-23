#pragma once
#include "Minigin.h"

class Game final : public dae::Minigin
{
public:
	Game() = default;
private:
	void LoadGame() const override;
};

