#pragma once
#include "sound_system.h"
#include "NullSound_System.h"
class servicelocator
{
public:
	static void initialize();
	static sound_system& get_sound_system();
	static void register_sound_system(sound_system* ss);

private:
	static sound_system* _ss_instance;
	static NullSound_System _nullsystem;
};


