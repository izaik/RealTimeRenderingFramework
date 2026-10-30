#include "DX11Framework.h"
#include <cmath>
#include "Util.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_dx11.h"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

LRESULT DX11Framework::HandleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    switch (message)
    {
    case WM_KEYDOWN:
        DX11Framework::OnKeyDown(lParam);
        return 0;

    case WM_KEYUP:
        DX11Framework::OnKeyUp(lParam);
        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    // Handle any messages the switch statement didn't.
    return DefWindowProc(hWnd, message, wParam, lParam);
}

void DX11Framework::OnKeyDown(LPARAM lParam)
{
    WORD keyFlags = HIWORD(lParam);

    WORD scanCode = LOBYTE(keyFlags);
    BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED;

    if (isExtendedKey)
        scanCode = MAKEWORD(scanCode, 0xE0);

    uint8_t index = LOBYTE(scanCode);
    if (HIBYTE(scanCode)) index |= 0x80;

    _keys[windows_scancode_table[index]] = true;
}

void DX11Framework::OnKeyUp(LPARAM lparam)
{
    WORD keyFlags = HIWORD(lparam);

    WORD scanCode = LOBYTE(keyFlags);
    BOOL isExtendedKey = (keyFlags & KF_EXTENDED) == KF_EXTENDED;

    if (isExtendedKey)
        scanCode = MAKEWORD(scanCode, 0xE0);

    uint8_t index = LOBYTE(scanCode);
    if (HIBYTE(scanCode)) index |= 0x80;

    _keys[windows_scancode_table[index]] = false;
}

void DX11Framework::Initialise(HINSTANCE hInstance, int nShowCmd)
{
    CreateWindowHandle(hInstance, nShowCmd);
    CreateD3DDevice();
    CreateSwapChainAndFrameBuffer();
    InitShadersAndInputLayout();
    InitImGui();
    InitPipelineVariables();
    InitRunTimeData();
}

void DX11Framework::CreateWindowHandle(HINSTANCE hInstance, int nCmdShow)
{
    const wchar_t* windowName  = L"DX11Framework";

    WNDCLASSW wndClass;
    wndClass.style = 0;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = 0;
    wndClass.hIcon = 0;
    wndClass.hCursor = 0;
    wndClass.hbrBackground = 0;
    wndClass.lpszMenuName = 0;
    wndClass.lpszClassName = windowName;

    RegisterClassW(&wndClass);

    _windowHandle = CreateWindowExW(0, windowName, windowName, WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 
        _WindowWidth, _WindowHeight, nullptr, nullptr, hInstance, this);

}

void DX11Framework::CreateD3DDevice()
{
    ThrowOnFail(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory)));

    constexpr D3D_FEATURE_LEVEL deviceFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

    DWORD createDeviceFlags = 0;
#ifdef _DEBUG
    // If we're building for Debug, contain device warnings and errors
    // These are output to the visual studio console
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ThrowOnFail(D3D11CreateDevice(nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        D3D11_CREATE_DEVICE_BGRA_SUPPORT | createDeviceFlags,
        &deviceFeatureLevel,
        1,
        D3D11_SDK_VERSION,
        _device.GetAddressOf(),
        nullptr,
        _immediateContext.GetAddressOf()));
}

void DX11Framework::CreateSwapChainAndFrameBuffer()
{
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width = 0; // Defer to WindowWidth
    swapChainDesc.Height = 0; // Defer to WindowHeight
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags = 0;

    ThrowOnFail(_dxgiFactory->CreateSwapChainForHwnd(_device.Get(), _windowHandle, &swapChainDesc, nullptr, nullptr, _swapChain.GetAddressOf()));

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ComPtr<ID3D11Texture2D> frameBuffer = nullptr;

    ThrowOnFail(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &frameBuffer));

    D3D11_RENDER_TARGET_VIEW_DESC framebufferDesc = {};
    framebufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    framebufferDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    ThrowOnFail(_device->CreateRenderTargetView(frameBuffer.Get(), &framebufferDesc, _frameBufferView.GetAddressOf()));

    // depth buffer

    D3D11_TEXTURE2D_DESC depthTextureDesc{};
    frameBuffer->GetDesc(&depthTextureDesc);

    depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthTextureDesc.Format = DXGI_FORMAT_R32_TYPELESS;

    ComPtr<ID3D11Texture2D> texture = nullptr;
    ThrowOnFail(_device->CreateTexture2D(&depthTextureDesc, nullptr, texture.GetAddressOf()));

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc{};
    depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
    depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    ThrowOnFail(_device->CreateDepthStencilView(texture.Get(), &depthStencilDesc, _depthStencilView.GetAddressOf()));
}

void DX11Framework::CompileShader(const std::wstring& fileName, const std::string& entryPoint, const std::string& profile, ComPtr<ID3DBlob>& shaderBlob) const
{
    ComPtr<ID3DBlob> errorBlob;

    DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
    // Set the D3DCOMPILE_DEBUG flag and D3DCOMPILE_SKIP_OPTIMIZATION
    // This means the shader runs slower, but debugging is easier
    // As optimizations will move/remove lines of code in the shader
    // https://learn.microsoft.com/en-us/windows/win32/direct3dhlsl/d3dcompile-constants
    dwShaderFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    HRESULT hr;
    hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), profile.c_str(), dwShaderFlags, 0, &shaderBlob, &errorBlob);
    if (FAILED(hr))
    {
        //Inspect msg or check VS Output to see compilation error
        std::string msg((char*)errorBlob->GetBufferPointer());
        OutputDebugStringA(msg.c_str());
        throw std::exception();
    }
}

void DX11Framework::InitShadersAndInputLayout()
{    
    ComPtr<ID3DBlob> vertexShaderBlob;

    CompileShader(L"SimpleShaders.hlsl", "VS_main", "vs_5_0", vertexShaderBlob);
    ThrowOnFail(_device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, _vertexShader.GetAddressOf()));

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,   0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    ThrowOnFail(_device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), _inputLayout.GetAddressOf()));
    ///////////////////////////////////////////////////////////////////////////////////////////////

    ComPtr<ID3DBlob> pixelShaderBlob;
    CompileShader(L"SimpleShaders.hlsl", "PS_main", "ps_5_0", pixelShaderBlob);

    ThrowOnFail(_device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, _pixelShader.GetAddressOf()));
}

//ImGui is a simple UI framework for development, it is not designed for release quality UI
void DX11Framework::InitImGui()
{
    //This project only contains ImGui implementations for Win32 and DirectX
    //ImGui works across many backends, for full specs see https://github.com/ocornut/imgui/
    ImGui_ImplWin32_EnableDpiAwareness();
    float main_scale = ImGui_ImplWin32_GetDpiScaleForMonitor(::MonitorFromPoint(POINT{ 0, 0 }, MONITOR_DEFAULTTOPRIMARY));

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    //ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

    // Setup scaling
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(main_scale);
    style.FontScaleDpi = main_scale;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(_windowHandle);
    ImGui_ImplDX11_Init(_device.Get(), _immediateContext.Get());
}

void DX11Framework::InitPipelineVariables()
{
    //Input Assembler
    _immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _immediateContext->IASetInputLayout(_inputLayout.Get());

    //Rasterizer - Fill
    D3D11_RASTERIZER_DESC rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    ThrowOnFail(_device->CreateRasterizerState(&rasterizerDesc, _fillState.GetAddressOf()));
    _immediateContext->RSSetState(_fillState.Get());

    //Rasterizer - Wireframe
    D3D11_RASTERIZER_DESC wireframeDesc = {};
    wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
    wireframeDesc.CullMode = D3D11_CULL_NONE;

    ThrowOnFail(_device->CreateRasterizerState(&wireframeDesc, _wireframeState.GetAddressOf()));

    //Viewport Values
    _viewport = { 0.0f, 0.0f, (float)_WindowWidth, (float)_WindowHeight, 0.0f, 1.0f };
    _immediateContext->RSSetViewports(1, &_viewport);

    //Constant Buffer
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
    constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    ThrowOnFail(_device->CreateBuffer(&constantBufferDesc, nullptr, _constantBuffer.GetAddressOf()));

    _immediateContext->VSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());
    _immediateContext->PSSetConstantBuffers(0, 1, _constantBuffer.GetAddressOf());

    //Texture Sampler Values
    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0;
    samplerDesc.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    ThrowOnFail(_device->CreateSamplerState(&samplerDesc, _anisotropicSampler.GetAddressOf()));
    _immediateContext->PSSetSamplers(0, 1, _anisotropicSampler.GetAddressOf());
}

void DX11Framework::InitRunTimeData()
{    
    //Camera
    XMFLOAT3 Eye = XMFLOAT3(0, 0, -20.0f);
    XMFLOAT3 At = XMFLOAT3(0, 0, 0);
    XMFLOAT3 Up = XMFLOAT3(0, 1, 0);

    XMStoreFloat4x4(&_View, XMMatrixLookAtLH(XMLoadFloat3(&Eye), XMLoadFloat3(&At), XMLoadFloat3(&Up)));

    //Projection
    float aspect = _viewport.Width / _viewport.Height;
    XMMATRIX perspective = XMMatrixPerspectiveFovLH(XMConvertToRadians(90), aspect, 0.01f, 100.0f);
    XMStoreFloat4x4(&_Projection, perspective);

    //store camera position for phong
    _cbData.camPos = Eye;

    //Materials & Lights
    _defaultMaterial.Diffuse = XMFLOAT4(0.75164f, 0.60648f, 0.22648f, 1.0f);
    _defaultMaterial.Ambient = XMFLOAT4(0.24725f, 0.1995f, 0.0745f, 1.0f);
    _defaultMaterial.Specular = XMFLOAT4(0.628281f, 0.555802f, 0.366065f, 1.0f);
    _defaultMaterial.Shininess = float(52.2f);
    //more material values at http://devernay.free.fr/cours/opengl/materials.html
   
    //load light from json config
    std::ifstream input_json(L"Lights/WhiteLight.json");
    json j;
    input_json >> j;

    _loadedLight.Diffuse = XMFLOAT4(j["Diffuse"][0], j["Diffuse"][1], j["Diffuse"][2], j["Diffuse"][3]);
    _loadedLight.Ambient = XMFLOAT4(j["Ambient"][0], j["Ambient"][1], j["Ambient"][2], j["Ambient"][3]);
    _loadedLight.Specular = XMFLOAT4(j["Specular"][0], j["Specular"][1], j["Specular"][2], j["Specular"][3]);
    _loadedLight.Direction = XMFLOAT3(j["Direction"][0], j["Direction"][1], j["Direction"][2]);
    

    //load crate diffuse
    CreateDDSTextureFromFile(
        _device.Get(),
        _immediateContext.Get(),
        L"Textures/Crate_COLOR.dds",
        nullptr,
        _crateDiffuseTex.GetAddressOf()
    );
    //load crate specular
    CreateDDSTextureFromFile(
        _device.Get(),
        _immediateContext.Get(),
        L"Textures/Crate_SPEC.dds",
        nullptr,
        _crateSpecularTex.GetAddressOf()
    );

    objectTest->SetMeshData(OBJLoader::Load("Models/Airplane/Hercules.obj", _device));
    objectTest->SetDiffuse(_crateDiffuseTex, _immediateContext);
    objectTest->SetSpecular(_crateSpecularTex, _immediateContext);
}

DX11Framework::~DX11Framework()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void DX11Framework::Update()
{
    //Static initializes this value only once    
    static ULONGLONG frameStart = GetTickCount64();

    ULONGLONG frameNow = GetTickCount64();
    deltaTime = (frameNow - frameStart) / 1000.0f;
    frameStart = frameNow;

    simpleCount += deltaTime;

    _cbData.timeSinceStart = simpleCount;

    objectTest->Update(_cbData.timeSinceStart);

    if (_keys[SCANCODE_TAB])
    {
        _immediateContext->RSSetState(_wireframeState.Get());
    }
    else
    {
        _immediateContext->RSSetState(_fillState.Get());
    }
}

void DX11Framework::Draw()
{                
    SetUpUI();

    //Present unbinds render target, so rebind and clear at start of each frame
    float backgroundColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };  
    _immediateContext->OMSetRenderTargets(1, _frameBufferView.GetAddressOf(), _depthStencilView.Get());
    _immediateContext->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
    _immediateContext->ClearRenderTargetView(_frameBufferView.Get(), backgroundColor);
  
    //Store this frames data in constant buffer struct
    _cbData.World = XMMatrixTranspose(XMLoadFloat4x4(&_World));
    _cbData.View = XMMatrixTranspose(XMLoadFloat4x4(&_View));
    _cbData.Projection = XMMatrixTranspose(XMLoadFloat4x4(&_Projection));
    _cbData.mat = _defaultMaterial;
    _cbData.light = _loadedLight;

    //Write constant buffer data onto GPU
    D3D11_MAPPED_SUBRESOURCE mappedSubresource;
    _immediateContext->Map(_constantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
    memcpy(mappedSubresource.pData, &_cbData, sizeof(_cbData));
    _immediateContext->Unmap(_constantBuffer.Get(), 0);

    _immediateContext->VSSetShader(_vertexShader.Get(), nullptr, 0);
    _immediateContext->PSSetShader(_pixelShader.Get(), nullptr, 0);

    objectTest->Draw(_cbData, _immediateContext, _constantBuffer, mappedSubresource);

    //UI renders last, as this is painted over whatver is on screen
    RenderUI();

    //Present Backbuffer to screen
    _swapChain->Present(0, 0);
}

void DX11Framework::SetUpUI() const
{
    //Can be done at any point in a frame
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    //ImGui::ShowDemoWindow();

    ImGui::Begin("Simple Metrics");
    ImGui::Text("Time since start: %.3f", simpleCount);
    float angle = XMConvertToDegrees(userRotate);
    ImGui::Text("Square Z rotation: %.0f°", std::fmod(angle, 360.f));
    ImGui::End();
}

void DX11Framework::RenderUI()
{
    // Must be done after drawing objects and before present
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}