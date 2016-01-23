
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

#include <OgreOverlay.h>
#include <OgreOverlayContainer.h>
#include <OgreTextAreaOverlayElement.h>

#include "Game.h"
#include "Content.h"
#include "GUIManager.h"


GUIManager::GUIManager(Content *_content): content(_content), _menuVisible(true) // toggle later in constructor
{
  Game::singletonPtr()->getInputManager()->regKeyboardListener(this);

  ovl_mgr = Ogre::OverlayManager::getSingletonPtr();
  overlay = ovl_mgr->getByName("RootOverlay");
  overlay->show();

  showWinTip(false);
  _toggleMenu();
}



GUIManager::~GUIManager()
{
}



void GUIManager::showWinTip(bool show)
{
  if (show)
    overlay->getChild("RootPanel")->getChild("WinMessageTip")->show();
  else
    overlay->getChild("RootPanel")->getChild("WinMessageTip")->hide();
}



void GUIManager::_toggleMenu()
{
  Ogre::OverlayContainer *panel = overlay->getChild("RootPanel");
  if (_menuVisible)
  {
    panel->getChild("MenuTip")->setCaption("(M) Show menu");
    panel->getChild("ExitTip")->hide();
    panel->getChild("RandomizeTip")->hide();
    panel->getChild("ResetTip")->hide();
    panel->getChild("DecSizeTip")->hide();
    panel->getChild("IncSizeTip")->hide();
    panel->getChild("SaveTip")->hide();
    panel->getChild("LoadTip")->hide();
  }
  else
  {
    panel->getChild("MenuTip")->setCaption("(M) Hide menu");
    panel->getChild("ExitTip")->show();
    panel->getChild("RandomizeTip")->show();
    panel->getChild("ResetTip")->show();
    panel->getChild("DecSizeTip")->show();
    panel->getChild("IncSizeTip")->show();
    panel->getChild("SaveTip")->show();
    panel->getChild("LoadTip")->show();
  }
  _menuVisible = !_menuVisible;
}



void GUIManager::onKeyboardEvent(const SDL_KeyboardEvent &evt)
{
  if (evt.type == SDL_KEYDOWN)
  {
    switch (evt.keysym.sym)
    {
    case SDLK_s:
      content->getCube()->save("save.dat");
      break;
    case SDLK_l:
      content->getCube()->load("save.dat");
      break;
    case SDLK_m:
      _toggleMenu();
      break;
    case SDLK_r:
      content->getCube()->randomize();
      showWinTip(false);
      break;
    case SDLK_t:
      content->getCube()->reset(content->getCube()->getSize());
      showWinTip(false);
      break;
    case SDLK_1:
      content->getCube()->reset(content->getCube()->getSize() - 1);
      showWinTip(false);
      break;
    case SDLK_2:
      content->getCube()->reset(content->getCube()->getSize() + 1);
      showWinTip(false);
    default:;
    }
  }
}


