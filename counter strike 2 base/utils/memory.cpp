// Standard library includes
#include <locale>
#include <codecvt>
#include <filesystem>

// Project-specific includes
#include "Memory.h"

typedef struct _LDR_MODULE
{
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID BaseAddress;
	PVOID EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING FullDllName;
	UNICODE_STRING BaseDllName;
	ULONG Flags;
	SHORT LoadCount;
	SHORT TlsIndex;
	LIST_ENTRY HashTableEntry;
	ULONG TimeDateStamp;
} LDR_MODULE, * PLDR_MODULE;

typedef struct _PEB_LOADER_DATA
{
	ULONG Length;
	BOOLEAN Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
} PEB_LOADER_DATA, * PPEB_LOADER_DATA;

namespace utils
{
	inline std::string to_utf8(const std::wstring& wstr)
	{
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
		std::string str(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &str[0], size_needed, nullptr, nullptr);
		return str;
	}

	inline std::wstring to_wstring(const std::string& str)
	{
		int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
		std::wstring wstr(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
		return wstr;
	}

	std::optional<module_t> find_module(uint32_t hash)
	{
		const auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;
		if (!peb)
			return {};

		const auto ldr = reinterpret_cast<PEB_LOADER_DATA*>(peb->Ldr);
		if (!ldr)
			return {};

		const auto head = &ldr->InLoadOrderModuleList;
		auto cur = head->Flink;

		while (cur != head)
		{
			const auto mod = CONTAINING_RECORD(cur, LDR_MODULE, InLoadOrderModuleList);

			// Convert the wide string to a UTF-8 string using to_utf8 function
			std::filesystem::path module_path(mod->FullDllName.Buffer);
			std::wstring wide_name = module_path.filename().wstring();
			std::string name = to_utf8(wide_name);  // Convert to UTF-8 string

			std::transform(name.begin(), name.end(), name.begin(), ::tolower);  // Convert to lowercase

			// Compare the name hash
			if (FNV1A_CMP_IM(name.c_str(), hash))
				return module_t{ reinterpret_cast<uintptr_t>(mod->BaseAddress), mod->SizeOfImage };

			cur = cur->Flink;
		}

		return {};  // Return an empty optional if no match is found
	}

	std::string convert_wstring_to_string(const std::wstring& wstr) {
		if (wstr.empty()) return std::string();
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string str_to(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &str_to[0], size_needed, NULL, NULL);
		return str_to;
	}


	uintptr_t get_module_base_handle(uint32_t* wszModuleName)
	{
		const auto peb = NtCurrentTeb()->ProcessEnvironmentBlock;
		if (!peb)
			return 0;

		// If no module name is specified, return the base address of the main executable
		if (!wszModuleName) {
			return reinterpret_cast<uintptr_t>(peb->Reserved3[1]); // Main executable's ImageBaseAddress
		}

		// Traverse the PEB's InMemoryOrderModuleList to find the specified module
		for (LIST_ENTRY* entry = peb->Ldr->InMemoryOrderModuleList.Flink;
			entry != &peb->Ldr->InMemoryOrderModuleList;
			entry = entry->Flink) {

			const _LDR_DATA_TABLE_ENTRY* module_entry =
				CONTAINING_RECORD(entry, _LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);

			// Ensure module entry and FullDllName.Buffer are valid
			if (module_entry->FullDllName.Buffer) {
				std::wstring module_name(module_entry->FullDllName.Buffer);

				// Convert std::wstring to std::string (narrow string) using the conversion function
				std::string module_name_str = convert_wstring_to_string(module_name);

				// Calculate the hash of the module's name using fnv1a
				uint32_t module_hash = fnv1a(module_name_str.c_str());

				// Compare the module's hash with the requested hash (case insensitive)
				if (module_hash == *wszModuleName) {
					return reinterpret_cast<uintptr_t>(module_entry->DllBase);  // Return the module base address
				}
			}
		}

		// If the module was not found, return 0
		return 0;
	}

	uintptr_t find_export(uint32_t hash, uint32_t exp)
	{
		const auto mod = find_module(hash);
		if (!mod)
			return 0;

		const auto base = mod->base;
		const auto dos = reinterpret_cast<PIMAGE_DOS_HEADER>(base);
		const auto nt = reinterpret_cast<PIMAGE_NT_HEADERS>(base + dos->e_lfanew);

		if (dos->e_magic != IMAGE_DOS_SIGNATURE || nt->Signature != IMAGE_NT_SIGNATURE)
			return 0;

		const auto exports = reinterpret_cast<PIMAGE_EXPORT_DIRECTORY>(base + nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		const auto names = reinterpret_cast<uint32_t*>(base + exports->AddressOfNames);

		auto ordinal_index = static_cast<size_t>(-1);
		for (uint32_t i = 0; i < exports->NumberOfFunctions; i++)
			if (FNV1A_CMP_IM(reinterpret_cast<const char*>(base + names[i]), exp))
			{
				ordinal_index = i;
				break;
			}

		if (ordinal_index > exports->NumberOfFunctions)
			return 0;

		const auto ordinals = reinterpret_cast<uint16_t*>(base + exports->AddressOfNameOrdinals);
		const auto addresses = reinterpret_cast<uint32_t*>(base + exports->AddressOfFunctions);
		return static_cast<uintptr_t>(base + addresses[ordinals[ordinal_index]]);
	}

	// Utility function to convert hex characters to byte values
	uint8_t CharToHexInt(char c) {
		c |= ('a' ^ 'A');  // Convert to lowercase
		return (c >= 'a') ? (c - 'a' + 10) : (c - '0');
	}

	// Optimized pattern parsing function
	void parse_pattern_optimized(const char* pattern, std::vector<uint8_t>& bytes, std::vector<char>& mask) {
		size_t byteIndex = 0;

		while (*pattern) {
			// Skip spaces
			if (*pattern == ' ') {
				++pattern;
				continue;
			}

			// Wildcard parsing
			if (*pattern == '?') {
				bytes.push_back(0U);  // Wildcard (0x00)
				mask.push_back('?');
				++pattern; // Skip the second '?' if it's present
				if (*pattern == '?') ++pattern;
			}
			// Valid hex byte parsing
			else if (isxdigit(pattern[0]) && isxdigit(pattern[1])) {
				uint8_t byte = (CharToHexInt(pattern[0]) << 4) | CharToHexInt(pattern[1]);
				bytes.push_back(byte);
				mask.push_back('x');
				pattern += 2;  // Skip the parsed byte
			}
			else {
				// Invalid input handling (optional)
				throw std::invalid_argument("Invalid pattern format");
			}
		}
	}

	static uintptr_t resolve_relative(uintptr_t address, int offset, int instructionSize) {
		uint32_t relativeOffset = *reinterpret_cast<uint32_t*>(address + offset);
		uintptr_t instructionPointer = address + instructionSize;

		return instructionPointer + relativeOffset;
	}

	// Find the pattern in the specified module
	std::uint8_t* find_pattern(uint32_t wszModuleName, const char* szPattern) {
		const size_t maxPatternSize = 512; // Max pattern size

		// Use std::vector for dynamic memory allocation
		std::vector<uint8_t> pattern_bytes;
		std::vector<char> pattern_mask;

		// Parse the pattern
		parse_pattern_optimized(szPattern, pattern_bytes, pattern_mask);

		// Find module base and size
		auto module = find_module(wszModuleName);
		if (!module.has_value()) {

#ifdef _DEBUG
			MessageBoxA(nullptr, string_format("Module not found: %s", szPattern).c_str(), "Error", MB_ICONERROR);
#endif

			return nullptr; // Module not found, return null
		}

		const uint8_t* module_base = reinterpret_cast<const uint8_t*>(module->base);
		size_t module_size = module->size;

		// Optimized scanning using pattern bytes and mask
		for (size_t i = 0; i < module_size - pattern_bytes.size(); ++i) {
			bool found = true;

			// Check the bytes with a fast comparison using the mask
			for (size_t j = 0; j < pattern_bytes.size(); ++j) {
				if (pattern_mask[j] == 'x' && module_base[i + j] != pattern_bytes[j]) {
					found = false;
					break;
				}
			}

			if (found) {
				return const_cast<std::uint8_t*>(module_base + i); // Pattern found, return address
			}
		}

#ifdef _DEBUG
		MessageBoxA(nullptr, string_format("Pattern not found: %s", szPattern).c_str(), "Error", MB_ICONERROR);
#endif

		return nullptr; // Pattern not found
	}
}