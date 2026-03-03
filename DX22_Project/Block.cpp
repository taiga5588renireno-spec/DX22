#include "Block.h"
#include "Collision.h"
#include "Geometory.h"
#include <DirectXMath.h>

using namespace DirectX;

Block::Block(XMFLOAT3 size)
    : m_collision()
    , m_plane()
    , m_rotate()
{   

    // 当たり判定サイズ設定
    m_collision.size = size;
}

Block::~Block()
{
}

void Block::Update()
{
    // GameObject が持つ位置を当たり判定の中心に設定
    m_collision.center = m_pos;
}

void Block::Draw()
{
#if _DEBUG
    // 変換行列作成
    XMMATRIX T = XMMatrixTranslation(m_pos.x, m_pos.y, m_pos.z);
    XMMATRIX mRot[] = {
    XMMatrixRotationX(
            XMConvertToRadians(m_rotate.x)),
        XMMatrixRotationY(
            XMConvertToRadians(m_rotate.y)),
        XMMatrixRotationZ(
            XMConvertToRadians(m_rotate.z)),
    };
    XMMATRIX R = mRot[0] * mRot[1] * mRot[2];
    XMMATRIX S = XMMatrixScaling(
        m_collision.size.x,
        m_collision.size.y,
        m_collision.size.z
    );


    XMMATRIX mat;

   // XMMATRIX mat = S * T;
    mat = XMMatrixTranspose(S*R*T);

    XMFLOAT4X4 fMat;
    XMStoreFloat4x4(&fMat, mat);

    // 当たり判定ボックス描画
    Geometory::SetWorld(fMat);
    Geometory::DrawBox();
#endif
}

void Block::SetRotate(float x, float y, float z)
{
    m_rotate = { x,y,z };
}

Collision::Box Block::GetCollision()
{
    return m_collision;
}

Collision::Plane Block::GetPlane()
{
    XMMATRIX mRot[] =
    {
        XMMatrixRotationX(
            XMConvertToRadians(m_rotate.x)),
            XMMatrixRotationY(
            XMConvertToRadians(m_rotate.y)),
            XMMatrixRotationZ(
            XMConvertToRadians(m_rotate.z)),
    };
    DirectX::XMMATRIX mMat =
        mRot[0] * mRot[1] * mRot[2];

    m_plane.normal = { 0.0f, 1.0f, 0.0f };
    m_plane.pos =
    {
        0.0f,m_collision.size.y * 0.5f,0.0f
    };

    XMVECTOR vPlaneN =
        XMLoadFloat3(&m_plane.normal);
    XMVECTOR vPlanePos =
        XMLoadFloat3(&m_plane.pos);

    vPlaneN = XMVector3TransformCoord(
        vPlaneN, mMat);
    vPlaneN = XMVector3TransformCoord(
        vPlanePos, mMat);

    XMStoreFloat3(&m_plane.normal, vPlaneN);
    XMStoreFloat3(&m_plane.pos, vPlanePos);

    m_plane.pos.x += m_pos.x;
    m_plane.pos.y += m_pos.y;
    m_plane.pos.z += m_pos.z;
        
    return m_plane;
}