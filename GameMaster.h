#pragma once
#include"Scene.h"
#include"common.h"

using namespace std;

class Scene;

class GameMaster {
public:
	Scene* currentScene; //tymczasowo publiczne

	GameMaster(Vector2i size);
	void MainLoop();
	void GameMaster::AddScene(Scene *s, bool setCurrent = false);
	static double GetDeltaTime();
	static double GetTime();
	static void Log(string message, int line);
	static Font& getFont();
	static Vector2i getSize();
private:
	vector<Scene*> scenes;
	RenderWindow window;
	static Vector2i size;
	static Clock clk;
	static void SetDeltaTime(double deltaTime);
	static double deltaTime;
	static Font font;
	//log
	static Text log;
};