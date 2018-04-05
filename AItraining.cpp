#include"Scene.h"
#include"GameMaster.h"

Vector2f AItraining::landingVelocity = Vector2f(0.0, 100);
const double AItraining::simTime = 10.0;

void AItraining::Action() {
	if (!isRunning)
		return;
	auto it = sceneObjects.begin();
	RocketAI* tmp;
	double tmpTime = simStart - GameMaster::GetTime() + simTime;
	if (tmpTime <= 0) {
		while (it != sceneObjects.end()) {
			if (tmp = dynamic_cast<RocketAI*>(*it)) {
				tmp->timeout = true;
			}
			it++;
		}
	}

	it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		(*it)->action(); //action performed by this object
		isRunning = false;
		if ((tmp = dynamic_cast<RocketAI*>(*it)) != nullptr && tmp->isActive) {
			GameMaster::displayOnGUI("Time: " + std::to_string(tmpTime), GameMaster::GUI::UPPER_RIGHT1);
			isRunning = true;
		//TODO jeszcze czs
			double result;
			if (tmp->timeout) {
				result = 0.5 / (1.0 + 0.005*(platform->getPosition().y - tmp->getPosition().y + (tmp->getSize().y - platform->getSize().y)*0.5));
				tmp->isActive = false;
				it++;
				results[tmp->id] = result;
			}
			else if (landingCheck(tmp)) {
				if (tmp->getVelocity().y >= landingVelocity.y) {
					it = sceneObjects.erase(it);
					result = 0.5 / (1.0 + 0.01*(tmp->getVelocity().y - landingVelocity.y));
				}
				else {
					result = 1 - 0.5 * tmp->getVelocity().y / landingVelocity.y;
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
	if (!isRunning)
		sendResults();
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
}

void AItraining::spawnRockets(int n) {
	rocketCount = n;
	results = new double[n];
	for (int i = 0; i < n; i++) {
		AddRocket(new RocketAI((Vector2f)GameMaster::getSize() * 0.5, i));
	}
	simStart = GameMaster::GetTime();
	isRunning = true;
}

void AItraining::sendResults() {
	std::string r = "";
	for (int i = 0; i < rocketCount; i++) {
		r += std::to_string(results[i]) + " ";
	}
	std::cout << r << std::endl;
	delete[] results;
	isRunning = false;
}