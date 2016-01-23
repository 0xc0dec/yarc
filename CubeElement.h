
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

#ifndef _CUBE_ELEMENT
#define _CUBE_ELEMENT

#include <Ogre.h>

#include "DiscreteVector.h"


class CubeElement
{
public:
  /// Holds scene node represented by this element
  Ogre::SceneNode *node;
  /// Holds element position
  DiscreteVector pos;
  /// Indicates if this element is pivot. Used for cube save/load operation
  bool isPivot;
  /// If pivot, this holds pivot array number (of 4)
  //int pivotArrayNumber;
  int pivotArrays[2];
  /// If pivot, this holds index in pivot array
  //int pivotArrayIndex;
  int pivotArrayIndexes[2];

  CubeElement(Ogre::SceneNode *_node, int i, int j, int k):
    node(_node), pos(i, j, k), isPivot(false)
  {
    pivotArrays[0] = pivotArrays[1] = -1;
    pivotArrayIndexes[0] = pivotArrayIndexes[1] = -1;
  }
};



#endif
