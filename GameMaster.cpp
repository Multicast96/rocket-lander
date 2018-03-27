#include"GameMaster.h"

double GameMaster::deltaTime = 0;
Text GameMaster::log = Text();
Font GameMaster::font = Font();
double GameMaster::GetDeltaTime(){ return deltaTime; }
void GameMaster::SetDeltaTime(double dTime) { deltaTime = dTime;}
Font& GameMaster::getFont() { return font; }
Clock GameMaster::clk = Clock();
Vector2i GameMaster::size = Vector2i(0, 0);
Vector2i GameMaster::getSize() { return size; }


GameMaster::GameMaster(Vector2i size) {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	GameMaster::size = size;
	window.create(VideoMode(size.x, size.y), "Rocket Lander", Style::Default, settings);
	window.setFramerateLimit(60);
	currentScene = NULL;
	
	//Global font
	if (!font.loadFromFile("assets/resourcess/sansation.ttf")) {
		throw std::runtime_error("No font file");
	}

	log.setCharacterSize(size.y*0.05);
	log.setPosition(0, 0);
	log.setFont(font);
	log.setFillColor(Color::Black);
}

//setCurrent is false by default
void GameMaster::AddScene(Scene *s, bool setCurrent) { 
	scenes.push_back(s);
	currentScene = s;
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
		window.draw(log);
		window.display();
	}
}

void GameMaster::Log(string message, int line) {
	log.setString(message);
}