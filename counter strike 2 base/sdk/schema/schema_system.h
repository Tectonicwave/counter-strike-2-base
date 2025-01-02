#pragma once

#include "../../utils/memory.h"
#include "../../utils/utl/Utlthash.h"
#include "../../utils/utl/UtlVector.hpp"

#define SCHEMASYSTEM_TYPE_SCOPES_OFFSET 0x188
#define SCHEMASYSTEMTYPESCOPE_OFF1 0x3F8
#define SCHEMASYSTEMTYPESCOPE_OFF2 0x8

namespace sdk
{
	using SchemaString_t = const char*;
	struct SchemaMetadataEntryData_t;

	class CSchemaSystemTypeScope;
	class CSchemaType;

	struct CSchemaClassBinding
	{
		CSchemaClassBinding* pParent;
		const char* szBinaryName; // ex: C_World
		const char* szModuleName; // ex: libclient.so
		const char* szClassName; // ex: client
		void* pClassInfoOldSynthesized;
		void* pClassInfo;
		void* pThisModuleBindingPointer;
		CSchemaType* pSchemaType;
	};

	class CSchemaType
	{
	public:
		bool get_sizes(int* pOutSize, uint8_t* unkPtr)
		{
			return utils::CallVFunc<bool, 3U>(this, pOutSize, unkPtr);
		}

	public:
		bool get_size(int* out_size)
		{
			uint8_t smh = 0;
			return get_sizes(out_size, &smh);
		}

	public:
		void* pVtable; // 0x0000
		const char* szName; // 0x0008

		CSchemaSystemTypeScope* pSystemTypeScope; // 0x0010
		uint8_t nTypeCategory; // ETypeCategory 0x0018
		uint8_t nAatomicCategory; // EAtomicCategory 0x0019
	};

	struct SchemaClassFieldData_t
	{
		SchemaString_t szName; // 0x0000
		CSchemaType* pSchemaType; // 0x0008
		std::uint32_t nSingleInheritanceOffset; // 0x0010
		std::int32_t nMetadataSize; // 0x0014
		SchemaMetadataEntryData_t* pMetaData; // 0x0018
	};

	struct SchemaClassInfoData_t;

	struct SchemaBaseClassInfoData_t
	{
		int32_t nOffset;
		SchemaClassInfoData_t* pClass;
	};

	struct SchemaClassInfoData_t
	{
	private:
		void* pVtable; // 0x0000
	public:
		const char* szName; // 0x0008
		char* szDescription; // 0x0010

		int m_nSize; // 0x0018
		std::int16_t nFieldSize; // 0x001C
		std::int16_t nStaticSize; // 0x001E
		std::int16_t nMetadataSize; // 0x0020

		std::uint8_t nAlignOf; // 0x0022
		std::uint8_t nBaseClassesCount; // 0x0023
		char pad2[0x4]; // 0x0024
		SchemaClassFieldData_t* pFields; // 0x0028
		char pad3[0x8]; // 0x0030
		SchemaBaseClassInfoData_t* pBaseClasses; // 0x0038
		char pad4[0x28]; // 0x0040

		bool InheritsFrom(SchemaClassInfoData_t* pClassInfo)
		{
			if (pClassInfo == this && pClassInfo != nullptr)
				return true;
			else if (pBaseClasses == nullptr || pClassInfo == nullptr)
				return false;

			for (int i = 0; i < nBaseClassesCount; i++)
			{
				auto& baseClass = pBaseClasses[i];
				if (baseClass.pClass->InheritsFrom(pClassInfo))
					return true;
			}

			return false;
		}
	};

	struct SchemaEnumeratorInfoData_t
	{
		SchemaString_t szName;

		union
		{
			unsigned char value_char;
			unsigned short value_short;
			unsigned int value_int;
			unsigned long long value;
		};

		Memory_pad(0x10); // 0x0010
	};

	class CSchemaEnumInfo
	{
	public:
		SchemaEnumeratorInfoData_t enumInfoData;
	};

	class CSchemaEnumBinding
	{
	public:
		virtual const char* GetBindingName() = 0;
		virtual CSchemaClassBinding* AsClassBinding() = 0;
		virtual CSchemaEnumBinding* AsEnumBinding() = 0;
		virtual const char* GetBinaryName() = 0;
		virtual const char* GetProjectName() = 0;

	public:
		char* szBindingName_; // 0x0008
		char* szDllName_; // 0x0010
		std::int8_t nAlign_; // 0x0018
		Memory_pad(0x3); // 0x0019
		std::int16_t nSize_; // 0x001C
		std::int16_t nFlags_; // 0x001E
		SchemaEnumeratorInfoData_t* pEnumInfo_;
		Memory_pad(0x8); // 0x0028
		CSchemaSystemTypeScope* pTypeScope_; // 0x0030
		Memory_pad(0x8); // 0x0038
		std::int32_t unk1_; // 0x0040
	};

	class CSchemaSystemTypeScope
	{
	public:
		void find_declared_class(SchemaClassInfoData_t** p_return_class, const char* sz_class_name)
		{
			return utils::CallVFunc<void, 2U>(this, p_return_class, sz_class_name);
		}

		CSchemaType* find_schema_type_by_name(const char* sz_name, std::uintptr_t* p_schema)
		{
			return utils::CallVFunc<CSchemaType*, 4U>(this, sz_name, p_schema);
		}

		CSchemaType* find_type_declared_class(const char* sz_name)
		{
			return utils::CallVFunc<CSchemaType*, 5U>(this, sz_name);
		}

		CSchemaType* find_type_declared_enum(const char* sz_name)
		{
			return utils::CallVFunc<CSchemaType*, 6U>(this, sz_name);
		}

		CSchemaClassBinding* find_raw_class_binding(const char* sz_name)
		{
			return utils::CallVFunc<CSchemaClassBinding*, 7U>(this, sz_name);
		}


		void* pVtable; // 0x0000
		char szName[256U]; // 0x0008
		Memory_pad(SCHEMASYSTEMTYPESCOPE_OFF1); // 0x0108
		utils::CUtlTSHash<CSchemaClassBinding*, 256, unsigned int> hash_classes; // 0x0588
		Memory_pad(SCHEMASYSTEMTYPESCOPE_OFF2); // 0x05C8
		utils::CUtlTSHash<CSchemaEnumBinding*, 256, unsigned int> hash_enumes; // 0x2DD0
	};

	class ISchemaSystem
	{
	public:
		CSchemaSystemTypeScope* global_type_scope()
		{
			return utils::CallVFunc<CSchemaSystemTypeScope*, 11U>(this);
		}

		CSchemaSystemTypeScope* find_type_scope_for_module(const char* m_module_name)
		{
			return utils::CallVFunc<CSchemaSystemTypeScope*, 13U>(this, m_module_name, nullptr);
		}

	private:
		Memory_pad(SCHEMASYSTEM_TYPE_SCOPES_OFFSET); // 0x0000
	public:
		// table of type scopes
		utils::CUtlVector<CSchemaSystemTypeScope*> vecTypeScopes;
	};

}
