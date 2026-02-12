#include "CameraPlayer.h"
#include <cmath>

using namespace DirectX;

CameraPlayer::CameraPlayer()
    : m_radXZ(0.0f)
    , m_radY(0.3f)
    , m_lateXZ(0.0f)
    , m_lateY(0.3f)
    , m_distance(10.0f)
    , m_target(0, 0, 0)
{
}

CameraPlayer::~CameraPlayer()
{
}

void CameraPlayer::SetTarget(const XMFLOAT3& target)
{
    m_target = target;
}

void CameraPlayer::Update()
{
    // ===== キー入力などで m_radXZ / m_radY を変更する想定 =====

    float lateRate = 0.1f;   // 減速割合（0～1）

    // 補間式
    m_lateXZ = (m_radXZ - m_lateXZ) * lateRate + m_lateXZ;
    m_lateY = (m_radY - m_lateY) * lateRate + m_lateY;

    // 球面座標で位置計算
    m_pos.x = m_target.x + cosf(m_lateY) * sinf(m_lateXZ) * m_distance;
    m_pos.y = m_target.y + sinf(m_lateY) * m_distance;
    m_pos.z = m_target.z + cosf(m_lateY) * cosf(m_lateXZ) * m_distance;

    m_look = m_target;
}
