#pragma once
#include"SceneObject.h"
#include<stdexcept>

using namespace sf;

Vector2f operator*(Vector2f v, double d) {
	return Vector2f(v.x * d, v.y * d);
}


Vector2f Platform::size = Vector2f(200, 100);

Platform::Platform(Vector2f pos) {
	sprite = RectangleShape(size);
	sprite.setPosition(pos + Vector2f(-size.x/2, 0));
	sprite.setFillColor(Color(128, 16, 0));
}

void Platform::draw(RenderTarget &target, RenderStates state)const {
	target.draw(sprite);
}

void Platform::move(Vector2f pos) {

}

void Platform::action() {

}