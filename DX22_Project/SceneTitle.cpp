#include "SceneTitle.h"
#include "Texture.h"
#include "Sprite.h"
#include "Input.h"
#include "Defines.h"
#include <windows.h>


SceneTitle::SceneTitle()
    : m_pLogo(nullptr)
{
    m_pLogo = new Texture();

    if (FAILED(m_pLogo->Create("Assets/Texture/Tittle.png"))) {
        MessageBox(nullptr, "Load failed SceneTitle.", "Error", MB_OK);
    }
}

SceneTitle::~SceneTitle()
{
    delete m_pLogo;
    m_pLogo = nullptr;
}

void SceneTitle::Update()
{
    // Enterキーで次のシーンへ
    if (IsKeyTrigger(VK_RETURN)) {
        SetNext(1);   // 1 = GAME（あとで定数化）
    }
}
void SceneTitle::Draw()
{
    if (!m_pLogo) return;

    Sprite::SetTexture(m_pLogo);

    Sprite::SetSize({
        (float)SCREEN_WIDTH,
        (float)SCREEN_HEIGHT
        });

    Sprite::SetOffset({ 0.0f, 0.0f });
    Sprite::SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

    Sprite::Draw();
}