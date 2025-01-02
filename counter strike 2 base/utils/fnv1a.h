#pragma once
#include "../framework.h"

using FNV1A_t = std::uint64_t;

namespace utils
{
	inline constexpr uint32_t fnv1a_seed = 2166136261u;
	inline constexpr FNV1A_t ullBasis = 0xCBF29CE484222325ULL;
	inline constexpr FNV1A_t ullPrime = 0x100000001B3ULL;
	inline constexpr uint32_t fnv1a_prime = 16777619;

	consteval FNV1A_t HashConst(const char* szString, const FNV1A_t uKey = ullBasis) noexcept
	{
		return (szString[0] == '\0') ? uKey : HashConst(&szString[1], (uKey ^ static_cast<FNV1A_t>(szString[0])) * ullPrime);
	}

	inline uint32_t fnv1a(const char* key)
	{
		if (!key)
			return 0u;

		const char* data = const_cast<char*>(key);
		auto hash = fnv1a_seed;

		for (size_t i = 0; i < strlen(key); ++i)
		{
			const uint8_t value = data[i];
			hash = hash ^ value;
			hash *= fnv1a_prime;
		}

		return hash;
	}

	constexpr uint32_t fnv1a_ct(const char* str, const uint32_t value = fnv1a_seed) noexcept
	{
		return !*str ? value : fnv1a_ct(str + 1, static_cast<unsigned>(1ull * (value ^ static_cast<uint8_t>(*str)) * fnv1a_prime));
	}

}

#define FNV1A(s) (CONSTANT(::utils::fnv1a_ct(s)))
#define FNV1A_CMP(a, b) (CONSTANT(::utils::fnv1a_ct(b)) == ::utils::fnv1a(a))
#define FNV1A_CMP_IM(a, b) (b == ::utils::fnv1a(a))