
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
#include <SDL_syswm.h>
#include <SDL.h>
#include <windows.h>
#else
#include <iostream>
#endif

#include "Game.h"
#include "Content.h"




bool Game::init()
{
  try
  {
    // Read config first
    config.read("./config.cfg");

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_putenv((char*)"SDL_VIDEO_CENTERED=true");
    int flags = SDL_OPENGL;
    if (config.getFullscreen())
      flags |= SDL_FULLSCREEN;
    SDL_SetVideoMode(config.getCanvasWidth(), config.getCanvasHeight(), config.getBits(), flags);
    SDL_WM_SetCaption("Yet Another Rubik's Cube", NULL);

    // Ogre
    root = new Ogre::Root();
    #ifdef _WINDOWS
      root->loadPlugin("./RenderSystem_GL.dll");
    #else
      root->loadPlugin("./RenderSystem_GL.so");
    #endif
    Ogre::RenderSystem *rs = root->getRenderSystemByName("OpenGL Rendering Subsystem");
    root->setRenderSystem(rs);
    rs->setConfigOption("Full Screen", config.getFullscreenStr());
    rs->setConfigOption("Video Mode", config.getVideoModeStr());
    root->initialise(false);

    // Window
    Ogre::NameValuePairList params;
    #ifdef _WINDOWS
      SDL_SysWMinfo info;
      SDL_VERSION(&info.version);
      SDL_GetWMInfo(&info);
      params["externalWindowHandle"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.window));
      params["externalGLContent"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(info.hglrc));
      params["externalGLControl"] = Ogre::String("True");
    #else
      params["currentGLContext"] = Ogre::String("True");
    #endif
    wnd = root->createRenderWindow("Main window", config.getCanvasWidth(), config.getCanvasHeight(),
        config.getFullscreen(), &params);
    wnd->setVisible(true);

    // Input
    imgr = new InputManager();

    // Hard-code res location
    Ogre::ResourceGroupManager *rgm = Ogre::ResourceGroupManager::getSingletonPtr();
    rgm->addResourceLocation("res.gdat", "Zip", "General");
    rgm->initialiseAllResourceGroups();

    // Window and rendering events listener
    root->addFrameListener(this);
    imgr->regEventListener(this);
    imgr->regKeyboardListener(this);
  }
  catch (std::exception &e)
  {
    #ifdef _WINDOWS
      MessageBoxA(NULL, e.what(), "Exception occured", MB_OK | MB_ICONERROR);
    #else
      std::cout << e.what();
    #endif
    return false;
  }
  return true;
}




void Game::deInit()
{
  delete imgr;
  delete root;
  SDL_Quit();
}




void Game::run()
{
  // All runtime logic here

  // Create content
  Content *content = new Content();

  // MAIN LOOP
  while (!stop)
  {
    imgr->capture();
    root->renderOneFrame();
    // Doesn't work properly under linux. Seems like OGRE swaps buffers itself
    #ifdef _WINDOWS
      SDL_GL_SwapBuffers();
    #endif
  }

  // Remove content
  delete content;
}



void Game::onEvent(const SDL_Event &evt)
{
  if (evt.type == SDL_QUIT)
    stop = true;
}



void Game::onKeyboardEvent(const SDL_KeyboardEvent &evt)
{
  if (evt.keysym.sym == SDLK_ESCAPE)
    stop = true;
}
