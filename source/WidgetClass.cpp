// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/WidgetClass.hpp"
#include "VACA/Widget.hpp"
#include "VACA/Debug.hpp"

using namespace vaca;

const WidgetClassName WidgetClassName::None;

// ======================================================================
// WidgetClassName

WidgetClassName::WidgetClassName()
  : m_className(L"")
{
}

WidgetClassName::WidgetClassName(const String& className)
  : m_className(className)
{
}

WidgetClassName::WidgetClassName(const WidgetClassName& className) = default;

WidgetClassName::~WidgetClassName()
= default;

WidgetClassName& WidgetClassName::operator=(const WidgetClassName& className)
= default;

bool WidgetClassName::operator==(const WidgetClassName& className) const
{
  return m_className == className.m_className;
}

bool WidgetClassName::operator!=(const WidgetClassName& className) const
{
  return m_className != className.m_className;
}

// ======================================================================
// WidgetClass

/**
   Returns the class name.

   @win32
     The class name is used in the lpszClassName field of
     the @msdn{WNDCLASSEX} structure inside the @link Register#Register Register constructor@endlink.
   @endwin32
*/
WidgetClassName WidgetClass::getClassName()
{
  return WidgetClassName(L"Vaca.Widget");
}

/**
   Returns the set of CS_ Win32 flags.

   All Widgets by default received double-clicks, so it returns
   CS_DBLCLKS.
*/
int WidgetClass::getStyle()
{
  return CS_DBLCLKS;
}

int WidgetClass::getColor()
{
  return COLOR_3DFACE;
}

int WidgetClass::getWndExtra()
{
  return 0;
}

WNDPROC WidgetClass::getWndProc()
{
  return Widget::getGlobalWndProc();
}
