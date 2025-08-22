#pragma once

//==============================================================
// �K�v�ȃ��C�u����
//==============================================================
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

//==============================================================
// Direct3D �N���X�i�V���O���g���j
//==============================================================
class Direct3D
{
public:
    //==========================================================
    // �����o�ϐ��i�{���� private �ɂ��ׂ���������͊ȈՉ��̂��� public�j
    //==========================================================
    ComPtr<ID3D11Device>            m_device;        // Direct3D�f�o�C�X
    ComPtr<ID3D11DeviceContext>     m_deviceContext; // �f�o�C�X�R���e�L�X�g
    ComPtr<IDXGISwapChain>          m_swapChain;     // �X���b�v�`�F�C��
    ComPtr<ID3D11RenderTargetView>  m_backBufferView;// �o�b�N�o�b�t�@��RT�r���[

    //==========================================================
    // ����������
    // hWnd   : �E�B���h�E�n���h��
    // width  : ��ʂ̕�
    // height : ��ʂ̍���
    //==========================================================
    bool Initialize(HWND hWnd, int width, int height);

    //==========================================================
    // �V���O���g������
    //==========================================================
private:
    static inline Direct3D* s_instance = nullptr; // �B��̃C���X�^���X
    Direct3D() = default;                         // �R���X�g���N�^�͔���J

public:
    static void CreateInstance()
    {
        DeleteInstance();
        s_instance = new Direct3D();
    }

    static void DeleteInstance()
    {
        delete s_instance;
        s_instance = nullptr;
    }

    static Direct3D& GetInstance()
    {
        return *s_instance;
    }
};

//==============================================================
// �V���[�g�J�b�g�}�N��
//==============================================================
#define D3D Direct3D::GetInstance()
