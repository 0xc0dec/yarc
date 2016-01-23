
/**
 * This file is part of YetAnotherRubiksCube game source code
 *
 * Copyright (C) 2010  Aleksey "Adaon" Fedotov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifdef _WINDOWS
#include <windows.h>
#endif

#include "Game.h"


#ifdef _WINDOWS
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpSmdLine, int nShowCmd)
#else
int main()
#endif
{
  Game *app = Game::singletonPtr();
  if (!app->init())
    return 1;
  app->run();
  app->deInit();
	return 0;
}
