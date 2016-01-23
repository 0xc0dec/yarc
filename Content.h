
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

#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <Ogre.h>

#include "GUIManager.h"
#include "Cube.h"



class Content
{
private:
  /// Ogre scene manager
  Ogre::SceneManager *smgr;
  /// User interface manager
  GUIManager *gui_mgr;

  /// Cube itself
  Cube *cube;

private:
  /// Creates scene
  void createScene();

  /// Creates meshes for the game (elements, cursor)
  void _createMeshes() const;

public:
  Content();
  ~Content();

  /// Returns Ogre scene manager
  Ogre::SceneManager *getSceneManager() const { return smgr; }
  /// Returns GUI manager
  GUIManager *getGUIManager() const { return gui_mgr; }
  /// Returns cube
  Cube *getCube() const { return cube; }
};



#endif
