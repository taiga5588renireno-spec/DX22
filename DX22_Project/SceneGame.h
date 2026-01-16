#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "model.h"
#include "CameraDebug.h"
#include "CPlayer.h"
#include "Block.h"
#include "GaugeUI.h"

class SceneGame : public Scene
{
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

    CameraDebug* m_pCamera = nullptr;
    CPlayer* m_pCPlayer = nullptr;

    Block* m_pBlock = nullptr;
    GaugeUI* m_pGaugeUI = nullptr;
};

#endif
