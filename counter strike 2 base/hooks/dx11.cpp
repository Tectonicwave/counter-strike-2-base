#include "hook_manager.h"
#include "menu/menu.h"
#include "../renderer/draw_manager.h"

namespace hooks::dx11
{
    HRESULT present(IDXGISwapChain* chain, UINT sync, UINT flags)
    {
        // Initialize render target and related resources if not already initialized
        if (manager->D3DX11.render_target_view == nullptr)
        {
            // Retrieve the Direct3D device and context from the swap chain
            manager->swap_chain->pDXGISwapChain->GetDevice(__uuidof(ID3D11Device),
                reinterpret_cast<void**>(&manager->D3DX11.device));
            manager->D3DX11.device->GetImmediateContext(&manager->D3DX11.context);

            // Attempt to find the game window using its class and title
            HWND window = FindWindowA("SDL_app", "Counter-Strike 2");
            if (!window) {
                // Release resources and call the original present function if the window is not found
                manager->D3DX11.device->Release();
                manager->D3DX11.context->Release();
                return hook_manager.present->call(chain, sync, flags);
            }

            // Store the window handle in game variables
            manager->game_vars.window = window;

            // Hook the window procedure to intercept input events
            manager->game_vars.old_window_proc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(hooks::input_system::wnd_proc)));

            // Create the render target for drawing
            draw->create_render_target();

            // Set up the draw manager with the window, device, and context
            draw->setup(manager->game_vars.window, manager->D3DX11.device, manager->D3DX11.context);

            // Update display size if it has changed
            if (draw->display_size.x != ImGui::GetIO().DisplaySize.x ||
                draw->display_size.y != ImGui::GetIO().DisplaySize.y) {
                draw->display_size = ImGui::GetIO().DisplaySize;
            }
        }

        // Set the render target for rendering
        if (manager->D3DX11.render_target_view != nullptr)
            manager->D3DX11.context->OMSetRenderTargets(1, &manager->D3DX11.render_target_view, nullptr);

        // Perform rendering if the draw manager has been initialized
        if (draw->Initialized)
        {
            // Begin a new ImGui frame
            draw->new_frame();
            {
                // Draw the GUI menu or other elements
                menu->menu();
            }
            // Render the frame
            draw->render();
        }

        // Call the original present function to continue the rendering pipeline
        return hook_manager.present->call(chain, sync, flags);
    }
}