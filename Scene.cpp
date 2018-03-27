#pragma once
#include"Scene.h"


Scene::Scene(Scene::SCENE_NAME sceneName) {
	this->sceneName = sceneName;
}

void Scene::draw(RenderTarget &target, RenderStates state)const {
	for (SceneObject *obj : sceneObjects) target.draw(*obj);
}

void Scene::Add(SceneObject *sceneObject) {
	sceneObjects.push_back(sceneObject);
}

void Scene::AddPlatform(SceneObject *sceneObject) {
	sceneObjects.push_back(sceneObject);
	platform = dynamic_cast<Platform*>(sceneObject);
}