
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

#include <fstream>
#include <sstream>
#include <stack>
#include <stdlib.h>
#include <time.h>

#include "Game.h"
#include "Content.h"
#include "Cube.h"
#include "CubeCursor.h"
#include "GUIManager.h"




Cube::Cube(Content *_content, Ogre::String _modelMesh, Ogre::String _material, int _size, float _visualSize,
    float _elemSpacing, float _viewRotSpeed, float _animSpeed)
{
  content = _content;
  smgr = content->getSceneManager();
  viewRotSpeed = _viewRotSpeed;
  visualSize = _visualSize;
  elemSpacing = _elemSpacing;
  modelMesh = _modelMesh;
  material = _material;
  viewRotation = false;
  rndCounter = 0;
  cursor = NULL;

  // Create root cube node
  cubeRootNode = smgr->getRootSceneNode()->createChildSceneNode();
  cubeRootNode->rotate(Ogre::Vector3::UNIT_X, Ogre::Radian(Ogre::Degree(30.0)), Ogre::SceneNode::TS_PARENT);
  cubeRootNode->rotate(Ogre::Vector3::UNIT_Y, Ogre::Radian(Ogre::Degree(30.0)), Ogre::SceneNode::TS_LOCAL);

  rotConstraint = Ogre::Radian(Ogre::Degree(30.0));

  // Init cube
  reset(_size);

  // Register for events listening
  Game::singletonPtr()->getRoot()->addFrameListener(this);
  Game::singletonPtr()->getInputManager()->regKeyboardListener(this);
}



Cube::~Cube()
{
  delete cursor;
  Game::singletonPtr()->getRoot()->removeFrameListener(this);
  Game::singletonPtr()->getInputManager()->unregKeyboardListener(this);
}



void Cube::clear()
{
  if (!data.empty())
  {
    for (int i = 0; i < size; ++i)
      for (int j = 0; j < size; ++j)
        for (int k = 0; k < size; ++k)
        {
          Ogre::Entity *ent = dynamic_cast<Ogre::Entity*>(data[i][j][k]->node->getAttachedObject(0));
          data[i][j][k]->node->detachAllObjects();
          smgr->destroyEntity(ent);
          delete data[i][j][k];
        }
    cubeRootNode->removeAndDestroyAllChildren();
    data.clear();
    for (int i = 0; i < 4; ++i)
      pivots[i].clear();
  }
  // Reset cursor
  if (cursor)
    delete cursor;
}



void Cube::reset(int _size)
{
  // Reset only if not currently randomizing
  if (rndCounter == 0)
  {
    // Available sizes - from 3 to 10
    if (_size < 3 || _size > 10)
      return;

    // Clear old data
    clear();

    // Calc new parameters
    size = _size;
    elemVisualSize = (visualSize - (size - 1) * elemSpacing) / (float)size;

    // Create assembled cube
    Ogre::Vector3 pos(-visualSize / 2.0 + elemVisualSize / 2.0);
    std::stack<Ogre::Vector3> tmp;
    data.resize(size);
    for (int i = 0; i < size; ++i) // x
    {
      tmp.push(pos);
      data[i].resize(size);
      for (int j = 0; j < size; ++j) // y
      {
        tmp.push(pos);
        data[i][j].resize(size);
        for (int k = 0; k < size; ++k) // z
        {
          // Create entity
          std::ostringstream name;
          name << "Element_" << i << '.' << j << '.' << k;
          Ogre::Entity * ent = smgr->createEntity(name.str(), modelMesh);

          // Apply materials
          ent->getSubEntity(0)->setMaterialName(i == 0 ? material + "_0" : material + "_blank");        // X min
          ent->getSubEntity(1)->setMaterialName(i == size - 1 ? material + "_1" : material + "_blank"); // X max
          ent->getSubEntity(2)->setMaterialName(j == 0 ? material + "_2" : material + "_blank");        // Y min
          ent->getSubEntity(3)->setMaterialName(j == size - 1 ? material + "_3" : material + "_blank"); // Y max
          ent->getSubEntity(4)->setMaterialName(k == 0 ? material + "_4" : material + "_blank");        // Z min
          ent->getSubEntity(5)->setMaterialName(k == size - 1 ? material + "_5" : material + "_blank"); // Z max

          // Create node
          Ogre::SceneNode *node = cubeRootNode->createChildSceneNode(name.str());
          node->setScale(Ogre::Vector3(elemVisualSize));
          node->setPosition(pos);
          node->attachObject(ent);

          // Save element
          CubeElement *e = new CubeElement(node, i, j, k);
          data[i][j][k] = e;

          // Save pivot points
          if (i == 0 && k == 0)
          {
            pivots[0].push_back(e);
            e->isPivot = true;
            e->pivotArrays[0] = 0;
            e->pivotArrayIndexes[0] = pivots[0].size() - 1;
          }
          if (i == size - 1 && k == size - 1)
          {
            pivots[3].push_back(e);
            e->isPivot = true;
            e->pivotArrays[0] = 3;
            e->pivotArrayIndexes[0] = pivots[3].size() - 1;
          }
          if (i == k)
          {
            if (j == 0)
            {
              pivots[2].push_back(e);
              e->isPivot = true;
              e->pivotArrays[e->pivotArrays[0] != -1 ? 1 : 0] = 2;
              e->pivotArrayIndexes[e->pivotArrayIndexes[0] != -1 ? 1 : 0] = pivots[2].size() - 1;
            }
            else if (j == size - 1)
            {
              pivots[1].push_back(e);
              e->isPivot = true;
              e->pivotArrays[e->pivotArrays[0] != -1 ? 1 : 0] = 1;
              e->pivotArrayIndexes[e->pivotArrayIndexes[0] != -1 ? 1 : 0] = pivots[1].size() - 1;
            }
          }
          // Increment position
          pos.z += elemSpacing + elemVisualSize;
        }
        pos = tmp.top();
        pos.y += elemSpacing + elemVisualSize;
        tmp.pop();
      }
      pos = tmp.top();
      pos.x += elemSpacing + elemVisualSize;
      tmp.pop();
    }
    cursor = new CubeCursor(this);
  }
}



void Cube::randomize()
{
  srand(time(NULL));
  rndCounter = 33; // why 33?
}



void Cube::check()
{
  // Calculate directions
  DiscreteVector dirs[4];
  for (int i = 0; i < 4; ++i)
  {
    dirs[i] = pivots[i][size - 1]->pos - pivots[i][0]->pos;
    dirs[i].normalize();
  }
  // Move along directions and check pivot points
  for (int i = 0; i < size; ++i)
  {
    DiscreteVector tmp;
    for (int j = 0; j < 4; ++j)
    {
      tmp = pivots[j][0]->pos + dirs[j] * i;
      if (data[tmp.i][tmp.j][tmp.k] != pivots[j][i])
        return;
    }
  }
  content->getGUIManager()->showWinTip(true);
}



void Cube::save(Ogre::String file)
{
  if (rndCounter == 0)
  {
    std::ofstream output(file.c_str());

    // Write cube size
    output << size << std::endl;

    for (int i = 0; i < size; ++i)
      for (int j = 0; j < size; ++j)
        for (int k = 0; k < size; ++k)
        {
          CubeElement *e = data[i][j][k];
          Ogre::Entity *ent = static_cast<Ogre::Entity*>(e->node->getAttachedObject(0));
          Ogre::SceneNode *node = e->node;
          // Write name, indexes and position
          output << node->getName() << "\n"
                 << "\t" << i << " " << j << " " << k << "\n" // index in data array
                 << "\t" << node->getPosition().x << " " << node->getPosition().y << " " << node->getPosition().z << "\n"; // position
          // Write orientation
          Ogre::Vector3 orient_axis;
          Ogre::Degree orient_angle;
          node->getOrientation().ToAngleAxis(orient_angle, orient_axis);
          output << "\t" << orient_axis.x << " " << orient_axis.y << " " << orient_axis.z << " " // orientation axis
                 << orient_angle.valueDegrees() << "\n"; // orientation angle
          output << "\t" << ent->getSubEntity(0)->getMaterialName() << "\n"
                 << "\t" << ent->getSubEntity(1)->getMaterialName() << "\n"
                 << "\t" << ent->getSubEntity(2)->getMaterialName() << "\n"
                 << "\t" << ent->getSubEntity(3)->getMaterialName() << "\n"
                 << "\t" << ent->getSubEntity(4)->getMaterialName() << "\n"
                 << "\t" << ent->getSubEntity(5)->getMaterialName() << "\n";
          // Write pivot info
          output << "\t" << e->isPivot << "\n";
          if (e->isPivot)
          {
            // pivot indexes
            output << "\t" << e->pivotArrays[0] << " " << e->pivotArrayIndexes[0] << "\n"
                   << "\t" << e->pivotArrays[1] << " " << e->pivotArrayIndexes[1] << "\n";
          }
          // flush
          output << std::endl;
        }

    output.close();
  }
}



bool Cube::load(Ogre::String file)
{
  if (rndCounter == 0)
  {
    // Open file. If failed - return false
    std::ifstream input(file.c_str());
    if (!input.is_open())
      return false;

    // Clear first
    clear();

    // Read size
    input >> size;
    // Calculate visual parameters
    elemVisualSize = (visualSize - (size - 1) * elemSpacing) / (float)size;

    // Prepare pivot arrays
    for (int i = 0; i < 4; ++i)
      pivots[i].resize(size);

    data.resize(size);
    for (int i = 0; i < size; ++i)
    {
      data[i].resize(size);
      for (int j = 0; j < size; ++j)
      {
        data[i][j].resize(size);
        for (int k = 0; k < size; ++k)
        {
          // Read name
          Ogre::String name("");
          input >> name;

          // Create node and position it
          Ogre::SceneNode *node = cubeRootNode->createChildSceneNode(name);
          // Read indexes
          int i_pos, j_pos, k_pos;
          input >> i_pos >> j_pos >> k_pos;
          // Read position
          Ogre::Vector3 pos = Ogre::Vector3::ZERO;
          input >> pos.x >> pos.y >> pos.z;
          // Read orientation
          Ogre::Vector3 orient_axis = Ogre::Vector3::ZERO;
          float orient_angle = 0;
          input >> orient_axis.x >> orient_axis.y >> orient_axis.z >> orient_angle;
          Ogre::Quaternion orient(Ogre::Radian(Ogre::Degree(orient_angle)), orient_axis);
          node->setPosition(pos);
          node->setOrientation(orient);
          node->setScale(Ogre::Vector3(elemVisualSize));

          // Create entity and setup it
          Ogre::Entity *ent = smgr->createEntity(name, modelMesh);
          Ogre::String mat("");
          for (int s = 0; s < 6; ++s)
          {
            input >> mat;
            ent->getSubEntity(s)->setMaterialName(mat);
          }
          node->attachObject(ent);

          // Finally, create element
          CubeElement *e = new CubeElement(node, i_pos, j_pos, k_pos);
          input >> e->isPivot;
          if (e->isPivot)
          {
            input >> e->pivotArrays[0] >> e->pivotArrayIndexes[0] >> e->pivotArrays[1] >> e->pivotArrayIndexes[1];
            pivots[e->pivotArrays[0]][e->pivotArrayIndexes[0]] = e;
            if (e->pivotArrays[1] != -1)
              pivots[e->pivotArrays[1]][e->pivotArrayIndexes[1]] = e;
          }
          data[i_pos][j_pos][k_pos] = e;
        }
      }
    }

    cursor = new CubeCursor(this);

    input.close();
  }
  return true;
}



void Cube::onKeyboardEvent(const SDL_KeyboardEvent &evt)
{
  if (evt.keysym.sym == SDLK_LSHIFT)
  {
    viewRotation = !viewRotation;
    return;
  }
  if (evt.type == SDL_KEYDOWN)
  {
    if (!viewRotation && !cursor->rotActive)
    {
      switch (evt.keysym.sym)
      {
      //
      // Cursor movement
      case SDLK_UP: case SDLK_DOWN:
      {
        int dir = evt.keysym.sym == SDLK_UP ? 1 : -1;
        if (cursor->getAxis() == Ogre::Vector3::UNIT_X)
        {
          Ogre::Vector3 x = cubeRootNode->getLocalAxes().GetColumn(0);
          x.z >= 0.0 ? cursor->slide(-dir) : cursor->slide(dir);
        }
        else if (cursor->getAxis() == Ogre::Vector3::UNIT_Y)
        {
          Ogre::Vector3 y = cubeRootNode->getLocalAxes().GetColumn(1);
          y.z >= 0.7 ? cursor->slide(-dir) : cursor->slide(dir);
        }
        else if (cursor->getAxis() == Ogre::Vector3::UNIT_Z)
        {
          Ogre::Vector3 z = cubeRootNode->getLocalAxes().GetColumn(2);
          z.z >= 0 ? cursor->slide(-dir) : cursor->slide(dir);
        }
        break;
      }
      case SDLK_LEFT: case SDLK_RIGHT:
        cursor->setAxis(evt.keysym.sym == SDLK_RIGHT);
        break;
      case SDLK_LCTRL: case SDLK_LALT:
      {
        int dir = evt.keysym.sym == SDLK_LCTRL ? 1 : -1;
        if (cursor->getAxis() == Ogre::Vector3::UNIT_X)
        {
          Ogre::Vector3 x = cubeRootNode->getLocalAxes().GetColumn(0);
          dir *= (x.z >= 0.0) ? 1 : -1;
        }
        else if (cursor->getAxis() == Ogre::Vector3::UNIT_Y)
        {
          Ogre::Vector3 y = cubeRootNode->getLocalAxes().GetColumn(1);
          dir *= (y.z >= 0.0) ? 1 : -1;
        }
        else if (cursor->getAxis() == Ogre::Vector3::UNIT_Z)
        {
          Ogre::Vector3 z = cubeRootNode->getLocalAxes().GetColumn(2);
          dir *= (z.z >= 0) ? 1 : -1;
        }
        cursor->rotate(static_cast<Direction>(dir), 200.0, true);
        content->getGUIManager()->showWinTip(false);
      }
      default:; // shut up warnings about unused constants
      }
    }
  }
}



bool Cube::frameStarted(const Ogre::FrameEvent &evt)
{
  // View rotation mode
  if (viewRotation)
  {
    Ogre::Radian angle = Ogre::Radian(Ogre::Degree(viewRotSpeed)) * evt.timeSinceLastFrame;
    if (Game::singletonPtr()->getInputManager()->isKeyDown(SDLK_RIGHT))
        cubeRootNode->rotate(Ogre::Vector3::UNIT_Y, angle, Ogre::SceneNode::TS_LOCAL);
    if (Game::singletonPtr()->getInputManager()->isKeyDown(SDLK_LEFT))
        cubeRootNode->rotate(Ogre::Vector3::UNIT_Y, -angle, Ogre::SceneNode::TS_LOCAL);
    if (Game::singletonPtr()->getInputManager()->isKeyDown(SDLK_UP))
    {
      if (Ogre::Math::Abs((rotConstraint - angle).valueDegrees()) <= 90.0)
      {
        cubeRootNode->rotate(Ogre::Vector3::UNIT_X, -angle, Ogre::SceneNode::TS_PARENT);
        rotConstraint -= angle;
      }
    }
    if (Game::singletonPtr()->getInputManager()->isKeyDown(SDLK_DOWN))
    {
      if (Ogre::Math::Abs((rotConstraint + angle).valueDegrees()) <= 90.0)
      {
        cubeRootNode->rotate(Ogre::Vector3::UNIT_X, angle, Ogre::SceneNode::TS_PARENT);
        rotConstraint += angle;
      }
    }
  }
  if (!cursor->rotActive && rndCounter > 0)
  {
    static const Ogre::Vector3 axis_arr[] = { Ogre::Vector3::UNIT_X, Ogre::Vector3::UNIT_Y, Ogre::Vector3::UNIT_Z };
    static const Direction dir_arr[] = { DIR_CW, DIR_CCW };
    rndCounter--;
    cursor->setPosition(axis_arr[rand() % 3], rand() % size);
    cursor->rotate(dir_arr[rand() % 2], 800.0, false);
  }
  cursor->animateRotation(evt.timeSinceLastFrame);
  return true;
}

