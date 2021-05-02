// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#ifndef VACA_MENUITEMEVENT_H
#define VACA_MENUITEMEVENT_H

#include "VACA/Base.h"
#include "VACA/Event.h"

namespace vaca {

class VACA_DLL MenuItemEvent : public Event
{

public:

  MenuItemEvent(MenuItem* source);
  virtual ~MenuItemEvent();

  MenuItem* getMenuItem();

};

} // namespace vaca

#endif // VACA_MENUITEMEVENT_H
