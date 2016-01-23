
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

#ifndef _GUI_MANAGER_H_
#define _GUI_MANAGER_H_

#include <Ogre.h>

#include "SDLInput.h"

class Content;


class GUIManager: public KeyboardListener
{
private:
  Content *content;
  Ogre::OverlayManager *ovl_mgr;
  Ogre::Overlay *overlay;
  Ogre::OverlayContainer *root_panel;

  /// Stores if the menu is currently visible
  bool _menuVisible;

private:
  void onKeyboardEvent(const SDL_KeyboardEvent &evt);
  /// Toggles menu visibility
  void _toggleMenu();

public:
  GUIManager(Content *content);
  ~GUIManager();

  /// Shows win tip
  void showWinTip(bool show);
};


#endif
