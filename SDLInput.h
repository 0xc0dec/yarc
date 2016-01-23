
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

#ifndef _SDL_INPUT_H_
#define _SDL_INPUT_H_

#include <list>
#include <SDL.h>

/**
 * Keyboard events listener base class
 */
class KeyboardListener
{
public:
  virtual ~KeyboardListener() {}
  virtual void onKeyboardEvent(const SDL_KeyboardEvent &evt) = 0;
};


/**
 * Mouse events listener base class
 */
class MouseListener
{
public:
  virtual ~MouseListener() {}
  virtual void onMouseMotion(const SDL_MouseMotionEvent &evt) = 0;
  virtual void onMouseBtn(const SDL_MouseButtonEvent &evt) = 0;
};


/**
 * Generic event listener. Cab
 */
class EventListener
{
public:
  virtual ~EventListener() {}
  virtual void onEvent(const SDL_Event &evt) = 0;
};



/**
 * SDL input manager. Dispatches events to registered listeners
 */
class InputManager
{
private:
  /// Subscribed keyboard listeners list
  typedef std::list<KeyboardListener*> KeyboardListenersList;
  /// Subscribed mouse listeners list
  typedef std::list<MouseListener*> MouseListenersList;
  /// Subscribed general type events listeners list
  typedef std::list<EventListener*> OtherEventsListenersList;

private:
  /// Subscribed keyboard listeners
  KeyboardListenersList kb_listeners;
  /// Subscribed mouse listeners
  MouseListenersList mouse_listeners;
  /// Subscribed general type events listeners
  OtherEventsListenersList other_listeners;

public:
  InputManager() {}
  ~InputManager() {}

  /// Captures input from devices. Should be used to update information about input events
  void capture();

  /// Returns current state of the given key
  bool isKeyDown(const SDLKey key) const;

  /// Registers new keyboard events listener
  void regKeyboardListener(KeyboardListener *l);
  /// Unregisters keyboard listener
  void unregKeyboardListener(KeyboardListener *l);

  /// Registers new mouse events listener
  void regMouseListener(MouseListener *l);
  /// Unregisters mouse listener
  void unregMouseListener(MouseListener *l);

  /// Registers other general events listener
  void regEventListener(EventListener *l);
  /// Unregisters general event listener
  void unregEventListener(EventListener *l);
};



#endif
