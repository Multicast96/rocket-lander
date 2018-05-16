#include"Rocket.h"
#include"../Core/GameMaster.h"

double Rocket::gravity = 98.1;
std::string Rocket::names[] = { "Alfa", "Beta", "Gamma", "Delta", "Epsilon", "Dzeta", "Eta", "Theta", "Jota", "Kappa", "Lambda", "Mi", "Ni", "Ksi", "Omikron", "Pi", "Rho", "Sigma", "Tau", "Ipsilon", "Fi", "Chi", "Psi", "Omega" };

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
	frameReady = false;

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
	name.setPosition(position.x - size.x*2, position.y - size.y*0.8);
	label.setPosition(position.x - size.x*2, position.y - size.y * 0.75 + label.getCharacterSize());
	name.setFont(GameMaster::getFont(MAIN));
	label.setFont(GameMaster::getFont(MAIN));
	name.setFillColor(Color::Black);
	label.setFillColor(Color::Black);
	name.setString ("Janusz");
	//vv - vertical velocity
	label.setString("vv[m/s]: " + to_string(int(-velocity.y)));


	// Flame positioning
	flameSprites[currentFlameFrame].setPosition(Vector2f(-100,-100));
}

Texture Rocket::rocketTexture = Texture();
Texture Rocket::flameTexture = Texture();

void Rocket::InitTextures() {
	if (!Rocket::rocketTexture.loadFromFile("assets/textures/rocket.png")) throw std::runtime_error("Couldn't load rocket image");
	if (!Rocket::flameTexture.loadFromFile("assets/textures/flame_sheet.png")) throw std::runtime_error("Couldn't load flame image");

	flameTexture.setSmooth(true);
	rocketTexture.setSmooth(true);
}

double Rocket::distanceToPlatformY() {
	return GameMaster::getSize().y - 150 - position.y + size.y / 2;
}

double Rocket::distanceToPlatformX() {
	// 640 - pozycja platformy
	return position.x - 640;
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
	frameReady = true;
}

void Rocket::draw(RenderTarget &target, RenderStates state)const {
	if (isCrashed) return;
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

//Funkcja kt�ra przygotowuje dane do wys�ania do AI
void RocketAI::prepareSendData(char *tab, float distance_y, float distance_x, float timeLeft, char state) {
	memcpy(tab, &state, 1);
	memcpy(tab + 1, &distance_y, 4); //wysoko�� nad platform�
	memcpy(tab + 5, &velocity.y, 4);
	memcpy(tab + 9, &timeLeft, 4);
	memcpy(tab + 13, &distance_x, 4); // odleg�o�� x od platformy
}

void RocketAI::HandleInput(std::future<void> futureObj) {
	void* responder = zmq_socket(context, ZMQ_PAIR);
	int rc = zmq_bind(responder, ("tcp://*:" + std::to_string(60000 + id)).c_str());
	try {
		assert(rc == 0);
	}
	catch (runtime_error &e) {
		cout << "Asser error" << endl;
	}
	int timeout = 5000;
	zmq_setsockopt(responder, ZMQ_RCVTIMEO, &timeout, sizeof(int));
	char buffer[32];
	//Wys�anie startowej pozycji rakiety
	prepareSendData(buffer, distanceToPlatformY(),distanceToPlatformX(), AItraining::simTime , Commands::FLIGHT);
	zmq_send(responder, buffer, 17, 0);
	while(true){
		if(DEBUGINHO) cout << distanceToPlatformY()<<" " <<distanceToPlatformX()<< endl;
		if (zmq_recv(responder, buffer, 2, 0) == EAGAIN) {
			zmq_close(responder);
			return;
		}
		if (DEBUGINHO) cout << "rakieta " << id << ": " << buffer[0];
		switch (buffer[0]) {
		case KILL:
			zmq_close(responder);
			return;
		case CONTROL:
			if (DEBUGINHO) cout << " " << buffer[1] + '0' << endl;
			thrust = buffer[1] & 1;
			rightMovement = buffer[1] & 2;
			leftMovement = !rightMovement;

			//Synchronizacja klatek - AI mo�e wykonywa� tylko jeden ruch na klatk�.
			while (!frameReady && isActive) Sleep(1);
			frameReady = false;

			if (!isActive && velocity.y == 0)
				prepareSendData(buffer, 0, distanceToPlatformX(), AItraining::getTimeLeft(), Commands::LANDING);
			else if (isCrashed) {
				prepareSendData(buffer, 0, -1, distanceToPlatformX(), Commands::BOOM);
				if(DEBUGINHO) cout << "Crashed" << endl;
			}
			else
				prepareSendData(buffer, distanceToPlatformY(), distanceToPlatformX(), AItraining::getTimeLeft(), Commands::FLIGHT);

			zmq_send(responder, buffer, 17, 0);
			break;
		}
	}
}

RocketAI::RocketAI(Vector2f pos, int id, void* context) : Rocket(pos), context(context) {
	futureObj = exitSignal.get_future();
	input = std::thread(&RocketAI::HandleInput, this, std::move(futureObj));
	name.setString(names[(int)(GameMaster::random() * 24 - 1e-9)] + " #" + std::to_string(id));
	this->id = id;
	isCrashed = false;
}

RocketAI::~RocketAI() {
	exitSignal.set_value();
	input.join();
}