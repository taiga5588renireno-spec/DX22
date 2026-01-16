#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "Camera.h"
#include "model.h"
#include "Camera.h"
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
	Model* m_pModel;

	Model* m_pBranchModel;

	Model* m_pBushModel;

	Camera* m_pCamera;

	CPlayer* m_pCPlayer;

	Block* m_pBlock;   // Å© ïœêî


	GaugeUI* m_pGaugeUI;
};

#endif // __SCENE_GAME_H__

