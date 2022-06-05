#include "MiniginPCH.h"
#include "ElapsedTime.h"

void Engine::ElapsedTime::SetElapsedTime(float time)
{
	m_ElapsedSec = time;
}

float Engine::ElapsedTime::GetElapsedTime()
{
	return m_ElapsedSec;
}

void Engine::ElapsedTime::SetStaticElapsedTime(float time)
{
	m_StaticElapsedSec = time;
}

float Engine::ElapsedTime::GetStaticElapsedTime()
{
	return m_StaticElapsedSec;
}
