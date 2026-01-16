//Collision.cpp
#include "Collision.h"

using namespace DirectX;

//=====================================================
// 立方体（AABB）同士の当たり判定
//=====================================================
Collision::Result Collision::Hit(Box a, Box b)
{
    Result out = {};


    // 計算用に変換
    XMVECTOR vPosA = XMLoadFloat3(&a.center);
    XMVECTOR vPosB = XMLoadFloat3(&b.center);
    XMVECTOR vSizeA = XMLoadFloat3(&a.size);
    XMVECTOR vSizeB = XMLoadFloat3(&b.size);

    // 半サイズへ
    vSizeA = XMVectorScale(vSizeA, 0.5f);
    vSizeB = XMVectorScale(vSizeB, 0.5f);

    // 最大値・最小値
    XMVECTOR vMaxA = XMVectorAdd(vPosA, vSizeA);
    XMVECTOR vMinA = XMVectorSubtract(vPosA, vSizeA);
    XMVECTOR vMaxB = XMVectorAdd(vPosB, vSizeB);
    XMVECTOR vMinB = XMVectorSubtract(vPosB, vSizeB);

    XMFLOAT3 maxA, minA, maxB, minB;
    XMStoreFloat3(&maxA, vMaxA);
    XMStoreFloat3(&minA, vMinA);
    XMStoreFloat3(&maxB, vMaxB);
    XMStoreFloat3(&minB, vMinB);

    out.isHit = false;
    if (maxA.x >= minB.x && minA.x <= maxB.x)
    {
        if (maxA.y >= minB.y && minA.y <= maxB.y)
        {
            if (maxA.z >= minB.z && minA.z <= maxB.z)
            {
                out.isHit = true;

                DirectX::XMVECTOR vDist = 
                    DirectX::XMVectorSubtract(vPosA, vPosB);
                vDist = DirectX::XMVectorAbs(vDist);

                DirectX::XMVECTOR vSumsize =
                    DirectX::XMVectorAdd(vSizeA,vSizeB);

                DirectX::XMVECTOR vOverIap =
                    DirectX::XMVectorSubtract(vSumsize, vDist);

                DirectX::XMFLOAT3 overIap;
                DirectX::XMStoreFloat3(&overIap,vOverIap);


                if (overIap.x < overIap.y) {
                    if (overIap.x < overIap.z)
                    {
                        out.dir = { a.center.x < b.center.x ? -1.0f : 1.0f,0.0f,0.0f };
                    }
                    else
                        out.dir = { 0.0f,0.0f,a.center.z < b.center.z ? -1.0f : 1.0f };
                }
                else {
                    if (overIap.y < overIap.z)
                        out.dir = { 0.0f,a.center.y < b.center.y ? -1.0f : 1.0f,0.0f };
                    else
                        out.dir = { 0.0f,0.0f,a.center.z < b.center.z ? -1.0f : 1.0f };
                }
                

            }
        }
    }

    //out.dir = { 0.0f, 0.0f, 0.0f };
    return out;
}



//=====================================================
// 球同士の当たり判定
//=====================================================
Collision::Result Collision::Hit(Sphere a, Sphere b)
{
    Result out{};
    
    // 中心座標
    XMVECTOR vPosA = XMLoadFloat3(&a.center);
    XMVECTOR vPosB = XMLoadFloat3(&b.center);

    // 距離ベクトル
    XMVECTOR vDist = XMVectorSubtract(vPosA, vPosB);

    // 長さ
    XMVECTOR vLen = XMVector3Length(vDist);
    float length;
    XMStoreFloat(&length, vLen);

    out.isHit = (a.radius + b.radius >= length);

   

    return out;
}
