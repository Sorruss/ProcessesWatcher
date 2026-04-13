#define _WIN32_DCOM

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#include <atlbase.h>

#pragma comment(lib, "wbemuuid.lib")

namespace FG
{
	class COMInitializer
	{
	public:
		COMInitializer();
		~COMInitializer();
	};

	CComPtr<IWbemLocator> ObtainLocator();
	CComPtr<IWbemServices> ObtainServices(const CComPtr<IWbemLocator>& locator, const bstr_t& bstrNamespace);

	void SubscribeSink(const CComPtr<IWbemServices>& services, IWbemObjectSink* sink, const bstr_t& bstrQuery);
}
