// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/Layout.hpp"
#include "VACA/Debug.hpp"
#include "VACA/Widget.hpp"

using namespace Wg;

Layout::Layout()
= default;

Layout::~Layout()
= default;

Size Layout::getPreferredSize(Widget* parent, WidgetList& widgets, const Size& fitIn)
{
  return Size(0, 0);
}

//////////////////////////////////////////////////////////////////////
// WidgetsMovement

#include "win32/WidgetsMovementImpl.hpp"

WidgetsMovement::WidgetsMovement(const WidgetList& widgets)
  : m_impl(new WidgetsMovementImpl(widgets))
{
}

WidgetsMovement::~WidgetsMovement()
{
  delete m_impl;
}

void WidgetsMovement::moveWidget(Widget* widget, const Rect& rc)
{
  m_impl->moveWidget(widget, rc);
}
