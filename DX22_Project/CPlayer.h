#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <DirectXMath.h>
#include "GaneObject.h"
#include "Camera.h"
#include "Input.h"
#include "Collision.h"

class CPlayer : public GameObject
{
private:
    Camera* m_pCamera;

    DirectX::XMFLOAT3 m_move;
    bool  m_isStop;
    bool  m_isGround;

    int   m_shotStep;
    float m_shotPower;

    Collision::Box m_collision;

public:
    enum BoundAxis {
        BoundX,
        BoundY,
        BoundZ,
    };

public:
    CPlayer();
    ~CPlayer();

    void Update();
    void Draw();

    void SetCamera(Camera* pCamera);

    Collision::Box GetCollision();

public:
    void Bound(BoundAxis axis);

private:
    bool CheckStop();
    void UpdateShot();
    void UpdateMove();
};

#endif
