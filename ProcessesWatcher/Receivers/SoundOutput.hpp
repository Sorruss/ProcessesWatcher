#pragma once

#include "../Interfaces/IWatcherSubscriber.hpp"

namespace FG
{
	class SoundOutput : public IWatcherSubscriber
	{
	private:
		std::wstring m_eventOnSFX;
		std::wstring m_eventOffSFX;

	public:
		SoundOutput(const std::wstring& soundOn, const std::wstring& soundOff);

		bool CheckIfFileExists(const std::wstring& filename);

		// --------------------------------
		// INHERITED VIA IWatcherSubscriber

		void OnEventStarted(const EventData& data) override;
		void OnEventEnded(const EventData& data) override;
	};
}
