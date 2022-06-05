#pragma once
#include "Sound.h"
namespace Engine
{
	class NullSound final : public Sound
	{
	public:
		NullSound() = default;
		virtual ~NullSound() override = default;

		NullSound(const NullSound& other) = delete;
		NullSound(NullSound&& other) = delete;
		NullSound& operator=(const NullSound& other) = delete;
		NullSound& operator=(NullSound&& other) = delete;

		virtual int loadSound(const char*, SoundType) override { return 0; };
		virtual void play(const sound_id, const float, SoundType) override {};
		virtual void stopAllSounds() override {};
		virtual void update() override {};
		virtual  void Remove() override {};
	};
}