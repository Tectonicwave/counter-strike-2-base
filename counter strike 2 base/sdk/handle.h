#pragma once

#include <assert.h>
#include <cstdint>

constexpr auto invalid_ehandle_index = 0xffffffff;
constexpr auto ent_entry_mask = 0x7fff;
constexpr auto num_serial_num_shift_bits = 15;
constexpr auto ent_max_networked_entry = 16384;

namespace sdk {
    template <typename T>
    class CHandle {
    public:
        uint32_t index;

        // Default constructor
        CHandle() noexcept : index(invalid_ehandle_index) {}

        // Parameterized constructor
        CHandle(const int nEntry, const int nSerial) noexcept {
            assert(nEntry >= 0 && (nEntry & ent_entry_mask) == nEntry);
            assert(nSerial >= 0 && nSerial < (1 << num_serial_num_shift_bits));
            index = nEntry | (nSerial << num_serial_num_shift_bits);
        }

        // Comparison operators
        bool operator!=(const CHandle& other) const noexcept {
            return index != other.index;
        }

        bool operator==(const CHandle& other) const noexcept {
            return index == other.index;
        }

        bool operator<(const CHandle& other) const noexcept {
            return index < other.index;
        }

        // Check if the handle is valid
        bool valid() const noexcept {
            return index != invalid_ehandle_index;
        }

        // Get the entry index, return a default value if invalid
        uint32_t get_entry_index() const noexcept {
            return valid() ? (index & ent_entry_mask) : ent_entry_mask;
        }

        // Get the serial number
        uint32_t get_serial_number() const noexcept {
            return index >> num_serial_num_shift_bits;
        }

        // Get the underlying object pointer
        T* get();

        // Get the pointer as another type
        template <typename C>
        C* get_as();
    };
}