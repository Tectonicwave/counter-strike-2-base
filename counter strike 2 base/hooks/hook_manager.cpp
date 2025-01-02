// Project-specific includes
#include "hook_manager.h"
#include "../utils/memory.h"
#include "hook_manager.h"
#include "../sdk/interfaces/events.h"

hook_manager_t hook_manager;
using namespace utils;// Brings all symbols from the utils namespace into scope.

void hook_manager_t::init()
{
	if (MH_Initialize() != MH_OK)
		return;

	create_hook(mouse_input_enabled, reinterpret_cast<uintptr_t>(GetVFunc(manager->input, VTable::input::MOUSEINPUTENABLED)), &hooks::input_system::mouse_input_enabled);
	create_hook(is_relative_mouse_mode, reinterpret_cast<uintptr_t>(GetVFunc(manager->input_system, VTable::input_system::ISRELATIVEMOUSEMODE)), &hooks::input_system::is_relative_mouse_mode);
	create_hook(present, reinterpret_cast<uintptr_t>(GetVFunc(manager->swap_chain->pDXGISwapChain, VTable::D3D11::PRESENT)), &hooks::dx11::present);
	create_hook(frame_stage_notify, reinterpret_cast<uintptr_t>(GetVFunc(manager->Source_client, VTable::client::FRAMESTAGENOTIFY)), &hooks::client::frame_stage_notify);
	create_hook(create_move, reinterpret_cast<uintptr_t>(GetVFunc(manager->input, 21u)), &hooks::client::create_move);

	//signature hooks
	create_hook(get_matrix_for_view, reinterpret_cast<uintptr_t>(find_pattern(FNV1A("client.dll"), "40 53 48 81 EC ? ? ? ? 49 8B C1")), &hooks::client::get_matrix_for_view);
	create_hook(on_render_start, reinterpret_cast<uintptr_t>(find_pattern(FNV1A("client.dll"), "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 4C 8B F1 48 8D 94 24")), &hooks::client::on_render_start);

	manager->game_event_manager->AddListeners(&sdk::evnt, "player_hurt", false);
}

void hook_manager_t::attach() const
{
	for (auto &hook : hooks)
		hook->attach();
}

void hook_manager_t::detach() const
{
	for (auto &hook : hooks)
		hook->detach();
}
