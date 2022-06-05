#pragma once

using sound_id = unsigned short;

enum class SoundType {
	MUSIC,
	EFFECT
};

class Sound
{
public:
	Sound() = default;
	virtual ~Sound() = default;

	Sound(const Sound& other) = delete;
	Sound(Sound&& other) = delete;
	Sound& operator=(const Sound& other) = delete;
	Sound& operator=(Sound&& other) = delete;

	virtual int loadSound(const char* filename, SoundType soundType) = 0;
	virtual void play(const sound_id id, const float volume, SoundType soundType) = 0;
	virtual void stopAllSounds() = 0;
	virtual void update() = 0;
	virtual void Remove() = 0;
};

