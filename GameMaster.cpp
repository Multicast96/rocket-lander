#include"GameMaster.h"

double GameMaster::deltaTime = 0;
Font GameMaster::font = Font();
double GameMaster::GetDeltaTime(){ return deltaTime; }
void GameMaster::SetDeltaTime(double dTime) { deltaTime = dTime;}
Font& GameMaster::getFont() { return font; }
Clock GameMaster::clk = Clock();
Vector2i GameMaster::size = Vector2i(0, 0);
Vector2i GameMaster::getSize() { return size; }

double GameMaster::smallFontSize = 0.03; //procent wysokoœci ekranu
double GameMaster::normalFontSize = 0.05;
double GameMaster::bigFontSize = 0.08;

GameMaster::GUI GameMaster::gui = GUI();

void GameMaster::setGUIText(std::string message, GUI::PLACEHOLDER place) {
	gui.texts[place].setString(message);
}


GameMaster::GameMaster(Vector2i size) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	GameMaster::size = size;
	window.create(VideoMode(size.x, size.y), "Rock4et Lander", Style::Default, settings);
	window.setFramerateLimit(60);
	currentScene = NULL;
	smallFontSize *= size.y;
	normalFontSize *= size.y;
	bigFontSize *= size.y;
	
	//Global font
	if (!font.loadFromFile("assets/resourcess/sansation.ttf")) {
		throw std::runtime_error("No font file");
	}

	gui = GUI((sf::Vector2f)size);
	setGUIText("Rocket Lander", GUI::UPPER_LEFT1);
	setGUIText("Alpha 0.0.3", GUI::BOTTOM_LEFT);
}

//setCurrent is false by default
void GameMaster::AddScene(Scene *s, bool setCurrent) { 
	scenes.push_back(s);
	if (setCurrent) {
		currentScene = s;
		setGUIText(s->getName(), GUI::UPPER_LEFT2);
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
					currentScene->Add(new RocketPlayer(Vector2f(size.x / 2, size.y / 2)));
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