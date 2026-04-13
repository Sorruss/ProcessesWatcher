#include "WMIManager.hpp"

namespace FG
{
    COMInitializer::COMInitializer()
    {
        // COM INIIALIZATION
        HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);
        if (FAILED(hres))
            throw std::runtime_error("Failed to initialize COM library");

        // COM SECURITY INITIALIZATION
        hres = CoInitializeSecurity(
            NULL,
            -1,                             // COM negotiates service                  
            NULL,                           // Authentication services
            NULL,                           // Reserved
            RPC_C_AUTHN_LEVEL_DEFAULT,      // authentication
            RPC_C_IMP_LEVEL_IMPERSONATE,    // Impersonation
            NULL,                           // Authentication info 
            EOAC_NONE,                      // Additional capabilities
            NULL                            // Reserved
        );
        if (FAILED(hres))
            throw std::runtime_error("Failed to initialize security");
    }

    COMInitializer::~COMInitializer()
    {
        CoUninitialize();
    }

    CComPtr<IWbemLocator> ObtainLocator()
    {
        CComPtr<IWbemLocator> locator = 0;
        HRESULT hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator, (LPVOID*)&locator);
        if (FAILED(hres))
            throw std::runtime_error("Failed to create IWbemLocator object");

        return locator;
    }

    CComPtr<IWbemServices> ObtainServices(const CComPtr<IWbemLocator>& locator, const bstr_t& bstrNamespace)
    {
        CComPtr<IWbemServices> services = 0;
        HRESULT hres = locator->ConnectServer(
            bstrNamespace,           // WMI namespace
            NULL,                    // User name
            NULL,                    // User password
            0,                       // Locale
            NULL,                    // Security flags                 
            0,                       // Authority       
            0,                       // Context object
            &services                // IWbemServices proxy
        );
        if (FAILED(hres))
            throw std::runtime_error("Could not connect to namespace.");

        hres = CoSetProxyBlanket(
            services,                     // the proxy to set
            RPC_C_AUTHN_WINNT,            // authentication service
            RPC_C_AUTHZ_NONE,             // authorization service
            NULL,                         // Server principal name
            RPC_C_AUTHN_LEVEL_CALL,       // authentication level
            RPC_C_IMP_LEVEL_IMPERSONATE,  // impersonation level
            NULL,                         // client identity 
            EOAC_NONE                     // proxy capabilities     
        );
        if (FAILED(hres))
            throw std::runtime_error("Could not set proxy blanket.");

        return services;
    }

    void SubscribeSink(const CComPtr<IWbemServices>& services, IWbemObjectSink* sink, const bstr_t& bstrQuery)
    {
        HRESULT hres = services->ExecNotificationQueryAsync(
            bstr_t("WQL"),
            bstrQuery,
            WBEM_FLAG_SEND_STATUS,
            NULL,
            sink
        );
        if (FAILED(hres))
        {
            services->CancelAsyncCall(sink);
            throw std::runtime_error("[WMIManager::SubscribeSink] Subscribtion to events failed.");
        }
    }
}
