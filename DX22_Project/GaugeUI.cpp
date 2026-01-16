#include "GaugeUI.h"

#include "Sprite.h"
#include <DirectXMath.h>
#include "Defines.h"

using namespace DirectX;

GaugeUI::GaugeUI()
    : m_pFrame(nullptr)
    , m_pGauge(nullptr)
    , m_rate(0.5f)
{
    m_pFrame = new Texture();
    if (FAILED(m_pFrame->Create("Assets/Texture/UIFrame.png")))
    {
        MessageBox(nullptr, "UIFrame load failed", "Error", MB_OK);
    }

    m_pGauge = new Texture();
    if (FAILED(m_pGauge->Create("Assets/Texture/UIGauge.png")))
    {
        MessageBox(nullptr, "UIGauge load failed", "Error", MB_OK);
    }
}

GaugeUI::~GaugeUI()
{
    delete m_pFrame;
    delete m_pGauge;
}

void GaugeUI::Draw()
{
    using namespace DirectX;

    XMFLOAT2 size = { 300.0f, 30.0f };

    XMFLOAT2 pos =
    {
        SCREEN_WIDTH - (size.x * 0.5f) - 10.0f,
        (size.y * 0.5f) + 10.0f
    };

    // ===============================
    // フレーム
    // ===============================
    {
        XMMATRIX W =
            XMMatrixTranslation(pos.x, pos.y, 0.0f); // ← БЕЗ SCALE

        XMFLOAT4X4 world;
        XMStoreFloat4x4(&world, XMMatrixTranspose(W));

        Sprite::SetWorld(world);
        Sprite::SetSize(size);                  // ← ТОЛЬКО ТУТ
        Sprite::SetOffset({ 0.0f, 0.0f });
        Sprite::SetTexture(m_pFrame);
        Sprite::SetUVPos({ 0.0f, 0.0f });
        Sprite::SetUVScale({ 1.0f, 1.0f });
        Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        Sprite::Draw();
    }

    // ===============================
    // ゲージ
    // ===============================
    {
        float gaugeWidth = size.x * m_rate;

        XMMATRIX W =
            XMMatrixTranslation(pos.x, pos.y, 0.0f); // ← БЕЗ SCALE

        XMFLOAT4X4 world;
        XMStoreFloat4x4(&world, XMMatrixTranspose(W));

        Sprite::SetWorld(world);
        Sprite::SetSize({ gaugeWidth, size.y }); // ← ширина = rate
        Sprite::SetOffset({ 0.0f, 0.0f });
        Sprite::SetTexture(m_pGauge);
        Sprite::SetUVPos({ 0.0f, 0.0f });
        Sprite::SetUVScale({ 1.0f, 1.0f });
        Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
        Sprite::Draw();
    }
}
