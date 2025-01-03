#pragma once
#include <imgui_impl_dx11.h>
#include <d3d11.h>
#include <dxgi.h>

#include "pch.h"
#include "utils/managment.h"
#include "utils/memory.h"
#include "sdk/game/entity_classes.h"
#include "sdk/interfaces/swap_chain_dx11.h"
#include "sdk/interfaces/input_system.h"
#include "sdk/interfaces/csgo_input.h"
#include "sdk/game/game_enums.h"
#include "sdk/interfaces/view_setup.h"
#include "sdk/schema/schema_system.h"
#include "sdk/interfaces/engine.h"
#include "sdk/maths/vector.h"
#include "sdk/maths/qangle.h"
#include "sdk/interfaces/globalvars.h"
#include "../sdk/maths/matrix.h"
#include "sdk/interfaces/network_client.h"
#include "sdk/interfaces/prediction.h"
#include "sdk/interfaces/game_event_manager.h"
#include "sdk/interfaces/cvar.h"
#include "sdk/interfaces/engine_trace.h"

using namespace utils;// Brings all symbols from the utils namespace into scope.

class ISource2Client;

class CUtlBuffer;
class Ray_t;

struct manager_t
{
	manager_t(uintptr_t base, uint32_t tok);

	void init();

	using utl_buffer_init_t = void(__fastcall*)(CUtlBuffer*, int, int, int);
	using utl_buffer_put_string_t = void(__fastcall*)(CUtlBuffer*, const char*);
	using utl_buffer_ensure_capacity_t = void(__fastcall*)(CUtlBuffer*, int);

	struct function_table_t
	{
		utl_buffer_init_t utl_buffer_init;
		utl_buffer_put_string_t utl_buffer_put_string;
		utl_buffer_ensure_capacity_t utl_buffer_ensure_capacity;

	} function;

	//dont really need this could just do another way, but have some advantages 
	library tier0, sdl3, gameoverlayrenderer64, client, engine2, inputsystem, localize, panorama, scenesystem, rendersystemdx11, schema;

	struct game_vars_t
	{
		sdk::CCSPlayerController* Local_controller = nullptr;
		sdk::C_CSPlayerPawn* Local_Pawn = nullptr;

		// current window
		WNDPROC old_window_proc = nullptr;

		// saved window messages handler
		HWND window = nullptr;

		sdk::ViewMatrix_t view_matrix = sdk::ViewMatrix_t();

	} game_vars;

	struct D3DX11_store
	{
		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* context = nullptr;
		UINT g_ResizeWidth = 0, g_ResizeHeight = 0;
		ID3D11RenderTargetView* render_target_view = nullptr;

	} D3DX11;

	sdk::cgame_resource_service* resource_service = nullptr;
	sdk::ISwapChainDx11* swap_chain = nullptr;
	sdk::IInputSystem* input_system = nullptr;
	sdk::game_event_manager_t* game_event_manager = nullptr;
	sdk::ccsgo_input* input = nullptr;
	ISource2Client* Source_client = nullptr;
	sdk::ISchemaSystem* schema_system = nullptr;
	sdk::cengine_client* engine = nullptr;
	sdk::global_vars_t* global_vars = nullptr;
	sdk::network_client_service_t* network_client_service = nullptr;
	sdk::cprediction* prediction = nullptr;
	sdk::ccvar* cvar = nullptr;
	sdk::engine_trace_t* engine_trace = nullptr;

private:
	uintptr_t base;
};

inline std::unique_ptr<manager_t> manager = std::make_unique<manager_t>(/* base */ 0, /* tok */ 0);