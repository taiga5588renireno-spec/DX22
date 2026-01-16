#include "CameraDebug.h"

#include <cmath>

CameraDebug::CameraDebug()
    : m_radXZ(0.0f), m_radY(0.3f), m_radius(15.0f)
{
    m_look = { 0.0f, 0.2f, 0.0f };
    
}

void CameraDebug::Update()
{
    const float rotSpeed = 0.02f;
    const float moveSpeed = 0.5f;

  
    float speed = moveSpeed;

    using namespace DirectX;

    XMVECTOR forward = GetForward();
    XMVECTOR up = XMVectorSet(0, 1, 0, 0);
    XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, forward));

    XMVECTOR look = XMLoadFloat3(&m_look);

    float dashMul = IsKeyPress(VK_SHIFT) ? 4.0f : 1.0f;
    speed = moveSpeed * dashMul;


    if (IsKeyPress('W')) look += forward * speed;
    if (IsKeyPress('S')) look -= forward * speed;
    if (IsKeyPress('A')) look -= right * speed;
    if (IsKeyPress('D')) look += right * speed;



    XMStoreFloat3(&m_look, look);


    // 範囲制限
    if (m_radius < 3.0f) m_radius = 3.0f;
    if (m_radius > 50.0f) m_radius = 50.0f;


    // sin, cosでカメラ位置を計算（注視点中心の回転）
    m_pos.x = cosf(m_radY) * sinf(m_radXZ) * m_radius + m_look.x;
    m_pos.y = sinf(m_radY) * m_radius + m_look.y;
    m_pos.z = cosf(m_radY) * cosf(m_radXZ) * m_radius + m_look.z;


    DirectX::XMMATRIX view =
        DirectX::XMMatrixLookAtLH(
            DirectX::XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 1.0f),
            DirectX::XMVectorSet(m_look.x, m_look.y, m_look.z, 1.0f),
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        );

    DirectX::XMStoreFloat4x4(&m_view, view);

   
   

 
 
}

DirectX::XMVECTOR CameraDebug::GetForward() const
{
    using namespace DirectX;

    XMVECTOR look = XMLoadFloat3(&m_look);
    XMVECTOR pos = XMLoadFloat3(&m_pos);

    // ★ 向きを反転
    XMVECTOR forward = XMVectorSubtract(look, pos);

    return XMVector3Normalize(forward);
}