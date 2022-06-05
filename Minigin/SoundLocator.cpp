#include "MiniginPCH.h"
#include "SoundLocator.h"
Engine::NullSound Engine::SoundLocator::_nullsystem{};
Engine::Sound* Engine::SoundLocator::_ss_instance = &_nullsystem;

Engine::Sound& Engine::SoundLocator::get_sound_system()
{
	return *_ss_instance;
}

void Engine::SoundLocator::register_sound_system(Sound* ss)
{
	_ss_instance = ss;
}
