#include "manager.h"
#include "hooks/hook_manager.h"
#include "sdk/schema/schema_var.h"

std::unique_ptr<manager_t> manager;

// game modules Constructor
manager_t::manager_t(uintptr_t base, uint32_t reserved) : base(base)
{
	tier0 = library(XOR("tier0.dll"));
	sdl3 = library(XOR("sdl3.dll"));
	gameoverlayrenderer64 = library(XOR("gameoverlayrenderer64.dll"));
	client = library(XOR("client.dll"));
	engine2 = library(XOR("engine2.dll"));
	inputsystem = library(XOR("inputsystem.dll"));
	localize = library(XOR("localize.dll"));
	panorama = library(XOR("panorama.dll"));
	scenesystem = library(XOR("scenesystem.dll"));
	rendersystemdx11 = library(XOR("rendersystemdx11.dll"));
	schema = library(XOR("schemasystem.dll"));
}

void manager_t::init()
{
	resource_service = find_interface_list<sdk::cgame_resource_service>(engine2.get_interface(), XOR("GameResourceServiceClientV001"));
	input_system = find_interface_list<sdk::IInputSystem>(inputsystem.get_interface(), XOR("InputSystemVersion001"));
	Source_client = find_interface_list<ISource2Client>(client.get_interface(), XOR("Source2Client00"));
	schema_system = find_interface_list<sdk::ISchemaSystem>(schema.get_interface(), XOR("SchemaSystem_00"));
	engine = find_interface_list<sdk::cengine_client>(engine2.get_interface(), XOR("Source2EngineToClient00"));
	global_vars = *reinterpret_cast<sdk::global_vars_t**>(solve_address(FIND_PATTERN("client.dll", "48 89 15 ?? ?? ?? ?? 48 89 42"), 0x3, 0x7));
	network_client_service = sdk::find_interface_list<sdk::network_client_service_t>(engine2.get_interface(), XOR("NetworkClientService_001"));
	prediction = sdk::find_interface_list<sdk::cprediction>(client.get_interface(), XOR("Source2ClientPrediction001"));

	swap_chain = **reinterpret_cast<sdk::ISwapChainDx11***>(solve_address(FIND_PATTERN("rendersystemdx11.dll", "66 0F 7F 0D ? ? ? ? 48 8B F7 66 0F 7F 05"), 0x4, 0x8));
	game_event_manager = *reinterpret_cast<sdk::game_event_manager_t**>(get_absolute_address(FIND_PATTERN("client.dll", "48 8B 0D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B 0D ?? ?? ?? ?? 48 85 C9 74 2D"), 0x3));
	input = *reinterpret_cast<sdk::ccsgo_input**>(solve_address(FIND_PATTERN("client.dll", "48 8B 0D ? ? ? ? 4C 8B C6 8B 10 E8"), 0x3, 0x7));

	hook_manager.init();
	hook_manager.attach();
}
