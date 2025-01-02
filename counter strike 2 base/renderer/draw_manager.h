#pragma once
// used: [d3d]
#include <d3d11.h>       // Core Direct3D 11 functionality
#include <imgui_internal.h>

namespace font
{
	inline ImFont* icomoon = nullptr;
	inline ImFont* icomoon_tabs = nullptr;
	inline ImFont* icomoon_widget = nullptr;

	inline ImFont* inter_child = nullptr;
	inline ImFont* inter_element = nullptr;
}

class draw_manager_t
{
public:
	bool Setup(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void NewFrame();
	void Render();
	void ResetDrawData();
	void SwapDrawData();
	void render_draw_data(ImDrawData* pDrawData);

	void Destroy();

	void create_render_target();

	void destroy_render_target();

	bool on_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// rendering engine initialization state
	bool Initialized = false;
	// active draw data container used to store
	ImDrawList* draw_list_active = nullptr;
	// safe draw data container
	ImDrawList* draw_list_safe = nullptr;
	// actual draw data container used to render
	ImDrawList* draw_list_render = nullptr;

	ImVec2 display_size{};
};

inline draw_manager_t* draw = new draw_manager_t;
