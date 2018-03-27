#include"Rocket.h"
#include"GameMaster.h"

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
	rocketSprite.setScale(Vector2f(1280/1980.0, 720/1080.0));
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
	name.setCharacterSize(GameMaster::getSize().y*0.03);
	label.setCharacterSize(GameMaster::getSize().y*0.03);
	name.setPosition(position.x + size.x, position.y - size.y/2);
	label.setPosition(position.x + size.x, position.y - size.y / 2 + label.getCharacterSize());
	name.setFont(GameMaster::getFont());
	label.setFont(GameMaster::getFont());
	name.setFillColor(Color::Black);
	label.setFillColor(Color::Black);
	name.setString ("Janusz");

}

Texture Rocket::rocketTexture = Texture();
Texture Rocket::flameTexture = Texture();

void Rocket::InitTextures() {
	if (!Rocket::rocketTexture.loadFromFile("assets/textures/rocket.png")) throw std::runtime_error("Couldn't load rocket image");
	if (!Rocket::flameTexture.loadFromFile("assets/textures/flame_sheet.png")) throw std::runtime_error("Couldn't load flame image");

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
	if (!isActive) return;

	int moveSpeed = 100;
	int thrust = 120;
	if(isThrusting()) { acceleration.y = -thrust;}
	else { acceleration.y = gravity;}
	if (isMovingLeft()) timeout = true;
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
	flameSprites[currentFlameFrame].setScale(Vector2f(1280/1980.0, isThrusting() ? 720 / 1080.0 : 0.3));
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
