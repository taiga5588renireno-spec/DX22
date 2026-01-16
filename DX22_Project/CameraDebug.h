#pragma once
#include "Camera.h"
#include "Input.h"
#include <DirectXMath.h>

class GameObject; 

class CameraDebug : public Camera
{
public:
    CameraDebug();
    ~CameraDebug() {}

    void SetTarget(GameObject* pTarget) { m_pTarget = pTarget; }

    void UpdateInput();
    void UpdateView();

    void Update() final { UpdateInput(); UpdateView(); }

    DirectX::XMFLOAT4X4 GetViewMatrix() const { return m_view; }
    DirectX::XMFLOAT3  GetPos()        const { return m_pos; }

private:
    float m_radXZ;   
    float m_radY;    
    float m_radius;  

    GameObject* m_pTarget = nullptr;

    DirectX::XMFLOAT4X4 m_view;
};
