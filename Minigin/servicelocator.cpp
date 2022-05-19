#include "MiniginPCH.h"
#include "servicelocator.h"
sound_system* servicelocator::_ss_instance{};
NullSound_System servicelocator::_nullsystem{};
void servicelocator::initialize()
{
	_ss_instance = &_nullsystem;
}

sound_system& servicelocator::get_sound_system()
{
	return *_ss_instance;
}

void servicelocator::register_sound_system(sound_system* ss)
{
	if (ss == NULL)
		_ss_instance = &_nullsystem;
	else
		_ss_instance = ss;
}
