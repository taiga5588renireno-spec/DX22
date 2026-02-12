#include "CameraEvent.h"
#include <algorithm>

using namespace DirectX;

CameraEvent::CameraEvent()
    : m_start(0, 0, 0)
    , m_end(0, 0, 0)
    , m_time(0.0f)
    , m_totalTime(0.0f)
{
}

CameraEvent::~CameraEvent()
{
}

void CameraEvent::SetEvent(XMFLOAT3 startPos,
    XMFLOAT3 endPos,
    float time)
{
    m_start = startPos;
    m_end = endPos;
    m_time = 0.0f;
    m_totalTime = time;
}

bool CameraEvent::IsEvent()
{
    return m_time <=m_totalTime;
}

void CameraEvent::Update()
{
    if (!IsEvent()) return;

    // 媒介変数
    float rate = m_time / m_totalTime;

    // clamp
    if (rate < 0.0f) rate = 0.0f;
    if (rate > 1.0f) rate = 1.0f;

    // ===== 線形補間 =====
    XMFLOAT3 lerp;
    lerp.x = m_start.x * (1.0f - rate) + m_end.x * rate;
    lerp.y = m_start.y * (1.0f - rate) + m_end.y * rate;
    lerp.z = m_start.z * (1.0f - rate) + m_end.z * rate;

    // 注視点を更新
    m_look = lerp;

    // カメラ位置（少し後ろ）
    m_pos.x = m_look.x;
    m_pos.y = m_look.y;
    m_pos.z = m_look.z - 10.0f;

    m_time += 1.0f / 60.0f;

}

