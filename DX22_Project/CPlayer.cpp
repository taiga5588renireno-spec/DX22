#include "CPlayer.h"
#include "Input.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "GaneObject.h"
#include "CameraDebug.h"
#include "SceneGame.h"
#include "Camera.h"
#include "Defines.h"

using namespace std;
 

enum eShotStep {
    SHOT_WAIT,
    SHOT_KEEP,
    SHOT_RELEASE,
};

CPlayer::CPlayer()
    :m_pCamera(nullptr), m_move()
    , m_isStop(true), m_isGround(true), m_shotStep(0), m_shotPower(0.0f)
    ,m_collision()
   
{
    m_collision.size = { 0.2f,0.2f,0.2f };
}

CPlayer::~CPlayer()
{

}
void CPlayer::Update()
{
    if (!m_pCamera){return;}

    if (m_isStop)
        UpdateShot();
    else
        UpdateMove();

    m_collision.center = m_pos;
}
void CPlayer::Draw()
{
    using namespace DirectX;

    // スケール（今回はそのまま）
    XMMATRIX S = XMMatrixScaling(1.0f, 1.0f, 1.0f);

    // ★ Y軸に90度回転
    XMMATRIX R = XMMatrixRotationY(XMConvertToRadians(90.0f));

    // 平行移動
    XMMATRIX T = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

    // 行列合成（重要な順番）
    XMMATRIX mat = S * R * T;
    mat = XMMatrixTranspose(mat);



   /* DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
    DirectX::XMMATRIX mat;
    mat = XMMatrixTranspose(T);*/

    DirectX::XMFLOAT4X4 fMat;
    DirectX::XMStoreFloat4x4(&fMat, mat);

    //描画
    Geometory::SetWorld(fMat);
    Geometory::DrawBox();

}

void CPlayer::SetCamera(Camera* pCamera)
{
    m_pCamera = pCamera;
}

Collision::Box CPlayer::GetCollision()
{
    return m_collision;
}

//====================================
// 描画
//====================================


//====================================
// 球を打つ処理
//====================================
void CPlayer::UpdateShot()
    {
        switch (m_shotStep)
        {
            //====================================
            // WAIT：Z を押すのを待つ
            //====================================
        case SHOT_WAIT:
            if (IsKeyTrigger('Z')) {
                m_shotPower = 0.0f;   // パワーを０にリセット 
                m_shotStep = SHOT_KEEP;

                //// カメラ位置
                //DirectX::XMFLOAT3 camPos = m_pCamera->GetPos();
                //DirectX::XMVECTOR vCamPos = DirectX::XMLoadFloat3(&camPos);

                //// プレイヤー位置
                //DirectX::XMVECTOR vPos = DirectX::XMLoadFloat3(&m_pos);

                //// カメラ → プレイヤー方向
                ////DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(vPos, vCamPos);
                //DirectX::XMVECTOR vec = DirectX::XMVectorSubtract(vPos, vCamPos);
               
                //// 正規化
                //vec = DirectX::XMVector3Normalize(vec);

                //// パワーをかけて速度にする
                //vec = DirectX::XMVectorScale(vec, m_shotPower * 1.5f);

                //// 速度に保存
                //DirectX::XMStoreFloat3(&m_move, vec);
               
                break;
            }
            //====================================
            // KEEP：押している間パワーをためる
            //====================================
        case SHOT_KEEP:
            m_shotPower += 0.02f;
            if (m_shotPower > 1.0f) m_shotPower = 0.5f;

            if (IsKeyRelease('Z'))
            {
                m_shotStep = SHOT_RELEASE;
            }
            break;

            //====================================
            // RELEASE：離した瞬間に飛ばす
            //====================================
        case SHOT_RELEASE:

           /* DirectX::XMFLOAT3 camPos = カメラの位置を取得;
            DirectX::XMVECTOR vCamPos = camPosを計算用のデータに変換;
            DirectX::XMVECTOR vPos = プレイヤーの位置を計算用のデータに変換;
            DirectX::XMVECTOR vec = カメラからプレイヤーに向かうベクトルを計算;
            vec = vecを長さ１のベクトルに変換;
            vec = 長さ１となったvecに球の打ちだし強さ(m_shotPower)をかける;
            計算用のvecをプレイヤーの移動スピード(読み取り用のm_move)に格納;*/
           

            //// カメラの位置を取得
            //DirectX::XMFLOAT3 camPos = 
            //    m_pCamera->GetPos();

            // カメラ位置を計算用データ（XMVECTOR）に変換
           /* DirectX::XMVECTOR vCamPos =
                DirectX::XMLoadFloat3(&camPos);*/

            // プレイヤーの位置を計算用データに変換
          /*  DirectX::XMVECTOR vPos = 
                DirectX::XMLoadFloat3(&m_pos);*/

                // ★ カメラ正面方向を取得する（超重要）
            DirectX::XMVECTOR vec = m_pCamera->GetForward();

            // 念のため正規化
            vec = DirectX::XMVector3Normalize(vec);

            

            // パワーを掛ける
           /* vec = DirectX::XMVectorScale(vec, m_shotPower);*/

            // 速度として保存
            DirectX::XMStoreFloat3(&m_move, vec);
            // vec を正規化（長さ 1 に）
           /* vec = DirectX::XMVector3Normalize(vec);*/

            // 正規化された vec にショット強さ（m_shotPower）を乗算
            vec = DirectX::XMVectorScale(vec, m_shotPower);

            // 計算結果をプレイヤーの移動スピード m_move（XMFLOAT3）へ書き戻す
            DirectX::XMStoreFloat3(&m_move, vec);


            m_isStop = false;
            m_isGround = false;
            m_shotStep = SHOT_WAIT;
            break;
        }
       
    } 

    void CPlayer::UpdateMove()
    {
        // 重力 
        m_move.y -= 0.02f;

        // 減速処理(空気抵抗 
        m_move.x *= 0.99f;
        m_move.y *= 0.99f;
        m_move.z *= 0.99f;

        // 移動処理 
        m_pos.x += m_move.x;
        m_pos.y += m_move.y;
        m_pos.z += m_move.z;

        // 地面接触判定 
        if (m_pos.y < 0.0f) {
            m_pos.y = 0.0f;
            Bound(BoundY);
        }

        // 停止判定 
        if (CheckStop()) {
            m_isStop = true;
            m_shotStep = SHOT_WAIT;
        }
    }

        //====================================
        // バウンド処理
        //====================================
        void CPlayer::Bound(BoundAxis axis)
        {
            DirectX::XMFLOAT3 friction = { 0.8f, 0.8f, 0.8f };

            switch (axis) {
                case BoundX:m_pos.x -= m_move.x;  break;
                case BoundY: m_pos.y -= m_move.y; break;
                case BoundZ: m_pos.z -= m_move.z; break;
            }

            // 摩擦
           
            switch (axis) {
            case BoundX: friction = DirectX::XMFLOAT3(0.95f,1.0f,1.0f);  break;
            case BoundY: friction = DirectX::XMFLOAT3(0.95f, 0.7f, 0.95f); break;
            case BoundZ: friction = DirectX::XMFLOAT3(1.0f, 1.0f, 0.95f);  break;
            }

            m_move.x *= friction.x;
            m_move.y *= friction.y;
            m_move.z *= friction.z;


            // 反転
            switch (axis) {
            case BoundX: m_move.x = -m_move.x; break;
            case BoundY: m_move.y = -m_move.y; break;
            case BoundZ: m_move.z = -m_move.z; break;
            }

            // 地面接地判定
            if (0.0f < m_move.y && m_move.y < 0.05f) {
                m_move.y = 0.0f;
                m_isGround = true;
            }
        }

        //====================================
        // 停止判定
        //====================================
        bool CPlayer::CheckStop()
        {
            float speed;
            DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_move);
            DirectX::XMVECTOR vLen = DirectX::XMVector3Length(vMove);

            DirectX::XMStoreFloat(&speed, vLen);

            return m_isGround && speed < 0.5f;
        }

        //aaa