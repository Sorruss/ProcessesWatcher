#include "FileOutput.hpp"

#include <filesystem>
#include <fstream>
#include <format>

#include <chrono>

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
		std::ofstream file(m_filename);
		file.clear();
		file.close();
	}

	void FileOutput::EnsureDirectoryExists()
	{
		std::filesystem::path path(m_filename);
		std::filesystem::path parent = path.parent_path();

		if (std::filesystem::exists(parent))
			return;

		std::filesystem::create_directories(parent);
	}

	// -------------
	// SUPPLEMENTARY

	std::wstring FileOutput::GetCurrentFormattedTime()
	{
		auto now = std::chrono::system_clock::now();
		auto time_t = std::chrono::system_clock::to_time_t(now);
		std::tm tm;
		localtime_s(&tm, &time_t);

		std::wostringstream wss;
		wss << std::put_time(&tm, L"%H:%M:%S");
		return wss.str();
	}

	// --------------------------------
	// INHERITED VIA IWatcherSubscriber

	void FileOutput::OnEventStarted(const EventData& data)
	{
		std::wofstream file(m_filename, std::ios::app);
		file << std::format(L"[STARTED] {} at time {}\n", data.name, GetCurrentFormattedTime());
		file.close();
	}

	void FileOutput::OnEventEnded(const EventData & data)
	{
		std::wofstream file(m_filename, std::ios::app);
		file << std::format(L"[ENDED] {} at time {}\n", data.name, GetCurrentFormattedTime());
		file.close();
	}
}
