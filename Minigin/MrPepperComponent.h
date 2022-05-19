#pragma once
#include "BaseComponent.h"
#include "Transform.h"
#include "Subject.h"
namespace dae
{
	static std::vector<SDL_Event> pressedKeys;
	static int amountOfPlayers;
	static int currentPlayer = 0;
	class Texture2D;
	class MrPepper final : public Subject
	{
	public:
		void Update(float elapsedSec) override;

		explicit MrPepper(SDL_KeyCode die, SDL_KeyCode dropBurger, SDL_KeyCode killEnemy, int lives);
		virtual ~MrPepper() = default;
		MrPepper(const MrPepper& other) = delete;
		MrPepper(MrPepper&& other) = delete;
		MrPepper& operator=(const MrPepper& other) = delete;
		MrPepper& operator=(MrPepper&& other) = delete;

	private:
		//bool m_NeedsUpdate;
		SDL_KeyCode m_Die;
		SDL_KeyCode m_DropBurger;
		SDL_KeyCode m_KillEnemy;
		int m_Lives;
		bool m_IsDead;
	};
}

