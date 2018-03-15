#pragma once
#include"Scene.h"
#include<iostream>

Scene::Scene(Scene::Scenes sceneName) {
	this->sceneName = sceneName;
}

void Scene::Action() {
	for (SceneObject *obj : sceneObjects) {
		obj->action(); //action performed by this object
		landingCheck(dynamic_cast<Rocket*>(obj));
	}
}

void Scene::draw(RenderTarget &target, RenderStates state)const {
	for (SceneObject *obj : sceneObjects) target.draw(*obj);
}

//ostatni w vectorze obiekt - platforma
void Scene::Add(SceneObject *sceneObject) {
	sceneObjects.push_back(sceneObject);
}

void Scene::landingCheck(Rocket *r){
	if (r == nullptr) return;

}