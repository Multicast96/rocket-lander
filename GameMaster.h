#pragma once
#include"Scene.h"
#include<SFML\Graphics.hpp>
#include<vector>

using namespace std;


class GameMaster {
public:
	Scene* currentScene; //tymczasowo publiczne

	GameMaster(Vector2i size);
	void MainLoop();
	void GameMaster::AddScene(Scene *s, bool setCurrent = false);
	static double GetDeltaTime();
	static double GetTime();
	static void Log(string message);
	const Vector2i size;
private:
	vector<Scene*> scenes;
	RenderWindow window;
	static Clock clk;
	static void SetDeltaTime(double deltaTime);
	static double deltaTime;
	static Font font;
	//log
	static Text log;
};