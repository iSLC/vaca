// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#ifndef VACA_RESIZEEVENT_HPP
#define VACA_RESIZEEVENT_HPP

#include "VACA/Base.hpp"
#include "VACA/Event.hpp"
#include "VACA/Size.hpp"

namespace vaca {

class VACA_DLL ResizeEvent : public Event
{
  Size m_size;

public:

  ResizeEvent(Component* source, const Size& size);
  virtual ~ResizeEvent();

  Size getSize() const;

};

} // namespace vaca

#endif // VACA_RESIZEEVENT_HPP
