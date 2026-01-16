#ifndef __SCENE_H__
#define __SCENE_H__

class Fade;

class Scene
{
public:
	Scene();
	virtual ~Scene();


	void RootUpdate();
	void RootDraw();

	void SetFade(Fade* fade);

	virtual bool IsChangeScene();

	int  GetNext() const;
	void SetNext(int next);

	
	virtual void Update() = 0;
	virtual void Draw() = 0;
protected:
	Fade* m_pFade;
	int   m_next;
};

#endif // __SCENE_H__