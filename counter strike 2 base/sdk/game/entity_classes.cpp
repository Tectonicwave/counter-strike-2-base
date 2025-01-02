// Project-specific includes
#include "entity_classes.h"
#include "../../manager.h"

namespace sdk {

    // Definition of `get`
    template <typename T>
    T* CHandle<T>::get() {
        return !valid() ? nullptr : manager->resource_service->game_entity_system->get_entity_by_index<T>(get_entry_index());
    }

    // Definition of `get_as`
    template <typename T>
    template <typename C>
    C* CHandle<T>::get_as() {
        return !valid() ? nullptr : manager->resource_service->game_entity_system->get_entity_by_index<C>(get_entry_index());
    }

    // Explicit template instantiations
    template class CHandle<sdk::C_BasePlayerPawn>;
    template class CHandle<sdk::C_CSPlayerPawn>;
    template class CHandle<sdk::C_BasePlayerWeapon>;
    template class CHandle<sdk::C_BaseEntity>;
    template class CHandle<sdk::CEntityInstance>; // Add support for CEntityInstance

    // Explicit `get_as()` specializations
    template sdk::C_CSPlayerPawn* CHandle<sdk::C_BasePlayerPawn>::get_as();
    template sdk::C_CSPlayerPawn* CHandle<sdk::C_BasePlayerPawn>::get_as();

    // Add explicit specialization for get_as for C_CSPlayerPawn
    template sdk::C_CSPlayerPawn* CHandle<sdk::C_CSPlayerPawn>::get_as();

    template sdk::CEntityInstance* CHandle<sdk::C_BaseEntity>::get_as();
    template sdk::CEntityInstance* CHandle<sdk::CEntityInstance>::get_as();

    CCSPlayerController* cgame_entity_system::get_local_player_controller()
    {
        const int index = manager->engine->get_local_player();
        return get_entity_by_index<CCSPlayerController>(index);
    }

    CCSPlayerController* cgame_entity_system::get_player_controller(const uint32_t index)
    {
        return get_entity_by_index<CCSPlayerController>(index);
    }

    bool C_CSPlayerPawn::is_enemy()
    {
        const auto local_player_controller = reinterpret_cast<CCSPlayerController*>(manager->game_vars.Local_controller);
        return !local_player_controller || !local_player_controller->get_player_pawn().valid() ? true : get_team_num() != local_player_controller->get_team_num();
    }
}