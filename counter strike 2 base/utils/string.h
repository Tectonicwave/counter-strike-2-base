#pragma once
#include <string>
#include <cstdarg>
#include <ctime>
#include <cstdio>
#include <algorithm>

template <typename C> requires (std::is_same_v<C, char> || std::is_same_v<C, wchar_t>)
constexpr C* StringCharR(const C* tszSource, const int iSearch)
{
	C* tszLastOccurrence = nullptr;

	do
	{
		if (*tszSource == iSearch)
			tszLastOccurrence = const_cast<C*>(tszSource);
	} while (*tszSource++ != C('\0'));

	return tszLastOccurrence;
}

template <typename C> requires (std::is_same_v<C, char> || std::is_same_v<C, wchar_t>)
constexpr C* StringCat(C* tszDestination, const C* tszSource)
{
	while (*tszDestination != C('\0'))
		++tszDestination;

	while (*tszSource != C('\0'))
		*tszDestination++ = *tszSource++;

	*tszDestination = C('\0');
	return tszDestination;
}

template <typename C> requires (std::is_same_v<C, char> || std::is_same_v<C, wchar_t>)
constexpr C* StringCopy(C* tszDestination, const C* tszSource)
{
	while (*tszSource != C('\0'))
		*tszDestination++ = *tszSource++;

	*tszDestination = C('\0');
	return tszDestination;
}

template <size_t SIZE = _MAX_PATH>
struct String_t
{
    // Default constructor
    String_t()
    {
        szBuffer[0] = '\0';  // Initialize as an empty string
    }

    // Constructor from a C-string
    String_t(const char* szStr)
    {
        std::strncpy(szBuffer, szStr, SIZE - 1);  // Copy the string, ensuring null-termination
        szBuffer[SIZE - 1] = '\0';  // Ensure null-termination
    }

    // Constructor with formatting
    String_t(const char* szFormat, ...)
    {
        va_list args;
        va_start(args, szFormat);

        // Use vsnprintf to format into the buffer
        std::vsnprintf(szBuffer, SIZE, szFormat, args);
        va_end(args);
    }

    // Constructor with time formatting (example formatting)
    String_t(const char* szFormat, const std::tm* pTime)
    {
        std::strftime(szBuffer, SIZE, szFormat, pTime);
    }

    // Return the length of the string
    size_t Length() const
    {
        return std::strlen(szBuffer);
    }

    // Get the C-style string (const char*)
    const char* Data() const
    {
        return szBuffer;
    }

    // Append data to the string
    void Append(const char* szData)
    {
        size_t currentLen = std::strlen(szBuffer);
        size_t dataLen = std::strlen(szData);

        // Make sure we don't overflow the buffer
        if (currentLen + dataLen < SIZE)
        {
            std::strncpy(szBuffer + currentLen, szData, SIZE - currentLen - 1);
            szBuffer[currentLen + dataLen] = '\0';  // Ensure null-termination
        }
    }

    // Compare the string with another C-string
    int Compare(const char* szData) const
    {
        return std::strcmp(szBuffer, szData);
    }

    // Find a substring (returns a pointer to the first occurrence)
    const char* Find(const char* szSearch) const
    {
        const char* result = std::strstr(szBuffer, szSearch);
        return result ? result : nullptr;
    }

    // Convert the string to uppercase
    void ToUpper()
    {
        std::transform(szBuffer, szBuffer + std::strlen(szBuffer), szBuffer, ::toupper);
    }

    // Convert the string to lowercase
    void ToLower()
    {
        std::transform(szBuffer, szBuffer + std::strlen(szBuffer), szBuffer, ::tolower);
    }

private:
    char szBuffer[SIZE];
};
