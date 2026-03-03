#pragma once


#include "GaneObject.h"
#include "Collision.h"

class Block final : public GameObject
{
private: 
	Collision::Box m_collision;
	Collision::Plane m_plane;
	DirectX::XMFLOAT3 m_rotate;
public:
	Block(DirectX::XMFLOAT3 size);
	~Block();

	void Update();
	void Draw();

	void SetRotate(float x, float y, float z);

	Collision::Box GetCollision();
	Collision::Plane GetPlane();
};