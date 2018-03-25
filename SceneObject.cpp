#pragma once
#include"SceneObject.h"
#include<stdexcept>

using namespace sf;

Vector2f operator*(Vector2f v, double d) {
	return Vector2f(v.x * d, v.y * d);
}

Vector2f SceneObject::getPosition() { return position; }
Vector2f SceneObject::getSize() { return size; }
SceneObject::SceneObject(Vector2f position, Vector2f size) : position(position), size(size) {}


Platform::Platform(Vector2f position, Vector2f size) : SceneObject(position,size) {
	sprite = RectangleShape(size);
	sprite.setPosition(position - Vector2f(size.x/2, size.y/2));
	sprite.setFillColor(Color(128, 16, 0));
}

void Platform::draw(RenderTarget &target, RenderStates state)const {
	target.draw(sprite);
}

void Platform::move(Vector2f v) {

}

void Platform::action() {

}