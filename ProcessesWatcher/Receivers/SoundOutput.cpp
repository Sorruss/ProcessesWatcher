#include "SoundOutput.hpp"

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace FG
{
	SoundOutput::SoundOutput(const std::wstring& soundOn, const std::wstring& soundOff)
		: m_eventOnSFX(soundOn), m_eventOffSFX(soundOff)
	{}

	void SoundOutput::OnEventStarted(const EventData& data)
	{
		PlaySound(m_eventOnSFX.c_str(), NULL, SND_FILENAME | SND_ASYNC);
	}

	void SoundOutput::OnEventEnded(const EventData & data)
	{
		PlaySound(m_eventOffSFX.c_str(), NULL, SND_FILENAME | SND_ASYNC);
	}
}
