#pragma once
#include "Fade.h"

class Texture;

class FadeBlack : public Fade {
private:
    Texture* m_pTexture;

public:
    FadeBlack();
    ~FadeBlack() override;

protected:
    void DrawFadeIn(float alpha) override;
    void DrawFadeOut(float alpha) override;

private:
    void DrawFade(float alpha);
};
