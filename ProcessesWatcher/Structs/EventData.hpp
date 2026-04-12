#pragma once

#include <iostream>
#include <comdef.h>

namespace FG
{
	struct EventData
	{
		std::wstring name;
		LONG id;

		EventData()
			: name(L""), id(0L)
		{}
	};
}
