#ifndef __GANE_OBJECT_H__
#define __GANE_OBJECT_H__

#include <DirectXMath.h>

struct GameObject
{
protected:
    DirectX::XMFLOAT3 m_pos;

public:
    GameObject();
    virtual ~GameObject();

    virtual void Update();
    virtual void Draw();


    DirectX::XMFLOAT3 GetPos();
    void SetPos(DirectX::XMFLOAT3 pos);


};
#endif