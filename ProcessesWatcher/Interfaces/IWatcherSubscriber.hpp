#pragma once

#include "../Structs/EventData.hpp"

namespace FG
{
	class IWatcherSubscriber
	{
	public:
		virtual void OnEventStarted(const EventData& data) = 0;
		virtual void OnEventEnded(const EventData& data) = 0;
	};
}
