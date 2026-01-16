#pragma once


#include "GaneObject.h"
#include "Collision.h"

class Block final : public GameObject
{
private: 
	Collision::Box m_collision;

public:
	Block(DirectX::XMFLOAT3 size);
	~Block();

	void Update();
	void Draw();

	Collision::Box GetCollision();
};