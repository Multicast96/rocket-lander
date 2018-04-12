#pragma once
#include"../Scenes/Scene.h"
#include"../common.h"

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
	static Font & getFont(Fonts option);
	static Vector2i getSize();
	static double smallFontSize;
	static double normalFontSize;
	static double bigFontSize;

	class GUI : public Drawable {
	public:
		static enum PLACEHOLDER {
			UPPER_LEFT1,
			UPPER_LEFT2,
			UPPER_RIGHT1,
			UPPER_RIGHT2,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
		};
		GUI();
		GUI(sf::Vector2f sc);
		void draw(RenderTarget &target, RenderStates state)const;
		sf::Text texts[6];
	private:
		sf::Vector2f screen;
	};
	static void displayOnGUI(std::string message , GUI::PLACEHOLDER place);
private:
	vector<Scene*> scenes;
	RenderWindow window;
	static Vector2i size;
	static Clock clk;
	static void SetDeltaTime(double deltaTime);
	static double deltaTime;
	static Font global_font, secondary_font;
	static GUI gui;
};