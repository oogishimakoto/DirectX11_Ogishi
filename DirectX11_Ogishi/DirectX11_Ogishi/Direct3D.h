#pragma once

//==============================================================
// 必要なライブラリ
//==============================================================
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

//==============================================================
// Direct3D クラス（シングルトン）
//==============================================================
class Direct3D
{
public:
    //==========================================================
    // メンバ変数（本来は private にすべきだが今回は簡易化のため public）
    //==========================================================
    ComPtr<ID3D11Device>            m_device;        // Direct3Dデバイス
    ComPtr<ID3D11DeviceContext>     m_deviceContext; // デバイスコンテキスト
    ComPtr<IDXGISwapChain>          m_swapChain;     // スワップチェイン
    ComPtr<ID3D11RenderTargetView>  m_backBufferView;// バックバッファのRTビュー

    //==========================================================
    // 初期化処理
    // hWnd   : ウィンドウハンドル
    // width  : 画面の幅
    // height : 画面の高さ
    //==========================================================
    bool Initialize(HWND hWnd, int width, int height);

    //==========================================================
    // シングルトン実装
    //==========================================================
private:
    static inline Direct3D* s_instance = nullptr; // 唯一のインスタンス
    Direct3D() = default;                         // コンストラクタは非公開

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
// ショートカットマクロ
//==============================================================
#define D3D Direct3D::GetInstance()
