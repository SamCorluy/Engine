#include "MiniginPCH.h"
#include "MrPepperComponent.h"

void dae::MrPepper::Update(float)
{
	SDL_Event e;
	if (currentPlayer == 0)
	{
		pressedKeys.clear();
		while (SDL_PollEvent(&e)) {
			pressedKeys.push_back(e);
		}
	}
	for (auto keystroke : pressedKeys)
	{
		if (m_Lives > 0)
		{
			if (keystroke.key.keysym.sym == m_Die && keystroke.type == SDL_KEYUP)
			{
				--m_Lives;
				std::cout << "dead\n";
				Notify(Event::CHARACTER_DEAD);
			}
			if (keystroke.key.keysym.sym == m_DropBurger && keystroke.type == SDL_KEYUP)
			{
				std::cout << "burger dropped\n";
				Notify(Event::BURGER_DROP);
			}
			if (keystroke.key.keysym.sym == m_KillEnemy && keystroke.type == SDL_KEYUP)
			{
				std::cout << "enemy killed\n";
				Notify(Event::ENEMY_KILL);
			}
		}
	}
	++currentPlayer %= amountOfPlayers;
}


dae::MrPepper::MrPepper(SDL_KeyCode die, SDL_KeyCode dropBurger, SDL_KeyCode killEnemy, int lives)
	: m_Die(die)
	, m_DropBurger(dropBurger)
	, m_KillEnemy(killEnemy)
	, m_Lives(lives)
	//,m_NeedsUpdate(true)
{
	++amountOfPlayers;
}
