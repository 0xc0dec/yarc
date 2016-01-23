
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

#ifndef _CUBE_CURSOR_H_
#define _CUBE_CURSOR_H_

#include <Ogre.h>

#include "Cube.h"


/**
 * Only Cube has access to it
 */
class CubeCursor
{
  friend class Cube;
private:
  /// Pointer to cube
  Cube *cube;
  /// Scene node cursor attached to
  Ogre::SceneNode *node;
  /// Scene node used for elements rotation
  Ogre::SceneNode *rotNode;
  /// Graphical representation
  Ogre::ManualObject *cursor;

  /// Current cursor axis
  Ogre::Vector3 axis;
  /// Index
  int index;

  /**
   * Rotation info
   */
  /// If currently rotating
  bool rotActive;
  /// If perform check for cube assembly after animation
  bool doCheck;
  /// Rotation speed
  float rotSpeed;
  /// Rotation direction
  Cube::Direction rotDir;
  /// Current rotation angle
  float rotAngle;

private:
  CubeCursor(Cube *cube);
  ~CubeCursor() {}

  /// Moves cursor along current axis
  void slide(int offset);
  /// Changes cursor axis to the next or previous one
  void setAxis(bool next);
  /// Changes cursor axis directly to the given one
  void setAxis(Ogre::Vector3 axis);

  /// Sets cursor position and axis
  void setPosition(Ogre::Vector3 axis, int index);

  /// Returns cursor axis
  Ogre::Vector3 getAxis() const { return axis; }

  /// Starts rotation
  void rotate(Cube::Direction dir, float speed, bool checkAfter);

  /// Performs elements rotation animation
  void animateRotation(float timeStep);
};



#endif
