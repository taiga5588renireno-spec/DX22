#pragma once
class Fade
{
private:
    bool  m_isFadeIn;   // フェードインか
    float m_time;       // 残り時間
    float m_maxTime;    // 最大時間

public:
    Fade();
    virtual ~Fade();

    void Update();
    void Draw();

    void Start(float time, bool isIn);

    bool IsFinish();
    bool IsFadeIn();
    bool IsFadeOut();

    float GetRate();
    float GetAlpha();

protected:
    virtual void DrawFadeIn(float alpha) = 0;
    virtual void DrawFadeOut(float alpha) = 0;
};


