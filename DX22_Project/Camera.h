#pragma once
#include <DirectXMath.h>

class Camera
{
public:
    Camera()
        : m_pos(0.0f, 5.0f, -10.0f),
        m_look(0.0f, 0.0f, 0.0f),
        m_up(0.0f, 1.0f, 0.0f),
        m_fovy(DirectX::XMConvertToRadians(60.0f)),
        m_aspect(16.0f / 9.0f),
        m_near(0.3f),
        m_far(1000.0f)
    {}

    virtual ~Camera() {}

    virtual void Update() = 0;

    DirectX::XMFLOAT4X4 GetViewMatrix(bool transpose = true)
    {
        using namespace DirectX;
        XMFLOAT4X4 mat;
      
        XMMATRIX view = XMMatrixLookAtLH(
            XMLoadFloat3(&m_pos),
            XMLoadFloat3(&m_look),
            XMLoadFloat3(&m_up)
        );

        if (transpose)
            view = XMMatrixTranspose(view);

        XMStoreFloat4x4(&mat, view);
        return mat;
    }

    DirectX::XMFLOAT4X4 GetProjectionMatrix(bool transpose = true)
    {
        using namespace DirectX;
        XMMATRIX proj = XMMatrixPerspectiveFovLH(m_fovy, m_aspect, m_near, m_far);

        if (transpose)
            proj = XMMatrixTranspose(proj);

        XMFLOAT4X4 mat;
        XMStoreFloat4x4(&mat, proj);
        return mat;
    }
    DirectX::XMVECTOR GetForward() const
    {
        using namespace DirectX;

        XMVECTOR pos = XMLoadFloat3(&m_pos);
        XMVECTOR look = XMLoadFloat3(&m_look);

        XMVECTOR forward = XMVectorSubtract(look, pos);
        return XMVector3Normalize(forward);
    }
    DirectX::XMFLOAT3 GetPos() const { return m_pos; }
    DirectX::XMFLOAT3 GetLook() const { return m_look; }

   
protected:
    DirectX::XMFLOAT3 m_pos;   // カメラ位置
    DirectX::XMFLOAT3 m_look;  // 注視点
    DirectX::XMFLOAT3 m_up;    // 上方向
    float m_fovy;              // 画角
    float m_aspect;            // アスペクト比
    float m_near;              // ニアクリップ
    float m_far;               // ファークリップ
};


