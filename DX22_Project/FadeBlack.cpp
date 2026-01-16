//#include "FadeBlack.h"
//#include "Texture.h"
//#include "Sprite.h"
//#include <windows.h>
//
//FadeBlack::FadeBlack()
//    : m_pTexture(nullptr)
//{
//    m_pTexture = new Texture();
//
//    // 1x1の黒画像（white.png を黒で塗ったものでもOK）
//    if (FAILED(m_pTexture->Create("Asset/black.png"))) {
//        MessageBox(nullptr, "Load failed FadeBlack.", "Error", MB_OK);
//    }
//}
//
//FadeBlack::~FadeBlack()
//{
//    delete m_pTexture;
//    m_pTexture = nullptr;
//}
//
//void FadeBlack::DrawFadeIn(float alpha)
//{
//    DrawFade(alpha);
//}
//
//void FadeBlack::DrawFadeOut(float alpha)
//{
//    DrawFade(alpha);
//}
//
//void FadeBlack::DrawFade(float alpha)
//{
//    Sprite::SetTexture(m_pTexture->GetResource());
//
//    Sprite::SetSize({
//        (float)Application::GetWidth(),
//        (float)Application::GetHeight()
//        });
//
//    Sprite::SetOffset({ 0.0f, 0.0f });
//
//    Sprite::SetColor({ 0.0f, 0.0f, 0.0f, alpha });
//
//    Sprite::Draw();
//}
#include "FadeBlack.h"
#include "Texture.h"
#include "Sprite.h"
#include "Defines.h"
#include <windows.h>

FadeBlack::FadeBlack()
    : m_pTexture(nullptr)
{
    m_pTexture = new Texture();

    if (FAILED(m_pTexture->Create("Assets/Texture/Fade.png"))) {
        MessageBox(nullptr, "Load failed FadeBlack.", "Error", MB_OK);
    }
}

FadeBlack::~FadeBlack()
{
    delete m_pTexture;
    m_pTexture = nullptr;
}

void FadeBlack::DrawFadeIn(float alpha)
{
    DrawFade(alpha);
}

void FadeBlack::DrawFadeOut(float alpha)
{
    DrawFade(alpha);
}

void FadeBlack::DrawFade(float alpha)
{
    // テクスチャ設定（←重要：GetResource）
    Sprite::SetTexture(m_pTexture);

    // 画面全体サイズ
    Sprite::SetSize({
        (float)SCREEN_WIDTH,
        (float)SCREEN_HEIGHT
        });

    Sprite::SetOffset({ 0.0f, 0.0f });

    // 黒フェード
    Sprite::SetColor({ 0.0f, 0.0f, 0.0f, alpha });

    Sprite::Draw();
}
