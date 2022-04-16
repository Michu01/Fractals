#pragma once
#include <sstream>

namespace Utility
{
	template<class T>
	std::string ToString(const T& value)
	{
		std::ostringstream oss;
		oss << value;
		return oss.str();
	}
}