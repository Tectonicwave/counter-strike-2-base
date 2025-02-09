#include "hook_manager.h"
#include "menu/menu.h"
#include "../renderer/draw_manager.h"

namespace hooks::input_system
{
	LRESULT wnd_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		// prevent process when e.g. binding something in-menu
		if (draw->on_window_proc(wnd, msg, wparam, lparam))
			return 1L;

		return ::CallWindowProcW(manager->game_vars.old_window_proc, wnd, msg, wparam, lparam);
	}

	bool mouse_input_enabled(void* rcx)
	{

		if (menu->window_opened)
			return false;

		return hook_manager.mouse_input_enabled->call(rcx);
	}

	void* is_relative_mouse_mode(void* pThisptr, bool bActive)
	{
		menu->main_active = bActive;

		if (menu->window_opened)
			return hook_manager.is_relative_mouse_mode->call(pThisptr, false);

		return hook_manager.is_relative_mouse_mode->call(pThisptr, bActive);
	}
}