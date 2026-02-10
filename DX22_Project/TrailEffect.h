#pragma once
#include "PolylineEffect.h" 
#include "CPlayer.h" 

class TrailEffect : public PolylineEffect
{
private:
	CPlayer* m_pPlayer;         // プレイヤー情報 
	DirectX::XMFLOAT3 m_oldPos; // 以前にプレイヤーがいた位置 

public:
	// 初期化処理 
	TrailEffect(CPlayer* pPlayer);
	virtual ~TrailEffect();

protected:
	// ポリライン制御点更新処理 
	void UpdateControlPoints(LineID id, ControlPoints& controlPoints) final;

};
