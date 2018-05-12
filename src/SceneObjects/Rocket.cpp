#include"Rocket.h"
#include"../Core/GameMaster.h"

double Rocket::gravity = 98.1;

Vector2f Rocket::getVelocity() {
	return velocity;
}
void Rocket::setVelocity(Vector2f v) {
	velocity = v;
}

Rocket::Rocket(Vector2f position) : SceneObject(position){
	Rocket::InitTextures();
	isActive = true;
	timeout = false;

	rocketSprite.setTexture(Rocket::rocketTexture);
	rocketSprite.setScale(Vector2f(GameMaster::getSize().x/1980.0, GameMaster::getSize().y/1080.0));
	size = Vector2f(rocketSprite.getGlobalBounds().width, rocketSprite.getGlobalBounds().height);

	currentFlameFrame = 0;
	for (int i = 0; i < flameFrames; i++) {
		flameSprites[i].setTexture(Rocket::flameTexture);
		flameSprites[i].setTextureRect(IntRect(Vector2<int>(33 * i, 0), Vector2<int>(33, 66)));
		flameSprites[i].setScale(Vector2f(1, 1));
	}


	velocity = Vector2f(0, 0);
	acceleration = Vector2f(0, gravity);
	rocketSprite.setPosition(position.x - size.x/2, position.y - size.y/2);

	//texts
	name = Text();
	label = Text();
	name.setCharacterSize(GameMaster::smallFontSize);
	label.setCharacterSize(GameMaster::smallFontSize);
	name.setPosition(position.x + size.x, position.y - size.y/2);
	label.setPosition(position.x + size.x, position.y - size.y / 2 + label.getCharacterSize());
	name.setFont(GameMaster::getFont(MAIN));
	label.setFont(GameMaster::getFont(MAIN));
	name.setFillColor(Color::Black);
	label.setFillColor(Color::Black);
	name.setString ("Janusz");

}

Texture Rocket::rocketTexture = Texture();
Texture Rocket::flameTexture = Texture();

void Rocket::InitTextures() {
	if (!Rocket::rocketTexture.loadFromFile("assets/textures/rocket.png")) throw std::runtime_error("Couldn't load rocket image");
	if (!Rocket::flameTexture.loadFromFile("assets/textures/flame_sheet.png")) throw std::runtime_error("Couldn't load flame image");

	flameTexture.setSmooth(true);
	rocketTexture.setSmooth(true);
}

double Rocket::distanceToPlatform() {
	return GameMaster::getSize().y - 175 - position.y + size.y / 2;
}

void Rocket::action() {
	if (!isActive) return;

	int moveSpeed = 100;
	int thrust = 120;
	if(isThrusting()) { acceleration.y = -thrust;}
	else { acceleration.y = gravity;}
	if (isMovingLeft()) move(Vector2f(-GameMaster::GetDeltaTime()*moveSpeed, 0));
	if (isMovingRight()) move(Vector2f(GameMaster::GetDeltaTime()*moveSpeed, 0));

	//vv - vertical velocity
	label.setString("vv[m/s]: " + to_string(int(-velocity.y)));

	move(velocity * GameMaster::GetDeltaTime());
	velocity += acceleration * GameMaster::GetDeltaTime();

	//sprite'y
	currentFlameFrame = (currentFlameFrame + 1) % flameFrames;

	Vector2f tmp(position);
	tmp.y += size.y / 2;
	tmp.x -= flameSprites[currentFlameFrame].getGlobalBounds().width / 2;
	flameSprites[currentFlameFrame].setPosition(tmp);
	flameSprites[currentFlameFrame].setScale(Vector2f(rocketSprite.getScale().x, isThrusting() ? rocketSprite.getScale().y : rocketSprite.getScale().y*0.25));
}

void Rocket::draw(RenderTarget &target, RenderStates state)const {
	target.draw(flameSprites[currentFlameFrame]);
	target.draw(rocketSprite);
	target.draw(name);
	target.draw(label);
}

void Rocket::move(Vector2f v) {
	position += v;
	rocketSprite.move(v);
	name.move(v);
	label.move(v);
}

bool Steerable::isMovingLeft() { return leftMovement; } 
bool Steerable::isMovingRight() { return rightMovement; }
bool Steerable::isThrusting() { return thrust; }

void RocketPlayer::HandleInput(std::future<void> futureObj) {
	while (futureObj.wait_for(std::chrono::milliseconds(10)) == std::future_status::timeout){
		thrust = Keyboard::isKeyPressed(Keyboard::W);
		leftMovement = Keyboard::isKeyPressed(Keyboard::A);
		rightMovement = Keyboard::isKeyPressed(Keyboard::D);
	}
}

RocketPlayer::RocketPlayer(Vector2f pos) : Rocket(pos) {
	futureObj = exitSignal.get_future();
	input = std::thread(&RocketPlayer::HandleInput, this, std::move(futureObj));
}

RocketPlayer::~RocketPlayer() {
	exitSignal.set_value();
	input.join();
}


void RocketAI::HandleInput(std::future<void> futureObj) {
	int rc = zmq_bind(responder, ("tcp://*:" + std::to_string(50000 + id)).c_str());
	assert(rc == 0);
	std::cout << distanceToPlatform() << std::endl;
	char buffer[16];
	while (true) {
		zmq_recv(responder, buffer, 2, 0);
		if (DEBUGINHO) cout << "rakieta " << id << ": " << buffer << endl;
		switch (buffer[0]) {
		case KILL:
			zmq_close(responder);
			return;
		case CONTROL:
			thrust = buffer[1];
			float distance = distanceToPlatform();
			float timeLeft = AItraining::getTimeLeft(); //czas do zakoñczenia symulacji
			memcpy(buffer, &distance, 4); //wysokoœæ nad platform¹
			memcpy(buffer + 4, &velocity.y, 4);
			memcpy(buffer + 8, &time, 4);
			zmq_send(responder, buffer, 12, 0);
			break;
		}
	}
}

RocketAI::RocketAI(Vector2f pos, int id, void* responder) : Rocket(pos), responder(responder) {
	futureObj = exitSignal.get_future();
	input = std::thread(&RocketAI::HandleInput, this, std::move(futureObj));
	name.setString("Janusz #" + std::to_string(id));
	this->id = id;
}

RocketAI::~RocketAI() {
	exitSignal.set_value();
	input.join();
}