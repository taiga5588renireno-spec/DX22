#include "Collision.h"
#include <cmath>

Collision::Result Collision::Hit(Box a, Box b)
{
    Result out{};

    const float aHx = a.size.x * 0.5f;
    const float aHy = a.size.y * 0.5f;
    const float aHz = a.size.z * 0.5f;

    const float bHx = b.size.x * 0.5f;
    const float bHy = b.size.y * 0.5f;
    const float bHz = b.size.z * 0.5f;

    const float dx = a.center.x - b.center.x;
    const float px = (aHx + bHx) - std::fabs(dx);
    if (px <= 0.0f) return out;

    const float dy = a.center.y - b.center.y;
    const float py = (aHy + bHy) - std::fabs(dy);
    if (py <= 0.0f) return out;

    const float dz = a.center.z - b.center.z;
    const float pz = (aHz + bHz) - std::fabs(dz);
    if (pz <= 0.0f) return out;

    out.isHit = true;

    if (px <= py && px <= pz)
        out.dir = { (dx < 0.0f ? -1.0f : 1.0f), 0.0f, 0.0f };
    else if (py <= px && py <= pz)
        out.dir = { 0.0f, (dy < 0.0f ? -1.0f : 1.0f), 0.0f };
    else
        out.dir = { 0.0f, 0.0f, (dz < 0.0f ? -1.0f : 1.0f) };

    return out;
}

Collision::Result Collision::Hit(Sphere a, Sphere b)
{
    Result out{};

    const float dx = a.center.x - b.center.x;
    const float dy = a.center.y - b.center.y;
    const float dz = a.center.z - b.center.z;

    const float distSq = dx * dx + dy * dy + dz * dz;
    const float r = a.radius + b.radius;

    if (distSq > r * r) return out;

    out.isHit = true;

    const float dist = std::sqrt(distSq);
    if (dist > 0.00001f)
        out.dir = { dx / dist, dy / dist, dz / dist };
    else
        out.dir = { 1.0f, 0.0f, 0.0f };

    return out;
}
Collision::Result Collision::Hit(Plane plane, Ray ray)
{
    Result result = {};
    result.isHit = false;

    float rayLength;
    DirectX::XMVECTOR vRayStart =
        DirectX::XMLoadFloat3(&ray.start);
    DirectX::XMVECTOR vRayVec=
        DirectX::XMLoadFloat3(&ray.direction);
    DirectX::XMVECTOR vRayEnd =
        DirectX::XMVectorAdd(vRayStart, vRayVec);
    DirectX::XMStoreFloat(&rayLength,
        DirectX::XMVector3Length(vRayVec));

    DirectX::XMVECTOR vPlaneN =
        DirectX::XMLoadFloat3(&plane.normal);
    DirectX::XMVECTOR vPlanePos =
        DirectX::XMLoadFloat3(&plane.pos);
    vPlaneN = DirectX::XMVector3Normalize(vPlaneN);

    float P1;
    DirectX::XMVECTOR vToStart =
        DirectX::XMVectorSubtract(vRayStart, vPlanePos);
    DirectX::XMStoreFloat(&P1,
        DirectX::XMVector3Dot(vPlaneN, vToStart));

    float P2;
    DirectX::XMVECTOR vToEnd =
        DirectX::XMVectorSubtract(vRayEnd, vPlanePos);
    DirectX::XMStoreFloat(&P2,
        DirectX::XMVector3Dot(vPlaneN, vToEnd));

    if (P1 > 0 && P2 < 0)
    {
        result.isHit = true;
        result.dir = plane.normal;
        float rate = fabsf(P1) / (fabsf(P1) + fabsf(P2));
        DirectX::XMStoreFloat3(&result.point,
            DirectX::XMVectorAdd(
                vRayStart,
                DirectX::XMVectorScale(vRayVec, rate)));
    }

    return result;
}