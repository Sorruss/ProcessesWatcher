#pragma once

#include <memory>
#include <iostream>

namespace FG
{
	namespace helpers
	{
        template<typename T, typename... Args>
		std::shared_ptr<T> CreateShared(Args&&... args)
		{
            try
            {
                return std::make_shared<T>(std::forward<Args>(args)...);
            }
            catch (const std::exception& e)
            {
                std::cerr << "[CreateShared] FAILED: " << e.what() << '\n';
                return nullptr;
            }
		}

        std::wstring GetCurrentFormattedTime();
	}
}
