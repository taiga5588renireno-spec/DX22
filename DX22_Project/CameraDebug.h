#pragma once
#include "Camera.h"
#include "Input.h"  // ←自分のプロジェクトにあるキー入力ヘッダ

class CameraDebug : public Camera
{
public:
    CameraDebug();
   
    ~CameraDebug() {};

    void Update() final;

    DirectX::XMFLOAT4X4 GetViewMatrix() const { return m_view; }
    DirectX::XMFLOAT3  GetPos()        const { return m_pos; }

private:
    float m_radXZ;   // 横回転角
    float m_radY;    // 縦回転角
    float m_radius;  // カメラと注視点の距離


private:
    DirectX::XMFLOAT4X4 m_view;
    DirectX::XMVECTOR GetForward() const;

};


