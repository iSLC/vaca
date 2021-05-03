// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Event.hpp"

namespace vaca {

class VACA_DLL MenuItemEvent : public Event
{

public:

  MenuItemEvent(MenuItem* source);
  ~MenuItemEvent() override;

  MenuItem* getMenuItem();

};

} // namespace vaca