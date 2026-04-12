#include "WMIManager.hpp"
#include "ProcessesWatcher.hpp"

int main()
{
    IWbemLocator* locator = 0;
    IWbemServices* services = 0;
    IWbemObjectSink* watcher = new FG::ProcessesWatcher();

    try
    {
        FG::InitializeCOM();
        FG::ObtainLocator(locator);
        FG::ObtainServices(locator, services, L"ROOT\\CIMV2");
        FG::SubscribeSink(services, watcher, L"SELECT * FROM Win32_ProcessStartTrace");
        FG::SubscribeSink(services, watcher, L"SELECT * FROM Win32_ProcessStopTrace");

        std::cin.get();
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << "[ERROR] " << error.what() << '\n';
        FG::CleanSink(services, watcher);
        FG::Cleanup(locator, services);
        return 1;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "[EXCEPTION] " << exception.what() << '\n';
        FG::CleanSink(services, watcher);
        FG::Cleanup(locator, services);
        return 1;
    }
    catch (...)
    {
        std::cerr << "[UNKNOWN ERROR]\n";
        FG::CleanSink(services, watcher);
        FG::Cleanup(locator, services);
        return 1;
    }

    FG::CleanSink(services, watcher);
    FG::Cleanup(locator, services);
    return 0;
}
