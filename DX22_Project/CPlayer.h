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
    //--- 各種メンバー変数 
    Camera* m_pCamera;         // ボールを追いかけるカメラの情報 
    DirectX::XMFLOAT3 m_move;  // ボールの移動速度 
    bool     m_isStop;         // ボールの停止判定 
    bool     m_isGround;       // 地面接地判定 
    int     m_shotStep;        //ボールの処理手順 
    float    m_shotPower;      //ボールの打ち出し強さ 
    Collision::Box m_collision;

public:
    // バウンド方向の定義（教材と同じ）
    enum BoundAxis {
        BoundX,
        BoundY,
        BoundZ,
    };

public:
    //基本の処理
    CPlayer();
       
    ~CPlayer();

    //更新処理
    void Update();
    //描画
    void Draw();
    
    //カメラ設定
    void SetCamera(Camera* pCamera);//{ m_pCamera = pCamera; }


    Collision::Box GetCollision();

public:
    void Bound(BoundAxis axis);
   
private:
    bool CheckStop();
    void UpdateShot();
    void UpdateMove();
   
    

};

#endif