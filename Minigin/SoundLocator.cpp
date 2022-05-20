#include "MiniginPCH.h"
#include "SoundLocator.h"
NullSound SoundLocator::_nullsystem{};
Sound* SoundLocator::_ss_instance = &_nullsystem;

Sound& SoundLocator::get_sound_system()
{
	return *_ss_instance;
}

void SoundLocator::register_sound_system(Sound* ss)
{
	_ss_instance = ss;
}
