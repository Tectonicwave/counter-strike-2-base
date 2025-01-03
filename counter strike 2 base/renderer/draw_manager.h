#pragma once
// used: [d3d]
#include <d3d11.h>       // Core Direct3D 11 functionality
#include <imgui_internal.h>
#include <string>
#include <memory>

namespace image
{
	inline ID3D11ShaderResourceView* background_preview = nullptr;
	inline ID3D11ShaderResourceView* preview_model = nullptr;
	inline ID3D11ShaderResourceView* logo = nullptr;

	inline ID3D11ShaderResourceView* bg = nullptr;
	inline ID3D11ShaderResourceView* logo_general = nullptr;

	inline ID3D11ShaderResourceView* arrow = nullptr;
	inline ID3D11ShaderResourceView* bell_notify = nullptr;
	inline ID3D11ShaderResourceView* roll = nullptr;
}

class draw_manager_t
{
public:
	bool setup(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void new_frame();
	void render();
	void reset_draw_data();
	void swap_draw_data();
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

inline std::unique_ptr<draw_manager_t> draw = std::make_unique<draw_manager_t>();
