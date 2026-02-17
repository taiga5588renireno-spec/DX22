#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "model.h"
#include "CameraDebug.h"
#include "CPlayer.h"
#include "Block.h"
#include "GaugeUI.h"
#include "Goal.h"
#include "Camera.h"
#include "CameraEvent.h"

class SceneGame : public Scene
{
    enum CameraKind {
        CAM_DEBUG, // デバッグ用のカメラ 
        CAM_EVENT, // イベント中のカメラ 
        CAM_PLAYER, //プレイヤー用のカメラを別に作成できると非常によい
        MAX_CAMERA
    };


public:
    SceneGame();
    ~SceneGame();

    void Update() final;
    void Draw() final;

    Collision::Box m_wallCollision;

private:

    Model* m_pModel = nullptr;
    Model* m_pBranchModel = nullptr;
    Model* m_pBushModel = nullptr;
    CPlayer* m_pCPlayer = nullptr;
    Block* m_pBlock = nullptr;
    GaugeUI* m_pGaugeUI = nullptr; 
    Goal* m_pGoal = nullptr;
    CameraDebug* m_pCamera = nullptr;      // 通常カメラ
    CameraEvent* m_pEventCamera = nullptr; // イベントカメラ

    bool m_isEvent = false;
};

#endif
