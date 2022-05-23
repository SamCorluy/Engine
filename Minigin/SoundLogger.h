#pragma once
#include "Sound.h"
class SoundLogger final: public Sound
{
public:
	SoundLogger(Sound* wrapped);
	~SoundLogger();
	SoundLogger(const SoundLogger& other) = delete;
	SoundLogger(SoundLogger&& other) = delete;
	SoundLogger& operator=(const SoundLogger& other) = delete;
	SoundLogger& operator=(SoundLogger&& other) = delete;

	int loadSound(const char* filename, SoundType soundType) override;
	void play(const sound_id id, const float volume, SoundType soundType)override;
	void stopAllSounds() override;

private:
	void log(const char* message);
	Sound* wrapped_;
};

