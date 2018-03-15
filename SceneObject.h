#pragma once
#include<SFML\Graphics.hpp>
#include<thread>

using namespace sf;
class SceneObject :  public Drawable{
public:
	virtual void draw(RenderTarget &target, RenderStates state) const = 0;
	virtual void move(Vector2f pos) = 0;
	virtual void action() = 0;
};

class Rocket : public SceneObject {
public:
	Rocket(float x, float y);
	static void InitTextures();
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f pos);
	void action();
	~Rocket();

private:
	//TODO zrobiæ to sensowniej
	void RemoteConnection();
	std::thread remote; // Nowy w¹tek s³ucha komunikatów od zdalnego programu


	Vector2f position;
	Vector2f velocity;
	Vector2f acceleration;
	static double gravity;
	static Texture rocketTexture;
	static Texture flameTexture;
	const int flameFrames = 4;
	Sprite flameSprites[4];
	Sprite rocketSprite;
	int currentFlameFrame;
};

class Platform : public SceneObject {
public:
	Platform(Vector2f pos);
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f pos);
	void action();
	static Vector2f size;

private:
	RectangleShape sprite;
};