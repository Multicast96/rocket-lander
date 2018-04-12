#pragma once
#include"../SceneObjects/SceneObject.h"
#include"../Core/GameMaster.h"
#include<stdexcept>


sf::Vector2f operator*(sf::Vector2f v, double d) {
	return sf::Vector2f(v.x * d, v.y * d);
}

using namespace sf;
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
	sprite.move(v);
	position += v;
}

void Platform::action() {
	// dla beki :)
	//const int speed = 10;
	//double vertical = speed * sin(GameMaster::GetTime())  * GameMaster::GetDeltaTime();
	//vertical *= vertical;
	//double horizontal = speed * sin(GameMaster::GetTime()) * GameMaster::GetDeltaTime();

	//move(Vector2f(vertical, horizontal));
}

sf::Texture Wave::waveTexture = sf::Texture();

Wave::Wave(Vector2f position, sf::Color color, bool direction, Vector2f size) : SceneObject(position), direction(direction) {
	InitTextures();
	waveSprite1.setTexture(waveTexture);
	waveSprite2.setTexture(waveTexture);

	Vector2f scale = Vector2f(size.x / waveSprite1.getGlobalBounds().width, size.y / waveSprite1.getGlobalBounds().height);
	waveSprite1.setScale(scale);
	waveSprite2.setScale(scale);

	waveSprite1.setPosition(Vector2f(position.x - size.x, position.y - size.y / 2));
	waveSprite2.setPosition(Vector2f(position.x, position.y - size.y/2));

	waveSprite1.setColor(color);
	waveSprite2.setColor(color);

	speed = (rand() % 50 + 10) *  (direction ? 1 : -1);
}

void Wave::draw(RenderTarget &target, RenderStates state)const {
	target.draw(waveSprite1);
	target.draw(waveSprite2);
}

void Wave::move(Vector2f v) {}
void Wave::action() {
	int dir = direction ? 1 : -1;

	waveSprite1.move(Vector2f(speed * GameMaster::GetDeltaTime(), 0));
	waveSprite2.move(Vector2f(speed * GameMaster::GetDeltaTime(), 0));

	if ( (direction && waveSprite2.getPosition().x > GameMaster::getSize().x) || (!direction && waveSprite2.getPosition().x < -GameMaster::getSize().x)) {
			waveSprite2.move(Vector2f(-dir * GameMaster::getSize().x*2, 0));
	}
	if ((direction && waveSprite1.getPosition().x > GameMaster::getSize().x) || (!direction && waveSprite1.getPosition().x < -GameMaster::getSize().x)) {
		waveSprite1.move(Vector2f(-dir * GameMaster::getSize().x*2, 0));
	}
}

void Wave::InitTextures() {
	if (!Wave::waveTexture.loadFromFile("assets/textures/wave.png")) throw std::runtime_error("Couldn't load wave.png");
	waveTexture.setSmooth(true);
}