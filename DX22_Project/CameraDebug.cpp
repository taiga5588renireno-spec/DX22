#include "CameraDebug.h"
#include "GaneObject.h"
#include <cmath>

CameraDebug::CameraDebug()
    : m_radXZ(0.0f)
    , m_radY(0.35f)
    , m_radius(12.0f)
{
    m_look = { 0.0f, 0.2f, 0.0f };

    DirectX::XMStoreFloat4x4(&m_view, DirectX::XMMatrixIdentity());
}

void CameraDebug::UpdateInput()
{
    const float rotSpeed = 0.02f;
    const float zoomSpeed = 0.5f;

    const float mul = IsKeyPress(VK_SHIFT) ? 2.5f : 1.0f;

    if (IsKeyPress('A') || IsKeyPress(VK_LEFT))  m_radXZ -= rotSpeed * mul;
    if (IsKeyPress('D') || IsKeyPress(VK_RIGHT)) m_radXZ += rotSpeed * mul;

    if (IsKeyPress('W') || IsKeyPress(VK_UP))    m_radY += rotSpeed * mul;
    if (IsKeyPress('S') || IsKeyPress(VK_DOWN))  m_radY -= rotSpeed * mul;

    if (IsKeyPress('Q')) m_radius -= zoomSpeed * mul;
    if (IsKeyPress('E')) m_radius += zoomSpeed * mul;

    if (m_radY < -1.2f) m_radY = -1.2f;
    if (m_radY > 1.2f) m_radY = 1.2f;

    if (m_radius < 3.0f)  m_radius = 3.0f;
    if (m_radius > 50.0f) m_radius = 50.0f;
}

void CameraDebug::UpdateView()
{
    using namespace DirectX;


    if (m_pTarget)
    {
        m_look = m_pTarget->GetPos();
        m_look.y += 0.2f; 
    }

    m_pos.x = cosf(m_radY) * sinf(m_radXZ) * m_radius + m_look.x;
    m_pos.y = sinf(m_radY) * m_radius + m_look.y;
    m_pos.z = cosf(m_radY) * cosf(m_radXZ) * m_radius + m_look.z;

    XMMATRIX view =
        XMMatrixLookAtLH(
            XMVectorSet(m_pos.x, m_pos.y, m_pos.z, 1.0f),
            XMVectorSet(m_look.x, m_look.y, m_look.z, 1.0f),
            XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        );

    XMStoreFloat4x4(&m_view, XMMatrixTranspose(view));
}
