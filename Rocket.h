#pragma once
#include"SceneObject.h"
#include<thread>
#include<future>
#include<assert.h>
#include<chrono>

// REMOTE CONNECTION DEPENDENCIES
//#include <zmq.h>
//#include <stdio.h>
//#include <io.h>
//#include <string.h>
//#include <assert.h>

class Steerable {
public:
	bool isMovingLeft();
	bool isMovingRight();
	bool isThrusting();
	virtual void HandleInput(std::future<void>) = 0;

	bool leftMovement = false;
	bool rightMovement = false;
	bool thrust = false;
	bool handleInputStop = false;
	std::promise<void> exitSignal;
	std::future<void> futureObj;
};

class Rocket : public SceneObject, public Steerable{
public:
	Rocket(Vector2f pos);
	static void InitTextures();
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f pos);
	void action();

private:
	//TODO zrobiæ to sensowniej
	//void RemoteConnection();
	//std::thread remote; // Nowy w¹tek s³ucha komunikatów od zdalnego programu
	Vector2f velocity;
	Vector2f acceleration;
	static double gravity;
	static Texture rocketTexture;
	static Texture flameTexture;
	const int flameFrames = 1;
	Sprite flameSprites[1];
	Sprite rocketSprite;
	int currentFlameFrame;
};

class RocketPlayer : public Rocket{
public:
	RocketPlayer(Vector2f pos);
	~RocketPlayer();
	void HandleInput(std::future<void>);
	std::thread input; //Nowy w¹tek s³ucha komunikatów sterowania
};