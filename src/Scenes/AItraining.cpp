#include"Scene.h"
#include"../Core/GameMaster.h"

Vector2f AItraining::landingVelocity = Vector2f(0.0, 100);
const double AItraining::simTime = 10.0;
double AItraining::simStart = 0;

float AItraining::getTimeLeft() {
	double tmpTime = simStart - GameMaster::GetTime() + simTime;
	if (tmpTime < 0) return 0;
	else return tmpTime;
}

void AItraining::Action() {
	auto it = sceneObjects.begin();
	RocketAI* tmp;

	if (!isRunning) return;
	it = sceneObjects.begin();
	//Jeœli czas symulacji siê skoñczy oznaczamy timeout w ka¿dej rakiecie na true
	if (!getTimeLeft()) {
		while (it != sceneObjects.end()) {
			if (tmp = dynamic_cast<RocketAI*>(*it)) {
				tmp->timeout = true;
			}
			it++;
		}
	}

	isRunning = false; //czy zosta³a choæ jedna aktywna rakieta na ekranie

	it = sceneObjects.begin();
	GameMaster::displayOnGUI("Time: " + std::to_string(getTimeLeft()), GameMaster::GUI::UPPER_RIGHT1);
	GameMaster::displayOnGUI("Generation: " + std::to_string(iteration), GameMaster::GUI::UPPER_RIGHT2);

	while (it != sceneObjects.end()) {
		(*it)->action(); //action performed by this object

		if ((tmp = dynamic_cast<RocketAI*>(*it)) != nullptr && tmp->isActive) {
			isRunning = true;

		//TODO jeszcze czs
			double result = 0;
			if (tmp->timeout) {
				//result = 0.5 / (1.0 + 0.005*(platform->getPosition().y - tmp->getPosition().y + (tmp->getSize().y - platform->getSize().y)*0.5));
				tmp->isActive = false;
				it = sceneObjects.erase(it);
				results[tmp->id] = result;
			}
			else if (landingCheck(tmp)) {
				if (tmp->getVelocity().y >= landingVelocity.y) {
					tmp->isActive = false;
					it = sceneObjects.erase(it);
					//result = 0.5 / (1.0 + 0.01*(tmp->getVelocity().y - landingVelocity.y));
				}
				else {
					//result = 1 - 0.5 * tmp->getVelocity().y / landingVelocity.y;
					tmp->setVelocity(Vector2f(0, 0));
					tmp->isActive = false;
					it++;
				}
				results[tmp->id] = result;
			}
			else it++;
		}
		else it++;
	}
	if (!isRunning) resultsReady = true;
}

bool AItraining::landingCheck(Rocket *r) {
	Vector2f rPos = r->getPosition();
	Vector2f rSize = r->getSize();
	if (rPos.y + rSize.y / 2 > platform->getPosition().y - platform->getSize().y / 2) {
		return true;
	}
	return false;
}

AItraining::AItraining(Scene::SCENE_NAME sceneName) : Scene(sceneName), isRunning(false) {
	server = std::thread(&AItraining::handleServer, this);
	iteration = 0;
}

AItraining::~AItraining() {
	server.join();
}

void AItraining::spawnRockets(int n) {
	rocketCount = n;
	resultsReady = false;
	results = new double[n];
	for (int i = 0; i < n; i++) {
		AddRocket(new RocketAI(Vector2f((GameMaster::getSize().x/n) * (n-i) - 100, GameMaster::getSize().y*0.3), i, context));
	}
	iteration++;
}

void AItraining::sendResults(void* responder) {
	std::string r = "";
	for (int i = 0; i < rocketCount; i++) {
		r += std::to_string(results[i]) + " ";
	}
	zmq_send(responder, r.c_str(), r.size(), 0);
	delete[] results;
	isRunning = false;
}

void AItraining::handleServer() {
	context = zmq_ctx_new();
	auto it = sceneObjects.begin();
	RocketAI *tmp;

	//  Socket to talk to clients
	void *responder = zmq_socket(context, ZMQ_PAIR);
	int rc = zmq_bind(responder, "tcp://*:5555");
	assert(rc == 0);

	char buffer[10];
	while (true) {
		zmq_recv(responder, buffer, 1, 0);
		if (DEBUGINHO) cout << "init: " << buffer[0] << endl;
		switch (buffer[0])
		{
		case SCENE_INIT:
			//Rozpoczêcie inicjalizacji sceny
			buffer[0] = OK;
			zmq_send(responder, buffer, 1, 0);

			//Odbieramy iloœæ rakiet do utworzenia
			memset(buffer, 0, sizeof(buffer));
			zmq_recv(responder, buffer, 10, 0);
			if (DEBUGINHO) cout << "scena: " << buffer[0] << endl;
			spawnRockets(stoi(buffer));

			//Rakiety s¹ stworzone i oczekuj¹ na po³¹czenie
			buffer[0] = OK;
			zmq_send(responder, buffer, 1, 0);

			//Python tworzy w¹tki do sterowania rakiet
			//i czeka na wyniki
			zmq_recv(responder, buffer, 1, 0);

			//Usuniêcie obiektów poprzedniej iteracji
			while (it != sceneObjects.end()) {
				tmp = dynamic_cast<RocketAI*>(*it);
				if (tmp != NULL && !tmp->isActive) it = sceneObjects.erase(it);
				else it++;
			}


			simStart = GameMaster::GetTime();
			isRunning = true;
			while (!resultsReady) {
				Sleep(10);
			}
			sendResults(responder);
			break;
		case KILL:
			zmq_close(responder);
			zmq_ctx_destroy(context);
			return;
		}
	}
}