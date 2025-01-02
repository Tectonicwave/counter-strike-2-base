#pragma once

// Enum for key states
enum KeyState_t
{
    KEY_STATE_NONE = 0,
    KEY_STATE_DOWN,
    KEY_STATE_UP,
    KEY_STATE_RELEASED
};

// KeyManager class to handle key states
class KeyManager
{
public:
    KeyManager()
    {
        // Initialize all key states to KEY_STATE_NONE
        std::fill(std::begin(key_state), std::end(key_state), KEY_STATE_NONE);
    }

    // Check if a key was released
    [[nodiscard]] bool IsKeyReleased(const std::uint32_t uButtonCode)
    {
        if (key_state[uButtonCode] == KEY_STATE_RELEASED)
        {
            key_state[uButtonCode] = KEY_STATE_UP; // Reset state to UP
            return true;
        }
        return false;
    }

    // Check if a key is pressed
    [[nodiscard]] bool IsKeyPressed(const std::uint32_t uButtonCode) const
    {
        return key_state[uButtonCode] == KEY_STATE_DOWN;
    }

    // Check if a key is currently down
    [[nodiscard]] bool IsKeyDown(const std::uint32_t uButtonCode) const
    {
        return key_state[uButtonCode] == KEY_STATE_DOWN || key_state[uButtonCode] == KEY_STATE_RELEASED;
    }

    // Update the key state based on Windows messages
    void UpdateKeyState(UINT uMsg, WPARAM wParam)
    {
        int nKey = 0;
        KeyState_t state = KEY_STATE_NONE;

        switch (uMsg)
        {
        case WM_KEYDOWN:
        case WM_SYSKEYDOWN:
            if (wParam < 256U)
            {
                nKey = static_cast<int>(wParam);
                state = KEY_STATE_DOWN;
            }
            break;

        case WM_KEYUP:
        case WM_SYSKEYUP:
            if (wParam < 256U)
            {
                nKey = static_cast<int>(wParam);
                state = KEY_STATE_UP;
            }
            break;

        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
            nKey = VK_LBUTTON;
            state = (uMsg == WM_LBUTTONUP) ? KEY_STATE_UP : KEY_STATE_DOWN;
            break;

        case WM_RBUTTONDOWN:
        case WM_RBUTTONUP:
            nKey = VK_RBUTTON;
            state = (uMsg == WM_RBUTTONUP) ? KEY_STATE_UP : KEY_STATE_DOWN;
            break;

        case WM_MBUTTONDOWN:
        case WM_MBUTTONUP:
            nKey = VK_MBUTTON;
            state = (uMsg == WM_MBUTTONUP) ? KEY_STATE_UP : KEY_STATE_DOWN;
            break;

        case WM_XBUTTONDOWN:
        case WM_XBUTTONUP:
            nKey = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2;
            state = (uMsg == WM_XBUTTONUP) ? KEY_STATE_UP : KEY_STATE_DOWN;
            break;

        default:
            return; // Ignore other messages
        }

        // Update key states
        if (state == KEY_STATE_UP && key_state[nKey] == KEY_STATE_DOWN)
        {
            key_state[nKey] = KEY_STATE_RELEASED;
        }
        else
        {
            key_state[nKey] = state;
        }
    }

private:
    KeyState_t key_state[256]; // Array to track key states
};
