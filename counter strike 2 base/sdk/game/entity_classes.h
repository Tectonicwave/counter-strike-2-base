#pragma once

#include "../handle.h"
#include "../../definitions.h"
#include "../../utils/memory.h"
#include "../schema/schema_var.h"
#include "../sdk/maths/vector.h"

#include <cstdint>
#include "../../utils/utl/UtlString.hpp"

//using namespace utils;// Brings all symbols from the utils namespace into scope.

namespace sdk
{
	//cs2 max players still 64 you can check the entity list in ida
	constexpr uint16_t max_players = 64;

	using GameTime_t = std::float_t;
	using GameTick_t = std::int32_t;

	//forward the class
	class cuser_cmd;

	class CEntityIdentity
	{
	public:


	};

	class CEntityInstance
	{
	public:

		void get_schema_class_info(sdk::SchemaClassInfoData_t** pReturn)
		{
			return utils::CallVFunc<void, 38U>(this, pReturn);
		}


	};

	class CCollisionProperty {
	public:



	};

	struct cgame_entity_system;
	//class C_CSPlayerPawn;
	struct cgame_resource_service
	{
		Memory_pad(0x58);
		cgame_entity_system* game_entity_system;
	};

	class CPlayerPawnComponent {
	public:


	}; // class CPlayerPawnComponent

	// forward decleration
	class C_BasePlayerWeapon;
	class CPlayer_WeaponServices : public CPlayerPawnComponent {
	public:



	}; // class CPlayer_WeaponServices

	class CCSPlayer_WeaponServices : public CPlayer_WeaponServices {
	public:



	}; // class CCSPlayer_WeaponServices

	class CModel
	{
	public:

	};

	class CModelState
	{
	public:



	};

	//foward
	class CSkeletonInstance;

	class CGameSceneNode
	{
	public:
		SCHEMA(sdk::vector, vec_abs_origin, "CGameSceneNode", "m_vecAbsOrigin");


	};

	class CSkeletonInstance : public CGameSceneNode
	{
	public:


	};


	class C_BaseEntity : public CEntityInstance
	{
	public:
		SCHEMA(std::int32_t, health, "C_BaseEntity", "m_iHealth");
		SCHEMA(std::int32_t, max_health, "C_BaseEntity", "m_iMaxHealth");
		SCHEMA(std::int32_t, team_num, "C_BaseEntity", "m_iTeamNum");
		SCHEMA(std::int32_t, life_state, "C_BaseEntity", "m_lifeState");
		SCHEMA(CGameSceneNode*, game_scene_node, "C_BaseEntity", "m_pGameSceneNode");
		SCHEMA(uint32_t, player_flags, "C_BaseEntity", "m_fFlags");
		SCHEMA(float, water_level, "C_BaseEntity", "m_flWaterLevel");
		SCHEMA(std::uint8_t, move_type, "C_BaseEntity", "m_MoveType");


		bool find_class(std::uint64_t name)
		{
			sdk::SchemaClassInfoData_t* info = nullptr;

			this->get_schema_class_info(&info);
			if (info == nullptr)
				return false;

			if (FNV1A_CMP_IM(info->szName, name))
				return true;

			return false;
		}


	};

	class C_BaseModelEntity : public C_BaseEntity
	{
	public:
		SCHEMA(sdk::vector, vec_view_offset, "C_BaseModelEntity", "m_vecViewOffset");

		const sdk::vector get_computed_abs_origin()
		{
			if (this->get_game_scene_node())
				return get_game_scene_node()->get_vec_abs_origin();

			return sdk::vector(0.f, 0.f, 0.f);  // Return a temporary object by value
		}

	};

	class CBaseAnimGraph : public C_BaseModelEntity {
	public:



	}; // class CBaseAnimGraph

	struct cs2_econ_item_view
	{



	};

	class C_BaseFlex : public CBaseAnimGraph {
	public:


	};

	class CAttributeManager
	{
	public:
		virtual ~CAttributeManager() = 0;
	};

	class C_AttributeContainer : public CAttributeManager
	{
	public:



	};

	class C_EconEntity : public C_BaseFlex {
	public:



	}; // class C_EconEntity

	//for now atm
	class C_BasePlayerWeapon : public C_EconEntity {
	public:


	};

	///foward
	class CCSWeaponBaseVData;

	class C_CSWeaponBase : public C_BasePlayerWeapon
	{
	public:


	};

	class CCSPlayer_MovementServices;
	class C_BasePlayerPawn : public C_BaseModelEntity
	{
	public:
		SCHEMA(GameTime_t, immune_to_gun_game_damage_time, "C_BasePlayerPawn", "m_pWeaponServices");
		SCHEMA(CCSPlayer_MovementServices*, movement_services, "C_BasePlayerPawn", "m_pMovementServices");

	};

	class CCSPlayer_ViewModelServices;

	class C_CSPlayerPawnBase : public C_BasePlayerPawn
	{
	public:
		SCHEMA(float, flash_duration, "C_CSPlayerPawnBase", "m_flFlashDuration");
		SCHEMA(GameTime_t, last_spawn_time, "C_CSPlayerPawnBase", "m_flLastSpawnTimeIndex");

	};


	class C_CSPlayerPawn : public C_CSPlayerPawnBase
	{
	public:

		bool is_enemy();
	};

	class CBasePlayerController : public C_BaseModelEntity
	{
	public:
		SCHEMA(bool, is_local_player_controller, "CBasePlayerController", "m_bIsLocalPlayerController");

	};

	class CCSPlayerController : public CBasePlayerController
	{
	public:
		SCHEMA(bool, pawn_alive, "CCSPlayerController", "m_bPawnIsAlive");
		SCHEMA(CHandle<C_CSPlayerPawn>, player_pawn, "CCSPlayerController", "m_hPlayerPawn");
		SCHEMA(uint32_t, tick_base, "CCSPlayerController", "m_nTickBase");
		SCHEMA(utils::CUtlString, sanitized_player_name, "CCSPlayerController", "m_sSanitizedPlayerName");
		SCHEMA_OFFSET(sdk::cuser_cmd*, current_command, "CBasePlayerController", "m_steamID", -0x8);

		bool is_enemy() { return !get_pawn() || get_pawn()->is_enemy(); }

		C_CSPlayerPawn* get_pawn()
		{
			return get_player_pawn().valid() ? get_player_pawn().get_as<C_CSPlayerPawn>() : nullptr;
		}


	};

	class CPlayer_MovementServices
	{
	public:


	};

	// entries 12 | size 0x218
	class CPlayer_MovementServices_Humanoid : public CPlayer_MovementServices
	{
	public:

	};

	class CCSPlayer_MovementServices : public CPlayer_MovementServices_Humanoid
	{
	public:
		void RunCommand(sdk::cuser_cmd* pCmd)
		{
			utils::CallVFunc<void, 23U>(this, pCmd);
		}

		void SetPredictionCommand(sdk::cuser_cmd* pCmd)
		{
			utils::CallVFunc<void, 34U>(this, pCmd);
		}

		void ResetPredictionCommand()
		{
			utils::CallVFunc<void, 35U>(this);
		}

	};

	struct cgame_entity_system
	{
		void* get_base_entity_by_index(int nIndex)//GetBaseEntity
		{///40 53 48 81 EC ? ? ? ? 48 8B 0D ? ? ? ? 48 8D 94 24 ? ? ? ?
			using fnGetBaseEntity = void* (__thiscall*)(void*, int);
			static auto GetBaseEntity = reinterpret_cast<fnGetBaseEntity>(FIND_PATTERN("client.dll", "81 FA ? ? ? ? 77 ? 8B C2 C1 F8 ? 83 F8 ? 77 ? 48 98 48 8B 4C C1 ? 48 85 C9 74 ? 8B C2 25 ? ? ? ? 48 6B C0 ? 48 03 C8 74 ? 8B 41 ? 25 ? ? ? ? 3B C2 75 ? 48 8B 01"));
			return GetBaseEntity(this, nIndex);
		}

		template <typename T = C_BaseEntity>
		T* get_entity_by_index(const uint32_t nIndex)
		{
			return reinterpret_cast<T*>(this->get_base_entity_by_index(nIndex));
		}

		CCSPlayerController* get_local_player_controller();
		CCSPlayerController* get_player_controller(const uint32_t index);
	};

	/// <summary>
	/// VData crap below here
	/// </summary>
	using CFiringModeFloat = std::float_t[2];
	using CSkillFloat = std::float_t[4];

	class CBasePlayerVData
	{
	public:


	};

	class CBasePlayerWeaponVData
	{
	public:


	};

	using CFiringModeFloat = std::float_t[2];

	class CCSWeaponBaseVData : public CBasePlayerWeaponVData
	{
	public:


	};

}
