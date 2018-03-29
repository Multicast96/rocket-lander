#pragma once
#include"Scene.h"

const std::string Scene::SCENE_NAMES[2] = { "Menu","AI training",};
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

std::string Scene::getName()
{
	return SCENE_NAMES[sceneName];
}
