#pragma once
#include"SceneObject.h"
#include<stdexcept>

using namespace sf;

sf::Vector2f operator*(sf::Vector2f v, double d) {
	return sf::Vector2f(v.x * d, v.y * d);
}


Vector2f SceneObject::getPosition() { return position; }
Vector2f SceneObject::getSize() { return size; }
SceneObject::SceneObject(Vector2f position, Vector2f size) : position(position), size(size) {}


Background::Background(Vector2f position, Vector2f size) : SceneObject(position) {
	if (!bgTexture.loadFromFile("assets/textures/background.png")) throw std::runtime_error("Couldn't load background image");
	bgTexture.setSmooth(false);
	bgSprite.setTexture(bgTexture);
	size = Vector2f(size.x / bgSprite.getGlobalBounds().width, size.y / bgSprite.getGlobalBounds().height);
	bgSprite.setScale(size);
}

void Background::draw(RenderTarget &target, RenderStates state)const {
	target.draw(bgSprite);
}

void Background::move(Vector2f v) {}
void Background::action() {}


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