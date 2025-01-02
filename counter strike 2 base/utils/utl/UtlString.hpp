#pragma once

#include <cstdint>
#include <cstring>

#include "UtlMemory.hpp"
#include "../../definitions.h"

#include <cstring>
#include <cstdarg>
#include <limits>
#include <string>

// Updated CUtlBinaryBlock
namespace utils
{
	class CUtlString
	{
	public:
		const char* get() const { return mem.memory; }

		CUtlMemory<char> mem;
		int actual_length;
	};

}
