#include "GaneObject.h"

GameObject::GameObject()
    : m_pos(0.0f, 0.0f, 0.0f)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
}

void GameObject::Draw()
{
}



DirectX::XMFLOAT3 GameObject::GetPos()
{
	return m_pos;
}

void GameObject::SetPos(DirectX::XMFLOAT3 pos)
{
	m_pos = pos;
}