// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#ifndef VACA_MENUITEMEVENT_HPP
#define VACA_MENUITEMEVENT_HPP

#include "VACA/Base.hpp"
#include "VACA/Event.hpp"

namespace vaca {

class VACA_DLL MenuItemEvent : public Event
{

public:

  MenuItemEvent(MenuItem* source);
  virtual ~MenuItemEvent();

  MenuItem* getMenuItem();

};

} // namespace vaca

#endif // VACA_MENUITEMEVENT_HPP
