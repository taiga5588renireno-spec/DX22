#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <DirectXMath.h>
#include "GaneObject.h"
#include "Camera.h"
#include "Input.h"
#include "Collision.h"
#include "PolylineEffect.h"

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
    PolylineEffect* m_pTrail;
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
private:
    Texture* m_pShadowTex;                 // 影テクスチャ
    DirectX::XMFLOAT3 m_shadowPos;          // 影位置
    Collision::Box m_shadowCollision;       // 影当たり判定

public:
    void SetShadowPos(DirectX::XMFLOAT3 pos)
    {
        m_shadowPos = pos;
    }

    Collision::Box GetShadowCollision()
    {
        return m_shadowCollision;
    }
};

#endif
