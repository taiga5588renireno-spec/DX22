#include "Fade.h"
Fade::Fade()
    : m_isFadeIn(false)
    , m_time(0.0f)
    , m_maxTime(0.0f)
{
}

Fade::~Fade()
{
}

void Fade::Update()
{
    if (IsFinish()) return;

    m_time -= 1.0f / 60.0f;
    if (m_time < 0.0f)
        m_time = 0.0f;
}

void Fade::Draw()
{
    if (IsFadeIn())
        DrawFadeIn(GetAlpha());
    else
        DrawFadeOut(GetAlpha());
}

void Fade::Start(float time, bool isIn)
{
    m_isFadeIn = isIn;
    m_time = time;
    m_maxTime = time;
}

bool Fade::IsFinish()
{
    return m_time <= 0.0f;
}

bool Fade::IsFadeIn()
{
    return m_isFadeIn;
}

bool Fade::IsFadeOut()
{
    return !m_isFadeIn;
}

float Fade::GetRate()
{
    if (m_maxTime == 0.0f)
        return 0.0f;

    return m_time / m_maxTime;
}

float Fade::GetAlpha()
{
    float alpha = GetRate();

    if (IsFadeOut())
        alpha = 1.0f - alpha;

    return alpha;
}