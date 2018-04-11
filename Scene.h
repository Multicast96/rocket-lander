#pragma once
#include"common.h"
#include"Rocket.h"

// REMOTE CONNECTION DEPENDENCIES
#include <zmq.h>
//#include <stdio.h>
//#include <io.h>
//#include <string.h>
#include <assert.h>

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
	void AddRocket(SceneObject * sceneObject);
	void AddPlatform(SceneObject * sceneObject);
	virtual void Action() = 0;
	std::string getName();
protected:
	static const std::string SCENE_NAMES[2];
	Platform* platform;
	list<SceneObject*> sceneObjects;
};

class AItraining : public Scene {
public:
	AItraining(SCENE_NAME sceneName);
	~AItraining();
	void Action();
	const static double simTime;
	void spawnRockets(int n);
private:
	void* context; //zmq connection
	std::thread server;
	bool isRunning;
	double simStart;
	double* results;
	bool resultsReady;
	int rocketCount;
	void sendResults(void * responder);
	void handleServer();
	bool landingCheck(Rocket * r);
	static Vector2f landingVelocity;
};