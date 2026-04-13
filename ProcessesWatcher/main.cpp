#include "WMIManager.hpp"
#include "ProcessesWatcher.hpp"

#include "Receivers/ConsoleOutput.hpp"
#include "Receivers/SoundOutput.hpp"
#include "Receivers/FileOutput.hpp"

#include "Helpers.hpp"

int main()
{
    FG::COMInitializer com;
    CComPtr<IWbemLocator> locator = 0;
    CComPtr<IWbemServices> services = 0;
    FG::ProcessesWatcher* watcher = new FG::ProcessesWatcher();

    try
    {
        // INITIALIZATION OF COM AND WMI
        locator = FG::ObtainLocator();
        services = FG::ObtainServices(locator, L"ROOT\\CIMV2");

        // SUBSCRIBTION TO WMI EVENTS
        FG::SubscribeSink(services, watcher, L"SELECT * FROM Win32_ProcessStartTrace");
        FG::SubscribeSink(services, watcher, L"SELECT * FROM Win32_ProcessStopTrace");

        // SUBSCRIBTION TO PROCESSES WATCHER
        auto consoleOutput = FG::helpers::CreateShared<FG::ConsoleOutput>();
        auto soundOutput = FG::helpers::CreateShared<FG::SoundOutput>(L"./media/sfx/on.wav", L"./media/sfx/off.wav");
        auto fileOutput = FG::helpers::CreateShared<FG::FileOutput>("./media/logs/fileOutput.txt");

        if (consoleOutput) watcher->Subscribe(consoleOutput);
        if (soundOutput) watcher->Subscribe(soundOutput);
        if (fileOutput) watcher->Subscribe(fileOutput);

        Sleep(INFINITE);
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << "[ERROR] " << error.what() << '\n';
    }
    catch (const std::exception& exception)
    {
        std::cerr << "[EXCEPTION] " << exception.what() << '\n';
    }
    catch (...)
    {
        std::cerr << "[UNKNOWN ERROR]\n";
    }

    if (services && watcher)
    {
        services->CancelAsyncCall(watcher);
        watcher->Release();
    }
    
    return 0;
}
