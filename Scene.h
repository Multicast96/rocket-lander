#pragma once
#include"Rocket.h"
#include"common.h"

using namespace std;

class Scene : public Drawable{
public:
	static enum SCENE_NAME {
		MENU,
		AI_TRAINING
	};
	SCENE_NAME sceneName;
	Scene(SCENE_NAME sceneName);
	virtual void draw(RenderTarget &target, RenderStates state)const;
	void Add(SceneObject *sceneObject);
	void AddPlatform(SceneObject * sceneObject);
	virtual void Action() = 0;

protected:
	Platform* platform;
	list<SceneObject*> sceneObjects;
};

class AItraining : public Scene {
public:
	AItraining(SCENE_NAME sceneName);
	void Action();
private:
	bool landingCheck(Rocket * r);
	static Vector2f landingVelocity;
};