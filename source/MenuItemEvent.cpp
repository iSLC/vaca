// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/MenuItemEvent.hpp"
#include "Wg/Menu.hpp"

using namespace Wg;

MenuItemEvent::MenuItemEvent(MenuItem* source)
  : Event(source)
{
}

MenuItemEvent::~MenuItemEvent()
= default;

MenuItem* MenuItemEvent::getMenuItem()
{
  return dynamic_cast<MenuItem*>(getSource());
}
