#pragma once

// Standard library includes
#include <D3D11.h>
#include <stdexcept>

// Project-specific includes
#include "minhook/minhook.h"
#include "../definitions.h"
#include "../manager.h"
#include "../sdk/interfaces/usercmd.h"

class IViewRender
{
public:
    sdk::vector vecOrigin; // 0x0000
    sdk::qangle vecAngles; // 0x000C
    float flFov; // 0x0018
    char pad_0x001C[0x14]; // 0x001C
    sdk::ViewMatrix_t matUNK1; // 0x0030
    char pad_0x0070[0x30]; // 0x0070
    sdk::ViewMatrix_t matUNK2; // 0x00A0
    char pad_0x00E0[0xC8]; // 0x00E0
    sdk::ViewMatrix_t matUNK3; // 0x01A8
    char pad_0x01E8[0x28]; // 0x01E8
};

class CRenderGameSystem;

namespace hooks
{

    namespace dx11
    {
        HRESULT present(IDXGISwapChain* chain, UINT sync, UINT flags);
    }

    namespace input_system
    {
        LRESULT wnd_proc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam);
        bool mouse_input_enabled(void* rcx);
        void* is_relative_mouse_mode(void* pThisptr, bool bActive);
    }

    namespace client
    {
        sdk::ViewMatrix_t* get_matrix_for_view(CRenderGameSystem* pRenderGameSystem, IViewRender* pViewRender, sdk::ViewMatrix_t* pOutWorldToView, sdk::ViewMatrix_t* pOutViewToProjection, sdk::ViewMatrix_t* pOutWorldToProjection, sdk::ViewMatrix_t* pOutWorldToPixels);
        bool __fastcall create_move(sdk::ccsgo_input* input, int slot, sdk::cuser_cmd* pUserCmd);
        void frame_stage_notify(void* rcx, sdk::client_frame_stage stage);
        void on_render_start(sdk::cview_render* view_render);
        void override_view(void* rcx, sdk::cview_setup* view_setup);
    }


	/// <summary>
	/// i have this crap here 
	/// </summary>
	class hook_interface
	{
	public:
		hook_interface() = default;
		hook_interface(hook_interface const& other) = delete;
		hook_interface& operator=(hook_interface const& other) = delete;
		hook_interface(hook_interface&& other) = delete;
		hook_interface& operator=(hook_interface&& other) = delete;

		virtual ~hook_interface() = default;
		virtual void attach() = 0;
		virtual void detach() = 0;
		virtual uintptr_t get_endpoint() const = 0;
		virtual uintptr_t get_target() const = 0;
	};

    template <typename T>
    class hook final : public hook_interface {
    public:
        // Constructor
        explicit hook(T* target, T* endpoint) : hook_interface(), target(CAST_TO_PTR(T, target)), endpoint(CAST_TO_PTR(T, endpoint)), is_attached(false)
        {
            void* org = nullptr;
            MH_CreateHook(CAST_TO_PTR(void, target), CAST_TO_PTR(void, endpoint), &org);
            trampoline = CAST_TO_PTR(T, org);
        }

        // Destructor
        ~hook() {
            if (is_attached) {
                try {
                    detach();
                }
                catch (const std::runtime_error&) {
                    // Silently handle errors during destruction
                }
            }

            MH_RemoveHook(CAST_TO_PTR(void, target));
        }

        // Attach the hook
        void attach() override {
            MH_EnableHook(CAST_TO_PTR(void, target));
            is_attached = true;
        }

        // Detach the hook
        void detach() override {
            MH_DisableHook(CAST_TO_PTR(void, target));
            is_attached = false;
        }

        // Get endpoint address
        uintptr_t get_endpoint() const override {
            return reinterpret_cast<uintptr_t>(endpoint);
        }

        // Get target address
        uintptr_t get_target() const override {
            return reinterpret_cast<uintptr_t>(target);
        }

        // Get original function pointer
        T* get_original() const {
            return trampoline;
        }

        // Call the original function
        template <typename... A>
        auto call(A... args) const {
            return get_original()(args...);
        }

    private:
        // Change the endpoint and reattach the hook if necessary
        void set_endpoint(T* new_endpoint) {
            const auto was_attached = is_attached;

            if (is_attached) {
                detach();
            }

            endpoint = new_endpoint;

            if (endpoint && was_attached) {
                attach();
            }
        }

        T* target;
        T* endpoint;
        T* trampoline;
        bool is_attached;
    };

} // namespace hooks

