#pragma
#include <cstdint>
#include <array>
#include <utility>

#include "random.h"

namespace util
{
	template<typename T, T A, T B>
	class xor_value
	{
	public:
		__forceinline static T get() { return value ^ cipher; }

	private:
		volatile static const inline T value{ A ^ B }, cipher{ B };
	};

	template <size_t N, char K>
	struct xor_str
	{
	private:
		static constexpr char enc(const char c) { return c ^ K; }

	public:
		template <size_t... s>
		constexpr __forceinline xor_str(const char* str, std::index_sequence<s...>)
			: encrypted{ enc(str[s])... } { }

		__forceinline std::string dec()
		{
			std::string dec;
			dec.resize(N);

			for (auto i = 0u; i < N; i++)
				dec[i] = encrypted[i] ^ K;

			return dec;
		}

		__forceinline std::string ot(bool decrypt = true)
		{
			std::string dec;
			dec.resize(N);

			for (auto i = 0u; i < N; i++)
			{
				dec[i] = decrypt ? (encrypted[i] ^ K) : encrypted[i];
				encrypted[i] = '\0';
			}

			return dec;
		}

		volatile char encrypted[N];
	};
}

#define XOR_16(val) \
    (decltype(val))(::util::xor_value<uint16_t, (uint16_t)val, ::util::random::_uint<__COUNTER__, 0xFFFF>::value>::get())

#define XOR_32(val) \
    (decltype(val))(::util::xor_value<uint32_t, (uint32_t)val, ::util::random::_uint<__COUNTER__, 0xFFFFFFFF>::value>::get())

#define XOR_64(val) \
    (decltype(val))(::util::xor_value<uint64_t, (uint64_t)val, ::util::random::_uint64<__COUNTER__, 0xFFFFFFFFFFFFFFFF>::value>::get())

#define XOR_STR_S(s) ::util::xor_str<sizeof(s), ::util::random::_char<__COUNTER__>::value>(s, std::make_index_sequence<sizeof(s)>())
#define XOR_STR(s) XOR_STR_S(s).dec().c_str()
#define XOR_STR_OT(s) XOR_STR_S(s).ot().c_str()
#define XOR_FORMAT(s, ...) tfm::format(XOR_STR(s), __VA_ARGS__)
#define XOR_STR_STORE(s) []() -> std::pair<std::string, char> \
{ \
	constexpr auto key = ::util::random::_char<__COUNTER__>::value; \
	return std::make_pair(::util::xor_str<sizeof(s), key>(s, std::make_index_sequence<sizeof(s)>()).ot(false), key); \
}()
#define XOR_STR_STACK(n, s) auto (n) = reinterpret_cast<char*>(alloca(((s).first.size() + 1) * sizeof(char))); \
	for (size_t i = 0; i < (s).first.size(); i++) \
        (n)[i] = (s).first[i] ^ (s).second; \
    (n)[(s).first.size()] = '\0'
#define XOR_STR_STACK_WIDE(n, s) auto (n) = reinterpret_cast<wchar_t*>(alloca(((s).first.size() + 1) * sizeof(wchar_t))); \
	for (size_t i = 0; i < (s).first.size(); i++) \
        (n)[i] = (s).first[i] ^ (s).second; \
    (n)[(s).first.size()] = '\0'

#ifdef _DEBUG
#define XOR(STRING) STRING
#else
#define XOR(s) XOR_STR(s)
#endif