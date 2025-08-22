#include "Direct3D.h"
#include <windows.h>

bool Direct3D::Initialize(HWND hWnd, int width, int height)
{
    //=====================================================
    // DXGI�t�@�N�g���쐬
    //=====================================================
    ComPtr<IDXGIFactory> factory;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
    {
        return false;
    }

    //=====================================================
    // �f�o�C�X�쐬
    //=====================================================
    UINT creationFlags = 0;

#ifdef _DEBUG
    // �f�o�b�O�r���h���̂� Direct3D �f�o�b�O��L����
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
        nullptr,                     // �A�_�v�^�inullptr�Ȃ�����GPU�j
        D3D_DRIVER_TYPE_HARDWARE,    // �n�[�h�E�F�A�����_�����O
        nullptr,                     // �\�t�g�E�F�A�����_�����O�͎g�p���Ȃ�
        creationFlags,               // �t���O
        featureLevels,               // �T�|�[�g����@�\���x��
        _countof(featureLevels),     // �z��v�f��
        D3D11_SDK_VERSION,           // SDK�o�[�W����
        &m_device,                   // �f�o�C�X
        &featureLevel,               // ���ۂɎg�p�����@�\���x��
        &m_deviceContext)))          // �f�o�C�X�R���e�L�X�g
    {
        return false;
    }

    //=====================================================
    // �X���b�v�`�F�C���쐬
    //=====================================================
    DXGI_SWAP_CHAIN_DESC scDesc = {};
    scDesc.BufferDesc.Width = width;
    scDesc.BufferDesc.Height = height;
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    scDesc.BufferDesc.RefreshRate.Numerator = 0;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;
    scDesc.SampleDesc.Count = 1;  // MSAA�Ȃ�
    scDesc.SampleDesc.Quality = 0;
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scDesc.BufferCount = 2;  // �_�u���o�b�t�@
    scDesc.OutputWindow = hWnd;
    scDesc.Windowed = TRUE;
    scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    if (FAILED(factory->CreateSwapChain(m_device.Get(), &scDesc, &m_swapChain)))
    {
        return false;
    }

    //=====================================================
    // �o�b�N�o�b�t�@���擾���A�����_�[�^�[�Q�b�g�r���[���쐬
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
    // �����_�����O�ݒ�
    //=====================================================
    // �o�b�N�o�b�t�@�������_�[�^�[�Q�b�g�Ƃ��Đݒ�
    m_deviceContext->OMSetRenderTargets(1, m_backBufferView.GetAddressOf(), nullptr);

    // �r���[�|�[�g�ݒ�
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
