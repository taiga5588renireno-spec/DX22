#pragma once
#include "Texture.h"

class GaugeUI
{
private:
    Texture* m_pFrame;
    Texture* m_pGauge;
    float    m_rate; // 0.0f Å` 1.0f

public:
    GaugeUI();
    ~GaugeUI();

    void Draw();
    void SetRate(float rate);
};


