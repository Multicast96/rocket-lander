#include"GameMaster.h"
#include"common.h"
#include<Windows.h>

int main()
{
	try {
		srand(time(NULL));
		GameMaster master(Vector2i(1280,720));

		AItraining s1(Scene::SCENE_NAME::AI_TRAINING);
		s1.Add(new Background((Vector2f)master.getSize() * 0.5,  (Vector2f)master.getSize()));
		s1.Add(new Wave(Vector2f(1280 / 2, 720 - 60), sf::Color(6, 36, 90), true, Vector2f(1280, 150)));
		s1.AddPlatform(new Platform(Vector2f(GameMaster::getSize().x / 2.0, GameMaster::getSize().y), Vector2f(300, 175)));
		s1.Add(new Wave(Vector2f(1280/2, 720-35),sf::Color(41,90,198),false,Vector2f(1280, 150)));
		s1.Add(new Wave(Vector2f(1280 / 2, 720-10), sf::Color(47, 100, 217), true, Vector2f(1280, 150)));

		master.AddScene(&s1,true);
		master.MainLoop();
	}
	catch (std::exception &e) {
		MessageBoxA(NULL, (LPCSTR)e.what(), (LPCSTR)"Wyst¹pi³ problem :(",0);
	}
}