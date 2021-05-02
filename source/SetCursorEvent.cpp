// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/SetCursorEvent.h"
#include "VACA/Widget.h"
#include "VACA/Cursor.h"

using namespace vaca;

SetCursorEvent::SetCursorEvent(Widget* source, Point point, WidgetHit hit)
  : MouseEvent(source, point, 0, 0, MouseButton::None)
  , m_hit(hit)
{
}

SetCursorEvent::~SetCursorEvent()
{
}

void SetCursorEvent::setCursor(const Cursor& cursor)
{
  ::SetCursor(const_cast<Cursor*>(&cursor)->getHandle());
  consume();
}

WidgetHit SetCursorEvent::getWidgetHit() const
{
  return m_hit;
}
