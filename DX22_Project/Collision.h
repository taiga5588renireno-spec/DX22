
#pragma once
#include <DirectXMath.h>

class Collision {
public:

    //--- 立方体
    struct Box {
        DirectX::XMFLOAT3 center; // 中心座標
        DirectX::XMFLOAT3 size;   // サイズ
    };

    //--- 球
    struct Sphere {
        DirectX::XMFLOAT3 center; // 中心座標
        float radius;             // 半径
    };

    //--- 結果
    struct Result {
        bool isHit;
        DirectX::XMFLOAT3 dir; // 跳ね返り方向
    };

public:
    static Result Hit(Box a, Box b);
    static Result Hit(Sphere a, Sphere b);
};
