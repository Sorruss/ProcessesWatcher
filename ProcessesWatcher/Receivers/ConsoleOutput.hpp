#pragma once

#include "../Interfaces/IWatcherSubscriber.hpp"

namespace FG
{
	class ConsoleOutput : public IWatcherSubscriber
	{
	public:
		// --------------------------------
		// INHERITED VIA IWatcherSubscriber

		void OnEventStarted(const EventData& data) override;
		void OnEventEnded(const EventData& data) override;
	};
}
