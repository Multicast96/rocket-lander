#pragma once
#include"SceneObject.h"
#include"../common.h"
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
	Vector2f getVelocity();
	void setVelocity(Vector2f v);
	Rocket(Vector2f pos);
	static void InitTextures();
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f pos);
	void action();
	bool isActive;
	bool timeout;
protected:
	sf::Text name;
	Vector2f velocity;
private:
	Vector2f acceleration;
	static double gravity;
	static Texture rocketTexture;
	static Texture flameTexture;
	const int flameFrames = 1;
	sf::Sprite flameSprites[1];
	sf::Sprite rocketSprite;
	int currentFlameFrame;
	sf::Text label;
};

class RocketPlayer : public Rocket{
public:
	RocketPlayer(Vector2f pos);
	~RocketPlayer();
	void HandleInput(std::future<void>);
	std::thread input; //Nowy w¹tek s³ucha komunikatów sterowania
};

class RocketAI : public Rocket {
public:
	int id;
	RocketAI(Vector2f pos, int id);
	~RocketAI();
	void HandleInput(std::future<void>);
	std::thread input; //Nowy w¹tek s³ucha komunikatów sterowania
};