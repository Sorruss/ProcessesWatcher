#include "ConsoleOutput.hpp"

namespace FG
{
	void ConsoleOutput::OnEventStarted(const EventData& data)
	{
		std::wcout << "[STARTED]: " << data.name << '\t';
		std::wcout << "id -> " << data.id << '\n';
	}

	void ConsoleOutput::OnEventEnded(const EventData& data)
	{
		std::wcout << "[ENDED]: " << data.name << '\n';
	}
}
