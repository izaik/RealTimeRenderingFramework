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

    DX11Framework* application;

    switch (message)
    {
    case WM_CREATE: //WindowProc needs to be static for CreateWindow(), we pass a DX11Framework pointer as an additional parameter
    {               //We reintepret the window creation information into a DX11Framework pointer
        application = static_cast<DX11Framework*>(reinterpret_cast<LPCREATESTRUCT>(lParam)->lpCreateParams);
        //Now add that pointer as some extra data associated with our window
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(application));
    }
    return 0;
    }

    //If not creating, we should have already stored the correct instance pointer, so now get it and call it
    application = reinterpret_cast<DX11Framework*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return application->HandleMessage(hWnd, message, wParam, lParam);
}