#pragma once

#include "../Interfaces/IWatcherSubscriber.hpp"

namespace FG
{
	class FileOutput : public IWatcherSubscriber
	{
	private:
		std::string m_filename;

	public:
		FileOutput(const std::string& filename);

		// --------------------
		// FILE RELATED METHODS

		void ClearFile();
		void EnsureDirectoryExists();

		// -------------
		// SUPPLEMENTARY

		std::wstring GetCurrentFormattedTime();

		// --------------------------------
		// INHERITED VIA IWatcherSubscriber

		void OnEventStarted(const EventData& data) override;
		void OnEventEnded(const EventData& data) override;
	};
}
