#pragma once

#include <unordered_map>
#include <string>
#include "fnv1a.h"
#include "../sdk/interfaces/interface.h"

namespace utils
{
	class library {
	public:
		// Default constructor
		library() = default;

		// Constructor that takes the name of the library (DLL)
		explicit library(const char* name) : module_name(name) {
			handle = GetModuleHandleA(name);
		}

		// Method to get the hash of the module name (compile-time hash)
		uint32_t get_module_hash() const {
			return utils::fnv1a(module_name);  // Use module_name member with FNV1A hash function
		}

		// Directly call sdk::get_interface with the hash of the module
		const sdk::interface_entry_t* get_interface() const {
			return sdk::get_interface(get_module_hash());
		}

		// Method to get the base address of the library at a specific offset
		uintptr_t at(const ptrdiff_t off) const {
			return reinterpret_cast<uintptr_t>(handle) + off;
		}

		// Method to dereference a pointer at a specific offset in the module
		uintptr_t deref(const ptrdiff_t off) const {
			return *reinterpret_cast<uintptr_t*>(reinterpret_cast<uintptr_t>(handle) + off);
		}

	private:
		HANDLE handle = nullptr;  // Handle to the loaded library
		const char* module_name;  // Name of the library (DLL)
	};

	//should prob do sigs here

	namespace VTable {

		namespace D3D11 {
			enum {
				PRESENT = 8U,
				RESIZEBUFFERS = 13U,
				RESIZEBUFFERS_CSTYLE = 39U,
			};
		}

		namespace DXGI {
			enum {
				CREATESWAPCHAIN = 10U,
			};
		}

		namespace client {
			enum {
				FRAMESTAGENOTIFY = 36U,
			};
		}

		namespace input {
			enum {
				CREATEMOVE = 21U,//5
				MOUSEINPUTENABLED = 19U,
				FRAMESTAGENOTIFY = 36U,
			};
		}

		namespace input_system {
			enum {
				ISRELATIVEMOUSEMODE = 76U,
			};
		}
	}
}