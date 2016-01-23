
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

#include "ConfigManager.h"


void ConfigManager::read(const char *file)
{
  cf.clear();
  cf.load(file);
}


int ConfigManager::getCanvasWidth() const
{
  return Ogre::StringConverter::parseInt(cf.getSetting("CanvasWidth", "Settings", "640"));
}


int ConfigManager::getCanvasHeight() const
{
  return Ogre::StringConverter::parseInt(cf.getSetting("CanvasHeight", "Settings", "480"));
}


int ConfigManager::getBits() const
{
  return Ogre::StringConverter::parseInt(cf.getSetting("Bits", "Settings", "16"));
}


bool ConfigManager::getFullscreen() const
{
  return Ogre::StringConverter::parseBool(cf.getSetting("Fullscreen", "Settings", "false"));
}


Ogre::String ConfigManager::getFullscreenStr() const
{
  Ogre::String str = cf.getSetting("Fullscreen", "Settings", "false");
  if (str.compare("false") == 0)
    return Ogre::String("No");
  else if (str.compare("true") == 0)
    return Ogre::String("Yes");
  return Ogre::String("No");
}


Ogre::String ConfigManager::getVideoModeStr() const
{
  Ogre::String m = cf.getSetting("CanvasWidth", "Settings", "640").append(" x ")
      .append(cf.getSetting("CanvasHeight", "Settings", "480")).append(" ")
      .append(cf.getSetting("Bits", "Settings", "16")).append("-bit colour");
  return m;
}
