#pragma once

#include "value_obfucation.h"
#include "fnv1a.h"
#include "../definitions.h"

namespace utils
{
	struct module_t
	{
		uintptr_t base;
		uint32_t size;
	};

	uintptr_t get_module_base_handle(uint32_t* wszModuleName);
	std::optional<module_t> find_module(uint32_t hash);
	uintptr_t find_export(uint32_t hash, uint32_t exp);

	inline std::uint8_t* solve_address(std::uint8_t* nAddressBytes, std::uint32_t nRVAOffset, std::uint32_t nRIPOffset)
	{
		// Read the RVA value from the specified offset
		std::uint32_t nRVA = *reinterpret_cast<std::uint32_t*>(nAddressBytes + nRVAOffset);

		// Compute the RIP value
		std::uint64_t nRIP = reinterpret_cast<std::uint64_t>(nAddressBytes) + nRIPOffset;

		// Return the calculated address
		return reinterpret_cast<std::uint8_t*>(nRVA + nRIP);
	}

	template <typename... Args>
	inline std::string string_format(const char* fmt, const Args&... args)
	{
		// Get the size of the formatted string by using a temporary buffer
		int size = std::snprintf(nullptr, 0, fmt, args...);
		if (size <= 0)
			return ""; // Return empty string if formatting fails

		std::string result(size, '\0'); // Create a string with the required size
		std::snprintf(&result[0], size + 1, fmt, args...); // Fill the string with formatted data

		return result;
	}

	std::uint8_t* find_pattern(uint32_t wszModuleName, const char* szPattern);

	template <typename T = std::uint8_t>
	T* get_absolute_address(T* pRelativeAddress, int nPreOffset = 0x0, int nPostOffset = 0x0)
	{
		pRelativeAddress += nPreOffset;
		pRelativeAddress += sizeof(std::int32_t) + *reinterpret_cast<std::int32_t*>(pRelativeAddress);
		pRelativeAddress += nPostOffset;
		return pRelativeAddress;
	}

	template <typename T, std::size_t nIndex, class CBaseClass, typename... Args_t>
	static __forceinline T CallVFunc(CBaseClass* thisptr, Args_t... argList)
	{
		using VirtualFn_t = T(__thiscall*)(const void*, decltype(argList)...);
		return (*reinterpret_cast<VirtualFn_t* const*>(reinterpret_cast<std::uintptr_t>(thisptr)))[nIndex](thisptr, argList...);
	}

	template <typename T = void*>
	__forceinline T GetVFunc(const void* thisptr, std::size_t nIndex)
	{
		return (*static_cast<T* const*>(thisptr))[nIndex];
	}
}