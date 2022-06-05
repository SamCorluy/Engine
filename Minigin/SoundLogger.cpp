#include "MiniginPCH.h"
#include "SoundLogger.h"

Engine::SoundLogger::SoundLogger(Sound* wrapped)
	:wrapped_(wrapped)
{}

Engine::SoundLogger::~SoundLogger()
{
	delete wrapped_;
}

int Engine::SoundLogger::loadSound(const char* filename, SoundType soundType)
{
	std::string type = "load sound of type: ";
	type += (soundType == SoundType::EFFECT) ? "effect" : "sound";
		
	log(type.c_str());
	return wrapped_->loadSound(filename, soundType);
}

void Engine::SoundLogger::play(const sound_id id, const float volume, SoundType soundType)
{
	std::string type = "play sound of type: ";
	type += (soundType == SoundType::EFFECT) ? "effect" : "sound";
	log(type.c_str());
	wrapped_->play(id, volume, soundType);
}

void Engine::SoundLogger::stopAllSounds()
{
	log("stop all sounds");
	wrapped_->stopAllSounds();
}

void Engine::SoundLogger::update()
{
	wrapped_->update();
}

void Engine::SoundLogger::Remove()
{
	wrapped_->Remove();
}

void Engine::SoundLogger::log(const char* message)
{
	std::cout << message << "\n";
}
