#include "Block.h"
#include "Collision.h"
#include "Geometory.h"
#include <DirectXMath.h>

using namespace DirectX;

Block::Block(XMFLOAT3 size)
    : m_collision()
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
    XMMATRIX S = XMMatrixScaling(
        m_collision.size.x,
        m_collision.size.y,
        m_collision.size.z
    );

    XMMATRIX mat = S * T;
    mat = XMMatrixTranspose(mat);

    XMFLOAT4X4 fMat;
    XMStoreFloat4x4(&fMat, mat);

    // 当たり判定ボックス描画
    Geometory::SetWorld(fMat);
    Geometory::DrawBox();
#endif
}

Collision::Box Block::GetCollision()
{
    return m_collision;
}

