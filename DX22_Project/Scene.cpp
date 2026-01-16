#include "Scene.h"
#include "Fade.h"
Scene::Scene()
	: m_pFade(nullptr)
	, m_next(-1)
{
}
Scene::~Scene()
{
	m_pFade = nullptr;
}

void Scene::RootUpdate()
{
	Update();

	if (m_pFade)
		m_pFade->Update();
}
void Scene::RootDraw()
{
	Draw();

	if (m_pFade)
		m_pFade->Draw();
}

void Scene::SetFade(Fade* fade)
{
	m_pFade = fade;
}

bool Scene::IsChangeScene()
{
	if (m_pFade)
		return m_pFade->IsFinish() && m_pFade->IsFadeOut();

	return false;
}

int Scene::GetNext() const
{
	return m_next;
}

void Scene::SetNext(int next)
{
	m_next = next;

	// シーン切り替え要求時にフェードアウト開始
	if (m_pFade)
		m_pFade->Start(1.0f, false);
}