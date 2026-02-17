#pragma once
#include <DirectXMath.h>

class Collision
{
public:
    struct Box
    {
        DirectX::XMFLOAT3 center;
        DirectX::XMFLOAT3 size;
    };

    struct Sphere
    {
        DirectX::XMFLOAT3 center;
        float radius;
    };

    struct Result
    {
        bool isHit = false;
        DirectX::XMFLOAT3 dir = { 0.0f, 0.0f, 0.0f };
        DirectX::XMFLOAT3 point;
    };
public:
    //--- 当たり判定の形状 
    struct Plane { // 面 
        DirectX::XMFLOAT3   normal;
        DirectX::XMFLOAT3 pos;
    };

    struct Ray { // レイ 
        DirectX::XMFLOAT3 start;
        DirectX::XMFLOAT3 direction;

    };

public:
    static Result Hit(Box a, Box b);
    static Result Hit(Sphere a, Sphere b);
    static Result Hit(Plane plane, Ray ray);



};
