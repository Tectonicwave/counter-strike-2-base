#pragma once
#include <cstdint>
#include "../../utils/fnv1a.h"
#include "schema_system.h"

class schema_t
{
public:
	int find_feild(const char* class_name, const char* field);
	int find_field_type_scope(sdk::CSchemaSystemTypeScope* typescope, const char* class_name, const char* field_name);
    bool Setup(const wchar_t* wszFileName, const char* szModuleName);
};

inline schema_t* schema_dump = new schema_t;

// SCHEMA Macros
#define SCHEMA(type, name, class_name, field)                                      \
    type& get_##name() {                                                                \
        /* Find the field offset using schema_dump */                             \
        static auto offset = schema_dump->find_feild(class_name, field);           \
        return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
    }

#define SCHEMA_OFFSET(type, name, class_name, field, eoffset)                      \
    type& get_##name() {                                                                \
        /* Find the field offset and add the extra offset */                      \
        static auto offset = schema_dump->find_feild(class_name, field);           \
        return *reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset + eoffset); \
    }

#define SCHEMA_P(type, name, class_name, field)                                   \
    type* get_##name() {                                                               \
        /* Find the field offset using schema_dump */                            \
        static auto offset = schema_dump->find_feild(class_name, field);          \
        return reinterpret_cast<type*>(reinterpret_cast<uintptr_t>(this) + offset); \
    }