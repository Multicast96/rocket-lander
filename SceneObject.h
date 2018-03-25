#pragma once
#include<SFML\Graphics.hpp>

using namespace sf;

Vector2f operator*(Vector2f v, double d);

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