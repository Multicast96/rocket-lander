#pragma once
#include"Scene.h"

Vector2f AItraining::landingVelocity = Vector2f(0.0, 100);

void AItraining::Action() {
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		(*it)->action(); //action performed by this object
		Rocket* tmp;
		if ((tmp = dynamic_cast<Rocket*>(*it)) != nullptr && tmp->isActive) {
			//jeszcze czas
			double result;
			if (tmp->timeout) {
				result = 0.5 / (1.0 + 0.005*(platform->getPosition().y - tmp->getPosition().y + (tmp->getSize().y - platform->getSize().y)*0.5));
				tmp->isActive = false;
				it++;
				std::cout << result << std::endl;
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
				std::cout << result << std::endl;
			}
			else it++;
		}
		else it++;
	}
}

bool AItraining::landingCheck(Rocket *r) {
	Vector2f rPos = r->getPosition();
	Vector2f rSize = r->getSize();
	if (rPos.y + rSize.y / 2 > platform->getPosition().y - platform->getSize().y / 2) {
		return true;
	}
	return false;
}

AItraining::AItraining(Scene::SCENE_NAME sceneName) : Scene(sceneName) {}
