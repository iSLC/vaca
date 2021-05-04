// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/FocusEvent.hpp"
#include "VACA/Widget.hpp"

using namespace Wg;

FocusEvent::FocusEvent(Widget* source, Widget* oldFocus, Widget* newFocus)
  : Event(source)
  , m_oldFocus(oldFocus)
  , m_newFocus(newFocus)
{
}

FocusEvent::~FocusEvent()
= default;

Widget* FocusEvent::getOldFocus() const
{
  return m_oldFocus;
}

Widget* FocusEvent::getNewFocus() const
{
  return m_newFocus;
}

