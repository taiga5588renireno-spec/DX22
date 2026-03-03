#include "CPlayer.h"
#include "Geometory.h"
#include <DirectXMath.h>
#include "Input.h"
#include "TrailEffect.h"
#include "Texture.h"
#include "Sprite.h"
using namespace DirectX;


enum eShotStep
{
    SHOT_WAIT,
    SHOT_KEEP,
    SHOT_RELEASE,
};

CPlayer::CPlayer()
    : m_pCamera(nullptr)
    , m_move(0.0f, 0.0f, 0.0f)
    , m_isStop(true)
    , m_isGround(true)
    , m_shotStep(SHOT_WAIT)
    , m_shotPower(0.0f)
    , m_collision()
    , m_pShadowTex(nullptr),m_shadowPos(),m_shadowCollision()
{
    m_collision.size = { 0.2f, 0.2f, 0.2f };
    m_collision.center = m_pos;

    m_shadowCollision.size = { 0.5f, 0.1f, 0.5f };
    m_pShadowTex = new Texture();
    if (FAILED(m_pShadowTex->Create("Assets/Texture/shadow.png")))
    {
        MessageBox(NULL, "Texture load failed.", "Error", MB_OK);
    }
    m_pTrail = new TrailEffect(this);
    m_pTrail->AddLine(20);

}

CPlayer::~CPlayer()
{
    if (m_pTrail)
    {
        delete m_pTrail;
        m_pTrail = nullptr;
    }

    if (m_pShadowTex)
    {
        delete m_pShadowTex;
        m_pShadowTex = nullptr;
    }
}

void CPlayer::SetCamera(Camera* pCamera)
{
    m_pCamera = pCamera;
}

void CPlayer::Update()
{
    if (!m_pCamera) { return; }

    if (m_isStop)
        UpdateShot();
    else
        UpdateMove();

    m_collision.center = m_pos;
    m_shadowCollision.center = m_pos;

    m_pTrail->Update();

}

void CPlayer::Draw()
{
   /* XMMATRIX S = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    XMMATRIX R = XMMatrixRotationY(XMConvertToRadians(90.0f));
    XMMATRIX T = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);*/
    XMMATRIX T =
        XMMatrixTranslation(
            m_shadowPos.x,
            m_shadowPos.y,
            m_shadowPos.z);
    XMMATRIX mat ;
    mat = XMMatrixTranspose(T);

    XMFLOAT4X4 fMat;
    XMStoreFloat4x4(&fMat, mat);

    Geometory::SetWorld(fMat);
    Geometory::DrawBox();

    float rate = (m_pos.y - m_shadowPos.y) / 4.0f;
   float scale = (1.0f - rate);

   XMMATRIX S = XMMatrixScaling(scale, -scale, 1.0f);
   XMMATRIX R = XMMatrixRotationX(DirectX::XMConvertToRadians(90.0f));
            T =
       XMMatrixTranslation(
           m_shadowPos.x,
           m_shadowPos.y + 0.01f,
           m_shadowPos.z);

   XMMATRIX mWorld = S * R * T;

   XMStoreFloat4x4(&fMat, XMMatrixTranspose(mWorld));

   Sprite::SetWorld(fMat);
   Sprite::SetSize({ 2.3f, 2.3f });
   Sprite::SetColor({ 0.0f, 0.0f, 0.0f, scale * 0.8f });
   Sprite::SetTexture(m_pShadowTex);
   Sprite::Draw();


    m_pTrail->SetView(m_pCamera->GetViewMatrix());
    m_pTrail->SetProjection(m_pCamera->GetProjectionMatrix());
    m_pTrail->Draw();

}

Collision::Box CPlayer::GetCollision()
{
    return m_collision;
}

void CPlayer::UpdateShot()
{
    switch (m_shotStep)
    {
    case SHOT_WAIT:
        if (IsKeyTrigger('Z'))
        {
            m_shotPower = 0.0f;
            m_shotStep = SHOT_KEEP;
        }
        break;

    case SHOT_KEEP:
        m_shotPower += 0.02f;
        if (m_shotPower > 1.0f) m_shotPower = 1.0f;

        if (IsKeyRelease('Z'))
        {
            m_shotStep = SHOT_RELEASE;
        }
        break;

    case SHOT_RELEASE:
    {
        XMVECTOR dir = m_pCamera->GetForward();

        XMFLOAT3 f;
        XMStoreFloat3(&f, dir);
        f.y = 0.0f;

        XMVECTOR flat = XMLoadFloat3(&f);
        flat = XMVector3Normalize(flat);

        const float baseSpeed = 2.0f;
        flat = XMVectorScale(flat, m_shotPower * baseSpeed);

        XMStoreFloat3(&m_move, flat);

        m_isStop = false;
        m_isGround = false;
        m_shotStep = SHOT_WAIT;
    }
    break;
    }
}

void CPlayer::UpdateMove()
{
    m_move.y -= 0.02f;

    m_move.x *= 0.99f;
    m_move.y *= 0.99f;
    m_move.z *= 0.99f;

    m_pos.x += m_move.x;
    m_pos.y += m_move.y;
    m_pos.z += m_move.z;

    if (m_pos.y < 0.0f)
    {
        m_pos.y = 0.0f;
        Bound(BoundY);
    }

    if (CheckStop())
    {
        m_isStop = true;
        m_shotStep = SHOT_WAIT;
    }
}

void CPlayer::Bound(BoundAxis axis)
{
    switch (axis)
    {
    case BoundX: m_pos.x -= m_move.x; break;
    case BoundY: m_pos.y -= m_move.y; break;
    case BoundZ: m_pos.z -= m_move.z; break;
    }

    const float bounce = 0.5f;

    switch (axis)
    {
    case BoundX: m_move.x = -m_move.x * bounce; break;
    case BoundY: m_move.y = -m_move.y * bounce; break;
    case BoundZ: m_move.z = -m_move.z * bounce; break;
    }

    if (axis == BoundY && m_move.y > 0.0f && m_move.y < 0.05f)
    {
        m_move.y = 0.0f;
        m_isGround = true;
    }

    m_collision.center = m_pos;
}

void CPlayer::Bound(DirectX::XMFLOAT3 dir)
{
    m_pos.x -= m_move.x;
    m_pos.y -= m_move.y;
    m_pos.z -= m_move.z;

    DirectX::XMVECTOR vPlaneN = DirectX::XMLoadFloat3(&dir);
    DirectX::XMVECTOR vMove = DirectX::XMLoadFloat3(&m_move);
    DirectX::XMVECTOR vDot = DirectX::XMVector3Dot(vPlaneN, vMove);

    DirectX::XMVECTOR vAdd = 
        DirectX::XMVectorMultiply(vPlaneN,
        DirectX::XMVectorAbs(vDot));

    vMove = DirectX::XMVectorAdd(vMove,
        DirectX::XMVectorScale(vAdd, 2.0f));
    vMove = DirectX::XMVectorScale(vMove, 0.7f);

    DirectX::XMStoreFloat3(&m_move, vMove);

    if (0.0f < m_move.y && m_move.y < 0.05f)
    {
        m_move.y = 0.0f;
        m_isGround = true;
    }
}

bool CPlayer::CheckStop()
{
    float speed = 0.0f;

    XMVECTOR vMove = XMLoadFloat3(&m_move);
    XMVECTOR vLen = XMVector3Length(vMove);
    XMStoreFloat(&speed, vLen);

    return m_isGround && speed < 0.5f;
}
