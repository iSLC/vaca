// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/SetCursorEvent.hpp"
#include "VACA/Widget.hpp"
#include "VACA/Cursor.hpp"

using namespace Wg;

SetCursorEvent::SetCursorEvent(Widget* source, const Point& point, WidgetHit hit)
  : MouseEvent(source, point, 0, 0, MouseButton::None)
  , m_hit(hit)
{
}

SetCursorEvent::~SetCursorEvent()
= default;

void SetCursorEvent::setCursor(const Cursor& cursor)
{
  ::SetCursor(const_cast<Cursor*>(&cursor)->getHandle());
  consume();
}

WidgetHit SetCursorEvent::getWidgetHit() const
{
  return m_hit;
}
