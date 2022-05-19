#pragma once
#include "sound_system.h"
class NullSound_System final: public sound_system
{
public:
	virtual int loadsound(const char* filename) override;
	virtual void play(const sound_id id, const float volume) override;
	virtual void stopAllSounds() override;
};

