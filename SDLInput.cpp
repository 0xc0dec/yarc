
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

#include <algorithm>

#include "SDLInput.h"


void InputManager::capture()
{
  SDL_Event evt;
  while (SDL_PollEvent(&evt))
  {
    switch (evt.type)
    {
    case SDL_KEYUP: case SDL_KEYDOWN:
    {
      KeyboardListenersList::const_iterator kb_it = kb_listeners.begin(), kb_end = kb_listeners.end();
      for (; kb_it != kb_end; ++kb_it)
        (*kb_it)->onKeyboardEvent(evt.key);
      break;
    }
    case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP: case SDL_MOUSEMOTION:
    {
      MouseListenersList::const_iterator m_it = mouse_listeners.begin(), m_end = mouse_listeners.end();
      for (; m_it != m_end; ++m_it)
      {
        if (evt.type == SDL_MOUSEBUTTONDOWN || evt.type == SDL_MOUSEBUTTONUP)
          (*m_it)->onMouseBtn(evt.button);
        if (evt.type == SDL_MOUSEMOTION)
          (*m_it)->onMouseMotion(evt.motion);
      }
      break;
    }
    default:
      OtherEventsListenersList::const_iterator other_it = other_listeners.begin(), other_end = other_listeners.end();
      for (; other_it != other_end; ++other_it)
        (*other_it)->onEvent(evt);
    }
  }
}



bool InputManager::isKeyDown(const SDLKey key) const
{
  return SDL_GetKeyState(NULL)[key] == 1;
}



void InputManager::regKeyboardListener(KeyboardListener *l)
{
  if (l && std::find(kb_listeners.begin(), kb_listeners.end(), l) == kb_listeners.end())
    kb_listeners.push_back(l);
}



void InputManager::unregKeyboardListener(KeyboardListener *l)
{
  if (l)
    kb_listeners.remove(l);
}



void InputManager::regMouseListener(MouseListener *l)
{
  if (l && std::find(mouse_listeners.begin(), mouse_listeners.end(), l) == mouse_listeners.end())
    mouse_listeners.push_back(l);
}



void InputManager::unregMouseListener(MouseListener *l)
{
  if (l)
    mouse_listeners.remove(l);
}



void InputManager::regEventListener(EventListener *l)
{
  if (l && std::find(other_listeners.begin(), other_listeners.end(), l) == other_listeners.end())
    other_listeners.push_back(l);
}



void InputManager::unregEventListener(EventListener *l)
{
  if (l)
    other_listeners.remove(l);
}
