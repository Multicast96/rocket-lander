#include"GameMaster.h"

double GameMaster::deltaTime = 0;
Font GameMaster::global_font = Font();
Font GameMaster::secondary_font = Font();

double GameMaster::GetDeltaTime(){ return deltaTime; }
void GameMaster::SetDeltaTime(double dTime) { deltaTime = dTime;}
Font& GameMaster::getFont(Fonts option) {
	switch (option)
	{
	case SPACE :
		return secondary_font;
	case MAIN:
		return global_font;
	default:
		return global_font;
	}
}
Clock GameMaster::clk = Clock();
Vector2i GameMaster::size = Vector2i(0, 0);
Vector2i GameMaster::getSize() { return size; }

double GameMaster::smallFontSize = 0.03; //procent wysokoœci ekranu
double GameMaster::normalFontSize = 0.05;
double GameMaster::bigFontSize = 0.08;

GameMaster::GUI GameMaster::gui = GUI();

void GameMaster::displayOnGUI(std::string message, GUI::PLACEHOLDER place) {
	gui.texts[place].setString(message);
}


GameMaster::GameMaster(Vector2i size) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	GameMaster::size = size;
	window.create(VideoMode(size.x, size.y), "Rocket Lander", Style::Default, settings);
	window.setFramerateLimit(60);
	currentScene = NULL;
	smallFontSize *= size.y;
	normalFontSize *= size.y;
	bigFontSize *= size.y;
	
	//Global font
	if (!global_font.loadFromFile("assets/resourcess/sansation.ttf")) {
		throw std::runtime_error("No sansation.ttf font file");
	}

	//Secondary font
	if (!secondary_font.loadFromFile("assets/resourcess/space-age.regular.ttf")) {
		throw std::runtime_error("No space-age.regular.ttf font file");
	}

	gui = GUI((sf::Vector2f)size);
	displayOnGUI("Rocket Lander", GUI::UPPER_LEFT1);
	displayOnGUI("Alpha 0.0.3", GUI::BOTTOM_LEFT); 
}

//setCurrent is false by default
void GameMaster::AddScene(Scene *s, bool setCurrent) { 
	scenes.push_back(s);
	if (setCurrent) {
		currentScene = s;
		displayOnGUI(s->getName(), GUI::UPPER_LEFT2);
	}
}

double GameMaster::GetTime() {
	return clk.getElapsedTime().asSeconds();
}

void GameMaster::MainLoop() {	
	GameMaster::SetDeltaTime(clk.getElapsedTime().asSeconds());
	double lastFrame = clk.getElapsedTime().asSeconds();

	while (window.isOpen()){
		GameMaster::SetDeltaTime(clk.getElapsedTime().asSeconds() - lastFrame);
		lastFrame = clk.getElapsedTime().asSeconds();

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::Space)) {
					dynamic_cast<AItraining*>(currentScene)->spawnRockets(1);
				}
			}
		}

		//todo
		if(Keyboard::isKeyPressed(Keyboard::Escape))
			window.close();

		window.clear(Color::White);
		currentScene->Action();
		window.draw(*currentScene);
		window.draw(gui);
		window.display();
	}
}