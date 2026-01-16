#include "GaugeUI.h"

#include "Sprite.h"
#include <DirectXMath.h>
#include "Defines.h" 
GaugeUI::GaugeUI()
    : m_pFrame(nullptr)
    , m_pGauge(nullptr)
    , m_rate(0.5f)
{
    m_pFrame = new Texture();
    if (FAILED(m_pFrame->Create("Assets/Texture/UIFrame.png"))) {
        MessageBox(nullptr, "UIFrame load failed", "Error", MB_OK);
    }
    m_pGauge = new Texture();
    if (FAILED(m_pGauge->Create("Assets/Texture/UIGauge.png"))) {
        MessageBox(nullptr, "UI_Gauge load failed", "Error", MB_OK);
    }
}
GaugeUI::~GaugeUI()
{
    delete m_pFrame;
    delete m_pGauge;
}
void GaugeUI::Draw()
{
   
    // ===== View / Projection（2D用）=====
    DirectX::XMFLOAT4X4 view, proj;

    auto mView = DirectX::XMMatrixIdentity(); // ★ UIはこれ
    auto mProj = DirectX::XMMatrixOrthographicLH(
        (float)SCREEN_WIDTH,
        (float)SCREEN_HEIGHT,
        0.0f,
        1.0f
    );

    DirectX::XMStoreFloat4x4(&view, DirectX::XMMatrixTranspose(mView));
    DirectX::XMStoreFloat4x4(&proj, DirectX::XMMatrixTranspose(mProj));

    Sprite::SetView(view);
    Sprite::SetProjection(proj);
    // ===== 位置・サイズ =====
    DirectX::XMFLOAT2 size = { 300, 30 };

    // 右上に配置
    DirectX::XMFLOAT2 pos = { SCREEN_WIDTH - size.x * 0.5f - 10.0f, 10.0f };
    // 右端から10px, 上から10px
    //// ===== 位置・サイズ =====
    //DirectX::XMFLOAT2 pos = { 200, 100 };
    //DirectX::XMFLOAT2 size = { 300, 30 };

    // ===== フレーム =====
    {
        auto T = DirectX::XMMatrixTranslation(
            pos.x - size.x * 0.5f,
            pos.y,
            0.0f
        );

        auto S = DirectX::XMMatrixScaling(
            1.0f,
            -1.0f,   // ★ Y反転
            1.0f
        );

        auto W = S * T;

        DirectX::XMFLOAT4X4 world;
        DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));

        Sprite::SetWorld(world);
        Sprite::SetSize(size);
        Sprite::SetOffset({ size.x * 0.5f, 0.0f });
        Sprite::SetTexture(m_pFrame);
        Sprite::Draw();
    }

    // ===== ゲージ =====
    {
        auto T = DirectX::XMMatrixTranslation(
            pos.x - size.x * 0.5f,
            pos.y,
            0.0f
        );

        auto S = DirectX::XMMatrixScaling(
            m_rate,
            -1.0f,   // ★ Y反転
            1.0f
        );

        auto W = S * T;

        DirectX::XMFLOAT4X4 world;
        DirectX::XMStoreFloat4x4(&world, DirectX::XMMatrixTranspose(W));

        Sprite::SetWorld(world);
        Sprite::SetSize(size);
        Sprite::SetOffset({ size.x * 0.5f, 0.0f });
        Sprite::SetTexture(m_pGauge);
        Sprite::Draw();
    }
}

