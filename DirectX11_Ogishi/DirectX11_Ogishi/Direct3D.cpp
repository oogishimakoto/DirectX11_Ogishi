#include "Direct3D.h"
#include <windows.h>

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
    //=====================================================
    // DXGIファクトリ作成
    //=====================================================
    ComPtr<IDXGIFactory> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
    {
        return false;
    }

    //=====================================================
    // デバイス作成
    //=====================================================
    UINT creationFlags = 0;

#ifdef _DEBUG
    // デバッグビルド時のみ Direct3D デバッグを有効化
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_1, // Direct3D 11.1 ShaderModel 5
        D3D_FEATURE_LEVEL_11_0, // Direct3D 11   ShaderModel 5
        D3D_FEATURE_LEVEL_10_1, // Direct3D 10.1 ShaderModel 4
        D3D_FEATURE_LEVEL_10_0, // Direct3D 10.0 ShaderModel 4
        D3D_FEATURE_LEVEL_9_3,  // Direct3D 9.3  ShaderModel 3
        D3D_FEATURE_LEVEL_9_2,  // Direct3D 9.2  ShaderModel 3
        D3D_FEATURE_LEVEL_9_1,  // Direct3D 9.1  ShaderModel 3
    };

    D3D_FEATURE_LEVEL featureLevel;
    if (FAILED(D3D11CreateDevice(
        nullptr,                     // アダプタ（nullptrなら既定のGPU）
        D3D_DRIVER_TYPE_HARDWARE,    // ハードウェアレンダリング
        nullptr,                     // ソフトウェアレンダリングは使用しない
        creationFlags,               // フラグ
        featureLevels,               // サポートする機能レベル
        _countof(featureLevels),     // 配列要素数
        D3D11_SDK_VERSION,           // SDKバージョン
        &m_device,                   // デバイス
        &featureLevel,               // 実際に使用される機能レベル
        &m_deviceContext)))          // デバイスコンテキスト
    {
        return false;
    }

    //=====================================================
    // スワップチェイン作成
    //=====================================================
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferDesc.Width = width;
    scDesc.BufferDesc.Height = height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.SampleDesc.Count = 1;  // MSAAなし
    scDesc.SampleDesc.Quality = 0;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.BufferCount = 2;  // ダブルバッファ
    scDesc.OutputWindow = hWnd;
    scDesc.Windowed = TRUE;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, &m_swapChain)))
    {
        return false;
    }

    //=====================================================
    // バックバッファを取得し、レンダーターゲットビューを作成
    //=====================================================
    ComPtr<ID3D11Texture2D> backBuffer;
    if (FAILED(m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer))))
    {
        return false;
    }

    D3D11_RENDER_TARGET_VIEW_DESC rtvDesc = {};
    rtvDesc.Format = scDesc.BufferDesc.Format;
    rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    if (FAILED(m_device->CreateRenderTargetView(backBuffer.Get(), &rtvDesc, &m_backBufferView)))
    {
        return false;
    }

    //=====================================================
    // レンダリング設定
    //=====================================================
    // バックバッファをレンダーターゲットとして設定
    m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

    // ビューポート設定
    D3D11_VIEWPORT vp = {};
    vp.TopLeftX = 0.0f;
    vp.TopLeftY = 0.0f;
    vp.Width = static_cast<float>(width);
    vp.Height = static_cast<float>(height);
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;

    m_deviceContext->RSSetViewports(1, &vp);

    return true;
}
