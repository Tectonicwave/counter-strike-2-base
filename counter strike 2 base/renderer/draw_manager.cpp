#include "draw_manager.h"

#define IMGUI_DEFINE_MATH_OPERATORS
//#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../imgui/imgui_freetype.h"
#include "menu/menu.h"

#include "../resources/font.h"
#include "../resources/image.h"
#include <cstddef>
#include "../manager.h"

#include "../hooks/hook_manager.h"

#include "key_manager.h"

namespace image
{
	ID3D11ShaderResourceView* background_preview = nullptr;
	ID3D11ShaderResourceView* preview_model = nullptr;
	ID3D11ShaderResourceView* logo = nullptr;
}

// thread-safe draw data mutex
static SRWLOCK drawLock = {};

static void* __cdecl ImGuiAllocWrapper(const std::size_t nSize, [[maybe_unused]] void* pUserData = nullptr)
{
	const HANDLE hHeap = ::GetProcessHeap();
	return ::HeapAlloc(hHeap, 0UL, nSize);
}

static void __cdecl ImGuiFreeWrapper(void* pMemory, [[maybe_unused]] void* pUserData = nullptr) noexcept
{
	if (pMemory != nullptr)
	{
		const HANDLE hHeap = ::GetProcessHeap();
		::HeapFree(hHeap, 0UL, pMemory);
	}
}

bool draw_manager_t::Setup(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	// check is it were already initialized
	if (Initialized)
		return true;

	ImGui::SetAllocatorFunctions(ImGuiAllocWrapper, ImGuiFreeWrapper);

	ImGui::CreateContext();

	// setup platform and renderer bindings
	if (!ImGui_ImplWin32_Init(hWnd))
		return false;

	if (!ImGui_ImplDX11_Init(pDevice, pContext))
		return false;

	// create draw data containers
	draw_list_active = IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());
	draw_list_safe = IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());
	draw_list_render = IM_NEW(ImDrawList)(ImGui::GetDrawListSharedData());

	// setup styles
#pragma region draw_setup_style
	ImGuiStyle& style = ImGui::GetStyle();
	style.Alpha = 1.0f;
	style.WindowPadding = ImVec2(8, 8);
	style.WindowRounding = 4.0f;
	style.WindowBorderSize = 1.0f;
	style.WindowMinSize = ImVec2(32, 32);
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
	style.ChildRounding = 4.0f;
	style.ChildBorderSize = 1.0f;
	style.PopupRounding = 4.0f;
	style.PopupBorderSize = 1.0f;
	style.FramePadding = ImVec2(4, 2);
	style.FrameRounding = 4.0f;
	style.FrameBorderSize = 1.0f;
	style.ItemSpacing = ImVec2(8, 4);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.IndentSpacing = 6.0f;
	style.ColumnsMinSpacing = 6.0f;
	style.ScrollbarSize = 6.0f;
	style.ScrollbarRounding = 9.0f;
	style.GrabMinSize = 0.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;
	style.TabBorderSize = 1.0f;
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
	style.SelectableTextAlign = ImVec2(0.0f, 0.5f);
	style.WindowShadowSize = 0.f;
	style.AntiAliasedLines = true;
	style.AntiAliasedFill = true;
	style.AntiAliasedLinesUseTex = true;
	style.ColorButtonPosition = ImGuiDir_Right;
#pragma endregion

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	ImFontConfig cfg;
	cfg.FontBuilderFlags = ImGuiFreeTypeBuilderFlags_ForceAutoHint | ImGuiFreeTypeBuilderFlags_LightHinting | ImGuiFreeTypeBuilderFlags_LoadColor | ImGuiFreeTypeBuilderFlags_Bitmap;

	font::inter_element = io.Fonts->AddFontFromMemoryTTF(inter_semibold, sizeof(inter_semibold), 12.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::inter_child = io.Fonts->AddFontFromMemoryTTF(inter_semibold, sizeof(inter_semibold), 14.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	font::icomoon = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), 19.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::icomoon_tabs = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), 22.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());
	font::icomoon_widget = io.Fonts->AddFontFromMemoryTTF(icomoon, sizeof(icomoon), 16.f, &cfg, io.Fonts->GetGlyphRangesCyrillic());

	//not for directx11
	//D3DX11_IMAGE_LOAD_INFO info; ID3DX11ThreadPump* pump{ nullptr };
	//if (image::background_preview == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, background, sizeof(background), &info, pump, &image::background_preview, 0);
	//if (image::preview_model == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, preview_model, sizeof(preview_model), &info, pump, &image::preview_model, 0);
	//if (image::logo == nullptr) D3DX11CreateShaderResourceViewFromMemory(g_pd3dDevice, logo, sizeof(logo), &info, pump, &image::logo, 0);

	Initialized = io.Fonts->Build();
	return Initialized;
}

void draw_manager_t::NewFrame()
{
	// Start a new frame for ImGui rendering
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void draw_manager_t::Render()
{
	// Finalize the ImGui rendering process and draw the prepared data
	ImGui::Render();
	render_draw_data(ImGui::GetDrawData());
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

// Resets the draw data for a new frame
void draw_manager_t::ResetDrawData()
{
	// Reset the active draw list for the new frame
	draw_list_active->_ResetForNewFrame();

	// Push the current font texture ID into the draw list
	draw_list_active->PushTextureID(ImGui::GetIO().Fonts->TexID);

	// Set the clipping rectangle to cover the full screen
	draw_list_active->PushClipRectFullScreen();
}

// Swaps the draw data between the active and safe draw lists
void draw_manager_t::SwapDrawData()
{
	// Acquire an exclusive lock for thread safety
	::AcquireSRWLockExclusive(&drawLock);

	// Ensure the vertex and index buffers are correctly written
	IM_ASSERT(draw_list_active->VtxBuffer.Size == 0 ||
		draw_list_active->_VtxWritePtr == draw_list_active->VtxBuffer.Data + draw_list_active->VtxBuffer.Size);
	IM_ASSERT(draw_list_active->IdxBuffer.Size == 0 ||
		draw_list_active->_IdxWritePtr == draw_list_active->IdxBuffer.Data + draw_list_active->IdxBuffer.Size);

	// Verify the vertex index is consistent with the buffer size if offsets are not allowed
	if (!(draw_list_active->Flags & ImDrawListFlags_AllowVtxOffset))
		IM_ASSERT(static_cast<int>(draw_list_active->_VtxCurrentIdx) == draw_list_active->VtxBuffer.Size);

	// Copy the active draw list to the safe draw list for rendering
	*draw_list_safe = *draw_list_active;

	// Release the exclusive lock
	::ReleaseSRWLockExclusive(&drawLock);
}

void draw_manager_t::render_draw_data(ImDrawData* pDrawData)
{
	if (::TryAcquireSRWLockExclusive(&drawLock))
	{
		*draw_list_render = *draw_list_safe;
		::ReleaseSRWLockExclusive(&drawLock);
	}

	if (draw_list_render->CmdBuffer.empty())
		return;

	// remove trailing command if unused
	// @note: equivalent to  pDrawList->_PopUnusedDrawCmd()
	if (const ImDrawCmd& lastCommand = draw_list_render->CmdBuffer.back(); lastCommand.ElemCount == 0 && lastCommand.UserCallback == nullptr)
	{
		draw_list_render->CmdBuffer.pop_back();
		if (draw_list_render->CmdBuffer.empty())
			return;
	}

	ImGuiContext* pContext = ImGui::GetCurrentContext();
	ImGuiViewportP* pViewport = pContext->Viewports[0];
	ImVector<ImDrawList*>* vecDrawLists = pViewport->DrawDataBuilder.Layers[0];
	vecDrawLists->push_front(draw_list_render); // this one being most background

	pDrawData->CmdLists.push_front(draw_list_render);
	pDrawData->CmdListsCount = vecDrawLists->Size;
	pDrawData->TotalVtxCount += draw_list_render->VtxBuffer.Size;
	pDrawData->TotalIdxCount += draw_list_render->IdxBuffer.Size;
}

void draw_manager_t::Destroy()
{
	// check is it already destroyed or wasn't initialized at all
	if (!Initialized)
		return;

	// free draw data containers
	IM_DELETE(draw_list_active);
	IM_DELETE(draw_list_safe);
	IM_DELETE(draw_list_render);

	// shutdown imgui directx9 renderer binding
	ImGui_ImplDX11_Shutdown();

	// shutdown imgui win32 platform binding
	ImGui_ImplWin32_Shutdown();

	// destroy imgui context
	ImGui::DestroyContext();

	Initialized = false;
}

void draw_manager_t::create_render_target()
{
	// Get swap chain description
	DXGI_SWAP_CHAIN_DESC sd;
	manager->swap_chain->pDXGISwapChain->GetDesc(&sd);

	ID3D11Texture2D* pBackBuffer = nullptr;

	// Try to get the back buffer from the swap chain
	if (SUCCEEDED(manager->swap_chain->pDXGISwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer))))
	{
		if (pBackBuffer)
		{
			// Set up render target description with preferred format
			D3D11_RENDER_TARGET_VIEW_DESC desc{};
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // Use preferred format
			desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;  // Set view dimension

			// Create the render target view
			HRESULT hr = manager->D3DX11.device->CreateRenderTargetView(pBackBuffer, &desc, &manager->D3DX11.render_target_view);
			
			if (FAILED(hr))
			{
				assert(false);
			}

			// Release back buffer after usage
			pBackBuffer->Release();
		}
	}
}

void draw_manager_t::destroy_render_target()
{
	if (manager->D3DX11.render_target_view != nullptr)
	{
		manager->D3DX11.render_target_view->Release();
		manager->D3DX11.render_target_view = nullptr;
	}
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool draw_manager_t::on_window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Check if drawing is initialized
	// If not initialized, return false to prevent further processing
	if (!Initialized)
		return false;

	// Ensure the key manager updates the state based on the received message
	static KeyManager keyManager;
	keyManager.UpdateKeyState(uMsg, wParam);

	// If the DELETE key is released, toggle the menu state
	if (keyManager.IsKeyReleased(VK_DELETE))
	{
		// Toggle the menu state between opened and closed
		menu->window_opened = !menu->window_opened;

		// Toggle relative mouse mode based on the current state of the menu
		// If the menu is opened, set relative mouse mode to false
		// Otherwise, set it to the value of `main_active`
		hook_manager.is_relative_mouse_mode->call(manager->input_system, menu->window_opened ? false : menu->main_active);

		// Return true to indicate the message was processed successfully
		return true;
	}

	// If the DELETE key wasn't pressed, forward the message to ImGui's window message handler
	// This will also block the game's input if the menu is open
	return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam) || menu->window_opened;
}