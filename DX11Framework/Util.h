#pragma once

#include <Windows.h>
#include <exception>
#include "IMGUI/imgui_impl_win32.h"

inline void ThrowOnFail(HRESULT x)
{
    if (FAILED(x))
        //Check VS Output for error and call stack for location of error
        throw std::exception();
};

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
        return true;

    DX11Framework* application = nullptr;

    //without all these if statements gethandle crashes :(
    if (message == WM_CREATE)
    {
        application = static_cast<DX11Framework*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
        return 0;
    }
    else
    {
        application = reinterpret_cast<DX11Framework*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    // if the application isnt initialized yet, skip and use default window proc
    if (application == nullptr) {
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    if (application->GetHandle() != nullptr) {
        RAWINPUTDEVICE rawInputDevice[1];
        rawInputDevice[0].usUsagePage = 0x01;
        rawInputDevice[0].usUsage = 0x02;
        rawInputDevice[0].dwFlags = RIDEV_INPUTSINK;
        rawInputDevice[0].hwndTarget = application->GetHandle();

        RegisterRawInputDevices(rawInputDevice, 1, sizeof(RAWINPUTDEVICE));
    }

    ShowCursor(FALSE);

    return application->_userInput.HandleMessage(hWnd, message, wParam, lParam);
}