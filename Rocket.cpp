#include"Rocket.h"
#include"GameMaster.h"

double Rocket::gravity = 98.1;

Rocket::Rocket(Vector2f pos) {
	Rocket::InitTextures();

	rocketSprite.setTexture(Rocket::rocketTexture);
	rocketSprite.setScale(Vector2f(0.3, 0.3));

	currentFlameFrame = 0;
	for (int i = 0; i < flameFrames; i++) {
		flameSprites[i].setTexture(Rocket::flameTexture);
		flameSprites[i].setTextureRect(IntRect(Vector2<int>(90 * i, 0), Vector2<int>(90, 256)));
		flameSprites[i].setScale(Vector2f(0.3, 0.3));
	}

	position = pos;
	velocity = Vector2f(0, 0);
	acceleration = Vector2f(0, gravity);
	rocketSprite.setPosition(position.x - rocketSprite.getGlobalBounds().width / 2, position.y - rocketSprite.getGlobalBounds().height / 2);

	////REMOTE CONNECTION
	//remote = std::thread(&Rocket::RemoteConnection, this);
}

Rocket::~Rocket() {
	//remote.~thread();
}

Texture Rocket::rocketTexture = Texture();
Texture Rocket::flameTexture = Texture();

void Rocket::InitTextures() {
	if (!Rocket::rocketTexture.loadFromFile("rocket.png")) throw std::runtime_error("Couldn't load rocket image");
	if (!Rocket::flameTexture.loadFromFile("flame_sheet.png")) throw std::runtime_error("Couldn't load flame image");

	flameTexture.setSmooth(false);
	rocketTexture.setSmooth(true);
}

//void Rocket::RemoteConnection() {
//	void *context = zmq_ctx_new();
//
//	//  Socket to talk to clients
//	void *responder = zmq_socket(context, ZMQ_REP);
//	int rc = zmq_bind(responder, "tcp://*:5555");
//	assert(rc == 0);
//	char buffer[10];
//	std::string response = "";
//	while (true) {
//		zmq_recv(responder, buffer, 10, 0);
//		int b = atoi(buffer);
//		if (b & 0x1) move(Vector2f(-GameMaster::GetDeltaTime() * 100, 0)); //LEFT
//		if (b & 0x2) move(Vector2f(GameMaster::GetDeltaTime() * 100, 0)); //RIGHT
//		if (b & 0x4) acceleration.y = -120;
//		else acceleration.y = gravity;
//
//		memset(buffer, 0, sizeof(buffer));
//
//		response = '(' + std::to_string(position.x) + " " + std::to_string(position.y) + ')'; //testy testy testy
//		char tab2[1024];
//		strcpy(tab2, response.c_str()); // brzyyyyydko
//		zmq_send(responder, tab2, 25, 0);
//	}
//	zmq_ctx_destroy(context); //zwolnienie zasobów po³¹czenia 
//}

void Rocket::action() {
	int moveSpeed = 100;
	int thrust = 120;
	if(isThrusting()) { acceleration.y = -thrust;}
	else { acceleration.y = gravity;}
	if (isMovingLeft()) move(Vector2f(-GameMaster::GetDeltaTime()*moveSpeed, 0));
	if (isMovingRight()) move(Vector2f(GameMaster::GetDeltaTime()*moveSpeed, 0));

	GameMaster::Log("Vertical velocity: " + to_string(int(-velocity.y / 10)) + " m/s");
	move(velocity * GameMaster::GetDeltaTime());
	velocity += acceleration * GameMaster::GetDeltaTime();

	//sprite'y
	currentFlameFrame = (currentFlameFrame + 1) % flameFrames;

	Vector2f tmp(position);
	tmp.y += rocketSprite.getGlobalBounds().height / 2;
	tmp.x -= flameSprites[currentFlameFrame].getGlobalBounds().width / 2;
	flameSprites[currentFlameFrame].setPosition(tmp);
	flameSprites[currentFlameFrame].setScale(Vector2f(0.25, isThrusting() ? 0.4 : 0.05));
}

void Rocket::draw(RenderTarget &target, RenderStates state)const {
	target.draw(flameSprites[currentFlameFrame]);
	target.draw(rocketSprite);
}

void Rocket::move(Vector2f pos) {
	position += pos;
	rocketSprite.move(pos);
}

bool Steerable::isMovingLeft() { return leftMovement; } 
bool Steerable::isMovingRight() { return rightMovement; }
bool Steerable::isThrusting() { return thrust; }

void RocketPlayer::HandleInput() {
	while (true){
		thrust = Keyboard::isKeyPressed(Keyboard::W);
		leftMovement = Keyboard::isKeyPressed(Keyboard::A);
		rightMovement = Keyboard::isKeyPressed(Keyboard::D);
	}
}

RocketPlayer::RocketPlayer(Vector2f pos) : Rocket(pos) {
	input = std::thread(&RocketPlayer::HandleInput, this);
}

