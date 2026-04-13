#include "FileOutput.hpp"

#include <filesystem>
#include <fstream>
#include <format>

#include "../Helpers.hpp"

namespace FG
{
	FileOutput::FileOutput(const std::string& filename)
		: m_filename(filename)
	{
		EnsureDirectoryExists();
		ClearFile();
	}

	// --------------------
	// FILE RELATED METHODS

	void FileOutput::ClearFile()
	{
		std::ofstream file(m_filename, std::ios::trunc);
		if (!file)
			throw std::runtime_error("[FileOutput::ClearFile] Failed to open file.");
	}

	void FileOutput::EnsureDirectoryExists()
	{
		std::filesystem::path path(m_filename);
		std::filesystem::path parent = path.parent_path();

		if (std::filesystem::exists(parent))
			return;

		std::filesystem::create_directories(parent);
	}

	// --------------------------------
	// INHERITED VIA IWatcherSubscriber

	void FileOutput::OnEventStarted(const EventData& data)
	{
		std::wofstream file(m_filename, std::ios::app);
		if (!file)
			throw std::runtime_error("[FileOutput::OnEventStarted] Failed to open file.");

		file << std::format(L"[STARTED] {} at time {}\n", data.name, FG::helpers::GetCurrentFormattedTime());
	}

	void FileOutput::OnEventEnded(const EventData & data)
	{
		std::wofstream file(m_filename, std::ios::app);
		if (!file)
			throw std::runtime_error("[FileOutput::OnEventEnded] Failed to open file.");

		file << std::format(L"[ENDED] {} at time {}\n", data.name, FG::helpers::GetCurrentFormattedTime());
	}
}
