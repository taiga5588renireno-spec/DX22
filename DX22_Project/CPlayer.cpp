#include "CPlayer.h"
#include "Geometory.h"
#include <DirectXMath.h>

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
{
    m_collision.size = { 0.2f, 0.2f, 0.2f };
    m_collision.center = m_pos;
}

CPlayer::~CPlayer()
{
}

void CPlayer::SetCamera(Camera* pCamera)
{
    m_pCamera = pCamera;
}

void CPlayer::Update()
{
    if (!m_pCamera) return;

    if (m_isStop)
        UpdateShot();
    else
        UpdateMove();

    m_collision.center = m_pos;
}

void CPlayer::Draw()
{
    XMMATRIX S = XMMatrixScaling(1.0f, 1.0f, 1.0f);
    XMMATRIX R = XMMatrixRotationY(XMConvertToRadians(90.0f));
    XMMATRIX T = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);

    XMMATRIX mat = S * R * T;
    mat = XMMatrixTranspose(mat);

    XMFLOAT4X4 fMat;
    XMStoreFloat4x4(&fMat, mat);

    Geometory::SetWorld(fMat);
    Geometory::DrawBox();
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

bool CPlayer::CheckStop()
{
    float speed = 0.0f;

    XMVECTOR vMove = XMLoadFloat3(&m_move);
    XMVECTOR vLen = XMVector3Length(vMove);
    XMStoreFloat(&speed, vLen);

    return m_isGround && speed < 0.5f;
}
