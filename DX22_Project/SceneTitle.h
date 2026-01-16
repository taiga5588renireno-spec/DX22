#pragma once
#include "Scene.h"

class Texture;

class SceneTitle : public Scene {
private:
    Texture* m_pLogo;   // © ‚±‚ê‚ª•K—v

public:
    SceneTitle();
    ~SceneTitle() override;

protected:
    void Update() override;
    void Draw() override;
};
