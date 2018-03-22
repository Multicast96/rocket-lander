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
	Platform(Vector2f pos);
	void draw(RenderTarget &target, RenderStates state)const;
	void move(Vector2f pos);
	void action();
	static Vector2f size;

private:
	RectangleShape sprite;
};