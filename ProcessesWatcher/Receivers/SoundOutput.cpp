#include "SoundOutput.hpp"

#include <filesystem>

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

namespace FG
{
	SoundOutput::SoundOutput(const std::wstring& soundOn, const std::wstring& soundOff)
		: m_eventOnSFX(soundOn), m_eventOffSFX(soundOff)
	{
		if (!CheckIfFileExists(m_eventOnSFX) || !CheckIfFileExists(m_eventOffSFX))
			throw std::runtime_error("[SoundOutput::SoundOutput] WAV file doesn't exist.");
	}

	bool SoundOutput::CheckIfFileExists(const std::wstring & filename)
	{
		return std::filesystem::exists(filename);
	}

	void SoundOutput::OnEventStarted(const EventData& data)
	{
		if (!PlaySound(m_eventOnSFX.c_str(), NULL, SND_FILENAME | SND_ASYNC))
			throw std::runtime_error("[SoundOutput::OnEventStarted] Failed to play sound.");
	}

	void SoundOutput::OnEventEnded(const EventData & data)
	{
		if (!PlaySound(m_eventOffSFX.c_str(), NULL, SND_FILENAME | SND_ASYNC))
			throw std::runtime_error("[SoundOutput::OnEventEnded] Failed to play sound.");
	}
}
