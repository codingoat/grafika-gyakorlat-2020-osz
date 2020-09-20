#include <SDL.h>
#include <iostream>
#include <time.h>

void exitProgram()
{
	std::cout << "Kilépéshez nyomj meg egy billentyût..." << std::endl;
	std::cin.get();
}

int main( int argc, char* args[] )
{
	atexit( exitProgram );
	//
	// 1. lépés: inicializáljuk az SDL-t
	//

	// a grafikus alrendszert kapcsoljuk csak be, ha gond van, akkor jelezzük és lépjün ki
	if ( SDL_Init( SDL_INIT_VIDEO ) == -1 )
	{
		// irjuk ki a hibat es terminaljon a program
		std::cout << "[SDL indítása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
		return 1;
	}
			
	//
	// 2. lépés: hozzuk létre az ablakot, amire rajzolni fogunk
	//

	SDL_Window *win = nullptr;
    win = SDL_CreateWindow( "Hello SDL!",				// az ablak fejléce
							100,						// az ablak bal-felsõ sarkának kezdeti X koordinátája
							100,						// az ablak bal-felsõ sarkának kezdeti Y koordinátája
							640,						// ablak szélessége
							480,						// és magassága
							SDL_WINDOW_SHOWN);			// megjelenítési tulajdonságok

	// ha nem sikerült létrehozni az ablakot, akkor írjuk ki a hibát, amit kaptunk és lépjünk ki
    if (win == nullptr)
	{
		std::cout << "[Ablak létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 3. lépés: hozzunk létre egy renderelõt, rajzolót
	//

    SDL_Renderer *ren = nullptr;
    ren = SDL_CreateRenderer(	win, // melyik ablakhoz rendeljük hozzá a renderert
								-1,  // melyik indexú renderert inicializáljuka
									 // a -1 a harmadik paraméterben meghatározott igényeinknek megfelelõ elsõ renderelõt jelenti
								SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);	// az igényeink, azaz
																						// hardveresen gyorsított és vsync-et beváró
    if (ren == nullptr)
	{
        std::cout << "[Renderer létrehozása]Hiba az SDL inicializálása közben: " << SDL_GetError() << std::endl;
        return 1;
    }

	//
	// 4. lépés: indítsuk el a fõ üzenetfeldolgozó ciklust
	// 

	// véget kell-e érjen a program futása?
	bool quit = false;
	// feldolgozandó üzenet ide kerül
	SDL_Event ev;
	// egér X és Y koordinátái
	Sint32 mouseX = 0, mouseY = 0;

	float cursorColor[] = { 0, 255, 255 };
	srand(time(nullptr));

	while (!quit)
	{
		// amíg van feldolgozandó üzenet dolgozzuk fel mindet:
		while ( SDL_PollEvent(&ev) )
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_KEYDOWN:
				if (ev.key.keysym.sym == SDLK_ESCAPE)
				{
					quit = true;
				}
				else if (ev.key.keysym.sym == SDLK_SPACE)
				{
					cursorColor[0] = rand() % 256;
					cursorColor[1] = rand() % 256;
					cursorColor[2] = rand() % 256;
				}

				break;
			case SDL_MOUSEMOTION:
				mouseX = ev.motion.x;
				mouseY = ev.motion.y;
				break;
			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_LEFT)
				{
					cursorColor[0] = 255;
					cursorColor[1] = cursorColor[2] = 0;
				}
				else if (ev.button.button == SDL_BUTTON_RIGHT)
				{
					cursorColor[2] = 255;
					cursorColor[0] = cursorColor[1] = 0;
				}
				// egérgomb felengedésének eseménye; a felengedett gomb a ev.button.button -ban található
				// a lehetséges gombok a következõek: SDL_BUTTON_LEFT, SDL_BUTTON_MIDDLE, 
				//		SDL_BUTTON_RIGHT, SDL_BUTTON_WHEELUP, SDL_BUTTON_WHEELDOWN
				break;
			}
		}

		/*
		// töröljük a hátteret fehérre
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		// aktuális rajzolási szín legyen zöld és rajzoljunk ki egy vonalat
		SDL_SetRenderDrawColor(	ren,	// renderer címe, aminek a rajzolási színét be akarjuk állítani
								0,		// piros
								255,	// zöld
								0,		// kék
								255);	// átlátszatlanság

		SDL_RenderDrawLine(	ren,	// renderer címe, ahová vonalat akarunk rajzolni
							0, 0, // vonal kezdõpontjának (x,y) koordinátái
							mouseX, mouseY);// vonal végpontjának (x,y) koordinátái
		
		// definiáljunk egy (mouseX, mouseY) középpontó, tengelyekkel párhuzamos oldalú
		// 20x20-as négyzetet:
		SDL_SetRenderDrawColor(ren,
			cursorColor[0],
			cursorColor[1],
			cursorColor[2],
			255);

		SDL_Rect cursor_rect;
		cursor_rect.x = mouseX - 10;
		cursor_rect.y = mouseY - 10;
		cursor_rect.w = 20 * abs(sin(SDL_GetTicks() / 1000.0f * M_PI));
		cursor_rect.h = 20 * abs(sin(SDL_GetTicks() / 1000.0f * M_PI));
		// legyen a kitöltési szín piros
		//SDL_SetRenderDrawColor( ren, 255, 0, 0, 255 );
		SDL_RenderFillRect( ren, &cursor_rect);

		// 1. feladat: az eltelt idõ függvényében periodikusan nõjjön és csökkenjen
		//    az egérmutató középpontjával kirajzolt négyszög

		//std::cout << SDL_GetTicks() << std::endl;

		// 2. feladat: ha a user a bal egérgombot nyomja meg akkor a téglalap színe váltson pirosra,
		//    ha a jobb egérgombot, akkor kékre



		// 3. feladat: rajzolj ki egy 50 sugarú körvonalat az egérmutató köré!
		// segítség: használd a SDL_RenderDrawLines()-t

		SDL_Point points[30];
		int r = 50;
		for (int i = 0; i < 30; i++)
		{
			float a = M_PI * 2 / 29 * i;
			points[i] = { mouseX + (int)(r * cosf(a)), mouseY + (int)(r * sinf(a)) };
		}

		SDL_RenderDrawLines(ren, points, 30);
		*/


		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);

		int w = 640;
		int h = 480;
		int size = 150;
		int offsetAmount = 30;

		SDL_Point offset = { (mouseX / (float)w - 0.5f) * 2 * offsetAmount, (mouseY / (float)h - 0.5f) * 2 * offsetAmount };

		SDL_Point points[] = { {(w - size) / 2 + offset.x, (h - size) / 2 + offset.y},
								{(w - size) / 2 + size + offset.x, (h - size) / 2 + offset.y},
								{(w - size) / 2 + size + offset.x, (h - size) / 2 + size + offset.y},
								{(w - size) / 2 + offset.x, (h - size) / 2 + size + offset.y},

								{(w - size) / 2 - offset.x, (h - size) / 2 - offset.y},
								{(w - size) / 2 + size - offset.x, (h - size) / 2 - offset.y},
								{(w - size) / 2 + size - offset.x, (h - size) / 2 + size - offset.y},
								{(w - size) / 2 - offset.x, (h - size) / 2 + size - offset.y}
		};

		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);

		SDL_RenderDrawLines(ren, points, 4);
		SDL_RenderDrawLine(ren, points[3].x, points[3].y, points[0].x, points[0].y);

		SDL_RenderDrawLines(ren, points + 4, 4);
		SDL_RenderDrawLine(ren, points[7].x, points[7].y, points[4].x, points[4].y);

		for (int i = 0; i < 4; i++)
		{
			SDL_RenderDrawLine(ren, points[0 + i].x, points[0 + i].y, points[4 + i].x, points[4 + i].y);
		}


		// jelenítsük meg a backbuffer tartalmát
		SDL_RenderPresent(ren);
	}



	//
	// 4. lépés: lépjünk ki
	// 

	SDL_DestroyRenderer( ren );
	SDL_DestroyWindow( win );

	SDL_Quit();

	return 0;
}