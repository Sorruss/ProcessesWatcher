#define _WIN32_DCOM

#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

namespace FG
{
	void InitializeCOM();
	void ObtainLocator(IWbemLocator*& locator);
	void ObtainServices(IWbemLocator*& locator, IWbemServices*& services);
	void Cleanup(IWbemLocator*& locator, IWbemServices*& services);
}
