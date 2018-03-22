#include"GameMaster.h"
#include<Windows.h>
#include<string>

using namespace sf;

int main()
{
	try {
		GameMaster master(Vector2i(1280,720));
		RocketPlayer r1(Vector2f(master.size.x/2, master.size.y/2));
		Platform p1(Vector2f(1280 / 2, 720 - Platform::size.y));


		Scene s1(Scene::MENU);
		s1.Add(&r1);
		s1.Add(&p1);

		master.AddScene(&s1,true);
		master.MainLoop();
	}
	catch (exception &e) {
		MessageBoxA(NULL, (LPCSTR)e.what(), (LPCSTR)":(",0);
	}
}