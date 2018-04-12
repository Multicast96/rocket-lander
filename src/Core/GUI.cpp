#include"GameMaster.h"


GameMaster::GUI::GUI() {}
GameMaster::GUI::GUI(sf::Vector2f sc) : screen(sc) {
	for (int i = 0; i < 6; i++) {
		texts[i] = Text();
		switch (i) {
		case UPPER_LEFT1:
			texts[i].setCharacterSize(GameMaster::bigFontSize);
			texts[i].setPosition(0, 0);
			texts[i].setFont(GameMaster::getFont(SPACE));
			break;
		case UPPER_LEFT2:
			texts[i].setCharacterSize(GameMaster::normalFontSize);
			texts[i].setPosition(0, texts[UPPER_LEFT1].getCharacterSize());
			texts[i].setFont(GameMaster::getFont(SPACE));
			break;
		case UPPER_RIGHT1:
			texts[i].setCharacterSize(GameMaster::normalFontSize);
			texts[i].setPosition(screen.x*0.75, 0);
			texts[i].setFont(GameMaster::getFont(MAIN));
			break;
		case UPPER_RIGHT2:
			texts[i].setCharacterSize(GameMaster::normalFontSize);
			texts[i].setPosition(texts[UPPER_RIGHT1].getPosition().x, texts[UPPER_RIGHT1].getCharacterSize());
			texts[i].setFont(GameMaster::getFont(MAIN));

			break;
		case BOTTOM_LEFT:
			texts[i].setCharacterSize(GameMaster::smallFontSize);
			texts[i].setPosition(0, screen.y - texts[i].getCharacterSize());
			texts[i].setFont(GameMaster::getFont(SPACE));
			break;
		case BOTTOM_RIGHT:
			texts[i].setCharacterSize(GameMaster::smallFontSize);
			texts[i].setPosition(screen.x*0.75, screen.y - texts[i].getCharacterSize());
			texts[i].setFont(GameMaster::getFont(MAIN));

			break;
		}
		texts[i].setFillColor(sf::Color::White);
		texts[i].setString("");
	}
}
void GameMaster::GUI::draw(RenderTarget &target, RenderStates state)const {
	for (int i = 0; i < 6; i++) {
		target.draw(texts[i]);
	}
}