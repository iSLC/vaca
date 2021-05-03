// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/ClientLayout.hpp"
#include "VACA/Widget.hpp"

using namespace vaca;

ClientLayout::ClientLayout()
{
  m_border = 0;
}

ClientLayout::ClientLayout(int border)
{
  m_border = border;
}

ClientLayout::~ClientLayout()
= default;

Size ClientLayout::getPreferredSize(Widget* parent, WidgetList& widgets, const Size& fitIn)
{
  Size sz(0, 0);

  for (auto widget : widgets) {
    if (!widget->isLayoutFree()) {
      Size pref = widget->getPreferredSize(fitIn);
      if (sz.w < pref.w) sz.w = pref.w;
      if (sz.h < pref.h) sz.h = pref.h;
    }
  }

  return sz + m_border;
}

void ClientLayout::layout(Widget* parent, WidgetList& widgets, const Rect& rc)
{
  Rect bounds = rc;
  bounds.shrink(m_border);
  WidgetsMovement movement(widgets);

  for (auto widget : widgets) {
    if (!widget->isLayoutFree())
      movement.moveWidget(widget, bounds);
  }
}
