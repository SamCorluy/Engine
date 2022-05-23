#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Game.h"

int main(int, char* []) {
	Game game;
	game.Run();
	return 0;
}