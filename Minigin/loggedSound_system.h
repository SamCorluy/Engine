#pragma once
#include "sound_system.h"
class loggedSound_system final: public sound_system
{
public:
	loggedSound_system(sound_system &wrapped);
	virtual int loadsound(const char* filename) override;
	virtual void play(const sound_id id, const float volume) override;
	virtual void stopAllSounds() override;

private:
	void log(const char* message);
	sound_system& wrapped_;
};

