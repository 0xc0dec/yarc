
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

#include "Game.h"
#include "Content.h"



Content::Content()
{
  // Create scene manager
  smgr = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, "smgr_Main");
  // Create gui manager
  gui_mgr = new GUIManager(this);
  // Create scene
  createScene();
}



Content::~Content()
{
  // All game resources will be removed during Ogre shutdown
  // ...
  delete cube;
}



void Content::createScene()
{
  // Create camera
  Ogre::Camera *camera = smgr->createCamera("cam_Main");
  camera->setPosition(0.0, 0.0, 300.0);
  camera->lookAt(Ogre::Vector3::ZERO);
  camera->setNearClipDistance(1.0);
  camera->setFarClipDistance(10000.0);
  camera->setFOVy(Ogre::Radian(Ogre::Degree(45.0)));
  camera->setAutoAspectRatio(true);
  Game *game = Game::singletonPtr();
  game->getWindow()->addViewport(camera);
  game->getWindow()->getViewport(0)->setBackgroundColour(Ogre::ColourValue(0.1, 0.2, 0.3));

  _createMeshes();

  // Create cube
  cube = new Cube(this, "SimpleCube", "Default", 3, 110.0, 0.5, 150.0, 200.0);
}



void Content::_createMeshes() const
{
  /*
   * Default element mesh
   */
  Ogre::ManualObject *obj = smgr->createManualObject("SimpleCubeManual");
  // x min - 0 submesh
  obj->begin("Blank", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    obj->position(-0.5, -0.5, -0.5);
    obj->position(-0.5, -0.5, 0.5);
    obj->position(-0.5, 0.5, 0.5);
    obj->position(-0.5, 0.5, -0.5);
    obj->normal(-1.0, 0.0, 0.0);
    obj->normal(-1.0, 0.0, 0.0);
    obj->normal(-1.0, 0.0, 0.0);
    obj->normal(-1.0, 0.0, 0.0);
    obj->index(0);
    obj->index(1);
    obj->index(2);
    obj->index(3);
  obj->end();
  // x max - 1 submesh
  obj->begin("Blank", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    obj->position(0.5, -0.5, -0.5);
    obj->position(0.5, -0.5, 0.5);
    obj->position(0.5, 0.5, 0.5);
    obj->position(0.5, 0.5, -0.5);
    obj->normal(1.0, 0.0, 0.0);
    obj->normal(1.0, 0.0, 0.0);
    obj->normal(1.0, 0.0, 0.0);
    obj->normal(1.0, 0.0, 0.0);
    obj->index(0);
    obj->index(3);
    obj->index(2);
    obj->index(1);
  obj->end();
  // y min - 2 submesh
  obj->begin("Blank", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    obj->position(-0.5, -0.5, -0.5);
    obj->position(-0.5, -0.5, 0.5);
    obj->position(0.5, -0.5, 0.5);
    obj->position(0.5, -0.5, -0.5);
    obj->normal(0.0, -1.0, 0.0);
    obj->normal(0.0, -1.0, 0.0);
    obj->normal(0.0, -1.0, 0.0);
    obj->normal(0.0, -1.0, 0.0);
    obj->index(0);
    obj->index(3);
    obj->index(2);
    obj->index(1);
  obj->end();
  // y max - 3 submesh
  obj->begin("Blank", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    obj->position(-0.5, 0.5, -0.5);
    obj->position(-0.5, 0.5, 0.5);
    obj->position(0.5, 0.5, 0.5);
    obj->position(0.5, 0.5, -0.5);
    obj->normal(0.0, 1.0, 0.0);
    obj->normal(0.0, 1.0, 0.0);
    obj->normal(0.0, 1.0, 0.0);
    obj->normal(0.0, 1.0, 0.0);
    obj->index(0);
    obj->index(1);
    obj->index(2);
    obj->index(3);
  obj->end();
  // z min - 4 submesh
  obj->begin("Blank", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    obj->position(-0.5, -0.5, -0.5);
    obj->position(-0.5, 0.5, -0.5);
    obj->position(0.5, 0.5, -0.5);
    obj->position(0.5, -0.5, -0.5);
    obj->normal(0.0, 0.0, -1.0);
    obj->normal(0.0, 0.0, -1.0);
    obj->normal(0.0, 0.0, -1.0);
    obj->normal(0.0, 0.0, -1.0);
    obj->index(0);
    obj->index(1);
    obj->index(2);
    obj->index(3);
  obj->end();
  // z max - 5 submesh
  obj->begin("Blank", Ogre::RenderOperation::OT_TRIANGLE_FAN);
    obj->position(-0.5, -0.5, 0.5);
    obj->position(-0.5, 0.5, 0.5);
    obj->position(0.5, 0.5, 0.5);
    obj->position(0.5, -0.5, 0.5);
    obj->normal(0.0, 0.0, 1.0);
    obj->normal(0.0, 0.0, 1.0);
    obj->normal(0.0, 0.0, 1.0);
    obj->normal(0.0, 0.0, 1.0);
    obj->index(0);
    obj->index(3);
    obj->index(2);
    obj->index(1);
  obj->end();
  obj->convertToMesh("SimpleCube", "Models");
  smgr->destroyManualObject(obj);

  /*
   * Cursor mesh
   */
  obj = smgr->createManualObject("Cursor");
  // 0
  obj->begin("Blank", Ogre::RenderOperation::OT_LINE_STRIP);
    obj->position(-0.5, -0.5, -0.5);
    obj->position(-0.5, -0.5, 0.5);
    obj->position(-0.5, 0.5, 0.5);
    obj->position(-0.5, 0.5, -0.5);
    obj->position(-0.5, -0.5, -0.5);
  obj->end();
  // 1
  obj->begin("Blank", Ogre::RenderOperation::OT_LINE_STRIP);
    obj->position(0.5, -0.5, -0.5);
    obj->position(0.5, -0.5, 0.5);
    obj->position(0.5, 0.5, 0.5);
    obj->position(0.5, 0.5, -0.5);
    obj->position(0.5, -0.5, -0.5);
  obj->end();
  // 2
  obj->begin("Blank", Ogre::RenderOperation::OT_LINE_LIST);
    obj->position(-0.5, -0.5, -0.5);
    obj->position(0.5, -0.5, -0.5);
    obj->position(-0.5, -0.5, 0.5);
    obj->position(0.5, -0.5, 0.5);
    obj->position(-0.5, 0.5, 0.5);
    obj->position(0.5, 0.5, 0.5);
    obj->position(-0.5, 0.5, -0.5);
    obj->position(0.5, 0.5, -0.5);
  obj->end();
}

