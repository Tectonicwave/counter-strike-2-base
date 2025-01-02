// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "manager.h"
#include "utils/memory.h"

// Link to ntdll.lib, which contains the LdrDisableThreadCalloutsForDll function. lazy so its here
#pragma comment(lib, "ntdll.lib")

// Declare the external function from ntdll.dll, which disables thread callouts for a specific DLL.
extern "C" NTSTATUS NTAPI LdrDisableThreadCalloutsForDll(PVOID BaseAddress);

// Entry point for the DLL
bool APIENTRY DllMain(uintptr_t base, uint32_t reason_for_call, uint32_t reserved) {

    // Disable thread callouts for the 'client.dll' module
    // The BaseAddress of 'client.dll' is obtained using a utility function (find_module) and passed to LdrDisableThreadCalloutsForDll
    LdrDisableThreadCalloutsForDll(PVOID(utils::find_module(FNV1A("client.dll"))->base));

    if (reason_for_call == DLL_PROCESS_ATTACH)
    {
        // Create a new thread to initialize the manager asynchronously
        // The new thread will be detached immediately after it starts
        std::thread([base, reserved]()
            {
                // Create a new manager instance, passing the base address and reserved value to the constructor
                manager = std::make_unique<manager_t>(base, reserved);

                // Call the manager's init function to perform initialization tasks
                manager->init();
            }).detach();  // Detach the thread to let it run independently of the main thread
    }

    return TRUE;
}

