#include"GameMaster.h"
#include"common.h"

int main()
{
	try {
		GameMaster master(Vector2i(1280,720));

		AItraining s1(Scene::SCENE_NAME::MENU);
		s1.Add(new Background((Vector2f)master.getSize() * 0.5,  (Vector2f)master.getSize()));
		s1.Add(new RocketPlayer(Vector2f(GameMaster::getSize().x/2, GameMaster::getSize().y / 2)));
		s1.AddPlatform(new Platform(Vector2f(GameMaster::getSize().x / 2.0, GameMaster::getSize().y)));

		master.AddScene(&s1,true);
		master.MainLoop();
	}
	catch (std::exception &e) {
		MessageBoxA(NULL, (LPCSTR)e.what(), (LPCSTR)"Wyst¹pi³ problem :(",0);
	}
}