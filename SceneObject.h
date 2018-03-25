#pragma once
#include<SFML\Graphics.hpp>

using namespace sf;

Vector2f operator*(Vector2f v, double d);

class SceneObject : public Drawable{
public:
	virtual void draw(RenderTarget &target, RenderStates state) const = 0;
	virtual void move(Vector2f pos) = 0;
	virtual void action() = 0;
};

class Platform : public SceneObject {
public:
	Vector2f size;

	Platform(Vector2f pos, Vector2f size = Vector2f(200, 100));
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f pos);
	void action();

private:
	RectangleShape sprite;
};