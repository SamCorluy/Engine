#include "MiniginPCH.h"
#include "SoundService.h"
#include "SDL_mixer.h"
class Engine::SoundService::SDLMixerPimpl
{
	std::vector<Mix_Chunk*> effects;
	std::vector<Mix_Music*> music;

public:
	SDLMixerPimpl()
	{
		Mix_Init(MIX_INIT_MP3);
		SDL_Init(SDL_INIT_AUDIO);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}
	~SDLMixerPimpl()
	{
		for (auto& effect : effects)
			Mix_FreeChunk(effect);
		effects.clear();
		for (auto& track : music)
			Mix_FreeMusic(track);
		music.clear();
	}
	int loadSound(const char* filename, SoundType soundType)
	{
		if (soundType == SoundType::EFFECT)
		{
			Mix_Chunk* m = NULL;
			m = Mix_LoadWAV(filename);
			if (m == NULL)
			{
				std::cout << "loading failed " << Mix_GetError();
				return -1;
			}
			effects.push_back(m);
			return (int)effects.size() - 1;
		}
		else
		{
			Mix_Music* m = NULL;
			m = Mix_LoadMUS(filename);
			if (m == NULL)
			{
				std::cout << "loading failed " << Mix_GetError();
				return -1;
			}
			music.push_back(m);
			return (int)music.size() - 1;
		}
	}
	void play(const sound_id id, const float volume, SoundType soundType)
	{
		assert(numPending_ < MAX_PENDING);

		pending_[numPending_].id = id;
		pending_[numPending_].volume = volume;
		pending_[numPending_].type = soundType;
		numPending_++;
	}
	void update()
	{
		for (int i = 0; i < numPending_; ++i)
		{
			if (pending_[i].type == SoundType::EFFECT)
			{
				if (pending_[i].id >= effects.size())
					return;
				Mix_Volume(-1, (int)(pending_[i].volume * 100));
				Mix_PlayChannel(-1, effects[pending_[i].id], 0);
			}
			else
			{
				if (pending_[i].id >= music.size())
					return;
				Mix_VolumeMusic((int)(pending_[i].volume * 100));
				Mix_PlayMusic(music[pending_[i].id], -1);
			}
		}
		numPending_ = 0;
	}
	void stopAllSounds()
	{
		Mix_HaltMusic();
		Mix_HaltChannel(-1);
	}
};

const int Engine::SoundService::MAX_PENDING = 16;
int Engine::SoundService::numPending_{};
Engine::playSound Engine::SoundService::pending_[MAX_PENDING]{};

Engine::SoundService::SDLMixerPimpl* Engine::SoundService::m_pImpl{};

void Engine::SoundService::init()
{
	numPending_ = 0;
}

Engine::SoundService::SoundService()
{
	m_pImpl = new SDLMixerPimpl();
}

Engine::SoundService::~SoundService()
{
	delete m_pImpl;
}

int Engine::SoundService::loadSound(const char* filename, SoundType soundType)
{
	return m_pImpl->loadSound(filename, soundType);
}

void Engine::SoundService::play(const sound_id id, const float volume, SoundType soundType)
{
	m_pImpl->play(id, volume, soundType);
}

void Engine::SoundService::stopAllSounds()
{
	m_pImpl->stopAllSounds();
}

void Engine::SoundService::update()
{
	m_pImpl->update();
}

void Engine::SoundService::Remove()
{
	delete m_pImpl;
}
