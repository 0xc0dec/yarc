
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

#ifndef _CUBE_H_
#define _CUBE_H_

#include <vector>

#include <Ogre.h>

#include "ColorGenerator.h"
#include "CubeElement.h"
#include "DiscreteVector.h"
#include "SDLInput.h"

// Forward
class Content;
class CubeCursor;


class Cube: public Ogre::FrameListener, public KeyboardListener
{
  friend class CubeCursor;
private:

  /// Rotation direction
  enum Direction
  {
    DIR_CW = 1,
    DIR_CCW = -1
  };

  typedef std::vector<CubeElement*> ElementsCollection1D;
  typedef std::vector<ElementsCollection1D> ElementsCollection2D;
  typedef std::vector<ElementsCollection2D> ElementsCollection3D;

private:
  /// The content that created the cube
  Content *content;
  Ogre::SceneManager *smgr;

  /// Cube size logical size
  int size;
  /// Cube visual size - side length
  float visualSize;
  /// Each element size
  float elemVisualSize;
  /// Spacing between elements
  float elemSpacing;

  /// Holds orientation constraint angle around X global axis
  Ogre::Radian rotConstraint;

  /// Cube view rotation speed
  float viewRotSpeed;
  /// View rotation mode
  bool viewRotation;

  /// Random counter. Used for randomization
  int rndCounter;

  /// Root cube node, holding all entities
  Ogre::SceneNode *cubeRootNode;
  /// Hols elements configuration
  ElementsCollection3D data;

  /**
   * Stores pointers to pivot elements.
   * 0: from (0, 0, 0) to (0, size - 1, 0);
   * 1: from (0, size - 1, 0) to (size - 1, size - 1, size - 1)
   * 2: from (0, 0, 0) to (size - 1, 0, size - 1);
   * 3: from (size - 1, 0, size - 1) to (size - 1, size - 1, size - 1);
   * Used for checking
   */
  ElementsCollection1D pivots[4];

  /// Current model used to represent elements
  Ogre::String modelMesh;
  /// Current material set used for elements
  Ogre::String material;

  /// Cursor
  CubeCursor *cursor;

private:
  /// Keyboard processing
  void onKeyboardEvent(const SDL_KeyboardEvent &evt);

  /// Frame processing
  bool frameStarted(const Ogre::FrameEvent &evt);

  /// Creates assembled cube
  void init(int size);

  /// Checks if the cube is assembled
  void check();

  /**
   * Performs cube cleanup, i.e. removes all nodes and entities
   * Used by reset() and load()
   */
  void clear();

public:
  Cube(Content *content, Ogre::String modelMesh, Ogre::String material, int size, float visualSize, float elemSpacing,
      float viewRotSpeed, float animSpeed);
  ~Cube();

  /// Returns current cube size
  int getSize() const { return size; }

  /// Creates new cube with the given size
  void reset(int size);
  /// Randomizes cube
  void randomize();

  /// Saves cube state to file
  void save(Ogre::String file);
  /// Loads cube state from file
  bool load(Ogre::String file);
};



#endif
