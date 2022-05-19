#include "MiniginPCH.h"
#include "loggedSound_system.h"

loggedSound_system::loggedSound_system(sound_system& wrapped)
	:wrapped_(wrapped)
{}

int loggedSound_system::loadsound(const char* filename)
{
	log("load sound");
	return wrapped_.loadsound(filename);
}

void loggedSound_system::play(const sound_id id, const float volume)
{
	log("play sound");
	wrapped_.play(id, volume);
}

void loggedSound_system::stopAllSounds()
{
	log("stop all sounds");
	wrapped_.stopAllSounds();
}

void loggedSound_system::log(const char* message)
{
	std::cout << message << "\n";
}
