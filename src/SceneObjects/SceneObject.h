#pragma once
#include"../common.h"

sf::Vector2f operator*(sf::Vector2f v, double d);

using namespace sf;


class SceneObject : public Drawable{
public:
	SceneObject(Vector2f position, Vector2f size = Vector2f());
	virtual void draw(RenderTarget &target, RenderStates state) const = 0;
	virtual void move(Vector2f position) = 0;
	virtual void action() = 0;

	Vector2f getSize();
	Vector2f getPosition();
protected:
	Vector2f size;
	Vector2f position;
};

class Platform : public SceneObject {
public:
	Platform(Vector2f position, Vector2f size = Vector2f(200, 100));
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f position);
	void action();

private:
	RectangleShape sprite;
};

class Background : public SceneObject {
public:
	Background(Vector2f position, Vector2f size = Vector2f(1920, 1680));
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f position);
	void action();
private:
	Texture bgTexture;
	Sprite bgSprite;
};

class Wave : public SceneObject {
public:
	Wave(Vector2f position, sf::Color color, bool direction, Vector2f size = Vector2f(1920, 200));
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f position);
	void action();
	static void InitTextures();
private:
	bool direction;
	float speed;
	Sprite waveSprite1, waveSprite2;
	static Texture waveTexture;
};