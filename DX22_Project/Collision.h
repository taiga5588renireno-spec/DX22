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
    };

public:
    static Result Hit(Box a, Box b);
    static Result Hit(Sphere a, Sphere b);
};
