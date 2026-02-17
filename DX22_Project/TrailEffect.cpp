#include "TrailEffect.h"

TrailEffect::TrailEffect(CPlayer* pPlayer)
	:m_pPlayer(pPlayer)
{
	m_oldPos = m_pPlayer->GetPos();
}
TrailEffect::~TrailEffect()
{

}
void TrailEffect::UpdateControlPoints(LineID id, ControlPoints& controlPoints)
{
	ControlPoints::iterator it = controlPoints.begin();
	while (it != controlPoints.end())
	{
		it->bold *= 0.95f;
		++it;
	}

	float distance = 0.0f;
	DirectX::XMFLOAT3 pos = m_pPlayer->GetPos();
	DirectX::XMVECTOR vOld = DirectX::XMLoadFloat3(&m_oldPos);
	DirectX::XMVECTOR vNow = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR vDir =
    DirectX::XMVectorSubtract(vOld, vNow);

	//
	DirectX::XMStoreFloat(&distance, DirectX::XMVector3Length(vDir));

	if (distance >= 0.2f)
	{
		for (int i = controlPoints.size() - 1; i > 0; --i)
		{
			controlPoints[i] = controlPoints[i - 1];
		}
		controlPoints[0].pos = pos;
		controlPoints[0].bold = 0.1f;


		m_oldPos = pos;
	}
}