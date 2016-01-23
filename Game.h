
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

#ifndef _GAME_H_
#define _GAME_H_

#include <list>
#include <Ogre.h>

#include "ConfigManager.h"
#include "SDLInput.h"



class Game: public Ogre::FrameListener, public KeyboardListener, public EventListener
{
private:
  /// Ogre
  Ogre::Root *root;
  Ogre::RenderWindow *wnd;

  /// Input
  InputManager *imgr;

  /// Config manager
  ConfigManager config;

  /// Indicates if application should stop
  bool stop;

private:
  /*
   * Events processing
   */
  /// System events processing
  void onEvent(const SDL_Event &evt);
  /// Keyboard events processing
  void onKeyboardEvent(const SDL_KeyboardEvent &evt);

  /// ctor
  Game(): stop(false) {}
  /// dtor
  ~Game() {}

public:
  /// Returns Ogre Root object
  Ogre::Root *getRoot() const { return root; }
  /// Returns render window
  Ogre::RenderWindow *getWindow() const { return wnd; }

  /// Returns input manager
  InputManager *getInputManager() const { return imgr; }

  /// Initialises application
  bool init();
  /// Frees resources
  void deInit();
  /// Starts application execution
  void run();

  /// Returns single allowed copy of Game object
  static Game* singletonPtr()
  {
    static Game app;
    return &app;
  }
};



#endif
