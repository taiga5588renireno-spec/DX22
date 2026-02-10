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
    // --- ビルボード計算用行列 ---
    DirectX::XMMATRIX mCamInv =
        DirectX::XMMatrixIdentity();

    if (m_pCamera)
    {
        // スプライト表示用の行列を設定
      /*  Sprite::SetView(m_pCamera->GetViewMatrix());
        Sprite::SetProjection(m_pCamera->GetProjectionMatrix());*/

        // 転置していないカメラの行列を取得
        DirectX::XMFLOAT4X4 view = m_pCamera->GetViewMatrix();

        // 読み取り用 → 計算用
        mCamInv = XMLoadFloat4x4(&view);

  
        // 逆行列を計算
        mCamInv = XMMatrixInverse(nullptr, XMLoadFloat4x4(&view));

        // 計算用 → 読み取り用
        XMStoreFloat4x4(&view, mCamInv);

        // 移動成分を削除
        view._41 = 0.0f;
        view._42 = 0.0f;
        view._43 = 0.0f;

        // 再び計算用に変換
        mCamInv = XMLoadFloat4x4(&view);
    }

    // --- ビルボードを含めたワールド行列 ---
    DirectX::XMMATRIX WorldMat =
        mCamInv *
        DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

    // スプライト用に転置
    DirectX::XMFLOAT4X4 world;
    DirectX::XMStoreFloat4x4(&world, XMMatrixTranspose(WorldMat));

    // --- 描画 ---
    Sprite::SetColor({ 1,1,1,1 });
    Sprite::SetOffset({ 0.0f,0.0f });
    Sprite::SetWorld(world);
    Sprite::SetSize({ 0.64f,0.32f });
    Sprite::SetTexture(m_pGoalTex);
    Sprite::Draw();
}
//Collision::Box Goal::GetCollision()
//{
//    return m_collision;
//}
void Goal::SetCamera(Camera* camera)
{
    m_pCamera = camera;
}