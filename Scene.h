#pragma once
#include<vector>
#include"SceneObject.h"

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
	vector<SceneObject*> sceneObjects;
	void landingCheck(Rocket*);
};