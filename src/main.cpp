// main.cpp - Entry point for Roxy Engine
//

#include <SDL.h>
#include <SDL_syswm.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <iostream>
#include <Windows.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

using namespace DirectX;

// Globals for DirectX 11
ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_deviceContext = nullptr;
IDXGISwapChain* g_swapChain = nullptr;
ID3D11RenderTargetView* g_renderTargetView = nullptr;

// Window dimensions
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

bool InitializeDirectX11(SDL_Window* window)
{
    // Get window handle from SDL
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(window, &wmInfo);
    HWND hwnd = wmInfo.info.win.window;

    // Swap chain description
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = WINDOW_WIDTH;
    swapChainDesc.BufferDesc.Height = WINDOW_HEIGHT;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    // Create device, device context, and swap chain
    D3D_FEATURE_LEVEL featureLevel;
    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        flags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &g_swapChain,
        &g_device,
        &featureLevel,
        &g_deviceContext
    );

    if (FAILED(hr))
    {
        std::cerr << "Failed to create D3D11 device and swap chain!" << std::endl;
        return false;
    }

    // Create render target view
    ID3D11Texture2D* backBuffer = nullptr;
    g_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    g_device->CreateRenderTargetView(backBuffer, nullptr, &g_renderTargetView);
    backBuffer->Release();

    // Set render target
    g_deviceContext->OMSetRenderTargets(1, &g_renderTargetView, nullptr);

    // Setup viewport
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<float>(WINDOW_WIDTH);
    viewport.Height = static_cast<float>(WINDOW_HEIGHT);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    g_deviceContext->RSSetViewports(1, &viewport);

    std::cout << "DirectX 11 initialized successfully!" << std::endl;
    return true;
}

void Render()
{
    // Clear the back buffer to a nice blue color (Portal-ish)
    float clearColor[4] = { 0.1f, 0.2f, 0.3f, 1.0f };
    g_deviceContext->ClearRenderTargetView(g_renderTargetView, clearColor);

    // Present the frame
    g_swapChain->Present(1, 0);
}

void Cleanup()
{
    if (g_renderTargetView) g_renderTargetView->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_deviceContext) g_deviceContext->Release();
    if (g_device) g_device->Release();
}

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Roxy Engine - Portal Clone",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Initialize DirectX 11
    if (!InitializeDirectX11(window))
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    std::cout << "==================================" << std::endl;
    std::cout << "    Roxy Engine - Portal Clone    " << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "DirectX 11 Renderer Initialized" << std::endl;
    std::cout << "Press ESC to exit" << std::endl;

    // Main game loop
    bool running = true;
    SDL_Event event;

    while (running)
    {
        // Handle events
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }
        }

        // Render
        Render();
    }

    // Cleanup
    Cleanup();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}