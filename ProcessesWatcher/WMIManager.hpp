#define _WIN32_DCOM

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

namespace FG
{
	void InitializeCOM();
	void ObtainLocator(IWbemLocator*& locator);
	void ObtainServices(IWbemLocator*& locator, IWbemServices*& services, const bstr_t& bstrNamespace);

	void SubscribeSink(IWbemServices*& services, IWbemObjectSink*& sink, const bstr_t& bstrQuery);

	void Cleanup(IWbemLocator*& locator, IWbemServices*& services);
}
