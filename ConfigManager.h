
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

#ifndef _CONFIG_MANAGER_H_
#define _CONFIG_MANAGER_H_

#include <Ogre.h>


class ConfigManager
{
private:
  Ogre::ConfigFile cf;
public:
  ConfigManager() {}
  ~ConfigManager() {}

  /// Read configuration from the given file
  void read(const char *file);

  /// Returns canvas width
  int getCanvasWidth() const;
  /// Returns canvas height
  int getCanvasHeight() const;
  /// Returns bits per pixel
  int getBits() const;
  /// Returns true if fullscreen mode should be set
  bool getFullscreen() const;

  /// Returns fullscreen mode flag as a string
  Ogre::String getFullscreenStr() const;
  /// Returns video mode string in format "width x height bpp-bits colour" suitable for Ogre
  Ogre::String getVideoModeStr() const;

};



#endif
