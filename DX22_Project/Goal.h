#pragma once
#include "GaneObject.h"
#include "Texture.h"
#include "Camera.h"

class Goal : public GameObject
{
private:
    Texture* m_pGoalTex;
    Camera* m_pCamera;

public:
    // ★ PDF通り：サイズを受け取る
    Goal(DirectX::XMFLOAT3 size);
    ~Goal();

    void Update() override;
    void Draw() override;

    void SetCamera(Camera* camera);
};
