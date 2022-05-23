#pragma once
#include "Singleton.h"

class ElapsedTime : public dae::Singleton<ElapsedTime>
{
public:
	void SetElapsedTime(float time);
	float GetElapsedTime();

	void SetStaticElapsedTime(float time);
	float GetStaticElapsedTime();
private:
	ElapsedTime() = default;
	float m_ElapsedSec;
	float m_StaticElapsedSec;
};

