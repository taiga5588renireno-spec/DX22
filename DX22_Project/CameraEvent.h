#pragma once
#include "Camera.h"
#include <DirectXMath.h>

class CameraEvent : public Camera
{

private:
    DirectX::XMFLOAT3 m_start;     // 開始位置
    DirectX::XMFLOAT3 m_end;       // 終了位置

    float m_time;                  // 現在時間
    float m_totalTime;             // 総時間

public:
    CameraEvent();
    ~CameraEvent() override;

    void Update() override;

    void SetEvent(DirectX::XMFLOAT3 startPos,
        DirectX::XMFLOAT3 endPos,
        float time);

    bool IsEvent();
};
