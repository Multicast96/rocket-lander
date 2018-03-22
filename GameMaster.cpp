#include"GameMaster.h"

double GameMaster::deltaTime = 0;
Text GameMaster::log = Text();
Font GameMaster::font = Font();
double GameMaster::GetDeltaTime(){ return deltaTime; }
void GameMaster::SetDeltaTime(double dTime) { deltaTime = dTime;}
Clock GameMaster::clk = Clock();


GameMaster::GameMaster(Vector2i size) : size(size) {
	window.create(VideoMode(size.x, size.y), "Rocket Lander");
	currentScene = NULL;
	
	//Global font
	if (!font.loadFromFile("sansation.ttf")) {
		throw std::runtime_error("No font file");
	}

	
	log.setCharacterSize(size.y*0.05);
	log.setPosition(0, size.y*0.05);
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

void GameMaster::Log(string message) { log.setString(message); }