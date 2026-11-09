#include "UserInput.h"

UserInput::UserInput() {};

LRESULT UserInput::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_KEYDOWN:
        UserInput::OnKeyDown(lParam);
        return 0;

    case WM_KEYUP:
        UserInput::OnKeyUp(lParam);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
};

void UserInput::OnKeyDown(LPARAM lParam)
{
    WORD keyFlags = HIWORD(lParam);

    WORD scanCode = LOBYTE(keyFlags);
    BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED;

    if (isExtendedKey)
        scanCode = MAKEWORD(scanCode, 0xE0);

    uint8_t index = LOBYTE(scanCode);
    if (HIBYTE(scanCode)) index |= 0x80;

    _keys[windows_scancode_table[index]] = true;
};

void UserInput::OnKeyUp(LPARAM lparam)
{
    WORD keyFlags = HIWORD(lparam);

    WORD scanCode = LOBYTE(keyFlags);
    BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED;

    if (isExtendedKey)
        scanCode = MAKEWORD(scanCode, 0xE0);

    uint8_t index = LOBYTE(scanCode);
    if (HIBYTE(scanCode)) index |= 0x80;

    _keys[windows_scancode_table[index]] = false;
};

void UserInput::ProcessInput(ComPtr<ID3D11DeviceContext>& context,
    ComPtr<ID3D11RasterizerState>& wireframeState, ComPtr<ID3D11RasterizerState>& fillState,
    Camera* camera, float deltaTime) {
    if (_keys[SCANCODE_TAB])
    {
        context->RSSetState(wireframeState.Get());
    }
    else
    {
        context->RSSetState(fillState.Get());
    }

    if (_keys[SCANCODE_W]) {
        camera->ProcessKeyboardInput(FORWARD, deltaTime);
    }
    if (_keys[SCANCODE_D]) {
        camera->ProcessKeyboardInput(RIGHT, deltaTime);
    }
    if (_keys[SCANCODE_S]) {
        camera->ProcessKeyboardInput(BACKWARD, deltaTime);
    }
    if (_keys[SCANCODE_A]) {
        camera->ProcessKeyboardInput(LEFT, deltaTime);
    }
};