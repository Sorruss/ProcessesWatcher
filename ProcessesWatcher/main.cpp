#include "WMIManager.hpp"
#include "ProcessesWatcher.hpp"

#include "Receivers/ConsoleOutput.hpp"
#include "Receivers/SoundOutput.hpp"
#include "Receivers/FileOutput.hpp"

int main()
{
    IWbemLocator* locator = 0;
    IWbemServices* services = 0;
    FG::ProcessesWatcher* watcher = new FG::ProcessesWatcher();
    IWbemObjectSink* watcherSink = watcher;

    try
    {
        // INITIALIZATION OF COM AND WMI
        FG::InitializeCOM();
        FG::ObtainLocator(locator);
        FG::ObtainServices(locator, services, L"ROOT\\CIMV2");

        // SUBSCRIBTION TO WMI EVENTS
        FG::SubscribeSink(services, watcherSink, L"SELECT * FROM Win32_ProcessStartTrace");
        FG::SubscribeSink(services, watcherSink, L"SELECT * FROM Win32_ProcessStopTrace");

        // SUBSCRIBTION TO CONSOLE OUTPUT
        auto consoleOutput = std::make_unique<FG::ConsoleOutput>();
        watcher->Subscribe(consoleOutput.get());

        // SUBSCRIBTION TO SOUND OUTPUT
        auto soundOutput = std::make_unique<FG::SoundOutput>(L"./media/sfx/on.wav", L"./media/sfx/off.wav");
        watcher->Subscribe(soundOutput.get());

        // SUBSCRIBTION TO FILE OUTPUT
        auto fileOutput = std::make_unique<FG::FileOutput>("./media/logs/fileOutput.txt");
        watcher->Subscribe(fileOutput.get());

        std::cin.get();
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << "[ERROR] " << error.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "[UNKNOWN ERROR]\n";
    }

    watcher->Cleanup(services);
    FG::Cleanup(locator, services);
    return 0;
}
