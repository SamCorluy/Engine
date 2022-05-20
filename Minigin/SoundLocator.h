#pragma once
#include "Sound.h"
#include "NullSound.h"
class SoundLocator
{
public:
	static Sound& get_sound_system();
	static void register_sound_system(Sound* ss);

private:
	static Sound* _ss_instance;
	static NullSound _nullsystem;
};


