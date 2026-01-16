//
//#pragma once
//
//#ifndef __GAME_OBJECT_H__
//#define __GAME_OBJECT_H__
//
//#include <DirectXMath.h>
//
//class GameObject
//{
//public:
//    //Šî–{‘€ì
//    GameObject()
//        :m_pos(0.0f, 0.0f, 0.0f) {}
//
//    virtual ~GameObject() {}
//    virtual void Update() {}
//    virtual void Draw() {}
//
//    // À•W‘€ì
//    DirectX::XMFLOAT3 GetPos();              // © ’†g‚ğíœ‚µ‚ÄéŒ¾‚¾‚¯‚É‚·‚é
//    void SetPos(DirectX::XMFLOAT3 pos);
//
//protected:
//    DirectX::XMFLOAT3 m_pos;
//};
//
//#endif // !__GAME_OBJECT_H__
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <DirectXMath.h>

class GameObject
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