#include "MiniginPCH.h"
#include "sound_system.h"
#include "SDL_mixer.h"
class sound_system::SDLMixerPimpl
{
	std::vector<Mix_Chunk*> sounds;

public:
	SDLMixerPimpl()
	{
		Mix_Init(MIX_INIT_MP3);
		SDL_Init(SDL_INIT_AUDIO);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}
	int loadsound(const char* filename)
	{
		Mix_Chunk* m = NULL;
		m = Mix_LoadWAV(filename);
		if (m == NULL)
		{
			std::cout << "loading failed " << Mix_GetError();
			return -1;
		}
		sounds.push_back(m);
		return (int)sounds.size() - 1;
	}
	void play(const sound_id id, const float volume)
	{
		if (id >= sounds.size())
			return;
		Mix_Volume(-1, (int)(volume*100));
		Mix_PlayChannel(-1, sounds[id], 0);
	}
	void stopAllSounds()
	{
		Mix_PauseMusic();
	}
};

const int sound_system::MAX_PENDING = 16;
int sound_system::numPending_{};
playSound sound_system::pending_[MAX_PENDING]{};

sound_system::SDLMixerPimpl* sound_system::m_pImpl{};

void sound_system::init()
{
	numPending_ = 0;
}

sound_system::sound_system()
{
	m_pImpl = new SDLMixerPimpl();
}

sound_system::~sound_system()
{
	delete m_pImpl;
}

int sound_system::loadsound(const char* filename)
{
	return m_pImpl->loadsound(filename);
}

void sound_system::play(const sound_id id, const float volume)
{
	assert(numPending_ < MAX_PENDING);

	pending_[numPending_].id = id;
	pending_[numPending_].volume = volume;
	numPending_++;
}

void sound_system::stopAllSounds()
{
	m_pImpl->stopAllSounds();
}

void sound_system::update()
{
	for (int i = 0; i < numPending_; ++i)
	{
		m_pImpl->play(pending_[i].id, pending_[i].volume);
	}
	numPending_ = 0;
}
