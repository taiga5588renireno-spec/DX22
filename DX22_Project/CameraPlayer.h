#pragma once
#include "Camera.h"
#include <DirectXMath.h>

class CameraPlayer : public Camera
{
private:
    float m_radXZ;     // 目的角度XZ
    float m_radY;      // 目的角度Y

    float m_lateXZ;    // 現在角度XZ
    float m_lateY;     // 現在角度Y

    float m_distance;  // プレイヤーとの距離

    DirectX::XMFLOAT3 m_target; // プレイヤー座標

public:
    CameraPlayer();
    ~CameraPlayer() override;

    void Update() override;

    void SetTarget(const DirectX::XMFLOAT3& target);
};

