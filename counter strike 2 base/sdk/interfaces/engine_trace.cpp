// used: manager trace manager
#include "engine_trace.h"
#include "../../manager.h"

namespace sdk
{
	SurfaceData_t* game_trace_t::get_surface_data()
	{
		using fn_get_surface_data = std::uint64_t(__fastcall*)(void*);
		static fn_get_surface_data o_get_surface_data = reinterpret_cast<fn_get_surface_data>(
			utils::get_absolute_address(
			FIND_PATTERN("client.dll", "E8 ? ? ? ? 48 85 C0 74 ? 44 38 60"),
				0x1, 0x0
			)
			);

		if (!o_get_surface_data)
			return nullptr;

		return reinterpret_cast<SurfaceData_t*>(o_get_surface_data(surface));
	}

	int game_trace_t::get_hitbox_id() const
	{
		return hitbox_data ? hitbox_data->hitbox_id : 0;
	}

	int game_trace_t::get_hitgroup() const
	{
		return hitbox_data ? hitbox_data->hit_group : 0;
	}

	bool game_trace_t::is_visible() const
	{
		return fraction > 0.97f;
	}

	bool game_trace_t::did_hit_world() const
	{
		C_BaseEntity* world_entity = manager->resource_service->game_entity_system->get_entity_by_index(0);
		if (!world_entity)
			return false;

		return reinterpret_cast<C_BaseEntity*>(hit_entity) == world_entity;
	}

	trace_filter_t::trace_filter_t(std::uint64_t trace_mask, sdk::C_CSPlayerPawn* skip1, sdk::C_CSPlayerPawn* skip2, int layer)
		: trace_mask(trace_mask), v1{ 0, 0 }, v2(7), v3(layer), v4(0x49), v5(0)
	{
		if (skip1)
		{
			arr_skip_handles[0] = skip1->get_ref_ehandle().get_entry_index();
			arr_skip_handles[2] = skip1->get_owner_entity().get_entry_index();
			arr_collisions[0] = skip1->get_collision_mask();
		}
		else
		{
			arr_skip_handles[0] = 0;
			arr_skip_handles[2] = 0;
			arr_collisions[0] = 0;
		}

		if (skip2)
		{
			arr_skip_handles[1] = skip2->get_ref_ehandle().get_entry_index();
			arr_skip_handles[3] = skip2->get_owner_entity().get_entry_index();
			arr_collisions[1] = skip2->get_collision_mask();
		}
		else
		{
			arr_skip_handles[1] = 0;
			arr_skip_handles[3] = 0;
			arr_collisions[1] = 0;
		}
	}
}
