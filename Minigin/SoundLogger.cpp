#include "MiniginPCH.h"
#include "SoundLogger.h"

SoundLogger::SoundLogger(Sound* wrapped)
	:wrapped_(wrapped)
{}

SoundLogger::~SoundLogger()
{
	delete wrapped_;
}

int SoundLogger::loadSound(const char* filename, SoundType soundType)
{
	std::string type = "load sound of type: ";
	type += (soundType == SoundType::EFFECT) ? "effect" : "sound";
		
	log(type.c_str());
	return wrapped_->loadSound(filename, soundType);
}

void SoundLogger::play(const sound_id id, const float volume, SoundType soundType)
{
	std::string type = "play sound of type: ";
	type += (soundType == SoundType::EFFECT) ? "effect" : "sound";
	log(type.c_str());
	wrapped_->play(id, volume, soundType);
}

void SoundLogger::stopAllSounds()
{
	log("stop all sounds");
	wrapped_->stopAllSounds();
}

void SoundLogger::update()
{
	wrapped_->update();
}

void SoundLogger::Remove()
{
	wrapped_->Remove();
}

void SoundLogger::log(const char* message)
{
	std::cout << message << "\n";
}
