#pragma once
#include"Scene.h"

Scene::Scene(Scene::Scenes sceneName) {
	this->sceneName = sceneName;
}

void Scene::Action() {
	auto it = sceneObjects.begin();
	while (it != sceneObjects.end()) {
		(*it)->action(); //action performed by this object
		if (landingCheck(*it)){
			SceneObject *tmp = dynamic_cast<Rocket*>(*it);
			//delete tmp;
			it = sceneObjects.erase(it);
		}
		else it++;
	}
}

void Scene::draw(RenderTarget &target, RenderStates state)const {
	for (SceneObject *obj : sceneObjects) target.draw(*obj);
}

//ostatni w vectorze obiekt - platforma
void Scene::Add(SceneObject *sceneObject) {
	sceneObjects.push_front(sceneObject);
}

bool Scene::landingCheck(SceneObject *r){
	if ((r = dynamic_cast<Rocket*>(r)) == nullptr) return false;
	Vector2f rPos = r->getPosition();
	Vector2f rSize = r->getSize();
	Platform *platform = dynamic_cast<Platform*>(sceneObjects.back());
	if (rPos.y + rSize.y / 2 > platform->getPosition().y - platform->getSize().y / 2) {
		return true;
	}
	return false;

}