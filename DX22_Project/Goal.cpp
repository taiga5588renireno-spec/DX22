#include "Goal.h"
#include "Sprite.h"
#include <DirectXMath.h>
#include "Collision.h"


Goal::Goal(DirectX::XMFLOAT3 size)
    : m_pGoalTex(nullptr)
    , m_pCamera(nullptr)
{
    

        m_pGoalTex = new Texture();
        if (FAILED(m_pGoalTex->Create("Assets/Texture/GoalTex.png"))) {
            MessageBox(NULL, "Texture load failed.\nGoal.cpp", "Error", MB_OK);
        }
    
}

Goal::~Goal()
{
    if (m_pGoalTex) {
        delete m_pGoalTex;
        m_pGoalTex = nullptr;
    }
}
void Goal::Update()
{
    // PDFでは詳細省略（必要なら上下にふわふわさせる処理など）
}

void Goal::Draw()
{
    using namespace DirectX;

    XMMATRIX mCamInv = XMMatrixIdentity();

    if (m_pCamera)
    {
        XMFLOAT4X4 viewT = m_pCamera->GetViewMatrix();

        XMMATRIX view = XMMatrixTranspose(XMLoadFloat4x4(&viewT));

        mCamInv = XMMatrixInverse(nullptr, view);

        XMFLOAT4X4 invView;
        XMStoreFloat4x4(&invView, mCamInv);
        invView._41 = 0.0f;
        invView._42 = 0.0f;
        invView._43 = 0.0f;

        mCamInv = XMLoadFloat4x4(&invView);
    }

    XMMATRIX worldMat =
        mCamInv *
        XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

    XMFLOAT4X4 world;
    XMStoreFloat4x4(&world, XMMatrixTranspose(worldMat));

    Sprite::SetColor({ 1,1,1,1 });
    Sprite::SetOffset({ 0.0f,0.0f });
    Sprite::SetWorld(world);
    Sprite::SetSize({ 0.64f,0.32f });
    Sprite::SetTexture(m_pGoalTex);
    Sprite::Draw();
}
void Goal::SetCamera(Camera* camera)
{
    m_pCamera = camera;
}