#pragma once
#include "Singleton.h"

namespace Engine
{
	class ElapsedTime : public Singleton<ElapsedTime>
	{
	public:
		void SetElapsedTime(float time);
		float GetElapsedTime();

		void SetStaticElapsedTime(float time);
		float GetStaticElapsedTime();
	private:
		friend class Singleton<ElapsedTime>;
		ElapsedTime() = default;
		float m_ElapsedSec;
		float m_StaticElapsedSec;
	};
}

