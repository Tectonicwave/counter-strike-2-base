#pragma once

// Standard library includes
#include <list>

// Project-specific includes
#include "hooks.h"
#include <memory>

struct hook_manager_t
{
	hook_manager_t() = default;
	void init();
	void attach() const;
	void detach() const;

	std::shared_ptr<hooks::hook<decltype(hooks::client::get_matrix_for_view)>> get_matrix_for_view;
	std::shared_ptr<hooks::hook<decltype(hooks::dx11::present)>> present;
	std::shared_ptr<hooks::hook<decltype(hooks::input_system::mouse_input_enabled)>> mouse_input_enabled;
	std::shared_ptr<hooks::hook<decltype(hooks::input_system::is_relative_mouse_mode)>> is_relative_mouse_mode;
	std::shared_ptr<hooks::hook<decltype(hooks::client::frame_stage_notify)>> frame_stage_notify;
	std::shared_ptr<hooks::hook<decltype(hooks::client::on_render_start)>> on_render_start;
	std::shared_ptr<hooks::hook<decltype(hooks::client::create_move)>> create_move;
private:
	std::list<std::shared_ptr<hooks::hook_interface>> hooks;

	template <typename T>
	void create_hook(std::shared_ptr<hooks::hook<T>>& entry, uintptr_t original, T* fn) {
		auto ptr = std::make_shared<hooks::hook<T>>(CAST_TO_PTR(T, original), fn);
		entry = ptr;
		hooks.push_back(ptr);
	}
};

extern hook_manager_t hook_manager;
