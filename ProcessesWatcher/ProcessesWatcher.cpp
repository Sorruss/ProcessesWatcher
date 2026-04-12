#include "WMIManager.hpp"

int main()
{
    IWbemLocator* locator = 0;
    IWbemServices* services = 0;

    try
    {
        FG::InitializeCOM();
        FG::ObtainLocator(locator);
        FG::ObtainServices(locator, services);
    }
    catch (const std::runtime_error& error)
    {
        std::cerr << "[ERROR] " << error.what() << '\n';
        FG::Cleanup(locator, services);
        return 1;
    }
    catch (const std::exception& exception)
    {
        std::cerr << "[EXCEPTION] " << exception.what() << '\n';
        FG::Cleanup(locator, services);
        return 1;
    }
    catch (...)
    {
        std::cerr << "[UNKNOWN ERROR]\n";
        FG::Cleanup(locator, services);
        return 1;
    }

    FG::Cleanup(locator, services);
    return 0;
}
