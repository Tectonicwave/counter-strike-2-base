#include "hook_manager.h"
#include "menu/menu.h"
#include "../renderer/draw_manager.h"
#include "../sdk/interfaces/usercmd.h"
#include "../features/movement.h"
#include "../features/visuals.h"
#include "../features/prediction.h"
#include "../features/player_log.h"

namespace hooks::client
{
	sdk::ViewMatrix_t* get_matrix_for_view(CRenderGameSystem* pRenderGameSystem, IViewRender* pViewRender, sdk::ViewMatrix_t* pOutWorldToView, sdk::ViewMatrix_t* pOutViewToProjection, sdk::ViewMatrix_t* pOutWorldToProjection, sdk::ViewMatrix_t* pOutWorldToPixels)
	{
		sdk::ViewMatrix_t* matResult = hook_manager.get_matrix_for_view->call(pRenderGameSystem, pViewRender, pOutWorldToView, pOutViewToProjection, pOutWorldToProjection, pOutWorldToPixels);

		// get view matrix, could just get sig for this from the actual function
		manager->game_vars.view_matrix = *pOutWorldToProjection;

		return matResult;
	}

	bool __fastcall create_move(sdk::ccsgo_input* input, int slot, sdk::cuser_cmd* pUserCmd)
	{
		// Check if the game is connected and in-game
		if (!manager->engine->is_connected() || !manager->engine->in_game())
			return hook_manager.create_move->call(input, slot, pUserCmd);

		// Check if the user command has a valid base command
		if (!pUserCmd->csgoUserCmd.pBaseCmd)
			return hook_manager.create_move->call(input, slot, pUserCmd);

		auto base_cmd = pUserCmd->csgoUserCmd.pBaseCmd;

		// Check if the base command has valid view angles
		if (!base_cmd->pViewAngles)
			return hook_manager.create_move->call(input, slot, pUserCmd);

		// Retrieve the local player controller
		manager->game_vars.Local_controller = manager->resource_service->game_entity_system->get_local_player_controller();
		if (manager->game_vars.Local_controller == nullptr)
			return hook_manager.create_move->call(input, slot, pUserCmd);

		// Retrieve the local player's pawn (character/entity)
		manager->game_vars.Local_Pawn = manager->game_vars.Local_controller->get_pawn();
		if (manager->game_vars.Local_Pawn == nullptr)
			return hook_manager.create_move->call(input, slot, pUserCmd);

		prediction->Begin(input, pUserCmd);
		{
			movement->bhop(pUserCmd);
		}
		prediction->end(input, pUserCmd);

		// Fix movement after modifications
		movement->movement_fix(pUserCmd);

		// Call the original create_move function
		return hook_manager.create_move->call(input, slot, pUserCmd);
	}

	void frame_stage_notify(void* rcx, sdk::client_frame_stage stage)
	{
		if (stage == sdk::client_frame_stage::FRAME_NET_UPDATE_POSTDATAUPDATE_END)
		{
			//bare bones no flash
			if (local_pawn)
				local_pawn->get_flash_duration() = 0.f;

			// run our esp data store, store out data we need for the esp
			visuals->run();
		}

		player_records->run_log(stage);

		hook_manager.frame_stage_notify->call(rcx, stage);
	}

	// Called at the start of the render process
	void on_render_start(sdk::cview_render* view_render)
	{
		// Call the original render start function (if hooked)
		hook_manager.on_render_start->call(view_render);

		// Proceed only if the draw manager is initialized
		if (draw->Initialized)
		{
			// game and imgui are based on immediate render mode principe
			// this means that we should always reset draw data from previous frame and re-store it again
			// Reset draw data for the new frame
			draw->reset_draw_data();

			// Render visuals, we passing the render setup information, for off arrows and other stuff 
			visuals->render(&view_render->setup);

			// Swap the updated draw data to the safe draw list for use in rendering
			draw->swap_draw_data();
		}
	}
}