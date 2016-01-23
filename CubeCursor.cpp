
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

#include <math.h>

#include "Cube.h"
#include "CubeCursor.h"


CubeCursor::CubeCursor(Cube *cube): cube(cube)
{
  // Rotation defaults
  rotActive = false;
  rotAngle = 0.0;
  rotDir = Cube::DIR_CW;
  rotSpeed = 0.0;

  doCheck = false;

  cursor = cube->smgr->getManualObject("Cursor");
  for (int i = 0; i < 3; ++i)
    cursor->setMaterialName(i, cube->material + "_cursor");
  node = cube->cubeRootNode->createChildSceneNode();
  node->translate(-cube->visualSize / 2.0 + cube->elemVisualSize / 2.0, 0.0, 0.0, Ogre::SceneNode::TS_PARENT);
  node->attachObject(cursor);
  node->setScale(cube->elemVisualSize + 5.0, (float)cube->size * (cube->elemVisualSize + cube->elemSpacing) + 5.0,
      (float)cube->size * (cube->elemVisualSize + cube->elemSpacing) + 5.0);
  axis = Ogre::Vector3::UNIT_X;
  index = 0;
}



void CubeCursor::slide(int offset)
{
  if (index + offset >= 0 && index + offset < cube->size)
  {
    index += offset;
    // Always move cursor along its local X axis
    node->translate(Ogre::Vector3::UNIT_X * offset * (cube->elemVisualSize + cube->elemSpacing), Ogre::SceneNode::TS_LOCAL);
  }
}



void CubeCursor::setAxis(bool next)
{
  // Set new axis
  if (axis == Ogre::Vector3::UNIT_X)
    axis = next ? Ogre::Vector3::UNIT_Y : Ogre::Vector3::UNIT_Z;
  else if (axis == Ogre::Vector3::UNIT_Y)
    axis = next ? Ogre::Vector3::UNIT_Z : Ogre::Vector3::UNIT_X;
  else if (axis == Ogre::Vector3::UNIT_Z)
    axis = next ? Ogre::Vector3::UNIT_X : Ogre::Vector3::UNIT_Y;

  // Reset orientation. For X new axis this is the correct one
  node->setOrientation(Ogre::Quaternion::IDENTITY);

  // Apply corresponding transforms. Only for Y and Z
  if (axis == Ogre::Vector3::UNIT_Y)
    node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(90.0)), Ogre::SceneNode::TS_PARENT);
  else if (axis == Ogre::Vector3::UNIT_Z)
    node->rotate(Ogre::Vector3::UNIT_Y, -Ogre::Radian(Ogre::Degree(90.0)), Ogre::SceneNode::TS_PARENT);

  // Position to the middle
  index = cube->size / 2;

  // Adjust position
  node->setPosition(Ogre::Vector3::ZERO);
  node->translate(
      (-cube->visualSize + cube->elemVisualSize) / 2.0 + (float)index * (cube->elemVisualSize + cube->elemSpacing),
      0.0, 0.0, Ogre::SceneNode::TS_LOCAL);
}



void CubeCursor::setAxis(Ogre::Vector3 axis)
{
  this->axis = axis;

  // Reset orientation. For X new axis this is the correct one
  node->setOrientation(Ogre::Quaternion::IDENTITY);

  // Apply corresponding transforms. Only for Y and Z
  if (axis == Ogre::Vector3::UNIT_Y)
    node->rotate(Ogre::Vector3::UNIT_Z, Ogre::Radian(Ogre::Degree(90.0)), Ogre::SceneNode::TS_PARENT);
  else if (axis == Ogre::Vector3::UNIT_Z)
    node->rotate(Ogre::Vector3::UNIT_Y, -Ogre::Radian(Ogre::Degree(90.0)), Ogre::SceneNode::TS_PARENT);

  // Adjust position
  node->setPosition(Ogre::Vector3::ZERO);
  node->translate(
      (-cube->visualSize + cube->elemVisualSize) / 2.0 + (float)index * (cube->elemVisualSize + cube->elemSpacing),
      0.0, 0.0, Ogre::SceneNode::TS_LOCAL);
}



void CubeCursor::setPosition(Ogre::Vector3 axis, int index)
{
  setAxis(axis);
  slide(index - this->index);
}



void CubeCursor::rotate(Cube::Direction dir, float speed, bool checkAfter)
{
  rotDir = dir;
  rotAngle = 0.0;
  rotSpeed = speed;
  rotActive = true;

  doCheck = checkAfter;

  rotNode = cube->cubeRootNode->createChildSceneNode("RotationNode");

  int size = cube->size;

  CubeElement ***tmp = new CubeElement**[size];
  for (int i = 0; i < size; ++i)
    tmp[i] = new CubeElement*[size];

  if (axis == Ogre::Vector3::UNIT_X)
  {
    for (int j = 0; j < size; ++j)
      for (int k = 0; k < size; ++k)
      {
        cube->cubeRootNode->removeChild(cube->data[index][j][k]->node);
        rotNode->addChild(cube->data[index][j][k]->node);
        if (dir == Cube::DIR_CCW)
        {
          tmp[k][size - 1 - j] = cube->data[index][j][k];
          cube->data[index][j][k]->pos.j = k;
          cube->data[index][j][k]->pos.k = size - 1 - j;
        }
        else if (dir == Cube::DIR_CW)
        {
          tmp[size - 1 - k][j] = cube->data[index][j][k];
          cube->data[index][j][k]->pos.j = size - 1 - k;
          cube->data[index][j][k]->pos.k = j;
        }
      }
    for (int j = 0; j < size; ++j)
      for (int k = 0; k < size; ++k)
        cube->data[index][j][k] = tmp[j][k];
  }

  else if (axis == Ogre::Vector3::UNIT_Y)
  {
    for (int i = 0; i < size; ++i)
      for (int k = 0; k < size; ++k)
      {
        cube->cubeRootNode->removeChild(cube->data[i][index][k]->node);
        rotNode->addChild(cube->data[i][index][k]->node);
        if (dir == Cube::DIR_CW)
        {
          tmp[k][size - 1 - i] = cube->data[i][index][k];
          cube->data[i][index][k]->pos.i = k;
          cube->data[i][index][k]->pos.k = size - 1 - i;
        }
        else if (dir == Cube::DIR_CCW)
        {
          tmp[size - 1 - k][i] = cube->data[i][index][k];
          cube->data[i][index][k]->pos.i = size - 1 - k;
          cube->data[i][index][k]->pos.k = i;
        }
      }
    for (int i = 0; i < size; ++i)
      for (int k = 0; k < size; ++k)
        cube->data[i][index][k] = tmp[i][k];
  }

  else if (axis == Ogre::Vector3::UNIT_Z)
  {
    for (int i = 0; i < size; ++i)
      for (int j = 0; j < size; ++j)
      {
        cube->cubeRootNode->removeChild(cube->data[i][j][index]->node);
        rotNode->addChild(cube->data[i][j][index]->node);
        if (dir == Cube::DIR_CCW)
        {
          tmp[j][size - 1 - i] = cube->data[i][j][index];
          cube->data[i][j][index]->pos.i = j;
          cube->data[i][j][index]->pos.j = size - 1 - i;
        }
        else if (dir == Cube::DIR_CW)
        {
          tmp[size - 1 - j][i] = cube->data[i][j][index];
          cube->data[i][j][index]->pos.i = size - 1 - j;
          cube->data[i][j][index]->pos.j = i;
        }
      }
    for (int i = 0; i < size; ++i)
      for (int j = 0; j < size; ++j)
        cube->data[i][j][index] = tmp[i][j];
  }

  for (int i = 0; i < size; ++i)
    delete [] tmp[i];
  delete [] tmp;
}



void CubeCursor::animateRotation(float timeStep)
{
  if (rotActive)
  {
    float delta = rotSpeed * timeStep;
    if (rotAngle + delta >= 90.0)
    {
      delta = 90.0 - rotAngle;
      rotAngle = 0.0;
      rotActive = false;
    }
    // Perform animation
    rotNode->rotate(axis, Ogre::Radian(Ogre::Degree(rotDir * delta)), Ogre::SceneNode::TS_PARENT);
    // If this was the last animation step - detach elements
    if (!rotActive)
    {
      // Detach all elements
      while (rotNode->numChildren() > 0)
      {
        Ogre::SceneNode *tmp = dynamic_cast<Ogre::SceneNode*>(rotNode->getChild(0));
        Ogre::Matrix4 m1(rotNode->getOrientation());
        Ogre::Matrix4 m2(tmp->getOrientation());
        m1.setTrans(rotNode->getPosition());
        m2.setTrans(tmp->getPosition());
        m2 = m1 * m2;
        rotNode->removeChild(tmp);
        cube->cubeRootNode->addChild(tmp);
        tmp->setPosition(m2.getTrans());
        tmp->setOrientation(m2.extractQuaternion());
      }
      cube->cubeRootNode->removeAndDestroyChild("RotationNode");
      // Check if the cube is assembled
      if (doCheck)
        cube->check();
    }
    else
      rotAngle += delta;
  }
}
