#include "MiniginPCH.h"
#include "ElapsedTime.h"

void ElapsedTime::SetElapsedTime(float time)
{
	m_ElapsedSec = time;
}

float ElapsedTime::GetElapsedTime()
{
	return m_ElapsedSec;
}

void ElapsedTime::SetStaticElapsedTime(float time)
{
	m_StaticElapsedSec = time;
}

float ElapsedTime::GetStaticElapsedTime()
{
	return m_StaticElapsedSec;
}
