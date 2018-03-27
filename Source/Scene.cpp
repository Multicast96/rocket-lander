#pragma once
#include"Scene.h"
#include<iostream>

Vector2f Scene::landingVelocity = Vector2f(0.0, 100);

Scene::Scene(Scene::Scenes sceneName) {
	this->sceneName = sceneName;
}

void Scene::Action() {
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		(*it)->action(); //action performed by this object
		Rocket* tmp;
		if ((tmp = dynamic_cast<Rocket*>(*it)) != nullptr && tmp->isActive){
			//jeszcze czas
			double result;
			if (tmp->timeout) {
				result = 0.5 / (1.0 + 0.005*(sceneObjects.back()->getPosition().y - tmp->getPosition().y + (tmp->getSize().y - sceneObjects.back()->getSize().y)*0.5));
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

void Scene::draw(RenderTarget &target, RenderStates state)const {
	for (SceneObject *obj : sceneObjects) target.draw(*obj);
}

//ostatni w liscie obiekt - platforma
void Scene::Add(SceneObject *sceneObject) {
	sceneObjects.push_front(sceneObject);
}

bool Scene::landingCheck(Rocket *r){
	Vector2f rPos = r->getPosition();
	Vector2f rSize = r->getSize();
	Platform *platform = dynamic_cast<Platform*>(sceneObjects.back());
	if (rPos.y + rSize.y / 2 > platform->getPosition().y - platform->getSize().y / 2) {
		return true;
	}
	return false;

}