#pragma once
#include "Sound.h"
namespace Engine
{
	//using sound_id = unsigned short;
	struct playSound
	{
		sound_id id;
		float volume;
		SoundType type;
	};

	class SoundService : public Sound
	{
		class SDLMixerPimpl;
		static SDLMixerPimpl* m_pImpl;
	public:
		static void init();
		SoundService();
		~SoundService();
		SoundService(const SoundService& other) = delete;
		SoundService(SoundService&& other) = delete;
		SoundService& operator=(const SoundService& other) = delete;
		SoundService& operator=(SoundService&& other) = delete;

		int loadSound(const char* filename, SoundType soundType) override;
		void play(const sound_id id, const float volume, SoundType soundType)override;
		void stopAllSounds() override;

		void update() override;
		void Remove() override;
	private:
		static const int MAX_PENDING;
		static playSound pending_[];
		static int numPending_;
	};
}

