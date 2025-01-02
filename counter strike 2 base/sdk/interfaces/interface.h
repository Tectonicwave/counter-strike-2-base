#pragma once

// Project-specific includes
#include "../utils/memory.h"

#include <cassert> 

namespace sdk
{
	using interface_fn = void* (__cdecl*)();

	struct interface_entry_t
	{
		interface_fn callback;
		const char* name;
		interface_entry_t* flink;
	};

	constexpr int _NUMBER_MAX_BASE = 36;

	using InstantiateInterfaceFn_t = void* (*)();

	static const interface_entry_t* get_interface(uint32_t mod)
	{
		std::uint8_t* create_interface = reinterpret_cast<std::uint8_t*>(utils::find_export(mod, FNV1A("CreateInterface")));

		if (create_interface == nullptr)
		{
			return nullptr;
		}

		return *reinterpret_cast<interface_entry_t**>(utils::solve_address(create_interface, 0x3, 0x7));
	}

	template <typename T = void*>
	T* find_interface_list(const interface_entry_t* pInterfaceList, const char* szInterfaceName)
	{
		if (pInterfaceList == nullptr) {
			#ifdef _DEBUG
				MessageBoxA(nullptr, utils::string_format("Module not found: %s", szInterfaceName).c_str(), "Error", MB_ICONERROR);
			#endif
			return nullptr; // Early return if the list is null
		}

		for (const interface_entry_t* pEntry = pInterfaceList; pEntry != nullptr; pEntry = pEntry->flink)
		{
			// Compare the interface name
			if (const std::size_t nInterfaceNameLength = strlen(szInterfaceName);
				strncmp(szInterfaceName, pEntry->name, nInterfaceNameLength) == 0 && (strlen(pEntry->name) == nInterfaceNameLength || atoi(pEntry->name + nInterfaceNameLength) > 0)) // Check for version suffix
			{
				return static_cast<T*>(pEntry->callback()); // Invoke the callback to get the interface
			}
		}

		#ifdef _DEBUG
				MessageBoxA(nullptr, utils::string_format("Module not found: %s", szInterfaceName).c_str(), "Error", MB_ICONERROR);
		#endif
		return nullptr; // Return null if no match is found
	}

}