#include "schema_var.h"
#include <cassert>
#include "../../manager.h"
#include <filesystem>


using namespace utils;
using namespace sdk;

static constexpr const char* schema_modules[]{
	"client.dll",
	"engine2.dll",
	"schemasystem.dll"
};

int schema_t::find_feild(const char* class_name, const char* field) {
    // Attempt to find the field in the global type scope.
    int offset = find_field_type_scope(manager->schema_system->global_type_scope(), class_name, field);

    // If the field was found in the global scope, return its offset.
    if (offset)
        return offset;

    // If the field was not found in the global scope, iterate through schema modules.
    for (auto module : schema_modules) {
        // Retrieve the type scope for the current module.
        CSchemaSystemTypeScope* typescope = manager->schema_system->find_type_scope_for_module(module);

        // If the type scope is null, skip this module.
        if (!typescope)
            continue;

        // Attempt to find the field in the current module's type scope.
        int offset = find_field_type_scope(typescope, class_name, field);

        // If the field was found, return its offset.
        if (offset)
            return offset;
    }

    // If the field was not found in any type scope, return 0.
    return 0;
}


int schema_t::find_field_type_scope(CSchemaSystemTypeScope* typescope, const char* class_name, const char* field_name) {
    // Pointer to hold the class information.
    SchemaClassInfoData_t* class_info = nullptr;

    // Use the FindDeclaredClass function to populate class_info.
    typescope->find_declared_class(&class_info, class_name);

    // If class_info is null, the class wasn't found; return 0.
    if (!class_info)
        return 0;

    // Retrieve the fields and the number of fields in the class.
    auto fields_size = class_info->nFieldSize;
    SchemaClassFieldData_t* fields = class_info->pFields;

    // Iterate over the fields to find the one matching the given field_name.
    for (int i = 0; i < fields_size; i++) {
        SchemaClassFieldData_t& field = fields[i];

        // Compare field names and return the offset if a match is found.
        if (!strcmp(field.szName, field_name))
        {
            return field.nSingleInheritanceOffset;
        }
    }

    // If no field was found, return 0.
    return 0;
}