#pragma once
#include<vector>
#include<list>
#include"Rocket.h"

using namespace std;

class Scene : public Drawable{
public:
	static enum Scenes {
		MENU,
		AI_TRAINING
	};
	Scenes sceneName;
	Scene(Scenes sceneName);
	virtual void draw(RenderTarget &target, RenderStates state)const;
	void Add(SceneObject *sceneObject);
	void Action();
private:
	list<SceneObject*> sceneObjects;
	bool landingCheck(SceneObject * r);
};