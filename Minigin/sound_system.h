#pragma once
using sound_id = unsigned short;
struct playSound
{
	sound_id id;
	float volume;
};

class sound_system
{
	class SDLMixerPimpl;
	static SDLMixerPimpl* m_pImpl;
public:
	static void init();
	sound_system();
	~sound_system();
	virtual int loadsound(const char* filename);
	virtual void play(const sound_id id, const float volume);
	virtual void stopAllSounds();

	static void update();
private:
	static const int MAX_PENDING;
	static playSound pending_[];
	static int numPending_;
};

